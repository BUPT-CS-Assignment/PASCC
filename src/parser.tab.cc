/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "scripts/parser.y"
	
#include"parser.h"
using namespace ast;
extern "C"			
{					
	void yyerror(const char *s);
	extern int yylex(void);
    extern int line_count;
    extern char* yytext;
    extern int char_count;
}
extern std::string buf;
int len =0,column=0;
char my_buffer[1024];
//AST real_ast;

void yyerror(ast::AST* real_ast,char *msg);

void fresh_argu(){
    column = buf.size();
    len = strlen(yytext);
}

void yyerror_(const char *error_s){
    fprintf(stderr,"%d:%d:\033[01;31m \terror\033[0m : %s\n", line_count, column, error_s);
    memset(my_buffer,'\0',1024);
    memset(my_buffer,' ',column-len);
    my_buffer[column-len]='^';
    memset(my_buffer+column-len+1,'~',len-1);
    std::cout<<"  "<<line_count<<"\t|\t"<<buf<<std::endl;
    fprintf(stderr,"\t|\t\033[01;31m%s\033[0m\n",my_buffer);
}

#define DEBUG 0


#line 108 "src/parser.tab.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROGRAM = 3,                    /* PROGRAM  */
  YYSYMBOL_FUNCTION = 4,                   /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 5,                  /* PROCEDURE  */
  YYSYMBOL_TO = 6,                         /* TO  */
  YYSYMBOL_DOWNTO = 7,                     /* DOWNTO  */
  YYSYMBOL_SUBCATALOG = 8,                 /* SUBCATALOG  */
  YYSYMBOL_ARRAY = 9,                      /* ARRAY  */
  YYSYMBOL_VAR = 10,                       /* VAR  */
  YYSYMBOL_TYPE = 11,                      /* TYPE  */
  YYSYMBOL_BASIC_TYPE = 12,                /* BASIC_TYPE  */
  YYSYMBOL_INT_NUM = 13,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 14,                  /* REAL_NUM  */
  YYSYMBOL_CHAR = 15,                      /* CHAR  */
  YYSYMBOL_CONST = 16,                     /* CONST  */
  YYSYMBOL_RECORD = 17,                    /* RECORD  */
  YYSYMBOL_IF = 18,                        /* IF  */
  YYSYMBOL_THEN = 19,                      /* THEN  */
  YYSYMBOL_ELSE = 20,                      /* ELSE  */
  YYSYMBOL_CASE = 21,                      /* CASE  */
  YYSYMBOL_OF = 22,                        /* OF  */
  YYSYMBOL_WHILE = 23,                     /* WHILE  */
  YYSYMBOL_DO = 24,                        /* DO  */
  YYSYMBOL_FOR = 25,                       /* FOR  */
  YYSYMBOL_REPEAT = 26,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 27,                     /* UNTIL  */
  YYSYMBOL_BEGIN_ = 28,                    /* BEGIN_  */
  YYSYMBOL_END = 29,                       /* END  */
  YYSYMBOL_RELOP = 30,                     /* RELOP  */
  YYSYMBOL_ADDOP = 31,                     /* ADDOP  */
  YYSYMBOL_MULOP = 32,                     /* MULOP  */
  YYSYMBOL_NOT = 33,                       /* NOT  */
  YYSYMBOL_PLUS = 34,                      /* PLUS  */
  YYSYMBOL_UMINUS = 35,                    /* UMINUS  */
  YYSYMBOL_ASSIGNOP = 36,                  /* ASSIGNOP  */
  YYSYMBOL_TRUE = 37,                      /* TRUE  */
  YYSYMBOL_FALSE = 38,                     /* FALSE  */
  YYSYMBOL_ID = 39,                        /* ID  */
  YYSYMBOL_40_ = 40,                       /* '.'  */
  YYSYMBOL_41_ = 41,                       /* '('  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_43_ = 43,                       /* ';'  */
  YYSYMBOL_44_ = 44,                       /* ','  */
  YYSYMBOL_45_ = 45,                       /* "="  */
  YYSYMBOL_46_ = 46,                       /* '='  */
  YYSYMBOL_47_ = 47,                       /* '['  */
  YYSYMBOL_48_ = 48,                       /* ']'  */
  YYSYMBOL_49_ = 49,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_program = 51,                   /* program  */
  YYSYMBOL_program_head = 52,              /* program_head  */
  YYSYMBOL_53_1 = 53,                      /* $@1  */
  YYSYMBOL_54_2 = 54,                      /* $@2  */
  YYSYMBOL_program_body = 55,              /* program_body  */
  YYSYMBOL_id_list = 56,                   /* id_list  */
  YYSYMBOL_const_declarations = 57,        /* const_declarations  */
  YYSYMBOL_const_declaration = 58,         /* const_declaration  */
  YYSYMBOL_const_variable = 59,            /* const_variable  */
  YYSYMBOL_num = 60,                       /* num  */
  YYSYMBOL_type_declarations = 61,         /* type_declarations  */
  YYSYMBOL_type_declaration = 62,          /* type_declaration  */
  YYSYMBOL_type = 63,                      /* type  */
  YYSYMBOL_record_body = 64,               /* record_body  */
  YYSYMBOL_standrad_type = 65,             /* standrad_type  */
  YYSYMBOL_periods = 66,                   /* periods  */
  YYSYMBOL_period = 67,                    /* period  */
  YYSYMBOL_var_declarations = 68,          /* var_declarations  */
  YYSYMBOL_var_declaration = 69,           /* var_declaration  */
  YYSYMBOL_70_3 = 70,                      /* $@3  */
  YYSYMBOL_subprogram_declarations = 71,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration = 72,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_body = 73,           /* subprogram_body  */
  YYSYMBOL_subprogram_head = 74,           /* subprogram_head  */
  YYSYMBOL_formal_parameter = 75,          /* formal_parameter  */
  YYSYMBOL_parameter_lists = 76,           /* parameter_lists  */
  YYSYMBOL_parameter_list = 77,            /* parameter_list  */
  YYSYMBOL_var_parameter = 78,             /* var_parameter  */
  YYSYMBOL_value_parameter = 79,           /* value_parameter  */
  YYSYMBOL_compound_statement = 80,        /* compound_statement  */
  YYSYMBOL_statement_list = 81,            /* statement_list  */
  YYSYMBOL_statement = 82,                 /* statement  */
  YYSYMBOL_variable = 83,                  /* variable  */
  YYSYMBOL_id_varparts = 84,               /* id_varparts  */
  YYSYMBOL_id_varpart = 85,                /* id_varpart  */
  YYSYMBOL_else_part = 86,                 /* else_part  */
  YYSYMBOL_case_body = 87,                 /* case_body  */
  YYSYMBOL_branch_list = 88,               /* branch_list  */
  YYSYMBOL_branch = 89,                    /* branch  */
  YYSYMBOL_const_list = 90,                /* const_list  */
  YYSYMBOL_updown = 91,                    /* updown  */
  YYSYMBOL_call_procedure_statement = 92,  /* call_procedure_statement  */
  YYSYMBOL_expression_list = 93,           /* expression_list  */
  YYSYMBOL_expression = 94,                /* expression  */
  YYSYMBOL_simple_expression = 95,         /* simple_expression  */
  YYSYMBOL_term = 96,                      /* term  */
  YYSYMBOL_factor = 97,                    /* factor  */
  YYSYMBOL_98_4 = 98,                      /* $@4  */
  YYSYMBOL_unsigned_const_variable = 99    /* unsigned_const_variable  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  219

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,     2,     2,    44,     2,    40,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    43,
       2,    46,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    74,    74,    85,    93,    93,    96,    96,   100,   116,
     122,   127,   130,   136,   142,   150,   155,   160,   164,   169,
     173,   177,   183,   188,   193,   197,   202,   207,   213,   219,
     225,   232,   235,   242,   250,   257,   263,   272,   276,   281,
     288,   295,   305,   315,   315,   320,   324,   329,   334,   339,
     346,   354,   357,   362,   366,   371,   375,   380,   385,   390,
     398,   402,   410,   414,   418,   422,   427,   432,   437,   442,
     446,   453,   460,   463,   471,   476,   481,   484,   489,   492,
     497,   501,   506,   511,   515,   520,   524,   529,   534,   539,
     546,   554,   561,   567,   571,   575,   579,   586,   593,   601,
     605,   613,   618,   623,   628,   633,   633,   637,   643,   648,
     653,   658
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PROGRAM", "FUNCTION",
  "PROCEDURE", "TO", "DOWNTO", "SUBCATALOG", "ARRAY", "VAR", "TYPE",
  "BASIC_TYPE", "INT_NUM", "REAL_NUM", "CHAR", "CONST", "RECORD", "IF",
  "THEN", "ELSE", "CASE", "OF", "WHILE", "DO", "FOR", "REPEAT", "UNTIL",
  "BEGIN_", "END", "RELOP", "ADDOP", "MULOP", "NOT", "PLUS", "UMINUS",
  "ASSIGNOP", "TRUE", "FALSE", "ID", "'.'", "'('", "')'", "';'", "','",
  "\"=\"", "'='", "'['", "']'", "':'", "$accept", "program",
  "program_head", "$@1", "$@2", "program_body", "id_list",
  "const_declarations", "const_declaration", "const_variable", "num",
  "type_declarations", "type_declaration", "type", "record_body",
  "standrad_type", "periods", "period", "var_declarations",
  "var_declaration", "$@3", "subprogram_declarations",
  "subprogram_declaration", "subprogram_body", "subprogram_head",
  "formal_parameter", "parameter_lists", "parameter_list", "var_parameter",
  "value_parameter", "compound_statement", "statement_list", "statement",
  "variable", "id_varparts", "id_varpart", "else_part", "case_body",
  "branch_list", "branch", "const_list", "updown",
  "call_procedure_statement", "expression_list", "expression",
  "simple_expression", "term", "factor", "$@4", "unsigned_const_variable", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-128)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-73)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,   -12,    40,    79,    29,     8,    57,  -128,    97,    70,
     140,    60,   123,   120,   119,   122,  -128,   127,   157,  -128,
    -128,    26,  -128,    41,   129,   125,   126,   123,  -128,   130,
     131,  -128,  -128,  -128,     5,    12,  -128,  -128,  -128,   132,
      92,   133,    15,   135,   173,    33,  -128,  -128,  -128,  -128,
    -128,  -128,    41,   128,  -128,    14,  -128,  -128,   138,    24,
     123,   136,   137,   146,    63,   143,    79,  -128,  -128,    41,
     158,   145,    92,  -128,  -128,    64,  -128,   148,   148,   106,
     106,   106,   151,    63,    76,  -128,    42,  -128,   155,  -128,
    -128,   140,  -128,   184,    90,  -128,  -128,   123,  -128,    30,
       4,   144,   152,  -128,    59,    59,    59,  -128,  -128,   153,
     106,  -128,  -128,   177,   118,   165,  -128,  -128,   176,   175,
     164,    50,   106,    71,  -128,    63,   106,   157,    41,    41,
     179,  -128,  -128,   123,    78,   114,  -128,  -128,  -128,   190,
    -128,  -128,   165,   165,   106,   161,    63,   106,    59,    59,
      59,    59,    41,    63,   106,   106,    84,  -128,   166,   106,
    -128,  -128,  -128,   178,  -128,  -128,    92,  -128,   190,  -128,
       4,   167,    91,  -128,   203,   187,   124,   165,   165,   165,
    -128,  -128,   180,   168,  -128,    81,  -128,   154,  -128,  -128,
     106,  -128,   102,  -128,  -128,  -128,  -128,  -128,  -128,   169,
      63,  -128,  -128,    41,    41,    63,  -128,  -128,   106,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,   189,    63,  -128
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     6,     0,    11,     0,     0,     0,     1,     0,     0,
      24,     0,     0,     0,     0,     0,     2,     0,    37,     5,
      10,     0,     7,     0,    12,     0,     0,    43,    45,     0,
       0,    22,    23,    21,     0,     0,    17,    14,    19,     0,
       0,    25,     0,     0,     0,     0,     3,     9,    15,    20,
      16,    18,     0,     0,    33,    31,    27,    28,     0,     0,
      38,     0,     0,     0,    70,     0,    11,     8,    13,     0,
       0,    32,     0,    42,    40,     0,    44,    51,    51,     0,
       0,     0,     0,    70,    88,    64,     0,    61,     0,    63,
      46,    24,    47,     0,     0,    35,    30,     0,    26,     0,
       0,     0,     0,   109,     0,     0,     0,   110,   111,    72,
       0,   108,   102,     0,    92,    93,    99,   101,     0,     0,
       0,     0,     0,    71,    59,    70,     0,    37,     0,     0,
       0,    41,    39,     0,     0,     0,    54,    55,    56,     0,
      50,   107,    94,    95,     0,   105,    70,     0,     0,     0,
       0,     0,    78,    70,     0,     0,     0,    90,     0,     0,
      73,    60,    62,     0,    36,    34,     0,    57,     0,    52,
       0,     0,     0,   104,     0,    76,    91,    96,    97,    98,
     100,    84,     0,    79,    81,     0,    67,     0,    68,    87,
       0,    75,     0,    48,    29,    58,    53,    49,   103,     0,
      70,    65,    66,     0,     0,    70,    85,    86,     0,    89,
      74,   106,    77,    80,    83,    82,     0,    70,    69
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,  -128,  -128,  -128,  -128,    -3,   142,  -128,   -21,
     -23,   134,  -128,   -51,  -128,  -123,  -128,    85,    88,   162,
    -128,  -128,  -128,  -128,  -128,   141,  -128,    46,  -128,    87,
     -39,   139,  -118,   -63,  -128,  -128,  -128,  -128,  -128,    18,
    -128,  -128,  -128,  -127,   -76,    77,   -83,   -91,  -128,  -128
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,     6,     9,   134,    10,    15,    93,
     111,    18,    26,    56,    70,    57,    94,    95,    28,    43,
      44,    45,    65,    92,    66,   101,   135,   136,   137,   138,
      85,    86,    87,   112,   123,   160,   201,   182,   183,   184,
     185,   208,    89,   156,   157,   114,   115,   116,   174,   117
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    88,    37,   113,   118,   119,    67,   161,    74,    21,
       1,    49,    51,   141,   133,   -43,   171,   172,    31,    32,
      88,    98,   142,   143,    42,    31,    32,     5,   175,    38,
      11,    68,   192,    53,   145,   186,    54,    62,    63,    53,
       7,    55,    54,    20,    48,   195,    38,    55,   132,    12,
     162,    50,    42,    20,    31,    32,    33,    75,    13,    30,
     180,    64,    88,    73,    59,   177,   178,   179,    29,   131,
      30,   124,    31,    32,   103,    34,    35,   155,   187,   188,
      36,    79,   212,    88,    80,   125,    81,   215,    82,    83,
      88,    64,   104,   125,    75,     8,   107,   108,   109,   218,
     110,    53,    84,    19,    54,    38,    38,   164,    30,    55,
      16,   158,   -72,    99,   209,   194,   -72,   122,   159,    31,
      32,   103,    30,   -72,   193,   204,   189,   168,   190,    38,
     205,   181,   216,   198,   129,   190,    14,    88,   130,   104,
     105,   106,    88,   107,   108,   109,   190,   110,   147,   148,
     210,    17,   149,   150,    88,   148,   169,   170,   149,   150,
     206,   207,    20,    22,    23,    24,    25,    27,    39,    41,
      47,    40,    58,    46,    61,    69,    77,    52,    60,    76,
      38,    38,   181,   214,    72,    78,    90,    96,    97,   100,
     120,   126,   128,   139,   144,   140,   146,   151,   152,   153,
     154,   166,    54,   173,   199,   191,    64,   200,    91,   202,
     197,   203,   211,   217,   165,   163,   196,    71,     0,   102,
     167,   213,   121,     0,   176,   127
};

static const yytype_int16 yycheck[] =
{
      23,    64,    23,    79,    80,    81,    45,   125,    59,    12,
       3,    34,    35,   104,    10,     1,   139,   144,    13,    14,
      83,    72,   105,   106,    27,    13,    14,    39,   146,    52,
       1,    52,   159,     9,   110,   153,    12,     4,     5,     9,
       0,    17,    12,    39,    39,   168,    69,    17,    99,    41,
     126,    39,    55,    39,    13,    14,    15,    60,     1,    44,
     151,    28,   125,    39,    49,   148,   149,   150,    42,    39,
      44,    29,    13,    14,    15,    34,    35,    27,   154,   155,
      39,    18,   200,   146,    21,    43,    23,   205,    25,    26,
     153,    28,    33,    43,    97,    16,    37,    38,    39,   217,
      41,     9,    39,    43,    12,   128,   129,   128,    44,    17,
      40,    40,    36,    49,   190,   166,    40,    41,    47,    13,
      14,    15,    44,    47,   163,    44,    42,    49,    44,   152,
      49,   152,   208,    42,    44,    44,    39,   200,    48,    33,
      34,    35,   205,    37,    38,    39,    44,    41,    30,    31,
      48,    11,    34,    35,   217,    31,    42,    43,    34,    35,
       6,     7,    39,    43,    45,    43,    39,    10,    39,    43,
      39,    46,    39,    43,     1,    47,    39,    45,    43,    43,
     203,   204,   203,   204,    46,    39,    43,    29,    43,    41,
      39,    36,     8,    49,    41,    43,    19,    32,    22,    24,
      36,    22,    12,    42,     1,    39,    28,    20,    66,    29,
      43,    43,    43,    24,   129,   127,   170,    55,    -1,    78,
     133,   203,    83,    -1,   147,    91
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    51,    52,    53,    39,    54,     0,    16,    55,
      57,     1,    41,     1,    39,    58,    40,    11,    61,    43,
      39,    56,    43,    45,    43,    39,    62,    10,    68,    42,
      44,    13,    14,    15,    34,    35,    39,    59,    60,    39,
      46,    43,    56,    69,    70,    71,    43,    39,    39,    60,
      39,    60,    45,     9,    12,    17,    63,    65,    39,    49,
      43,     1,     4,     5,    28,    72,    74,    80,    59,    47,
      64,    69,    46,    39,    63,    56,    43,    39,    39,    18,
      21,    23,    25,    26,    39,    80,    81,    82,    83,    92,
      43,    57,    73,    59,    66,    67,    29,    43,    63,    49,
      41,    75,    75,    15,    33,    34,    35,    37,    38,    39,
      41,    60,    83,    94,    95,    96,    97,    99,    94,    94,
      39,    81,    41,    84,    29,    43,    36,    61,     8,    44,
      48,    39,    63,    10,    56,    76,    77,    78,    79,    49,
      43,    97,    96,    96,    41,    94,    19,    30,    31,    34,
      35,    32,    22,    24,    36,    27,    93,    94,    40,    47,
      85,    82,    94,    68,    59,    67,    22,    79,    49,    42,
      43,    65,    93,    42,    98,    82,    95,    96,    96,    96,
      97,    59,    87,    88,    89,    90,    82,    94,    94,    42,
      44,    39,    93,    80,    63,    65,    77,    43,    42,     1,
      20,    86,    29,    43,    44,    49,     6,     7,    91,    94,
      48,    43,    82,    89,    59,    82,    94,    24,    82
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    53,    52,    54,    52,    55,    56,
      56,    57,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    61,    61,    62,    62,    63,    63,
      63,    64,    64,    65,    66,    66,    67,    68,    68,    69,
      69,    69,    69,    70,    69,    71,    71,    72,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    78,    79,    80,
      81,    81,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    83,    84,    84,    85,    85,    86,    86,    87,    87,
      88,    88,    89,    90,    90,    91,    91,    92,    92,    93,
      93,    94,    94,    95,    95,    95,    95,    95,    95,    96,
      96,    97,    97,    97,    97,    98,    97,    97,    99,    99,
      99,    99
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     6,     0,     3,     0,     4,     5,     3,
       1,     0,     3,     5,     3,     2,     2,     1,     2,     1,
       2,     1,     1,     1,     0,     3,     5,     3,     1,     6,
       3,     0,     1,     1,     3,     1,     3,     0,     3,     5,
       3,     5,     3,     0,     3,     0,     3,     2,     4,     6,
       4,     0,     3,     3,     1,     1,     1,     2,     3,     3,
       3,     1,     3,     1,     1,     5,     5,     4,     4,     8,
       0,     2,     0,     2,     3,     2,     0,     2,     0,     1,
       3,     1,     3,     3,     1,     1,     1,     4,     1,     3,
       1,     3,     1,     1,     2,     2,     3,     3,     3,     1,
       3,     1,     1,     4,     3,     0,     5,     2,     1,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (real_ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, real_ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ast::AST *real_ast)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (real_ast);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ast::AST *real_ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, real_ast);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, ast::AST *real_ast)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], real_ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, real_ast); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, ast::AST *real_ast)
{
  YY_USE (yyvaluep);
  YY_USE (real_ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ast::AST *real_ast)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: program_head program_body '.'  */
