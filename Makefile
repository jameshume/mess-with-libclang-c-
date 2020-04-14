CLANG_INC_DIR=/usr/lib/llvm-9/include
CLANG_LIB_DIR=/usr/lib/llvm-9/lib
LDLIBS=-lclang -lstdc++
LOADLIBES=-L$(CLANG_LIB_DIR)

CXXFLAGS+=-I$(CLANG_INC_DIR)


test: test.o 

dump_ast: dump_ast.o
