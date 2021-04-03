// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cpptok/tokenizer.h"

#include <cassert>
#include <cstring>
#include <memory>
#include <stdexcept>

namespace cpptok
{

/*!
 * \fn void tokenize(const std::string& str)
 * \param a string to tokenize
 * 
 * Warning: do not pass temporary string to this function. The output 
 * tokens store the text as a string_view.
 */
void Tokenizer::tokenize(const std::string& str)
{
  tokenize(str.data(), str.length());
}

/*!
 * \fn void tokenize(const char* str)
 * \param a string to tokenize
 */
void Tokenizer::tokenize(const char* str)
{
  tokenize(str, std::strlen(str));
}

/*!
 * \fn void tokenize(const char* str, size_t len)
 * \param the string to tokenize
 * \param the length of the string
 */
void Tokenizer::tokenize(const char* str, size_t len)
{
  m_chars = str;
  m_len = len;
  m_pos = 0;
  m_start = 0;

  if (state == State::LongComment)
    readMultiLineComment();

  while (!atEnd())
    read();
}

void Tokenizer::read()
{
  consumeDiscardable();

  if (atEnd())
    return;

  m_start = pos();
  size_t p = m_start;

  char c = readChar();
  CharacterType ct = ctype(c);

  switch (ct)
  {
  case Digit:
    return readNumericLiteral();
  case DoubleQuote:
    return readStringLiteral();
  case SingleQuote:
    return readCharLiteral();
  case Letter:
  case Underscore:
    return readIdentifier();
  case LeftPar:
    return write(TokenType::LeftPar);
  case RightPar:
    return write(TokenType::RightPar);
  case LeftBrace:
    return write(TokenType::LeftBrace);
  case RightBrace:
    return write(TokenType::RightBrace);
  case LeftBracket:
    return write(TokenType::LeftBracket);
  case RightBracket:
    return write(TokenType::RightBracket);
  case Semicolon:
    return write(TokenType::Semicolon);
  case Colon:
    return readColonOrColonColon();
  case QuestionMark:
    return write(TokenType::QuestionMark);
  case Comma:
    return write(TokenType::Comma);
  case Dot:
    return write(TokenType::Dot);
  case Punctuator:
    return readFromPunctuator(c);
  default:
    return write(TokenType::Invalid);
  }
}

void Tokenizer::write(const Token& tok)
{
  this->output.push_back(tok);
}

void Tokenizer::write(TokenType type)
{
  write(Token(type, currentText()));
}

bool Tokenizer::atEnd() const
{
  return m_pos == m_len;
}

size_t Tokenizer::pos() const
{
  return m_pos;
}

char Tokenizer::readChar()
{
  return *(m_chars + m_pos++);
}

void Tokenizer::discardChar() noexcept
{
  ++m_pos;
}

char Tokenizer::charAt(size_t pos)
{
  return m_chars[pos];
}

char Tokenizer::currentChar() const
{
  return *(m_chars + m_pos);
}

void Tokenizer::consumeDiscardable()
{
  while (!atEnd() && isDiscardable(peekChar()))
    discardChar();
}

string_view Tokenizer::currentText() const
{
  return string_view(m_chars + m_start, pos() - m_start);
}

Tokenizer::CharacterType Tokenizer::ctype(char c)
{
  static const CharacterType map[] = {
    Invalid, // NUL    (Null char.)
    Invalid, // SOH    (Start of Header)
    Invalid, // STX    (Start of Text)
    Invalid, // ETX    (End of Text)
    Invalid, // EOT    (End of Transmission)
    Invalid, // ENQ    (Enquiry)
    Invalid, // ACK    (Acknowledgment)
    Invalid, // BEL    (Bell)
    Invalid, //  BS    (Backspace)
    Tabulation, //  HT    (Horizontal Tab)
    LineBreak, //  LF    (Line Feed)
    Invalid, //  VT    (Vertical Tab)
    Invalid, //  FF    (Form Feed)
    CarriageReturn, //  CR    (Carriage Return)
    Invalid, //  SO    (Shift Out)
    Invalid, //  SI    (Shift In)
    Invalid, // DLE    (Data Link Escape)
    Invalid, // DC1    (XON)(Device Control 1)
    Invalid, // DC2    (Device Control 2)
    Invalid, // DC3    (XOFF)(Device Control 3)
    Invalid, // DC4    (Device Control 4)
    Invalid, // NAK    (Negative Acknowledgement)
    Invalid, // SYN    (Synchronous Idle)
    Invalid, // ETB    (End of Trans. Block)
    Invalid, // CAN    (Cancel)
    Invalid, //  EM    (End of Medium)
    Invalid, // SUB    (Substitute)
    Invalid, // ESC    (Escape)
    Invalid, //  FS    (File Separator)
    Invalid, //  GS    (Group Separator)
    Invalid, //  RS    (Request to Send)(Record Separator)
    Invalid, //  US    (Unit Separator)
    Space, //  SP    (Space)
    Punctuator, //   !    (exclamation mark)
    DoubleQuote, //   "    (double quote)
    Punctuator, //   #    (number sign)
    Punctuator, //   $    (dollar sign)
    Punctuator, //   %    (percent)
    Punctuator, //   &    (ampersand)
    SingleQuote, //   '    (single quote)
    LeftPar, //   (    (left opening parenthesis)
    RightPar, //   )    (right closing parenthesis)
    Punctuator, //   *    (asterisk)
    Punctuator, //   +    (plus)
    Comma, //   ,    (comma)
    Punctuator, //   -    (minus or dash)
    Dot, //   .    (dot)
    Punctuator, //   /    (forward slash)
    Digit, //   0
    Digit, //   1
    Digit, //   2
    Digit, //   3
    Digit, //   4
    Digit, //   5
    Digit, //   6
    Digit, //   7
    Digit, //   8
    Digit, //   9
    Colon, //   :    (colon)
    Semicolon, //   ;    (semi-colon)
    Punctuator, //   <    (less than sign)
    Punctuator, //   =    (equal sign)
    Punctuator, //   >    (greater than sign)
    QuestionMark, //   ?    (question mark)
    Punctuator, //   @    (AT symbol)
    Letter, //   A
    Letter, //   B
    Letter, //   C
    Letter, //   D
    Letter, //   E
    Letter, //   F
    Letter, //   G
    Letter, //   H
    Letter, //   I
    Letter, //   J
    Letter, //   K
    Letter, //   L
    Letter, //   M
    Letter, //   N
    Letter, //   O
    Letter, //   P
    Letter, //   Q
    Letter, //   R
    Letter, //   S
    Letter, //   T
    Letter, //   U
    Letter, //   V
    Letter, //   W
    Letter, //   X
    Letter, //   Y
    Letter, //   Z
    LeftBracket, //   [    (left opening bracket)
    Punctuator, //   \    (back slash)
    RightBracket, //   ]    (right closing bracket)
    Punctuator, //   ^    (caret cirumflex)
    Underscore, //   _    (underscore)
    Punctuator, //   `
    Letter, //   a
    Letter, //   b
    Letter, //   c
    Letter, //   d
    Letter, //   e
    Letter, //   f
    Letter, //   g
    Letter, //   h
    Letter, //   i
    Letter, //   j
    Letter, //   k
    Letter, //   l
    Letter, //   m
    Letter, //   n
    Letter, //   o
    Letter, //   p
    Letter, //   q
    Letter, //   r
    Letter, //   s
    Letter, //   t
    Letter, //   u
    Letter, //   v
    Letter, //   w
    Letter, //   x
    Letter, //   y
    Letter, //   z
    LeftBrace, //   {    (left opening brace)
    Punctuator, //   |    (vertical bar)
    RightBrace, //   }    (right closing brace)
    Punctuator, //   ~    (tilde)
    Invalid, // DEL    (delete)
  };

  if(c <= 127)
    return map[c];
  return Other;
}

bool Tokenizer::isDiscardable(char c)
{
  return ctype(c) == Space || ctype(c) == LineBreak || ctype(c) == CarriageReturn || ctype(c) == Tabulation;
}

void Tokenizer::readNumericLiteral()
{
  if (atEnd()) {
    if (charAt(m_start) == '0')
      return write(TokenType::OctalLiteral);
    else
      return write(TokenType::IntegerLiteral);
  }

  char c = peekChar();

  // Reading binary, octal or hexadecimal number
  // eg. : 0b00110111
  //       018
  //       0xACDBE
  if (charAt(m_start) == '0' && c != '.')
  {
    if (c == 'x') // hexadecimal
      return readHexa();
    else if (c == 'b') // binary
      return readBinary();
    else if (Tokenizer::isDigit(c))// octal
      return readOctal();
    else // it is zero
      return write(TokenType::OctalLiteral);
  }

  return readDecimal();
}

void Tokenizer::readHexa()
{
  const char x = readChar();
  assert(x == 'x');

  if (atEnd())  // input ends with '0x' -> error
    return write(TokenType::Invalid);

  while (!atEnd() && Tokenizer::isHexa(peekChar()))
    readChar();

  if (pos() - m_start == 2) // e.g. 0x+
    return write(TokenType::Invalid);
  
  return write(TokenType::HexadecimalLiteral);
}

void Tokenizer::readOctal()
{
  while (!atEnd() && Tokenizer::isOctal(peekChar()))
    readChar();

  return write(TokenType::OctalLiteral);
}

void Tokenizer::readBinary()
{
  const char b = readChar();
  assert(b == 'b');

  if (atEnd())  // input ends with '0b' -> error
    return write(TokenType::Invalid);

  while (!atEnd() && Tokenizer::isBinary(peekChar()))
    readChar();

  return write(TokenType::BinaryLiteral);
}

void Tokenizer::readDecimal()
{
  // Reading decimal numbers
  // eg. : 25
  //       3.14
  //       3.14f
  //       100e100
  //       6.02e23
  //       6.67e-11

  while (!atEnd() && Tokenizer::isDigit(peekChar()))
    readChar();

  if (atEnd())
    return write(TokenType::IntegerLiteral);

  bool is_decimal = false;

  if (peekChar() == '.')
  {
    readChar();
    is_decimal = true;

    while (!atEnd() && Tokenizer::isDigit(peekChar()))
      readChar();

    if (atEnd())
      return write(TokenType::DecimalLiteral);
  }

  if (peekChar() == 'e')
  {
    readChar();
    is_decimal = true;

    if (atEnd())
      return write(TokenType::Invalid);

    if (peekChar() == '+' || peekChar() == '-')
    {
      readChar();
      if (atEnd())
        return write(TokenType::Invalid);
    }

    while (!atEnd() && Tokenizer::isDigit(peekChar()))
      readChar();

    if (atEnd())
      return write(TokenType::DecimalLiteral);
  }


  if (peekChar() == 'f') // eg. 125.f
  {
    readChar();
    is_decimal = true;
  }
  else
  {
    if (tryReadLiteralSuffix())
      return write(TokenType::UserDefinedLiteral);
  }

  return write(is_decimal ? TokenType::DecimalLiteral : TokenType::IntegerLiteral);
}

bool Tokenizer::tryReadLiteralSuffix()
{
  auto cpos = pos();

  if (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || peekChar() == '_'))
    readChar();
  else
    return false;