#line 75 "scripts/parser.y"
    {   
        ProgramNode* node = new ProgramNode();
        // node->append_child(dynamic_cast<ProgramHeadNode*>($1));
        // node->append_child(dynamic_cast<ProgramBodyNode*>($2));
        node->append_child((yyvsp[-2].program_head_node));
        node->append_child((yyvsp[-1].program_body_node));
        real_ast->set_root(node);
        if(DEBUG) printf("program -> program_head program_body.\n");
    }
#line 1359 "src/parser.tab.cc"
    break;

  case 3: /* program_head: PROGRAM ID '(' id_list ')' ';'  */
#line 85 "scripts/parser.y"
                                   {
        (yyval.program_head_node) = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode((yyvsp[-4].token_info).value.m_OTHERS,false);
        (yyval.program_head_node)->append_child(leaf_node);
        //$$->append_child(dynamic_cast<IdListNode*>($4));
        (yyval.program_head_node)->append_child((yyvsp[-2].id_list_node));
        if(DEBUG) printf("program_head -> program id(id_list);\n");
    }
#line 1372 "src/parser.tab.cc"
    break;

  case 4: /* $@1: %empty  */
#line 93 "scripts/parser.y"
      {fresh_argu();}
#line 1378 "src/parser.tab.cc"
    break;

  case 5: /* program_head: $@1 error ';'  */
