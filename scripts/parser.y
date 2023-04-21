%{	
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

%}

%union
{
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
}
%parse-param {ast::AST *real_ast}
%start program
%token PROGRAM FUNCTION PROCEDURE TO DOWNTO SUBCATALOG
%token ARRAY VAR TYPE BASIC_TYPE INT_NUM REAL_NUM CHAR CONST RECORD
%token IF THEN ELSE CASE OF WHILE DO FOR REPEAT UNTIL BEGIN_ END
%token RELOP ADDOP MULOP NOT PLUS UMINUS ASSIGNOP TRUE FALSE
%token<token_info> ID
%type<program_head_node> program_head
%type<program_body_node> program_body
%type<id_list_node> id_list
%type<const_declarations_node> const_declarations
%type<variable_declarations_node> var_declarations
%type<type_declarations_node> type_declarations
%type<subprogram_declarations_node> subprogram_declarations
%type<compound_statement_node> compound_statement
%type<statement_list_node> statement_list
%type<statement_node> statement

%%

program : 
    program_head program_body '.'
    {   
        ProgramNode* node = new ProgramNode();
        // node->append_child(dynamic_cast<ProgramHeadNode*>($1));
        // node->append_child(dynamic_cast<ProgramBodyNode*>($2));
        node->append_child($1);
        node->append_child($2);
        real_ast->set_root(node);
        if(DEBUG) printf("program -> program_head program_body.\n");
    };
program_head :
    PROGRAM ID '(' id_list ')' ';' {
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        //$$->append_child(dynamic_cast<IdListNode*>($4));
        $$->append_child($4);
        if(DEBUG) printf("program_head -> program id(id_list);\n");
    }
    | {fresh_argu();}error ';'{
        yyerror_("Every program must begin with the symbol program.\n");
    }
    | PROGRAM {column = buf.size();len = strlen(yytext);} error ';'{
        yyerror_("An identifier is expected.\n");
    };
program_body :
    const_declarations type_declarations var_declarations 
    subprogram_declarations compound_statement {
        $$ = new ProgramBodyNode();
        // $$->append_child(dynamic_cast<ConstDeclarationsNode*>($1));
        // $$->append_child(dynamic_cast<TypeDeclarationsNode*>($2));
        // $$->append_child(dynamic_cast<VariableDeclarationsNode*>($3));
        // $$->append_child(dynamic_cast<SubprogramDeclarationsNode*>($4));
        // $$->append_child(dynamic_cast<CompoundStatementNode*>($5));
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
        $$->append_child($5);
        if(DEBUG) printf("program_body -> const_declarations type_declarations var_declarations subprogram_declarations compound_statement\n");
    };                  
id_list :
    id_list ',' ID { 
        $$ = new IdListNode((IdListNode::GrammarType)1);
        LeafNode* leaf_node = new LeafNode($3.value.m_OTHERS,false);
        //$$->append_child(dynamic_cast<IdListNode*>($1));
        $$->append_child($1);
        $$->append_child(leaf_node);
    } | ID {   
        $$ = new IdListNode((IdListNode::GrammarType)0);
        LeafNode* leaf_node = new LeafNode($1.value.m_OTHERS,false);
        $$->append_child(leaf_node);
    };
const_declarations :{
        $$ = new ConstDeclarationsNode();
    }
    | CONST const_declaration ';'
    {   
        // const_declarations -> const const_declaration 
        if(DEBUG) printf("const_declarations -> const const_declaration\n");
    };
const_declaration :
    const_declaration ';' ID "=" const_variable
    {
        //TODO 插入符号表
        //new(ID.name, ID.type, ID.decl_line,ID.val)
        // const_declaration -> const_declaration ; id = const_variable.
    }
    | ID "=" const_variable
    {   //TODO 插入符号表
        //new(ID.name, $3.type, ID.decl_line,$3.val)
        // Insert(ID.name, tmp)
        //检查是否重复定义
        // const_declaration -> id = const_variable.
    };
const_variable :
    PLUS ID
    {   
        //TODO
        // const_variable -> + id.
    }
    | UMINUS ID
    {
        //TODO
        // const_variable -> - id.
    }
    | ID
    {
        // const_variable -> id.
    }
    |UMINUS num
    {   
        //TODO
        // const_variable -> - num.
    }
    | num
    {   
        // const_variable -> num.
    }
    |PLUS num
    {  
        // const_variable -> +num.
    }
    | CHAR
    {
        // const_variable -> 'letter'.
    };

num :
    INT_NUM
    {
        //$$.num = $1; TODO
        // num -> int_num.
    }
    | REAL_NUM
    {   
        //$$.real_num = $1; TODO
        // num -> real_num.
    };
type_declarations : {
        // type_declarations -> empty.
        $$ = new TypeDeclarationsNode();
    }
    | TYPE type_declaration ';'
    {
        // type_declarations -> type type_declaration.
    };