  while (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || Tokenizer::isDigit(peekChar()) || peekChar() == '_'))
    readChar();

  const bool read = (cpos != pos());
  return read;
}

void Tokenizer::readPreprocessor()
{
  consumeDiscardable();

  if (atEnd() || !isIdentifier(peekChar()))
    return write(TokenType::Invalid);

  while (!atEnd() && isIdentifierOrDigit(peekChar()))
    readChar();

  write(TokenType::Preproc);

  if (this->output.back().text() == "#include")
  {
    consumeDiscardable();
    m_start = pos();

    if (atEnd() || (peekChar() != '<' && peekChar() != '"'))
      return;

    char c = readChar();
    c = c == '<' ? '>' : '"';

    while (!atEnd() && peekChar() != c)
      readChar();

    if (atEnd())
      return write(TokenType::Invalid);

    readChar();

    return write(TokenType::Include);
  }
}

void Tokenizer::readIdentifier()
{
  while (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || Tokenizer::isDigit(peekChar()) || peekChar() == '_'))
    readChar();

  return write(identifierType(m_start, pos()));
}


struct Keyword {
  const char *name;
  TokenType toktype;
};

const Keyword l2k[] = {
  { "do", TokenType::Do },
  { "if", TokenType::If },
};

const Keyword l3k[] = {
  { "for", TokenType::For },
  { "int", TokenType::Int },
  { "try", TokenType::Try },
};

