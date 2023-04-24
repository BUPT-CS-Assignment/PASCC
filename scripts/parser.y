%{	
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

%}

%union
{
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

}
%parse-param {ast::AST *real_ast}
%start program
%token PROGRAM FUNCTION PROCEDURE TO DOWNTO SUBCATALOG
%token ARRAY VAR TYPE CONST RECORD
%token IF THEN ELSE CASE OF WHILE DO FOR REPEAT UNTIL BEGIN_ END
%token ADDOP NOT PLUS UMINUS ASSIGNOP TRUE FALSE CONSTASSIGNOP
%token<token_info> ID CHAR INT_NUM REAL_NUM BASIC_TYPE RELOP MULOP
%type<id_list_node_info> id_list
%type<value_node_info> const_variable num
%type<periods_node_info> periods
%type<period_node_info> period
%type<type_node_info> type
%type<record_node_info> record_body
%type<variable_declaration_node_info> var_declaration
%type<standared_type_node_info> standrad_type
%type<formal_parameter_node_info> formal_parameter
%type<parameter_lists_node_info> parameter_lists
%type<parameter_list_node_info> parameter_list
%type<var_parameter_node_info> var_parameter
%type<value_parameter_node_info> value_parameter
%type<variable_node_info> variable
%type<expression_node_info> expression
%type<simple_expression_node_info> simple_expression
%type<term_node_info> term
%type<factor_node_info> factor
%type<unsigned_constant_var_node_info> unsigned_const_variable
%type<id_varparts_node_info> id_varparts
%type<id_varpart_node_info> id_varpart
%type<expression_list_node_info> expression_list
%type<case_body_node_info> case_body
%type<branch_list_node_info> branch_list
%type<branch_node_info> branch
%type<const_list_node_info> const_list

%type<program_head_node> program_head
%type<program_body_node> program_body
%type<const_declarations_node> const_declarations
%type<const_declaration_node> const_declaration
%type<variable_declarations_node> var_declarations
%type<type_declarations_node> type_declarations
%type<type_declaration_node> type_declaration
%type<subprogram_declarations_node> subprogram_declarations
%type<subprogram_declaration_node> subprogram_declaration
%type<subprogram_head_node> subprogram_head
%type<subprogram_body_node> subprogram_body
%type<compound_statement_node> compound_statement
%type<statement_list_node> statement_list
%type<statement_node> statement
%type<else_node> else_part
%type<updown_node> updown
%type<procedure_call_node> call_procedure_statement

%%

program : 
    program_head program_body '.'
    {   
        ProgramNode* node = new ProgramNode();
        // node->append_child(dynamic_cast<ProgramHeadNode*>($1));
        // node->append_child(dynamic_cast<ProgramBodyNode*>($2));
        node->append_child($1);
        node->append_child($2);
        if(!error_flag){
            real_ast->set_root(node);
        } else {
            real_ast->set_root(nullptr); 
        }
        
        if(DEBUG) printf("program -> program_head program_body.\n");
    };
program_head :
    PROGRAM ID '(' id_list ')' ';' {
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        $$->append_child($4.id_list_node);
        if(DEBUG) printf("program_head -> program id(id_list);\n");
        table_set_queue.push(top_table_set);
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
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
        $$->append_child($5);
        if(DEBUG) printf("program_body -> const_declarations type_declarations var_declarations subprogram_declarations compound_statement\n");
    };                  
id_list :
    id_list ',' ID { 
        $1.list_ref->push_back(std::make_pair($3.value.m_OTHERS,$3.line_num));
        $$.list_ref = $1.list_ref;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        LeafNode* leaf_node = new LeafNode($3.value.m_OTHERS,false);
        $$.id_list_node->append_child($1.id_list_node);
        $$.id_list_node->append_child(leaf_node);
    } | ID {
        $$.list_ref = new std::vector<std::pair<std::string,int>>();
        $$.list_ref->push_back(std::make_pair($1.value.m_OTHERS,$1.line_num));
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode($1.value.m_OTHERS,false);
        $$.id_list_node->append_child(leaf_node);
    };
const_declarations :{
        $$ = new ConstDeclarationsNode();
    }
    | CONST const_declaration ';'
    {   
        // const_declarations -> const const_declaration
        $$ = new ConstDeclarationsNode(); 
        $$->append_child($2);
        if(DEBUG) printf("const_declarations -> const const_declaration\n");
    };
