// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPTOK_TOKEN_H
#define CPPTOK_TOKEN_H

#include "cpptok/cpptok-defs.h"

#include <string>
#include <string_view>

namespace cpptok
{

using std::string_view;

class TokenCategory
{
public:
  enum Value
  {
    Punctuator = 0x010000,
    Literal = 0x020000,
    OperatorToken = 0x040000,
    Identifier = 0x080000,
    Keyword = 0x100000 | Identifier,
  };
};

class TokenType
{
public:
  enum Value
  {
    Invalid = 0,
    // literals 
    IntegerLiteral = TokenCategory::Literal | 1,
    DecimalLiteral,
    BinaryLiteral,
    OctalLiteral,
    HexadecimalLiteral,
    // do we need to create a CharLiteral 
    StringLiteral,
    // punctuators 
    LeftPar = TokenCategory::Punctuator | 7,
    RightPar,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    Semicolon,
    Colon,
    Dot,
    QuestionMark,
    SlashSlash,
    SlashStar,
    StarSlash,
    // keywords 
    Auto = TokenCategory::Keyword | 20,
    Bool,
    Break,
    Case,
    Catch,
    Char,
    Class,
    Const,
    Constexpr,
    Continue,
    Default,
    Delete,
    Do,
    Double,
    Else,
    Enum,
    Explicit,
    Export,
    False,
    Final,
    Float,
    For,
    Friend,
    If,
    Import,
    Inline,
    Int,
    Mutable,
    Namespace,
    Noexcept,
    Operator,
    Override,
    Private,
    Protected,
    Public,
    Return,
    Static,
    Struct,
    Template,
    This,
    Throw,
    True,
    Try,
    Typedef,
    Typeid,
    Typename,
    Using,
    Virtual,
    Void,
    While,
    //Operators
    ScopeResolution = TokenCategory::OperatorToken | 68,
    PlusPlus,
    MinusMinus,
    Plus,
    Minus,
    LogicalNot,
    BitwiseNot,
    Mul,
    Div,
    Remainder,
    LeftShift,
    RightShift,
    Less,
    GreaterThan,
    LessEqual,
    GreaterThanEqual,
    EqEq,
    Neq,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    LogicalAnd,
    LogicalOr,
    Eq,
    MulEq,
    DivEq,
    AddEq,
    SubEq,
    RemainderEq,
    LeftShiftEq,
    RightShiftEq,
    BitAndEq,
    BitOrEq,
    BitXorEq,
    Comma,
    // Misc
    UserDefinedName = TokenCategory::Identifier | 103,
    UserDefinedLiteral = TokenCategory::Literal | 104,
    SingleLineComment = 102,
    LeftRightPar,
    LeftRightBracket,
    //perhaps it would be better to have two tokens for
    //multiline comments : an opening token and a  closing one
    MultiLineComment,
    //alias
    Ampersand = BitwiseAnd,
    Ref = Ampersand,
    RefRef = LogicalAnd,
    LeftAngle = Less,
    RightAngle = GreaterThan,
    LeftLeftAngle = LeftShift,
    RightRightAngle = RightShift,
    Tilde = BitwiseNot,
    Asterisk = Mul,
    Star = Asterisk,
    Zero = OctalLiteral, // Zero is an octal literal of length 1
  };

private:
  Value m_value = Invalid;

public:
  TokenType() = default;
  TokenType(Value v) : m_value(v) { }
  

  Value value() const { return m_value; }
};

inline bool operator==(const TokenType& lhs, const TokenType& rhs)
{
  return lhs.value() == rhs.value();
}

inline bool operator!=(const TokenType& lhs, const TokenType& rhs)
{
  return lhs.value() != rhs.value();
}

class Token
{
private:
  TokenType type_;
  string_view str_;

public:
  Token();
  Token(const Token & ) = default;
  ~Token() = default;

  Token(TokenType t, string_view str)
    : type_(t), str_(str)
  {

  }

  bool isValid() const { return type_ != TokenType::Invalid; }

  TokenType type() const { return type_; }
  string_view text() const { return str_; }
  std::string to_string() const { return std::string(str_.begin(), str_.end()); }

  bool isOperator() const { return type_.value() & TokenCategory::OperatorToken; }
  bool isIdentifier() const { return type_.value() & TokenCategory::Identifier; }
  bool isKeyword() const { return (type_.value() & TokenCategory::Keyword) == TokenCategory::Keyword; }
  bool isLiteral() const { return type_.value() & TokenCategory::Literal; }

  bool operator==(const Token& other) const { return type_ == other.type_ && other.str_ == str_; }
  bool operator!=(const Token & other) const { return !operator==(other); }
  bool operator==(TokenType tok) const { return type_ == tok; }
  bool operator!=(TokenType tok) const { return !operator==(tok); }

  Token & operator=(const Token&) = default;
};

inline Token::Token()
  : type_(TokenType::Invalid), str_()
{

}

} // namespace cpptok

#endif // CPPTOK_TOKEN_H
