#include "calc/Lexer.h"

#include <cctype>
#include <sstream>

using namespace calc;

std::optional<Token> VectorLexer::peek() {
  if (!Tok.has_value())
    if (Cursor != Toks.end())
      Tok = *Cursor++;
  
  return *Tok;
}

Token VectorLexer::pop() {
  assert(Tok.has_value());
  return *std::exchange(Tok, std::nullopt);
}

bool VectorLexer::empty() const { return Cursor == Toks.end(); }

std::optional<Token> IOSLexer::peek() {
  if (!Tok.has_value()) {
    Tok = next();
  }
  return Tok;
}

Token IOSLexer::pop() {
  assert(Tok.has_value());
  return *std::exchange(Tok, std::nullopt);
}

bool IOSLexer::empty() const {
  return IS.eof();
}

SLoc IOSLexer::location() const { return std::make_pair(Line, Col); }

std::optional<Token> IOSLexer::next() {
  if (IS.eof()) {
    return std::nullopt;
  }

  char c = IS.peek();

  // Skip whitespace
  while (std::isspace(c)) {
    IS.get();
    if (c == '\n') {
      Line++;
      Col = 0;
    } else {
      Col++;
    }
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
    { '!', Token::Bang },
  };

  for (const auto &Match : Matches) {
    if (c == Match.C) {
      IS.get();
      Col++;
      return Token(location(), Match.Kind);
    }
  }

  if (std::isdigit(c)) {
    std::stringstream Terminal;
    while (std::isdigit(c)) {
      IS.get();
      Col++;
      Terminal << c;
      c = IS.peek();
    }
    return Token(location(), Token::Number, Terminal.str());
  }

  return Token(location(), Token::Unknown);
}