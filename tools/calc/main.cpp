#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "calc/Token.h"

#include <sstream>
#include <string>

using namespace calc;

int main() {
#if 0
  std::stringstream SS("1");
  IOSLexer L(SS);
#elif 0
  VectorLexer L({Token(Token::Number, "1"), Token::Plus,
                 Token(Token::Number, "2")});
#else
  VectorLexer L({Token(Token::Number, "1"), Token::Times,
                 Token(Token::Number, "2"), Token::Plus,
                 Token(Token::Number, "3")});
#endif

  Parser<int> P(L);

  std::cout << "Result: " << P.parseExpr() << "\n";
}
