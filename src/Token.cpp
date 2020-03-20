#include "calc/Token.h"

using namespace calc;

void Token::print(std::ostream &OS) const {
  if (K == Token::Number) {
    OS << '<' << Loc.first << ':' << Loc.second << ":#" << V << '>';
    return;
  }

  OS << '<' << Loc.first << ':' << Loc.second << ':' << toString(K) << '>';
}

void Token::dump() const {
  print(std::cout);
  std::cout << "\n";
}

std::ostream &calc::operator<<(std::ostream &OS, const Token &Tok) {
  Tok.print(OS);
  return OS;
}

const char *calc::toString(Token::Kind Kind) {
  switch (Kind) {
  case Token::Number: return "number";
  case Token::Unknown: return "<unknown>";
  case Token::Plus: return "+";
  case Token::Minus: return "-";
  case Token::Times: return "*";
  case Token::Divide: return "/";
  case Token::LParen: return "(";
  case Token::RParen: return ")";
  case Token::Bang: return "!";
  }
  __builtin_unreachable();
}