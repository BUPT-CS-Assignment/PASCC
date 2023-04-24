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
using namespace pascal_symbol;
using namespace pascal_type;
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
//symbol_table::SymbolTable *real_symbol_table = new symbol_table::SymbolTable();
std::stack<symbol_table::TableSet*> table_set_queue;
symbol_table::TableSet* top_table_set = new symbol_table::TableSet("main",nullptr);
//table_set_queue.push(top_table_set);
int error_flag=0;

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
    error_flag=1;
}

#define DEBUG 0


#line 116 "src/parser.tab.cc"

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
  YYSYMBOL_CONST = 12,                     /* CONST  */
  YYSYMBOL_RECORD = 13,                    /* RECORD  */
  YYSYMBOL_IF = 14,                        /* IF  */
  YYSYMBOL_THEN = 15,                      /* THEN  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_CASE = 17,                      /* CASE  */
  YYSYMBOL_OF = 18,                        /* OF  */
  YYSYMBOL_WHILE = 19,                     /* WHILE  */
  YYSYMBOL_DO = 20,                        /* DO  */
  YYSYMBOL_FOR = 21,                       /* FOR  */
  YYSYMBOL_REPEAT = 22,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 23,                     /* UNTIL  */
  YYSYMBOL_BEGIN_ = 24,                    /* BEGIN_  */
  YYSYMBOL_END = 25,                       /* END  */
  YYSYMBOL_ADDOP = 26,                     /* ADDOP  */
  YYSYMBOL_NOT = 27,                       /* NOT  */
  YYSYMBOL_PLUS = 28,                      /* PLUS  */
  YYSYMBOL_UMINUS = 29,                    /* UMINUS  */
  YYSYMBOL_ASSIGNOP = 30,                  /* ASSIGNOP  */
  YYSYMBOL_TRUE = 31,                      /* TRUE  */
  YYSYMBOL_FALSE = 32,                     /* FALSE  */
  YYSYMBOL_CONSTASSIGNOP = 33,             /* CONSTASSIGNOP  */
  YYSYMBOL_ID = 34,                        /* ID  */
  YYSYMBOL_CHAR = 35,                      /* CHAR  */
  YYSYMBOL_INT_NUM = 36,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 37,                  /* REAL_NUM  */
  YYSYMBOL_BASIC_TYPE = 38,                /* BASIC_TYPE  */
  YYSYMBOL_RELOP = 39,                     /* RELOP  */
  YYSYMBOL_MULOP = 40,                     /* MULOP  */
  YYSYMBOL_41_ = 41,                       /* '.'  */
  YYSYMBOL_42_ = 42,                       /* '('  */
  YYSYMBOL_43_ = 43,                       /* ')'  */
  YYSYMBOL_44_ = 44,                       /* ';'  */
  YYSYMBOL_45_ = 45,                       /* ','  */
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
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  112
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  221

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
      42,    43,     2,     2,    45,     2,    41,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    44,
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   153,   153,   169,   177,   177,   180,   180,   184,   195,
     202,   209,   212,   220,   250,   278,   304,   330,   351,   368,
     382,   399,   414,   421,   428,   433,   441,   474,   507,   519,
     533,   546,   552,   562,   579,   590,   600,   629,   634,   649,
     664,   678,   699,   720,   720,   725,   730,   741,   750,   760,
     795,   832,   838,   846,   856,   866,   875,   885,   898,   915,
     922,   929,   936,   949,   957,   964,   974,   982,   992,  1001,
    1013,  1020,  1041,  1047,  1065,  1076,  1089,  1094,  1102,  1108,
    1117,  1129,  1140,  1151,  1164,  1174,  1180,  1187,  1202,  1216,
    1228,  1240,  1261,  1276,  1286,  1295,  1306,  1317,  1333,  1347,
    1363,  1371,  1395,  1403,  1411,  1427,  1436,  1436,  1440,  1450,
    1466,  1476,  1486
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
  "CONST", "RECORD", "IF", "THEN", "ELSE", "CASE", "OF", "WHILE", "DO",
  "FOR", "REPEAT", "UNTIL", "BEGIN_", "END", "ADDOP", "NOT", "PLUS",
  "UMINUS", "ASSIGNOP", "TRUE", "FALSE", "CONSTASSIGNOP", "ID", "CHAR",
  "INT_NUM", "REAL_NUM", "BASIC_TYPE", "RELOP", "MULOP", "'.'", "'('",
  "')'", "';'", "','", "'='", "'['", "']'", "':'", "$accept", "program",
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

#define YYPACT_NINF (-127)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-73)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      15,    14,    32,    62,    80,    61,    97,  -127,    53,    69,
     118,    87,   126,   123,   122,   125,  -127,   136,   153,  -127,
    -127,    66,  -127,   117,   137,   127,   128,   126,  -127,   130,
     141,    90,   113,  -127,  -127,  -127,  -127,  -127,  -127,   131,
      17,   142,     0,   134,   178,    88,  -127,  -127,  -127,  -127,
    -127,  -127,   117,   133,    13,  -127,  -127,  -127,   135,     6,
     126,   143,   152,   154,    54,   145,    62,  -127,  -127,   117,
     165,   147,    17,  -127,  -127,    37,  -127,   150,   150,    86,
      86,    86,   160,    54,    55,  -127,    10,  -127,   163,  -127,
    -127,   118,  -127,   187,     8,  -127,  -127,   126,  -127,    29,
       7,   148,   155,   106,   106,   106,  -127,  -127,   156,  -127,
      86,  -127,  -127,   181,    31,   161,  -127,  -127,   182,   183,
     172,    -7,    86,   -14,  -127,    54,    86,   153,   117,   117,
     186,  -127,  -127,   126,    45,    56,  -127,  -127,  -127,   167,
    -127,  -127,   161,   161,    86,   164,    54,   106,   106,   106,
      86,    86,   106,   117,    54,    86,    86,   116,  -127,   174,
      86,  -127,  -127,  -127,   185,  -127,  -127,    17,  -127,   167,
    -127,     7,   162,   119,  -127,   209,   195,   161,   161,   161,
     129,   129,  -127,  -127,   188,   168,  -127,    59,  -127,   159,
    -127,  -127,    86,  -127,    24,  -127,  -127,  -127,  -127,  -127,
    -127,   170,    54,  -127,  -127,   117,   117,    54,  -127,  -127,
      86,  -127,  -127,  -127,  -127,  -127,  -127,  -127,   196,    54,
    -127
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     6,     0,    11,     0,     0,     0,     1,     0,     0,
      24,     0,     0,     0,     0,     0,     2,     0,    37,     5,
      10,     0,     7,     0,    12,     0,     0,    43,    45,     0,
       0,     0,     0,    17,    21,    22,    23,    14,    19,     0,
       0,    25,     0,     0,     0,     0,     3,     9,    15,    20,
      16,    18,     0,     0,    31,    33,    27,    28,     0,     0,
      38,     0,     0,     0,    70,     0,    11,     8,    13,     0,
       0,    32,     0,    42,    40,     0,    44,    51,    51,     0,
       0,     0,     0,    70,    88,    64,     0,    61,     0,    63,
      46,    24,    47,     0,     0,    35,    30,     0,    26,     0,
       0,     0,     0,     0,     0,     0,   111,   112,    72,   110,
       0,   109,   103,     0,    93,    94,   100,   102,     0,     0,
       0,     0,     0,    71,    59,    70,     0,    37,     0,     0,
       0,    41,    39,     0,     0,     0,    54,    55,    56,     0,
      50,   108,    95,    96,     0,   106,    70,     0,     0,     0,
       0,     0,     0,    78,    70,     0,     0,     0,    90,     0,
       0,    73,    60,    62,     0,    36,    34,     0,    57,     0,
      52,     0,     0,     0,   105,     0,    76,    97,    98,    99,
      91,    92,   101,    84,     0,    79,    81,     0,    67,     0,
      68,    87,     0,    75,     0,    48,    29,    58,    53,    49,
     104,     0,    70,    65,    66,     0,     0,    70,    85,    86,
       0,    89,    74,   107,    77,    80,    83,    82,     0,    70,
      69
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -127,  -127,  -127,  -127,  -127,  -127,    -2,   149,  -127,   -21,
     -23,   132,  -127,   -48,  -127,  -126,  -127,    89,    92,   166,
    -127,  -127,  -127,  -127,  -127,   139,  -127,    50,  -127,    91,
     -39,   144,  -118,   -63,  -127,  -127,  -127,  -127,  -127,    20,
    -127,  -127,  -127,  -121,   -76,   -15,   -83,   -91,  -127,  -127
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,     6,     9,   134,    10,    15,    93,
     111,    18,    26,    56,    70,    57,    94,    95,    28,    43,
      44,    45,    65,    92,    66,   101,   135,   136,   137,   138,
      85,    86,    87,   112,   123,   161,   203,   184,   185,   186,
     187,   210,    89,   157,   158,   114,   115,   116,   175,   117
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    88,    37,   113,   118,   119,    67,   162,    49,    51,
      21,    74,   141,   172,   -43,    53,   156,   133,     1,    54,
      88,   142,   143,   173,    98,    42,    53,   159,   176,    38,
      54,    68,     7,   160,   145,   124,   188,   125,    53,   194,
      73,    20,    54,   197,    55,    30,    38,    20,     5,    59,
     163,   132,    42,   129,   125,    55,   130,   147,    75,   148,
     149,   182,    88,   131,   177,   178,   179,    55,    79,   192,
     150,    80,   212,    81,     8,    82,    83,   151,    64,   189,
     190,    11,    30,    88,   214,   -72,    99,    14,    84,   217,
      30,    88,    62,    63,   169,    75,   -72,   122,    13,   170,
     171,   220,   -72,    12,   206,    38,    38,   165,   207,    29,
      16,    30,    64,   103,   104,   105,   211,   106,   107,   196,
     108,   109,    35,    36,    48,   195,    35,    36,   110,    17,
      38,    19,   183,   103,   218,   180,   181,   106,   107,    88,
     108,   109,    35,    36,    88,    31,    32,    50,   110,    35,
      36,    33,    34,    35,    36,   147,    88,   148,   149,   191,
      20,   192,   200,    27,   192,   208,   209,    22,    23,    24,
      25,    39,    41,    40,    46,    47,    58,    52,    60,    61,
      69,    72,    38,    38,   183,   216,    77,    76,    78,    90,
      96,    97,   100,   126,   120,   128,   146,   139,   144,   140,
     153,   152,   155,   154,   167,    55,   199,   174,   193,    64,
     201,   202,   205,   204,   213,    91,   219,   102,   166,   164,
      71,   198,     0,   127,   168,   215,     0,   121
};

