#pragma once

#include <git2.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

namespace gitxx {

/// @brief A test fixture helper that creates and manipulates a real git
///        repository using libgit2 directly.
class GitCommands {
 public:
  /// @brief Construct with an explicit path.
  ///        The directory is created if absent.
  explicit GitCommands(std::string_view path) : m_path(path) {
    std::filesystem::create_directories(m_path);
    m_repo = openOrInit(m_path);
  }

  /// @brief Construct with a path derived from the calling test's name via
  ///        source_location – one unique directory per test, cleaned up on
  ///        entry.
  explicit GitCommands(
      const std::source_location location = std::source_location::current())
      : m_path(TEST_OUTPUT_DIR "/gitxx_test2") {
    git_libgit2_init();

    const std::string func_name = location.function_name();
    const auto first = func_name.rfind(' ') + 1;
    const auto end = func_name.rfind("_Test");
    const auto len = func_name.size() - first - (func_name.size() - end);
    m_path.append(func_name.substr(first, len));

    std::filesystem::remove_all(m_path);
    std::filesystem::create_directories(m_path);
    m_repo = openOrInit(m_path);
  }

  ~GitCommands() {
    std::filesystem::remove_all(m_path);
    git_libgit2_shutdown();
  }
  GitCommands(const GitCommands&) = delete;
  GitCommands& operator=(const GitCommands&) = delete;
  GitCommands(GitCommands&&) = default;
  GitCommands& operator=(GitCommands&&) = default;

  [[nodiscard]] std::filesystem::path path() const { return m_path; }

  /// @brief Stage a file by path (relative to the repo root).
  ::testing::AssertionResult add(const std::filesystem::path& filename) {
    auto index = getIndex();
    if (!index) {
      return fail("git_repository_index");
    }

    if (git_index_add_bypath(index.get(), filename.c_str()) != 0) {
      return fail("git_index_add_bypath");
    }

    if (git_index_write(index.get()) != 0) {
      return fail("git_index_write");
    }

    return ::testing::AssertionSuccess();
  }

  /// @brief Create a commit from the current index state.
  ::testing::AssertionResult commit(std::string_view message) {
    return createCommit(message);
  }

  /// @brief Create a commit even when nothing has changed.
  ::testing::AssertionResult makeEmptyCommit(std::string_view message) {
    return createCommit(message);
  }

  /// @brief Create a new file (or overwrite) with the given content.
  ::testing::AssertionResult createFile(const std::filesystem::path& filename,
                                        std::string_view content) {
    std::ofstream out{m_path / filename};
    out << content;
    out.close();

    if (std::filesystem::exists(m_path / filename)) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure() << filename << " was not created";
  }

  /// @brief Overwrite an existing file
  ::testing::AssertionResult modifyFile(const std::filesystem::path& filename,
                                        std::string_view content) {
    return createFile(filename, content);
  }

  /// @brief Delete a file from the working directory (does not stage the
  ///        removal).
  ::testing::AssertionResult deleteFile(const std::filesystem::path& filename) {
    std::error_code error{};
    if (std::filesystem::remove(m_path / filename, error)) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure()
           << "Tried deleting: " << (m_path / filename)
           << " Got error: " << error.message();
  }

  /// @brief Create a directory inside the repo.
  ::testing::AssertionResult createDir(const std::filesystem::path& name) {
    std::error_code error{};
    if (std::filesystem::create_directory(m_path / name, error)) {
      return ::testing::AssertionSuccess();
    }

    return ::testing::AssertionFailure()
           << "Tried creating dir: " << (m_path / name)
           << " Got error: " << error;
  }