#line 93 "scripts/parser.y"
                              {
        yyerror_("Every program must begin with the symbol program.\n");
    }
#line 1386 "src/parser.tab.cc"
    break;

  case 6: /* $@2: %empty  */
#line 96 "scripts/parser.y"
              {column = buf.size();len = strlen(yytext);}
#line 1392 "src/parser.tab.cc"
    break;

  case 7: /* program_head: PROGRAM $@2 error ';'  */
#line 96 "scripts/parser.y"
                                                                   {
        yyerror_("An identifier is expected.\n");
    }
#line 1400 "src/parser.tab.cc"
    break;

  case 8: /* program_body: const_declarations type_declarations var_declarations subprogram_declarations compound_statement  */
#line 101 "scripts/parser.y"
                                               {
        (yyval.program_body_node) = new ProgramBodyNode();
        // $$->append_child(dynamic_cast<ConstDeclarationsNode*>($1));
        // $$->append_child(dynamic_cast<TypeDeclarationsNode*>($2));
        // $$->append_child(dynamic_cast<VariableDeclarationsNode*>($3));
        // $$->append_child(dynamic_cast<SubprogramDeclarationsNode*>($4));
        // $$->append_child(dynamic_cast<CompoundStatementNode*>($5));
        (yyval.program_body_node)->append_child((yyvsp[-4].const_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-3].type_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-2].variable_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-1].subprogram_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[0].compound_statement_node));
        if(DEBUG) printf("program_body -> const_declarations type_declarations var_declarations subprogram_declarations compound_statement\n");
    }
