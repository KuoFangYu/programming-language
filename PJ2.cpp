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
  string type ;
  double value ;
  int size ;
  bool is_float ;
};

 
class PJ2 {
  
  public :
  vector< vector< Token_data > > m_all_token ;
  vector< Container > m_variable ;
  vector< Container > m_fake_variable ;
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
         && str != ">=" && str != ":" && str != "." && str != "{" && str != "}" && str != "!"
         && str != "%" && str != "&" && str != "|" ) {
      return true ;
    } // if()
  
    return false ;
  
  } // Is_first()
    
  void Is_special_sign( char ch, Token_data &atoken, vector < Token_data > tokens,
                        bool &newline ) {
    char ch2 ; 
    atoken.content = ch ;
    ch2 = cin.peek() ;

    if ( ( ch == ':' && ch2 == '=' ) || ( ch == '<' && ch2 == '>' ) 
         || ( ch == '<' && ch2 == '<' ) || ( ch == '>' && ch2 == '>' ) 
         || ( ch == '<' && ch2 == '=' ) || ( ch == '>' && ch2 == '=' ) 
         || ( ch == '=' && ch2 == '=' ) || ( ch == '+' && ch2 == '+' ) 
         || ( ch == '-' && ch2 == '-' ) || ( ch == '!' && ch2 == '=' )
         || ( ch == '|' && ch2 == '|' ) || ( ch == '&' && ch2 == '&' ) ) { 
      cin.get( ch2 ) ;
      atoken.content += ch2 ;
    } // if()
  
    else if ( ch == '/' && ch2 == '/' ) {
      cin.get( ch2 ) ;
      atoken.content = "//" ;
      newline = true ;
      
      ch2 = cin.peek() ;
      while ( ch2 != '\n' ) {
        cin.get( ch2 ) ; 
        ch2 = cin.peek() ;
      } // while()
      
      

    } // else if()
    
    
      
  } // Is_special_sign()
  
  
  string Peek_token( int &count ) {
    string str = "" ;
    string atoken = "" ;
    char ch = cin.peek() ; 
    while ( ch == ' ' || ch == '\n' || ch == '\t' ) {   // read the first one non white space
      ch = cin.get() ; 
      if ( ch == '\n' )
        count++ ;
        
      str = str + ch ;
      ch = cin.peek() ;
    } // while()
    
    if ( Isletter( ch ) ) {     // is letter 
      while ( Isletter( ch ) ) {   // get the word
        ch = cin.get() ; 
        str = str + ch ;
        atoken = atoken + ch ;
        ch = cin.peek() ;
      } // while()      
    } // if()
    
    if ( atoken != "if" && atoken != "else" ) {
      for ( int i = str.size() - 1 ; i > -1 ; i-- )  // put all letter back 
        cin.putback( str[i] ) ;
      count = 0 ;
    } // if()
    
    return atoken ;
    
  } // Peek_token()
  
  void Push( vector< Token_data > &tokens, Token_data &atoken, bool &newline ) {
    if ( newline ) 
      atoken.is_new_line = true ;
      
    newline = false ;
    tokens.push_back( atoken ) ;
    
    atoken.content.clear() ;
    atoken.type.clear() ;
    atoken.is_new_line = false ;
  } // Push()
   
  void Get_token() {
    bool isfunc = false ;
    bool newline = true ;
    bool isdone = false ;
    bool nextline = false ;
    bool end = false ;
    bool isEOF = false ;
    bool d_quote = false ;
    bool fuck = false  ;
    bool quote = false ;
    bool if_else = false ;
    bool change = false ;
    Token_data atoken ;
    vector < Token_data > tokens ;
    char ch ;
    int brackets = 0 ;
    int count = 0 ;
    cin.get( ch ) ;
    atoken.is_new_line = true ;
    
    while ( isdone == false ) {
      
      if ( !isfunc && !if_else ) {
        atoken.line = 1 ;
        brackets = 0 ;
      } // if()
      // cout << "line: " << atoken.line << endl ;
      bool is_double = false ;
      bool is_int = false ;
      
      while ( nextline == false ) {
        cin.get( ch ) ;
        // cout << "'" << ch << "' " ; 
         
        if ( cin.eof() ) {    // is eof one condition is ok
          if ( !atoken.content.empty() )
            Push( tokens, atoken, newline ) ; 
        } // if()
        else if ( ch == ' ' || ch == '\t' || ch == '\n' ) {   // 是不是空白之類的
          if ( !atoken.content.empty() ) {  // 不是空的
            Push( tokens, atoken, newline ) ;     // 丟到序列
            // atoken.is_new_line = false ;
          } // if()

              
          if ( ch == '\n' ) {
            newline = true ;
            atoken.line++ ;
            atoken.is_new_line = true ;
            int x = tokens.size() - 1 ;
            
            // if ()
              
            if ( fuck ) {
              nextline = true ;
              fuck = false ;
              if ( !isfunc && !if_else ) { 
                atoken.line = 1 ;
                brackets = 0 ;
              } // if() 
            } // if()
            
              
            
            if ( x >= 0 && ( tokens[x].content == ";" || tokens[x].content == "{" 
                             || tokens[x].content == "}" ) ) {   
              nextline = true ;

              if ( !isfunc && !if_else ) {
                atoken.line = 1 ;
                brackets = 0 ;
              } // if()  
            } // if()
            
          } // if()
          
        } // else if()
        else if ( IsOperator( ch ) ) {   // 是符號嗎 
          if ( !atoken.content.empty() ) {   // 不是空的
            Push( tokens, atoken, newline ) ;  // 丟進去符號前面的 
          } // if()
        
          Is_special_sign( ch, atoken, tokens, newline ) ;   
          // 是不是兩個符號組的符號 
          
          if ( atoken.content == ";" ) {
            fuck = true ;
            nextline = true ;
            
            if ( if_else && !isfunc ) {
              fuck = false ;
              nextline = false ;

              string str = Peek_token( count ) ;
            
              if ( str == "if" || str == "else" ) {
                Push( tokens, atoken, newline ) ;
                  
                if ( !tokens.empty() )
                  m_all_token.push_back( tokens ) ;
                
                tokens.clear() ;
                
                if ( count > 0 ) {
                  atoken.line += count ;
                  newline = true ;
                  count = 0 ;
                } // if()
                
                if ( str == "if" )
                  atoken.content = "if" ;
                else if ( str == "else" )
                  atoken.content = "else" ;
                
                Push( tokens, atoken, newline ) ;
                
              } // if()
              else if ( str != "if" || str != "else" )
                if_else = false ;
                
                
            } // if()
  
          } // if() 
          
          if ( atoken.content == "{" ) {
            fuck = true ;
            nextline = true ;
            brackets++ ;
          } // if()
          else if ( atoken.content == "}" ) {
            fuck = true ;
            nextline = true ;
            if ( brackets > 0 )
              brackets-- ;
          } // if()
          
          if ( atoken.content.size() == 1 ) {   
            char ctemp = atoken.content[0] ;
            
            if ( atoken.content[0] == '"'  && !d_quote && !quote ) 
              d_quote = true ;
            
            if ( atoken.content == "'"  && !quote && !d_quote ) 
              quote = true ;
              
            if ( quote || d_quote ) {
              Push( tokens, atoken, newline ) ;   // push ' or "
              
              ch = cin.get() ;
              while ( ch != ctemp ) {    
                atoken.content += ch ;
                ch = cin.get() ;
              } // while()
              
              Push( tokens, atoken, newline ) ;   // push content ;
              
              atoken.content += ch ;        // is ' or "
              quote = false ;
              d_quote = false ;
            } // if()
          } // if()
          
          if ( atoken.content != "//" ) {  // 不是註解
            if ( !atoken.content.empty() )   // 不是空的
              Push( tokens, atoken, newline ) ;  // 丟進去符號前面的 
          } // if())
          
          atoken.content.clear() ;
          atoken.type.clear() ;
        } // else if()
        else if ( ch == '.' ) {
          Is_Num( atoken.content, is_double, is_int ) ;
          if ( !is_double && !is_int )
            Push( tokens, atoken, newline ) ;

          atoken.content += ch ;
          fuck = false ;
        } // else if()
        else if ( ch == '_' ) {
          Is_Num( atoken.content, is_double, is_int ) ;
          if ( is_double || is_int ) {
            Push( tokens, atoken, newline ) ;
            atoken.content += ch ;
            Push( tokens, atoken, newline ) ;
          } // if()
          else {
            atoken.content += ch ;
            fuck = false ;
          } // else()
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
          fuck = false ;
        } // else if() 
        else {
          atoken.content += ch ;
          fuck = false ;
        } // else()
      
        if ( brackets > 0 )
          isfunc = true ;
        else
          isfunc = false ;
        
        
        int index = tokens.size() ;
        
        if ( index > 0 && ( tokens[index-1].content == "else" || tokens[index-1].content == "if" ) ) 
          if_else = true ;
          
        if ( isfunc )
          if_else = false ;
          
        if ( index > 2 ) {
          if ( tokens[0].content == "Done" && tokens[1].content == "("
               && tokens[2].content == ")" ) {
            isdone = true ;
            nextline = true ;  
            
          } // if()
          
        } // if()
  
      
      } // while()
      
      nextline = false ;
      if ( !tokens.empty() )
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
  
    
  bool Find_fake_variable( string str, int & position ) {
    position = -1 ;
    for ( int i = 0 ; i < m_fake_variable.size() ; i++ ) {
      if ( str == m_fake_variable[i].name ) 
        position = i ;
    } // for() 
    
    if ( m_fake_variable.size() == 0 || position == -1 ) 
      return false ;
    else 
      return true ;
    
  } // Find_fake_variable()
  
  void Judgment_Type() {
    
    bool is_int = false ;
    bool is_double = false ;
    
    for ( int i = 0 ; i < m_all_token.size() ; i++ ) {
      // m_all_token[i].pop_back() ;
      for ( int j = 0 ; j < m_all_token[i].size() ; j++ ) {
        Is_Num( m_all_token[i][j].content, is_double, is_int ) ;
        
        if ( Isletter( m_all_token[i][j].content[0] ) ) {
          m_all_token[i][j].type = "ident" ;
          if ( m_all_token[i][j].content == "string" || m_all_token[i][j].content == "char" 
               || m_all_token[i][j].content == "int" || m_all_token[i][j].content == "float"
               || m_all_token[i][j].content == "bool" )
            m_all_token[i][j].type = "Definition" ;
            
          if ( m_all_token[i][j].content == "cin" || m_all_token[i][j].content == "cout" )
            m_all_token[i][j].type = "cincout" ;
            
          if ( m_all_token[i][j].content == "ListAllVariables" 
               || m_all_token[i][j].content == "ListVariable" )
            m_all_token[i][j].type = "Function" ;
            
          if ( m_all_token[i][j].content == "if" || m_all_token[i][j].content == "else" )
            m_all_token[i][j].type = "ifelse" ;
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
  
  
  double Unconditional_carry( double num ) {
    
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
  
  void Cin_Cout( bool & correct, int & column, int & row, bool &syntax_error, bool fake ) {
  // <term>       ::= <factor> {* <factor> | / <factor>}
    bool expCorrect ;
    double expValue ;
    row++ ;
    if ( m_all_token[column][row].content == "cin" ) {
      row++ ;
      
      if ( m_all_token[column][row].content != ">>" &&  m_all_token[column][row].content != ";" ) {
        correct = false ;
        if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
             && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
             && Is_first(  m_all_token[column][row].content ) ) {
          
          cout << "> Line " << m_all_token[column][row].line << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row].content << "'" << endl ;
          
        } // if()
        else {
          cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ; 
          
          
        } // else
        
        return ;
      
      } // if()
      
      while ( m_all_token[column][row].content != ";" ) {
        
        Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
        if ( !expCorrect ) {
          correct = false ;
          syntax_error = true ;
          return ;
        } // if()
        
        if ( m_all_token[column][row].type == "ident" ) {
          if ( m_all_token[column][row+1].content == "(" ) {
            Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
            
            if ( !expCorrect ) {
              correct = false ;
              syntax_error = true ;
              return ;
              
            } // if()
          } // if()
        } // if()
        
        row++ ;
        if ( m_all_token[column][row].content != ";" && m_all_token[column][row].content != ">>" ) {
          
          correct = false ;
          if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
               && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
               && Is_first(  m_all_token[column][row].content ) ) {
            
            cout << "> Line " << m_all_token[column][row].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row].content << "'" << endl ;
            
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row].content << "'" << endl ; 
            
            
          } // else
          
          return ;
        
        } // if()
        
        
      } // while()
      
      
    } // if()
    else if ( m_all_token[column][row].content == "cout" ) {
      row++ ;
      if ( m_all_token[column][row].content != "<<" && m_all_token[column][row].content != ";" ) {
        correct = false ;
        if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
             && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
             && Is_first(  m_all_token[column][row].content ) ) {
          
          cout << "> Line " << m_all_token[column][row].line ;
          cout << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row].content << "'" << endl ;
          
        } // if()
        else {
          
          cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ; 
          
          
        } // else
        
        return ;
      
      } // if()
      
      while ( m_all_token[column][row].content != ";" ) {
        
        Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
        if ( !expCorrect ) {
          correct = false ;
          syntax_error = true ;
          return ;
        } // if()
        
        
        if ( m_all_token[column][row].type == "ident" ) {
          
          if ( m_all_token[column][row+1].content == "(" ) {
            Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
            
            if ( !expCorrect ) {
              correct = false ;
              syntax_error = true ;
              return ;
              
            } // if()
          } // if()
        } // if()
        
        row++ ;
        
        if ( m_all_token[column][row].content != ";" && m_all_token[column][row].content != "<<" ) {
          
          correct = false ;
          if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
               && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
               && Is_first(  m_all_token[column][row].content ) ) {
            
            cout << "> Line " << m_all_token[column][row].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row].content << "'" << endl ;
            
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row].content << "'" << endl ; 
            
            
          } // else
          
          return ;
        
        } // if()
        
        
      } // while()
      
    } // else if()
   
    
    
    
    correct = true ;
    
  } // Cin_Cout()
  
  void Definition( bool & correct, int & column, int & row, vector<string> 
                   & ans, vector<string> & old_define, bool fake ) {
    Container con ;
    row++ ;
    con.value = 1.0 ;
    string str  = "" ;
    bool old = false ;
    int position ;
    int g = 0 ;
    if ( m_all_token[column][row].type == "Definition" ) {
      
      if ( m_all_token[column][row].content == "int" ) {
        con.type = "int" ;
      } // if()  
      
      if ( m_all_token[column][row].content == "bool" ) {
        con.type = "bool" ;
      } // if()  
      
      if ( m_all_token[column][row].content == "float" ) {
        con.type = "float" ;
      } // if()  
      
      if ( m_all_token[column][row].content == "char" ) {
        con.type = "char" ;
        
      } // if()  
      
      if ( m_all_token[column][row].content == "string" ) {
        con.type = "string" ;
      } // if()        
    
      row++ ;
      
      if ( m_all_token[column][row].type != "ident" ) {
        correct = false ;
        if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
             && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
             && Is_first(  m_all_token[column][row].content ) ) {
          
          cout << "> Line " << m_all_token[column][row].line ;
          cout << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row].content << "'" << endl ;
          
        } // if()
        else {
          
          cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ; 
          
          
        } // else
        
        for ( int i = 0 ; i < g ; i++ ) 
          m_variable.pop_back() ;
        return ;
      } // if()
        
      while ( m_all_token[column][row].content != ";" ) {
      
        if ( m_all_token[column][row].type != "ident" ) {
          correct = false ;
          if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
               && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
               && Is_first(  m_all_token[column][row].content ) ) {
            
            cout << "> Line " << m_all_token[column][row].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row].content << "'" << endl ;
            
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row].content << "'" << endl ; 
            
            
          } // else
        
          for ( int i = 0 ; i < g ; i++ ) 
            m_variable.pop_back() ;
          return ;
        } // if()
        else {
          if ( !fake ) {
          
            if ( Find_variable( m_all_token[column][row].content, position ) ) {
              old = true ;
              
            } // if()
            else 
              old = false ;
          } // if()
          else {
            if ( Find_fake_variable( m_all_token[column][row].content, position ) ) {
              old = true ;
              
            } // if()
            else 
              old = false ;
            
          } // else
          
          con.name =  m_all_token[column][row].content ;
          str = con.name ;
          
          if ( old )
            old_define.push_back( str ) ;
          else
            ans.push_back( str ) ;
            
          row++ ;
          
          if ( m_all_token[column][row].content == "[" ) {
            row++ ;
            if ( m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double" ) {
            
              correct = false ;
              if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
                   && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
                   && Is_first(  m_all_token[column][row].content ) ) {
                
                cout << "> Line " << m_all_token[column][row].line ;
                cout << " : unrecognized token with first char : '" ;
                cout << m_all_token[column][row].content << "'" << endl ;
                
              } // if()
              else {
                
                cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
                cout << m_all_token[column][row].content << "'" << endl ; 
              } // else
                 
              for ( int i = 0 ; i < g ; i++ ) 
                m_variable.pop_back() ;
              return ;
            } // if()
            
            con.size = atof( m_all_token[column][row].content.c_str() ) ;
            row++ ;
            if ( m_all_token[column][row].content != "]" ) {
            
              correct = false ;
              if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
                   && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
                   && Is_first(  m_all_token[column][row].content ) ) {
                
                cout << "> Line " << m_all_token[column][row].line ;
                cout << " : unrecognized token with first char : '" ;
                cout << m_all_token[column][row].content << "'" << endl ;
                
              } // if()
              else {
                
                cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
                cout << m_all_token[column][row].content << "'" << endl ; 
              } // else
                 
              for ( int i = 0 ; i < g ; i++ ) 
                m_variable.pop_back() ;
              return ;
            } // if()
            
            row++ ;
          } // if()
          else
            con.size = 1 ;
        
          
          
          
          if ( m_all_token[column][row].content != ";" && m_all_token[column][row].content != "," ) {
            correct = false ;
            
            if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
                 && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
                 && Is_first(  m_all_token[column][row].content ) ) {
              
              cout << "> Line " << m_all_token[column][row].line ;
              cout << " : unrecognized token with first char : '" ;
              cout << m_all_token[column][row].content << "'" << endl ;
              
            } // if()
            else {
              
              cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
              cout << m_all_token[column][row].content << "'" << endl ; 
              
              
            } // else
            
            for ( int i = 0 ; i < g ; i++ ) 
              m_variable.pop_back() ;
            return ;
          } // if()
          else { 
            if ( !fake ) {
            
              if ( old ) 
                m_variable[position] = con ;
              else {
              
                m_variable.push_back( con ) ;
                g++ ;
              } // else
            } // if()
            
            if ( fake ) {
            
              if ( old ) 
                m_fake_variable[position] = con ;
              else 
                m_fake_variable.push_back( con ) ;
            } // if()
            
            if ( m_all_token[column][row].content == "," ) 
              row++ ;
            else {
            
              correct = true ;
            } // else
          } // else 
        } // else
      } // while ()
    } // if()
    
    
  } // Definition()

  void If_else( bool & correct, int & column, int & row, bool &syntax_error ) {
    bool statement ;
    bool boolean1 ;
    bool exp ;
    double value = 0.0 ;
    bool fake = true ;
    vector <string> old ;
    bool hif = false ;
    bool recur = true ;
    row++ ;
    
    if ( m_all_token[column][row].content == "if" ) {
      hif = true ;
      row++ ;
      if ( m_all_token[column][row].content == "(" ) {
        while ( recur ) {
          recur = false ;
          
          if ( m_all_token[column][row].content == "(" ) {
            m_left++ ;
            for ( int i = row+1 ; m_all_token[column][i].content == "(" ; i++ ) {
              row = i ;
              m_left++ ;
            } // for
            
            BooleanExp( correct, column, row, syntax_error, fake ) ;
              
            if ( syntax_error ) {
            
              correct = false ;
              
              return ;
            } // if()
            
            
            
            for ( int i = row+1 ; m_all_token[column][i].content == ")" ; i++ ) {
              row = i ;
              m_right++ ;
            } // for
            
            if ( m_right > m_left ) {
              cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
              cout << m_all_token[column][row].content << "'" << endl ; 
              
              correct = false ;
              return ;
            } // if()
            
            if ( m_all_token[column][row+1].content == "&&" || 
                 m_all_token[column][row+1].content == "||" ) {
              recur = true ;
              row += 2 ;
              
            } // if()
          } // if()
        } // while()
        
        int j = row+1 ;
        
        if ( m_all_token[column][row+1].content == "{" ) {
          
          Big_par( correct, column, row, syntax_error ) ;
          if ( !correct ) {
            return ;
          } // if()
          
        } // if()
        else {
        
          if ( m_all_token[column][j].type == "ident" ) {
            for ( int i = j+1 ; i < m_all_token[column].size() ; i++ ) {
              if ( m_all_token[column][i].content == "=" )
                statement = true ; 
            } // for()
          } // if()
          else {
          
            while ( j < m_all_token[column].size() ) {
            
              if ( m_all_token[column][j].type == "boolean" && !statement )
                boolean1 = true ;
              else if ( m_all_token[column][j].type == "operator" && !boolean1 && !statement )
                exp = true ;  
                
              j++ ;  
            } // while()
          } // else
          
          if ( m_all_token[column][row+1].content == "while" ) {
            
            While_a( correct, column, row, syntax_error ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // if()
          else if ( m_all_token[column][row+1].content == "if" ) {
            If_else( correct, column, row, syntax_error ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // else if()
      
          else if ( m_all_token[column][row+1].type == "cincout" ) {
            string ans ;
            Cin_Cout( correct, column, row, syntax_error, fake ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // else if()
          else if ( m_all_token[column][row+1].type == "Definition" ) {
            vector < string > ans ;
            
            Definition( correct, column, row, ans, old, fake ) ;
            
            if ( correct )  {
              ;
            } // if()
            else 
              syntax_error = true ;
            
          } // else if()
          else if ( statement ) {
            
            Statement( correct, value, column, row, syntax_error, fake ) ;
            // cout << correct << endl ; 
            if ( correct ) {
              
                
              ;
             
            } // if()
            else {
              syntax_error = true ;
            } // else
              
          } // else if()
          
          else if ( boolean1 ) {
            // cout << "2" << endl ; 
            BooleanExp( correct, column, row, syntax_error, fake ) ;
            
            if ( !syntax_error ) {
            
              ;
              
            } // if()
          } // else if()
          
          else {
            // cout << "3" << endl ; 
            Exp( correct, value, column, row, syntax_error, fake ) ;
            
            if ( correct ) {
              ;
            } // if()
            else 
              syntax_error = true ;
          } // else 
          
          
          if ( !correct ) {
            return ;
          } // if()
          
          
        } // else
      } // if()
      
    } // if()
    
    column++ ;
    int a = row ;
    row = 0 ;
      
    if ( m_all_token[column][row].content == "else" && hif ) {
      
      int j = row+1 ;
      if ( m_all_token[column][row+1].content == "{" ) {
        
        Big_par( correct, column, row, syntax_error ) ;
        if ( !correct ) {
          return ;
        } // if()
        
      } // if()
      
      else {
       
        if ( m_all_token[column][j].type == "ident" ) {
          for ( int i = j+1 ; i < m_all_token[column].size() ; i++ ) {
            if ( m_all_token[column][i].content == "=" )
              statement = true ; 
          } // for()
        } // if()
        else {
        
          while ( j < m_all_token[column].size() ) {
          
            if ( m_all_token[column][j].type == "boolean" && !statement )
              boolean1 = true ;
            else if ( m_all_token[column][j].type == "operator" && !boolean1 && !statement )
              exp = true ;  
              
            j++ ;  
          } // while()
        } // else
        
        
        if ( m_all_token[column][row+1].type == "cincout" ) {
          string ans ;
          Cin_Cout( correct, column, row, syntax_error, fake ) ;
          if ( correct )  
            ;
          else 
            syntax_error = true ;
          
        } // if()
        else if ( m_all_token[column][row+1].content == "if" ) {
          If_else( correct, column, row, syntax_error ) ;
          if ( correct )  
            ;
          else 
            syntax_error = true ;
          
        } // else if()
        else if ( m_all_token[column][row+1].type == "Definition" ) {
          vector < string > ans ;
          
          Definition( correct, column, row, ans, old, fake ) ;
          
          if ( correct )  {
            ;
          } // if()
          else 
            syntax_error = true ;
          
        } // else if()
        else if ( statement ) {
          
          Statement( correct, value, column, row, syntax_error, fake ) ;
          // cout << correct << endl ; 
          if ( correct ) {
            
              
            ;
           
          } // if()
          else {
            syntax_error = true ;
          } // else
            
        } // else if()
        
        else if ( boolean1 ) {
          // cout << "2" << endl ; 
          BooleanExp( correct, column, row, syntax_error, fake ) ;
          
          if ( !syntax_error ) {
          
            ;
            
          } // if()
        } // else if()
        
        else {
          // cout << "3" << endl ; 
          Exp( correct, value, column, row, syntax_error, fake ) ;
          
          if ( correct ) {
            ;
          } // if()
          else 
            syntax_error = true ;
        } // else 
        
        
        if ( !correct ) {
          return ;
        } // if()
        
        
        
      } // else
        
    } // if()
    else {
      column-- ;
      row = a ;
    } // else 
    
    
    
    
  } // If_else()
  
  void While_a( bool & correct, int & column, int & row, bool &syntax_error ) {
    bool statement ;
    bool boolean1 ;
    bool exp ;
    double value = 0.0 ;
    bool fake = true ;
    vector <string> old ;
    bool hif = false ;
    bool recur = true ;
    row++ ;
    
    if ( m_all_token[column][row].content == "while" ) {
      hif = true ;
      row++ ;
      if ( m_all_token[column][row].content == "(" ) {
        while ( recur ) {
          recur = false ;
          
          if ( m_all_token[column][row].content == "(" ) {
            m_left++ ;
            for ( int i = row+1 ; m_all_token[column][i].content == "(" ; i++ ) {
              row = i ;
              m_left++ ;
            } // for
            
            BooleanExp( correct, column, row, syntax_error, fake ) ;
              
            if ( syntax_error ) {
            
              correct = false ;
              
              return ;
            } // if()
            
            
            
            for ( int i = row+1 ; m_all_token[column][i].content == ")" ; i++ ) {
              row = i ;
              m_right++ ;
            } // for
            
            if ( m_right > m_left ) {
              cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
              cout << m_all_token[column][row].content << "'" << endl ; 
              
              correct = false ;
              return ;
            } // if()
            
            if ( m_all_token[column][row+1].content == "&&" || m_all_token[column][row+1].content == "||" ) {
              recur = true ;
              row += 2 ;
              
            } // if()
          } // if()
        } // while()
        
        int j = row+1 ;
        
        if ( m_all_token[column][row+1].content == "{" ) {
          
          Big_par( correct, column, row, syntax_error ) ;
          if ( !correct ) {
            return ;
          } // if()
          
        } // if()
        else {
        
          if ( m_all_token[column][j].type == "ident" ) {
            for ( int i = j+1 ; i < m_all_token[column].size() ; i++ ) {
              if ( m_all_token[column][i].content == "=" )
                statement = true ; 
            } // for()
          } // if()
          else {
          
            while ( j < m_all_token[column].size() ) {
            
              if ( m_all_token[column][j].type == "boolean" && !statement )
                boolean1 = true ;
              else if ( m_all_token[column][j].type == "operator" && !boolean1 && !statement )
                exp = true ;  
                
              j++ ;  
            } // while()
          } // else
          
          if ( m_all_token[column][row+1].content == "while" ) {
            
            While_a( correct, column, row, syntax_error ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // if()
          else if ( m_all_token[column][row+1].content == "if" ) {
            If_else( correct, column, row, syntax_error ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // else if()
          
          else if ( m_all_token[column][row+1].type == "cincout" ) {
            string ans ;
            Cin_Cout( correct, column, row, syntax_error, fake ) ;
            if ( correct )  
              ;
            else 
              syntax_error = true ;
            
          } // else if()
          else if ( m_all_token[column][row+1].type == "Definition" ) {
            vector < string > ans ;
            
            Definition( correct, column, row, ans, old, fake ) ;
            
            if ( correct )  {
              ;
            } // if()
            else 
              syntax_error = true ;
            
          } // else if()
          else if ( statement ) {
            
            Statement( correct, value, column, row, syntax_error, fake ) ;
            // cout << correct << endl ; 
            if ( correct ) {
              
                
              ;
             
            } // if()
            else {
              syntax_error = true ;
            } // else
              
          } // else if()
          
          else if ( boolean1 ) {
            // cout << "2" << endl ; 
            BooleanExp( correct, column, row, syntax_error, fake ) ;
            
            if ( !syntax_error ) {
            
              ;
              
            } // if()
          } // else if()
          
          else {
            // cout << "3" << endl ; 
            Exp( correct, value, column, row, syntax_error, fake ) ;
            
            if ( correct ) {
              ;
            } // if()
            else 
              syntax_error = true ;
          } // else 
          
          
          if ( !correct ) {
            return ;
          } // if()
          
          
        } // else
      } // if()
      
    } // if()
    
    
    
    
    
    
  } // While_a()
  
  void Big_par( bool & correct, int & column, int & row, bool &syntax_error ) {
    
    row++ ;
    
    if ( m_all_token[column][row].content == "{" ) {
      
      Simulation( correct, column, row, syntax_error ) ;
      
      if ( !correct ) 
        return ;
    } // if()
    
    
    if ( m_all_token[column][row+1].content == "}" ) {
      
      return ;
    } // if()
    else {
      correct = false ;
      return ;
    } // else
    
    
    
    
    
    
    
  } // Big_par()
  
  void Simulation( bool & correct, int & column, int & row, bool & syntax_error ) {
    
    bool statement ;
    bool boolean1 ;
    bool exp ;
    double value = 0.0 ;
    bool fake = true ;
    vector <string> old ;
    if ( row + 1 >= m_all_token[column].size() ) {
      column++ ;
      row = -1 ;
    } // if()
    else {
      ;
    } // else
    
    while ( m_all_token[column][row+1].content != "}" ) {
      statement = false ;
      boolean1 = false ;
      exp = false ;
      
      int j = row+1 ;
      if ( m_all_token[column][row+1].content == "{" ) {
        
        Big_par( correct, column, row, syntax_error ) ;
        if ( !correct ) {
          return ;
        } // if()
        
      } // if()
      
      if ( m_all_token[column][row+1].content != "}" ) {
        
     
        if ( m_all_token[column][j].type == "ident" ) {
          for ( int i = j+1 ; i < m_all_token[column].size() ; i++ ) {
            if ( m_all_token[column][i].content == "=" )
              statement = true ; 
          } // for()
        } // if()
        else {
        
          while ( j < m_all_token[column].size() ) {
          
            if ( m_all_token[column][j].type == "boolean" && !statement )
              boolean1 = true ;
            else if ( m_all_token[column][j].type == "operator" && !boolean1 && !statement )
              exp = true ;  
              
            j++ ;  
          } // while()
        } // else
        
        if ( m_all_token[column][row+1].content == "while" ) {
            
          While_a( correct, column, row, syntax_error ) ;
          if ( correct )  
            ;
          else 
            syntax_error = true ;
          
        } // if()
        else if ( m_all_token[column][row+1].content == "if" ) {
          If_else( correct, column, row, syntax_error ) ;
          if ( correct )  
            ;
          else 
            syntax_error = true ;
          
        } // else if()
        else if ( m_all_token[column][row+1].type == "cincout" ) {
          string ans ;
          Cin_Cout( correct, column, row, syntax_error, fake ) ;
          if ( correct )  
            ;
          else 
            syntax_error = true ;
          
        } // else if()
        else if ( m_all_token[column][row+1].type == "Definition" ) {
          vector < string > ans ;
          
          Definition( correct, column, row, ans, old, fake ) ;
          
          if ( correct )  {
            ;
          } // if()
          else 
            syntax_error = true ;
          
        } // else if()
        else if ( statement ) {
          
          Statement( correct, value, column, row, syntax_error, fake ) ;
          
          // cout << correct << endl ; 
          if ( correct ) {
            
              
            ;
           
          } // if()
          else {
            syntax_error = true ;
          } // else
            
        } // else if()
        
        else if ( boolean1 ) {
          // cout << "2" << endl ; 
          BooleanExp( correct, column, row, syntax_error, fake ) ;
          
          if ( !syntax_error ) {
          
            ;
            
          } // if()
        } // else if()
        
        else {
          
          Exp( correct, value, column, row, syntax_error, fake ) ;
          
          if ( correct ) {
            ;
          } // if()
          else 
            syntax_error = true ;
        } // else 
        
        
        if ( !correct ) {
          return ;
        } // if()
        
        
        
      } // if()
      
      column++ ;
      row = -1 ;
        
      
    } // while()
    
    correct = true ;
    
    
    
    
  } // Simulation()
  
  void Statement( bool & correct, double & value, int & column, int & row, bool &syntax_error, bool fake ) {
 
    bool exp1Correct ;
    bool exp2Correct ;
    double exp1Value ;
    double exp2Value ;
    Container con ;
    int position = 0 ;
    string type = "" ;
    
    
    row++ ;
     
    if ( m_all_token[column][row].type != "ident" ) {
      correct = false ;
      value = 0.0 ;
      return ;
    } // if()
    
    if ( !fake ) {
    
      if ( Find_variable( m_all_token[column][row].content, position ) ) {
        value = m_variable[position].value ;
        type = m_variable[position].type ;
        
      } // if()
      else {
        correct = false ;
        cout << "> Line " << m_all_token[column][row].line << " : undefined identifier : '" ; 
        cout << m_all_token[column][row].content << "'" << endl ; 
          
        row++ ;
        return ;
      } // else
    } // if()
    else {
      if ( Find_fake_variable( m_all_token[column][row].content, position ) ) {
        value = m_fake_variable[position].value ;
        type = m_fake_variable[position].type ;
        
      } // if()
      
      else if ( Find_variable( m_all_token[column][row].content, position ) ) {
        value = m_variable[position].value ;
        type = m_variable[position].type ;
        
      } // else if()
      else {
        correct = false ;
          
        row++ ;
        return ;
      } // else
      
    } // else
    
    
    if (  m_all_token[column][row+1].content == "[" ) {
      row++ ;
      bool expCorrect = false ;
      double expValue = 0.0 ;
      Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
      if ( !expCorrect ) {
        correct = false ;
        syntax_error = true ;
        value = 0.0 ;
        return ; 
      } // if()
      
      row++ ;
      
    } // if()
    
    row++ ;
    if ( m_all_token[column][row].content != "=" 
         && ( m_all_token[column][row].content != "+" || m_all_token[column][row+1].content != "=" )
         && ( m_all_token[column][row].content != "-" || m_all_token[column][row+1].content != "=" )
         && ( m_all_token[column][row].content != "*" || m_all_token[column][row+1].content != "=" )
         && ( m_all_token[column][row].content != "/" || m_all_token[column][row+1].content != "=" ) ) {
      correct = false ;
      value = 0.0 ;
      
       
      if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
           && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
           && Is_first(  m_all_token[column][row].content ) ) {
        
        cout << "> Line " << m_all_token[column][row].line ;
        cout << " : unrecognized token with first char : '" ;
        cout << m_all_token[column][row].content << "'" << endl ;
        row++ ;
      } // if()
      else {
        
        cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
        cout << m_all_token[column][row].content << "'" << endl ; 
        
        row++ ;
      } // else
      
      
      
        
      return ;
    } // if()
    
    bool boolean1 = false ;
    for ( int i = row ; i < m_all_token[column].size() ; i++ ) {
      if ( m_all_token[column][i].content == "?" )
        boolean1 = true ;
    } // for()
    
    
    if ( !boolean1 ) {
      
      if ( ( m_all_token[column][row].content == "+" && m_all_token[column][row+1].content == "=" )
           || ( m_all_token[column][row].content == "-" && m_all_token[column][row+1].content == "=" ) 
           || ( m_all_token[column][row].content == "*" && m_all_token[column][row+1].content == "=" ) 
           || ( m_all_token[column][row].content == "/" && m_all_token[column][row+1].content == "=" ) ) {
        row++ ;
      } // if()
      
      if ( m_all_token[column][row+1].content == ";" ) {
        correct = false ;
        value = 0.0 ;
        
        cout << "> Unexpected token : '" ; 
        cout << m_all_token[column][row+1].content << "'" << endl ; 
          
        return ;
      } // if()
      
      Exp( exp1Correct, exp1Value, column, row, syntax_error, fake ) ;
      
      if ( !exp1Correct ) {
        correct = false ;
        value = 0.0 ;
        return ;
        
      } // if()
      
      if (  m_all_token[column][row+1].content != ";" && m_all_token[column][row+1].content != ")" ) {
        syntax_error = true ;
        correct = false ;
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
          
        return ;
        
      } // if()
             
             
  
        
      
      
      value = exp1Value ;
      correct = true ;
    } // if()
    else {
      
      BooleanExp( correct, column, row, syntax_error, fake ) ;
      
      if ( syntax_error )
        correct = false ;
      else
        correct = true ;
    } // else 
    
  } // Statement()
  
  
  void BooleanExp( bool & correct, int & column, int & row, bool & syntax_error, bool fake ) {
    bool exp1Correct ;
    bool exp2Correct ;
    double exp1Value ;
    double exp2Value ;
  
    double tolerance = 0.01 ;
    
    Exp( exp1Correct, exp1Value, column, row, syntax_error, fake );
    
    if ( !exp1Correct ) {
      syntax_error = true ;
      correct = false ;
      return ;
      
    } // if()
    
    row++ ;
    
    if ( m_all_token[column][row].content == ")" ) {
      
      syntax_error = false ;
      correct = true ;
      return ;
      
    } // if()
    
    if ( m_all_token[column][row].content != "=" && m_all_token[column][row].content != ">"  
         && m_all_token[column][row].content != ">=" && m_all_token[column][row].content != "<" 
         && m_all_token[column][row].content != "<=" && m_all_token[column][row].content != "<>" 
         && m_all_token[column][row].content != "!="  ) {
      syntax_error = true ;
      correct = false ;
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
    
    
    
    string compare ;
    compare = m_all_token[column][row].content ;
    
    Exp( exp2Correct, exp2Value, column, row, syntax_error, fake );
    
    if ( !exp2Correct ) {
      syntax_error = true ;
      correct = false ;
      return ;
      
    } // if()
    
    row++ ;
    

    if ( m_all_token[column][row].content != ")" && m_all_token[column][row].content != ";" &&
         m_all_token[column][row].content != "?" ) {
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
    
    if ( m_all_token[column][row].content == "?" ) {
      Exp( exp1Correct, exp1Value, column, row, syntax_error, fake );
    
      if ( !exp1Correct ) {
        syntax_error = true ;
        correct = false ;
        return ;
      } // if()
        
      row++ ;
      if ( m_all_token[column][row].content == ":" ) {
        Exp( exp1Correct, exp1Value, column, row, syntax_error, fake );
        
        if ( !exp1Correct ) {
          syntax_error = true ;
          correct = false ;
          return ;
        } // if() 
      } // if()
      else {
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
      } // else
      
      if ( m_all_token[column][row+1].content != ";" ) {
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
      
      
    } // if()
    
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
   

  void Exp( bool & correct, double & value, int & column, int & row, bool &syntax_error, bool fake ) {
  // <Exp>        ::= <term> {+ <term> | - <term>}
    bool term1Correct ;
    bool term2Correct ;
    double term1Value ;
    double term2Value ;
    int peek = 0 ;
    
    if ( m_all_token[column][row+1].content == ";" ) {
      correct = true ;
      return ;
    } // if()
      
    Term( term1Correct, term1Value, column, row, syntax_error, fake ) ;
    
    if ( !term1Correct ) {
      correct = false ;
      value = 0.0 ;
      return ;
      
    } // if()
    
     
    
    
    if ( m_all_token[column][row].type == "ident" ) {
      if ( m_all_token[column][row+1].content == "++" ) {
      
        if ( row != 0 ) {
          if ( m_all_token[column][row-1].type == "operator" ) {
            cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '++'" << endl ; 
            correct = false ;
            row ++ ;
            return ;
       
          } // if()
          
        } // if()
        
        if ( m_all_token[column][row+2].content == ";" ) {
          
          correct = true ;
          row ++ ;
          return ;
        } // if()
        else {
          correct = false ;
          if ( m_all_token[column][row+2].content.size() == 1 && m_all_token[column][row+2].type != "ident"
               && m_all_token[column][row+2].type != "int" && m_all_token[column][row+2].type != "double"
               && Is_first(  m_all_token[column][row+2].content ) ) {
            
            cout << "> Line " << m_all_token[column][row+2].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row+2].content << "'" << endl ;
            row += 2 ;
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row+2].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row+2].content << "'" << endl ; 
            
            row += 2 ;
          } // else
          
          return ;
        } // else
       
      } // if()
      
    } // if ()
    
    
    if ( m_all_token[column][row].type == "ident" ) {
      
      if ( m_all_token[column][row+1].content == "--" ) {
      
        if ( row != 0 ) {
          if ( m_all_token[column][row-1].type == "operator" ) {
            cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '--'" << endl ; 
            correct = false ;
            row ++ ;
            return ;
       
          } // if()
          
        } // if()
        
        if ( m_all_token[column][row+2].content == ";" ) {
          correct = true ;
          row ++ ;
          return ;
        } // if()
        else {
          correct = false ;
          if ( m_all_token[column][row+2].content.size() == 1 && m_all_token[column][row+2].type != "ident"
               && m_all_token[column][row+2].type != "int" && m_all_token[column][row+2].type != "double"
               && Is_first(  m_all_token[column][row+2].content ) ) {
            
            cout << "> Line " << m_all_token[column][row+2].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row+2].content << "'" << endl ;
            row++ ;
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row+2].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row+2].content << "'" << endl ; 
            
            row++ ;
          } // else
          
          return ;
        } // else
        
      } // if()
      
    } // if ()
    
    do {
      Token_data peek_token ;
      peek = row + 1 ;
      peek_token = m_all_token[column][peek] ;
      
      if ( peek_token.content == "quit" || peek_token.content == ";" 
           || ( peek_token.content != "+" && peek_token.content != "-" 
                && peek_token.content != "++" && peek_token.content != "--" ) ) {
        
        correct = true ;
        value = term1Value ;
        
        if ( peek_token.content == "(" && m_all_token[column][row].type == "ident" ) {
          bool expCorrect = false ;
          double expValue = 0.0 ;
          
          m_left++ ;
          Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
          
          if ( !expCorrect ) {
            correct = false ;
            syntax_error = true ;
            value = 0.0 ;
            return ; 
          } // if()
          
          
        } // if()
        else if ( peek_token.content != ";" && peek_token.content != ")" && peek_token.content != ">>"
                  && peek_token.content != ">" && peek_token.content != "<<" &&  peek_token.content != ":"
                  && peek_token.content != "<" && peek_token.content != ">=" && peek_token.content != "<="
                  && peek_token.content != "<>" && peek_token.content != "]" && peek_token.content != "?" ) {
                 
          correct = false ;
          if ( ( int ) m_all_token[column][row+1].content[0] == 34 ) {
            cout << "> Line " <<  m_all_token[column][row+1].line << " : unexpected token : '" ; 
            // cout << m_all_token[column][row+1].content <<  m_all_token[column][row+2].content ;
            cout <<  m_all_token[column][row+2].content << "'" << endl ; 
            row += 3 ;
            return ;
          } // if()
          else if ( peek_token.content.size() == 1 && Is_first( peek_token.content ) 
                    && m_all_token[column][row+1].type != "int" 
                    && m_all_token[column][row+1].type != "double" 
                    && m_all_token[column][row+1].type != "ident" ) {
            cout << "> Line " << peek_token.line << " : unrecognized token with first char : '" ;
            cout << peek_token.content << "'" << endl ;
          } // else if()
          else {
            
            if (  peek_token.content[0] == '<' 
                 || peek_token.content[0] == '>' ) {
              cout << "> Line " << peek_token.line << " : unexpected token : '" ; 
              cout << peek_token.content[1] << "'" << endl ; 
            } // if()
            else {
              cout << "> Line " << peek_token.line << " : unexpected token : '" ; 
              cout << peek_token.content << "'" << endl ; 
            } // else
          } // else
          
          row++ ;
        } // else if()
        
        
        
        if ( peek_token.content == ")" ) {
          m_right++ ;
          
          if ( m_right > m_left ) {
            cout << "> Line " << peek_token.line << " : unexpected token : '" ; 
            cout << peek_token.content << "'" << endl ; 
            row++ ;
            correct = false ;
            syntax_error = true ;
          } // if()
        } // if()
        
        return ;
      } // if()
      
      if ( m_all_token[column][row+1].content == "++" ) {
        
        cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ;
        cout << m_all_token[column][row+1].content << "'" << endl ; 
        correct = false ;
        row ++ ;
        return ;
         
    
      } // if()
      
      if ( m_all_token[column][row+1].content == "--" ) {
        
        cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ;
        cout << m_all_token[column][row+1].content << "'" << endl ; 
        correct = false ;
        row ++ ;
        return ;
         
      
      } // if()
      
      if ( m_all_token[column][row+1].content == "-" || m_all_token[column][row+1].content == "+"
           || m_all_token[column][row+1].content == "/" || m_all_token[column][row+1].content == "*" ) {
        if ( m_all_token[column][row+2].content == "=" ) {
          
          cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ;
          cout << m_all_token[column][row+1].content << m_all_token[column][row+2].content ;
          cout << "'" << endl ; 
          correct = false ;
          row ++ ;
          return ;
          
         
        } // if()
      } // if()
      
      row++ ;
    
    
      Term( term2Correct, term2Value, column, row, syntax_error, fake ) ;
    
    
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

  void Term( bool & correct, double & value, int & column, int & row, bool &syntax_error, bool fake ) {
  // <term>       ::= <factor> {* <factor> | / <factor>}
 
    bool factor1Correct ;
    bool factor2Correct ;
    double factor1Value ;
    double factor2Value ;
    int peek = 0 ;
   
    Factor( factor1Correct, factor1Value, column, row, syntax_error, fake );
    
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
           || ( peek_token.content != "*" && peek_token.content != "/" && peek_token.content != "%" ) ) {
        correct = true ;
        value = factor1Value ;
        
        return ;
      } // if()
      
      if ( m_all_token[column][row+1].content == "*" || m_all_token[column][row+1].content == "/" 
           || m_all_token[column][row+1].content == "%" ) {
        if ( m_all_token[column][row+2].content == "=" ) {
          
          correct = true ;
          row ++ ;
          
         
        } // if()
      } // if()
    
      // there is '*' or '/' behind the first factor
    
      row++ ;
      
      Factor( factor2Correct, factor2Value, column, row, syntax_error, fake ) ;
    
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
  
  void Factor( bool & correct, double & value, int & column, int & row, bool &syntax_error, bool fake ) { 
  // <factor>     ::= NUM | IDENT | (<Exp>)
    int float_error = 0 ;
    int position = 0 ;
    bool expCorrect = false ;
    double expValue = 0.0 ;
    row++ ;
    if ( m_all_token[column][row].content == "!" ) {
    
      if ( m_all_token[column][row+1].type == "ident" || m_all_token[column][row+1].type == "int" 
           || m_all_token[column][row+1].type == "double" 
           || ( int ) m_all_token[column][row+1].content[0] == 34 ) {
        row ++ ;
      
      } // if()
      
      correct = true ;
    } // if()
    
    if ( row == 0 || m_all_token[column][row].is_new_line ) {
    
      if ( m_all_token[column][row].content == "++" || m_all_token[column][row].content == "--" ) {
        if ( m_all_token[column][row+1].type == "ident" ) {
        
          
          if ( m_all_token[column][row+2].content == ";" ) {
            
            correct = true ;
            row ++ ;
            return ;
          } // if()
          else {
            correct = false ;
            if ( m_all_token[column][row+2].content.size() == 1 && m_all_token[column][row+2].type != "ident"
                 && m_all_token[column][row+2].type != "int" && m_all_token[column][row+2].type != "double"
                 && Is_first(  m_all_token[column][row+2].content ) ) {
              
              cout << "> Line " << m_all_token[column][row+2].line ;
              cout << " : unrecognized token with first char : '" ;
              cout << m_all_token[column][row+2].content << "'" << endl ;
              row += 2 ;
            } // if()
            else {
              
              cout << "> Line " << m_all_token[column][row+2].line << " : unexpected token : '" ; 
              cout << m_all_token[column][row+2].content << "'" << endl ; 
              
              row += 2 ;
            } // else
            
            return ;
          } // else
         
        } // if()
        else {
          correct = false ;
          if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
               && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
               && Is_first(  m_all_token[column][row+1].content ) ) {
            
            cout << "> Line " << m_all_token[column][row+1].line ;
            cout << " : unrecognized token with first char : '" ;
            cout << m_all_token[column][row+1].content << "'" << endl ;
            row++ ;
          } // if()
          else {
            
            cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ; 
            cout << m_all_token[column][row+1].content << "'" << endl ; 
            
            row++ ;
          } // else
          
          return ;
          
          
        } // else
      } // if()
    } // if()
    
    if ( m_all_token[column][row].content == "+" || m_all_token[column][row].content == "-" ) { 
      if ( m_all_token[column][row+1].type == "double" || m_all_token[column][row+1].type == "int" ) {
        
        if ( m_all_token[column][row].content[0] == '-' ) {
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
      else if ( m_all_token[column][row+1].content == ";" ) {
        correct = false ;
        cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ; 
        cout << m_all_token[column][row+1].content << "'" << endl ; 
        return ;
      } // else if()
      else if ( m_all_token[column][row+1].type == "ident" || 
                m_all_token[column][row+1].type == "left_Paren" ) {
                  
        Factor( correct, value, column, row, syntax_error, fake ) ;
  
        return ;
      } // else if()
      else if ( ( int ) m_all_token[column][row+1].content[0] == 34 || 
                ( int ) m_all_token[column][row+1].content[0] == 39 ) {
                  
        Factor( correct, value, column, row, syntax_error, fake ) ;
  
        return ;
      } // else if()
      
      else if ( m_all_token[column][row+1].content == "+" || m_all_token[column][row+1].content == "-" ) {
        if ( m_all_token[column][row+2].content == ";" ) {
        
          correct = false ;
          cout << "> Line " << m_all_token[column][row+2].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row+2].content << "'" << endl ; 
          return ;
        } // if()
      } // else if()
      
    } // if()
    
    else if ( ( int ) m_all_token[column][row].content[0] == 34 ) { // "
      
      // cout << "f" ;
      row++ ;
      
      while ( ( int ) m_all_token[column][row].content[0] != 34 ) {
        row++ ;
        
        if ( row + 1 > m_all_token[column].size() ) {
          column++ ;
          row = 0 ;
          
        } // if()
      } // while()
      
      if ( m_all_token[column][row+1].content != ";" && m_all_token[column][row+1].content != "+"
           && m_all_token[column][row+1].content != ">>" && m_all_token[column][row+1].content != "<<" 
           && m_all_token[column][row+1].content != ")" ) {
        correct = false ;
        if ( ( int ) m_all_token[column][row+1].content[0] == 34 ) {
          cout << "> Line " <<  m_all_token[column][row+1].line << " : unexpected token : '" ; 
          // cout << m_all_token[column][row+1].content <<  m_all_token[column][row+2].content ;
          cout <<  m_all_token[column][row+2].content << "'" << endl ; 
          row += 3 ;
          return ;
        } // if()
          
        if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
             && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
             && Is_first(  m_all_token[column][row+1].content ) ) {
          
          cout << "> Line " << m_all_token[column][row+1].line ;
          cout << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row+1].content << "'" << endl ;
          row++ ;
        } // if()
        else {
          cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row+1].content << "'" << endl ; 
          
          row++ ;
        } // else
        
        return ;
      } // if()
        
        
     
      
      correct = true ;
      return ;
    
    } // if()
    
    else if ( ( int ) m_all_token[column][row].content[0] == 39 ) { // "
      
      // cout << "f" ;
      row++ ;
      
      while ( ( int ) m_all_token[column][row].content[0] != 39 ) {
        
        
        row++ ;
        if ( row + 1 > m_all_token[column].size() ) {
          column++ ;
          row = 0 ;
          
        } // if()
      } // while()
      
      if ( m_all_token[column][row+1].content != ";" && m_all_token[column][row+1].content != "+"
           && m_all_token[column][row+1].content != ">>" && m_all_token[column][row+1].content != "<<" ) {
        correct = false ;
        if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
             && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
             && Is_first(  m_all_token[column][row+1].content ) ) {
          
          cout << "> Line " << m_all_token[column][row+1].line ;
          cout << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row+1].content << "'" << endl ;
          row++ ;
        } // if()
        else {
          cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row+1].content << "'" << endl ; 
          
          row++ ;
        } // else
        
        return ;
      } // if()
        
        
     
      
      correct = true ;
      return ;
    
    } // if()
    
    if ( m_all_token[column][row].type != "ident" && m_all_token[column][row].type != "left_Paren"
         && m_all_token[column][row].content != "!" &&  m_all_token[column][row].type != "Function"
         && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double" ) {
      
      if ( m_all_token[column][row].content == "+" || m_all_token[column][row].content == "-" ) { 
        
        if ( m_all_token[column][row+1].content.size() == 1 && m_all_token[column][row+1].type != "ident"
             && m_all_token[column][row+1].type != "int" && m_all_token[column][row+1].type != "double"
             && Is_first(  m_all_token[column][row+1].content ) ) {
          
          cout << "> Line " << m_all_token[column][row+1].line ;
          cout << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row+1].content << "'" << endl ;
          row++ ;
        } // if()
        else {
          
          cout << "> Line " << m_all_token[column][row+1].line << " : unexpected token : '" ; 
          cout << m_all_token[column][row+1].content << "'" << endl ; 
          
          row++ ;
        } // else
      } // if()
      else {
        if ( m_all_token[column][row].content.size() == 1 && m_all_token[column][row].type != "ident"
             && m_all_token[column][row].type != "int" && m_all_token[column][row].type != "double"
             && Is_first(  m_all_token[column][row].content ) ) {
               
          cout << "> Line " << m_all_token[column][row].line << " : unrecognized token with first char : '" ;
          cout << m_all_token[column][row].content << "'" << endl ;
        } // if()
        else {
          cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
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
        cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
        cout << "> Unexpected token : '" << error_str << "'" << endl ;
        correct = false ;
      } // if()
      else if ( m_all_token[column][row].content == "." ) {
        cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
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
    
    if ( m_all_token[column][row].type == "Function" ) {
      
            
      if ( m_all_token[column][row+1].content == "(" ) {
        m_left++ ;
        row++ ;
        if ( m_all_token[column][row+1].content != ")" ) 
          Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
        else {
          m_right++ ;
          expCorrect = true ;
        } // else 
        
        if ( !expCorrect ) {
          correct = false ;
          syntax_error = true ;
          value = 0.0 ;
          return ; 
        } // if()
        
        row++ ;
        
          
      } // if()
   
      
      correct = true ;
      return ;
    
    } // if()
    
    if ( m_all_token[column][row].type == "ident" ) {
      if ( !fake ) {
      
        if ( Find_variable( m_all_token[column][row].content, position ) ) {
          correct = true ;
          value = m_variable[position].value ;
          if ( !m_is_float )
            m_is_float = m_variable[position].is_float ;
            
          if (  m_all_token[column][row+1].content == "[" ) {
            row++ ;
            Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
            if ( !expCorrect ) {
              correct = false ;
              syntax_error = true ;
              value = 0.0 ;
              return ; 
            } // if()
            
            row++ ;
            
          } // if()
        } // if()
        else {
          if ( m_all_token[column][row].content != "else" ) {
            
          
            cout << "> Line " << m_all_token[column][row].line << " : undefined identifier : '" ; 
            cout << m_all_token[column][row].content << "'" << endl ;
          } // if()
          else {
            cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
            cout << "> Unexpected token : '" << m_all_token[column][row].content << "'" << endl ;
            
          } // else
         
          
          correct = false ;
        } // else
      } // if()
      else {
        if ( Find_fake_variable( m_all_token[column][row].content, position ) ) {
          correct = true ;
          value = m_fake_variable[position].value ;
          if ( !m_is_float )
            m_is_float = m_fake_variable[position].is_float ;
          if (  m_all_token[column][row+1].content == "[" ) {
            row++ ;
            Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
            if ( !expCorrect ) {
              correct = false ;
              syntax_error = true ;
              value = 0.0 ;
              return ; 
            } // if()
            
            row++ ;
            
          } // if()
        } // if()
        else if ( Find_variable( m_all_token[column][row].content, position ) ) {
          correct = true ;
          value = m_variable[position].value ;
          if ( !m_is_float )
            m_is_float = m_variable[position].is_float ;
          if (  m_all_token[column][row+1].content == "[" ) {
            row++ ;
            Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
            if ( !expCorrect ) {
              correct = false ;
              syntax_error = true ;
              value = 0.0 ;
              return ; 
            } // if()
            
            row++ ;
            
          } // if()
        } // else if()
        else {
          cout << "> Line " << m_all_token[column][row].line << " : undefined identifier : '" ; 
          cout << m_all_token[column][row].content << "'" << endl ;
            
         
          
          correct = false ;
        } // else
        
      } // else
      
      return ;
    
    } // if()
    
    else if ( m_all_token[column][row].content == "(" ) { 
      m_left++ ;
      
      if ( m_all_token[column][row+1].content != ")" ) 
        Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
      else 
        expCorrect = true ;
      
      if ( !expCorrect ) {
        correct = false ;
        syntax_error = true ;
        value = 0.0 ;
        return ; 
      } // if()
      else {
        row++ ;
        
        if ( m_all_token[column][row].content != ")" ) {
          cout << "> Line " << m_all_token[column][row].line << " : unexpected token : '" ; 
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
  PJ2 start ;
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
  vector <string> name ;
  bool fake = false ;
  start.Get_token() ;
  bool elf = false ;
  start.Judgment_Type() ;
  bool quit = false ;
  
  
  cout << endl << endl ;
  /*for ( int i = 0 ; i < start.m_all_token.size() ; i++ ) {
    for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
      cout << start.m_all_token[i][j].content << " ["  << start.m_all_token[i][j].line << "] " ;
      // cout << " {" << start.m_all_token[i][j].line << "} " ;
    } // for()
    
    cout << endl ;
  } // for() */ 
  
  
  
   
  cout << "Our-C running ..." << endl ;
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
    string stop = "" ;
    vector <string> old ;
    name.clear() ;
    bool big = false ;
    bool expCorrect = false ;
    double expValue = 0.0 ;
    
    if ( start.m_all_token[column].size() > row+3 && !syntax_error ) {
    
      if ( start.m_all_token[column][row+1].content == "Done" && 
           start.m_all_token[column][row+2].content == "(" && 
           start.m_all_token[column][row+3].content == ")" ) {
        quit = true ;
      } // if()
    } // if()
    
    else if ( start.m_all_token[column].size() > row+3 && syntax_error 
              && start.m_all_token[column][row+1].is_new_line == false ) {
      if ( start.m_all_token[column][row+1].content == "Done" && 
           start.m_all_token[column][row+2].content == "(" && 
           start.m_all_token[column][row+3].content == ")" ) {
           
        char dannis = cin.peek() ;
        while ( dannis != '\n' ) {
          dannis = cin.get() ;
          dannis = cin.peek() ;
        } // while()
        
        start.Get_token() ;
        start.Judgment_Type() ;
        
        
        cout << endl << endl ;
        /*for ( int i = 0 ; i < start.m_all_token.size() ; i++ ) {
          for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
            cout << start.m_all_token[i][j].content << " ["  << start.m_all_token[i][j].line << "] " ;
            // cout << " {" << start.m_all_token[i][j].line << "} " ;
          } // for()
          
          cout << endl ;
        } // for() */ 
        
      } // if()
      
      
    } // if() 
    
    if ( syntax_error ) {
      
      while ( !start.m_all_token[column][peek].is_new_line ) {
        row++ ;
        peek = row + 1 ;
        // cout << column << " " << row << endl ;
        if ( peek + 1 > start.m_all_token[column].size() ) {
          column++ ;
          row = -1 ;
          peek = row + 1 ;
        } // if()
      } // while
    } // if()
    
    syntax_error = false ;
    
    if ( start.m_all_token[column].size() > row+3 && !syntax_error ) {
    
      if ( start.m_all_token[column][row+1].content == "Done" && 
           start.m_all_token[column][row+2].content == "(" && 
           start.m_all_token[column][row+3].content == ")" ) {
        
        quit = true ;
      } // if()
    } // if()
    
      
      
    
      
    
      
    if ( !quit ) {
      j = row + 1 ;
      
      for ( j = row + 1 ; start.m_all_token[column][j].content == "(" ; j++ ) {
        ;
      } // for
      
      
      
      
       
      if ( start.m_all_token[column][j].type == "ident" ) {
        for ( int i = j+1 ; i < start.m_all_token[column].size() ; i++ ) {
          if ( start.m_all_token[column][i].content == "=" )
            statement = true ; 
        } // for()
        
      
      } // if()
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
      if ( start.m_all_token[column][row+1].content == "while" ) {
        big = true ;
        start.While_a( correct, column, row, syntax_error ) ;
        if ( correct )  
          cout << "> Statement executed ..." << endl ;
        else 
          syntax_error = true ;
        
      } // if()
      else if ( start.m_all_token[column][row+1].content == "if" ) {
        start.If_else( correct, column, row, syntax_error ) ;
        if ( correct )  
          cout << "> Statement executed ..." << endl ;
        else 
          syntax_error = true ;
        
      } // else if()
      else if ( start.m_all_token[column][row+1].content == "ListVariable" ) {
        row ++ ;
        int ex = row + 1 ;
        start.Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
          
          
        if ( !expCorrect ) {
          correct = false ;
          syntax_error = true ;
          
          value = 0.0 ;
          
        } // if()
        else {
        
            
          if ( start.m_all_token[column][ex].content == "(" ) {
            
            ex += 1 ;
            
              
            cout << "> " ;
            if ( ( int ) start.m_all_token[column][ex].content[0] == 34 ) {
              
              ex++ ;
              string str =  start.m_all_token[column][ex].content ;
              if ( ( int ) start.m_all_token[column][ex+1].content[0] == 34 ) {
                if ( start.m_all_token[column][ex+2].content == ")" ) {
                  if ( start.m_all_token[column][ex+3].content == ";" ) {
                    
                    for ( int i = 0 ; i < start.m_variable.size() ; i++ ) {
                      if ( start.m_variable[i].name == str ) {
                        
                        if ( start.m_variable[i].size == 1 ) {
                        
                          cout << start.m_variable[i].type << " " << start.m_variable[i].name ;
                          cout << " ;" << endl ;
                        } // if
                        else if ( start.m_variable[i].size > 1 ) {
                        
                          cout << start.m_variable[i].type << " " << start.m_variable[i].name ;
                          cout << "[ " << start.m_variable[i].size << " ] ;" << endl ;
                        } // else if()
                      } // if()
                    } // for()
                  
                    
                  } // if()
                  else {
                    cout << "> Line " << start.m_all_token[column][ex+3].line << " : unexpected token : '" ; 
                    cout << start.m_all_token[column][ex+3].content << "'" << endl ; 
                    
                    
                  } // else
                  
                  ex += 3 ;
                } // if()
                
              } // if()
            } // if()
            else if ( start.m_all_token[column][ex].type == "ident" ) {
              
              
              string str =  start.m_all_token[column][ex].content ;
              
             
                
                  
              for ( int i = 0 ; i < start.m_variable.size() ; i++ ) {
                if ( start.m_variable[i].name == str ) {
                  
                  if ( start.m_variable[i].size == 1 ) {
                  
                    cout << start.m_variable[i].type << " " << start.m_variable[i].name ;
                    cout << " ;" << endl ;
                  } // if
                  else if ( start.m_variable[i].size > 1 ) {
                  
                    cout << start.m_variable[i].type << " " << start.m_variable[i].name ;
                    cout << "[ " << start.m_variable[i].size << " ] ;" << endl ;
                  } // else if()
                } // if()
              } // for()
               
                
              ex += 2 ;
             
             
            } // else if()
            
            cout << "Statement executed ..." << endl ;
          } // if()
          
        } // else
       
        
      } // else if()
      
      else if ( start.m_all_token[column][row+1].content == "{" ) {
        start.Big_par( correct, column, row, syntax_error ) ;
        
        if ( correct )  
          cout << "> Statement executed ..." << endl ;
        else 
          syntax_error = true ;
        
      } // else if()
      
      else if ( start.m_all_token[column][row+1].content == "ListAllVariables" ) {
        row ++ ;
        start.Exp( expCorrect, expValue, column, row, syntax_error, fake ) ;
        int ex = row + 1 ;
          
        if ( !expCorrect ) {
          correct = false ;
          syntax_error = true ;
          
          value = 0.0 ;
          
        } // if()
        else {
          cout << "> " ;
          
          for ( int i = 0 ; i < start.m_variable.size() ; i++ ) {
            name.push_back( start.m_variable[i].name ) ;
          } // for()
          
          if ( name.size() > 0 ) {
            
            for ( int i = 0 ; i + 1 < name.size() ; i++ ) {
              
              for ( int j = 0 ; j + i + 1 < name.size() ; j++ ) {
                if ( name[j] > name[j+1] ) {
                  string temp = name[j] ;
                  name[j] = name[j+1] ;
                  name[j+1] = temp ;
                } // if()
              } // for()
            } // for()
            
            cout << name[0] << endl ;
            for ( int i = 1 ; i < name.size() ; i++ )
              cout << name[i] << endl ;
          } // if()
            
            
          
          
          cout << "Statement executed ..." << endl ;
        } // else
        
        
      } // else if()
      
      else if ( start.m_all_token[column][row+1].content == "{" ) {
        start.Big_par( correct, column, row, syntax_error ) ;
        
        if ( correct )  
          cout << "> Statement executed ..." << endl ;
        else 
          syntax_error = true ;
        
      } // else if()
      else if ( start.m_all_token[column][row+1].type == "cincout" ) {
        string ans ;
        start.Cin_Cout( correct, column, row, syntax_error, fake ) ;
        if ( correct )  
          cout << "> Statement executed ..." << endl ;
        else 
          syntax_error = true ;
        
      } // else if()
      else if ( start.m_all_token[column][row+1].type == "Definition" ) {
        vector < string > ans ;
        start.Definition( correct, column, row, ans, old, fake ) ;
        if ( correct )  {
          if ( ans.size() > 0 ) {
          
            cout << "> Definition of " << ans[0] << " entered ..." << endl ;
            for ( int i = 1 ; i < ans.size() ; i++ )  
              cout << "Definition of " << ans[i] << " entered ..." << endl ;
          } // if()
          
          if ( old.size() > 0 ) {
        
            cout << "> New definition of " << old[0] << " entered ..." << endl ;
            for ( int i = 1 ; i < old.size() ; i++ )  
              cout << "New definition of " << old[i] << " entered ..." << endl ;
          } // if()
          
        } // if()
        else 
          syntax_error = true ;
        
        
      } // else if()
      else if ( statement ) {
        for ( int i = row+1 ; start.m_all_token[column][i].content == "(" ; i++ ) {
          row = i ;
          start.m_left++ ;
        } // for
        
        
        start.Statement( correct, value, column, row, syntax_error, fake ) ;
        
        
        for ( int i = row+2 ; i < start.m_all_token[column].size() && 
              start.m_all_token[column][i].content == ")" ; i++ ) {
          row = i ;
          start.m_right++ ;
        } // for
        
        
        if ( start.m_right > start.m_left ) {
          cout << "> Line " << start.m_all_token[column][row].line << " : unexpected token : '" ; 
          cout << start.m_all_token[column][row].content << "'" << endl ; 
          
          correct = false ;
          
        } // if()
            
        
        if ( start.m_all_token[column][row-1].content == "else" )
          row-- ;
        // cout << correct << endl ; 
        if ( correct ) {
          
            
          cout << "> Statement executed ..." << endl ;
         
        } // if()
        else {
          syntax_error = true ;
        } // else
          
      } // else if()
      
      else if ( boolean1 ) {
        // cout << "2" << endl ; 
        
        start.BooleanExp( correct, column, row, syntax_error, fake ) ;
        
        if ( !syntax_error ) {
        
          if ( correct )  
            cout << "> Statement executed ..." << endl ;
          else 
            cout << "> Statement executed ..." << endl ;
          
        } // if()
      } // else if()
      
      else {
        // cout << "3" << endl ; 
        
        start.Exp( correct, value, column, row, syntax_error, fake ) ;
        
          
        if ( correct ) {
          cout << "> Statement executed ..." << endl ;
        } // if()
        else {
         
          
          syntax_error = true ;
        } // else
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
        
        for ( int i = 0 ; i < start.m_all_token[column].size() ; i++ ) {
          start.m_all_token[column][i].line -= column_error ;
          
        } // for()
        
        
        for ( int i = column + 1 ; i < start.m_all_token.size() && 
              start.m_all_token[i][0].is_new_line != true && !first ; i++ ) {
          
          for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
            start.m_all_token[i][j].line -= 1 ;
           
              
          } // for() 
          
        } // for()
        
        if ( start.m_all_token[column+1][0].content == "else" ) {
          elf = true ;
          if ( m_all_token[column].size() > 1 ) {
          
            if ( m_all_token[column][row+1].content == "{" ) {
              for ( int j = column+1 ;  j < start.m_all_token.size() && 
                    m_all_token[j][0].content != "}" ; j++ ) {
                for ( int i = 0 ; i < start.m_all_token[j].size() ; i++ ) {
              
                  if ( column_error < start.m_all_token[j][i].line )
                    start.m_all_token[j][i].line -= column_error ;
                } // for()
              } // for()
                
             
            } // if() 
            else {
              
          
          
              for ( int i = 0 ; i < start.m_all_token[column+1].size() ; i++ ) {
                
                if ( column_error < start.m_all_token[column+1][i].line )
                  start.m_all_token[column+1][i].line -= column_error ;
              } // for()
             
              
            } // else
          
          } // if()
          else {
          
          
            for ( int i = 0 ; i < start.m_all_token[column+1].size() ; i++ ) {
              
              if ( column_error < start.m_all_token[column+1][i].line )
                start.m_all_token[column+1][i].line -= column_error ;
            } // for()
          } // else
          
        } // if()
        
        if ( big ) {
        
          int right_col = -1 ;
          int right_row = -1 ;
          for ( int i = column ; i < start.m_all_token.size() && 
                start.m_all_token[i][0].content != "{" ; i++ ) {
            for ( int j = 0 ; j < start.m_all_token[i].size() && 
                  start.m_all_token[i][j].content != "{" ; j++ ) {
              if ( start.m_all_token[i][j].content == "}" ) {
                right_col = i ;
                right_row = j ;
              } // if()
                
            } // for() 
            
          } // for()
          
          
          
            
          
          if ( right_col != -1 && right_row != -1 ) {
          
            
            for ( int m = column+1 ; start.m_all_token[m][0].line > 1 && m <= right_col ; m++ ) {
              for ( int i = 0 ; i < start.m_all_token[m].size() ; i++ ) {
                
                if ( column_error < start.m_all_token[m][i].line )
                  start.m_all_token[m][i].line = 1 ;
              } // for()
            } // for()
            
          } // if()
        } // if()
      } // if()
      else {
        if ( start.m_all_token[column+1][0].content == "else" && elf ) {
          for ( int i = 0 ; i < start.m_all_token[column+1].size() ; i++ ) {
            
            if ( column_error < start.m_all_token[column+1][i].line )
              start.m_all_token[column+1][i].line -= 1 ;
          } // for()
          
          elf = false ;
        } // if()
        else {
          elf = false ;
          
        } // else
        
      } // else
      
      if ( !first ) {
        column++ ;
        row = -1 ;
        column_error = 0 ;
      } // if()
      
      
    } // if()
    
  } // while() 
  
  cout << "> Our-C exited ..." << endl ;
  column = 0 ;
  system("pause"); 
  
  /*
  cout << endl << endl ;
  for ( int i = 0 ; i < start.m_all_token.size() ; i++ ) {
    for ( int j = 0 ; j < start.m_all_token[i].size() ; j++ ) {
      cout << start.m_all_token[i][j].content << " ["  << start.m_all_token[i][j].line << "] " ;
      // cout << " {" << start.m_all_token[i][j].type << "} " ;
    } // for()
    
    cout << endl ;
  } // for() 
  
  */
  /*
  cout << endl << endl ;
  for ( int i = 0 ; i < start.m_variable.size() ; i++ ) {
    cout << start.m_variable[i].name << " ["  << start.m_variable[i].type << "] " ;
    
    cout << endl ;
    
  } // for() 
  */
  /*
  start.m_all_token.clear() ;
  start.m_variable.clear() ;
  */
} // main()
