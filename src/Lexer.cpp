#include "calc/Lexer.h"

#include <cctype>
#include <sstream>

using namespace calc;

const Token &VectorLexer::peek() const {
  assert(Cursor != Toks.end());
  return *Cursor;
}

Token VectorLexer::pop() {
  assert(Cursor != Toks.end());
  return *Cursor++;
}

bool VectorLexer::empty() const { return Cursor == Toks.end(); }

const Token &IOSLexer::peek() const {
  return *Tok;
}

Token IOSLexer::pop() {
  return *std::exchange(Tok, next());
}

bool IOSLexer::empty() const {
  return !Tok.has_value() && IS.eof();
}

std::optional<Token> IOSLexer::next() {
  if (IS.eof()) {
    return std::nullopt;
  }

  char c = IS.peek();

  // Skip whitespace
  while (std::isspace(c)) {
    IS.get();
    c = IS.peek();
  }

  const struct {
    char C;
    Token::Kind Kind;
  } Matches[] = {
    { '+', Token::Plus },
    { '-', Token::Minus },
    { '*', Token::Times },
    { '/', Token::Divide },
    { '(', Token::LParen },
    { ')', Token::RParen },
  };

  for (const auto &Match : Matches) {
    if (c == Match.C) {
      IS.get();
      return Token(Match.Kind);
    }
  }

  if (std::isdigit(c)) {
    std::stringstream Terminal;
    while (std::isdigit(c)) {
      IS.get();
      Terminal << c;
      c = IS.peek();
    }
    return Token(Token::Number, Terminal.str());
  }

  return std::nullopt;
}