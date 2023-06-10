# programming-language
Store all homework for programming languages

## PJ1 

For this project, you must write an interpreter that is capable of interpreting commands that are specified using the following syntax.
```
  <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';' 
                | <NOT_IDStartArithExpOrBexp> ';' 
                | QUIT

  <IDlessArithExpOrBexp>  ::= {   '+' <Term>   | '-' <Term> 
                                | '*' <Factor> | '/' <Factor> 
                              }
                              [ <BooleanOperator> <ArithExp> ]

  <BooleanOprator>        ::= '=' | '<>' | '>' | '<' | '>=' | '<='

  <NOT_ID_StartArithExpOrBexp> 
                          ::= <NOT_ID_StartArithExp> 
                              [ <BooleanOperator> <ArithExp> ]

  <NOT_ID_StartArithExp>  ::= <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }
  <NOT_ID_StartTerm>      ::= <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  <NOT_ID_StartFactor>    ::= [ SIGN ] NUM | '(' <ArithExp> ')'
  <ArithExp>              ::= <Term> { '+' <Term> | '-' <Term> }
  <Term>                  ::= <Factor> { '*' <Factor> | '/' <Factor> }
  <Factor>                ::= IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'
```


QUIT is the word 'quit', an IDENT starts with a letter and is followed by digits or letters or underlines (however, an IDENT cannot be the word 'quit' ), 
a NUM is either an integer or a float number (e.g., 35, 7, 43.8, 43., .35, 1.0 and 07) and SIGN is either '+' or '-'.

Apart from the above syntax for "the right input", there are also line-comments. A line-comment is any text that starts with '//' and ends with '\n'. 
Your program should always skip line-comments when they appear. Be aware though. Your program should only skip a line-comment and NOT "the line containing 
the line-comment". If there is any input that appears on the same line PRECEDEING the line-comment, your program should still read -in and process that input.
Finally, your program should terminate when the current input command is 'quit'.



## Pj2
For this project, you are to implement a syntax checker and a pretty printer that supports system-supported functions.

The system-supported functions of OurC system are listed below.

1.  ListAllVariables();          // just the names of the (global) variables, 
                                 // sorted (from smallest to greatest)
2.  ListAllFunctions();          // just the names of the (user-defined) 
                                 // functions, sorted
3.  ListVariable(char name[]);   // the definition of a particular variable

4.  ListFunction(char name[]);   // the definition of a particular function

5.  Done();                      // exit the interpreter
  
  
在error message的部分，共有三種error messages。其例如下：

  Line 3 : unrecognized token with first char : '$'                        // lexical error
  Line 2 : unexpected token : '*'                                         // syntactical error (token recognized)
  Line 5 : undefined identifier : 'bcd'                                  // semantic error (grammar ok)

