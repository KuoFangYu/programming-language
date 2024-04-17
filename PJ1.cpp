# include <iostream>
# include <vector>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <string>        // c_str,length
# include <math.h>
# include <iomanip>

using namespace std;


struct Token_data {
  string content ;
  string type ;
  int line ;
  bool is_new_line ;
};


struct Container {
  string name ;
  double value ;
  string type ;
  bool is_float ;
};


class Pj1 {
  
  public :
  vector< vector< Token_data > > m_all_token ;
  vector< Container > m_variable ;
  bool m_is_float ;
  int m_left ;
  int m_right ;
  
  bool IsOperator( char ch ) {
    if ( !Is_a_num( ch ) && !Isletter( ch ) && ch != ' ' && ch != '\t' &&
         ch != '\n' && ch != '.' && ch != '_' ) {
      return true ;
    } // if()
  
    return false ;
  
  } // IsOperator()
  
  bool Is_first( string str ) {
    if ( str != "+" && str != "-"  && str != "*" && str != "/" && str != "(" && str != ")" 
         && str != ":=" && str != ";" && str != "<" && str != ">" && str != "=" && str != "<=" 
         && str != ">=" && str != ":" && str != "." ) {
      return true ;
    } // if()
  
    return false ;
  
  } // Is_first()
  
