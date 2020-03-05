#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "calc/Lexer.h"
#include "calc/Token.h"

#include <optional>

namespace calc {

template <typename Expr> class Parser {
public:
  Parser(Lexer &Lexer) : Lexer(Lexer) {}

  std::optional<Token> accept(Token::Kind K) {
    if (Lexer.empty()) {
      return std::nullopt;
    }

    if (Lexer.peek().K == K) {
      return Lexer.pop();
    }
    return std::nullopt;
  }

  std::optional<Token> expect(Token::Kind K) {
    if (std::optional<Token> T = accept(K)) {
      return T;
    }
    std::cerr << "error: expected " << Token::toString(K) << "\n";
    return std::nullopt;
  }

  bool peek(Token::Kind K) {
    if (Lexer.empty()) {
      return false;
    }
    return Lexer.peek().K == K;
  }

  // exp : term
  //     | exp `+` term
  //     | exp `-` term
  Expr parseExpr() {
    Expr res = parseTerm();

    while (peek(Token::Plus) || peek(Token::Minus)) {
      if (accept(Token::Plus)) {
        res = res + parseTerm();
      } else if (accept(Token::Minus)) {
        res = res - parseTerm();
      }
    }

    return res;
  }

  // term : factor
  //      | factor `*` term
  //      | factor `/` term
  Expr parseTerm() {
    Expr res = parseFactor();

    while (peek(Token::Times) || peek(Token::Divide)) {
      if (accept(Token::Times)) {
        res = res * parseFactor();
      } else if (accept(Token::Divide)) {
        res = res / parseFactor();
      }
    }

    return res;
  }

  // factor : `number`
  //        | `(` exp `)`
  Expr parseFactor() {
    if (peek(Token::LParen)) {
      expect(Token::LParen);
      Expr res = parseExpr();
      expect(Token::RParen);
      return res;
    }

    Expr res = parseNumber();
    return res;
  }

  Expr parseNumber() {
    if (std::optional<Token> T = expect(Token::Number)) {
      return Expr(std::stoi(T->V));
    }
    return Expr();
  }

private:
  Lexer &Lexer;
};

} // namespace calc

#endif
