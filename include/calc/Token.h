#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <string>
#include <iostream>

namespace calc {

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
  Token(Kind K) : K(K) {}
  Token(Kind K, std::string V) : K(K), V(V) {}

  static const char *toString(Token::Kind Kind);
  void print(std::ostream &OS) const;
  void dump() const;

  Kind K;
  std::string V;
};

std::ostream &operator<<(std::ostream &OS, const Token &Tok);

} // namespace calc

#endif