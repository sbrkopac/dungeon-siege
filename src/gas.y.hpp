/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_GS_GAS_Y_HPP_INCLUDED
# define YY_GS_GAS_Y_HPP_INCLUDED
/* Debug traces.  */
#ifndef GSDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define GSDEBUG 1
#  else
#   define GSDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define GSDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined GSDEBUG */
#if GSDEBUG
extern int gsdebug;
#endif
/* "%code requires" blocks.  */
#line 21 "gas.y" /* yacc.c:1909  */


    #include <string>
    #include "gas.hpp"


#line 59 "gas.y.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef GSTOKENTYPE
# define GSTOKENTYPE
  enum gstokentype
  {
    Expression = 258,
    Identifier = 259
  };
#endif

/* Value type.  */
#if ! defined GSSTYPE && ! defined GSSTYPE_IS_DECLARED
typedef std::string GSSTYPE;
# define GSSTYPE_IS_TRIVIAL 1
# define GSSTYPE_IS_DECLARED 1
#endif



int gsparse (void * scanner, ehb::Node * node);
/* "%code provides" blocks.  */
#line 28 "gas.y" /* yacc.c:1909  */


    extern void gserror (void * yyscanner, ehb::Node * node, const char * msg);

    int gsparse_file (const char * filename, ehb::Node * node);
    int gsparse_string (const char * data, ehb::Node * node);

    // this some serious black magic for VS due to lack of c++11 relaxed union support
    #undef GSSTYPE_IS_TRIVIAL
    #define GSSTYPE_IS_TRIVIAL 0


#line 95 "gas.y.hpp" /* yacc.c:1909  */

#endif /* !YY_GS_GAS_Y_HPP_INCLUDED  */
