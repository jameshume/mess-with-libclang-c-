#include <iostream>
#include <set>
#include <string>
#include "clang-c/Index.h"

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
 	clang_disposeString(str);
	return stream;
}

static std::set<CXCursor *> function_declarations;
static std::set<CXCursor *> function_definitions;

int main(int argc, char *argv[])
{
	CXIndex index = clang_createIndex(0, 0);
 	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		"test_files/test2.c",
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
			const CXCursorKind kind = clang_getCursorKind(c);
			if (kind == CXCursor_FunctionDecl)
			{
				const CXLinkageKind linkage = clang_getCursorLinkage(c);
				if (linkage == CXLinkage_Internal)
				{
					if (clang_isCursorDefinition(c))
					{
						std::cout << clang_getCursorSpelling(c) << " IS DEFINTION - " << clang_getCursorLinkage(c) << "\n";
					}
					else
					{
						std::cout << clang_getCursorSpelling(c) << " IS DECLARATION - " << clang_getCursorLinkage(c) << "\n";
					}
				}
			}
			return CXChildVisit_Recurse;
		},
 		nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return 0;
}

