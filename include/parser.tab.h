/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROGRAM = 258,                 /* PROGRAM  */
    FUNCTION = 259,                /* FUNCTION  */
    PROCEDURE = 260,               /* PROCEDURE  */
    TO = 261,                      /* TO  */
    DOWNTO = 262,                  /* DOWNTO  */
    SUBCATALOG = 263,              /* SUBCATALOG  */
    ARRAY = 264,                   /* ARRAY  */
    VAR = 265,                     /* VAR  */
    TYPE = 266,                    /* TYPE  */
    BASIC_TYPE = 267,              /* BASIC_TYPE  */
    INT_NUM = 268,                 /* INT_NUM  */
    REAL_NUM = 269,                /* REAL_NUM  */
    CHAR = 270,                    /* CHAR  */
    CONST = 271,                   /* CONST  */
    RECORD = 272,                  /* RECORD  */
    IF = 273,                      /* IF  */
    THEN = 274,                    /* THEN  */
    ELSE = 275,                    /* ELSE  */
    CASE = 276,                    /* CASE  */
    OF = 277,                      /* OF  */
    WHILE = 278,                   /* WHILE  */
    DO = 279,                      /* DO  */
    FOR = 280,                     /* FOR  */
    REPEAT = 281,                  /* REPEAT  */
    UNTIL = 282,                   /* UNTIL  */
    BEGIN_ = 283,                  /* BEGIN_  */
    END = 284,                     /* END  */
    RELOP = 285,                   /* RELOP  */
    ADDOP = 286,                   /* ADDOP  */
    MULOP = 287,                   /* MULOP  */
    NOT = 288,                     /* NOT  */
    PLUS = 289,                    /* PLUS  */
    UMINUS = 290,                  /* UMINUS  */
    ASSIGNOP = 291,                /* ASSIGNOP  */
    TRUE = 292,                    /* TRUE  */
    FALSE = 293,                   /* FALSE  */
    ID = 294                       /* ID  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 39 "scripts/parser.y"

    Token token_info;
    ast::ProgramNode* program_node;
    ast::ProgramHeadNode* program_head_node;
    ast::ProgramBodyNode* program_body_node;
    ast::IdListNode* id_list_node;
    ast::ConstDeclarationsNode* const_declarations_node;
    ast::TypeDeclarationsNode* type_declarations_node;
    ast::VariableDeclarationsNode* variable_declarations_node;
    ast::SubprogramDeclarationsNode* subprogram_declarations_node;
    ast::CompoundStatementNode* compound_statement_node;
    ast::StatementListNode* statement_list_node;
    ast::StatementNode* statement_node;

#line 118 "include/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (ast::AST *real_ast);


#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
