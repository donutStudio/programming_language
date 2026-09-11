This is a ground up rewrite of a project I was working on before.

Currently the project has a small expression parser that builds a real AST-like structure for:

- integer literals
- boolean literals
- identifiers
- assignment expressions
- parentheses
- unary `!` and unary `-`
- `*` and `/`
- `+` and `-`
- comparison and equality operators
- `&&` and `||`

Assignment currently supports `identifier = expression` and is parsed as right-associative, so `a = b = 3` becomes `a = (b = 3)`.

The previous version of this project was a prototype interpreter I was making in python, found at [https://github.com/donutStudio/epicProgramminLanguage](https://github.com/donutStudio/epicProgramminLanguage)

## Build

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp parser.cpp statements\expression.cpp statements\operand\literal.cpp statements\operand\identifier.cpp statements\statement.cpp -o expr_app.exe
```

## Test

Run the built-in parser tests:

```powershell
.\expr_app.exe --test
```

This checks both:

- valid expressions parse into the expected tree shape
- invalid expressions are rejected

## Manual verification

Run the parser in REPL mode:

```powershell
.\expr_app.exe
```

Then try inputs like:

```text
1 + 2 * 3
(1 + 2) * 3
!ready || count >= 10
value == 3 && other != 4
```

The program prints the parsed tree, which is useful for verifying precedence and grouping before adding evaluation, statements, and declarations.

## Good next steps

The next layers that usually fit on top of this are:

- assignment expressions
- function call expressions
- member access
- statement parsing
- typed AST nodes
- evaluator or bytecode generation