  /// @brief Rename (and stage the rename: remove old path, add new path).
  ::testing::AssertionResult renameFile(const std::filesystem::path& oldName,
                                        const std::filesystem::path& newName) {
    std::error_code error{};
    std::filesystem::rename(m_path / oldName, m_path / newName, error);
    if (error) {
      return ::testing::AssertionFailure()
             << "Failed to rename " << oldName << " to " << newName << ": "
             << error.message();
    }

    auto index = getIndex();
    if (!index) {
      return fail("git_repository_index");
    }

    if (git_index_remove_bypath(index.get(), oldName.c_str()) != 0) {
      return fail("git_index_remove_bypath");
    }

    if (git_index_add_bypath(index.get(), newName.c_str()) != 0) {
      return fail("git_index_add_bypath");
    }

    if (git_index_write(index.get()) != 0) {
      return fail("git_index_write");
    }

    return ::testing::AssertionSuccess();
  }

  /// @brief Create a local branch at HEAD and check it out.
  ::testing::AssertionResult createBranchAndCheckout(
      std::string_view branchName) {
    git_object* raw_head = nullptr;
    if (git_revparse_single(&raw_head, m_repo.get(), "HEAD") != 0) {
      return fail("git_revparse_single HEAD");
    }
    ObjPtr head_obj{raw_head, git_object_free};

    git_commit* raw_commit = nullptr;
    if (git_commit_lookup(&raw_commit, m_repo.get(),
                          git_object_id(head_obj.get())) != 0) {
      return fail("git_commit_lookup");
    }
    CommitPtr head_commit{raw_commit, git_commit_free};

    git_reference* raw_ref = nullptr;
    if (git_branch_create(&raw_ref, m_repo.get(),
                          std::string(branchName).c_str(), head_commit.get(),
                          0) != 0) {
      return fail("git_branch_create");
    }
    RefPtr branch_ref{raw_ref, git_reference_free};

    return checkoutRef(branch_ref.get());
  }

  /// @brief Check out an existing local branch.
  ::testing::AssertionResult checkoutBranch(std::string_view branchName) {
    git_reference* raw_ref = nullptr;
    if (git_branch_lookup(&raw_ref, m_repo.get(),
                          std::string(branchName).c_str(),
                          GIT_BRANCH_LOCAL) != 0) {
      return fail("git_branch_lookup");
    }
    RefPtr ref{raw_ref, git_reference_free};

    return checkoutRef(ref.get());
  }

  /// @brief Merge a local branch into HEAD.
  /// @details When allowConflicts is true the method succeeds even if the merge
  ///          leaves the index in a conflicted state (mirrors `git merge ||
  ///          true`).
  ::testing::AssertionResult mergeBranch(std::string_view branchName,
                                         bool allowConflicts = false) {
    git_reference* raw_branch_ref = nullptr;
    if (git_branch_lookup(&raw_branch_ref, m_repo.get(),
                          std::string(branchName).c_str(),
                          GIT_BRANCH_LOCAL) != 0) {
      return fail("git_branch_lookup");
    }
    RefPtr branch_ref{raw_branch_ref, git_reference_free};

    git_annotated_commit* raw_annotated = nullptr;
    if (git_annotated_commit_from_ref(&raw_annotated, m_repo.get(),
                                      branch_ref.get()) != 0) {
      return fail("git_annotated_commit_from_ref");
    }
    AnnotatedPtr annotated{raw_annotated, git_annotated_commit_free};

    git_merge_analysis_t analysis = GIT_MERGE_ANALYSIS_NONE;
    git_merge_preference_t preference = GIT_MERGE_PREFERENCE_NONE;
    std::array<const git_annotated_commit*, 1> heads = {annotated.get()};

    if (git_merge_analysis(&analysis, &preference, m_repo.get(), heads.data(),
                           heads.size()) != 0) {
      return fail("git_merge_analysis");
    }

    if (bool(analysis & GIT_MERGE_ANALYSIS_UP_TO_DATE)) {
      return ::testing::AssertionSuccess();
    }

    if (bool(analysis & GIT_MERGE_ANALYSIS_FASTFORWARD)) {
      return performFastForward(annotated.get());
    }

    if (!(bool(analysis & GIT_MERGE_ANALYSIS_NORMAL))) {
      return ::testing::AssertionFailure() << "Unexpected merge analysis state";
    }

    git_merge_options merge_opts = GIT_MERGE_OPTIONS_INIT;
    git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
    checkout_opts.checkout_strategy =
        GIT_CHECKOUT_FORCE | GIT_CHECKOUT_ALLOW_CONFLICTS;

    const int result = git_merge(m_repo.get(), heads.data(), heads.size(),
                                 &merge_opts, &checkout_opts);

    if (result != 0 && !allowConflicts) {
      return fail("git_merge");
    }

    // If conflicts are present and not tolerated, abort and report.
    auto index = getIndex();
    if (index && (git_index_has_conflicts(index.get()) == 1) &&
        !allowConflicts) {
      git_repository_state_cleanup(m_repo.get());
      return ::testing::AssertionFailure() << "Merge produced conflicts";
    }

    return ::testing::AssertionSuccess();
  }

