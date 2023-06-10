# programming-language
Store all homework for programming languages

## PJ1 

For this project, you must write an interpreter that is capable of interpreting commands that are specified using the following syntax.

### <Command>      ::= <Statement> ';' | <BooleanExp> ';' | <ArithExp> ';' | QUIT
    
### <Statement>    ::= IDENT ':=' <ArithExp>

### <BooleanExp>   ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>

### <ArithExp>     ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>

### <Term>         ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>

### <Factor>       ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'
    
