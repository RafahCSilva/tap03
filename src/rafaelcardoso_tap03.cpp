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

//#define V true
#define V false

namespace T {
    enum t_enum {
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
    unsigned long id = 0;
};

std::vector< Token > lex;


void Analisador_Lexico() {
    if ( V )
        fprintf( stderr, "\nIniciando Analisador Lexico:\n" );

    std::string linha;
    int n, i;
    char c;

    while ( getline( std::cin, linha ) ) {
        n = ( int ) linha.size();
        i = 0;
        while ( i < n ) {
            c = linha[ i ];
            if ( V ) {
                if ( c == '-' )
                    fprintf( stderr, "  '%s'\n", linha.substr( i, 3 ).c_str() );
                else
                    fprintf( stderr, "  '%c'\n", c );
            }

            // "" ou \t
            if ( c == ' ' || c == '\t' ) {
            }
                // <
            else if ( c == '<' ) {
                lex.push_back( ( Token ) { T::SINAL_MENOR, '<', lex.size() } );
            }
                // >
            else if ( c == '>' ) {
                lex.push_back( ( Token ) { T::SINAL_MAIOR, '>', lex.size() } );
            }
                // Maiusculo
            else if ( std::isupper( c ) ) {
                lex.push_back( ( Token ) { T::LETRA_MAIUSCULA, c, lex.size() } );
            }
                // -->
            else if ( linha.substr( i, 3 ) == "-->" ) {
                lex.push_back( ( Token ) { T::FLECHA, '-', lex.size() } );
                i += 2;
            }
                // Terminal
            else {
                lex.push_back( ( Token ) { T::TERMINAL, c, lex.size() } );
            }

            i++;
        }
        // EoL
        lex.push_back( ( Token ) { T::EoL, ' ', lex.size() } );
    }
    // EoF
    lex.push_back( ( Token ) { T::EoF, ' ', lex.size() } );
}

void Imprimir_Lexico() {
    if ( V ) {
        fprintf( stderr, "\nImprimir Lexico:\n" );
        for ( auto l : lex )
            fprintf( stderr, "  %lu :  '%c' : %s\n", l.id, l.srt, T::printT( l.t ).c_str() );
    }
}


unsigned long LEX_N = 0;

bool lex_is( T::t_enum t ) {
    // TODO: confere se o LEX_N nao estoura
    if ( LEX_N >= lex.size() ) {
        throw std::runtime_error( "Estouro do LEX" );
    }
    if ( lex.at( LEX_N ).t == t ) {
        if ( V )
            fprintf( stderr, "  %lu : %s\n", LEX_N, T::printT( t ).c_str() );
        LEX_N++;
        return true;
    }
    if ( V )
        fprintf( stderr, "    nao terminal %lu : %s\n", LEX_N, T::printT( t ).c_str() );
    return false;
}


bool g_gramatica();

bool g_regra();

bool g_sentenca1();

bool g_sentenca2();

bool g_elemento();

bool g_variavel();

bool g_id();

/*
 * GRAMATICA:
 *   <gramatica> --> <regra> <gramatica>
 *   <gramatica> --> <regra> EOF
 *   <regra> --> <sentenca1> FLECHA <sentenca2> EOL
 *   <sentenca1> --> <elemento> <sentenca2>
 *   <sentenca2> --> <elemento> <sentenca2>
 *   <sentenca2> -->
 *   <elemento> --> <variavel>
 *   <elemento> --> TERMINAL
 *   <variavel> --> LETRAMAIUSCULA
 *   <variavel> --> SINALMENOR <id> SINALMAIOR
 *   <id> --> TERMINAL <id>
 *   <id> --> TERMINAL
 *
 */


bool g_gramatica() {
    if ( g_regra() ) {
        return g_gramatica();
    } else if ( lex_is( T::EoF ) ) {
        return true;
    }
    return false;
}

bool g_regra() {

    return g_sentenca1() &&
           ( lex_is( T::FLECHA ) ) &&
           g_sentenca2() &&
           ( lex_is( T::EoL ) );

}

bool g_sentenca1() {
    return g_elemento() &&
           g_sentenca2();
}

bool g_sentenca2() {
    while ( g_elemento() ) {

    }

    return true;
}

bool g_elemento() {
    if ( lex_is( T::TERMINAL ) ) {
        return true;
    }

    return g_variavel();
}

bool g_variavel() {
    if ( lex_is( T::LETRA_MAIUSCULA ) ) {
        return true;
    }
    return lex_is( T::SINAL_MENOR ) &&
           g_id() &&
           lex_is( T::SINAL_MAIOR );
}

bool g_id() {
    if ( lex_is( T::TERMINAL ) ) {
        g_id();
        return true;
    }

    return false;
}

void Analisador_Sintatico() {
    if ( V )
        fprintf( stderr, "\nIniciando Analisador Sintático:\n" );

    if ( g_gramatica() ) {
        fprintf( stdout, "CORRETO 0\n" );
    } else {
        fprintf( stdout, "ERRO\n" );
    }
}

int main() {
    Analisador_Lexico();
    Imprimir_Lexico();

    Analisador_Sintatico();

    return 0;
}