const_declaration :
    const_declaration ';' ID '=' const_variable
    {
        //TODO 插入符号表
        pascal_symbol::ConstSymbol *symbol ;
        if ($5.type_ptr == pascal_type::TYPE_INT){
            symbol = new ConstSymbol($3.value.m_OTHERS,$3.line_num,$5.value.m_INT);
        }
        else if ($5.type_ptr == pascal_type::TYPE_REAL){
            symbol = new ConstSymbol($3.value.m_OTHERS,$3.line_num,$5.value.m_REAL);
        }
        else if ($5.type_ptr == pascal_type::TYPE_CHAR){
            symbol = new ConstSymbol($3.value.m_OTHERS,$3.line_num,$5.value.m_CHAR);
        }else {
            symbol = new ConstSymbol($3.value.m_OTHERS,$3.line_num,$5.value.m_BOOLEAN);
        }

        if(!table_set_queue.top()->Insert<ConstSymbol>($3.value.m_OTHERS,symbol)){
            yyerror(real_ast,"The identifier has been declared.\n");
        } 
        else{
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION);
            $$->append_child($1);
            LeafNode* leaf_node = new LeafNode($3.value.m_OTHERS,false);
            $$->append_child(leaf_node);
            $$->append_child($5.const_variable_node);
            // const_declaration -> const_declaration ; id = const_variable.
            if(DEBUG) printf("const_declaration ->const_declaration ; id = const_variable.\n");
        }
        
    }
    | ID '=' const_variable
    {   // const_declaration -> id = const_variable.
        //TODO 插入符号表
        pascal_symbol::ConstSymbol *symbol ;
        if ($3.type_ptr == pascal_type::TYPE_INT){
            symbol = new ConstSymbol($3.value.m_OTHERS,$1.line_num,$3.value.m_INT);
        }
        else if ($3.type_ptr == pascal_type::TYPE_REAL){
            symbol = new ConstSymbol($3.value.m_OTHERS,$1.line_num,$3.value.m_REAL);
        }
        else if ($3.type_ptr == pascal_type::TYPE_CHAR){
            symbol = new ConstSymbol($3.value.m_OTHERS,$1.line_num,$3.value.m_CHAR);
        }else {
            symbol = new ConstSymbol($3.value.m_OTHERS,$1.line_num,$3.value.m_BOOLEAN);
        }

        if(!table_set_queue.top()->Insert<ConstSymbol>($1.value.m_OTHERS,symbol)){
            yyerror(real_ast,"The identifier has been declared.\n");
        } 
        else {
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE);
            LeafNode* leaf_node = new LeafNode($1.value.m_OTHERS,false);
            $$->append_child(leaf_node);
            $$->append_child($3.const_variable_node);
            if(DEBUG) printf("const_declaration -> id = const_variable.\n");
        } 
    };
const_variable :
    PLUS ID
    {   
        // const_variable -> + id.
        ConstSymbol  *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            $$.type_ptr = symbol->type();
            if($$.type_ptr==pascal_type::TYPE_INT){
                $$.value.m_INT = symbol->value<int>();
            } else if($$.type_ptr==pascal_type::TYPE_REAL){
                $$.value.m_REAL = symbol->value<float>();
            } else if($$.type_ptr==pascal_type::TYPE_BOOL){
                $$.value.m_BOOLEAN = symbol->value<bool>();
            } else {
                $$.value.m_CHAR = symbol->value<char>();
            }
            //$$.const_value = symbol->value();
            std::string s1("+");
            std::string s2($2.value.m_OTHERS);
            s1+=s2;
            $$.const_variable_node = new LeafNode(s1,false);
            
        }
        if(DEBUG) printf("const_variable -> +id.\n");
    }
    | UMINUS ID
    {
        // const_variable -> - id. todo -
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            $$.type_ptr = symbol->type();
            if($$.type_ptr==pascal_type::TYPE_INT){
                $$.value.m_INT = -symbol->value<int>();
            } else if($$.type_ptr==pascal_type::TYPE_REAL){
                $$.value.m_REAL = -symbol->value<float>();
            } else if($$.type_ptr==pascal_type::TYPE_BOOL){
                $$.value.m_BOOLEAN = symbol->value<bool>();// todo
            } else {
                $$.value.m_CHAR = -symbol->value<char>();
            }
            //$$.const_value = -(symbol->value());
            std::string s1("-");
            std::string s2($2.value.m_OTHERS);
            s1+=s2;
            $$.const_variable_node = new LeafNode(s1,false);
        }
        
        if(DEBUG) printf("const_variable -> -id.\n");
    }
    | ID
    {
        // const_variable -> id.
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($1.value.m_OTHERS);
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.\n");
        }else {
            $$.type_ptr = symbol->type();
            if($$.type_ptr==pascal_type::TYPE_INT){
                $$.value.m_INT = symbol->value<int>();
            } else if($$.type_ptr==pascal_type::TYPE_REAL){
                $$.value.m_REAL = symbol->value<float>();
            } else if($$.type_ptr==pascal_type::TYPE_BOOL){
                $$.value.m_BOOLEAN = symbol->value<bool>();
            } else {
                $$.value.m_CHAR = symbol->value<char>();
            }
        }
        $$.const_variable_node = new LeafNode($1.value.m_OTHERS,false);
        if(DEBUG) printf("const_variable -> id.\n");
    }
    |UMINUS num
    {   
        //TODO
        // const_variable -> - num.
        $$.type_ptr = $2.type_ptr;
        if($2.type_ptr==pascal_type::TYPE_INT){
            $$.value.m_INT = $2.value.m_INT*(-1);
            //$$.const_value = new ConstValue($2.value.m_INT*(-1));
            $$.const_variable_node = new LeafNode(-($2.value.m_INT));
        } else {
            $$.value.m_REAL = $2.value.m_REAL*(-1);
            //$$.const_value = new ConstValue($2.value.m_REAL*(-1));
            $$.const_variable_node = new LeafNode(-($2.value.m_REAL));
        }
        if(DEBUG) printf("const_variable -> -num.\n");
        
    }
    | num
    {   
        // const_variable -> num.
        $$.type_ptr = $1.type_ptr;
        $$.value = $1.value;
        if($1.type_ptr==pascal_type::TYPE_INT){
            $$.const_variable_node = new LeafNode($1.value.m_INT);
            //$$.const_value = new ConstValue($1.value.m_INT);
        } else {
            $$.const_variable_node = new LeafNode($1.value.m_REAL);
            //$$.const_value = new ConstValue($1.value.m_REAL);
        }
        if(DEBUG) printf("const_variable -> num.\n");
    }
    |PLUS num
    {  
        // const_variable -> +num.
        $$.type_ptr = $2.type_ptr;
        $$.value = $2.value;
        if($2.type_ptr==pascal_type::TYPE_INT){
            //$$.const_value = new ConstValue($2.value.m_REAL);
            $$.const_variable_node = new LeafNode($2.value.m_INT);
        } else {
            //$$.const_value = new ConstValue($2.value.m_REAL);
            $$.const_variable_node = new LeafNode($2.value.m_REAL);
        }
        if(DEBUG) printf("const_variable -> +num.\n");
        // $$.const_variable_node = new ConstVariableNode();
        // LeafNode* leaf_node = new LeafNode($2.value.m_INT);//?
        // $$.const_variable_node->append_child(leaf_node);
    }
    | CHAR
    {
        // const_variable -> 'letter'.
        $$.type_ptr = pascal_type::TYPE_CHAR;
        $$.value.m_CHAR = $1.value.m_CHAR;
        //$$.const_value = new ConstValue($2.value.m_CHAR);
        $$.const_variable_node = new LeafNode($1.value.m_CHAR);
        // $$.const_variable_node = new ConstVariableNode();
        // LeafNode* leaf_node = new LeafNode($1.value.m_CHAR);
        // $$.const_variable_node->append_child(leaf_node);
        if(DEBUG) printf("const_variable -> char.\n");

    };

