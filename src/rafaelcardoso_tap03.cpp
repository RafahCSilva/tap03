/**
 * @author Rafael Cardoso da Silva 21048012
 * Tecnicas Avancadas de Programacao – 2018.Q1
 * Projeto 2: Domino
 *
 * Compile com C++ 17:
 *   g++ rafaelcardoso_tap03.cpp -o rafaelcardoso_tap03.exe -std=c++17
 *
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cstdio>
#include <string>

#define V true

namespace T {
    enum {
        FLECHA,
        LETRA_MAIUSCULA,
        TERMINAL,
        SINAL_MENOR,
        SINAL_MAIOR,
        EoL,
        EoF,
        TOKEN_INVALIDO
    };

    std::string printT( int t ) {
      switch ( t ) {
        case FLECHA:
          return "FLECHA";
        case LETRA_MAIUSCULA:
          return "LETRA_MAIUSCULA";
        case TERMINAL:
          return "TERMINAL";
        case SINAL_MENOR:
          return "SINAL_MENOR";
        case SINAL_MAIOR:
          return "SINAL_MAIOR";
        case EoL:
          return "EoL";
        case EoF:
          return "EoF";
        case TOKEN_INVALIDO:
          return "TOKEN_INVALIDO";
        default:
          return "???";
      }
    }
}

struct Token {
//    Token( int t, std::string srt ) : t( t ), srt( std::move( srt )) {}
    int t = T::TOKEN_INVALIDO;
    char srt = '\0';
};

std::vector<Token> lex;


void Analisador_Lexico() {
  if ( V )
    fprintf(stderr, "\nIniciando Analisador Lexico:\n" );

  std::string linha;
  int n, i;
  char c;

  while ( getline( std::cin, linha )) {
    n = ( int ) linha.size();
    i = 0;
    while ( i < n ) {
      c = linha[ i ];
      if ( V )
        if ( c == '-' )
          fprintf(stderr, "  '%s'\n", linha.substr( i, 3 ).c_str());
        else
          fprintf(stderr, "  '%c'\n", c );

      // "" ou \t
      if ( c == ' ' || c == '\t' ) {
      }
        // <
      else if ( c == '<' ) {
        lex.push_back(( Token ) { T::SINAL_MENOR, '<' } );
      }
        // >
      else if ( c == '>' ) {
        lex.push_back(( Token ) { T::SINAL_MAIOR, '>' } );
      }
        // Maiusculo
      else if ( std::isupper( c )) {
        lex.push_back(( Token ) { T::LETRA_MAIUSCULA, c } );
      }
        // -->
      else if ( linha.substr( i, 3 ) == "-->" ) {
        lex.push_back(( Token ) { T::FLECHA, '-' } );
        i += 2;
      }
        // Terminal
      else {
        lex.push_back(( Token ) { T::TERMINAL, c } );
      }

      i++;
    }
    // EoL
    lex.push_back(( Token ) { T::EoL, ' ' } );
  }
  // EoF
  lex.push_back(( Token ) { T::EoF, ' ' } );
}

void Imprimir_Lexico() {
  if ( V ) {
    fprintf(stderr, "\nImprimir Lexico:\n" );
    for ( auto l : lex )
      fprintf(stderr, "  '%c' : %s\n", l.srt, T::printT( l.t ).c_str());
  }
}

int main() {
  Analisador_Lexico();
  Imprimir_Lexico();

  return 0;
}
