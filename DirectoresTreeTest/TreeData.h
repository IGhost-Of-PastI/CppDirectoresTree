#pragma once
#include <filesystem>
#include <type_traits>

class BaseData {};

template <class D, bool isLarger>
struct DirectoryTreeDataImpl;

template <class D>
struct DirectoryTreeDataImpl<D, true >
{
    static_assert(std::is_base_of<BaseData, D>::value, "D must inherit from BaseData");

    D data;  
    std::filesystem::path path;
};

template <class D>
struct DirectoryTreeDataImpl<D, false>
{
    static_assert(std::is_base_of<BaseData, D>::value, "D must inherit from BaseData");

    std::filesystem::path path;
    D data;
};
 
template <class D>
struct DirectoryTreeData : public DirectoryTreeDataImpl<D, (sizeof(D) > sizeof(std::filesystem::path))> {};