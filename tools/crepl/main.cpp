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

static void evaluate(const std::string &Prefix, Lexer &L) {
  Parser<float> P(L);
  ErrorOr<float> Res = P.parse();
  if (Res) {
    std::cout << *Res << "\n";
  } else {
    SLoc Loc = Res.getError().Loc;
    if (Prefix != "")
      std::cerr << Prefix << std::string(Loc.second, ' ') << "^\n";
    std::cerr << "error: " << Res.getError() << "\n";
  }
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
      StreamLexer L(SS);
      evaluate("  ", L);
    } while (true);
  } else if (argc == 2) {
    if (std::string("-h") == argv[1] ||
        std::string("--help") == argv[1]) {
      usage();
      return -1;
    }

    // Input via pipe
    if (std::string("-") == argv[1]) {
      StreamLexer L(std::cin);
      evaluate("", L);
      return 0;
    }

    // Input via command-line argument
    std::stringstream SS(argv[1]);
    StreamLexer L(SS);
    evaluate(std::string(argv[1]) + "\n", L);
    return 0;
  }

  usage();
  return -1;
}
