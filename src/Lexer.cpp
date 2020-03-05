#include "calc/Lexer.h"

using namespace calc;

Token VectorLexer::peek() const {
  assert(Cursor != Toks.end());
  return *Cursor;
}

Token VectorLexer::pop() {
  assert(Cursor != Toks.end());
  return *Cursor++;
}

bool VectorLexer::empty() const { return Cursor == Toks.end(); }

Token IOSLexer::peek() const {
  return Token(Token::Number, "1"); // FIXME
}

Token IOSLexer::pop() {
  return Token(Token::Number, "1"); // FIXME
}

bool IOSLexer::empty() const {
  return true; // FIXME
}