  void Is_special_sign( char ch, string &str, vector < Token_data > tokens, int &line, bool &newline ) {
    char ch2 ; 
    str = ch ;
    ch2 = cin.peek() ;
  
    if ( ( ch == ':' && ch2 == '=' ) || ( ch == '<' && ch2 == '>' ) 
         || ( ch == '<' && ch2 == '<' ) || ( ch == '>' && ch2 == '>' ) 
         || ( ch == '<' && ch2 == '=' ) || ( ch == '>' && ch2 == '=' ) 
         || ( ch == '=' && ch2 == '=' ) ) {
      cin.get( ch2 ) ;
      str += ch2 ;
    } // if()
  
    else if ( ch == '/' && ch2 == '/' ) {
      cin.get( ch2 ) ;
      str = "//" ;
      newline = true ;
      
      while ( ch2 != '\n' ) 
        cin.get( ch2 ) ; 
      
      for ( int i = 0; i < tokens.size() ; i++ ) {
        if ( tokens[i].line == line ) {
          line++ ;
          i = tokens.size() ;
        } // if()
      } // for()
      
    } // else if()
    
    
      
  } // Is_special_sign()
  
  
  void Push( vector< Token_data > &tokens, Token_data &atoken, bool &newline ) { // 
    if ( newline ) 
      atoken.is_new_line = true ; 
      
    newline = false ;
    tokens.push_back( atoken ) ;
    atoken.content.clear() ;
    atoken.type.clear() ;
    atoken.is_new_line = false ;
  } // Push()
  
  
  void Get_token() {
    bool newline = true ;
    bool isdone = false ;
    bool nextline = false ; 
    Token_data atoken ;
    vector < Token_data > tokens ;
    char ch ;
    
    while ( isdone == false ) {
      atoken.line = 1 ;  // 紀錄每個指令開始的行數 
      bool is_double = false ;
      bool is_int = false ;
      while ( nextline == false ) {     
        cin.get( ch ) ;
      
        if ( cin.eof() ) {
          isdone = true ;
          nextline = true ;
          if ( !atoken.content.empty() )
            Push( tokens, atoken, newline ) ; 
        } // if()
        
        else if ( ch == ' ' || ch == '\t' || ch == '\n' ) {    // 遇到換行、空格等符號將1個token push進tokens 
          if ( !atoken.content.empty() ) {  
            Push( tokens, atoken, newline ) ;      
            // atoken.is_new_line = false ;
          } // if()
          
          if ( ch == '\n' ) {   //跳行行數要算 
            newline = true ;
            for ( int i = 0; i < tokens.size() ; i++ ) {
              if ( tokens[i].line == atoken.line ) {
                atoken.line += 1 ;
                i = tokens.size() ;
              } // if()
            } // for()
          } // if()
          
        } // else if()
        else if ( IsOperator( ch ) ) {   //   遇到符號 
          if ( !atoken.content.empty() ) {   
            Push( tokens, atoken, newline ) ;   
            // atoken.is_new_line = false ; 
          } // if()
        
          Is_special_sign( ch, atoken.content, tokens, atoken.line, newline ) ;    //  如果是特殊符號 
          if ( atoken.content == ";" ) {
            if ( tokens.size() == 0 ) {
              char cha = '\0' ;
              while ( cha != '\n' ) 
                cin.get( cha ) ;
                
              newline = true ;
            } // if()
          
            nextline = true ; 
            
          } // if()
          
          if ( atoken.content != "//" ) {  // 註解要跳過 
            if ( atoken.content == ";" && tokens.size() == 0 ) {
              Push( tokens, atoken, newline ) ;   
              newline = true ; 
            } // if()
            else {
              Push( tokens, atoken, newline ) ;
              // atoken.is_new_line = false ;
            } // else()
            
          } // if()
          
          atoken.content.clear() ;
          atoken.type.clear() ;
        } // else if()
        else if ( ch == '.' ) {   //遇到點判斷是不是數字 
          Is_Num( atoken.content, is_double, is_int ) ;
          if ( !is_double && !is_int )
            Push( tokens, atoken, newline ) ;

          atoken.content += ch ;
            
        } // else if()
        else if ( ch == '_' ) {    
          Is_Num( atoken.content, is_double, is_int ) ;
          if ( is_double || is_int ) {
            Push( tokens, atoken, newline ) ;
            atoken.content += ch ;
            Push( tokens, atoken, newline ) ;
          } // if()
          else 
            atoken.content += ch ;
          
        } // else if()
        else if ( Isletter( ch ) ) {
          if ( atoken.content.size() > 0 ) {
            Is_Num( atoken.content, is_double, is_int ) ;
            if ( is_double || is_int ) {
              Push( tokens, atoken, newline ) ;
              // atoken.is_new_line = false ; 
            } // if()

          } // if()
          
          atoken.content += ch ;
        } // else if() 
        else
          atoken.content += ch ;
      
      
         
        if ( tokens.size() > 0 ) {
          string str = tokens[0].content.substr( 0, 4 ) ;
          if ( str == "quit" ) {
            isdone = true ;
            nextline = true ;
            tokens[0].is_new_line = true ; 
            
            for ( int i = tokens.size(); i > 1 ; i-- )
              tokens.pop_back();
              
          } // if()
  
        } // if()
        
       
      } // while()
  
      nextline = false ;
      m_all_token.push_back( tokens ) ;
      tokens.clear() ;
  
    } // while()
  
  } // Get_token()
  
  
  bool Is_a_num( char ch ) {    // is num
    if ( ch >= '0' && ch <= '9' )
      return true ;
    else 
      return false ;
  
  } // Is_a_num()
  
  
  bool Isletter( char ch ) {     //   is word
    if ( ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' ) )
      return true ;
    else 
      return false ;
  } // Isletter()
  
  
  bool Isident( string token ) {
    if ( token == "quit" ) 
      return false ;
    else {
      if ( Isletter( token[0] ) ) {
        for ( int i = 1 ; i < token.size() ; i++ ) {
          if ( !Isletter( token[i] ) && token[i] != '_' && !Is_a_num( token[i] ) )
            return false ;
        } // for()
  
        return true ;
      } // if()  
      else 
        return false ;
  
    } // else()
  } // Isident()
  
  
  bool Issign( string token ) {
    if ( token == "+" || token == "-" )
      return true ;
    else 
      return false ;
  } // Issign()
  
  
  void Is_Num( string token, bool & is_double, bool & is_int ) {
    int count = 0 ; 
  
    for ( int i = 0 ; i < token.size() ; i++ ) {
      if ( !Is_a_num( token[i] ) ) {
        if ( token[i] == '.' )
          is_double = true ;
        else 
          count++ ;
      } // if()
    } // for()
  
    if ( count > 0 )  {
      is_double = false ;
      is_int = false ;
    } // if()
    else if ( is_double == false ) 
      is_int = true ;
      
  
  
  } // Is_Num()
  
  
  bool Find_variable( string str, int & position ) {
    position = -1 ;
    for ( int i = 0 ; i < m_variable.size() ; i++ ) {
      if ( str == m_variable[i].name ) 
        position = i ;
    } // for() 
    
    if ( m_variable.size() == 0 || position == -1 ) 
      return false ;
    else 
      return true ;
    
  } // Find_variable()
  
  
  void Judgment_Type() {  // 判斷類型 
    
    bool is_int = false ;
    bool is_double = false ;
    
    for ( int i = 0 ; i < m_all_token.size() ; i++ ) {
      // m_all_token[i].pop_back() ;
      for ( int j = 0 ; j < m_all_token[i].size() ; j++ ) {
        Is_Num( m_all_token[i][j].content, is_double, is_int ) ;
        
        if ( Isletter( m_all_token[i][j].content[0] ) ) {
          m_all_token[i][j].type = "ident" ;
        } // if()
        else if ( is_int == true )
          m_all_token[i][j].type = "int" ;
          
        else if ( is_double == true )
          m_all_token[i][j].type = "double" ;
          
        else if ( m_all_token[i][j].content[0] == '=' || m_all_token[i][j].content[0] == '>' 
                  || m_all_token[i][j].content[0] == '<' ) {
          if ( m_all_token[i][j].content.size() == 1 )
            m_all_token[i][j].type = "boolean" ;  
            
          else if ( m_all_token[i][j].content.size() == 2 ) {
            if ( m_all_token[i][j].content[0] != '=' && m_all_token[i][j].content[1] == '=' ) 
              m_all_token[i][j].type = "boolean" ;
              
            else if ( m_all_token[i][j].content[0] == '<' && m_all_token[i][j].content[1] == '>' ) 
              m_all_token[i][j].type = "boolean" ;
              
          } // else if()
        } // else if() 
        
        else if ( m_all_token[i][j].content[0] == ':' ) {
          
          if ( m_all_token[i][j].content.size() == 2 ) {
            if ( m_all_token[i][j].content[1] == '=' )
              m_all_token[i][j].type = "statement" ;
          } // if()  
        } // else if()
       
        else if ( m_all_token[i][j].content[0] == '+' || m_all_token[i][j].content[0] == '-' 
                  || m_all_token[i][j].content[0] == '*' || m_all_token[i][j].content[0] == '/' ) {
                   
          m_all_token[i][j].type = "operator" ;
        } // else if()
       
        else if ( m_all_token[i][j].content[0] == '(' )
          m_all_token[i][j].type = "left_Paren" ;
        else if ( m_all_token[i][j].content[0] == ')' )
          m_all_token[i][j].type = "right_Paren" ;
        
        
        
        // cout << m_all_token[i][j].content << " ["  << m_all_token[i][j].type << "] " ;
        is_int = false ;
        is_double = false ;
      } // for()
      
      
    } // for() 
    
  } // Judgment_Type()
  
  
  double Unconditional_carry( double num ) {  // 無條件進位 
    
    int pos = 0 ;
  
    char n[50] ;
    sprintf( n, "%f", num ) ;
    
    for ( int i = 0 ; n[i] != '\0' ; i++ ) {
      
      if ( n[i] == '.' )
        pos = i ;
    } // for()
    
    string str ;
    
    for ( int i = 0 ; n[i] != '\0' && i < pos + 5 ; i++ ) {
      
      str += n[i] ;
      
    } // for()
    
    num = atof( str.c_str() ) ;
    
    
    return num ;
    
  } // Unconditional_carry()

  
  string Char_To_String( char ch[50], string str ) {
    
    for ( int i = 0 ; i < 49 && ch[i] != '\0' ; i++ ) {
      str += ch[i] ;
      
    } // for()
    
    return str ;
  } // Char_To_String()       
  
  void Statement( bool & correct, double & value, int & column, int & row, bool &syntax_error ) {
 	// <Statement> ::= IDENT ':=' <ArithExp> 
    bool exp1Correct ;
    bool exp2Correct ;
    double exp1Value ;
    double exp2Value ;
    Container con ;

    
    row++ ;
     
    if ( m_all_token[column][row].type != "ident" ) {
      correct = false ;
      value = 0.0 ;
      return ;
    } // if()
    
    con.name = m_all_token[column][row].content ;   
    row++ ;												// next token
    
    if ( m_all_token[column][row].type != "statement" ) {  // :=
      
      correct = false ;
      value = 0.0 ;
      
      
      /*
      if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
           && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
           && Is_first(  m_all_token[column][row+1].content ) ) {
        
        cout << "> Unrecognized token with first char : '" ;
        cout << m_all_token[column][row+1].content << "'" << endl ;
        row++ ;
      } // if()
      else {
      */
      cout << "> Unexpected token : '" ; 
      cout << m_all_token[column][row].content << "'" << endl ; 
        
      // } // else
      
        
      return ;
    } // if()
    
    Exp( exp1Correct, exp1Value, column, row, syntax_error );  // arithexp 算術表達式
    
    if ( !exp1Correct ) { 
      correct = false ;
      value = 0.0 ;
      return ;
      
    } // if()
    
    if ( m_all_token[column][row+1].content == "quit" || m_all_token[column][row+1].content != ";" ) {
      syntax_error = true ;
      correct = false ;
      if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"  // 出現奇怪符號 
           && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
           && Is_first( m_all_token[column][row+1].content ) ) {  // is . or <> or := ...
          
        cout << "> Unrecognized token with first char : '" ;
        cout << m_all_token[column][row+1].content << "'" << endl ;
        row++ ;
      } // if()
      else {
        cout << "> Unexpected token : '" ; 
        cout << m_all_token[column][row+1].content << "'" << endl ; 
          
      } // else
        
      return ;
      
    } // if()
           
           
    con.is_float = m_is_float ;    
    if ( !m_is_float ) { 
      int asd = ( int ) exp1Value ;
      exp1Value = ( double ) asd ;
      
    } // if()
      
    
    con.value = exp1Value ;
    value = exp1Value ;
    m_variable.push_back( con ) ;
    correct = true ;
   
  
  } // Statement()
  
  
  void BooleanExp( bool & correct, int & column, int & row, bool & syntax_error ) {
  	// <BooleanExp> ::= <Arithexp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp> 
    bool exp1Correct ;
    bool exp2Correct ;
    double exp1Value ;
    double exp2Value ;
  
    double tolerance = 0.01 ;
    
    Exp( exp1Correct, exp1Value, column, row, syntax_error ); //  Arithexp
    
    if ( !exp1Correct ) {  // problem 
      syntax_error = true ;
      correct = false ;
      return ;
      
    } // if()
    
    row++ ; 		// look next token

    if ( m_all_token[column][row].content != "=" && m_all_token[column][row].content != ">"         // boolean, statement
         && m_all_token[column][row].content != ">=" && m_all_token[column][row].content != "<" 
         && m_all_token[column][row].content != "<=" && m_all_token[column][row].content != "<>" 
         && m_all_token[column][row].content != "!="  ) {
           
      correct = false ;
      return ;
      
    } // if()
    
    
    
    string compare ;
    compare = m_all_token[column][row].content ;
    
    Exp( exp2Correct, exp2Value, column, row, syntax_error );
    
    if ( !exp2Correct ) {
      syntax_error = true ;
      correct = false ;
      return ;
      
    } // if()
    
    row++ ;
    

    if ( m_all_token[column][row].content == "quit" || m_all_token[column][row].content != ";" ) {   // 出現奇怪符號
      syntax_error = true ;
      
      if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
           && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
           && Is_first(  m_all_token[column][row].content ) ) {
          
        cout << "> Unrecognized token with first char : '" ;
        cout << m_all_token[column][row].content << "'" << endl ;
      } // if()
      else {
        cout << "> Unexpected token : '" ; 
        cout << m_all_token[column][row].content << "'" << endl ; 
          
      } // else
        
      return ;
      
    } // if()
    
