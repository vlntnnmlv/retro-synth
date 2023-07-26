#include "Utility.h"

std::string *read_file(const char *path)
{
    std::ifstream ifs(path);
    return new std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}