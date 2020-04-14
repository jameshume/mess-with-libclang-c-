#include <iostream>
#include "clang-c/Index.h"

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
 	clang_disposeString(str);
	return stream;
}

int main(int argc, char *argv[])
{
	CXIndex index = clang_createIndex(0, 0);
 	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		"test_files/test1.c",
		nullptr, 0,
		nullptr, 0,
		CXTranslationUnit_None);
	
	if (unit == nullptr)
	{
		std::cerr << "Unable to parse translation unit. Quitting.\n";
		exit(-1);
	}

	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(
		cursor,
		[](CXCursor c, CXCursor parent, CXClientData client_data)
		{
			std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
			          << clang_getCursorKindSpelling(clang_getCursorKind(c))
			          << ". Parent is '" << clang_getCursorSpelling(parent)
				  << "' of kind '" << clang_getCursorKindSpelling(clang_getCursorKind(parent)) << "'\n";
			return CXChildVisit_Recurse;
		},
 		nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return 0;
}