num :
    INT_NUM
    {
        // num -> int_num.
        $$.type_ptr = pascal_type::TYPE_INT;
        $$.value.m_INT = $1.value.m_INT;
        if(DEBUG) printf("num -> int_num.\n");
    }
    | REAL_NUM
    {   
        // num -> real_num.
        $$.type_ptr = pascal_type::TYPE_REAL;
        $$.value.m_REAL = $1.value.m_REAL;
        if(DEBUG) printf("num -> real_num.\n");
    };
type_declarations : {
        // type_declarations -> empty.
        $$ = new TypeDeclarationsNode();
        if(DEBUG) printf("type_declarations -> empty.\n");
    }
    | TYPE type_declaration ';'
    {
        $$ = new TypeDeclarationsNode();
        $$->append_child($2);
        // type_declarations -> type type_declaration.
        if(DEBUG) printf("type_declarations -> type type_declaration\n");
    };
type_declaration :
    type_declaration ';' ID '=' type
    {
        // TODO
        // type_declaration -> type_declaration ; id = type.
        if ($5.main_type == TypeAttr::BASIC) {
            pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>($5.type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>($3.value.m_OTHERS,basic_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            }
        } else if ($5.main_type == TypeAttr::ARRAY) {
            pascal_type::ArrayType *array_type = new pascal_type::ArrayType($5.type_ptr,*($5.bounds));
            if (!table_set_queue.top()->Insert<ArrayType>($3.value.m_OTHERS,array_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                $5.type_ptr = table_set_queue.top()->SearchEntry<ArrayType>($3.value.m_OTHERS);
            }
        } else if ($5.record_info) {
            pascal_type::RecordType *record_type = new pascal_type::RecordType(*($5.record_info));
            if (!table_set_queue.top()->Insert<RecordType>($3.value.m_OTHERS,record_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                $5.type_ptr = table_set_queue.top()->SearchEntry<RecordType>($3.value.m_OTHERS);
                $5.array_type_ptr = $5.type_ptr;
            }
        }

        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::MULTIPLE_DECL);
        $$->append_child($1);
        LeafNode *leaf_node = new LeafNode($3.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        $$->append_child($5.type_node);
        if(DEBUG) printf("type_declaration -> type_declaration ; id = type.\n");
    }
    | ID '=' type
    {
        // TODO!
        // type_declaration -> id = type.
        if ($3.main_type == TypeAttr::BASIC) {
            pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>($3.type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>($1.value.m_OTHERS,basic_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } 
        } else if ($3.main_type == TypeAttr::ARRAY) {
            pascal_type::ArrayType *array_type = new pascal_type::ArrayType($3.array_type_ptr,*($3.bounds));
            if (!table_set_queue.top()->Insert<ArrayType>($1.value.m_OTHERS,array_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                $3.type_ptr = table_set_queue.top()->SearchEntry<ArrayType>($1.value.m_OTHERS);
            }
        } else if ($3.record_info) {
            pascal_type::RecordType *record_type = new pascal_type::RecordType(*($3.record_info));
            if (!table_set_queue.top()->Insert<RecordType>($1.value.m_OTHERS,record_type)){
                yyerror(real_ast,"Error: redefinition of type %s.\n");
            } else {
                $3.type_ptr = table_set_queue.top()->SearchEntry<RecordType>($1.value.m_OTHERS);
                $3.array_type_ptr = $3.type_ptr;
            }
        }

        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::SINGLE_DECL);
        LeafNode *leaf_node = new LeafNode($1.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        $$->append_child($3.type_node);
        if(DEBUG) printf("type_declaration -> id = type.\n");
    };
type :
    standrad_type
    {
        // type -> standrad_type.
        $$.main_type = (TypeAttr::MainType)0;
        $$.type_ptr = $1.type_ptr;
        $$.array_type_ptr = $1.type_ptr;

        $$.type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        $$.type_node->set_base_type_node($$.type_node);
        $$.type_node->append_child($1.standard_type_node);
        if(DEBUG) printf("type -> standrad_type.\n");
    }
    | ARRAY '[' periods ']' OF type
    {
        // TODO
        // type -> array [periods] of stype.
        $$.main_type = (TypeAttr::MainType)1;
        $$.array_type_ptr = $6.array_type_ptr;
        $$.bounds = $3.bounds;

        $$.type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        $$.type_node->set_base_type_node($6.type_node);
        $$.type_node->append_child($3.periods_node);
        $$.type_node->append_child($6.type_node);
        if(DEBUG) printf("type -> array [periods] of type.\n");
    }
    | RECORD record_body END
    {
        // TODO
        $$.main_type = (TypeAttr::MainType)2;
        $$.record_info = $2.record_info;
        
        $$.type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        $$.type_node->append_child($2.record_body_node);
        $$.type_node->set_base_type_node($$.type_node);
        if(DEBUG) printf("type -> record record_body end.\n");
        // type -> record record_body end.
    };
record_body :
    {
        // record_body -> empty.
        $$.record_info = nullptr;
        $$.record_body_node = new RecordBodyNode();
        if(DEBUG) printf("record_body -> empty.\n");
    }
    | var_declaration
    {
        // record_body -> var_declaration.
        $$.record_info = $1.record_info;

        $$.record_body_node = new RecordBodyNode();
        $$.record_body_node->append_child($1.variable_declaration_node);
        if(DEBUG) printf("record_body -> var_declaration.\n");
    };
standrad_type :
    BASIC_TYPE
    {
        // standrad_type -> int|real|bool|char.
        //std::cout<<"$1.value.m_OTHERS:"<<$1.value.m_OTHERS<<std::endl;
        if (!strcmp($1.value.m_OTHERS,"integer")){
            $$.type_ptr = pascal_type::TYPE_INT;
        } else if(!strcmp($1.value.m_OTHERS,"real")){
            $$.type_ptr = pascal_type::TYPE_REAL;
        } else if(!strcmp($1.value.m_OTHERS,"Boolean")){
            $$.type_ptr = pascal_type::TYPE_BOOL;
        } else{
            $$.type_ptr = pascal_type::TYPE_CHAR;
        }
        $$.standard_type_node = new BasicTypeNode();
        $$.standard_type_node->set_type(dynamic_cast<pascal_type::BasicType*>($$.type_ptr));
    };
periods :
    periods ',' period
    {
        // periods -> periods,period.
        $$.bounds = $1.bounds;
        $$.bounds->push_back(*($3.bound));

        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.periods_node);
        $$.periods_node->append_child($3.period_node);
        if(DEBUG) printf("periods -> periods,period.");
    }
    | period
    {
        // periods -> period.
        $$.bounds = new std::vector<std::pair<int, int>>();
        $$.bounds->push_back(*($1.bound));
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.period_node);
        if(DEBUG) printf("periods -> period.");
    };
period :
    const_variable SUBCATALOG const_variable
    {
        // test type ID 
        // period -> const_variable .. const_variable.
        
        int arr_len;
        if (($1.type_ptr == pascal_type::TYPE_INT)&&($3.type_ptr == pascal_type::TYPE_INT)){
            arr_len = $3.value.m_INT - $1.value.m_INT;
            if(arr_len < 0){
                //ERROR
            }
            $$.bound = new std::pair<int, int>($1.value.m_INT, $3.value.m_INT);
        } else if(($1.type_ptr == pascal_type::TYPE_CHAR)&&($3.type_ptr == pascal_type::TYPE_CHAR)){
            arr_len =$3.value.m_CHAR - $1.value.m_CHAR;
            if(arr_len < 0){
                //ERROR
            }
            $$.bound = new std::pair<int, int>(int($1.value.m_CHAR), int($3.value.m_CHAR));
        } else{
            //ERROR
        }
        $$.period_node =new PeriodNode();
        $$.period_node->set_len(arr_len);
        $$.period_node->append_child($1.const_variable_node);
        $$.period_node->append_child($3.const_variable_node);
        if(DEBUG) printf("period -> const_variable .. const_variable.");
    };
var_declarations : 

    {
        // var_declarations -> empty.
        $$ = new VariableDeclarationsNode();
        if(DEBUG) printf("var_declarations -> empty.\n");
    }
    | VAR var_declaration ';'
    {
        // var_declarations -> var var_declaration.
        $$ = new VariableDeclarationsNode();
        $$->append_child($2.variable_declaration_node);
        if(DEBUG) printf("var_declarations -> var var_declaration.\n");

        for (auto i : *($2.record_info)){
            ObjectSymbol *obj = new ObjectSymbol(i.first, i.second,10);//TODO
            if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first,obj)){
                yyerror(real_ast,"redefinition of variable");
            }
        }
    };
var_declaration :
    var_declaration ';' id_list ':' type 
    {
        // var_declaration -> var_declaration ; id_list : type.
        $$.record_info = $1.record_info;
        for (auto i : *($3.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $5.type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");}
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        $$.variable_declaration_node->append_child($5.type_node);
        if(DEBUG) printf("var_declaration -> var_declaration ; id_list : type.\n");
    }
    | id_list ':' type 
    {
        $$.record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
        for (auto i : *($1.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $3.type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");}
        }
        // var_declaration -> id : type.
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.id_list_node);
        $$.variable_declaration_node->append_child($3.type_node);
        if(DEBUG) printf("var_declaration -> id : type.\n");
    }
    |var_declaration ';' id_list ':' ID
    {
        $$.record_info = $1.record_info;
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($5.value.m_OTHERS);
        if(tmp == nullptr){
            yyerror(real_ast,"undefined type");
        } else {
            for (auto i : *($3.list_ref)){
                auto res = $$.record_info->insert(make_pair(i.first, tmp));
                if (!res.second){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }  
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        LeafNode *leaf_node = new LeafNode($5.value.m_OTHERS,false);
        $$.variable_declaration_node->append_child(leaf_node);
        if(DEBUG) printf("var_declaration -> var_declaration ; id_list : ID.\n");
    }
    |id_list ':' ID
    {
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($3.value.m_OTHERS);
        if(tmp==nullptr){
            yyerror(real_ast,"undefined type");
        } else {
            $$.record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
            for (auto i : *($1.list_ref)){
                auto res = $$.record_info->insert(make_pair(i.first, tmp));
                if (!res.second){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }

        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.id_list_node);
        LeafNode *leaf_node = new LeafNode($3.value.m_OTHERS,false);
        $$.variable_declaration_node->append_child(leaf_node);
        if(DEBUG) printf("var_declaration -> id_list : ID.\n");
    }
    | {column = buf.size(); len = strlen(yytext);} error ';'
    {
        yyerror_("An identifier is expected.");
        // if ( DEBUG ) printf("An identifier is expected.\n");
    };
subprogram_declarations : {
        // subprogram_declarations -> empty.
        $$ = new SubprogramDeclarationsNode();
        if (DEBUG) printf("subprogram_declarations -> empty.\n");
    }
    | subprogram_declarations subprogram_declaration ';'
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration.
        $$ = new SubprogramDeclarationsNode();
        $$->append_child($1);
        $$->append_child($2);
        table_set_queue.pop();
        if  (DEBUG) printf("subprogram_declarations -> subprogram_declarations subprogram_declaration.\n");
        
    };
subprogram_declaration :
    subprogram_head subprogram_body
    {
        // subprogram_declaration -> subprogram_head program_body.
        $$ = new SubprogramDeclarationNode();
        $$->append_child($1);
        $$->append_child($2);
        if (DEBUG) printf("subprogram_declaration -> subprogram_head program_body.\n");
    };
subprogram_body :
    const_declarations type_declarations var_declarations compound_statement
    {
        $$ = new SubprogramBodyNode();
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
        if (DEBUG) printf("subprogram_body -> const_declarations type_declarations var_declarations compound_statement.\n");
    };
subprogram_head :
    FUNCTION ID formal_parameter ':' standrad_type ';'
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,$5,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> function id formal_parametert : standrad_type.
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.m_OTHERS, nullptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.m_OTHERS, nullptr, $2.line_num);
        }
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.m_OTHERS, tmp)){
            yyerror(real_ast,"redefinition of function");
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet($2.value.m_OTHERS, table_set_queue.top());
        table_set_queue.push(now_table_set);
        table_set_queue.top()->Insert<FunctionSymbol>($2.value.m_OTHERS, tmp);
        if ($3.parameters){
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }

        $$ = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::FUNCTION);
        LeafNode *leaf_node = new LeafNode($2.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        $$->append_child($5.standard_type_node);
        if(DEBUG) printf("subprogram_head -> function id formal_parameter : standrad_type.\n");
    }
    | PROCEDURE ID formal_parameter ';'
    {
        // TODO插入符号表
        // x = new FunctionSymbol(ID.name,NULL,ID.decline,$3);
        // insert(ID.name,x)
        // subprogram_head -> procedure id formal_parametert.
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.m_OTHERS, nullptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.m_OTHERS, nullptr, $2.line_num);
        }
        
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.m_OTHERS, tmp)){
            yyerror(real_ast,"redefinition of function");
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet($2.value.m_OTHERS,table_set_queue.top());
        table_set_queue.push(now_table_set);
        table_set_queue.top()->Insert<FunctionSymbol>($2.value.m_OTHERS, tmp);
        if ($3.parameters){
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                }
            }
        }
        

        $$ = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode($2.value.m_OTHERS,false);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        if(DEBUG) printf("subprogram_head -> procedure id formal_parameter.\n");
    };
