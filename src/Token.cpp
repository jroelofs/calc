#include "calc/Token.h"

using namespace calc;

void Token::print(std::ostream &OS) const {
  switch (K) {
  case Token::Number:
    OS << '<' << Loc.first << ':' << Loc.second << ':' << V << '>';
    return;
  case Token::Unknown:
  case Token::Plus:
  case Token::Minus:
  case Token::Times:
  case Token::Divide:
  case Token::LParen:
  case Token::RParen:
    OS << '<' << Loc.first << ':' << Loc.second << ':' << K << '>';
    return;
  }
  __builtin_unreachable();
}

void Token::dump() const {
  print(std::cout);
  std::cout << "\n";
}

std::ostream &operator<<(std::ostream &OS, const Token &Tok) {
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
  }
  __builtin_unreachable();
}