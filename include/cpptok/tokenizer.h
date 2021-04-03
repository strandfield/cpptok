// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPTOK_TOKENIZER_H
#define CPPTOK_TOKENIZER_H

#include "cpptok/token.h"

#include <vector>

namespace cpptok
{

class CPPTOK_API Tokenizer
{
public:

  enum State
  {
    Default,
    LongComment,
  };

public:
  State state = State::Default;
  std::vector<Token> output;

public:
  Tokenizer() = default;

  void tokenize(const std::string& str);
  void tokenize(const char* str);
  void tokenize(const char* str, size_t len);

  enum CharacterType {
    Invalid,
    Space,
    Letter,
    Digit,
    Dot,
    SingleQuote,
    DoubleQuote,
    LeftPar,
    RightPar,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Punctuator,
    Underscore,
    Semicolon,
    Colon, 
    QuestionMark,
    Comma,
    Tabulation,
    LineBreak,
    CarriageReturn,
    Other,
  };

  static CharacterType ctype(char c);
  inline static bool isLetter(char c) { return ctype(c) == Letter; }
  inline static bool isDigit(char c) { return ctype(c) == Digit; }
  inline static bool isBinary(char c) { return c == '0' || c == '1'; }
  inline static bool isOctal(char c) { return '0' <= c && c <= '7'; }
  inline static bool isDecimal(char c) { return isDigit(c); }
  inline static bool isHexa(char c) { return isDecimal(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }
  static bool isDiscardable(char c);
  inline static bool isSpace(char c) { return ctype(c) == Space; }

protected:
  Token read();
  Token readToken();
  bool atEnd() const;
  size_t pos() const;
  char readChar();
  void discardChar() noexcept;
  char charAt(size_t pos);
  char currentChar() const;
  inline char peekChar() const { return currentChar(); }
  void consumeDiscardable();
  string_view currentText() const;
  Token create(size_t pos, size_t length, TokenType type);
  Token create(size_t pos, TokenType type);
  Token readNumericLiteral();
  Token readHexa();
  Token readOctal();
  Token readBinary();
  Token readDecimal();
  Token readIdentifier();
  TokenType identifierType(size_t begin, size_t end) const;
  Token readStringLiteral();
  Token readCharLiteral();
  TokenType getOperator(size_t begin, size_t end) const;
  Token readOperator();
  Token readColonOrColonColon(size_t pos);
  Token readFromPunctuator(size_t pos);
  Token readSingleLineComment(size_t pos);
  Token createLongComment();
  Token readMultiLineComment();
  bool tryReadLiteralSuffix();

private:
  const char* m_chars = nullptr;
  size_t m_len = 0;
  size_t m_pos = 0;
  size_t m_start = 0;
};

} // namespace cpptok

#endif // CPPTOK_TOKENIZER_H
