
# `cpptok`

This is `cpptok`, a fast and easy to use line tokenizer for C++.

## Getting started

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Using the tokenizer

Depending on your need, include one of the two following headers:

```cpp
#include <cpptok/tokenizer.h> // for tokenizing lines of code
#include <cpptok/token.h> // if you only need to manipulate tokens
```

The `Tokenizer` class provides a simple interface to tokenize a source file 
line by line.

```cpp
cpptok::Tokenizer lexer;
lexer.tokenize(" int n = 5; ");
```

The resulting tokens are written in the `output` member of the tokenizer.
The `std::string_view` class is used for storing the text of each token;
tokens are therefore cheap to copy but shouldn't outlive the original string.

```cpp
for (cpptok::Token t : lexer.output)
  std::cout << t.text() << "\n";
```

The above would produce the following output:

```txt
int
n
=
5
;
```

The `Tokenizer` class maintains minimal state to be able to parse consecutive lines.

```cpp
cpptok::Tokenizer lexer;
lexer.tokenize(" /* long comments ");
lexer.tokenize(" are supported ! */ ");
```