    double larger ;
    double smaller ;
    
    
    
    exp1Value = Unconditional_carry( exp1Value ) ;    
    exp2Value = Unconditional_carry( exp2Value ) ;
    
    if (  compare == "=" ) {
      if ( exp1Value - exp2Value == 0  ) 
        correct = true ;
      else 
        correct = false ;
    
    } // if()
    
    if (  compare == ">" ) {
      if (  exp1Value > exp2Value ) 
        correct = true ;
      else 
        correct = false ;
      
    } // if()
    
    if ( compare == ">=" ) {
      if (  exp1Value >= exp2Value || exp1Value - exp2Value == 0 ) 
        correct = true ;
      else 
        correct = false ;
      
    } // if()
    
    if (  compare == "<" ) {
      
      if (  exp1Value < exp2Value ) 
        correct = true ;
      else 
        correct = false ;
      
    } // if()

    if ( compare == "<=" ) {
      if (  exp1Value < exp2Value || exp1Value - exp2Value == 0 ) 
        correct = true ;
      else 
        correct = false ;
      
    } // if()

    if ( compare == "<>" ) {
      if (  exp1Value != exp2Value ) 
        correct = true ;
      else 
        correct = false ;
      
    } // if()
        
    
    
  } // BooleanExp()
   

  void Exp( bool & correct, double & value, int & column, int & row, bool &syntax_error ) {   //Arithexp
  // <Exp> ::= <term> {+ <term> | - <term>}
    bool term1Correct ;
    bool term2Correct ;
    double term1Value ;
    double term2Value ;
    int peek = 0 ;
    
    Term( term1Correct, term1Value, column, row, syntax_error ) ;
    
    if ( !term1Correct ) {   // have problem
      correct = false ;
      value = 0.0 ;
      return ;
      
    } // if()
    
    do {
      Token_data peek_token ;
      peek = row + 1 ;
      peek_token = m_all_token[column][peek] ;
      
      if ( peek_token.content == "quit" || peek_token.content == ";" 
           || ( peek_token.content != "+" && peek_token.content != "-" ) ) {   // not + or -
        
        correct = true ;
        value = term1Value ;
        if ( peek_token.content != ";" && peek_token.content != ")" 
             && peek_token.content != ">" && peek_token.content != "=" 
             && peek_token.content != "<" && peek_token.content != ">=" && peek_token.content != "<="
             && peek_token.content != "<>" ) {
          correct = false ;
          
          if ( peek_token.content.size() == 1 && Is_first( peek_token.content ) 
               && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double" 
               && m_all_token[column][row+1].type != "ident" )
            cout << "> Unrecognized token with first char : '" << peek_token.content << "'" << endl ;
          else {
            
            if ( peek_token.content[0] == '=' || peek_token.content[0] == '<' 
                 || peek_token.content[0] == '>' )
              cout << "> Unexpected token : '" << peek_token.content[1] << "'" << endl ; 
            else 
              cout << "> Unexpected token : '" << peek_token.content << "'" << endl ; 
          } // else
          
          row++ ;
        } // if()
        
        if ( peek_token.content == ")" ) {
          m_right++ ;
          
          if ( m_right > m_left ) {
            cout << "> Unexpected token : '" << peek_token.content << "'" << endl ; 
            row++ ;
            correct = false ;
            syntax_error = true ;
          } // if()
        } // if()
        
        return ;
      } // if()
      
      row++ ;
    
    
      Term( term2Correct, term2Value, column, row, syntax_error ) ;
    
    
      if ( !term2Correct ) {
        correct = false ;
        value = 0.0 ;
        return ;
      } // if()
    
     
      correct = true ;
    
    
      if ( peek_token.content == "+" )
        term1Value = term1Value + term2Value ;
        
      else if ( peek_token.content == "-" )
        term1Value = term1Value - term2Value ;
        

    
    } while ( true );
  
  } // Exp()


  void Term( bool & correct, double & value, int & column, int & row, bool &syntax_error ) {
  // <term> ::= <factor> {* <factor> | / <factor>}
 
    bool factor1Correct ;
    bool factor2Correct ;
    double factor1Value ;
    double factor2Value ;
    int peek = 0 ;
   
    Factor( factor1Correct, factor1Value, column, row, syntax_error );
    
    if ( !factor1Correct ) {
      correct = false ;
      syntax_error = true ;
      value = 0.0 ;
      return ;
    } // if()
    
    
    do {
      Token_data peek_token ;
      peek = row + 1 ;
      peek_token = m_all_token[column][peek] ;
      
      if ( peek_token.content == "quit" || peek_token.content == ";"     
           || ( peek_token.content != "*" && peek_token.content != "/" ) ) {
        correct = true ;
        value = factor1Value ;
        
        return ;
      } // if()
    
    
      // there is '*' or '/' behind the first factor
    
      row++ ;
      
      Factor( factor2Correct, factor2Value, column, row, syntax_error ) ;
    
      if ( !factor2Correct ) {
        correct = false ;
        syntax_error = true ;
        value = 0.0 ;
        return ;
      } // if()
      
      correct = true ;
      
      if ( peek_token.content == "*" )
        factor1Value = factor1Value * factor2Value ;
        
      else if ( peek_token.content == "/" ) { 
        if ( factor2Value != 0.0 ) 
          factor1Value = factor1Value / factor2Value ;
        else {
          cout << "> Error" << endl ;
          
          correct = false ;
          return ;
        } // else
        
        
      } // else if() 
    } while ( true ) ;
    
    
  } // Term()
  
  
  void Factor( bool & correct, double & value, int & column, int & row, bool &syntax_error ) { 
  // <factor> ::= NUM | IDENT | (<Exp>)
    int float_error = 0 ;
    int position = 0 ;
    bool expCorrect = false ;
    double expValue = 0.0 ;
    row++ ;
    
    if ( m_all_token[column][row].content == "+" || m_all_token[column][row].content == "-" ) {   // is_num?
      if ( m_all_token[column][row+1].type == "double" || m_all_token[column][row+1].type == "int" ) {
        
        if ( m_all_token[column][row].content[0] == '-' ) {    // 負的做變號and合併
          double num = atof( m_all_token[column][row+1].content.c_str() ) ;
          
          num *= -1 ;
          char ch[50] ;
          string str ;
          sprintf( ch, "%f", num ) ;
          str = Char_To_String( ch, str ) ;
          m_all_token[column][row+1].content = str ;
          m_all_token[column][row+1].is_new_line = m_all_token[column][row].is_new_line ;
        } // if()
      
        m_all_token[column].erase( m_all_token[column].begin() + row ) ;
        
      } // if()
    } // if()
    
    if ( m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"    // not number or 變數 
         && m_all_token[column][row].type != "ident" && m_all_token[column][row].type != "left_Paren" ) {
      if ( m_all_token[column][row].content == "+" || m_all_token[column][row].content == "-" ) { 
        
        if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
             && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
             && Is_first(  m_all_token[column][row+1].content ) ) {
          
          cout << "> Unrecognized token with first char : '" ;
          cout << m_all_token[column][row+1].content << "'" << endl ;
          row++ ;
        } // if()
        else {
          
          cout << "> Unexpected token : '" ; 
          cout << m_all_token[column][row+1].content << "'" << endl ; 
          
          row++ ;
        } // else
      } // if()
      else {
        if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
             && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
             && Is_first(  m_all_token[column][row].content ) ) {
      
          cout << "> Unrecognized token with first char : '" ;
          cout << m_all_token[column][row].content << "'" << endl ;
        } // if()
        else {
        
          cout << "> Unexpected token : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ; 
        } // else
      } // else
      
      correct = false ;
      syntax_error = true ;
      value = 0.0 ;
      return ;
    
    } // if()
    
    if ( m_all_token[column][row].type == "int" ) {
      correct = true ;
      value = atof( m_all_token[column][row].content.c_str() ) ;
      return ;
    
    } // if()
    
    if ( m_all_token[column][row].type == "double" ) {
      string error_str = "" ;
      bool continuous = false ;
      for ( int m = 0 ; m <  m_all_token[column][row].content.size() ; m++ ) {
        
        if ( m_all_token[column][row].content[m] == '.' ) 
          float_error++ ;
        if ( float_error > 1 && float_error < 3 ) {
          error_str += m_all_token[column][row].content[m] ;
          if ( m_all_token[column][row].content[m] == '.' && m_all_token[column][row].content[m-1] == '.' )
            continuous = true ;
        } // if()
      } // for
      
      if ( float_error > 1 ) {
        
        cout << "> Unexpected token : '" << error_str << "'" << endl ;
        correct = false ;
      } // if()
      else if ( m_all_token[column][row].content == "." ) {
        cout << "> Unexpected token : '" ;
        cout << m_all_token[column][row].content << "'" << endl ;
        
      } // else if ()
      else {
        correct = true ;
        value = atof( m_all_token[column][row].content.c_str() ) ;
        m_is_float = true ;
      } // else
      
      float_error = 0 ;
      error_str = '\0' ;
    
      
      return ;
    
    } // if()
     
    if ( m_all_token[column][row].type == "ident" ) {
      
      if ( Find_variable( m_all_token[column][row].content, position ) ) {  // 變數是否宣告過 
        correct = true ;
        value = m_variable[position].value ;
        if ( !m_is_float )
          m_is_float = m_variable[position].is_float ;
      } // if()
      else {   
        if ( m_all_token[column][row].is_new_line ) {
          syntax_error = true ;
          if ( m_all_token[column][row+1].type != "operator" && m_all_token[column][row+1].type != "boolean" 
               && m_all_token[column][row+1].content != ";" 
               && m_all_token[column][row+1].type != "statement" ) {
               
            if ( m_all_token[column][row+1].content.size() == 1 
                 && m_all_token[column][row+1].type != "ident" 
                 && Is_first(  m_all_token[column][row+1].content ) 
                 && m_all_token[column][row+1].type != "int" 
                 && m_all_token[column][row+1].type != "double" ) {
                  
              cout << "> Unrecognized token with first char : '" ;
              cout << m_all_token[column][row+1].content << "'" << endl ;
              row++ ;
            } // if()
            else {
            
              cout << "> Unexpected token : '" ; 
              cout << m_all_token[column][row+1].content << "'" << endl ; 
              row++ ;
            } // else
         
          } // if()
          else {
            cout << "> Undefined identifier : '" << m_all_token[column][row].content << "'" << endl ;
          } // else
        } // if()
        else {
          cout << "> Undefined identifier : '" << m_all_token[column][row].content << "'" << endl ;
          
        } // else 
        
        correct = false ;
      } // else
      
      return ;
    
    } // if()
    
    else if ( m_all_token[column][row].content == "(" ) { 
      m_left++ ;
      Exp( expCorrect, expValue, column, row, syntax_error ) ;   // arithexp 
      
      if ( !expCorrect ) {
        correct = false ;
        syntax_error = true ;
        value = 0.0 ;
        return ; 
      } // if()
      else {
        row++ ;
        
        if ( m_all_token[column][row].content != ")" ) {
          cout << "> Unexpected token : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ; 
          correct = false ;
          value = 0.0 ;
          return ; 
        } // if ()
        else {
          
          correct = true ;
          value = expValue ;
          return ;
          
          
          
        } // else()
        
      } // else()
    
    } // else if()
    
  } // Factor()

};




