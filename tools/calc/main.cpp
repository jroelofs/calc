#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "calc/Token.h"

#include <sstream>
#include <string>

using namespace calc;

static void usage() {
    std::cerr << "usage:\n"
              << "\tcrepl <expression>\n";
    exit(-1);
}

int main(int argc, char **argv) {
#if 1
  if (argc != 2) {
    usage();
    return -1;
  }
  std::stringstream SS(argv[1]);
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

  int Res = P.parseExpr();
  std::cout << Res << "\n";
}
