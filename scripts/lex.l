%option case-insensitive
%{   
#include "parser.h"
#include "parser.tab.h"

int line_count = 1;
int char_count = 1;
int i;
std::string buf;

void wrap();
void process_anno();
void cache_line();

void to_str();
void to_int();
void to_double();
void to_char();
void to_bool();

extern "C"
{
    int yywrap();
    int yylex();
    void yyerror(const char *s);
}

%}

delim                   [ ]
ws                      {delim}+
letter                  [a-zA-Z]
digit                   [0-9]
digits                  {digit}+
fraction                (\.{digits})

/* 关键字 */
PROGRAM                 "program"
CONST                   "const"
TYPE                    "type"
RECORD                  "record"
ARRAY                   "array"
OF                      "of"
VAR                     "var"
FUNCTION                "function"
PROCEDURE               "procedure"
BEGIN_                  "begin"
END                     "end"
IF                      "if"
THEN                    "then"
ELSE                    "else"
CASE                    "case"
WHILE                   "while"
REPEAT                  "repeat"
UNTIL                   "until"
FOR                     "for"
TO                      "to"
DOWNTO                  "downto"
DO                      "do"
READ                    "read"
WRITE                   "write"
NOT                     "not"
BASIC_TYPE              ("integer"|"real"|"Boolean"|"char")
TRUE                    "true"
FALSE                   "false"
PLUS                    "+"
UMINUS                  "-"

/* 常数 */
INT_NUM                 {digits}
REAL_NUM                {digits}{fraction}

/* 注释 */
ANNO                    (\{[^}]*\})      

/* 标识符 */
ID                      {letter}({letter}|{digit})*

/* 字符 */
CHAR                    ('{letter}')

/* 运算符 */
RELOP                   ("="|"<>"|"<"|"<="|">"|">=")      
ADDOP                   "or"
MULOP                   ("*"|"/"|"div"|"mod"|"and")
ASSIGNOP                ":="

/* 分隔符 */
SEP                     (";"|"."|"("|")"|","|":"|"["|"]")
SUBCATALOG              ".."
  
%% 
"\t"                    { cache_line(); char_count += 4; }
"\n"                    { wrap(); }
{ws}                    { cache_line(); char_count += yyleng; }

{PROGRAM}               { cache_line(); to_str(); return PROGRAM; }
{CONST}                 { cache_line(); to_str(); return CONST; }
{TYPE}                  { cache_line(); to_str(); return TYPE; }

{ARRAY}                 { cache_line(); to_str(); return ARRAY; }
{OF}                    { cache_line(); to_str(); return OF; }
{VAR}                   { cache_line(); to_str(); return VAR; }
{FUNCTION}              { cache_line(); to_str(); return FUNCTION; }
{PROCEDURE}             { cache_line(); to_str(); return PROCEDURE; }
{BEGIN_}                { cache_line(); to_str(); return BEGIN_; }
{END}                   { cache_line(); to_str(); return END; }
{IF}                    { cache_line(); to_str(); return IF; }
{THEN}                  { cache_line(); to_str(); return THEN; }
{ELSE}                  { cache_line(); to_str(); return ELSE; }
{CASE}                  { cache_line(); to_str(); return CASE; }
{WHILE}                 { cache_line(); to_str(); return WHILE; }
{REPEAT}                { cache_line(); to_str(); return REPEAT; }
{UNTIL}                 { cache_line(); to_str(); return UNTIL; }
{FOR}                   { cache_line(); to_str(); return FOR; }
{TO}                    { cache_line(); to_str(); return TO; }
{DOWNTO}                { cache_line(); to_str(); return DOWNTO; }
{DO}                    { cache_line(); to_str(); return DO; }


{NOT}                   { cache_line(); to_str(); return NOT; }
{BASIC_TYPE}            { cache_line(); to_str(); return BASIC_TYPE; }
{TRUE}                  { cache_line(); to_bool(); return TRUE; }
{FALSE}                 { cache_line(); to_bool(); return FALSE; }
{PLUS}                  { cache_line(); to_str(); return PLUS; }
{UMINUS}                { cache_line(); to_str(); return UMINUS; }

{INT_NUM}               { cache_line(); to_int(); return INT_NUM; }
{REAL_NUM}              { cache_line(); to_double(); return REAL_NUM; }

{ANNO}                  { cache_line(); process_anno(); }

{CHAR}                  { cache_line(); to_char(); return CHAR; }

{RELOP}                 { cache_line(); to_str(); return RELOP; }
{ADDOP}                 { cache_line(); to_str(); return ADDOP; }
{MULOP}                 { cache_line(); to_str(); return MULOP; }
{ASSIGNOP}              { cache_line(); to_str(); return ASSIGNOP; }

{ID}                    { cache_line(); to_str(); return ID; }

{SEP}                   { cache_line(); to_str(); return yytext[0]; }
{SUBCATALOG}            { cache_line(); to_str(); return SUBCATALOG; }

('')                                            { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); }
('{letter}({letter}|[^{letter}'])+')            { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); }
('[^{letter}']({letter}|[^{letter}'])*')        { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); }

(\{[^}]*)               { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); }
('[^']*)                { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); }

.                       { cache_line(); printf("ERROR\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count); char_count++; }

%%
void wrap()
{
    buf.clear();
    line_count++; 
    char_count = 1;
}

void process_anno()
{
    printf("ANNO\t\t%s\t\t\t%d\t\t%d\n", yytext, line_count, char_count);
    for(i = 0; i < strlen(yytext); i++)
    {
        if(yytext[i] == '\n') wrap();
        else char_count++;
    }
}

void cache_line()
{
    buf += yytext;

}

void to_str()
{
    yylval.token_info.value.m_OTHERS = yytext;
    yylval.token_info.line_num = line_count;
    yylval.token_info.column_num = char_count;
    yylval.token_info.length = yyleng;
    //printf("to_str is ok\n");
}

void to_int()
{
    yylval.token_info.value.m_INT = std::stoi(yytext);
    yylval.token_info.line_num = line_count;
    yylval.token_info.column_num = char_count;
    yylval.token_info.length = yyleng;
}

void to_double()
{
    yylval.token_info.value.m_REAL = std::stod(yytext);
    yylval.token_info.line_num = line_count;
    yylval.token_info.column_num = char_count;
    yylval.token_info.length = yyleng;
}

void to_char()
{
    yylval.token_info.value.m_CHAR = yytext[1];
    yylval.token_info.line_num = line_count;
    yylval.token_info.column_num = char_count;
    yylval.token_info.length = yyleng;
}

void to_bool()
{
    if(yytext[0] == 'T') yylval.token_info.value.m_BOOLEAN = 1;
    else yylval.token_info.value.m_BOOLEAN = 0;
    yylval.token_info.line_num = line_count;
    yylval.token_info.column_num = char_count;
    yylval.token_info.length = yyleng;
}

int yywrap()
{
    return 1;
}