int main() {
  int qnum = 0 ;
  cin >> qnum ;
  Pj1 start ;
  bool correct ;
  bool statement ;
  bool boolean1 ;
  bool exp ;
  bool syntax_error = false ;
  int column = 0 ;
  int column_error = 0 ;
  int row = -1 ;
  int j = 0 ;
  double value ;
  vector< vector<Token_data> > m_all_token ;
  start.Get_token() ;
  start.Judgment_Type() ;
  bool quit = false ;
  /*
  cout << endl << endl ;
  for ( int i = 0 ; i < start.m_all_token.size() ; i++ ) {
    for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
      cout << start.m_all_token[i][j].content << " ["  << start.m_all_token[i][j].is_new_line << "] " ;
      // cout << " {" << start.m_all_token[i][j].line << "} " ;
    } // for()
    
    cout << endl ;
  } // for() 
  */
  
  
  cout << "Program starts..." << endl ;
  column = 0 ;
  bool first = false ;
  
  while ( column < start.m_all_token.size() && quit != true ) {
    start.m_is_float = false ;
    statement = false ;
    boolean1 = false ;
    exp = false ;
    value = 0.0 ;
    start.m_left = 0 ;
    start.m_right = 0 ;
    int peek = row + 1 ; 
    
      
    if ( syntax_error ) {
      
      while ( !start.m_all_token[column][peek].is_new_line ) {
        row++ ;
        peek = row + 1 ;
        // cout << column << " " << row << endl ;
        if ( peek > start.m_all_token[column].size()-1 ) {
          column++ ;
          row = -1 ;
          peek = row + 1 ;
        } // if()
      } // while
    } // if()
    
    
    if ( start.m_all_token[column][row+1].content == "quit" ) 
      quit = true ;
      
    syntax_error = false ;
      
    
      
    if ( !quit ) {
      
      
      j = row + 1 ;
      
      
      if ( start.m_all_token[column][j].type == "ident" && 
           start.m_all_token[column][j+1].content == ":=" )
        statement = true ; 
      else {
      
        while ( j < start.m_all_token[column].size() ) {
        
          if ( start.m_all_token[column][j].type == "boolean" && !statement )
            boolean1 = true ;
          else if ( start.m_all_token[column][j].type == "operator" && !boolean1 && !statement )
            exp = true ;  
            
          j++ ;  
        } // while()
      } // else
      
      first = false ;
      if ( statement ) {
        
        start.Statement( correct, value, column, row, syntax_error ) ;
        // cout << correct << endl ; 
        if ( correct ) {
          if ( start.m_is_float ) {
            
            cout << "> " << fixed << setprecision( 3 ) << value << endl ;
          } // if()
          else {
            int ans = ( int ) value ;
            cout << "> " << ans << endl ;
          } // else
        } // if()
        else 
          syntax_error = true ;
      } // if()
      
      else if ( boolean1 ) {   //boolesnExp
        // cout << "2" << endl ; 
        start.BooleanExp( correct, column, row, syntax_error ) ;
        
        if ( !syntax_error ) {
        
          if ( correct )  
            cout << "> true" << endl ;
          else 
            cout << "> false" << endl ;
          
        } // if()
      } // else if()
      
      else {
        // cout << "3" << endl ; 
        start.Exp( correct, value, column, row, syntax_error ) ;
        
        if ( correct ) {
          if ( start.m_is_float ) {
            cout << "> " << fixed << setprecision( 3 ) << value << endl ;
          } // if()
          else {
            int ans = ( int ) value ;
            cout << "> " << ans << endl ;
          } // else
        } // if()
        else 
          syntax_error = true ;
      } // else 
        
      // cout << fuck << endl << duck << endl ; 
      // cout << row ;
      if ( syntax_error ) {
        column_error = start.m_all_token[column][row].line ;
        for ( int i = 0 ; i < start.m_all_token[column].size() ; i++ ) {
          if ( column_error < start.m_all_token[column][i].line && first == false ) {
            row = i - 1 ;
            first = true ;
            

          } // if()
          
        
        } // for()
       
      } // if()
      
      if ( !first ) {
        column++ ;
        row = -1 ;
        column_error = 0 ;
      } // if()
      
      
    } // if()
    
  } // while() 
  
  cout << "> Program exits..." << endl ;
  column = 0 ;
  /*
  cout << endl << endl ;
  for ( int i = 0 ; i < start.m_all_token.size() ; i++ ) {
    for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
      cout << start.m_all_token[i][j].content << " ["  << start.m_all_token[i][j].is_new_line << "] " ;
      // cout << " {" << start.m_all_token[i][j].type << "} " ;
    } // for()
    
    cout << endl ;
  } // for() 
  */
  
  
  
  start.m_all_token.clear() ;
  start.m_variable.clear() ;
  
  
} // main()
