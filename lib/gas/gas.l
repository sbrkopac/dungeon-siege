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

%{

    #include "gas.y.hpp"

    // seems like a bug in flex?
    #define YYSTYPE GSSTYPE

    // #undef YY_INPUT
    // #define YY_INPUT(buf,result,max) (result = a_stream.readsome(buf,max))

%}

%top {

    #include <sstream>
    #include <string>

}

%option bison-bridge
%option never-interactive
%option nounistd
%option noyywrap
%option prefix="gs"
%option reentrant
%option stack

%x embedded_statement
%x expression_statement
%x multi_line_comment
%x single_line_comment
%x string_literal

%%

"//"                    yy_push_state (single_line_comment, yyscanner);
"/*"                    yy_push_state (multi_line_comment, yyscanner);

"["                     return '[';
"]"                     return ']';
"{"                     return '{';
"}"                     return '}';
":"                     return ':';
","                     return ',';
"="                     return (yy_push_state (expression_statement, yyscanner), '=');

[0-9a-zA-Z_\-\*\.]+     return (*yylval = yytext, Identifier);

<embedded_statement>{

    "]]"                yy_pop_state (yyscanner); return (*yylval = yytext, Expression);
    "//"                yy_push_state (single_line_comment, yyscanner);
    "/*"                yy_push_state (multi_line_comment, yyscanner);
    [\t\n\r ]           return (*yylval = yytext, Expression); // NOTE: do not ignore this!
    .                   return (*yylval = yytext, Expression);

}

<expression_statement>{

    ";"                 return (yy_pop_state (yyscanner), ';');
    "[["                yy_push_state (embedded_statement, yyscanner); return (*yylval = yytext, Expression);
    "//"                yy_push_state (single_line_comment, yyscanner);
    "/*"                yy_push_state (multi_line_comment, yyscanner);
    \"                  yy_push_state (string_literal, yyscanner); return (*yylval = yytext, Expression);
    [\t\n\r ]           return (*yylval = yytext, Expression);
    .                   return (*yylval = yytext, Expression);

}

<multi_line_comment>{

    "*/"                yy_pop_state (yyscanner);
    .                   ;

}

<single_line_comment>{

    \n                  yy_pop_state (yyscanner);
    .                   ;

}

<string_literal>{

    \"                  yy_pop_state (yyscanner); return (*yylval = yytext, Expression);
    [\t\r\n ]           return (*yylval = yytext, Expression); // needed because of <*>[\t\r ]+
    "\\."               return (*yylval = yytext, Expression); // TODO: interpret escape sequence?
    .                   return (*yylval = yytext, Expression);

}

<*>[\t\r ]+             ;
[\n]+                   ;
.                       return 0;

%%