#line 1419 "src/parser.tab.cc"
    break;

  case 9: /* id_list: id_list ',' ID  */
#line 116 "scripts/parser.y"
                   { 
        (yyval.id_list_node) = new IdListNode((IdListNode::GrammarType)1);
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        //$$->append_child(dynamic_cast<IdListNode*>($1));
        (yyval.id_list_node)->append_child((yyvsp[-2].id_list_node));
        (yyval.id_list_node)->append_child(leaf_node);
    }
#line 1431 "src/parser.tab.cc"
    break;

  case 10: /* id_list: ID  */
#line 122 "scripts/parser.y"
           {   
        (yyval.id_list_node) = new IdListNode((IdListNode::GrammarType)0);
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.id_list_node)->append_child(leaf_node);
    }
#line 1441 "src/parser.tab.cc"
    break;

  case 11: /* const_declarations: %empty  */
#line 127 "scripts/parser.y"
                    {
        (yyval.const_declarations_node) = new ConstDeclarationsNode();
    }
#line 1449 "src/parser.tab.cc"
    break;

  case 12: /* const_declarations: CONST const_declaration ';'  */
#line 131 "scripts/parser.y"
    {   
        // const_declarations -> const const_declaration 
        if(DEBUG) printf("const_declarations -> const const_declaration\n");
    }
