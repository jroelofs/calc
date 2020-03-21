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
  Parser(Lexer &Lexer) noexcept : Lexer(Lexer) {}

  std::optional<Token> accept(Token::Kind K) {
    if (std::optional<Token> T = Lexer.peek())
      if (T->K == K)
        return Lexer.pop();
    return std::nullopt;
  }

  ErrorOr<Token> expect(Token::Kind K) {
    if (std::optional<Token> T = accept(K)) {
      return *T;
    }
    return Error(Lexer.location(), std::string("expected ") + toString(K));
  }

  ErrorOr<Expr> parse() {
    ErrorOr<Expr> E = parseExpr();

    if (E.hasError())
      return E;

    if (!Lexer.empty())
      return Error(Lexer.location(), "unexpected trailing characters");

    return E;
  }

  // exp : term
  //     | exp `+` term
  //     | exp `-` term
  ErrorOr<Expr> parseExpr() {
    ErrorOr<Expr> E = parseTerm();

    while (E) {
      if (accept(Token::Plus)) {
        E = E + parseTerm();
      } else if (accept(Token::Minus)) {
        E = E - parseTerm();
      } else {
        break;
      }
    }

    return E;
  }

  // term : factor
  //      | factor `*` term
  //      | factor `/` term
  ErrorOr<Expr> parseTerm() {
    ErrorOr<Expr> E = parseFactor();

    while (E) {
      if (accept(Token::Times)) {
        E = E * parseFactor();
      } else if (accept(Token::Divide)) {
        // Clients can deal with potential div-by-0 here by supplying a class to
        // instantiate `Expr` as that does the appropriate checking.
        E = E / parseFactor();
      } else {
        break;
      }
    }

    return E;
  }

  // factor : `number`
  //        | `(` exp `)`
  //        | `+` factor
  //        | `-` factor
  //        | `!` factor
  ErrorOr<Expr> parseFactor() {
    if (accept(Token::LParen)) {
      ErrorOr<Expr> E = parseExpr();
      ErrorOr<Token> paren = expect(Token::RParen);
      if (paren.hasError())
        return paren.getError();
      return E;
    }

    if (accept(Token::Plus))
      return +parseFactor();

    if (accept(Token::Minus))
      return -parseFactor();

    if (accept(Token::Bang))
      return !parseFactor();

    return parseNumber();
  }

  ErrorOr<Expr> parseNumber() {
    ErrorOr<Token> T = expect(Token::Number);

    if (T.hasValue()) {
      try {
        return Expr(std::stoi(T->V));
      } catch (std::invalid_argument) {
        return Error(T->Loc, "invalid number");
      } catch (std::out_of_range) {
        return Error(T->Loc, "number out of range");
      }
    }

    return T.getError();
  }

private:
  Lexer &Lexer;
};

} // namespace calc

#endif
