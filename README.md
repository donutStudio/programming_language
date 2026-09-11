This is a ground up rewrite of a project I was working on before.

The previous version of this project was a prototype interpreter I was making in python, found at [https://github.com/donutStudio/epicProgramminLanguage](https://github.com/donutStudio/epicProgramminLanguage)

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp parser.cpp statements\expression.cpp statements\operand\literal.cpp statements\operand\identifier.cpp statements\statement.cpp -o expr_app.exe
```

```powershell
.\expr_app.exe --test
```