#line 1458 "src/parser.tab.cc"
    break;

  case 13: /* const_declaration: const_declaration ';' ID "=" const_variable  */
#line 137 "scripts/parser.y"
    {
        //TODO 插入符号表
        //new(ID.name, ID.type, ID.decl_line,ID.val)
        // const_declaration -> const_declaration ; id = const_variable.
    }
#line 1468 "src/parser.tab.cc"
    break;

  case 14: /* const_declaration: ID "=" const_variable  */
#line 143 "scripts/parser.y"
    {   //TODO 插入符号表
        //new(ID.name, $3.type, ID.decl_line,$3.val)
        // Insert(ID.name, tmp)
        //检查是否重复定义
        // const_declaration -> id = const_variable.
    }
#line 1479 "src/parser.tab.cc"
    break;

  case 15: /* const_variable: PLUS ID  */
#line 151 "scripts/parser.y"
    {   
        //TODO
        // const_variable -> + id.
    }
#line 1488 "src/parser.tab.cc"
    break;

  case 16: /* const_variable: UMINUS ID  */
#line 156 "scripts/parser.y"
    {
        //TODO
        // const_variable -> - id.
    }
#line 1497 "src/parser.tab.cc"
    break;

  case 17: /* const_variable: ID  */
#line 161 "scripts/parser.y"
    {
        // const_variable -> id.
    }
#line 1505 "src/parser.tab.cc"
    break;

  case 18: /* const_variable: UMINUS num  */
#line 165 "scripts/parser.y"
    {   
        //TODO
        // const_variable -> - num.
    }
#line 1514 "src/parser.tab.cc"
    break;

  case 19: /* const_variable: num  */
#line 170 "scripts/parser.y"
    {   
        // const_variable -> num.
    }
#line 1522 "src/parser.tab.cc"
    break;

  case 20: /* const_variable: PLUS num  */
#line 174 "scripts/parser.y"
    {  
        // const_variable -> +num.
    }
#line 1530 "src/parser.tab.cc"
    break;

  case 21: /* const_variable: CHAR  */
#line 178 "scripts/parser.y"
    {
        // const_variable -> 'letter'.
    }
#line 1538 "src/parser.tab.cc"
    break;

  case 22: /* num: INT_NUM  */
#line 184 "scripts/parser.y"
    {
        //$$.num = $1; TODO
        // num -> int_num.
    }
#line 1547 "src/parser.tab.cc"
    break;

  case 23: /* num: REAL_NUM  */
#line 189 "scripts/parser.y"
    {   
        //$$.real_num = $1; TODO
        // num -> real_num.
    }
#line 1556 "src/parser.tab.cc"
    break;

  case 24: /* type_declarations: %empty  */
#line 193 "scripts/parser.y"
                    {
        // type_declarations -> empty.
        (yyval.type_declarations_node) = new TypeDeclarationsNode();
    }
#line 1565 "src/parser.tab.cc"
    break;

  case 25: /* type_declarations: TYPE type_declaration ';'  */
#line 198 "scripts/parser.y"
    {
        // type_declarations -> type type_declaration.
    }
#line 1573 "src/parser.tab.cc"
    break;

  case 26: /* type_declaration: type_declaration ';' ID '=' type  */
#line 203 "scripts/parser.y"
    {
        // TODO!
        // type_declaration -> type_declaration ; id = type.
    }
#line 1582 "src/parser.tab.cc"
    break;

  case 27: /* type_declaration: ID '=' type  */
#line 208 "scripts/parser.y"
    {
        // TODO!
        // type_declaration -> id = type.
    }
#line 1591 "src/parser.tab.cc"
    break;

  case 28: /* type: standrad_type  */
#line 214 "scripts/parser.y"
    {
        // TODO
        // $$ = $1 
        // type -> standrad_type.
    }
#line 1601 "src/parser.tab.cc"
    break;

  case 29: /* type: ARRAY '[' periods ']' OF type  */
#line 220 "scripts/parser.y"
    {
        // TODO
        // $$ = new ArrayType($6, $3);
        // type -> array [periods] of stype.
    }
#line 1611 "src/parser.tab.cc"
    break;

  case 30: /* type: RECORD record_body END  */
#line 226 "scripts/parser.y"
    {
        // TODO
        // $$ = new RecordType($2);
        // type -> record record_body end.
    }
#line 1621 "src/parser.tab.cc"
    break;

  case 31: /* record_body: %empty  */
#line 232 "scripts/parser.y"
    {
        // record_body -> empty.
    }
#line 1629 "src/parser.tab.cc"
    break;

  case 32: /* record_body: var_declaration  */
#line 236 "scripts/parser.y"
    {
        // TODO
        // $$ = $1; std::unordered_map<std::string, TypeTemplate*>
        // record_body -> var_declaration.
    }
#line 1639 "src/parser.tab.cc"
    break;

  case 33: /* standrad_type: BASIC_TYPE  */
#line 243 "scripts/parser.y"
    {
        // todo
        //if($1 == "integer")
        //$$ = INT_PTR;
        // standrad_type -> int|real|bool|char.
    }
#line 1650 "src/parser.tab.cc"
    break;

  case 34: /* periods: periods ',' period  */
#line 251 "scripts/parser.y"
    {
        // $1->insert($1->end(), $3->begin(), $3->end());
        // delete $3;
        // $$ = $1;
        // periods -> periods,period.
    }
#line 1661 "src/parser.tab.cc"
    break;

  case 35: /* periods: period  */