formal_parameter :
    {   
        // formal_parameter -> empty.
        $$.parameters = nullptr;
        $$.formal_parameter_node = new FormalParamNode();

    }
    | '(' parameter_lists ')'
    {
        // formal_parameter -> (parameter_lists).
        $$.parameters = $2.parameters;
        $$.formal_parameter_node = new FormalParamNode();
        $$.formal_parameter_node->append_child($2.param_lists_node);
    };
parameter_lists :
    parameter_lists ';' parameter_list
    {   
        // parameter_lists -> parameter_lists ; parameter_list.
        $$.parameters = $1.parameters;
        $$.parameters->insert($$.parameters->end(), $3.parameters->begin(), $3.parameters->end());

        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_lists_node);
        $$.param_lists_node->append_child($3.param_list_node);
    }
    | parameter_list
    {  
        // parameter_lists -> parameter_list.
        $$.parameters = $1.parameters;

        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_list_node);
        if (DEBUG) printf("parameter_lists -> parameter_list.\n");
    };
parameter_list :
    var_parameter
    {   
        // parameter_list -> var_parameter.
        $$.parameters = $1.parameters;

        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.var_parameter_node);
        if (DEBUG) printf("parameter_list -> var_parameter.\n");
    }
    | value_parameter
    {   
        // parameter_list -> value_parameter.
        $$.parameters = $1.parameters;

        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.value_parameter_node);
        if (DEBUG) printf("parameter_list -> value_parameter.\n");
    };
