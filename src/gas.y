/*
 * Copyright (c) 2016-2017 aaron andersen, sam brkopac
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

%code requires {

    #include <string>
    #include "gas.hpp"

}

%code provides {

    extern void gserror (void * yyscanner, ehb::Node * node, const char * msg);

    int gsparse_file (const char * filename, ehb::Node * node);
    int gsparse_string (const char * data, ehb::Node * node);

    // this some serious black magic for VS due to lack of c++11 relaxed union support
    #undef GSSTYPE_IS_TRIVIAL
    #define GSSTYPE_IS_TRIVIAL 0

}

%code {

    extern int gslex (YYSTYPE * yylval_param, void * yyscanner);

}

%define api.prefix {gs}
%define api.pure true
%define api.value.type {std::string}

%defines "gas.y.hpp"
%output "gas.y.cpp"
%lex-param {void * scanner}
%parse-param {void * scanner}
%parse-param {ehb::Node * node}

%token Expression "expression"
%token Identifier "identifier"

%start translation_unit

%%

translation_unit
    : element_body
    ;

element
    : '[' element_name ']' '{' element_body '}' { node = node->parent(); }
    ;

element_name
    : simple_identifier { node = node->appendChild($1); }
    | simple_identifier ':' simple_identifier ',' simple_identifier ':' simple_identifier { node = node->appendChild($7, $3); }
    | simple_identifier ',' simple_identifier ':' simple_identifier ',' simple_identifier ':' simple_identifier {

        // TODO: do something with $1... like check for "dev"?
        node = node->appendChild($9, $5);

    }
    ;

element_body
    :
    | element_body_list
    ;

element_body_list
    : element_body_item
    | element_body_list element_body_item
    ;

element_body_item
    : attribute
    | element
    ;

attribute
    : identifier '=' expression_statement ';' { node->appendValue($1, $3); }
    | type_id identifier '=' expression_statement ';' {

        if ($1 == "x") {
            try {
                auto value = std::stoll($4, nullptr, 16);
                node->appendValue($2, std::to_string(value));
            } catch (std::exception & e) {
                // TODO: log a warning
                node->appendValue($2, $4);
            }
        } else {
            node->appendValue($2, $4);
        }

        // TODO: ensure these values are interpreted properly...?
        if ($1 == "b") {
        } else if ($1 == "f") {
        } else if ($1 == "i") {
        } else if ($1 == "x") {
        } else if ($1 == "p") {
        } else if ($1 == "q") {
        }

    }
    ;

type_id
    : simple_identifier
    ;

expression_list
    : "expression"
    | expression_list "expression" { $$ = $1 + $2; }
    ;

expression_statement
    :
    | expression_list {

        $$ = $1;

        $$.erase(0, $$.find_first_not_of(" \n\r\t"));
        $$.erase($$.find_last_not_of(" \n\r\t") + 1);

    }
    ;

simple_identifier
    : "identifier"
    ;

identifier
    : simple_identifier
    | identifier ':' simple_identifier { $$ = $1 + ':' + $3; }
    ;

%%

#include <iostream>
#include "gas.l.hpp"

void gserror(void * yyscanner, ehb::Node * node, const char * msg)
{
    // TODO: possibly add a getLastError function to the Gas class?
    // either way, stop spamming cout
    std::cout << "yyerror: " << msg << " (" << (node ? node->name() : "") << ")" << std::endl;
}

int gsparse_file(const char * filename, ehb::Node * node)
{
    int result;

    yyscan_t scanner;

    if (gslex_init(&scanner) == 0)
    {
        // TODO: replace FILE & filename with istream
        FILE * fp = fopen(filename, "r");

        gspush_buffer_state(gs_create_buffer(fp, YY_BUF_SIZE, scanner), scanner);

        result = gsparse(scanner, node);

        fclose(fp);

        gslex_destroy(scanner);
    }

    return result == 0;
}

int gsparse_string (const char * data, ehb::Node * node)
{
    int result;

    yyscan_t scanner;

    if (gslex_init(&scanner) == 0)
    {
        YY_BUFFER_STATE buffer = gs_scan_string(data, scanner);

        gspush_buffer_state(buffer, scanner);

        result = gsparse(scanner, node);

        gs_delete_buffer(buffer, scanner);

        gslex_destroy(scanner);
    }

    return result == 0;
}