#line 258 "scripts/parser.y"
    {
        //$$ = $1;
        // periods -> period.
    }
#line 1670 "src/parser.tab.cc"
    break;

  case 36: /* period: const_variable SUBCATALOG const_variable  */
#line 264 "scripts/parser.y"
    {
        // TODO
        // test type
        // $$ = new std::vector<std::pair<int, int>>={$1, $3}; 
        // period -> const_variable .. const_variable.
    }
#line 1681 "src/parser.tab.cc"
    break;

  case 37: /* var_declarations: %empty  */
#line 272 "scripts/parser.y"
    {
        // var_declarations -> empty.
        (yyval.variable_declarations_node) = new VariableDeclarationsNode();
    }
#line 1690 "src/parser.tab.cc"
    break;

  case 38: /* var_declarations: VAR var_declaration ';'  */
#line 277 "scripts/parser.y"
    {
        // var_declarations -> var var_declaration.
    }
#line 1698 "src/parser.tab.cc"
    break;

  case 39: /* var_declaration: var_declaration ';' id_list ':' type  */
#line 282 "scripts/parser.y"
    {
        // TODO 插入符号表
        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // $$->insert($3[i], $4);
        // var_declaration -> var_declaration ; id_list : type.
    }
#line 1709 "src/parser.tab.cc"
    break;

  case 40: /* var_declaration: id_list ':' type  */
#line 289 "scripts/parser.y"
    {
        // TODO 插入符号表
        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // $$->insert($1[i], $3);
        // var_declaration -> id : type.
    }
#line 1720 "src/parser.tab.cc"
    break;

  case 41: /* var_declaration: var_declaration ';' id_list ':' ID  */
#line 296 "scripts/parser.y"
    {
        //TODO:插入符号表
        //TYPE = TypeSymbolTable.Find($.name)???
        //symboltable.insert(id_list[i].first,TYPE)

        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // T *Find(std::string name);
        // $$->insert($3[i], T);
    }
#line 1734 "src/parser.tab.cc"
    break;

  case 42: /* var_declaration: id_list ':' ID  */
#line 306 "scripts/parser.y"
    {
        //TODO:插入符号表
        //TYPE = TypeSymbolTable.Find(ID.name)???
        //symboltable.insert(id_list[i].first,TYPE)

        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // T *Find(std::string name);
        // $$->insert($1[i], T);
    }
#line 1748 "src/parser.tab.cc"
    break;

  case 43: /* $@3: %empty  */
#line 315 "scripts/parser.y"
      {column = buf.size(); len = strlen(yytext);}
#line 1754 "src/parser.tab.cc"
    break;

  case 44: /* var_declaration: $@3 error ';'  */
#line 316 "scripts/parser.y"
    {
        yyerror_("An identifier is expected.");
        // if ( DEBUG ) printf("An identifier is expected.\n");
    }
#line 1763 "src/parser.tab.cc"
    break;

  case 45: /* subprogram_declarations: %empty  */
#line 320 "scripts/parser.y"
                          {
        // subprogram_declarations -> empty.
        (yyval.subprogram_declarations_node) = new SubprogramDeclarationsNode();
    }
#line 1772 "src/parser.tab.cc"
    break;

  case 46: /* subprogram_declarations: subprogram_declarations subprogram_declaration ';'  */
#line 325 "scripts/parser.y"
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration.
    }
#line 1780 "src/parser.tab.cc"
    break;

  case 47: /* subprogram_declaration: subprogram_head subprogram_body  */
#line 330 "scripts/parser.y"
    {
        // subprogram_declaration -> subprogram_head program_body.
    }
#line 1788 "src/parser.tab.cc"
    break;

  case 48: /* subprogram_body: const_declarations type_declarations var_declarations compound_statement  */
#line 335 "scripts/parser.y"
    {
        
    }
#line 1796 "src/parser.tab.cc"
    break;

  case 49: /* subprogram_head: FUNCTION ID formal_parameter ':' standrad_type ';'  */
#line 340 "scripts/parser.y"
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,$5,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> function id formal_parametert : standrad_type.
    }
#line 1807 "src/parser.tab.cc"
    break;

  case 50: /* subprogram_head: PROCEDURE ID formal_parameter ';'  */
#line 347 "scripts/parser.y"
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,NULL,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> procedure id formal_parametert.
    }
#line 1818 "src/parser.tab.cc"
    break;

  case 51: /* formal_parameter: %empty  */
#line 354 "scripts/parser.y"
    {   
        // formal_parameter -> empty.
    }
#line 1826 "src/parser.tab.cc"
    break;

  case 52: /* formal_parameter: '(' parameter_lists ')'  */
#line 358 "scripts/parser.y"
    {
        // formal_parameter -> (parameter_lists).
    }
#line 1834 "src/parser.tab.cc"
    break;

  case 53: /* parameter_lists: parameter_lists ';' parameter_list  */
#line 363 "scripts/parser.y"
    {   
        // parameter_lists -> parameter_lists ; parameter_list.
    }
#line 1842 "src/parser.tab.cc"
    break;

  case 54: /* parameter_lists: parameter_list  */
#line 367 "scripts/parser.y"
    {  
        // parameter_lists -> parameter_list.
    }
#line 1850 "src/parser.tab.cc"
    break;

  case 55: /* parameter_list: var_parameter  */
#line 372 "scripts/parser.y"
    {   
        // parameter_list -> var_parameter.
    }
#line 1858 "src/parser.tab.cc"
    break;

  case 56: /* parameter_list: value_parameter  */
#line 376 "scripts/parser.y"
    {   
        // parameter_list -> value_parameter.
    }
#line 1866 "src/parser.tab.cc"
    break;

  case 57: /* var_parameter: VAR value_parameter  */
#line 381 "scripts/parser.y"
    {   
        // var_parameter -> var value_parameter.
    }
#line 1874 "src/parser.tab.cc"
    break;

  case 58: /* value_parameter: id_list ':' standrad_type  */
#line 386 "scripts/parser.y"
    {   
        // value_parameter -> id_list : standrad_type.
    }
#line 1882 "src/parser.tab.cc"
    break;

  case 59: /* compound_statement: BEGIN_ statement_list END  */
