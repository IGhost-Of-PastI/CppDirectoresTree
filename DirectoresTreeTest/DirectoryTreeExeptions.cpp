#include "DirectoryTreeExeptions.h"
#include <format>

inline const char* directory_not_found::what() noexcept
{
    std::string formttedString = std::format("����� ��� ��������� : {} �� �������!",_dirName);
    return formttedString.c_str();
}

inline const char* directory_already_exist::what() noexcept
{
    std::string formttedString = std::format("����� ��� ��������� : {} ��� ����������!", _dirName);
    return formttedString.c_str();
}
