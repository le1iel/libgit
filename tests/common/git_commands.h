#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

namespace git {

class GitCommands {
 public:
  GitCommands(std::string_view path) : m_path(path) {
    std::filesystem::path path_obj(path);
    std::filesystem::create_directories(m_path);
    runCommand("git init");
  };

  ~GitCommands() {
    std::cout << m_path << std::endl;
    auto res = std::filesystem::remove_all(m_path);
    std::cout << res << std::endl;

  }

  int commit(std::string_view message) {
    return runCommand("git commit -m \"" + std::string(message) + "\"");
  }

  int makeEmptyCommit(std::string_view message) {
    std::string command{"git commit --allow-empty -m \""};
    command.append(message);
    command.append("\"");
    return runCommand(command);
  };

  int add(std::string_view filename) {
    return runCommand("git add " + std::string(filename));
  }

  int modifyFile(std::string_view filename, std::string_view content) {
    createFile(filename, content);
    return 0;
  }

  int createFile(std::string_view filename, std::string_view content) {
    std::string filePath = m_path + "/" + std::string(filename);
    std::ofstream file;
    file.open(filePath);
    file << content;
    file.close();
    return 0;
  }

  void printStatus() {
    std::string command{"git status"};
    runCommand(command);
  }

  int deleteFile(std::string_view filename) {
    std::string filePath = m_path + "/" + std::string(filename);
    std::filesystem::remove(filePath);
    return 0;
  }

  int renameFile(std::string_view oldName, std::string_view newName) {
    return runCommand("git mv " + std::string(oldName) + " " + std::string(newName));
  }

  int changeFileType(std::string_view filename, std::string_view mode) {
    std::string filePath = m_path + "/" + std::string(filename);
    std::filesystem::permissions(filePath, std::filesystem::perms::owner_all, std::filesystem::perm_options::replace);
    return 0;
  }

  int createBranchAndCheckout(std::string_view branchName) {
    return runCommand("git checkout -b " + std::string(branchName));
  }

  int checkoutBranch(std::string_view branchName) {
    return runCommand("git checkout " + std::string(branchName));
  }

  int mergeBranch(std::string_view branchName, bool allowConflicts = false) {
    std::string command = "git merge " + std::string(branchName);
    if (allowConflicts) {
      command.append(" || true");
    }
    return runCommand(command);
  }

private:

  int runCommand(std::string_view command) {
    if (command.empty()) {
      return 0;
    }

    std::string cmd{"cd "};
    cmd.append(m_path);
    cmd.append(" && ");
    cmd.append(command);
    return system(cmd.c_str());
  }

  std::string path() const { return m_path; }

 private:
  std::string m_path{};
};

}  // namespace git
