#include "StdAfx.h"
#include "helpers.h"

#include <sstream>

std::wstring helpers::IntToString(int elem) {
	std::wstringstream out;
	out << elem;
	return out.str();
}