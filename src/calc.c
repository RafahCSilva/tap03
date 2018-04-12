#include <stdio.h>
#include <stdlib.h>

enum TokenType {
    PLUS, MINUS, TIMES, SLASH, NUMBER, LPAR, RPAR, EOL, UNKNOWN
};

typedef struct s_token TOKEN;

struct s_token {
    enum TokenType ttype;
    double tvalue;
    int position;
    int length;
};

TOKEN token;

char *lineptr = NULL;
size_t n = 0;

enum TokenType next_token() {
  int pos = token.position + token.length;

  char c;

  while ( pos < n && (( c = lineptr[ pos ] ) == ' ' || c == '\t' ))
    pos++;

  token.position = pos;
  token.length = 1;

  switch ( c ) {
    case '(':
      return token.ttype = LPAR;

    case ')':
      return token.ttype = RPAR;

    case '\0':
      // this happens when pos == n, and
      // may happen when reading last line of a file w/o newline at the end
    case '\n':
      return token.ttype = EOL;

    case '+':
      return token.ttype = PLUS;

    case '-':
      return token.ttype = MINUS;

    case '*':
      return token.ttype = TIMES;

    case '/':
      return token.ttype = SLASH;

    default:
      if ( c < '0' || c > '9' )
        return token.ttype = UNKNOWN;
  }

  token.tvalue = c - '0';
  pos++;
  while ( pos < n && ( c = lineptr[ pos ] ) >= '0' && c <= '9' ) {
    token.tvalue *= 10;
    token.tvalue += ( c - '0' );
    token.length++;
    pos++;
  }

  return token.ttype = NUMBER;
}

void prompt() {
  printf( ">>> " );
}

double E();

double EL();

double T();

double TL();

double F();

double E() {
  return T() + EL();
}

double EL() {
  TOKEN t = token;

  if ( t.ttype != PLUS && t.ttype != MINUS )
    return 0;

  next_token();

  double first = T();

  if ( t.ttype == MINUS )
    first = -first;

  return first + EL();
}

double T() {
  return F() * TL();
}

double TL() {
  TOKEN t = token;

  if ( t.ttype != TIMES && t.ttype != SLASH )
    return 1;

  next_token();

  double first = F();

  if ( t.ttype == SLASH ) {
    if ( first == 0.0 )
      fprintf(stderr, "divisão por 0, por favor ignore o resultado\n" );
    else
      first = 1 / first;
  }

  return first * TL();
}

double F() {
  TOKEN t = token;

  if ( t.ttype == MINUS ) {
    next_token();
    return -F();
  }

  if ( t.ttype == NUMBER ) {
    next_token();
    return t.tvalue;
  }


  if ( t.ttype != LPAR ) {
    fprintf(stderr, "erro de sintaxe\n" );
    return 0;
  }

  next_token();
  double first = E();

  if ( token.ttype != RPAR ) {
    fprintf(stderr, "erro de sintaxe\n" );
    return 0;
  }

  next_token();
  return first;
}

int main( void ) {
  prompt();

  while ( getline( &lineptr, &n, stdin) > 0 ) {
    token.position = -token.length;
    next_token();
    printf( "%.2lf\n", E());
    prompt();
  }

  free( lineptr );

  return 0;
}
