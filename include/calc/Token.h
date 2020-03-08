#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <iostream>
#include <string>
#include <utility>

namespace calc {

using SLoc = std::pair<int, int>;

class Token {
public:
  typedef enum {
    Unknown,
    Number,
    LParen,
    RParen,
    Plus,
    Minus,
    Times,
    Divide,
  } Kind;

  Token() : K(Token::Unknown) {}
  Token(SLoc Loc, Kind K) : K(K) {}
  Token(SLoc Loc, Kind K, std::string V) : K(K), V(V) {}

  void print(std::ostream &OS) const;
  void dump() const;

  SLoc Loc;
  Kind K;
  std::string V;
};

std::ostream &operator<<(std::ostream &OS, const Token &Tok);
const char *toString(Token::Kind Kind);

} // namespace calc

#endif