#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

namespace git {

class GitCommands {
 public:
  GitCommands(std::string_view path) : m_path(path) {
    std::filesystem::path path_obj(path);
    std::filesystem::create_directories(m_path);
    std::string command{"cd "};
    command.append(m_path);
    command.append(" && git init");
    std::cout << command << std::endl;
    system(command.c_str());
  };

  ~GitCommands() {
    std::cout << m_path << std::endl;
    auto res = std::filesystem::remove_all(m_path);
    std::cout << res << std::endl;

  }

  void makeEmptyCommit(std::string_view message) {
    std::string command{"cd "};
    command.append(m_path);
    command.append("&& git commit --allow-empty -m \"");
    command.append(message);
    command.append("\"");
    std::cout << command << std::endl;
    int res = system(command.c_str());
  };

  std::string path() const { return m_path; }

 private:
  std::string m_path{};
};

}  // namespace git