const Keyword l4k[] = {
  { "auto", TokenType::Auto },
  { "bool", TokenType::Bool },
  { "case", TokenType::Case },
  { "char", TokenType::Char },
  { "else", TokenType::Else },
  { "enum", TokenType::Enum },
  { "this", TokenType::This },
  { "true", TokenType::True },
  { "void", TokenType::Void },
};

const Keyword l5k[] = {
  { "break", TokenType::Break },
  { "catch", TokenType::Catch },
  { "class", TokenType::Class },
  { "const", TokenType::Const },
  { "false", TokenType::False },
  { "final", TokenType::Final },
  { "float", TokenType::Float },
  { "throw", TokenType::Throw },
  { "using", TokenType::Using },
  { "while", TokenType::While },
};

const Keyword l6k[] = {
  { "delete", TokenType::Delete },
  { "double", TokenType::Double },
  { "export", TokenType::Export },
  { "friend", TokenType::Friend },
  { "import", TokenType::Import },
  { "inline", TokenType::Inline },
  { "public", TokenType::Public },
  { "return", TokenType::Return },
  { "static", TokenType::Static },
  { "struct", TokenType::Struct },
  { "typeid", TokenType::Typeid },
};

const Keyword l7k[] = {
  { "default", TokenType::Default },
  { "mutable", TokenType::Mutable },
  { "private", TokenType::Private },
  { "typedef", TokenType::Typedef },
  { "virtual", TokenType::Virtual },
};