static const yytype_int16 yycheck[] =
{
      23,    64,    23,    79,    80,    81,    45,   125,    31,    32,
      12,    59,   103,   139,     1,     9,    23,    10,     3,    13,
      83,   104,   105,   144,    72,    27,     9,    41,   146,    52,
      13,    52,     0,    47,   110,    25,   154,    44,     9,   160,
      34,    34,    13,   169,    38,    45,    69,    34,    34,    49,
     126,    99,    54,    45,    44,    38,    48,    26,    60,    28,
      29,   152,   125,    34,   147,   148,   149,    38,    14,    45,
      39,    17,    48,    19,    12,    21,    22,    46,    24,   155,
     156,     1,    45,   146,   202,    30,    49,    34,    34,   207,
      45,   154,     4,     5,    49,    97,    41,    42,     1,    43,
      44,   219,    47,    42,    45,   128,   129,   128,    49,    43,
      41,    45,    24,    27,    28,    29,   192,    31,    32,   167,
      34,    35,    36,    37,    34,   164,    36,    37,    42,    11,
     153,    44,   153,    27,   210,   150,   151,    31,    32,   202,
      34,    35,    36,    37,   207,    28,    29,    34,    42,    36,
      37,    34,    35,    36,    37,    26,   219,    28,    29,    43,
      34,    45,    43,    10,    45,     6,     7,    44,    46,    44,
      34,    34,    44,    46,    44,    34,    34,    46,    44,     1,
      47,    46,   205,   206,   205,   206,    34,    44,    34,    44,
      25,    44,    42,    30,    34,     8,    15,    49,    42,    44,
      18,    40,    30,    20,    18,    38,    44,    43,    34,    24,
       1,    16,    44,    25,    44,    66,    20,    78,   129,   127,
      54,   171,    -1,    91,   133,   205,    -1,    83
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    51,    52,    53,    34,    54,     0,    12,    55,
      57,     1,    42,     1,    34,    58,    41,    11,    61,    44,
      34,    56,    44,    46,    44,    34,    62,    10,    68,    43,
      45,    28,    29,    34,    35,    36,    37,    59,    60,    34,
      46,    44,    56,    69,    70,    71,    44,    34,    34,    60,
      34,    60,    46,     9,    13,    38,    63,    65,    34,    49,
      44,     1,     4,     5,    24,    72,    74,    80,    59,    47,
      64,    69,    46,    34,    63,    56,    44,    34,    34,    14,
      17,    19,    21,    22,    34,    80,    81,    82,    83,    92,
      44,    57,    73,    59,    66,    67,    25,    44,    63,    49,
      42,    75,    75,    27,    28,    29,    31,    32,    34,    35,
      42,    60,    83,    94,    95,    96,    97,    99,    94,    94,
      34,    81,    42,    84,    25,    44,    30,    61,     8,    45,
      48,    34,    63,    10,    56,    76,    77,    78,    79,    49,
      44,    97,    96,    96,    42,    94,    15,    26,    28,    29,
      39,    46,    40,    18,    20,    30,    23,    93,    94,    41,
      47,    85,    82,    94,    68,    59,    67,    18,    79,    49,
      43,    44,    65,    93,    43,    98,    82,    96,    96,    96,
      95,    95,    97,    59,    87,    88,    89,    90,    82,    94,
      94,    43,    45,    34,    93,    80,    63,    65,    77,    44,
      43,     1,    16,    86,    25,    44,    45,    49,     6,     7,
      91,    94,    48,    44,    82,    89,    59,    82,    94,    20,
      82
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
      93,    94,    94,    94,    95,    95,    95,    95,    95,    95,
      96,    96,    97,    97,    97,    97,    98,    97,    97,    99,
      99,    99,    99
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
       1,     3,     3,     1,     1,     2,     2,     3,     3,     3,
       1,     3,     1,     1,     4,     3,     0,     5,     2,     1,
       1,     1,     1
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
#line 154 "scripts/parser.y"
    {   
        ProgramNode* node = new ProgramNode();
        // node->append_child(dynamic_cast<ProgramHeadNode*>($1));
        // node->append_child(dynamic_cast<ProgramBodyNode*>($2));
        node->append_child((yyvsp[-2].program_head_node));
        node->append_child((yyvsp[-1].program_body_node));
        if(!error_flag){
            real_ast->set_root(node);
        } else {
            real_ast->set_root(nullptr); 
        }
        
        if(DEBUG) printf("program -> program_head program_body.\n");
    }
#line 1375 "src/parser.tab.cc"
    break;

  case 3: /* program_head: PROGRAM ID '(' id_list ')' ';'  */
#line 169 "scripts/parser.y"
                                   {
        (yyval.program_head_node) = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode((yyvsp[-4].token_info).value.m_OTHERS,false);
        (yyval.program_head_node)->append_child(leaf_node);
        (yyval.program_head_node)->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        if(DEBUG) printf("program_head -> program id(id_list);\n");
        table_set_queue.push(top_table_set);
    }
#line 1388 "src/parser.tab.cc"
    break;

  case 4: /* $@1: %empty  */
#line 177 "scripts/parser.y"
      {fresh_argu();}
#line 1394 "src/parser.tab.cc"
    break;

  case 5: /* program_head: $@1 error ';'  */
#line 177 "scripts/parser.y"
                              {
        yyerror_("Every program must begin with the symbol program.\n");
    }
#line 1402 "src/parser.tab.cc"
    break;

  case 6: /* $@2: %empty  */
#line 180 "scripts/parser.y"
              {column = buf.size();len = strlen(yytext);}
#line 1408 "src/parser.tab.cc"
    break;

  case 7: /* program_head: PROGRAM $@2 error ';'  */
#line 180 "scripts/parser.y"
                                                                   {
        yyerror_("An identifier is expected.\n");
    }
#line 1416 "src/parser.tab.cc"
    break;

  case 8: /* program_body: const_declarations type_declarations var_declarations subprogram_declarations compound_statement  */
#line 185 "scripts/parser.y"
                                               {
        (yyval.program_body_node) = new ProgramBodyNode();
        (yyval.program_body_node)->append_child((yyvsp[-4].const_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-3].type_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-2].variable_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[-1].subprogram_declarations_node));
        (yyval.program_body_node)->append_child((yyvsp[0].compound_statement_node));
        if(DEBUG) printf("program_body -> const_declarations type_declarations var_declarations subprogram_declarations compound_statement\n");
    }
#line 1430 "src/parser.tab.cc"
    break;

  case 9: /* id_list: id_list ',' ID  */
#line 195 "scripts/parser.y"
                   { 
        (yyvsp[-2].id_list_node_info).list_ref->push_back(std::make_pair((yyvsp[0].token_info).value.m_OTHERS,(yyvsp[0].token_info).line_num));
        (yyval.id_list_node_info).list_ref = (yyvsp[-2].id_list_node_info).list_ref;
        (yyval.id_list_node_info).id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.id_list_node_info).id_list_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.id_list_node_info).id_list_node->append_child(leaf_node);
    }
#line 1443 "src/parser.tab.cc"
    break;

  case 10: /* id_list: ID  */
#line 202 "scripts/parser.y"
           {
        (yyval.id_list_node_info).list_ref = new std::vector<std::pair<std::string,int>>();
        (yyval.id_list_node_info).list_ref->push_back(std::make_pair((yyvsp[0].token_info).value.m_OTHERS,(yyvsp[0].token_info).line_num));
        (yyval.id_list_node_info).id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.id_list_node_info).id_list_node->append_child(leaf_node);
    }
#line 1455 "src/parser.tab.cc"
    break;

  case 11: /* const_declarations: %empty  */
#line 209 "scripts/parser.y"
                    {
        (yyval.const_declarations_node) = new ConstDeclarationsNode();
    }
#line 1463 "src/parser.tab.cc"
    break;

  case 12: /* const_declarations: CONST const_declaration ';'  */
#line 213 "scripts/parser.y"
    {   
        // const_declarations -> const const_declaration
        (yyval.const_declarations_node) = new ConstDeclarationsNode(); 
        (yyval.const_declarations_node)->append_child((yyvsp[-1].const_declaration_node));
        if(DEBUG) printf("const_declarations -> const const_declaration\n");
    }
#line 1474 "src/parser.tab.cc"
    break;

  case 13: /* const_declaration: const_declaration ';' ID '=' const_variable  */
#line 221 "scripts/parser.y"
    {
        //TODO 插入符号表
        pascal_symbol::ConstSymbol *symbol ;
        if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_INT){
            symbol = new ConstSymbol((yyvsp[-2].token_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_INT);
        }
        else if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_REAL){
            symbol = new ConstSymbol((yyvsp[-2].token_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_REAL);
        }
        else if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_CHAR){
            symbol = new ConstSymbol((yyvsp[-2].token_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_CHAR);
        }else {
            symbol = new ConstSymbol((yyvsp[-2].token_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_BOOLEAN);
        }

        if(!table_set_queue.top()->Insert<ConstSymbol>((yyvsp[-2].token_info).value.m_OTHERS,symbol)){
            yyerror(real_ast,"The identifier has been declared.\n");
        } 
        else{
            (yyval.const_declaration_node) = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION);
            (yyval.const_declaration_node)->append_child((yyvsp[-4].const_declaration_node));
            LeafNode* leaf_node = new LeafNode((yyvsp[-2].token_info).value.m_OTHERS,false);
            (yyval.const_declaration_node)->append_child(leaf_node);
            (yyval.const_declaration_node)->append_child((yyvsp[0].value_node_info).const_variable_node);
            // const_declaration -> const_declaration ; id = const_variable.
            if(DEBUG) printf("const_declaration ->const_declaration ; id = const_variable.\n");
        }
        
    }
#line 1508 "src/parser.tab.cc"
    break;

  case 14: /* const_declaration: ID '=' const_variable  */