var_parameter :
    VAR value_parameter
    {   
        // var_parameter -> var value_parameter.
        int para_len = $2.parameters->size();
        for (int i = 0; i < para_len; i++){
            $2.parameters->at(i).second.second = FunctionSymbol::PARAM_MODE::REFERENCE;
        }

        $$.var_parameter_node = new VarParamNode();
        $$.var_parameter_node->append_child($2.value_parameter_node);
        if (DEBUG) printf("var_parameter -> var value_parameter.\n");
    };
value_parameter :
    id_list ':' standrad_type
    {   
        // value_parameter -> id_list : standrad_type.
        $$.parameters = new std::vector<std::pair<std::string, std::pair<BasicType*,FunctionSymbol::PARAM_MODE>>>();
        std::pair<BasicType*,FunctionSymbol::PARAM_MODE> tmp($3.type_ptr,FunctionSymbol::PARAM_MODE::VALUE);
        for (auto i : *($1.list_ref)){
            std::pair<std::string, std::pair<BasicType*,FunctionSymbol::PARAM_MODE>> tmp_pair(i.first,tmp);
            $$.parameters->push_back(tmp_pair);
        }
        

        $$.value_parameter_node = new ValueParamNode();
        $$.value_parameter_node->append_child($1.id_list_node);
        $$.value_parameter_node->append_child($3.standard_type_node);
        if (DEBUG) printf("value_parameter -> id_list : standrad_type.\n");
    };
