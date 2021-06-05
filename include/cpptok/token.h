// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPTOK_TOKEN_H
#define CPPTOK_TOKEN_H

#include "cpptok/cpptok-defs.h"

#include <string>
#include <string_view>

/*!
 * \namespace cpptok
 */

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

 /*!
  * \class TokenType
  * \brief describes the type of a token
  */

class TokenType
{
public:
  /*!
   * \enum Value
   * \brief the possible values
   */
  enum Value
  {
    /*! \value Invalid */
    Invalid = 0,
    // literals 
    /*! \value IntegerLiteral */
    IntegerLiteral = TokenCategory::Literal | 1, 
    /*! \value DecimalLiteral */
    DecimalLiteral, 
    /*! \value BinaryLiteral */
    BinaryLiteral, 
    /*! \value OctalLiteral */
    OctalLiteral, 
    /*! \value HexadecimalLiteral */
    HexadecimalLiteral, 
    // do we need to create a CharLiteral ?
    /*! \value StringLiteral */
    StringLiteral,
    // punctuators 
    /*! \value LeftPar */
    LeftPar = TokenCategory::Punctuator | 7,
    /*! \value RightPar */
    RightPar,
    /*! \value LeftBracket */
    LeftBracket,
    /*! \value RightBracket */
    RightBracket,
    /*! \value LeftBrace */
    LeftBrace,
    /*! \value RightBrace */
    RightBrace,
    /*! \value Semicolon */
    Semicolon,
    /*! \value Colon */
    Colon,
    /*! \value Dot */
    Dot,
    /*! \value QuestionMark */
    QuestionMark,
    /*! \value SlashSlash */
    SlashSlash,
    /*! \value SlashStar */
    SlashStar,
    /*! \value StarSlash */
    StarSlash,
    // keywords 
    /*! \value Auto */
    Auto = TokenCategory::Keyword | 20,
    /*! \value Bool */
    Bool,
    /*! \value Break */
    Break,
    /*! \value Case */
    Case,
    /*! \value Catch */
    Catch,
    /*! \value Char */
    Char,
    /*! \value Class */
    Class,
    /*! \value Const */
    Const,
    /*! \value Constexpr */
    Constexpr,
    /*! \value Continue */
    Continue,
    /*! \value Default */
    Default,
    /*! \value Delete */
    Delete,
    /*! \value Do */
    Do,
    /*! \value Double */
    Double,
    /*! \value Else */
    Else,
    /*! \value Enum */
    Enum,
    /*! \value Explicit */
    Explicit,
    /*! \value Export */
    Export,
    /*! \value False */
    False,
    /*! \value Final */
    Final,
    /*! \value Float */
    Float,
    /*! \value For */
    For,
    /*! \value Friend */
    Friend,
    /*! \value If */
    If,
    /*! \value Import */
    Import,
    /*! \value Inline */
    Inline,
    /*! \value Int */
    Int,
    /*! \value Mutable */
    Mutable,
    /*! \value Namespace */
    Namespace,
    /*! \value Noexcept */
    Noexcept,
    /*! \value Operator */
    Operator,
    /*! \value Override */
    Override,
    /*! \value Private */
    Private,
    /*! \value Protected */
    Protected,
    /*! \value Public */
    Public,
    /*! \value Return */
    Return,
    /*! \value Static */
    Static,
    /*! \value Struct */
    Struct,
    /*! \value Template */
    Template,
    /*! \value This */
    This,
    /*! \value Throw */
    Throw,
    /*! \value True */
    True,
    /*! \value Try */
    Try,
    /*! \value Typedef */
    Typedef,
    /*! \value Typeid */
    Typeid,
    /*! \value Typename */
    Typename,
    /*! \value Using */
    Using,
    /*! \value Virtual */
    Virtual,
    /*! \value Void */
    Void,
    /*! \value While */
    While,
    //Operators
    /*! \value ScopeResolution */
    ScopeResolution = TokenCategory::OperatorToken | 68,
    /*! \value PlusPlus */
    PlusPlus,
    /*! \value MinusMinus */
    MinusMinus,
    /*! \value Plus */
    Plus,
    /*! \value Minus */
    Minus,
    /*! \value LogicalNot */
    LogicalNot,
    /*! \value BitwiseNot */
    BitwiseNot,
    /*! \value Mul */
    Mul,
    /*! \value Div */
    Div,
    /*! \value Remainder */
    Remainder,
    /*! \value LeftShift */
    LeftShift,
    /*! \value RightShift */
    RightShift,
    /*! \value Less */
    Less,
    /*! \value GreaterThan */
    GreaterThan,
    /*! \value LessEqual */
    LessEqual,
    /*! \value GreaterThanEqual */
    GreaterThanEqual,
    /*! \value EqEq */
    EqEq,
    /*! \value Neq */
    Neq,
    /*! \value BitwiseAnd */
    BitwiseAnd,
    /*! \value BitwiseOr */
    BitwiseOr,
    /*! \value BitwiseXor */
    BitwiseXor,
    /*! \value LogicalAnd */
    LogicalAnd,
    /*! \value LogicalOr */
    LogicalOr,
    /*! \value Eq */
    Eq,
    /*! \value MulEq */
    MulEq,
    /*! \value DivEq */
    DivEq,
    /*! \value AddEq */
    AddEq,
    /*! \value SubEq */
    SubEq,
    /*! \value RemainderEq */
    RemainderEq,
    /*! \value LeftShiftEq */
    LeftShiftEq,
    /*! \value RightShiftEq */
    RightShiftEq,
    /*! \value BitAndEq */
    BitAndEq,
    /*! \value BitOrEq */
    BitOrEq,
    /*! \value BitXorEq */
    BitXorEq,
    /*! \value Comma */
    Comma,
    // Misc
    /*! \value UserDefinedName */
    UserDefinedName = TokenCategory::Identifier | 103,
    /*! \value UserDefinedLiteral */
    UserDefinedLiteral = TokenCategory::Literal | 104,
    /*! \value SingleLineComment */
    SingleLineComment = 102,
    /*! \value LeftRightPar */
    LeftRightPar,
    /*! \value LeftRightBracket */
    LeftRightBracket,
    //perhaps it would be better to have two tokens for
    //multiline comments : an opening token and a  closing one
    /*! \value MultiLineComment */
    MultiLineComment,
    /*! \value Preproc */
    Preproc,
    /*! \value Include */
    Include,
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