#line 251 "scripts/parser.y"
    {   // const_declaration -> id = const_variable.
        //TODO 插入符号表
        pascal_symbol::ConstSymbol *symbol ;
        if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_INT){
            symbol = new ConstSymbol((yyvsp[0].value_node_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_INT);
        }
        else if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_REAL){
            symbol = new ConstSymbol((yyvsp[0].value_node_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_REAL);
        }
        else if ((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_CHAR){
            symbol = new ConstSymbol((yyvsp[0].value_node_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_CHAR);
        }else {
            symbol = new ConstSymbol((yyvsp[0].value_node_info).value.m_OTHERS,(yyvsp[-2].token_info).line_num,(yyvsp[0].value_node_info).value.m_BOOLEAN);
        }

        if(!table_set_queue.top()->Insert<ConstSymbol>((yyvsp[-2].token_info).value.m_OTHERS,symbol)){
            yyerror(real_ast,"The identifier has been declared.\n");
        } 
        else {
            (yyval.const_declaration_node) = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE);
            LeafNode* leaf_node = new LeafNode((yyvsp[-2].token_info).value.m_OTHERS,false);
            (yyval.const_declaration_node)->append_child(leaf_node);
            (yyval.const_declaration_node)->append_child((yyvsp[0].value_node_info).const_variable_node);
            if(DEBUG) printf("const_declaration -> id = const_variable.\n");
        } 
    }
#line 1539 "src/parser.tab.cc"
    break;

  case 15: /* const_variable: PLUS ID  */
#line 279 "scripts/parser.y"
    {   
        // const_variable -> + id.
        ConstSymbol  *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>((yyvsp[0].token_info).value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            (yyval.value_node_info).type_ptr = symbol->type();
            if((yyval.value_node_info).type_ptr==pascal_type::TYPE_INT){
                (yyval.value_node_info).value.m_INT = symbol->value<int>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_REAL){
                (yyval.value_node_info).value.m_REAL = symbol->value<float>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_BOOL){
                (yyval.value_node_info).value.m_BOOLEAN = symbol->value<bool>();
            } else {
                (yyval.value_node_info).value.m_CHAR = symbol->value<char>();
            }
            //$$.const_value = symbol->value();
            std::string s1("+");
            std::string s2((yyvsp[0].token_info).value.m_OTHERS);
            s1+=s2;
            (yyval.value_node_info).const_variable_node = new LeafNode(s1,false);
            
        }
        if(DEBUG) printf("const_variable -> +id.\n");
    }
#line 1569 "src/parser.tab.cc"
    break;

  case 16: /* const_variable: UMINUS ID  */
#line 305 "scripts/parser.y"
    {
        // const_variable -> - id. todo -
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>((yyvsp[0].token_info).value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            (yyval.value_node_info).type_ptr = symbol->type();
            if((yyval.value_node_info).type_ptr==pascal_type::TYPE_INT){
                (yyval.value_node_info).value.m_INT = -symbol->value<int>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_REAL){
                (yyval.value_node_info).value.m_REAL = -symbol->value<float>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_BOOL){
                (yyval.value_node_info).value.m_BOOLEAN = symbol->value<bool>();// todo
            } else {
                (yyval.value_node_info).value.m_CHAR = -symbol->value<char>();
            }
            //$$.const_value = -(symbol->value());
            std::string s1("-");
            std::string s2((yyvsp[0].token_info).value.m_OTHERS);
            s1+=s2;
            (yyval.value_node_info).const_variable_node = new LeafNode(s1,false);
        }
        
        if(DEBUG) printf("const_variable -> -id.\n");
    }
#line 1599 "src/parser.tab.cc"
    break;

  case 17: /* const_variable: ID  */
#line 331 "scripts/parser.y"
    {
        // const_variable -> id.
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>((yyvsp[0].token_info).value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            (yyval.value_node_info).type_ptr = symbol->type();
            if((yyval.value_node_info).type_ptr==pascal_type::TYPE_INT){
                (yyval.value_node_info).value.m_INT = symbol->value<int>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_REAL){
                (yyval.value_node_info).value.m_REAL = symbol->value<float>();
            } else if((yyval.value_node_info).type_ptr==pascal_type::TYPE_BOOL){
                (yyval.value_node_info).value.m_BOOLEAN = symbol->value<bool>();
            } else {
                (yyval.value_node_info).value.m_CHAR = symbol->value<char>();
            }
        }
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        if(DEBUG) printf("const_variable -> id.\n");
    }
#line 1624 "src/parser.tab.cc"
    break;

  case 18: /* const_variable: UMINUS num  */
#line 352 "scripts/parser.y"
    {   
        //TODO
        // const_variable -> - num.
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        if((yyvsp[0].value_node_info).type_ptr==pascal_type::TYPE_INT){
            (yyval.value_node_info).value.m_INT = (yyvsp[0].value_node_info).value.m_INT*(-1);
            //$$.const_value = new ConstValue($2.value.m_INT*(-1));
            (yyval.value_node_info).const_variable_node = new LeafNode(-((yyvsp[0].value_node_info).value.m_INT));
        } else {
            (yyval.value_node_info).value.m_REAL = (yyvsp[0].value_node_info).value.m_REAL*(-1);
            //$$.const_value = new ConstValue($2.value.m_REAL*(-1));
            (yyval.value_node_info).const_variable_node = new LeafNode(-((yyvsp[0].value_node_info).value.m_REAL));
        }
        if(DEBUG) printf("const_variable -> -num.\n");
        
    }
#line 1645 "src/parser.tab.cc"
    break;

  case 19: /* const_variable: num  */
#line 369 "scripts/parser.y"
    {   
        // const_variable -> num.
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        (yyval.value_node_info).value = (yyvsp[0].value_node_info).value;
        if((yyvsp[0].value_node_info).type_ptr==pascal_type::TYPE_INT){
            (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value.m_INT);
            //$$.const_value = new ConstValue($1.value.m_INT);
        } else {
            (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value.m_REAL);
            //$$.const_value = new ConstValue($1.value.m_REAL);
        }
        if(DEBUG) printf("const_variable -> num.\n");
    }
#line 1663 "src/parser.tab.cc"
    break;

  case 20: /* const_variable: PLUS num  */
#line 383 "scripts/parser.y"
    {  
        // const_variable -> +num.
        (yyval.value_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;
        (yyval.value_node_info).value = (yyvsp[0].value_node_info).value;
        if((yyvsp[0].value_node_info).type_ptr==pascal_type::TYPE_INT){
            //$$.const_value = new ConstValue($2.value.m_REAL);
            (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value.m_INT);
        } else {
            //$$.const_value = new ConstValue($2.value.m_REAL);
            (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].value_node_info).value.m_REAL);
        }
        if(DEBUG) printf("const_variable -> +num.\n");
        // $$.const_variable_node = new ConstVariableNode();
        // LeafNode* leaf_node = new LeafNode($2.value.m_INT);//?
        // $$.const_variable_node->append_child(leaf_node);
    }
#line 1684 "src/parser.tab.cc"
    break;

  case 21: /* const_variable: CHAR  */
#line 400 "scripts/parser.y"
    {
        // const_variable -> 'letter'.
        (yyval.value_node_info).type_ptr = pascal_type::TYPE_CHAR;
        (yyval.value_node_info).value.m_CHAR = (yyvsp[0].token_info).value.m_CHAR;
        //$$.const_value = new ConstValue($2.value.m_CHAR);
        (yyval.value_node_info).const_variable_node = new LeafNode((yyvsp[0].token_info).value.m_CHAR);
        // $$.const_variable_node = new ConstVariableNode();
        // LeafNode* leaf_node = new LeafNode($1.value.m_CHAR);
        // $$.const_variable_node->append_child(leaf_node);
        if(DEBUG) printf("const_variable -> char.\n");

    }
#line 1701 "src/parser.tab.cc"
    break;

  case 22: /* num: INT_NUM  */
#line 415 "scripts/parser.y"
    {
        // num -> int_num.
        (yyval.value_node_info).type_ptr = pascal_type::TYPE_INT;
        (yyval.value_node_info).value.m_INT = (yyvsp[0].token_info).value.m_INT;
        if(DEBUG) printf("num -> int_num.\n");
    }
#line 1712 "src/parser.tab.cc"
    break;

  case 23: /* num: REAL_NUM  */
#line 422 "scripts/parser.y"
    {   
        // num -> real_num.
        (yyval.value_node_info).type_ptr = pascal_type::TYPE_REAL;
        (yyval.value_node_info).value.m_REAL = (yyvsp[0].token_info).value.m_REAL;
        if(DEBUG) printf("num -> real_num.\n");
    }
#line 1723 "src/parser.tab.cc"
    break;

  case 24: /* type_declarations: %empty  */
#line 428 "scripts/parser.y"
                    {
        // type_declarations -> empty.
        (yyval.type_declarations_node) = new TypeDeclarationsNode();
        if(DEBUG) printf("type_declarations -> empty.\n");
    }
#line 1733 "src/parser.tab.cc"
    break;

  case 25: /* type_declarations: TYPE type_declaration ';'  */
#line 434 "scripts/parser.y"
    {
        (yyval.type_declarations_node) = new TypeDeclarationsNode();
        (yyval.type_declarations_node)->append_child((yyvsp[-1].type_declaration_node));
        // type_declarations -> type type_declaration.
        if(DEBUG) printf("type_declarations -> type type_declaration\n");
    }
#line 1744 "src/parser.tab.cc"
    break;

  case 26: /* type_declaration: type_declaration ';' ID '=' type  */