compound_statement :
    BEGIN_ statement_list END {
        // compound_statement -> begin statement_list end.
        $$ = new CompoundStatementNode();
        $$->append_child($2);
        if (DEBUG) printf("compound_statement -> begin statement_list end.\n");
    };
statement_list :
    statement_list ';' statement
    {
        // statement_list -> statement_list ; statement.
        $$ = new StatementListNode();
        $$->append_child($1);
        $$->append_child($3);
        if (DEBUG) printf("statement_list -> statement_list ; statement.\n");
    } | statement
    {
        // statement_list -> statement.
        $$ = new StatementListNode();
        $$->append_child($1);
    };
statement:
    variable ASSIGNOP expression
    {   // 类型检查 指针为type_ptr
        // statement -> variable assigbop expression.
        std::string func_name = table_set_queue.top()->tag();
        if(func_name == *$1.name){
            $$ = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            $$ = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
        }
        $$->append_child($1.variable_node);
        $$->append_child($3.expression_node);
        if (DEBUG) printf("statement -> variable assigbop expression.\n");
    }
    | call_procedure_statement
    {
        // statement -> call_procedure_statement.
        // TODO check
        $$ = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        $$->append_child($1);
        if (DEBUG) printf("statement -> call_procedure_statement.\n");
    }
    | compound_statement
    {
        // statement -> compound_statement.
        $$ = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        $$->append_child($1);
        if (DEBUG) printf("statement -> compound_statement.\n");
    }
    | IF expression THEN statement else_part
    {   
        //类型检查
        // statement -> if expression then statement else_part.
        $$ = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);
        $$->append_child($5);
        if (DEBUG) printf("statement -> if expression then statement else_part.\n");
    }
    | CASE expression OF case_body END
    {
        //类型检查
        // statement -> case expression of case_body end.
        $$ = new StatementNode(StatementNode::GrammarType::CASE_STATEMET);
        $$->append_child($2.expression_node);
        $$->append_child($4.case_body_node);
    }
    | WHILE expression DO statement
    {
        //类型检查
        // statement -> while expression do if_statement_1.
        $$ = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);
        if (DEBUG) printf("statement -> while expression do statement.\n");

    } 
    | REPEAT statement_list UNTIL expression
    {
        //类型检查
        // statement -> repeat statement_list until expression.
        $$ = new StatementNode(StatementNode::GrammarType::REPEAT_STATEMENT);
        $$->append_child($2);
        $$->append_child($4.expression_node);
        if (DEBUG) printf("statement -> repeat statement_list until expression.\n");
    }
    | FOR ID ASSIGNOP expression updown expression DO statement
    {
        //类型检查
        // statement -> for id assignop expression updown expression do statement.
        $$ = new StatementNode(StatementNode::GrammarType::FOR_STATEMENT);
        LeafNode *id_node = new LeafNode($2.value.m_OTHERS,false);
        $$->append_child(id_node);
        $$->append_child($4.expression_node);
        $$->append_child($5);
        $$->append_child($6.expression_node);
        $$->append_child($8);
        if (DEBUG) printf("statement -> for id assignop expression updown expression do statement.\n");
    } | {
        // statement -> empty.
        $$ = new StatementNode(StatementNode::GrammarType::EPSILON);
        if (DEBUG) printf("statement -> empty.\n");
    };

variable:
    ID id_varparts
    {
        // variable -> id id_varparts.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.m_OTHERS);
        if(tmp == nullptr) {
             $$.type_ptr = nullptr;
            yyerror(real_ast,"variable not defined. this way is not allowed.");
        } else {
            //类型检查
            $$.type_ptr = tmp->type();
            $$.name = new std::string($1.value.m_OTHERS);
        }
        
        $$.variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode($1.value.m_OTHERS,false);
        $$.variable_node->append_child(id_node);
        $$.variable_node->append_child($2.id_varparts_node);
        if (DEBUG) printf("variable -> id id_varparts.\n");
    };

id_varparts:
    {
        // id_varparts -> empty.
        $$.var_parts = nullptr;
        $$.id_varparts_node = new IDVarPartsNode();
        if (DEBUG) printf("id_varparts -> empty.\n");
    }
    | id_varparts id_varpart
    {
        // id_varparts -> id_varparts id_varpart.
        if($$.var_parts){
            $$.var_parts = $1.var_parts;
        } else {
            $$.var_parts = new std::vector<VarParts>();
        }
        
        $$.var_parts->push_back(*($2.var_part));

        $$.id_varparts_node = new IDVarPartsNode();
        $$.id_varparts_node->append_child($1.id_varparts_node);
        $$.id_varparts_node->append_child($2.id_varpart_node);
        if (DEBUG) printf("id_varparts -> id_varparts id_varpart.\n");
    };

id_varpart:
    '[' expression_list ']'
    {   
        // id_varpart -> [expression_list].
        $$.var_part= new VarParts();
        $$.var_part->flag = 0;//数组
        $$.var_part->subscript = $2.type_ptr_list;

        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::EXP_LIST);
        $$.id_varpart_node->append_child($2.expression_list_node);
        if (DEBUG) printf("id_varpart -> [expression_list].\n");
    }
    | '.' ID
    {
        // id_varpart -> .id.
        $$.var_part= new VarParts();
        $$.var_part->flag = 1;//结构体
        strcpy($$.var_part->name,$2.value.m_OTHERS);

        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::_ID);
        LeafNode *id_node = new LeafNode($2.value.m_OTHERS,false);
        $$.id_varpart_node->append_child(id_node);
        if (DEBUG) printf("id_varpart -> .id.\n");
    };