  /// @brief Emit the status of every file in the repo to stdout (useful for
  ///        debugging failing tests).
  ::testing::AssertionResult printStatus() {
    git_status_foreach(
        m_repo.get(),
        [](const char* path, unsigned int flags, void*) -> int {
          std::cout << "  " << path << " (0x" << std::hex << flags << std::dec
                    << ")\n";
          return 0;
        },
        nullptr);
    return ::testing::AssertionSuccess();
  }

 private:
  // ------------------------------------------------------------------
  // Smart-pointer aliases for libgit2 objects
  // ------------------------------------------------------------------
  using RepoPtr =
      std::unique_ptr<git_repository, decltype(&git_repository_free)>;
  using IndexPtr = std::unique_ptr<git_index, decltype(&git_index_free)>;
  using RefPtr = std::unique_ptr<git_reference, decltype(&git_reference_free)>;
  using ObjPtr = std::unique_ptr<git_object, decltype(&git_object_free)>;
  using CommitPtr = std::unique_ptr<git_commit, decltype(&git_commit_free)>;
  using TreePtr = std::unique_ptr<git_tree, decltype(&git_tree_free)>;
  using SigPtr = std::unique_ptr<git_signature, decltype(&git_signature_free)>;
  using AnnotatedPtr = std::unique_ptr<git_annotated_commit,
                                       decltype(&git_annotated_commit_free)>;

  // ------------------------------------------------------------------
  // Helpers
  // ------------------------------------------------------------------

  /// @brief Initialise a repository at path with initial branch "main".
  static RepoPtr openOrInit(const std::filesystem::path& path) {
    git_repository_init_options opts = GIT_REPOSITORY_INIT_OPTIONS_INIT;
    opts.flags = GIT_REPOSITORY_INIT_MKPATH;
    opts.initial_head = "main";

    git_repository* repo = nullptr;
    git_repository_init_ext(&repo, path.c_str(), &opts);
    return RepoPtr{repo, git_repository_free};
  }

  /// @brief Return the repository's index, reading it from disk first.
  IndexPtr getIndex() {
    git_index* raw = nullptr;
    if (git_repository_index(&raw, m_repo.get()) != 0) {
      return {nullptr, git_index_free};
    }
    IndexPtr index{raw, git_index_free};
    git_index_read(index.get(), /*force=*/1);
    return index;
  }

  /// @brief Build an AssertionFailure carrying the last libgit2 error message.
  static ::testing::AssertionResult fail(std::string_view operation) {
    const git_error* err = git_error_last();
    return ::testing::AssertionFailure()
           << operation
           << " failed: " << (err != nullptr ? err->message : "unknown error");
  }