#line 442 "scripts/parser.y"
    {
        // TODO
        // type_declaration -> type_declaration ; id = type.
        if ((yyvsp[0].type_node_info).main_type == TypeAttr::BASIC) {
            pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>((yyvsp[0].type_node_info).type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>((yyvsp[-2].token_info).value.m_OTHERS,basic_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            }
        } else if ((yyvsp[0].type_node_info).main_type == TypeAttr::ARRAY) {
            pascal_type::ArrayType *array_type = new pascal_type::ArrayType((yyvsp[0].type_node_info).type_ptr,*((yyvsp[0].type_node_info).bounds));
            if (!table_set_queue.top()->Insert<ArrayType>((yyvsp[-2].token_info).value.m_OTHERS,array_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                (yyvsp[0].type_node_info).type_ptr = table_set_queue.top()->SearchEntry<ArrayType>((yyvsp[-2].token_info).value.m_OTHERS);
            }
        } else if ((yyvsp[0].type_node_info).record_info) {
            pascal_type::RecordType *record_type = new pascal_type::RecordType(*((yyvsp[0].type_node_info).record_info));
            if (!table_set_queue.top()->Insert<RecordType>((yyvsp[-2].token_info).value.m_OTHERS,record_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                (yyvsp[0].type_node_info).type_ptr = table_set_queue.top()->SearchEntry<RecordType>((yyvsp[-2].token_info).value.m_OTHERS);
                (yyvsp[0].type_node_info).array_type_ptr = (yyvsp[0].type_node_info).type_ptr;
            }
        }

        (yyval.type_declaration_node) = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::MULTIPLE_DECL);
        (yyval.type_declaration_node)->append_child((yyvsp[-4].type_declaration_node));
        LeafNode *leaf_node = new LeafNode((yyvsp[-2].token_info).value.m_OTHERS,false);
        (yyval.type_declaration_node)->append_child(leaf_node);
        (yyval.type_declaration_node)->append_child((yyvsp[0].type_node_info).type_node);
        if(DEBUG) printf("type_declaration -> type_declaration ; id = type.\n");
    }
#line 1781 "src/parser.tab.cc"
    break;

  case 27: /* type_declaration: ID '=' type  */
#line 475 "scripts/parser.y"
    {
        // TODO!
        // type_declaration -> id = type.
        if ((yyvsp[0].type_node_info).main_type == TypeAttr::BASIC) {
            pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>((yyvsp[0].type_node_info).type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>((yyvsp[-2].token_info).value.m_OTHERS,basic_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } 
        } else if ((yyvsp[0].type_node_info).main_type == TypeAttr::ARRAY) {
            pascal_type::ArrayType *array_type = new pascal_type::ArrayType((yyvsp[0].type_node_info).array_type_ptr,*((yyvsp[0].type_node_info).bounds));
            if (!table_set_queue.top()->Insert<ArrayType>((yyvsp[-2].token_info).value.m_OTHERS,array_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                (yyvsp[0].type_node_info).type_ptr = table_set_queue.top()->SearchEntry<ArrayType>((yyvsp[-2].token_info).value.m_OTHERS);
            }
        } else if ((yyvsp[0].type_node_info).record_info) {
            pascal_type::RecordType *record_type = new pascal_type::RecordType(*((yyvsp[0].type_node_info).record_info));
            if (!table_set_queue.top()->Insert<RecordType>((yyvsp[-2].token_info).value.m_OTHERS,record_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                (yyvsp[0].type_node_info).type_ptr = table_set_queue.top()->SearchEntry<RecordType>((yyvsp[-2].token_info).value.m_OTHERS);
                (yyvsp[0].type_node_info).array_type_ptr = (yyvsp[0].type_node_info).type_ptr;
            }
        }

        (yyval.type_declaration_node) = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::SINGLE_DECL);
        LeafNode *leaf_node = new LeafNode((yyvsp[-2].token_info).value.m_OTHERS,false);
        (yyval.type_declaration_node)->append_child(leaf_node);
        (yyval.type_declaration_node)->append_child((yyvsp[0].type_node_info).type_node);
        if(DEBUG) printf("type_declaration -> id = type.\n");
    }
#line 1817 "src/parser.tab.cc"
    break;

  case 28: /* type: standrad_type  */
#line 508 "scripts/parser.y"
    {
        // type -> standrad_type.
        (yyval.type_node_info).main_type = (TypeAttr::MainType)0;
        (yyval.type_node_info).type_ptr = (yyvsp[0].standared_type_node_info).type_ptr;
        (yyval.type_node_info).array_type_ptr = (yyvsp[0].standared_type_node_info).type_ptr;

        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        (yyval.type_node_info).type_node->set_base_type_node((yyval.type_node_info).type_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[0].standared_type_node_info).standard_type_node);
        if(DEBUG) printf("type -> standrad_type.\n");
    }
#line 1833 "src/parser.tab.cc"
    break;

  case 29: /* type: ARRAY '[' periods ']' OF type  */
#line 520 "scripts/parser.y"
    {
        // TODO
        // type -> array [periods] of stype.
        (yyval.type_node_info).main_type = (TypeAttr::MainType)1;
        (yyval.type_node_info).array_type_ptr = (yyvsp[0].type_node_info).array_type_ptr;
        (yyval.type_node_info).bounds = (yyvsp[-3].periods_node_info).bounds;

        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        (yyval.type_node_info).type_node->set_base_type_node((yyvsp[0].type_node_info).type_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[-3].periods_node_info).periods_node);
        (yyval.type_node_info).type_node->append_child((yyvsp[0].type_node_info).type_node);
        if(DEBUG) printf("type -> array [periods] of type.\n");
    }
#line 1851 "src/parser.tab.cc"
    break;

  case 30: /* type: RECORD record_body END  */
#line 534 "scripts/parser.y"
    {
        // TODO
        (yyval.type_node_info).main_type = (TypeAttr::MainType)2;
        (yyval.type_node_info).record_info = (yyvsp[-1].record_node_info).record_info;
        
        (yyval.type_node_info).type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        (yyval.type_node_info).type_node->append_child((yyvsp[-1].record_node_info).record_body_node);
        (yyval.type_node_info).type_node->set_base_type_node((yyval.type_node_info).type_node);
        if(DEBUG) printf("type -> record record_body end.\n");
        // type -> record record_body end.
    }
#line 1867 "src/parser.tab.cc"
    break;

  case 31: /* record_body: %empty  */
#line 546 "scripts/parser.y"
    {
        // record_body -> empty.
        (yyval.record_node_info).record_info = nullptr;
        (yyval.record_node_info).record_body_node = new RecordBodyNode();
        if(DEBUG) printf("record_body -> empty.\n");
    }
#line 1878 "src/parser.tab.cc"
    break;

  case 32: /* record_body: var_declaration  */
#line 553 "scripts/parser.y"
    {
        // record_body -> var_declaration.
        (yyval.record_node_info).record_info = (yyvsp[0].variable_declaration_node_info).record_info;

        (yyval.record_node_info).record_body_node = new RecordBodyNode();
        (yyval.record_node_info).record_body_node->append_child((yyvsp[0].variable_declaration_node_info).variable_declaration_node);
        if(DEBUG) printf("record_body -> var_declaration.\n");
    }
#line 1891 "src/parser.tab.cc"
    break;

  case 33: /* standrad_type: BASIC_TYPE  */
#line 563 "scripts/parser.y"
    {
        // standrad_type -> int|real|bool|char.
        //std::cout<<"$1.value.m_OTHERS:"<<$1.value.m_OTHERS<<std::endl;
        if (!strcmp((yyvsp[0].token_info).value.m_OTHERS,"integer")){
            (yyval.standared_type_node_info).type_ptr = pascal_type::TYPE_INT;
        } else if(!strcmp((yyvsp[0].token_info).value.m_OTHERS,"real")){
            (yyval.standared_type_node_info).type_ptr = pascal_type::TYPE_REAL;
        } else if(!strcmp((yyvsp[0].token_info).value.m_OTHERS,"Boolean")){
            (yyval.standared_type_node_info).type_ptr = pascal_type::TYPE_BOOL;
        } else{
            (yyval.standared_type_node_info).type_ptr = pascal_type::TYPE_CHAR;
        }
        (yyval.standared_type_node_info).standard_type_node = new BasicTypeNode();
        (yyval.standared_type_node_info).standard_type_node->set_type(dynamic_cast<pascal_type::BasicType*>((yyval.standared_type_node_info).type_ptr));
    }
#line 1911 "src/parser.tab.cc"
    break;

  case 34: /* periods: periods ',' period  */
#line 580 "scripts/parser.y"
    {
        // periods -> periods,period.
        (yyval.periods_node_info).bounds = (yyvsp[-2].periods_node_info).bounds;
        (yyval.periods_node_info).bounds->push_back(*((yyvsp[0].period_node_info).bound));

        (yyval.periods_node_info).periods_node = new PeriodsNode();
        (yyval.periods_node_info).periods_node->append_child((yyvsp[-2].periods_node_info).periods_node);
        (yyval.periods_node_info).periods_node->append_child((yyvsp[0].period_node_info).period_node);
        if(DEBUG) printf("periods -> periods,period.");
    }
#line 1926 "src/parser.tab.cc"
    break;

  case 35: /* periods: period  */
#line 591 "scripts/parser.y"
    {
        // periods -> period.
        (yyval.periods_node_info).bounds = new std::vector<std::pair<int, int>>();
        (yyval.periods_node_info).bounds->push_back(*((yyvsp[0].period_node_info).bound));
        (yyval.periods_node_info).periods_node = new PeriodsNode();
        (yyval.periods_node_info).periods_node->append_child((yyvsp[0].period_node_info).period_node);
        if(DEBUG) printf("periods -> period.");
    }
#line 1939 "src/parser.tab.cc"
    break;

  case 36: /* period: const_variable SUBCATALOG const_variable  */
#line 601 "scripts/parser.y"
    {
        // test type ID 
        // period -> const_variable .. const_variable.
        
        int arr_len;
        if (((yyvsp[-2].value_node_info).type_ptr == pascal_type::TYPE_INT)&&((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_INT)){
            arr_len = (yyvsp[0].value_node_info).value.m_INT - (yyvsp[-2].value_node_info).value.m_INT;
            if(arr_len < 0){
                //ERROR
            }
            (yyval.period_node_info).bound = new std::pair<int, int>((yyvsp[-2].value_node_info).value.m_INT, (yyvsp[0].value_node_info).value.m_INT);
        } else if(((yyvsp[-2].value_node_info).type_ptr == pascal_type::TYPE_CHAR)&&((yyvsp[0].value_node_info).type_ptr == pascal_type::TYPE_CHAR)){
            arr_len =(yyvsp[0].value_node_info).value.m_CHAR - (yyvsp[-2].value_node_info).value.m_CHAR;
            if(arr_len < 0){
                //ERROR
            }
            (yyval.period_node_info).bound = new std::pair<int, int>(int((yyvsp[-2].value_node_info).value.m_CHAR), int((yyvsp[0].value_node_info).value.m_CHAR));
        } else{
            //ERROR
        }
        (yyval.period_node_info).period_node =new PeriodNode();
        (yyval.period_node_info).period_node->set_len(arr_len);
        (yyval.period_node_info).period_node->append_child((yyvsp[-2].value_node_info).const_variable_node);
        (yyval.period_node_info).period_node->append_child((yyvsp[0].value_node_info).const_variable_node);
        if(DEBUG) printf("period -> const_variable .. const_variable.");
    }
#line 1970 "src/parser.tab.cc"
    break;

  case 37: /* var_declarations: %empty  */
#line 629 "scripts/parser.y"
    {
        // var_declarations -> empty.
        (yyval.variable_declarations_node) = new VariableDeclarationsNode();
        if(DEBUG) printf("var_declarations -> empty.\n");
    }
#line 1980 "src/parser.tab.cc"
    break;

  case 38: /* var_declarations: VAR var_declaration ';'  */
#line 635 "scripts/parser.y"
    {
        // var_declarations -> var var_declaration.
        (yyval.variable_declarations_node) = new VariableDeclarationsNode();
        (yyval.variable_declarations_node)->append_child((yyvsp[-1].variable_declaration_node_info).variable_declaration_node);
        if(DEBUG) printf("var_declarations -> var var_declaration.\n");

        for (auto i : *((yyvsp[-1].variable_declaration_node_info).record_info)){
            ObjectSymbol *obj = new ObjectSymbol(i.first, i.second,10);//TODO
            if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first,obj)){
                yyerror(real_ast,"redefinition of variable");
            }
        }
    }
#line 1998 "src/parser.tab.cc"
    break;

  case 39: /* var_declaration: var_declaration ';' id_list ':' type  */
#line 650 "scripts/parser.y"
    {
        // var_declaration -> var_declaration ; id_list : type.
        (yyval.variable_declaration_node_info).record_info = (yyvsp[-4].variable_declaration_node_info).record_info;
        for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
            auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, (yyvsp[0].type_node_info).type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");}
        }
        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-4].variable_declaration_node_info).variable_declaration_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[0].type_node_info).type_node);
        if(DEBUG) printf("var_declaration -> var_declaration ; id_list : type.\n");
    }
#line 2017 "src/parser.tab.cc"
    break;

  case 40: /* var_declaration: id_list ':' type  */
#line 665 "scripts/parser.y"
    {
        (yyval.variable_declaration_node_info).record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
        for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
            auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, (yyvsp[0].type_node_info).type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");}
        }
        // var_declaration -> id : type.
        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[0].type_node_info).type_node);
        if(DEBUG) printf("var_declaration -> id : type.\n");
    }
#line 2035 "src/parser.tab.cc"
    break;

  case 41: /* var_declaration: var_declaration ';' id_list ':' ID  */
