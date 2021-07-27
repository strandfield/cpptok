// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'cpptok' project
// For conditions of distribution and use, see copyright notice in LICENSE

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cpptok/tokenizer.h"

TEST_CASE("Tokenize keywords", "[cpptok]")
{
  cpptok::Tokenizer lexer;
  lexer.tokenize(" auto bool break case catch char class const ");
  lexer.tokenize("constexpr  continue default delete do double else ");
  lexer.tokenize("   enum explicit export false final float for friend ");
  lexer.tokenize(" if import inline int mutable namespace noexcept operator ");
  lexer.tokenize(" override private protected public return static struct ");
  lexer.tokenize(" template this throw true try typedef typeid typename ");
  lexer.tokenize(" using virtual void while ");
  lexer.tokenize(" goto decltype const_cast static_cast dynamic_cast reinterpret_cast ");
  lexer.tokenize(" static_assert ");

  REQUIRE(lexer.output.size() == 57);

  for (const cpptok::Token& tok : lexer.output)
  {
    REQUIRE(tok.isKeyword());
  }
}

TEST_CASE("Tokenize literals", "[cpptok]")
{
  cpptok::Tokenizer lexer;
  lexer.tokenize(" 0 1 3.14 1.01f 2.5e-10 'a' \"hello\" ");

  REQUIRE(lexer.output.size() == 7);

  for (const cpptok::Token& tok : lexer.output)
  {
    REQUIRE(tok.isLiteral());
  }
}

TEST_CASE("Tokenize simple decl", "[cpptok]")
{
  cpptok::Tokenizer lexer;
  lexer.tokenize(" int n = 5; ");

  REQUIRE(lexer.output.size() == 5);

  REQUIRE(lexer.output[0].text() == "int");
  REQUIRE(lexer.output[0].type() == cpptok::TokenType::Int);

  REQUIRE(lexer.output[1].text() == "n");
  REQUIRE(lexer.output[1].type() == cpptok::TokenType::UserDefinedName);

  REQUIRE(lexer.output[2].text() == "=");
  REQUIRE(lexer.output[2].type() == cpptok::TokenType::Eq);

  REQUIRE(lexer.output[3].text() == "5");
  REQUIRE(lexer.output[3].type() == cpptok::TokenType::IntegerLiteral);

  REQUIRE(lexer.output[4].text() == ";");
  REQUIRE(lexer.output[4].type() == cpptok::TokenType::Semicolon);
}

TEST_CASE("Tokenize multiline comment", "[cpptok]")
{
  cpptok::Tokenizer lexer;
  lexer.tokenize(" foo() /* bar ");

  REQUIRE(lexer.output.size() == 4);
  REQUIRE(lexer.state == cpptok::Tokenizer::LongComment);

  lexer.tokenize(" baz */ toast ");

  REQUIRE(lexer.output.size() == 6);
  REQUIRE(lexer.state == cpptok::Tokenizer::Default);

  REQUIRE(lexer.output[3].text() == "/* bar ");
  REQUIRE(lexer.output[4].text() == " baz */");
}

TEST_CASE("Tokenize preprocessor directive", "[cpptok]")
{
  cpptok::Tokenizer lexer;
  lexer.tokenize("#define FOO");

  REQUIRE(lexer.output.size() == 2);
  REQUIRE(lexer.output[0].type() == cpptok::TokenType::Preproc);
  REQUIRE(lexer.output[0].text() == "#define");
  REQUIRE(lexer.output[1].text() == "FOO");

  lexer.output.clear();

  lexer.tokenize("#include <vector>");

  REQUIRE(lexer.output.size() == 2);
  REQUIRE(lexer.output[0].text() == "#include");
  REQUIRE(lexer.output[1].type() == cpptok::TokenType::Include);
  REQUIRE(lexer.output[1].text() == "<vector>");
}
