#include <libgit/error.h>

const char* GitErrcCategory::name() const noexcept
{
    return "Git error code";
}

std::string GitErrcCategory::message(int ev) const
{
    return "Git error code";
}


std::error_code make_error_code(GitErrc e)
{
    return { static_cast<int>(e), gitErrcCategory};
}