#line 679 "scripts/parser.y"
    {
        (yyval.variable_declaration_node_info).record_info = (yyvsp[-4].variable_declaration_node_info).record_info;
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>((yyvsp[0].token_info).value.m_OTHERS);
        if(tmp == nullptr){
            yyerror(real_ast,"undefined type");
        } else {
            for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
                auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, tmp));
                if (!res.second){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }  
        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::ID);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-4].variable_declaration_node_info).variable_declaration_node);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        LeafNode *leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child(leaf_node);
        if(DEBUG) printf("var_declaration -> var_declaration ; id_list : ID.\n");
    }
#line 2060 "src/parser.tab.cc"
    break;

  case 42: /* var_declaration: id_list ':' ID  */
#line 700 "scripts/parser.y"
    {
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>((yyvsp[0].token_info).value.m_OTHERS);
        if(tmp==nullptr){
            yyerror(real_ast,"undefined type");
        } else {
            (yyval.variable_declaration_node_info).record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
            for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
                auto res = (yyval.variable_declaration_node_info).record_info->insert(make_pair(i.first, tmp));
                if (!res.second){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }

        (yyval.variable_declaration_node_info).variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::ID);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        LeafNode *leaf_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.variable_declaration_node_info).variable_declaration_node->append_child(leaf_node);
        if(DEBUG) printf("var_declaration -> id_list : ID.\n");
    }
#line 2085 "src/parser.tab.cc"
    break;

  case 43: /* $@3: %empty  */
#line 720 "scripts/parser.y"
      {column = buf.size(); len = strlen(yytext);}
#line 2091 "src/parser.tab.cc"
    break;

  case 44: /* var_declaration: $@3 error ';'  */
#line 721 "scripts/parser.y"
    {
        yyerror_("An identifier is expected.");
        // if ( DEBUG ) printf("An identifier is expected.\n");
    }
#line 2100 "src/parser.tab.cc"
    break;

  case 45: /* subprogram_declarations: %empty  */
#line 725 "scripts/parser.y"
                          {
        // subprogram_declarations -> empty.
        (yyval.subprogram_declarations_node) = new SubprogramDeclarationsNode();
        if (DEBUG) printf("subprogram_declarations -> empty.\n");
    }
#line 2110 "src/parser.tab.cc"
    break;

  case 46: /* subprogram_declarations: subprogram_declarations subprogram_declaration ';'  */
#line 731 "scripts/parser.y"
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration.
        (yyval.subprogram_declarations_node) = new SubprogramDeclarationsNode();
        (yyval.subprogram_declarations_node)->append_child((yyvsp[-2].subprogram_declarations_node));
        (yyval.subprogram_declarations_node)->append_child((yyvsp[-1].subprogram_declaration_node));
        table_set_queue.pop();
        if  (DEBUG) printf("subprogram_declarations -> subprogram_declarations subprogram_declaration.\n");
        
    }
#line 2124 "src/parser.tab.cc"
    break;

  case 47: /* subprogram_declaration: subprogram_head subprogram_body  */
#line 742 "scripts/parser.y"
    {
        // subprogram_declaration -> subprogram_head program_body.
        (yyval.subprogram_declaration_node) = new SubprogramDeclarationNode();
        (yyval.subprogram_declaration_node)->append_child((yyvsp[-1].subprogram_head_node));
        (yyval.subprogram_declaration_node)->append_child((yyvsp[0].subprogram_body_node));
        if (DEBUG) printf("subprogram_declaration -> subprogram_head program_body.\n");
    }
#line 2136 "src/parser.tab.cc"
    break;

  case 48: /* subprogram_body: const_declarations type_declarations var_declarations compound_statement  */
#line 751 "scripts/parser.y"
    {
        (yyval.subprogram_body_node) = new SubprogramBodyNode();
        (yyval.subprogram_body_node)->append_child((yyvsp[-3].const_declarations_node));
        (yyval.subprogram_body_node)->append_child((yyvsp[-2].type_declarations_node));
        (yyval.subprogram_body_node)->append_child((yyvsp[-1].variable_declarations_node));
        (yyval.subprogram_body_node)->append_child((yyvsp[0].compound_statement_node));
        if (DEBUG) printf("subprogram_body -> const_declarations type_declarations var_declarations compound_statement.\n");
    }
#line 2149 "src/parser.tab.cc"
    break;

  case 49: /* subprogram_head: FUNCTION ID formal_parameter ':' standrad_type ';'  */
#line 761 "scripts/parser.y"
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,$5,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> function id formal_parametert : standrad_type.
        FunctionSymbol* tmp ;
        if((yyvsp[-3].formal_parameter_node_info).parameters){
            tmp = new FunctionSymbol((yyvsp[-4].token_info).value.m_OTHERS, nullptr, (yyvsp[-4].token_info).line_num, *(yyvsp[-3].formal_parameter_node_info).parameters);
        } else {
            tmp = new FunctionSymbol((yyvsp[-4].token_info).value.m_OTHERS, nullptr, (yyvsp[-4].token_info).line_num);
        }
        if (!table_set_queue.top()->Insert<FunctionSymbol>((yyvsp[-4].token_info).value.m_OTHERS, tmp)){
            yyerror(real_ast,"redefinition of function");
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet((yyvsp[-4].token_info).value.m_OTHERS, table_set_queue.top());
        table_set_queue.push(now_table_set);
        table_set_queue.top()->Insert<FunctionSymbol>((yyvsp[-4].token_info).value.m_OTHERS, tmp);
        if ((yyvsp[-3].formal_parameter_node_info).parameters){
            for (auto i : *((yyvsp[-3].formal_parameter_node_info).parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, (yyvsp[-4].token_info).line_num);
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }

        (yyval.subprogram_head_node) = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::FUNCTION);
        LeafNode *leaf_node = new LeafNode((yyvsp[-4].token_info).value.m_OTHERS,false);
        (yyval.subprogram_head_node)->append_child(leaf_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-3].formal_parameter_node_info).formal_parameter_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-1].standared_type_node_info).standard_type_node);
        if(DEBUG) printf("subprogram_head -> function id formal_parameter : standrad_type.\n");
    }
#line 2188 "src/parser.tab.cc"
    break;

  case 50: /* subprogram_head: PROCEDURE ID formal_parameter ';'  */
#line 796 "scripts/parser.y"
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,NULL,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> procedure id formal_parametert.
        FunctionSymbol* tmp ;
        if((yyvsp[-1].formal_parameter_node_info).parameters){
            tmp = new FunctionSymbol((yyvsp[-2].token_info).value.m_OTHERS, nullptr, (yyvsp[-2].token_info).line_num, *(yyvsp[-1].formal_parameter_node_info).parameters);
        } else {
            tmp = new FunctionSymbol((yyvsp[-2].token_info).value.m_OTHERS, nullptr, (yyvsp[-2].token_info).line_num);
        }
        
        if (!table_set_queue.top()->Insert<FunctionSymbol>((yyvsp[-2].token_info).value.m_OTHERS, tmp)){
            yyerror(real_ast,"redefinition of function");
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet((yyvsp[-2].token_info).value.m_OTHERS,table_set_queue.top());
        table_set_queue.push(now_table_set);
        table_set_queue.top()->Insert<FunctionSymbol>((yyvsp[-2].token_info).value.m_OTHERS, tmp);
        if ((yyvsp[-1].formal_parameter_node_info).parameters){
            for (auto i : *((yyvsp[-1].formal_parameter_node_info).parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, (yyvsp[-2].token_info).line_num);
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }
        

        (yyval.subprogram_head_node) = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode((yyvsp[-2].token_info).value.m_OTHERS,false);
        (yyval.subprogram_head_node)->append_child(leaf_node);
        (yyval.subprogram_head_node)->append_child((yyvsp[-1].formal_parameter_node_info).formal_parameter_node);
        if(DEBUG) printf("subprogram_head -> procedure id formal_parameter.\n");
    }
#line 2228 "src/parser.tab.cc"
    break;

  case 51: /* formal_parameter: %empty  */
#line 832 "scripts/parser.y"
    {   
        // formal_parameter -> empty.
        (yyval.formal_parameter_node_info).parameters = nullptr;
        (yyval.formal_parameter_node_info).formal_parameter_node = new FormalParamNode();

    }
#line 2239 "src/parser.tab.cc"
    break;

  case 52: /* formal_parameter: '(' parameter_lists ')'  */
#line 839 "scripts/parser.y"
    {
        // formal_parameter -> (parameter_lists).
        (yyval.formal_parameter_node_info).parameters = (yyvsp[-1].parameter_lists_node_info).parameters;
        (yyval.formal_parameter_node_info).formal_parameter_node = new FormalParamNode();
        (yyval.formal_parameter_node_info).formal_parameter_node->append_child((yyvsp[-1].parameter_lists_node_info).param_lists_node);
    }
#line 2250 "src/parser.tab.cc"
    break;

  case 53: /* parameter_lists: parameter_lists ';' parameter_list  */
#line 847 "scripts/parser.y"
    {   
        // parameter_lists -> parameter_lists ; parameter_list.
        (yyval.parameter_lists_node_info).parameters = (yyvsp[-2].parameter_lists_node_info).parameters;
        (yyval.parameter_lists_node_info).parameters->insert((yyval.parameter_lists_node_info).parameters->end(), (yyvsp[0].parameter_list_node_info).parameters->begin(), (yyvsp[0].parameter_list_node_info).parameters->end());

        (yyval.parameter_lists_node_info).param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[-2].parameter_lists_node_info).param_lists_node);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[0].parameter_list_node_info).param_list_node);
    }
#line 2264 "src/parser.tab.cc"
    break;

  case 54: /* parameter_lists: parameter_list  */
#line 857 "scripts/parser.y"
    {  
        // parameter_lists -> parameter_list.
        (yyval.parameter_lists_node_info).parameters = (yyvsp[0].parameter_list_node_info).parameters;

        (yyval.parameter_lists_node_info).param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        (yyval.parameter_lists_node_info).param_lists_node->append_child((yyvsp[0].parameter_list_node_info).param_list_node);
        if (DEBUG) printf("parameter_lists -> parameter_list.\n");
    }
#line 2277 "src/parser.tab.cc"
    break;

  case 55: /* parameter_list: var_parameter  */
#line 867 "scripts/parser.y"
    {   
        // parameter_list -> var_parameter.
        (yyval.parameter_list_node_info).parameters = (yyvsp[0].var_parameter_node_info).parameters;

        (yyval.parameter_list_node_info).param_list_node = new ParamListNode();
        (yyval.parameter_list_node_info).param_list_node->append_child((yyvsp[0].var_parameter_node_info).var_parameter_node);
        if (DEBUG) printf("parameter_list -> var_parameter.\n");
    }
#line 2290 "src/parser.tab.cc"
    break;

  case 56: /* parameter_list: value_parameter  */
