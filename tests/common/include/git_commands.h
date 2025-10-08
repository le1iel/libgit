#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <source_location>
#include <string>
#include <string_view>
#include <tuple>

#include "gtest/gtest.h"

namespace gitxx {

class GitCommands {
 public:
  GitCommands(std::string_view path) : m_path(path) {
    std::filesystem::create_directories(m_path);
    std::ignore = runCommand("git init -q");
  };

  GitCommands(
      const std::source_location location = std::source_location::current())
      : m_home(TEST_GIT_HOME), m_path(TEST_OUTPUT_DIR "/gitxx_test") {
    const std::string test_function = location.function_name();
    const auto first = test_function.rfind(' ') + 1;
    const auto end = test_function.rfind("_Test");
    const auto len =
        test_function.size() - first - (test_function.size() - end);
    const std::string_view test_name{test_function.c_str() + first, len};
    m_path.append(test_name);
    std::filesystem::remove_all(m_path);

    std::filesystem::create_directories(m_path);

    std::ignore = runCommand("git init -q");
  }

  // ~GitCommands() { std::filesystem::remove_all(m_path); }

  ::testing::AssertionResult commit(std::string_view message) {
    return runCommand("git commit -m \"" + std::string(message) + "\"");
  }

  ::testing::AssertionResult initRepo() {
    std::string cmd{"git init -q"};
    return runCommand(cmd) ? testing::AssertionSuccess()
                           : testing::AssertionFailure()
                                 << "Error initializing repo (" << cmd << ")";
  }

  ::testing::AssertionResult makeEmptyCommit(std::string_view message) {
    std::string cmd{"git commit --allow-empty -m \""};
    cmd.append(message);
    cmd.append("\"");
    return runCommand(cmd);
  };

  ::testing::AssertionResult add(std::string_view filename) {
    return runCommand("git add " + std::string(filename));
  }

  ::testing::AssertionResult createDir(std::string_view name) {
    std::error_code ec{};

    if (std::filesystem::create_directory(m_path / name, ec) == true)
      return testing::AssertionSuccess();
    return testing::AssertionFailure()
           << "Tried creating dir: " << m_path / name << " Got error: " << ec;
  }

  ::testing::AssertionResult modifyFile(std::string_view filename,
                                        std::string_view content) {
    return createFile(filename, content);
  }

  ::testing::AssertionResult createFile(std::string_view filename,
                                        std::string_view content) {
    std::ofstream file{};
    file.open(m_path / filename);
    file << content;
    file.close();

    if (std::filesystem::exists(m_path / filename))
      return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure() << " was not made";
  }

  ::testing::AssertionResult printStatus() {
    std::string command{"git status"};
    return runCommand(command);
  }

  ::testing::AssertionResult deleteFile(std::string_view filename) {
    std::error_code ec{};
    if (std::filesystem::remove(m_path / filename, ec) == true)
      return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
           << "Tried deleting: " << (m_path / filename) << "Got error "
           << ec.message();
  }

  ::testing::AssertionResult renameFile(std::string_view oldName,
                                        std::string_view newName) {
    return runCommand("git mv " + std::string(oldName) + " " +
                      std::string(newName));
  }

  ::testing::AssertionResult createBranchAndCheckout(
      std::string_view branchName) {
    return runCommand("git checkout -b " + std::string(branchName));
  }

  ::testing::AssertionResult checkoutBranch(std::string_view branchName) {
    return runCommand("git checkout " + std::string(branchName));
  }

  ::testing::AssertionResult mergeBranch(std::string_view branchName,
                                         bool allowConflicts = false) {
    std::string command = "git merge " + std::string(branchName);
    if (allowConflicts) {
      command.append(" || true");
    }
    return runCommand(command);
  }

  std::filesystem::path path() const { return m_path; }

 private:
  void cd(std::string& cmd) {
    cmd.append("cd ");
    cmd.append(m_path);
    cmd.append(" && ");
    cmd.append("HOME=\"");
    cmd.append(m_home.string());
    cmd.append("\" ");
  }

  ::testing::AssertionResult runCommand(std::string_view command) {
    if (command.empty()) {
      return testing::AssertionFailure() << "Command is empty";
    }

    std::string cmd{};
    cd(cmd);
    cmd.append(command);

    if (system(cmd.c_str()) == 0) return testing::AssertionSuccess();

    return testing::AssertionFailure()
           << "Got error return code from (" << cmd << ")";
  }

  std::filesystem::path m_path{};
  std::filesystem::path m_home{};
};

}  // namespace gitxx
