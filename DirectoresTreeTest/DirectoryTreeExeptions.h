#pragma once
#include <exception>
#include <string>

class directory_not_found : public std::exception
{
private:
	std::wstring _dirName;
public:
	directory_not_found(std::wstring dirName) :_dirName(dirName) {};
	inline virtual const char* what() noexcept;
};

class directory_already_exist : public std::exception
{
private:
	std::wstring _dirName;
public:
	directory_already_exist(std::wstring dirName) :_dirName(dirName) {};
	inline virtual const char* what() noexcept;
};