#line 876 "scripts/parser.y"
    {   
        // parameter_list -> value_parameter.
        (yyval.parameter_list_node_info).parameters = (yyvsp[0].value_parameter_node_info).parameters;

        (yyval.parameter_list_node_info).param_list_node = new ParamListNode();
        (yyval.parameter_list_node_info).param_list_node->append_child((yyvsp[0].value_parameter_node_info).value_parameter_node);
        if (DEBUG) printf("parameter_list -> value_parameter.\n");
    }
#line 2303 "src/parser.tab.cc"
    break;

  case 57: /* var_parameter: VAR value_parameter  */
#line 886 "scripts/parser.y"
    {   
        // var_parameter -> var value_parameter.
        int para_len = (yyvsp[0].value_parameter_node_info).parameters->size();
        for (int i = 0; i < para_len; i++){
            (yyvsp[0].value_parameter_node_info).parameters->at(i).second.second = FunctionSymbol::PARAM_MODE::REFERENCE;
        }

        (yyval.var_parameter_node_info).var_parameter_node = new VarParamNode();
        (yyval.var_parameter_node_info).var_parameter_node->append_child((yyvsp[0].value_parameter_node_info).value_parameter_node);
        if (DEBUG) printf("var_parameter -> var value_parameter.\n");
    }
#line 2319 "src/parser.tab.cc"
    break;

  case 58: /* value_parameter: id_list ':' standrad_type  */
#line 899 "scripts/parser.y"
    {   
        // value_parameter -> id_list : standrad_type.
        (yyval.value_parameter_node_info).parameters = new std::vector<std::pair<std::string, std::pair<BasicType*,FunctionSymbol::PARAM_MODE>>>();
        std::pair<BasicType*,FunctionSymbol::PARAM_MODE> tmp((yyvsp[0].standared_type_node_info).type_ptr,FunctionSymbol::PARAM_MODE::VALUE);
        for (auto i : *((yyvsp[-2].id_list_node_info).list_ref)){
            std::pair<std::string, std::pair<BasicType*,FunctionSymbol::PARAM_MODE>> tmp_pair(i.first,tmp);
            (yyval.value_parameter_node_info).parameters->push_back(tmp_pair);
        }
        

        (yyval.value_parameter_node_info).value_parameter_node = new ValueParamNode();
        (yyval.value_parameter_node_info).value_parameter_node->append_child((yyvsp[-2].id_list_node_info).id_list_node);
        (yyval.value_parameter_node_info).value_parameter_node->append_child((yyvsp[0].standared_type_node_info).standard_type_node);
        if (DEBUG) printf("value_parameter -> id_list : standrad_type.\n");
    }
#line 2339 "src/parser.tab.cc"
    break;

  case 59: /* compound_statement: BEGIN_ statement_list END  */
#line 915 "scripts/parser.y"
                              {
        // compound_statement -> begin statement_list end.
        (yyval.compound_statement_node) = new CompoundStatementNode();
        (yyval.compound_statement_node)->append_child((yyvsp[-1].statement_list_node));
        if (DEBUG) printf("compound_statement -> begin statement_list end.\n");
    }
#line 2350 "src/parser.tab.cc"
    break;

  case 60: /* statement_list: statement_list ';' statement  */
#line 923 "scripts/parser.y"
    {
        // statement_list -> statement_list ; statement.
        (yyval.statement_list_node) = new StatementListNode();
        (yyval.statement_list_node)->append_child((yyvsp[-2].statement_list_node));
        (yyval.statement_list_node)->append_child((yyvsp[0].statement_node));
        if (DEBUG) printf("statement_list -> statement_list ; statement.\n");
    }
#line 2362 "src/parser.tab.cc"
    break;

  case 61: /* statement_list: statement  */
#line 930 "scripts/parser.y"
    {
        // statement_list -> statement.
        (yyval.statement_list_node) = new StatementListNode();
        (yyval.statement_list_node)->append_child((yyvsp[0].statement_node));
    }
#line 2372 "src/parser.tab.cc"
    break;

  case 62: /* statement: variable ASSIGNOP expression  */
#line 937 "scripts/parser.y"
    {   // 类型检查 指针为type_ptr
        // statement -> variable assigbop expression.
        std::string func_name = table_set_queue.top()->tag();
        if(func_name == *(yyvsp[-2].variable_node_info).name){
            (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
        }
        (yyval.statement_node)->append_child((yyvsp[-2].variable_node_info).variable_node);
        (yyval.statement_node)->append_child((yyvsp[0].expression_node_info).expression_node);
        if (DEBUG) printf("statement -> variable assigbop expression.\n");
    }
#line 2389 "src/parser.tab.cc"
    break;

  case 63: /* statement: call_procedure_statement  */
#line 950 "scripts/parser.y"
    {
        // statement -> call_procedure_statement.
        // TODO check
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        (yyval.statement_node)->append_child((yyvsp[0].procedure_call_node));
        if (DEBUG) printf("statement -> call_procedure_statement.\n");
    }
#line 2401 "src/parser.tab.cc"
    break;

  case 64: /* statement: compound_statement  */
#line 958 "scripts/parser.y"
    {
        // statement -> compound_statement.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[0].compound_statement_node));
        if (DEBUG) printf("statement -> compound_statement.\n");
    }
#line 2412 "src/parser.tab.cc"
    break;

  case 65: /* statement: IF expression THEN statement else_part  */
#line 965 "scripts/parser.y"
    {   
        //类型检查
        // statement -> if expression then statement else_part.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-3].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[-1].statement_node));
        (yyval.statement_node)->append_child((yyvsp[0].else_node));
        if (DEBUG) printf("statement -> if expression then statement else_part.\n");
    }
#line 2426 "src/parser.tab.cc"
    break;

  case 66: /* statement: CASE expression OF case_body END  */
#line 975 "scripts/parser.y"
    {
        //类型检查
        // statement -> case expression of case_body end.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::CASE_STATEMET);
        (yyval.statement_node)->append_child((yyvsp[-3].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[-1].case_body_node_info).case_body_node);
    }
#line 2438 "src/parser.tab.cc"
    break;

  case 67: /* statement: WHILE expression DO statement  */
#line 983 "scripts/parser.y"
    {
        //类型检查
        // statement -> while expression do if_statement_1.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-2].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[0].statement_node));
        if (DEBUG) printf("statement -> while expression do statement.\n");

    }
#line 2452 "src/parser.tab.cc"
    break;

  case 68: /* statement: REPEAT statement_list UNTIL expression  */
#line 993 "scripts/parser.y"
    {
        //类型检查
        // statement -> repeat statement_list until expression.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::REPEAT_STATEMENT);
        (yyval.statement_node)->append_child((yyvsp[-2].statement_list_node));
        (yyval.statement_node)->append_child((yyvsp[0].expression_node_info).expression_node);
        if (DEBUG) printf("statement -> repeat statement_list until expression.\n");
    }
#line 2465 "src/parser.tab.cc"
    break;

  case 69: /* statement: FOR ID ASSIGNOP expression updown expression DO statement  */
#line 1002 "scripts/parser.y"
    {
        //类型检查
        // statement -> for id assignop expression updown expression do statement.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::FOR_STATEMENT);
        LeafNode *id_node = new LeafNode((yyvsp[-6].token_info).value.m_OTHERS,false);
        (yyval.statement_node)->append_child(id_node);
        (yyval.statement_node)->append_child((yyvsp[-4].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[-3].updown_node));
        (yyval.statement_node)->append_child((yyvsp[-2].expression_node_info).expression_node);
        (yyval.statement_node)->append_child((yyvsp[0].statement_node));
        if (DEBUG) printf("statement -> for id assignop expression updown expression do statement.\n");
    }
#line 2482 "src/parser.tab.cc"
    break;

  case 70: /* statement: %empty  */
#line 1013 "scripts/parser.y"
        {
        // statement -> empty.
        (yyval.statement_node) = new StatementNode(StatementNode::GrammarType::EPSILON);
        if (DEBUG) printf("statement -> empty.\n");
    }
#line 2492 "src/parser.tab.cc"
    break;

  case 71: /* variable: ID id_varparts  */
#line 1021 "scripts/parser.y"
    {
        // variable -> id id_varparts.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>((yyvsp[-1].token_info).value.m_OTHERS);
        if(tmp == nullptr) {
             (yyval.variable_node_info).type_ptr = nullptr;
            yyerror(real_ast,"variable not defined. this way is not allowed.");
        } else {
            //类型检查
            (yyval.variable_node_info).type_ptr = tmp->type();
            (yyval.variable_node_info).name = new std::string((yyvsp[-1].token_info).value.m_OTHERS);
        }
        
        (yyval.variable_node_info).variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode((yyvsp[-1].token_info).value.m_OTHERS,false);
        (yyval.variable_node_info).variable_node->append_child(id_node);
        (yyval.variable_node_info).variable_node->append_child((yyvsp[0].id_varparts_node_info).id_varparts_node);
        if (DEBUG) printf("variable -> id id_varparts.\n");
    }
#line 2515 "src/parser.tab.cc"
    break;

  case 72: /* id_varparts: %empty  */
#line 1041 "scripts/parser.y"
    {
        // id_varparts -> empty.
        (yyval.id_varparts_node_info).var_parts = nullptr;
        (yyval.id_varparts_node_info).id_varparts_node = new IDVarPartsNode();
        if (DEBUG) printf("id_varparts -> empty.\n");
    }
#line 2526 "src/parser.tab.cc"
    break;

  case 73: /* id_varparts: id_varparts id_varpart  */
#line 1048 "scripts/parser.y"
    {
        // id_varparts -> id_varparts id_varpart.
        if((yyval.id_varparts_node_info).var_parts){
            (yyval.id_varparts_node_info).var_parts = (yyvsp[-1].id_varparts_node_info).var_parts;
        } else {
            (yyval.id_varparts_node_info).var_parts = new std::vector<VarParts>();
        }
        
        (yyval.id_varparts_node_info).var_parts->push_back(*((yyvsp[0].id_varpart_node_info).var_part));

        (yyval.id_varparts_node_info).id_varparts_node = new IDVarPartsNode();
        (yyval.id_varparts_node_info).id_varparts_node->append_child((yyvsp[-1].id_varparts_node_info).id_varparts_node);
        (yyval.id_varparts_node_info).id_varparts_node->append_child((yyvsp[0].id_varpart_node_info).id_varpart_node);
        if (DEBUG) printf("id_varparts -> id_varparts id_varpart.\n");
    }
#line 2546 "src/parser.tab.cc"
    break;

  case 74: /* id_varpart: '[' expression_list ']'  */
#line 1066 "scripts/parser.y"
    {   
        // id_varpart -> [expression_list].
        (yyval.id_varpart_node_info).var_part= new VarParts();
        (yyval.id_varpart_node_info).var_part->flag = 0;//数组
        (yyval.id_varpart_node_info).var_part->subscript = (yyvsp[-1].expression_list_node_info).type_ptr_list;

        (yyval.id_varpart_node_info).id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::EXP_LIST);
        (yyval.id_varpart_node_info).id_varpart_node->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        if (DEBUG) printf("id_varpart -> [expression_list].\n");
    }
