#include <string_view>
#include <string>

namespace git {


void Init(std::string_view path) {
    std::string command { "git init "};
    command.append(path);
    int res = system(command.c_str());

}

} // namespace git
