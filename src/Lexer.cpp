#include "calc/Lexer.h"

#include <cctype>
#include <sstream>

using namespace calc;

Lexer::~Lexer() = default;

std::optional<Token> VectorLexer::doPeek() {
  if (!Tok.has_value() && Cursor != Toks.size())
    Tok = Toks[Cursor++];
  
  return *Tok;
}

Token VectorLexer::doPop() {
  assert(Tok.has_value());
  return *std::exchange(Tok, std::nullopt);
}

bool VectorLexer::isEmpty() const { return Cursor == Toks.size(); }

std::optional<Token> IOSLexer::doPeek() {
  if (!Tok.has_value()) {
    Tok = next();
  }
  return Tok;
}

Token IOSLexer::doPop() {
  assert(Tok.has_value());
  return *std::exchange(Tok, std::nullopt);
}

bool IOSLexer::isEmpty() const {
  return IS.eof();
}

SLoc IOSLexer::getLocation() const { return std::make_pair(Line, Col); }

std::optional<Token> IOSLexer::next() {
  if (IS.eof()) {
    return std::nullopt;
  }

  // Skip whitespace
  for (int C = IS.peek(); std::isspace(C) && IS.get(); C = IS.peek()) {
    if (C == '\n') {
      Line++;
      Col = 0;
    } else {
      Col++;
    }
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

  static_assert(sizeof(Matches) / sizeof(Matches[0]) ==
                    Token::LAST - Token::FIRST,
                "unhandled token?");

  for (const auto &Match : Matches) {
    if (IS.peek() == Match.C) {
      IS.get();
      Col++;
      return Token(location(), Match.Kind);
    }
  }

  if (std::isdigit(IS.peek())) {
    std::stringstream Terminal;
    for (int C = IS.peek(); std::isdigit(C) && IS.get(); C = IS.peek()) {
      Col++;
      Terminal << static_cast<char>(C);
    }
    return Token(location(), Token::Number, Terminal.str());
  }

  return Token(location(), Token::Unknown);
}