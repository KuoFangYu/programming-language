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



