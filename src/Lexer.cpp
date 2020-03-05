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
  Token Ret = *Tok;
  next();
  return Ret;
}

bool IOSLexer::empty() const {
  return !Tok.has_value() && IS.eof();
}

void IOSLexer::next() {
  if (IS.eof()) {
    Tok = std::nullopt;
    return;
  }

  char c = IS.peek();

  // Skip whitespace
  while (std::isspace(c)) {
    IS.get();
    c = IS.peek();
  }

  if (c == '+') {
    IS.get();
    Tok = Token(Token::Plus);
    return;
  }

  if (c == '-') {
    IS.get();
    Tok = Token(Token::Minus);
    return;
  }

  if (c == '*') {
    IS.get();
    Tok = Token(Token::Times);
    return;
  }

  if (c == '/') {
    IS.get();
    Tok = Token(Token::Divide);
    return;
  }

  if (std::isdigit(c)) {
    std::stringstream Terminal;
    while (std::isdigit(c)) {
      IS.get();
      Terminal << c;
      c = IS.peek();
    }
    Tok = Token(Token::Number, Terminal.str());
    return;
  }

  Tok = std::nullopt;
}