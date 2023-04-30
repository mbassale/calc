add_executable (calc
  Calc.cpp
  Lexer.h
  Lexer.cpp
  Parser.h
  Parser.cpp
  AST.h
  Sema.h
  Sema.cpp
  )
target_link_libraries(calc PRIVATE ${llvm_libs})