type_declaration :
    type_declaration ';' ID '=' type
    {
        // TODO!
        // type_declaration -> type_declaration ; id = type.
    }
    | ID '=' type
    {
        // TODO!
        // type_declaration -> id = type.
    };
type :
    standrad_type
    {
        // TODO
        // $$ = $1 
        // type -> standrad_type.
    }
    | ARRAY '[' periods ']' OF type
    {
        // TODO
        // $$ = new ArrayType($6, $3);
        // type -> array [periods] of stype.
    }
    | RECORD record_body END
    {
        // TODO
        // $$ = new RecordType($2);
        // type -> record record_body end.
    };
record_body :
    {
        // record_body -> empty.
    }
    | var_declaration
    {
        // TODO
        // $$ = $1; std::unordered_map<std::string, TypeTemplate*>
        // record_body -> var_declaration.
    };
standrad_type :
    BASIC_TYPE
    {
        // todo
        //if($1 == "integer")
        //$$ = INT_PTR;
        // standrad_type -> int|real|bool|char.
    };
periods :
    periods ',' period
    {
        // $1->insert($1->end(), $3->begin(), $3->end());
        // delete $3;
        // $$ = $1;
        // periods -> periods,period.
    }
    | period
    {
        //$$ = $1;
        // periods -> period.
    };
period :
    const_variable SUBCATALOG const_variable
    {
        // TODO
        // test type
        // $$ = new std::vector<std::pair<int, int>>={$1, $3}; 
        // period -> const_variable .. const_variable.
    };
var_declarations : 

    {
        // var_declarations -> empty.
        $$ = new VariableDeclarationsNode();
    }
    | VAR var_declaration ';'
    {
        // var_declarations -> var var_declaration.
    };
var_declaration :
    var_declaration ';' id_list ':' type 
    {
        // TODO 插入符号表
        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // $$->insert($3[i], $4);
        // var_declaration -> var_declaration ; id_list : type.
    }
    | id_list ':' type 
    {
        // TODO 插入符号表
        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // $$->insert($1[i], $3);
        // var_declaration -> id : type.
    }
    |var_declaration ';' id_list ':' ID
    {
        //TODO:插入符号表
        //TYPE = TypeSymbolTable.Find($.name)???
        //symboltable.insert(id_list[i].first,TYPE)

        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // T *Find(std::string name);
        // $$->insert($3[i], T);
    }
    |id_list ':' ID
    {
        //TODO:插入符号表
        //TYPE = TypeSymbolTable.Find(ID.name)???
        //symboltable.insert(id_list[i].first,TYPE)

        // $$ = new std::unordered_map<std::string, TypeTemplate*>;//free?
        // T *Find(std::string name);
        // $$->insert($1[i], T);
    }
    | {column = buf.size(); len = strlen(yytext);} error ';'
    {
        yyerror_("An identifier is expected.");
        // if ( DEBUG ) printf("An identifier is expected.\n");
    };
subprogram_declarations : {
        // subprogram_declarations -> empty.
        $$ = new SubprogramDeclarationsNode();
    }
    | subprogram_declarations subprogram_declaration ';'
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration.
    };
subprogram_declaration :
    subprogram_head subprogram_body
    {
        // subprogram_declaration -> subprogram_head program_body.
    };
subprogram_body :
    const_declarations type_declarations var_declarations compound_statement
    {
        
    };
subprogram_head :
    FUNCTION ID formal_parameter ':' standrad_type ';'
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,$5,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> function id formal_parametert : standrad_type.
    }
    | PROCEDURE ID formal_parameter ';'
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,NULL,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> procedure id formal_parametert.
    };
formal_parameter :
    {   
        // formal_parameter -> empty.
    }
    | '(' parameter_lists ')'
    {
        // formal_parameter -> (parameter_lists).
    };
parameter_lists :
    parameter_lists ';' parameter_list
    {   
        // parameter_lists -> parameter_lists ; parameter_list.
    }
    | parameter_list
    {  
        // parameter_lists -> parameter_list.
    };
parameter_list :
    var_parameter
    {   
        // parameter_list -> var_parameter.
    }
    | value_parameter
    {   
        // parameter_list -> value_parameter.
    };
var_parameter :
    VAR value_parameter
    {   
        // var_parameter -> var value_parameter.
    };
value_parameter :
    id_list ':' standrad_type
    {   
        // value_parameter -> id_list : standrad_type.
    };
compound_statement :
    BEGIN_ statement_list END {
        // compound_statement -> begin statement_list end.
        $$ = new CompoundStatementNode();
        //$$->append_child(dynamic_cast<StatementListNode*>($2));
        $$->append_child($2);
        if (DEBUG) printf("compound_statement -> begin statement_list end.\n");
    };
statement_list :
    statement_list ';' statement
    {
        // TODO
        // statement_list -> statement_list ; statement.
    } | statement
    {
        // statement_list -> statement.
        $$ = new StatementListNode();
        //$$->append_child(dynamic_cast<StatementNode*>($1));
        $$->append_child($1);
    };
