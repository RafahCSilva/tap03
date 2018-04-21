/**
 * @author Rafael Cardoso da Silva 21048012
 * Tecnicas Avancadas de Programacao – 2018.Q1
 * Projeto 3: Reconhecendo Gramaticas
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
            // ignora "" ou \t ou \r\n
            if ( c == ' ' || c == '\t' || c == '\r' || c == '\n' ) {
                i++;
                continue;
            }
            if ( V ) {
                if ( c == '-' )
                    fprintf( stderr, "  '%s'\n", linha.substr( i, 3 ).c_str() );
                else
                    fprintf( stderr, "  '%c' = %d\n", c, c );
            }

            // <
            if ( c == '<' ) {
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
    if ( LEX_N >= lex.size() ) {
        throw std::runtime_error( "Estouro do LEX" );
    }
    if ( lex.at( LEX_N ).t == t ) {
        if ( V )
            fprintf( stderr, "  %lu : %s '%c'\n", LEX_N, T::printT( t ).c_str(), lex.at( LEX_N ).srt );
        LEX_N++;
        return true;
    }
    return false;
}

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
bool g_gramatica();

bool g_regra();

bool g_sentenca1();

bool g_sentenca2();

bool g_elemento();

bool g_variavel();

bool g_id();

bool g_gramatica() {
    if ( g_regra() ) { // 1
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
    while ( g_elemento() );

    return true;
}

bool g_elemento() {
    if ( lex_is( T::TERMINAL ) )
        return true;

    return g_variavel();
}

bool g_variavel() {
    if ( lex_is( T::LETRA_MAIUSCULA ) )
        return true;

    return lex_is( T::SINAL_MENOR ) &&
           g_id() &&
           lex_is( T::SINAL_MAIOR );
}

bool g_id() {
    if ( lex_is( T::TERMINAL ) ) {
        while ( lex_is( T::TERMINAL ) );
        return true;
    }

    return false;
}


/*
 * Classificador de Gramaticas:
 *   tipo 3 - gramaticas regulares
 *   tipo 2 - gramaticas livres de contexto
 *   tipo 1 - gramaticas sensıveis ao contexto
 *   tipo 0 - gramaticas irrestritas
 */
int classificar_gramatica() {
    if ( V )
        fprintf( stderr, "\nIniciando Classificacao da Gramatica:\n" );

    // flags
    bool tem_VAR_esq = false;
    bool tem_TERM_esq = false;
    bool tem_VAR_dir = false;
    bool tem_TERM_dir = false;
    bool vazio_dir = false;
    bool reg_dir = true;
    bool lado = false; // false = esq | true = dir
    LEX_N = 0;
    unsigned long size = lex.size();

    int cont_dir = 0, cont_TERM = 0, cont_VAR = 0;
    while ( LEX_N < size ) {

        // Acabou as regras, entao analisa as flags
        if ( lex_is( T::EoF ) ) {

            // tipo 3 - gramaticas regulares
            if ( tem_VAR_esq &&
                 !tem_TERM_esq &&
                 reg_dir ) {
                return 3;
            }

            // tipo 2 - gramaticas livres de contexto
            if ( tem_VAR_esq &&
                 !tem_TERM_esq ) {
                return 2;
            }

            // tipo 1 - gramaticas sensıveis ao contexto
            if ( tem_VAR_esq &&
                 tem_TERM_esq &&
                 ( tem_VAR_dir || tem_TERM_dir ) &&
                 !vazio_dir ) {
                return 1;
            }

            // tipo 0 - gramaticas irrestritas
            return 0;
        }

        if ( !lado ) {
            // lado ESQ da Regra
            if ( lex_is( T::SINAL_MENOR ) ) {
                while ( lex_is( T::TERMINAL ) );
                if ( lex_is( T::SINAL_MAIOR ) ) {
                    tem_VAR_esq = true;
                    continue;
                }
            } else if ( lex_is( T::TERMINAL ) ) {
                // eh TERM na esq
                tem_TERM_esq = true;
            } else if ( lex_is( T::LETRA_MAIUSCULA ) ) {
                // eh VAR na esq
                tem_VAR_esq = true;
            } else if ( lex_is( T::FLECHA ) ) {
                // Terminou a primeira parte da regra, entao troca de lado
                if ( V ) fprintf( stderr, "--> \n" );
                lado = true;

                continue;
            }
        } else {
            // lado DIR da Regra
            if ( lex_is( T::SINAL_MENOR ) ) { // se "< id >"
                while ( lex_is( T::TERMINAL ) );
                if ( lex_is( T::SINAL_MAIOR ) ) {
                    tem_VAR_dir = true;
                    cont_VAR++;
                    continue;
                }
            } else if ( lex_is( T::TERMINAL ) ) {
                // eh TERM a dir
                tem_TERM_dir = true;
                cont_dir++;
                cont_TERM++;
            } else if ( lex_is( T::LETRA_MAIUSCULA ) ) {
                // eh VAR a dir
                tem_VAR_dir = true;
                cont_dir++;
                cont_VAR++;
            } else if ( lex_is( T::EoL ) ) {
                // terminou a linha, troca o lado
                if ( V ) fprintf( stderr, "EoL \n\n" );
                lado = false;

                // E analisa se eh regular na direita, isto eh: aB | Ba | eps
                if ( !( ( cont_TERM == 1 && cont_VAR == 1 ) || cont_dir == 0 ) )
                    reg_dir = false;
                cont_TERM = 0;
                cont_VAR = 0;

                // se foi eps, ativa a flag
                if ( cont_dir == 0 )
                    vazio_dir = true;
                cont_dir = 0;

                continue;
            }
        }
    }

    return 0;
}

void Analisador_Sintatico() {
    if ( V )
        fprintf( stderr, "\nIniciando Analisador Sintático:\n" );

    if ( g_gramatica() ) {
        fprintf( stdout, "CORRETO %d\n", classificar_gramatica() );
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
