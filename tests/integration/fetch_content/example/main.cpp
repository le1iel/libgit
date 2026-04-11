#include <gitxx/init.hpp>
#include <gitxx/repository.hpp>

#include <filesystem>
#include <iostream>
#include <span>

int main(int argc, char** argv) {
    init_libgit();

    const std::span args{argv, static_cast<std::size_t>(argc)};
    auto path = argc > 1
        ? std::filesystem::path{args[1]}
        : std::filesystem::current_path();

    auto repo = gitxx::Repository::Open(path);
    if (!repo) {
        std::cerr << "Failed to open repository at: " << path << "\n";
        deinit_libgit();
        return 1;
    }

    std::cout << "Opened repository at: " << path << "\n";

    if (auto head = repo->head()) {
        std::cout << "HEAD: " << head->name() << "\n";
    } else {
        std::cout << "HEAD: (unborn)\n";
    }

    deinit_libgit();
    return 0;
}