#line 2561 "src/parser.tab.cc"
    break;

  case 75: /* id_varpart: '.' ID  */
#line 1077 "scripts/parser.y"
    {
        // id_varpart -> .id.
        (yyval.id_varpart_node_info).var_part= new VarParts();
        (yyval.id_varpart_node_info).var_part->flag = 1;//结构体
        strcpy((yyval.id_varpart_node_info).var_part->name,(yyvsp[0].token_info).value.m_OTHERS);

        (yyval.id_varpart_node_info).id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::_ID);
        LeafNode *id_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.id_varpart_node_info).id_varpart_node->append_child(id_node);
        if (DEBUG) printf("id_varpart -> .id.\n");
    }
#line 2577 "src/parser.tab.cc"
    break;

  case 76: /* else_part: %empty  */
#line 1089 "scripts/parser.y"
    {
        // else_part -> empty.
        (yyval.else_node) = new ElseNode(ElseNode::GrammarType::EPSILON);
        if (DEBUG) printf("else_part -> empty.\n");
    }
#line 2587 "src/parser.tab.cc"
    break;

  case 77: /* else_part: ELSE statement  */
#line 1095 "scripts/parser.y"
    {
        // else_part -> else statement.
        (yyval.else_node) = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        (yyval.else_node)->append_child((yyvsp[0].statement_node));
        if (DEBUG) printf("else_part -> else statement.\n");
    }
#line 2598 "src/parser.tab.cc"
    break;

  case 78: /* case_body: %empty  */
#line 1102 "scripts/parser.y"
    {
        // case_body -> empty.
        (yyval.case_body_node_info).type_ptr_list = nullptr;
        (yyval.case_body_node_info).case_body_node = new CaseBodyNode();
        if (DEBUG) printf("case_body -> empty.\n");
    }
#line 2609 "src/parser.tab.cc"
    break;

  case 79: /* case_body: branch_list  */
#line 1109 "scripts/parser.y"
    {
        // case_body -> branch_list.
        (yyval.case_body_node_info).type_ptr_list = (yyvsp[0].branch_list_node_info).type_ptr_list;
        (yyval.case_body_node_info).case_body_node = new CaseBodyNode();
        (yyval.case_body_node_info).case_body_node->append_child((yyvsp[0].branch_list_node_info).branch_list_node);
        if (DEBUG) printf("case_body -> branch_list.\n");
    }
#line 2621 "src/parser.tab.cc"
    break;

  case 80: /* branch_list: branch_list ';' branch  */
#line 1118 "scripts/parser.y"
    {
        // branch_list -> branch_list branch.
        (yyval.branch_list_node_info).type_ptr_list = (yyvsp[-2].branch_list_node_info).type_ptr_list;
        (yyval.branch_list_node_info).type_ptr_list->push_back((yyvsp[0].branch_node_info).type_ptr);
        //todo 检查类型是否一致

        (yyval.branch_list_node_info).branch_list_node = new BranchListNode();
        (yyval.branch_list_node_info).branch_list_node->append_child((yyvsp[-2].branch_list_node_info).branch_list_node);
        (yyval.branch_list_node_info).branch_list_node->append_child((yyvsp[0].branch_node_info).branch_node);
        if (DEBUG) printf("branch_list -> branch_list branch.\n");
    }
#line 2637 "src/parser.tab.cc"
    break;

  case 81: /* branch_list: branch  */
#line 1130 "scripts/parser.y"
    {
        // branch_list -> branch.
        (yyval.branch_list_node_info).type_ptr_list = new std::vector<pascal_type::TypeTemplate*>();
        (yyval.branch_list_node_info).type_ptr_list->push_back((yyvsp[0].branch_node_info).type_ptr);
        
        (yyval.branch_list_node_info).branch_list_node = new BranchListNode();
        (yyval.branch_list_node_info).branch_list_node->append_child((yyvsp[0].branch_node_info).branch_node);
        if (DEBUG) printf("branch_list -> branch.\n");
    }
#line 2651 "src/parser.tab.cc"
    break;

  case 82: /* branch: const_list ':' statement  */
#line 1141 "scripts/parser.y"
    {
        // branch -> const_list : statement.
        (yyval.branch_node_info).type_ptr = (yyvsp[-2].const_list_node_info).type_ptr;

        (yyval.branch_node_info).branch_node = new BranchNode();
        (yyval.branch_node_info).branch_node->append_child((yyvsp[-2].const_list_node_info).const_list_node);
        (yyval.branch_node_info).branch_node->append_child((yyvsp[0].statement_node));
        if (DEBUG) printf("branch -> const_list : statement.\n");
    }
#line 2665 "src/parser.tab.cc"
    break;

  case 83: /* const_list: const_list ',' const_variable  */
#line 1152 "scripts/parser.y"
    {
        // const_list -> const_list , const_variable.
        if((yyvsp[-2].const_list_node_info).type_ptr != (yyvsp[0].value_node_info).type_ptr) {
           yyerror(real_ast,"const_list type not match");
        }
        (yyval.const_list_node_info).type_ptr = (yyvsp[-2].const_list_node_info).type_ptr;

        (yyval.const_list_node_info).const_list_node = new ConstListNode();
        (yyval.const_list_node_info).const_list_node->append_child((yyvsp[-2].const_list_node_info).const_list_node);
        (yyval.const_list_node_info).const_list_node->append_child((yyvsp[0].value_node_info).const_variable_node);
        if (DEBUG) printf("const_list -> const_list , const_variable.\n");
    }
#line 2682 "src/parser.tab.cc"
    break;

  case 84: /* const_list: const_variable  */
#line 1165 "scripts/parser.y"
    {
        // const_list -> const_variable.
        (yyval.const_list_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;

        (yyval.const_list_node_info).const_list_node = new ConstListNode();
        (yyval.const_list_node_info).const_list_node->append_child((yyvsp[0].value_node_info).const_variable_node);
        if (DEBUG) printf("const_list -> const_variable.\n");
    }
#line 2695 "src/parser.tab.cc"
    break;

  case 85: /* updown: TO  */
#line 1175 "scripts/parser.y"
    {
        // updown -> to.
        (yyval.updown_node) = new UpdownNode(true);
        if (DEBUG) printf("updown -> to.\n");
    }
#line 2705 "src/parser.tab.cc"
    break;

  case 86: /* updown: DOWNTO  */
#line 1181 "scripts/parser.y"
    {
        // updown -> downto.
        (yyval.updown_node) = new UpdownNode(false);
        if (DEBUG) printf("updown -> downto.\n");
    }
#line 2715 "src/parser.tab.cc"
    break;

  case 87: /* call_procedure_statement: ID '(' expression_list ')'  */
#line 1188 "scripts/parser.y"
    {
        //类型检查
        // call_procedure_statement -> id (expression_list).
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>((yyvsp[-3].token_info).value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }

        (yyval.procedure_call_node) = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode((yyvsp[-3].token_info).value.m_OTHERS,false);
        (yyval.procedure_call_node)->append_child(id_node);
        (yyval.procedure_call_node)->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        if (DEBUG) printf("call_procedure_statement -> id (expression_list).\n");
    }
#line 2734 "src/parser.tab.cc"
    break;

  case 88: /* call_procedure_statement: ID  */
#line 1203 "scripts/parser.y"
    {   //类型检查
        // call_procedure_statement -> id.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>((yyvsp[0].token_info).value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }

        (yyval.procedure_call_node) = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode((yyvsp[0].token_info).value.m_OTHERS,false);
        (yyval.procedure_call_node)->append_child(id_node);
        if (DEBUG) printf("call_procedure_statement -> id.\n");
    }
#line 2751 "src/parser.tab.cc"
    break;

  case 89: /* expression_list: expression_list ',' expression  */
#line 1217 "scripts/parser.y"
    {
        //类型检查 检查是否为INT or CHAR???
        (yyval.expression_list_node_info).type_ptr_list = (yyvsp[-2].expression_list_node_info).type_ptr_list;
        (yyval.expression_list_node_info).type_ptr_list->push_back((yyvsp[0].expression_node_info).type_ptr);
        // expression_list -> expression_list , expression.

        (yyval.expression_list_node_info).expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[-2].expression_list_node_info).expression_list_node);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[0].expression_node_info).expression_node);
        if (DEBUG) printf("expression_list -> expression_list , expression.\n");
    }
#line 2767 "src/parser.tab.cc"
    break;

  case 90: /* expression_list: expression  */
#line 1229 "scripts/parser.y"
    {
        //类型检查 检查是否为INT or CHAR  ???  
        (yyval.expression_list_node_info).type_ptr_list = new std::vector<pascal_type::TypeTemplate*>();
        (yyval.expression_list_node_info).type_ptr_list->push_back((yyvsp[0].expression_node_info).type_ptr);
        // expression_list -> expression.

        (yyval.expression_list_node_info).expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        (yyval.expression_list_node_info).expression_list_node->append_child((yyvsp[0].expression_node_info).expression_node);
        if (DEBUG) printf("expression_list -> expression.\n");
    }
#line 2782 "src/parser.tab.cc"
    break;

  case 91: /* expression: simple_expression RELOP simple_expression  */
#line 1241 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // 先暂定 tmp = $1.type_ptr
        // expression -> simple_expression relop simple_expression.
        TypeTemplate* tmp = (yyvsp[-2].simple_expression_node_info).type_ptr;
        (yyval.expression_node_info).type_ptr = tmp;
        
        std::string relop = (yyvsp[-1].token_info).value.m_OTHERS;
        if(strcmp((yyvsp[-1].token_info).value.m_OTHERS,"<>") == 0) {
            relop = "!=";
        }

        (yyval.expression_node_info).expression_node = new ExpressionNode();
        (yyval.expression_node_info).expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *relop_node = new LeafNode(relop,true);
        (yyval.expression_node_info).expression_node->append_child(relop_node);
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression relop simple_expression.\n");
    }
#line 2807 "src/parser.tab.cc"
    break;

  case 92: /* expression: simple_expression '=' simple_expression  */
#line 1262 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // 先暂定 tmp = $1.type_ptr
        TypeTemplate* tmp = (yyvsp[-2].simple_expression_node_info).type_ptr;
        (yyval.expression_node_info).type_ptr = tmp;

        (yyval.expression_node_info).expression_node = new ExpressionNode();
        (yyval.expression_node_info).expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *relop_node = new LeafNode("==",true);
        (yyval.expression_node_info).expression_node->append_child(relop_node);
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression relop simple_expression.\n");
    }
#line 2826 "src/parser.tab.cc"
    break;

  case 93: /* expression: simple_expression  */