else_part:
    {
        // else_part -> empty.
        $$ = new ElseNode(ElseNode::GrammarType::EPSILON);
        if (DEBUG) printf("else_part -> empty.\n");
    }
    | ELSE statement 
    {
        // else_part -> else statement.
        $$ = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        $$->append_child($2);
        if (DEBUG) printf("else_part -> else statement.\n");
    } ;
case_body:
    {
        // case_body -> empty.
        $$.type_ptr_list = nullptr;
        $$.case_body_node = new CaseBodyNode();
        if (DEBUG) printf("case_body -> empty.\n");
    }
    | branch_list
    {
        // case_body -> branch_list.
        $$.type_ptr_list = $1.type_ptr_list;
        $$.case_body_node = new CaseBodyNode();
        $$.case_body_node->append_child($1.branch_list_node);
        if (DEBUG) printf("case_body -> branch_list.\n");
    };
branch_list:
    branch_list ';' branch
    {
        // branch_list -> branch_list branch.
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        //todo 检查类型是否一致

        $$.branch_list_node = new BranchListNode();
        $$.branch_list_node->append_child($1.branch_list_node);
        $$.branch_list_node->append_child($3.branch_node);
        if (DEBUG) printf("branch_list -> branch_list branch.\n");
    }
    | branch
    {
        // branch_list -> branch.
        $$.type_ptr_list = new std::vector<pascal_type::TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        
        $$.branch_list_node = new BranchListNode();
        $$.branch_list_node->append_child($1.branch_node);
        if (DEBUG) printf("branch_list -> branch.\n");
    };
branch:
    const_list ':' statement
    {
        // branch -> const_list : statement.
        $$.type_ptr = $1.type_ptr;

        $$.branch_node = new BranchNode();
        $$.branch_node->append_child($1.const_list_node);
        $$.branch_node->append_child($3);
        if (DEBUG) printf("branch -> const_list : statement.\n");
    };
const_list:
    const_list ',' const_variable
    {
        // const_list -> const_list , const_variable.
        if($1.type_ptr != $3.type_ptr) {
           yyerror(real_ast,"const_list type not match");
        }
        $$.type_ptr = $1.type_ptr;

        $$.const_list_node = new ConstListNode();
        $$.const_list_node->append_child($1.const_list_node);
        $$.const_list_node->append_child($3.const_variable_node);
        if (DEBUG) printf("const_list -> const_list , const_variable.\n");
    }
    | const_variable
    {
        // const_list -> const_variable.
        $$.type_ptr = $1.type_ptr;

        $$.const_list_node = new ConstListNode();
        $$.const_list_node->append_child($1.const_variable_node);
        if (DEBUG) printf("const_list -> const_variable.\n");
    };
updown:
    TO
    {
        // updown -> to.
        $$ = new UpdownNode(true);
        if (DEBUG) printf("updown -> to.\n");
    }
    | DOWNTO
    {
        // updown -> downto.
        $$ = new UpdownNode(false);
        if (DEBUG) printf("updown -> downto.\n");
    };
call_procedure_statement:
    ID '(' expression_list ')'
    {
        //类型检查
        // call_procedure_statement -> id (expression_list).
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }

        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value.m_OTHERS,false);
        $$->append_child(id_node);
        $$->append_child($3.expression_list_node);
        if (DEBUG) printf("call_procedure_statement -> id (expression_list).\n");
    };
    | ID
    {   //类型检查
        // call_procedure_statement -> id.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }

        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode($1.value.m_OTHERS,false);
        $$->append_child(id_node);
        if (DEBUG) printf("call_procedure_statement -> id.\n");
    };
expression_list:
    expression_list ',' expression
    {
        //类型检查 检查是否为INT or CHAR???
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        // expression_list -> expression_list , expression.

        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        $$.expression_list_node->append_child($1.expression_list_node);
        $$.expression_list_node->append_child($3.expression_node);
        if (DEBUG) printf("expression_list -> expression_list , expression.\n");
    }
    | expression
    {
        //类型检查 检查是否为INT or CHAR  ???  
        $$.type_ptr_list = new std::vector<pascal_type::TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        // expression_list -> expression.

        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        $$.expression_list_node->append_child($1.expression_node);
        if (DEBUG) printf("expression_list -> expression.\n");
    };
expression:
    simple_expression RELOP simple_expression
    {
        // 类型检查
        // 类型强转 为 *tmp
        // 先暂定 tmp = $1.type_ptr
        // expression -> simple_expression relop simple_expression.
        TypeTemplate* tmp = $1.type_ptr;
        $$.type_ptr = tmp;
        
        std::string relop = $2.value.m_OTHERS;
        if(strcmp($2.value.m_OTHERS,"<>") == 0) {
            relop = "!=";
        }

        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(relop,true);
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression relop simple_expression.\n");
    }
    | simple_expression '=' simple_expression
    {
        // 类型检查
        // 类型强转 为 *tmp
        // 先暂定 tmp = $1.type_ptr
        TypeTemplate* tmp = $1.type_ptr;
        $$.type_ptr = tmp;

        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode("==",true);
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression relop simple_expression.\n");
    }
    | simple_expression
    {
        // expression -> simple_expression.
        $$.type_ptr = $1.type_ptr;

        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        if (DEBUG) printf("expression -> simple_expression.\n");
    };
