#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include "calc/Lexer.h"
#include "calc/Token.h"
#include "calc/ErrorOr.h"

#include <optional>

namespace calc {

// Expr is expected to be either a numeric type like `int`, or a class that
// implements similar interfaces (but retains value semantics). An interesting
// implementation would be to build a class that has all the relevant operator
// overloads, that constructs an AST.
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

  ErrorOr<Token> expect(Token::Kind K) {
    if (std::optional<Token> T = accept(K)) {
      return *T;
    }
    return Error(std::string("expected ") + Token::toString(K));
  }

  bool peek(Token::Kind K) {
    if (Lexer.empty()) {
      return false;
    }
    return Lexer.peek().K == K;
  }

  ErrorOr<Expr> parse() {
    ErrorOr<Expr> res = parseExpr();

    if (res.hasError()) {
      return res;
    }

    if (!Lexer.empty()) {
      return Error("unexpected trailing characters");
    }

    return res;
  }

  // exp : term
  //     | exp `+` term
  //     | exp `-` term
  ErrorOr<Expr> parseExpr() {
    ErrorOr<Expr> res = parseTerm();

    while (res && (peek(Token::Plus) || peek(Token::Minus))) {
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
  ErrorOr<Expr> parseTerm() {
    ErrorOr<Expr> res = parseFactor();

    while (res && (peek(Token::Times) || peek(Token::Divide))) {
      if (accept(Token::Times)) {
        res = res * parseFactor();
      } else if (accept(Token::Divide)) {
        // Clients can deal with potential div-by-0 here by supplying a class to
        // instantiate `Expr` as that does the appropriate checking.
        res = res / parseFactor();
      }
    }

    return res;
  }

  // factor : `number`
  //        | `(` exp `)`
  ErrorOr<Expr> parseFactor() {
    if (accept(Token::LParen)) {
      ErrorOr<Expr> res = parseExpr();
      expect(Token::RParen);
      return res;
    }

    return parseNumber();
  }

  ErrorOr<Expr> parseNumber() {
    ErrorOr<Token> T = expect(Token::Number);

    if (T.hasValue()) {
      return Expr(std::stoi(T->V));
    }

    return T.getError();
  }

private:
  Lexer &Lexer;
};

} // namespace calc

#endif
