#ifndef PARSER_HPP
#define PARSER_HPP
#include <string.h>

#include <iostream>
#include <string>

#include "compiler.h"
// using namespace std;
struct Value {
  int m_INT;
  double m_REAL;
  char m_CHAR;
  bool m_BOOLEAN;
  char* m_OTHERS;
};

struct Token {
  struct Value value;
  int length;
  int line_num;
  int column_num;
};
extern std::string buf;

// #define YYSTYPE Token
// #define YYSTYPE
#endif