const Keyword l8k[] = {
  { "continue", TokenType::Continue },
  { "explicit", TokenType::Explicit },
  { "noexcept", TokenType::Noexcept },
  { "operator", TokenType::Operator },
  { "override", TokenType::Override },
  { "template", TokenType::Template },
  { "typename", TokenType::Typename },
};

const Keyword l9k[] = {
  { "constexpr", TokenType::Constexpr },
  { "namespace", TokenType::Namespace },
  { "protected", TokenType::Protected },
};

static TokenType findKeyword(const Keyword * keywords, size_t arraySize, const char *str, size_t length)
{
  for (int i(0); i < arraySize; ++i) {
    if (std::memcmp(keywords[i].name, str, length) == 0)
      return keywords[i].toktype;
  }
  return TokenType::UserDefinedName;
}

TokenType Tokenizer::identifierType(size_t begin, size_t end) const
{
  const char *str = m_chars + begin;
  const size_t l = end - begin;

  switch (l) {
  case 1:
    return TokenType::UserDefinedName;
  case 2:
    return findKeyword(l2k, sizeof(l2k) / sizeof(Keyword), str, l);
  case 3:
    return findKeyword(l3k, sizeof(l3k) / sizeof(Keyword), str, l);
  case 4:
    return findKeyword(l4k, sizeof(l4k) / sizeof(Keyword), str, l);
  case 5:
    return findKeyword(l5k, sizeof(l5k) / sizeof(Keyword), str, l);
  case 6:
    return findKeyword(l6k, sizeof(l6k) / sizeof(Keyword), str, l);
  case 7:
    return findKeyword(l7k, sizeof(l7k) / sizeof(Keyword), str, l);
  case 8:
    return findKeyword(l8k, sizeof(l8k) / sizeof(Keyword), str, l);
  case 9:
    return findKeyword(l9k, sizeof(l9k) / sizeof(Keyword), str, l);
  default:
    break;
  }

  return TokenType::UserDefinedName;
}

void Tokenizer::readStringLiteral()
{
  while (!atEnd() && peekChar() != '"')
  {
    if (peekChar() == '\\')
    {
      readChar();
      if (!atEnd())
        readChar();
    }
    else if (peekChar() == '\n')
    {
      return write(TokenType::Invalid);
    }
    else
    {
      readChar();
    }
  }

  if(atEnd())
    return write(TokenType::Invalid);

  assert(peekChar() == '"');
  readChar();

  if (tryReadLiteralSuffix())
    return write(TokenType::UserDefinedLiteral);
  
  return write(TokenType::StringLiteral);
}

void Tokenizer::readCharLiteral()
{
  if(atEnd())
    return write(TokenType::Invalid);

  readChar();

  if (atEnd())
    return write(TokenType::Invalid);

  if(ctype(readChar()) != SingleQuote)
    return write(TokenType::Invalid);

  return write(TokenType::StringLiteral);
}

void Tokenizer::readFromPunctuator(char p)
{
  if (p == '/')
  {
    if (atEnd())
      return write(TokenType::Div);
    if (peekChar() == '/')
      return readSingleLineComment();
    else if (peekChar() == '*')
      return readMultiLineComment();
    else
      return readOperator();
  }
  else if (p == '#')
  {
    return readPreprocessor();
  }
  
  return readOperator();
}


