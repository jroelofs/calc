#ifndef CALC_TOKEN_H
#define CALC_TOKEN_H

#include <string>

namespace calc {

class Token {
public:
  typedef enum {
    Number,
    LParen,
    RParen,
    Plus,
    Minus,
    Times,
    Divide,
  } Kind;

  Token(Kind K) : K(K) {}
  Token(Kind K, std::string V) : K(K), V(V) {}

  Kind K;
  std::string V;
};

} // namespace calc

#endif