#line 1277 "scripts/parser.y"
    {
        // expression -> simple_expression.
        (yyval.expression_node_info).type_ptr = (yyvsp[0].simple_expression_node_info).type_ptr;

        (yyval.expression_node_info).expression_node = new ExpressionNode();
        (yyval.expression_node_info).expression_node->append_child((yyvsp[0].simple_expression_node_info).simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression.\n");
    }
#line 2839 "src/parser.tab.cc"
    break;

  case 94: /* simple_expression: term  */
#line 1287 "scripts/parser.y"
    {   
        // simple_expression -> term.
        (yyval.simple_expression_node_info).type_ptr = (yyvsp[0].term_node_info).type_ptr;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> term.\n");
    }
#line 2852 "src/parser.tab.cc"
    break;

  case 95: /* simple_expression: PLUS term  */
#line 1296 "scripts/parser.y"
    {
        // simple_expression -> + term.
        (yyval.simple_expression_node_info).type_ptr = (yyvsp[0].term_node_info).type_ptr;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        LeafNode *plus_node = new LeafNode("+",true);
        (yyval.simple_expression_node_info).simple_expression_node->append_child(plus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> + term.\n");
    }
#line 2867 "src/parser.tab.cc"
    break;

  case 96: /* simple_expression: UMINUS term  */
#line 1307 "scripts/parser.y"
    {
        // simple_expression -> - term.
        (yyval.simple_expression_node_info).type_ptr = (yyvsp[0].term_node_info).type_ptr;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode("-",true);
        (yyval.simple_expression_node_info).simple_expression_node->append_child(minus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> - term.\n");
    }
#line 2882 "src/parser.tab.cc"
    break;

  case 97: /* simple_expression: simple_expression ADDOP term  */
#line 1318 "scripts/parser.y"
    {
        // simple_expression -> simple_expression or term.
        // 类型检查
        // 类型强转 为 *tmp
        TypeTemplate* tmp = (yyvsp[-2].simple_expression_node_info).type_ptr;
        (yyval.simple_expression_node_info).type_ptr = tmp;


        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *addop_node = new LeafNode("or",true);
        (yyval.simple_expression_node_info).simple_expression_node->append_child(addop_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> simple_expression or term.\n");
    }
#line 2902 "src/parser.tab.cc"
    break;

  case 98: /* simple_expression: simple_expression PLUS term  */
#line 1334 "scripts/parser.y"
    { 
        // 类型检查
        // simple_expression -> simple_expression + term.
        TypeTemplate* tmp = (yyvsp[-2].simple_expression_node_info).type_ptr;
        (yyval.simple_expression_node_info).type_ptr = tmp;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *plus_node = new LeafNode("+",true);
        (yyval.simple_expression_node_info).simple_expression_node->append_child(plus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> simple_expression + term.\n");
    }
#line 2920 "src/parser.tab.cc"
    break;

  case 99: /* simple_expression: simple_expression UMINUS term  */
#line 1348 "scripts/parser.y"
    {
        // 类型检查
        // 类型强转 为 *tmp
        // simple_expression -> simple_expression - term.
        TypeTemplate* tmp = (yyvsp[-2].simple_expression_node_info).type_ptr;
        (yyval.simple_expression_node_info).type_ptr = tmp;

        (yyval.simple_expression_node_info).simple_expression_node = new SimpleExpressionNode();
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[-2].simple_expression_node_info).simple_expression_node);
        LeafNode *minus_node = new LeafNode("-",true);
        (yyval.simple_expression_node_info).simple_expression_node->append_child(minus_node);
        (yyval.simple_expression_node_info).simple_expression_node->append_child((yyvsp[0].term_node_info).term_node);
        if (DEBUG) printf("simple_expression -> simple_expression - term.\n");
    }
#line 2939 "src/parser.tab.cc"
    break;

  case 100: /* term: factor  */
#line 1364 "scripts/parser.y"
    {   // term -> factor.
        (yyval.term_node_info).type_ptr = (yyvsp[0].factor_node_info).type_ptr;
        
        (yyval.term_node_info).term_node = new TermNode();
        (yyval.term_node_info).term_node->append_child((yyvsp[0].factor_node_info).factor_node);
        if (DEBUG) printf("term -> factor.\n");
    }
#line 2951 "src/parser.tab.cc"
    break;

  case 101: /* term: term MULOP factor  */
#line 1372 "scripts/parser.y"
    {   
        // term -> term mulop factor.
        // 类型检查
        // 类型强转 为 *tmp
        TypeTemplate *tmp = (yyvsp[-2].term_node_info).type_ptr;
        (yyval.term_node_info).type_ptr = tmp;
        
        std::string mulop = (yyvsp[-1].token_info).value.m_OTHERS;
        if(mulop == "div"){
            mulop = "/";
        } else if (mulop == "mod"){
            mulop = "%";
        } else if (mulop == "and"){
            mulop = "&&";
        }
        (yyval.term_node_info).term_node = new TermNode();
        (yyval.term_node_info).term_node->append_child((yyvsp[-2].term_node_info).term_node);
        LeafNode *mulop_node = new LeafNode(mulop,true);
        (yyval.term_node_info).term_node->append_child(mulop_node);
        (yyval.term_node_info).term_node->append_child((yyvsp[0].factor_node_info).factor_node);
        if (DEBUG) printf("term -> term mulop factor.\n");
    }
#line 2978 "src/parser.tab.cc"
    break;

  case 102: /* factor: unsigned_const_variable  */
#line 1396 "scripts/parser.y"
    {   // factor -> unsigned_const_variable.
        (yyval.factor_node_info).type_ptr = (yyvsp[0].unsigned_constant_var_node_info).type_ptr;
        
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::UCONST_VAR);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].unsigned_constant_var_node_info).unsigned_constant_var_node);
        if (DEBUG) printf("factor -> unsigned_const_variable.\n");
    }
#line 2990 "src/parser.tab.cc"
    break;

  case 103: /* factor: variable  */
#line 1404 "scripts/parser.y"
    {   // factor -> variable.
        (yyval.factor_node_info).type_ptr = (yyvsp[0].variable_node_info).type_ptr;
        
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::VARIABLE);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].variable_node_info).variable_node);
        if (DEBUG) printf("factor -> variable.\n");
    }
#line 3002 "src/parser.tab.cc"
    break;

  case 104: /* factor: ID '(' expression_list ')'  */
#line 1412 "scripts/parser.y"
    {
        // factor -> id (expression_list).
        // 类型检查
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>((yyvsp[-3].token_info).value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }
        (yyval.factor_node_info).type_ptr = tmp->type();
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode((yyvsp[-3].token_info).value.m_OTHERS,true);
        (yyval.factor_node_info).factor_node->append_child(id_node);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[-1].expression_list_node_info).expression_list_node);
        if (DEBUG) printf("factor -> id (expression_list).\n");

    }
#line 3022 "src/parser.tab.cc"
    break;

  case 105: /* factor: '(' expression ')'  */
#line 1428 "scripts/parser.y"
    {
        // factor -> (expression).
        (yyval.factor_node_info).type_ptr = (yyvsp[-1].expression_node_info).type_ptr;
        
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::EXP);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[-1].expression_node_info).expression_node);
        if (DEBUG) printf("factor -> (expression).\n");
    }
#line 3035 "src/parser.tab.cc"
    break;

  case 106: /* $@4: %empty  */
#line 1436 "scripts/parser.y"
                     {column = buf.size();len = strlen(yytext);}
#line 3041 "src/parser.tab.cc"
    break;

  case 107: /* factor: '(' expression $@4 error ';'  */
#line 1437 "scripts/parser.y"
    {
        yyerror_("A closing parenthesis is expected.\n");
    }
#line 3049 "src/parser.tab.cc"
    break;

  case 108: /* factor: NOT factor  */
#line 1441 "scripts/parser.y"
    {   // factor -> not factor.
        // 类型检查
        (yyval.factor_node_info).type_ptr = (yyvsp[0].factor_node_info).type_ptr;
        
        (yyval.factor_node_info).factor_node = new FactorNode(FactorNode::GrammarType::NOT);
        (yyval.factor_node_info).factor_node->append_child((yyvsp[0].factor_node_info).factor_node);
        if (DEBUG) printf("factor -> not factor.\n");
    }
#line 3062 "src/parser.tab.cc"
    break;

  case 109: /* unsigned_const_variable: num  */
#line 1451 "scripts/parser.y"
    {
        // unsigned_const_variable -> num
        (yyval.unsigned_constant_var_node_info).type_ptr = (yyvsp[0].value_node_info).type_ptr;

        LeafNode *num_node;
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        //$$.unsigned_constant_var_node->append_child($1.const_variable_node);
        if((yyvsp[0].value_node_info).type_ptr==pascal_type::TYPE_INT){
            num_node = new LeafNode((yyvsp[0].value_node_info).value.m_INT);
        } else {
            num_node = new LeafNode((yyvsp[0].value_node_info).value.m_REAL);
        }
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(num_node);
        if (DEBUG) printf("unsigned_const_variable -> num\n");
    }
#line 3082 "src/parser.tab.cc"
    break;

  case 110: /* unsigned_const_variable: CHAR  */
#line 1467 "scripts/parser.y"
    {
        // unsigned_const_variable -> 'LETTER'
        (yyval.unsigned_constant_var_node_info).type_ptr = pascal_type::TYPE_CHAR;
        
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode((yyvsp[0].token_info).value.m_CHAR);
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(char_node);
        if (DEBUG) printf("unsigned_const_variable -> 'LETTER'\n");
    }
#line 3096 "src/parser.tab.cc"
    break;

  case 111: /* unsigned_const_variable: TRUE  */
#line 1477 "scripts/parser.y"
    {
        // unsigned_const_variable -> true
        (yyval.unsigned_constant_var_node_info).type_ptr = pascal_type::TYPE_BOOL;
        
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(true);
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(true_node);
        if (DEBUG) printf("unsigned_const_variable -> true\n");
    }
#line 3110 "src/parser.tab.cc"
    break;

  case 112: /* unsigned_const_variable: FALSE  */
#line 1487 "scripts/parser.y"
    {   
        // unsigned_const_variable -> false
        (yyval.unsigned_constant_var_node_info).type_ptr = pascal_type::TYPE_BOOL;
        
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(false);
        (yyval.unsigned_constant_var_node_info).unsigned_constant_var_node->append_child(false_node);
        if (DEBUG) printf("unsigned_const_variable -> false\n");
    }
#line 3124 "src/parser.tab.cc"
    break;


#line 3128 "src/parser.tab.cc"

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

#line 1497 "scripts/parser.y"

 

void yyerror(ast::AST* real_ast,char *msg){
    printf("%d :",line_count);
    fprintf(stderr,"error:%s\n",msg);
    error_flag = 1;
}

// int main(){
//     AST *real_ast = new AST();
//     if(!yyparse(real_ast)&&!error_flag){
//         printf("successful analysis\n");
//         Compiler compiler;
//         compiler.Compile(real_ast,"basic_test");
//     }
// }

int yywrap(){
    return 1;
}
