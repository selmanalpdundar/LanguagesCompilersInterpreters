/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    GE = 258,
    LE = 259,
    EQ = 260,
    NE = 261,
    FALSE = 262,
    TRUE = 263,
    INCREMENT = 264,
    DECREMENT = 265,
    IF = 266,
    ELSE = 267,
    WHILE = 268,
    PRINT = 269,
    BOOL_TYPE = 270,
    INT_TYPE = 271,
    AND = 272,
    OR = 273,
    XOR = 274,
    REMAINDER = 275,
    ID = 276,
    QUESTION_MARK = 277,
    COLON = 278,
    VAL = 279,
    IF_ALONE = 280
  };
#endif
/* Tokens.  */
#define GE 258
#define LE 259
#define EQ 260
#define NE 261
#define FALSE 262
#define TRUE 263
#define INCREMENT 264
#define DECREMENT 265
#define IF 266
#define ELSE 267
#define WHILE 268
#define PRINT 269
#define BOOL_TYPE 270
#define INT_TYPE 271
#define AND 272
#define OR 273
#define XOR 274
#define REMAINDER 275
#define ID 276
#define QUESTION_MARK 277
#define COLON 278
#define VAL 279
#define IF_ALONE 280

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "parser.y"

  int value;
  size_t id;
  struct expr *expr;
  enum value_type {
    ERROR = -1,
    UNTYPED = 0,
    INTEGER = 1,
    BOOLEAN = 2,
  } type;
  struct stmt *stmt;

#line 120 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (LLVMModuleRef module, LLVMBuilderRef builder);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