#line 390 "scripts/parser.y"
                              {
        // compound_statement -> begin statement_list end.
        (yyval.compound_statement_node) = new CompoundStatementNode();
        //$$->append_child(dynamic_cast<StatementListNode*>($2));
        (yyval.compound_statement_node)->append_child((yyvsp[-1].statement_list_node));
        if (DEBUG) printf("compound_statement -> begin statement_list end.\n");
    }
#line 1894 "src/parser.tab.cc"
    break;

  case 60: /* statement_list: statement_list ';' statement  */
#line 399 "scripts/parser.y"
    {
        // TODO
        // statement_list -> statement_list ; statement.
    }
#line 1903 "src/parser.tab.cc"
    break;

  case 61: /* statement_list: statement  */
#line 403 "scripts/parser.y"
    {
        // statement_list -> statement.
        (yyval.statement_list_node) = new StatementListNode();
        //$$->append_child(dynamic_cast<StatementNode*>($1));
        (yyval.statement_list_node)->append_child((yyvsp[0].statement_node));
    }
#line 1914 "src/parser.tab.cc"
    break;

  case 62: /* statement: variable ASSIGNOP expression  */
#line 411 "scripts/parser.y"
    {   // 类型检查
        // statement -> variable assigbop expression.
    }
#line 1922 "src/parser.tab.cc"
    break;

  case 63: /* statement: call_procedure_statement  */
#line 415 "scripts/parser.y"
    {
        // statement -> call_procedure_statement.
    }
#line 1930 "src/parser.tab.cc"
    break;

  case 64: /* statement: compound_statement  */
#line 419 "scripts/parser.y"
    {
        // statement -> compound_statement.
    }
#line 1938 "src/parser.tab.cc"
    break;

  case 65: /* statement: IF expression THEN statement else_part  */
#line 423 "scripts/parser.y"
    {   
        //类型检查
        // statement -> if expression then statement else_part.
    }
#line 1947 "src/parser.tab.cc"
    break;

  case 66: /* statement: CASE expression OF case_body END  */
#line 428 "scripts/parser.y"
    {
        //类型检查
        // statement -> case expression of case_body end.
    }
#line 1956 "src/parser.tab.cc"
    break;

  case 67: /* statement: WHILE expression DO statement  */
#line 433 "scripts/parser.y"
    {
        //类型检查
        // statement -> while expression do if_statement_1.
    }
#line 1965 "src/parser.tab.cc"
    break;

  case 68: /* statement: REPEAT statement_list UNTIL expression  */
#line 438 "scripts/parser.y"
    {
        //类型检查
        // statement -> repeat statement_list until expression.
    }
#line 1974 "src/parser.tab.cc"
    break;

  case 69: /* statement: FOR ID ASSIGNOP expression updown expression DO statement  */
#line 443 "scripts/parser.y"
    {
        //类型检查
        // statement -> for id assignop expression updown expression do statement.
    }
#line 1983 "src/parser.tab.cc"
    break;

  case 70: /* statement: %empty  */
#line 446 "scripts/parser.y"
        {
        // statement -> empty.
        StatementNode *node = new StatementNode((StatementNode::GrammarType)0);
        (yyval.statement_node) = node;
    }
#line 1993 "src/parser.tab.cc"
    break;

  case 71: /* variable: ID id_varparts  */
#line 454 "scripts/parser.y"
    {
        //类型检查
        // variable -> id id_varparts.
    }
#line 2002 "src/parser.tab.cc"
    break;

  case 72: /* id_varparts: %empty  */
#line 460 "scripts/parser.y"
    {
        // id_varparts -> empty.
    }
#line 2010 "src/parser.tab.cc"
    break;

  case 73: /* id_varparts: id_varparts id_varpart  */
#line 464 "scripts/parser.y"
    {
        // $$ = $1;
        // $$->insert($$->end(), $2->begin(), $2->end());
        // id_varparts -> id_varparts id_varpart.
    }
#line 2020 "src/parser.tab.cc"
    break;

  case 74: /* id_varpart: '[' expression_list ']'  */
#line 472 "scripts/parser.y"
    {   
        // $$ = $1; 
        // id_varpart -> [expression_list].
    }
#line 2029 "src/parser.tab.cc"
    break;

  case 75: /* id_varpart: '.' ID  */
#line 477 "scripts/parser.y"
    {
        // id_varpart -> .id.
    }
#line 2037 "src/parser.tab.cc"
    break;

  case 76: /* else_part: %empty  */
#line 481 "scripts/parser.y"
    {
        // else_part -> empty.
    }
#line 2045 "src/parser.tab.cc"
    break;

  case 77: /* else_part: ELSE statement  */
#line 485 "scripts/parser.y"
    {
        // else_part -> else statement.
    }
#line 2053 "src/parser.tab.cc"
    break;

  case 78: /* case_body: %empty  */
#line 489 "scripts/parser.y"
    {
        // case_body -> empty.
    }
#line 2061 "src/parser.tab.cc"
    break;

  case 79: /* case_body: branch_list  */
#line 493 "scripts/parser.y"
    {
        // case_body -> branch_list.
    }
#line 2069 "src/parser.tab.cc"
    break;

  case 80: /* branch_list: branch_list ';' branch  */
#line 498 "scripts/parser.y"
    {
        // branch_list -> branch_list branch.
    }
#line 2077 "src/parser.tab.cc"
    break;

  case 81: /* branch_list: branch  */
#line 502 "scripts/parser.y"
    {
        // branch_list -> branch.
    }
#line 2085 "src/parser.tab.cc"
    break;

  case 82: /* branch: const_list ':' statement  */
#line 507 "scripts/parser.y"
    {
        // branch -> const_list : statement.
    }
#line 2093 "src/parser.tab.cc"
    break;

  case 83: /* const_list: const_list ',' const_variable  */
#line 512 "scripts/parser.y"
    {
        // const_list -> const_list , const_variable.
    }
#line 2101 "src/parser.tab.cc"
    break;

  case 84: /* const_list: const_variable  */
#line 516 "scripts/parser.y"
    {
        // const_list -> const_variable.
    }
#line 2109 "src/parser.tab.cc"
    break;

  case 85: /* updown: TO  */
#line 521 "scripts/parser.y"
    {
        // updown -> to.
    }
