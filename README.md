# Architecture DSL (with OpenGL)

The purpose of the given DSL is to create a friendly and functional environment for peopleworking in the field of construction planning.  Although there are many programs dedicated to this,such as AutoCad, none of them offer optimal use of resources and to use them, the programs needto be installed on computers with quite powerful hardware.  DSL will be compact and easy to use,even the person who has no programming experience, after a short training will be able to use it.

<kbd>
<img src="https://github.com/dimatrubca/FloorPlanDSL/blob/master/images/floor_plan.png" width="650">
</kbd>

## Main Components
- Lexer
- Parser
- Abstract Syntax Tree
- Object Model
- Rendering Classes (OpenGL)


## Lexer (FloorPlanDSL/Parsing/Lexer.h)
The lexer transforms the source code into tokens with their type specified.

## Parser (FloorPlanDSL/Parsing/Parser.h)
The parser takes the token produced by lexical analysis as input and generates the parse tree.  In this phase, token arrangements are checked against the source codegrammar, i.e.  the parser checks if the expression made by the tokens is syntactically correct.  Therole of the parser is to take the stream of tokens returned by the lexer, and convert them into anintermediate format, that can be later easily evaluated in order to populate our semantic model.  Forthe proposed DSL, we implemented a recursive descent parser.  It’s a top down operator precedenceparser, in which there is a method for each non terminal symbol from the grammar.  In each method,we call recursively the methods that form the current non terminal, till we reach the terminal symbols.As we parse the source code, we construct the corresponding AST. In that way each parsing method will return a node of the tree.