  /// @brief Core commit creation. Works for both empty and non-empty commits.
  ::testing::AssertionResult createCommit(std::string_view message) {
    auto index = getIndex();
    if (!index) {
      return fail("git_repository_index");
    }

    git_oid tree_oid;
    if (git_index_write_tree(&tree_oid, index.get()) != 0) {
      return fail("git_index_write_tree");
    }

    git_tree* raw_tree = nullptr;
    if (git_tree_lookup(&raw_tree, m_repo.get(), &tree_oid) != 0) {
      return fail("git_tree_lookup");
    }
    TreePtr tree{raw_tree, git_tree_free};

    git_signature* raw_sig = nullptr;
    if (git_signature_now(&raw_sig, "user", "user@example.com") != 0) {
      return fail("git_signature_now");
    }
    SigPtr sig{raw_sig, git_signature_free};

    // Collect parent commits (empty vector for the very first commit).
    std::vector<const git_commit*> parents;
    CommitPtr parent_owner{nullptr, git_commit_free};

    git_object* raw_head = nullptr;
    if (git_revparse_single(&raw_head, m_repo.get(), "HEAD") == 0) {
      ObjPtr head_obj{raw_head, git_object_free};
      git_commit* raw_parent = nullptr;
      if (git_commit_lookup(&raw_parent, m_repo.get(),
                            git_object_id(head_obj.get())) == 0) {
        parent_owner.reset(raw_parent);
        parents.push_back(raw_parent);
      }
    }

    git_oid commit_oid;
    if (git_commit_create(
            &commit_oid, m_repo.get(), "HEAD", sig.get(), sig.get(),
            /*message_encoding=*/nullptr, std::string(message).c_str(),
            tree.get(), parents.size(), parents.data()) != 0) {
      return fail("git_commit_create");
    }

    return ::testing::AssertionSuccess();
  }

  /// @brief Update HEAD and working directory to point at the target of ref.
  ::testing::AssertionResult checkoutRef(git_reference* ref) {
    git_reference* raw_resolved = nullptr;
    if (git_reference_resolve(&raw_resolved, ref) != 0) {
      return fail("git_reference_resolve");
    }
    RefPtr resolved{raw_resolved, git_reference_free};

    git_object* raw_target = nullptr;
    if (git_object_lookup(&raw_target, m_repo.get(),
                          git_reference_target(resolved.get()),
                          GIT_OBJECT_COMMIT) != 0) {
      return fail("git_object_lookup");
    }
    ObjPtr target{raw_target, git_object_free};

    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_SAFE | GIT_CHECKOUT_RECREATE_MISSING;

    if (git_checkout_tree(m_repo.get(), target.get(), &opts) != 0) {
      return fail("git_checkout_tree");
    }

    if (git_repository_set_head(m_repo.get(), git_reference_name(ref)) != 0) {
      return fail("git_repository_set_head");
    }

    return ::testing::AssertionSuccess();
  }

  /// @brief Advance the current branch ref to the target commit (fast-forward).
  ::testing::AssertionResult performFastForward(
      const git_annotated_commit* target) {
    const git_oid* target_oid = git_annotated_commit_id(target);

    git_object* raw_obj = nullptr;
    if (git_object_lookup(&raw_obj, m_repo.get(), target_oid,
                          GIT_OBJECT_COMMIT) != 0) {
      return fail("git_object_lookup (fast-forward)");
    }
    ObjPtr target_obj{raw_obj, git_object_free};

    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_SAFE;

    if (git_checkout_tree(m_repo.get(), target_obj.get(), &opts) != 0) {
      return fail("git_checkout_tree (fast-forward)");
    }

    git_reference* raw_head_ref = nullptr;
    if (git_repository_head(&raw_head_ref, m_repo.get()) != 0) {
      return fail("git_repository_head (fast-forward)");
    }
    RefPtr head_ref{raw_head_ref, git_reference_free};

    git_reference* raw_new_ref = nullptr;
    if (git_reference_set_target(&raw_new_ref, head_ref.get(), target_oid,
                                 "merge: Fast-forward") != 0) {
      return fail("git_reference_set_target (fast-forward)");
    }
    RefPtr new_ref{raw_new_ref, git_reference_free};

    return ::testing::AssertionSuccess();
  }

  std::filesystem::path m_path;
  RepoPtr m_repo{nullptr, git_repository_free};
};

}  // namespace gitxx
