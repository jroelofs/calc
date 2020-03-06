#include "calc/Lexer.h"
#include "calc/Parser.h"
#include "calc/Token.h"

#include <sstream>
#include <string>

using namespace calc;

static void usage() {
    std::cerr << "usage:\n"
              << "\tcrepl\n"
              << "\tcrepl <expression>\n"
              << "\techo <expression> | crepl -\n";
    exit(-1);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    // Read Evaluate Print Loop, REPL
    do {
      std::cout << "> ";

      std::string line;
      std::getline(std::cin, line);

      if (line == "q" || line == "quit")
        return 0;

      std::stringstream SS(line);
      IOSLexer L(SS);
      Parser<int> P(L);
      int Res = P.parseExpr();
      std::cout << Res << "\n";
    } while (true);
  } else if (argc == 2) {
    if (std::string("-h") == argv[1] ||
        std::string("--help") == argv[1]) {
      usage();
      return -1;
    }

    // Input via pipe
    if (std::string("-") == argv[1]) {
      IOSLexer L(std::cin);
      Parser<int> P(L);
      int Res = P.parseExpr();
      std::cout << Res << "\n";
      return 0;
    }

    // Input via command-line argument
    std::stringstream SS(argv[1]);
    IOSLexer L(SS);
    Parser<int> P(L);
    int Res = P.parseExpr();
    std::cout << Res << "\n";
    return 0;
  }

  usage();
  return -1;
}