void Tokenizer::readColonOrColonColon()
{
  if (atEnd())
    return write(TokenType::Colon);

  if (peekChar() == ':')
  {
    readChar();
    return write(TokenType::ScopeResolution);
  }

  return write(TokenType::Colon);
}


struct OperatorLexeme {
  const char *name;
  TokenType toktype;
};


const OperatorLexeme l1op[] = {
  { "+", TokenType::Plus },
  { "-", TokenType::Minus },
  { "!", TokenType::LogicalNot },
  { "~", TokenType::BitwiseNot },
  { "*", TokenType::Mul },
  { "/", TokenType::Div },
  { "%", TokenType::Remainder },
  { "<", TokenType::Less },
  { ">", TokenType::GreaterThan },
  { "&", TokenType::BitwiseAnd },
  { "^", TokenType::BitwiseXor },
  { "|", TokenType::BitwiseOr },
  { "=", TokenType::Eq },
};

const OperatorLexeme l2op[] = {
  { "++", TokenType::PlusPlus },
  { "--", TokenType::MinusMinus },
  { "<<", TokenType::LeftShift },
  { ">>", TokenType::RightShift },
  { "<=", TokenType::LessEqual },
  { ">=", TokenType::GreaterThanEqual },
  { "==", TokenType::EqEq },
  { "!=", TokenType::Neq },
  { "&&", TokenType::LogicalAnd },
  { "||", TokenType::LogicalOr },
  { "*=", TokenType::MulEq },
  { "/=", TokenType::DivEq },
  { "%=", TokenType::RemainderEq },
  { "+=", TokenType::AddEq },
  { "-=", TokenType::SubEq },
  { "&=", TokenType::BitAndEq },
  { "|=", TokenType::BitOrEq },
  { "^=", TokenType::BitXorEq },
};


const OperatorLexeme l3op[] = {
  { "<<=", TokenType::LeftShiftEq },
  { ">>=", TokenType::RightShiftEq },
};


static TokenType findOperator(const OperatorLexeme * ops, size_t arraySize, const char *str, size_t length)
{
  for (int i(0); i < arraySize; ++i) {
    if (std::memcmp(ops[i].name, str, length) == 0)
      return ops[i].toktype;
  }
  return TokenType::Invalid;
}


TokenType Tokenizer::getOperator(size_t begin, size_t end) const
{
  const char *str = m_chars + begin;
  const size_t l = end - begin;

  switch (l) {
  case 1:
    return findOperator(l1op, sizeof(l1op) / sizeof(OperatorLexeme), str, l);
  case 2:
    return findOperator(l2op, sizeof(l2op) / sizeof(OperatorLexeme), str, l);
  case 3:
    return findOperator(l3op, sizeof(l3op) / sizeof(OperatorLexeme), str, l);
  default:
    break;
  }

  return TokenType::Invalid;
}

void Tokenizer::readOperator()
{
  TokenType op = getOperator(m_start, pos());

  if (op == TokenType::Invalid)
    return write(TokenType::Invalid);
  
  while (!atEnd())
  {
    size_t p = pos();
    readChar();
    TokenType candidate = getOperator(m_start, pos());

    if (candidate == TokenType::Invalid)
    {
      --m_pos;
      break;
    }
    else
    {
      op = candidate;
    }
  }

  return write(op);
}

void Tokenizer::readSingleLineComment()
{
  readChar(); // reads the second '/'

  while (!atEnd() && peekChar() != '\n')
    readChar();

  return write(TokenType::SingleLineComment);
}

void Tokenizer::createLongComment()
{
  state = State::LongComment;
  return write(TokenType::MultiLineComment);
}

void Tokenizer::readMultiLineComment()
{
  if(state == State::Default)
    readChar(); // reads the '*' after opening '/'

  do {
    while (!atEnd() && peekChar() != '*')
      readChar();

    if (atEnd())
      return createLongComment();

    assert(peekChar() == '*');
    readChar(); // reads the '*'

    if (atEnd())
      return createLongComment();

  } while (peekChar() != '/');

  readChar(); // reads the closing '/'
  state = State::Default;
  return write(TokenType::MultiLineComment);
}

} // namespace cpptok