simple_expression:
    term
    {   
        // simple_expression -> term.
        $$.type_ptr = $1.type_ptr;

        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.term_node);
        if (DEBUG) printf("simple_expression -> term.\n");
    }
    |PLUS term
    {
        // simple_expression -> + term.
        $$.type_ptr = $2.type_ptr;

        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *plus_node = new LeafNode("+",true);
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($2.term_node);
        if (DEBUG) printf("simple_expression -> + term.\n");
    }
    |UMINUS term
    {
        // simple_expression -> - term.
        $$.type_ptr = $2.type_ptr;

        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode("-",true);
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($2.term_node);
        if (DEBUG) printf("simple_expression -> - term.\n");
    }
    | simple_expression ADDOP term
    {
        // simple_expression -> simple_expression or term.
        // 类型检查
        // 类型强转 为 *tmp
        TypeTemplate* tmp = $1.type_ptr;
        $$.type_ptr = tmp;


        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *addop_node = new LeafNode("or",true);
        $$.simple_expression_node->append_child(addop_node);
        $$.simple_expression_node->append_child($3.term_node);
        if (DEBUG) printf("simple_expression -> simple_expression or term.\n");
    }
    | simple_expression PLUS term
    { 
        // 类型检查
        // simple_expression -> simple_expression + term.
        TypeTemplate* tmp = $1.type_ptr;
        $$.type_ptr = tmp;

        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *plus_node = new LeafNode("+",true);
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($3.term_node);
        if (DEBUG) printf("simple_expression -> simple_expression + term.\n");
    }
    | simple_expression UMINUS term
    {
        // 类型检查
        // 类型强转 为 *tmp
        // simple_expression -> simple_expression - term.
        TypeTemplate* tmp = $1.type_ptr;
        $$.type_ptr = tmp;

        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *minus_node = new LeafNode("-",true);
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($3.term_node);
        if (DEBUG) printf("simple_expression -> simple_expression - term.\n");
    };
term:
    factor
    {   // term -> factor.
        $$.type_ptr = $1.type_ptr;
        
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.factor_node);
        if (DEBUG) printf("term -> factor.\n");
    }
    | term MULOP factor
    {   
        // term -> term mulop factor.
        // 类型检查
        // 类型强转 为 *tmp
        TypeTemplate *tmp = $1.type_ptr;
        $$.type_ptr = tmp;
        
        std::string mulop = $2.value.m_OTHERS;
        if(mulop == "div"){
            mulop = "/";
        } else if (mulop == "mod"){
            mulop = "%";
        } else if (mulop == "and"){
            mulop = "&&";
        }
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.term_node);
        LeafNode *mulop_node = new LeafNode(mulop,true);
        $$.term_node->append_child(mulop_node);
        $$.term_node->append_child($3.factor_node);
        if (DEBUG) printf("term -> term mulop factor.\n");
    };
factor:
    unsigned_const_variable
    {   // factor -> unsigned_const_variable.
        $$.type_ptr = $1.type_ptr;
        
        $$.factor_node = new FactorNode(FactorNode::GrammarType::UCONST_VAR);
        $$.factor_node->append_child($1.unsigned_constant_var_node);
        if (DEBUG) printf("factor -> unsigned_const_variable.\n");
    }
    | variable
    {   // factor -> variable.
        $$.type_ptr = $1.type_ptr;
        
        $$.factor_node = new FactorNode(FactorNode::GrammarType::VARIABLE);
        $$.factor_node->append_child($1.variable_node);
        if (DEBUG) printf("factor -> variable.\n");
    }
    |ID '(' expression_list ')'
    {
        // factor -> id (expression_list).
        // 类型检查
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.m_OTHERS);
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }
        $$.type_ptr = tmp->type();
        $$.factor_node = new FactorNode(FactorNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value.m_OTHERS,true);
        $$.factor_node->append_child(id_node);
        $$.factor_node->append_child($3.expression_list_node);
        if (DEBUG) printf("factor -> id (expression_list).\n");

    }
    | '(' expression ')'
    {
        // factor -> (expression).
        $$.type_ptr = $2.type_ptr;
        
        $$.factor_node = new FactorNode(FactorNode::GrammarType::EXP);
        $$.factor_node->append_child($2.expression_node);
        if (DEBUG) printf("factor -> (expression).\n");
    }
    | '(' expression {column = buf.size();len = strlen(yytext);}error ';'
    {
        yyerror_("A closing parenthesis is expected.\n");
    }
    | NOT factor
    {   // factor -> not factor.
        // 类型检查
        $$.type_ptr = $2.type_ptr;
        
        $$.factor_node = new FactorNode(FactorNode::GrammarType::NOT);
        $$.factor_node->append_child($2.factor_node);
        if (DEBUG) printf("factor -> not factor.\n");
    };
unsigned_const_variable :
    num
    {
        // unsigned_const_variable -> num
        $$.type_ptr = $1.type_ptr;

        LeafNode *num_node;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        //$$.unsigned_constant_var_node->append_child($1.const_variable_node);
        if($1.type_ptr==pascal_type::TYPE_INT){
            num_node = new LeafNode($1.value.m_INT);
        } else {
            num_node = new LeafNode($1.value.m_REAL);
        }
        $$.unsigned_constant_var_node->append_child(num_node);
        if (DEBUG) printf("unsigned_const_variable -> num\n");
    };
    | CHAR
    {
        // unsigned_const_variable -> 'LETTER'
        $$.type_ptr = pascal_type::TYPE_CHAR;
        
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode($1.value.m_CHAR);
        $$.unsigned_constant_var_node->append_child(char_node);
        if (DEBUG) printf("unsigned_const_variable -> 'LETTER'\n");
    }
    |TRUE
    {
        // unsigned_const_variable -> true
        $$.type_ptr = pascal_type::TYPE_BOOL;
        
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(true);
        $$.unsigned_constant_var_node->append_child(true_node);
        if (DEBUG) printf("unsigned_const_variable -> true\n");
    }
    | FALSE
    {   
        // unsigned_const_variable -> false
        $$.type_ptr = pascal_type::TYPE_BOOL;
        
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(false);
        $$.unsigned_constant_var_node->append_child(false_node);
        if (DEBUG) printf("unsigned_const_variable -> false\n");
    };

%%
 

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