  /*!
   * \endenum
   */

private:
  Value m_value = Invalid;

public:
  TokenType() = default;

  /*!
   * \fn TokenType(Value v)
   * \param value
   * \brief constructs a token type
   */
  TokenType(Value v) : m_value(v) { }
  
  /*!
   * \fn Value value() const
   * \brief returns the token type's value
   */
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

/*!
 * \endclass
 */

/*!
 * \class Token
 * \brief represents a token
 * 
 * Each token has a c{type()} and a \c{text()}.
 * 
 * Note that the token's text is stored as a string_view. 
 * This makes tokens cheap to copy but requires that the string 
 * that was used to create the token outlives the token.
 */

class Token
{
private:
  TokenType type_;
  string_view str_;

public:
  Token();
  Token(const Token & ) = default;
  ~Token() = default;

  Token(TokenType t, string_view str);

  bool isValid() const;

  TokenType type() const;
  string_view text() const;

  std::string to_string() const;

  bool isOperator() const;
  bool isIdentifier() const;
  bool isKeyword() const;
  bool isLiteral() const;

  bool operator==(const Token& other) const { return type_ == other.type_ && other.str_ == str_; }
  bool operator!=(const Token & other) const { return !operator==(other); }
  bool operator==(TokenType tok) const { return type_ == tok; }
  bool operator!=(TokenType tok) const { return !operator==(tok); }

  Token & operator=(const Token&) = default;
};

/*!
 * \fn Token()
 * \brief builds an invalid token
 *
 * Invalid tokens have \c{type()} TokenType::Invalid.
 */
inline Token::Token()
  : type_(TokenType::Invalid), str_()
{

}

/*!
 * \fn Token(TokenType t, string_view str)
 * \param token type
 * \param token text
 * \brief builds a token from a type and a text
 *
 * The text is stored as a string_view. 
 * The original string should outlive the token to avoid potential 
 * undefined behavior.
 */
inline Token::Token(TokenType t, string_view str)
  : type_(t), str_(str)
{

}

/*!
 * \fn bool isValid() const
 * \brief returns whether the token is valid
 * 
 * Invalid tokens have \c{type()} TokenType::Invalid.
 */
inline bool Token::isValid() const 
{ 
  return type_ != TokenType::Invalid; 
}

/*!
 * \fn TokenType type() const 
 * \brief returns the token's type
 */
inline TokenType Token::type() const 
{ 
  return type_; 
}

/*!
 * \fn string_view text() const
 * \brief returns the token's text
 */
inline string_view Token::text() const 
{ 
  return str_; 
}

/*!
 * \fn std::string to_string() const
 * \brief converts the token to a string
 */
inline std::string Token::to_string() const
{ 
  return std::string(str_.begin(), str_.end()); 
}

/*!
 * \fn bool isOperator() const 
 * \brief returns whether the token is an operator
 */
inline bool Token::isOperator() const 
{ 
  return type_.value() & TokenCategory::OperatorToken; 
}

/*!
 * \fn bool isIdentifier() const 
 * \brief returns whether the token is an identifier
 */
inline bool Token::isIdentifier() const 
{ 
  return type_.value() & TokenCategory::Identifier; 
}

/*!
 * \fn bool isKeyword() const
 * \brief returns whether the token is a keyword
 */
inline bool Token::isKeyword() const
{ 
  return (type_.value() & TokenCategory::Keyword) == TokenCategory::Keyword; 
}

/*!
 * \fn bool isLiteral() const 
 * \brief returns whether the token is a literal
 */
inline bool Token::isLiteral() const 
{ 
  return type_.value() & TokenCategory::Literal; 
}

/*!
 * \endclass
 */

/*!
 * \endnamespace
 */

} // namespace cpptok

#endif // CPPTOK_TOKEN_H