statement:
    variable ASSIGNOP expression
    {   // 类型检查
        // statement -> variable assigbop expression.
    }
    | call_procedure_statement
    {
        // statement -> call_procedure_statement.
    }
    | compound_statement
    {
        // statement -> compound_statement.
    }
    | IF expression THEN statement else_part
    {   
        //类型检查
        // statement -> if expression then statement else_part.
    }
    | CASE expression OF case_body END
    {
        //类型检查
        // statement -> case expression of case_body end.
    }
    | WHILE expression DO statement
    {
        //类型检查
        // statement -> while expression do if_statement_1.
    } 
    | REPEAT statement_list UNTIL expression
    {
        //类型检查
        // statement -> repeat statement_list until expression.
    }
    | FOR ID ASSIGNOP expression updown expression DO statement
    {
        //类型检查
        // statement -> for id assignop expression updown expression do statement.
    } | {
        // statement -> empty.
        StatementNode *node = new StatementNode((StatementNode::GrammarType)0);
        $$ = node;
    };

variable:
    ID id_varparts
    {
        //类型检查
        // variable -> id id_varparts.
    };

id_varparts:
    {
        // id_varparts -> empty.
    }
    | id_varparts id_varpart
    {
        // $$ = $1;
        // $$->insert($$->end(), $2->begin(), $2->end());
        // id_varparts -> id_varparts id_varpart.
    };

id_varpart:
    '[' expression_list ']'
    {   
        // $$ = $1; 
        // id_varpart -> [expression_list].
    }
    | '.' ID
    {
        // id_varpart -> .id.
    };
else_part:
    {
        // else_part -> empty.
    }
    | ELSE statement 
    {
        // else_part -> else statement.
    } ;
case_body:
    {
        // case_body -> empty.
    }
    | branch_list
    {
        // case_body -> branch_list.
    };
branch_list:
    branch_list ';' branch
    {
        // branch_list -> branch_list branch.
    }
    | branch
    {
        // branch_list -> branch.
    };
branch:
    const_list ':' statement
    {
        // branch -> const_list : statement.
    };
const_list:
    const_list ',' const_variable
    {
        // const_list -> const_list , const_variable.
    }
    | const_variable
    {
        // const_list -> const_variable.
    };
updown:
    TO
    {
        // updown -> to.
    }
    | DOWNTO
    {
        // updown -> downto.
    };
call_procedure_statement:
    ID '(' expression_list ')'
    {
        //类型检查
        // call_procedure_statement -> id (expression_list).
    };
    | ID
    {   //类型检查
        // call_procedure_statement -> id.
    };
expression_list:
    expression_list ',' expression
    {
        //类型检查 检查是否为INT or CHAR
        // $$ = $1;
        // $$->push_back($3);
        // expression_list -> expression_list , expression.
    }
    | expression
    {
        //类型检查 检查是否为INT or CHAR    
        // $$ = new vector<BasicType*>;
        // $$->push_back($1);
        // expression_list -> expression.
    };
expression:
    simple_expression RELOP simple_expression
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // expression -> simple_expression relop simple_expression.
    }
    | simple_expression
    {
        // $$ = $1;
        // expression -> simple_expression.
    };
simple_expression:
    term
    {   // $$ = $1;
        // simple_expression -> term.
    }
    |PLUS term
    {

    }
    |UMINUS term
    {

    }
    | simple_expression ADDOP term
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression or term.
    }
    | simple_expression PLUS term
    { 
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression + term.
    }
    | simple_expression UMINUS term
    {
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // simple_expression -> simple_expression - term.
    };
term:
    factor
    {   // $$ = $1;
        // term -> factor.
    }
    | term MULOP factor
    {   
        // 类型检查
        // 类型强转 为 *tmp
        // $$ = tmp;
        // term -> term mulop factor.
    };
factor:
    unsigned_const_variable
    {
        // $$ = $1;
        // factor -> unsigned_const_variable.
    }
    | variable
    {
        // $$ = $1;
        // factor -> variable.
    }
    |ID '(' expression_list ')'
    {
        // $$ = $1;
        // factor -> id (expression_list).
    }
    | '(' expression ')'
    {
        // $$ = $2;
        // factor -> (expression).
    }
    | '(' expression {column = buf.size();len = strlen(yytext);}error ';'
    {
        yyerror_("A closing parenthesis is expected.\n");
    }
    | NOT factor
    {
        // $$ = $2;
        // factor -> not factor.
    };
unsigned_const_variable :
    num
    {
        //$$ = $1.type;
        // unsigned_const_variable -> num
    };
    | CHAR
    {
        //$$ = CHAR;
        // unsigned_const_variable -> 'LETTER'
    }
    |TRUE
    {
        //$$ = BOOL_ptr;
        // unsigned_const_variable -> true
    }
    | FALSE
    {   //$$ = BOOL_ptr;
        // unsigned_const_variable -> false
    };

%%
 

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
