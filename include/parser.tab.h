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
    CONST = 267,                   /* CONST  */
    RECORD = 268,                  /* RECORD  */
    IF = 269,                      /* IF  */
    THEN = 270,                    /* THEN  */
    ELSE = 271,                    /* ELSE  */
    CASE = 272,                    /* CASE  */
    OF = 273,                      /* OF  */
    WHILE = 274,                   /* WHILE  */
    DO = 275,                      /* DO  */
    FOR = 276,                     /* FOR  */
    REPEAT = 277,                  /* REPEAT  */
    UNTIL = 278,                   /* UNTIL  */
    BEGIN_ = 279,                  /* BEGIN_  */
    END = 280,                     /* END  */
    ADDOP = 281,                   /* ADDOP  */
    NOT = 282,                     /* NOT  */
    PLUS = 283,                    /* PLUS  */
    UMINUS = 284,                  /* UMINUS  */
    ASSIGNOP = 285,                /* ASSIGNOP  */
    TRUE = 286,                    /* TRUE  */
    FALSE = 287,                   /* FALSE  */
    CONSTASSIGNOP = 288,           /* CONSTASSIGNOP  */
    ID = 289,                      /* ID  */
    CHAR = 290,                    /* CHAR  */
    INT_NUM = 291,                 /* INT_NUM  */
    REAL_NUM = 292,                /* REAL_NUM  */
    BASIC_TYPE = 293,              /* BASIC_TYPE  */
    RELOP = 294,                   /* RELOP  */
    MULOP = 295                    /* MULOP  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 47 "scripts/parser.y"

    Token token_info;
    IdListAttr id_list_node_info;
    ValueAttr value_node_info;
    TypeAttr type_node_info;
    StandardTypeAttr standared_type_node_info;
    PeriodsAttr periods_node_info;
    PeriodAttr period_node_info;
    RecordAttr record_node_info;
    FormalParameterAttr formal_parameter_node_info;
    ParameterListsAttr parameter_lists_node_info;
    ParameterListAttr parameter_list_node_info;
    VarParameterAttr var_parameter_node_info;
    ValueParameterAttr value_parameter_node_info;

    VariableDeclarationAttr variable_declaration_node_info;
    VariableAttr variable_node_info;
    ExpressionAttr expression_node_info;
    SimpleExpressionAttr simple_expression_node_info;
    TermAttr term_node_info;
    FactorAttr factor_node_info;
    UnsignedConstantVarAttr unsigned_constant_var_node_info;
    IDVarpartsAttr id_varparts_node_info;
    IDVarpartAttr id_varpart_node_info;
    ExpressionListAttr expression_list_node_info;
    CaseBodyAttr case_body_node_info;
    BranchListAttr branch_list_node_info;
    BranchAttr branch_node_info;
    ConstListAttr const_list_node_info;

    ast::ProgramNode* program_node;
    ast::ProgramHeadNode* program_head_node;
    ast::ProgramBodyNode* program_body_node;
    ast::ConstDeclarationsNode* const_declarations_node;
    ast::ConstDeclarationNode* const_declaration_node;
    ast::TypeDeclarationsNode* type_declarations_node;
    ast::TypeDeclarationNode* type_declaration_node;
    ast::BasicTypeNode* basic_type_node;
    ast::VariableDeclarationsNode* variable_declarations_node;
    ast::SubprogramDeclarationsNode* subprogram_declarations_node;
    ast::SubprogramDeclarationNode* subprogram_declaration_node;
    ast::SubprogramHeadNode* subprogram_head_node;
    ast::SubprogramBodyNode* subprogram_body_node;
    ast::CompoundStatementNode* compound_statement_node;
    ast::StatementListNode* statement_list_node;
    ast::StatementNode* statement_node;
    ast::ElseNode* else_node;
    ast::UpdownNode* updown_node;
    ast::ProcedureCallNode* procedure_call_node;


#line 156 "include/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (ast::AST *real_ast);


#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