#line 2117 "src/parser.tab.cc"
    break;

  case 86: /* updown: DOWNTO  */
#line 525 "scripts/parser.y"
    {
        // updown -> downto.
    }
#line 2125 "src/parser.tab.cc"
    break;

  case 87: /* call_procedure_statement: ID '(' expression_list ')'  */
#line 530 "scripts/parser.y"
    {
        //类型检查
        // call_procedure_statement -> id (expression_list).
    }
#line 2134 "src/parser.tab.cc"
    break;

  case 88: /* call_procedure_statement: ID  */
#line 535 "scripts/parser.y"
    {   //类型检查
        // call_procedure_statement -> id.
    }
#line 2142 "src/parser.tab.cc"
    break;

  case 89: /* expression_list: expression_list ',' expression  */
#line 540 "scripts/parser.y"
    {
        //类型检查 检查是否为INT or CHAR
        // $$ = $1;
        // $$->push_back($3);
        // expression_list -> expression_list , expression.
    }
#line 2153 "src/parser.tab.cc"
    break;

  case 90: /* expression_list: expression  */
#line 547 "scripts/parser.y"
    {
        //类型检查 检查是否为INT or CHAR    
        // $$ = new vector<BasicType*>;
        // $$->push_back($1);
        // expression_list -> expression.
    }
#line 2164 "src/parser.tab.cc"
    break;

  case 91: /* expression: simple_expression RELOP simple_expression  */
#line 555 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // expression -> simple_expression relop simple_expression.
    }
#line 2175 "src/parser.tab.cc"
    break;

  case 92: /* expression: simple_expression  */
#line 562 "scripts/parser.y"
    {
        // $$ = $1;
        // expression -> simple_expression.
    }
#line 2184 "src/parser.tab.cc"
    break;

  case 93: /* simple_expression: term  */
#line 568 "scripts/parser.y"
    {   // $$ = $1;
        // simple_expression -> term.
    }
#line 2192 "src/parser.tab.cc"
    break;

  case 94: /* simple_expression: PLUS term  */
#line 572 "scripts/parser.y"
    {

    }
#line 2200 "src/parser.tab.cc"
    break;

  case 95: /* simple_expression: UMINUS term  */
#line 576 "scripts/parser.y"
    {

    }
#line 2208 "src/parser.tab.cc"
    break;

  case 96: /* simple_expression: simple_expression ADDOP term  */
#line 580 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression or term.
    }
#line 2219 "src/parser.tab.cc"
    break;

  case 97: /* simple_expression: simple_expression PLUS term  */
#line 587 "scripts/parser.y"
    { 
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression + term.
    }
#line 2230 "src/parser.tab.cc"
    break;

  case 98: /* simple_expression: simple_expression UMINUS term  */
#line 594 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression - term.
    }
#line 2241 "src/parser.tab.cc"
    break;

  case 99: /* term: factor  */
#line 602 "scripts/parser.y"
    {   // $$ = $1;
        // term -> factor.
    }
#line 2249 "src/parser.tab.cc"
    break;

  case 100: /* term: term MULOP factor  */
#line 606 "scripts/parser.y"
    {   
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // term -> term mulop factor.
    }
#line 2260 "src/parser.tab.cc"
    break;

  case 101: /* factor: unsigned_const_variable  */
#line 614 "scripts/parser.y"
    {
        // $$ = $1;
        // factor -> unsigned_const_variable.
    }
#line 2269 "src/parser.tab.cc"
    break;

  case 102: /* factor: variable  */
#line 619 "scripts/parser.y"
    {
        // $$ = $1;
        // factor -> variable.
    }
#line 2278 "src/parser.tab.cc"
    break;

  case 103: /* factor: ID '(' expression_list ')'  */
#line 624 "scripts/parser.y"
    {
        // $$ = $1;
        // factor -> id (expression_list).
    }
#line 2287 "src/parser.tab.cc"
    break;

  case 104: /* factor: '(' expression ')'  */
#line 629 "scripts/parser.y"
    {
        // $$ = $2;
        // factor -> (expression).
    }
#line 2296 "src/parser.tab.cc"
    break;

  case 105: /* $@4: %empty  */
#line 633 "scripts/parser.y"
                     {column = buf.size();len = strlen(yytext);}
#line 2302 "src/parser.tab.cc"
    break;

  case 106: /* factor: '(' expression $@4 error ';'  */
#line 634 "scripts/parser.y"
    {
        yyerror_("A closing parenthesis is expected.\n");
    }
#line 2310 "src/parser.tab.cc"
    break;

  case 107: /* factor: NOT factor  */
#line 638 "scripts/parser.y"
    {
        // $$ = $2;
        // factor -> not factor.
    }
#line 2319 "src/parser.tab.cc"
    break;

  case 108: /* unsigned_const_variable: num  */
#line 644 "scripts/parser.y"
    {
        //$$ = $1.type;
        // unsigned_const_variable -> num
    }
#line 2328 "src/parser.tab.cc"
    break;

  case 109: /* unsigned_const_variable: CHAR  */
#line 649 "scripts/parser.y"
    {
        //$$ = CHAR;
        // unsigned_const_variable -> 'LETTER'
    }
#line 2337 "src/parser.tab.cc"
    break;

  case 110: /* unsigned_const_variable: TRUE  */
#line 654 "scripts/parser.y"
    {
        //$$ = BOOL_ptr;
        // unsigned_const_variable -> true
    }
#line 2346 "src/parser.tab.cc"
    break;

  case 111: /* unsigned_const_variable: FALSE  */
#line 659 "scripts/parser.y"
    {   //$$ = BOOL_ptr;
        // unsigned_const_variable -> false
    }
#line 2354 "src/parser.tab.cc"
    break;


#line 2358 "src/parser.tab.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (real_ast, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, real_ast);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, real_ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (real_ast, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, real_ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, real_ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 663 "scripts/parser.y"

 

void yyerror(ast::AST* real_ast,char *msg){
    //fprintf(stderr,"error:%s\n",s);
}

int main(){
    AST *real_ast = new AST();
    if(!yyparse(real_ast)){
        printf("successful analysis");
        Compiler compiler;
        compiler.Compile(real_ast,"basic_test");
    }
}

int yywrap(){
    return 1;
}
