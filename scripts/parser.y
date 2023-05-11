%{
#include "log.h"
#include"parser.h"
using namespace pascals;
using namespace pascals::ast;
using std::string;
extern "C"			
{					
    void yyerror(const char *s);
    extern int yylex(void);
    extern int line_count;
    extern bool new_line_flag;
    extern int yyleng;
    extern int last_line_count;
}
extern std::string cur_line_info;
extern std::string last_line_info;
extern int lex_error_flag;
int semantic_error_flag = 0;
std::stack<TableSet*> table_set_queue;
int _ = (log_set_level(LOG_INFO), 0);
TableSet* top_table_set = new TableSet("main",nullptr);

int error_flag=0;
char location_pointer[256];
void location_pointer_refresh();

void yyerror(AST* real_ast,const char *msg);
void yyerror_var(AST* real_ast,int line);
void yynote(std::string msg,int line);
void semantic_error(AST* real_ast,std::string msg,int line,int row){
    semantic_error_flag=1;
    real_ast->set_root(nullptr); 
    if (row)
        fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", line,row,msg.c_str());
    else
        fprintf(stderr,"%d:\033[01;31m \terror\033[0m : %s\n", line,msg.c_str());
}


%}

%union
{}
%parse-param {pascals::ast::AST *real_ast}
%start program
%token PROGRAM FUNCTION PROCEDURE TO DOWNTO 
%token ARRAY TYPE CONST RECORD
%token IF THEN ELSE CASE OF WHILE DO FOR REPEAT UNTIL BEGIN_ END
%token ADDOP NOT PLUS UMINUS CONSTASSIGNOP  
%token<token_info> ID CHAR INT_NUM REAL_NUM BASIC_TYPE RELOP MULOP STRING_ VAR SUBCATALOG
%token<token_info> ASSIGNOP WRITE WRITELN SEP READ READLN TRUE FALSE ';'
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
%type<variable_list_node_info> variable_list
%type<expression_node_info> expression
%type<simple_expression_node_info> simple_expression
%type<str_expression_node_info> str_expression
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
        // prgram -> program_head program_body '.'
	ProgramNode* node = new ProgramNode();
        node->append_child($1);
        node->append_child($2);

        if((!error_flag) && (!semantic_error_flag) && (!lex_error_flag)){
            real_ast->set_root(node);
            real_ast->set_valid(true);
        }
        delete top_table_set;
    };
program_head :
    PROGRAM ID '(' id_list ')' ';' {
        // program_head -> PROGRAM ID '(' id_list ')' ';'
    	if(error_flag)
	        break;
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    } | PROGRAM ID '('  ')' ';' {
        // program_head -> PROGRAM ID '('  ')' ';'
	    if(error_flag)
	        break;
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    } | PROGRAM ID ';' {
        // program_head -> PROGRAM ID ';'
        if(error_flag)
            break;
        $$ = new ProgramHeadNode();
        LeafNode* leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
    }
program_body :
    const_declarations type_declarations var_declarations 
    subprogram_declarations compound_statement {
        // program_body -> const_declarations type_declarations var_declarations subprogram_declarations compound_statement
        if(error_flag)
            break;
        $$ = new ProgramBodyNode();
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
        $$->append_child($5);
    };
id_list :
    id_list ',' ID { 
        // id_list -> id_list ',' ID
        $1.list_ref->push_back(std::make_pair($3.value.get<string>(),$3.column_num));
        $$.list_ref = $1.list_ref;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        if(error_flag)
            break;
        LeafNode* leaf_node = new LeafNode($3.value);
        $$.id_list_node->append_child($1.id_list_node);
        $$.id_list_node->append_child(leaf_node);
    } | ID {
        // id_list -> ID
        $$.list_ref = new std::vector<std::pair<std::string,int>>();
        $$.list_ref->push_back(std::make_pair($1.value.get<string>(),$1.column_num));
        if(error_flag)
            break;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode($1.value);
        $$.id_list_node->append_child(leaf_node);
    };
const_declarations :{
        // const_declarations -> empty
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode();
    }
    | CONST const_declaration ';'
    {   
        // const_declarations -> CONST const_declaration ';'
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode(); 
        $$->append_child($2);
    };
const_declaration :
    const_declaration ';' ID '=' const_variable
    {
        // const_declaration -> const_declaration ';' ID '=' const_variable
        if(error_flag)
            break;
        if (!$5.is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol($3.value.get<string>(),$5.value,$3.line_num);

        if(!table_set_queue.top()->Insert<ConstSymbol>($3.value.get<string>(),symbol)){
            string tn = $3.value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",$3.line_num,$3.column_num);
        } else{
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION,$5.type_ptr);
            $$->append_child($1);
            LeafNode* leaf_node = new LeafNode($3.value);
            $$->append_child(leaf_node);
            $$->append_child($5.const_variable_node);
        }
    }
    | ID '=' const_variable
    {   
        // const_declaration -> ID '=' const_variable
        if(error_flag)
            break;
        if (!$3.is_right)
            break;
        ConstSymbol *symbol = new ConstSymbol($1.value.get<string>(),$3.value,$1.line_num);

        if(!table_set_queue.top()->Insert<ConstSymbol>($1.value.get<string>(),symbol)){
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"redefinition of '"+tn+"'",$1.line_num,$1.column_num);
        } else {
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE,$3.type_ptr);
            LeafNode* leaf_node = new LeafNode($1.value);
            $$->append_child(leaf_node);
            $$->append_child($3.const_variable_node);
        }
    };
const_variable :
    PLUS ID
    {   
        // const_variable -> PLUS ID
        if(error_flag)
            break;
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        if(!symbol){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared or is not constant",$2.line_num,$2.column_num);
            $$.is_right = false;
        } else {
            /* You cannot use variables to assign values to constants */
            $$.value = symbol->value();
            $$.type_ptr = symbol->type();
            $$.const_variable_node = new LeafNode(ConstValue("+" + $2.value.get<string>()));     
        }
    }
    | UMINUS ID
    {
        // const_variable -> UMINUS ID
        if(error_flag)
            break;
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        if(!symbol){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared or is not constant",$2.line_num,$2.column_num);
            $$.is_right = false;
        } else {
            /* You cannot use variables to assign values to constants */
            $$.type_ptr = symbol->type();
            $$.value = symbol->value();
            $$.const_variable_node = new LeafNode(ConstValue("-" + $2.value.get<string>()));
        }
    }
    | ID
    {
        // const_variable -> ID
        if(error_flag)
            break;
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($1.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        if(!symbol){
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared or is not constant",$1.line_num,$1.column_num);
            $$.is_right = false;
        } else {
            $$.type_ptr = symbol->type();
            $$.value = symbol->value();
        }
        $$.const_variable_node = new LeafNode($1.value);
    }
    |UMINUS num
    {  
        // const_variable -> UMINUS num
        $$.type_ptr = $2.type_ptr;
        $2.value.set_unimus();
        $$.value = $2.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($2.value);
    }
    | num
    {   
        // const_variable -> num
        $$.type_ptr = $1.type_ptr;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);
    }
    |PLUS num
    {  
        // const_variable -> PLUS num
        $$.type_ptr = $2.type_ptr;
        $$.value = $2.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($2.value);
    }
    | CHAR
    {
        // const_variable -> CHAR
        $$.type_ptr = TYPE_CHAR;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);

    }| TRUE
    {
        $$.type_ptr = TYPE_BOOL;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode(ConstValue(true));
    }| FALSE
    {
        $$.type_ptr = TYPE_BOOL;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode(ConstValue(false));
    };

num :
    INT_NUM
    {
        // num -> INT_NUM
        $$.type_ptr = TYPE_INT;
        $$.value = $1.value;
    }
    | REAL_NUM
    {   
        // num -> REAL_NUM
        $$.type_ptr = TYPE_REAL;
        $$.value = $1.value;
    };
type_declarations : 
    {
        // type_declarations -> empty
        if(error_flag)
            break;
        $$ = new TypeDeclarationsNode();
    }
    | TYPE type_declaration ';'
    {
        // type_declarations -> TYPE type_declaration ';'
        if(error_flag)
            break;
        $$ = new TypeDeclarationsNode();
        $$->append_child($2);
    };
type_declaration :
    type_declaration ';' ID '=' type
    {
        // type_declaration -> type_declaration ';' ID '=' type
        if(error_flag)
            break;
        string tn = $3.value.get<string>();
        bool err1 = $5.main_type == TypeAttr::BASIC && !table_set_queue.top()->Insert<BasicType>(tn,dynamic_cast<BasicType*>($5.type_ptr));
        bool err2 = $5.main_type == TypeAttr::ARRAY && !table_set_queue.top()->Insert<ArrayType>(tn,dynamic_cast<ArrayType*>($5.type_ptr));
        bool err3 = $5.record_info && !table_set_queue.top()->Insert<RecordType>(tn,dynamic_cast<RecordType*>($5.type_ptr));
        if(err1 || err2 || err3){
	    semantic_error(real_ast,"redefinition of '"+tn+"'",$3.line_num,$3.column_num);
	}

        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::MULTIPLE_DECL);
        $$->append_child($1);
        LeafNode *leaf_node = new LeafNode($3.value);
        $$->append_child(leaf_node);
        $$->append_child($5.type_node);
        delete $5.record_info;
        if($5.bounds) {
            delete $5.bounds;
        }
    }
    | ID '=' type
    {
        // type_declaration -> ID '=' type
        if(error_flag)
            break;
        string tn = $1.value.get<string>();
        bool err1 = $3.main_type == TypeAttr::BASIC && !table_set_queue.top()->Insert<BasicType>(tn,dynamic_cast<BasicType*>($3.type_ptr));
        bool err2 = $3.main_type == TypeAttr::ARRAY && !table_set_queue.top()->Insert<ArrayType>(tn,dynamic_cast<ArrayType*>($3.type_ptr));
        bool err3 = $3.record_info && !table_set_queue.top()->Insert<RecordType>(tn,dynamic_cast<RecordType*>($3.type_ptr));
        if(err1 || err2 || err3){
	    semantic_error(real_ast,"redefinition of '"+tn+"'",$1.line_num,$1.column_num);
	}

        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::SINGLE_DECL);
        LeafNode *leaf_node = new LeafNode($1.value);
        $$->append_child(leaf_node);
        $$->append_child($3.type_node);
        delete $3.record_info;
        if($3.bounds) {
            delete $3.bounds;
        }
    };
type :
    standrad_type
    {
        // type -> standrad_type
        $$.main_type = (TypeAttr::MainType)0;
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->set_base_type_node($$.type_node);
        $$.type_node->append_child($1.standard_type_node);
    }
    | ARRAY '[' periods ']' OF type
    {
        // type -> ARRAY '[' periods ']' OF type
        $$.main_type = (TypeAttr::MainType)1;
        $$.base_type_node = $6.base_type_node;
        $$.bounds = $3.bounds;
        if ($3.bounds){
            auto merged_bounds = new std::vector<ArrayType::ArrayBound>();
            for (auto i : *($3.bounds)){
                merged_bounds->push_back(i);
            }
            auto basic_type = $6.type_ptr;
            if($6.type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
                for (auto i : *($6.bounds)){
                    merged_bounds->push_back(i);
                    $$.bounds->push_back(i);
                }
                basic_type = $6.type_ptr->DynamicCast<ArrayType>()->base_type();
            }
            PtrCollect($6.type_ptr);
            $$.type_ptr = new ArrayType(basic_type, *merged_bounds);
            
            delete merged_bounds;
        }
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        $$.type_node->set_base_type_node($6.base_type_node);
        $$.type_node->append_child($3.periods_node);
        $$.type_node->append_child($6.type_node);
        delete $6.bounds;
        if ($6.record_info){
            delete $6.record_info;
        }
    }
    | RECORD record_body END
    {
        // type -> RECORD record_body END
        $$.main_type = (TypeAttr::MainType)2;
        $$.record_info = $2.record_info;
        if ($2.record_info){
            $$.type_ptr = new RecordType(*($2.record_info));
        } else{
             $$.type_ptr = new RecordType();
        }
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->append_child($2.record_body_node);
        $$.type_node->set_base_type_node($$.type_node);
    };
record_body :
    {
        // record_body -> empty
        $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
    } | var_declaration {
	$$.record_info = $1.record_info;
	if(error_flag)
	    break;
	$$.record_body_node = new RecordBodyNode();
	$$.record_body_node->append_child($1.variable_declaration_node);
	delete $1.pos_info;
    } | var_declaration ';'
    {
        // record_body -> var_declaration ';'
        $$.record_info = $1.record_info;
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
        $$.record_body_node->append_child($1.variable_declaration_node);
        delete $1.pos_info;
    };
standrad_type :
    BASIC_TYPE
    {
        // standrad_type -> BASIC_TYPE
        string typestr = $1.value.get<string>();
        if (typestr == "integer"){
            $$.type_ptr = TYPE_INT;
        } else if(typestr == "real"){
            $$.type_ptr = TYPE_REAL;
        } else if(typestr == "boolean"){
            $$.type_ptr = TYPE_BOOL;
        } else{
            $$.type_ptr = TYPE_CHAR;
        }
        if(error_flag)
            break;
        $$.standard_type_node = new BasicTypeNode();
        $$.standard_type_node->set_type(dynamic_cast<BasicType*>($$.type_ptr));
    };
periods :
    periods ',' period
    {
        // periods -> periods ',' period
        $$.bounds = $1.bounds;
        $$.bounds->push_back(*($3.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.periods_node);
        $$.periods_node->append_child($3.period_node);
        delete $3.bound;
    }
    | period
    {
        // periods -> period
        $$.bounds = new std::vector<ArrayType::ArrayBound>();
        $$.bounds->push_back(*($1.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.period_node);
        delete $1.bound;
    };
period :
    const_variable SUBCATALOG const_variable
    {     
        // period -> const_variable SUBCATALOG const_variable
        int arr_len=0;
        $$.bound = new ArrayType::ArrayBound();
        if ($1.type_ptr == TYPE_INT&&$3.type_ptr == TYPE_INT){
            arr_len = ($3.value - $1.value).get<int>();
            $$.bound-> type_ = TYPE_INT;
            $$.bound->lb_ = $1.value.get<int>();
            $$.bound->ub_ = $3.value.get<int>();
        } else if($1.type_ptr == TYPE_CHAR&&$3.type_ptr == TYPE_CHAR){
            arr_len = (int)($3.value - $1.value).get<char>();
            $$.bound-> type_ = TYPE_CHAR;
            $$.bound->lb_ = int($1.value.get<int>());
            $$.bound->ub_ = int($3.value.get<int>());
        } else {
            semantic_error(real_ast,"array bound should be integer or char",$2.line_num,$2.column_num);
        }
        if(arr_len < 0){
            arr_len = 0;
            semantic_error(real_ast,"array bound should be positive",$2.line_num,$2.column_num);
        }
        if(error_flag){
            break;
        }
        $$.period_node =new PeriodNode();
        $$.period_node->set_len(arr_len+1);
        $$.period_node->append_child($1.const_variable_node);
        $$.period_node->append_child($3.const_variable_node);
    };
var_declarations : 
    {
        // var_declarations -> empty
        if(error_flag)
            break;
        $$ = new VariableDeclarationsNode();
    }
    | VAR var_declaration ';'
    {
        // var_declarations -> VAR var_declaration ';'
        if(error_flag)
            break;
        for (auto i : *($2.record_info)){
            int line = $2.pos_info->find(i.first)->second.first;
            int row = $2.pos_info->find(i.first)->second.second;
            ObjectSymbol *obj = new ObjectSymbol(i.first, i.second,line);
            if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first,obj)){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line,row);
                yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
            }
        }
        $$ = new VariableDeclarationsNode();
        $$->append_child($2.variable_declaration_node);
        delete $2.pos_info;
        if ($2.record_info){
            delete $2.record_info;
        }
    };
var_declaration :
    var_declaration ';' id_list ':' type 
    {
        // var_declaration -> var_declaration ';' id_list ':' type
         if(error_flag)
            break;   
        $$.record_info = $1.record_info;
        $$.pos_info = $1.pos_info;
        for (auto i : *($3.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $5.type_ptr));
            $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        $$.variable_declaration_node->append_child($5.type_node);
        delete $3.list_ref;
        if($5.bounds) {
            delete $5.bounds;
        }
        if($5.record_info) {
            delete $5.record_info;
        }
        PtrCollect($5.type_ptr);

    }
    | id_list ':' type 
    {
        // var_declaration -> id_list ':' type
        if(error_flag)
           break;
        $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
        $$.pos_info = new std::unordered_map<std::string, std::pair<int,int>>();
        for (auto i : *($1.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $3.type_ptr));
            $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
            if (!res.second){
                semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
            }
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.id_list_node);
        $$.variable_declaration_node->append_child($3.type_node);
        delete $1.list_ref;
        if($3.bounds) {
            delete $3.bounds;
        }
        if($3.record_info) {
            delete $3.record_info;
        }
        PtrCollect($3.type_ptr);
    }
    |var_declaration ';' id_list ':' ID
    {
        // var_declaration -> var_declaration ';' id_list ':' ID
        if(error_flag)
            break;
        $$.record_info = $1.record_info;
        $$.pos_info = $1.pos_info;
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($5.value.get<string>());
        if(tmp == nullptr){
            string tn = $5.value.get<string>();
            semantic_error(real_ast,"undefined type '"+tn+"'",$5.line_num,$5.column_num);
            break;
        } else {
            for (auto i : *($3.list_ref)){
                auto res = $$.record_info->insert(make_pair(i.first, tmp));
                $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
                if (!res.second){
                    semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
                }
            }
        }
        if(error_flag)
            break;
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        LeafNode *leaf_node = new LeafNode($5.value);
        $$.variable_declaration_node->append_child(leaf_node);
        delete $3.list_ref;
    }
    |id_list ':' ID
    {
        // var_declaration -> id_list ':' ID
        if(error_flag)
                break;
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($3.value.get<string>());
        if(tmp==nullptr){
            string tn = $3.value.get<string>();
            semantic_error(real_ast,"undefined type '"+tn+"'",$3.line_num,$3.column_num);
            $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
            $$.pos_info = new std::unordered_map<std::string, std::pair<int,int>>();
            break;
        } else {
            $$.record_info = new std::unordered_map<std::string, TypeTemplate*>();
            $$.pos_info = new std::unordered_map<std::string, std::pair<int,int>>();
            for (auto i : *($1.list_ref)){
                auto res = $$.record_info->insert(make_pair(i.first, tmp));
                $$.pos_info->insert(make_pair(i.first,std::make_pair(line_count,i.second)));
                if (!res.second){
                    semantic_error(real_ast,"redefinition of '"+ i.first +"'",line_count,i.second);
                }
            }
        }
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.id_list_node);
        LeafNode *leaf_node = new LeafNode($3.value);
        $$.variable_declaration_node->append_child(leaf_node);
        delete $1.list_ref;
    };
subprogram_declarations : 
    {
        // subprogram_declarations -> empty
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationsNode();
    }
    | subprogram_declarations subprogram_declaration ';'
    {
        // subprogram_declarations -> subprogram_declarations subprogram_declaration ';'
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationsNode();
        $$->append_child($1);
        $$->append_child($2);
        TableSet* top = table_set_queue.top();
        table_set_queue.pop();
        delete top;
    };
subprogram_declaration :
    subprogram_head subprogram_body
    {
        // subprogram_declaration -> subprogram_head subprogram_body
        if(error_flag)
            break;
        $$ = new SubprogramDeclarationNode();
        $$->append_child($1);
        $$->append_child($2);
    };
subprogram_body :
    const_declarations type_declarations var_declarations compound_statement
    {
        // subprogram_body -> const_declarations type_declarations var_declarations compound_statement
        if(error_flag)
            break;
        $$ = new SubprogramBodyNode();
        $$->append_child($1);
        $$->append_child($2);
        $$->append_child($3);
        $$->append_child($4);
    };
subprogram_head :
    FUNCTION ID formal_parameter ':' standrad_type ';'
    {
        // subprogram_head -> FUNCTION ID formal_parameter ':' standrad_type ';'
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num);
        }
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp)){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"redefinition of function '"+tn+"'",$2.line_num,$2.column_num);
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet($2.value.get<string>(), table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        string tag = $2.value.get<string>();
        table_set_queue.top()->Insert<FunctionSymbol>(tag, tmp2);
        ObjectSymbol* tmp3 = new ObjectSymbol("__"+tag+"__", $5.type_ptr, $2.line_num);
        table_set_queue.top()->Insert<ObjectSymbol>("__"+tag+"__", tmp3);
        if ($3.parameters){
            int cnt = 0;
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if (i.second.second == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    int line = $3.pos_info->at(cnt).first;
                    int row = $3.pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.first +"'",line,row);
                    yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
                }
                cnt++;
            }
        }
        if(error_flag)
            break;
        $$ = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::FUNCTION);
        $$->set_id($2.value.get<string>());
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        $$->append_child($5.standard_type_node);
        if($3.parameters){
            delete $3.parameters;
        }
        if($3.pos_info){
            delete $3.pos_info;
        }
    }
    | PROCEDURE ID formal_parameter ';'
    {
        // subprogram_head -> PROCEDURE ID formal_parameter ';'
        if(error_flag)
            break;
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num);
        }
        
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp)){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"redefinition of procedure '"+tn+"'",$2.line_num,$2.column_num);
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        TableSet* now_table_set = new TableSet($2.value.get<string>(),table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        FunctionSymbol* tmp2 = new FunctionSymbol(*tmp);
        table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp2);
        if ($3.parameters){
            int cnt = 0;
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if (i.second.second == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    int line = $3.pos_info->at(cnt).first;
                    int row = $3.pos_info->at(cnt).second;
                    semantic_error(real_ast,"redefinition of '"+ i.first +"'",line,row);
                    yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
                }
                cnt++;
            }
        }
        
        $$ = new SubprogramHeadNode(SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        delete $3.parameters;
        delete $3.pos_info;
    };
formal_parameter :
    {   
        // formal_parameter -> empty
        $$.parameters = new std::vector<FunctionSymbol::Parameter>();
        $$.pos_info = new std::vector<std::pair<int,int>>();
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();
    }
    | '(' parameter_lists ')'
    {
        // formal_parameter -> '(' parameter_lists ')'
        $$.parameters = $2.parameters;
        $$.pos_info = $2.pos_info;
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();
        $$.formal_parameter_node->append_child($2.param_lists_node);
    };
parameter_lists :
    parameter_lists ';' parameter_list
    {   
        // parameter_lists -> parameter_lists ';' parameter_list
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        $$.parameters->insert($$.parameters->end(), $3.parameters->begin(), $3.parameters->end());
        $$.pos_info->insert($$.pos_info->end(),$3.pos_info->begin(), $3.pos_info->end());
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_lists_node);
        $$.param_lists_node->append_child($3.param_list_node);
    }
    | parameter_list
    {  
        // parameter_lists -> parameter_list
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_list_node);
    };
parameter_list :
    var_parameter
    {   
        // parameter_list -> var_parameter
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.var_parameter_node);
    }
    | value_parameter
    {   
        // parameter_list -> value_parameter
        $$.parameters = $1.parameters;
        $$.pos_info = $1.pos_info;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.value_parameter_node);
    };
var_parameter :
    VAR value_parameter
    {   
        // var_parameter -> VAR value_parameter
        int para_len = $2.parameters->size();
        for (int i = 0; i < para_len; i++){
            $2.parameters->at(i).second.second = FunctionSymbol::PARAM_MODE::REFERENCE;
        }
        $$.parameters = $2.parameters;
        $$.pos_info = $2.pos_info;
        if(error_flag)
            break;
        $$.var_parameter_node = new VarParamNode();
        $$.var_parameter_node->append_child($2.value_parameter_node);
    };
value_parameter :
    id_list ':' standrad_type
    {   
        // value_parameter -> id_list ':' standrad_type
        $$.parameters = new std::vector<FunctionSymbol::Parameter>();
        $$.pos_info = new std::vector<std::pair<int,int>>();
        FunctionSymbol::ParamType tmp($3.type_ptr,FunctionSymbol::PARAM_MODE::VALUE);
        for (auto i : *($1.list_ref)){
            FunctionSymbol::Parameter tmp_pair(i.first,tmp);
            $$.parameters->push_back(tmp_pair);
            $$.pos_info->push_back(std::make_pair(line_count,i.second));
        }
        
        if(error_flag)
            break;
        $$.value_parameter_node = new ValueParamNode();
        $$.value_parameter_node->append_child($1.id_list_node);
        $$.value_parameter_node->append_child($3.standard_type_node);
        delete $1.list_ref;
    };
compound_statement :
    BEGIN_ statement_list END 
    {
        // compound_statement -> BEGIN_ statement_list END
        if(error_flag)
            break;
        $$ = new CompoundStatementNode();
        $$->append_child($2);
    };
statement_list :
    statement_list ';' statement
    {
        // statement_list -> statement_list ';' statement
        if(error_flag)
            break;
        $$ = new StatementListNode();
        $$->append_child($1);
        $$->append_child($3);
    } | statement
    {
        // statement_list -> statement
        if(error_flag)
            break;
        $$ = new StatementListNode();
        $$->append_child($1);
    };
statement:
    variable ASSIGNOP expression
    {   
        //statement -> variable ASSIGNOP expression
        if(error_flag)
            break;
        bool var_flag = ($1.type_ptr==TYPE_REAL && $3.type_ptr==TYPE_INT) || is_same($1.type_ptr,$3.type_ptr);
        bool str_flag = ($1.type_ptr != TYPE_ERROR &&
        		 $1.type_ptr->StringLike() &&
        		 $3.type_ptr==TYPE_STRINGLIKE);
        if(!var_flag && !str_flag){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
            break;
        }
        std::string func_name = table_set_queue.top()->tag();

        if(func_name == *$1.name){
            $$ = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            $$ = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
            if (!$1.is_lvalue){
                semantic_error(real_ast,"lvalue required as left operand of assignment",$2.line_num,$2.column_num);
            }
        }
        if(error_flag)
            break;
        $$->append_child($1.variable_node);
        $$->append_child($3.expression_node);
        delete $1.name;
    }
    | call_procedure_statement
    {
        // statement -> call_procedure_statement
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        $$->append_child($1);
    }
    | compound_statement
    {
        // statement -> compound_statement
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        $$->append_child($1);
    }
    | IF expression THEN statement else_part
    {   
        // statement -> IF expression THEN statement else_part
        if(error_flag)
            break;
        //类型检查
        if(!is_same($2.type_ptr,TYPE_BOOL)){
            string tn = type_name($2.type_ptr);
            semantic_error(real_ast,"IF quantity cannot be '"+tn+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);
        $$->append_child($5);
    }
    | CASE expression OF case_body END
    {
        // statement -> CASE expression OF case_body END
        if(error_flag)
            break;
        //类型检查
        bool valid = $2.type_ptr == TYPE_INT || $2.type_ptr == TYPE_BOOL || $2.type_ptr == TYPE_CHAR;
        if(!valid){
            string tn = type_name($2.type_ptr);
            semantic_error(real_ast,"CASE quantity cannot be '"+tn+"'",line_count,0);
        }
        if(!is_same($4.type_ptr,TYPE_ERROR)){
            if(!is_same($2.type_ptr,$4.type_ptr)){
            	string tn = type_name($2.type_ptr);
            	string tn2 = type_name($4.type_ptr);
                semantic_error(real_ast,"CASE label cannot reduce from '"+tn2+"' to '"+tn+"'",line_count,0);
            }
        }
        $$ = new StatementNode(StatementNode::GrammarType::CASE_STATEMET);
        $$->append_child($2.expression_node);
        $$->append_child($4.case_body_node);
    }
    | WHILE expression DO statement
    {
        // statement -> WHILE expression DO statement
        if(error_flag)
            break;
        if(!is_same($2.type_ptr,TYPE_BOOL)){
            string tn = type_name($2.type_ptr);
            semantic_error(real_ast,"WHILE quantity cannot be '"+tn+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);

    } 
    | REPEAT statement_list UNTIL expression
    {
        // statement -> REPEAT statement_list UNTIL expression
        if(error_flag)
            break;
        //类型检查
        if(!is_same($4.type_ptr,TYPE_BOOL)){
            string tn = type_name($4.type_ptr);
            semantic_error(real_ast,"REPEAT quantity cannot be '"+tn+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::REPEAT_STATEMENT);
        $$->append_child($2);
        $$->append_child($4.expression_node);
    }
    | FOR ID ASSIGNOP expression updown expression DO statement
    {
        // statement -> FOR ID ASSIGNOP expression updown expression DO statement
        if(error_flag)
            break;
        //类型检查
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($2.value.get<string>());
        if(tmp==nullptr){
            string tn = $2.value.get<string>();
            semantic_error(real_ast,"'"+tn+"' undeclared",$2.line_num,$2.column_num);
            break;
        }
        if((!is_basic(tmp->type()))||(!is_same(tmp->type(),$4.type_ptr))){
            string tn1 = type_name(tmp->type());
            string tn2 = type_name($4.type_ptr);
            semantic_error(real_ast,"incompatible type assigning '"+tn1+"' from '"+tn2+"'",line_count,0);
        }

        if((!is_same($4.type_ptr,$6.type_ptr))||(is_same($4.type_ptr,TYPE_REAL))){
            string tn1 = type_name($4.type_ptr);
            string tn2 = type_name($6.type_ptr);
            semantic_error(real_ast,"invalid updown type from '"+tn1+"' to '"+tn2+"'",line_count,0);
        }
        $$ = new StatementNode(StatementNode::GrammarType::FOR_STATEMENT);
        LeafNode *id_node = new LeafNode($2.value);
        $$->append_child(id_node);
        $$->append_child($4.expression_node);
        $$->append_child($5);
        $$->append_child($6.expression_node);
        $$->append_child($8);
    }
    | 
    {
        // statement -> empty
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::EPSILON);
    }
    |READ '(' variable_list ')'
    {
        // statement -> READ '(' variable_list ')'
        if(error_flag)
            break;
        if(!$3.variable_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in READ",$1.line_num,$1.column_num);
        }  
        $$ = new StatementNode(StatementNode::GrammarType::READ_STATEMENT);
        $$->append_child($3.variable_list_node);
        delete $3.type_ptr_list;
    }
    |READLN '(' variable_list ')'
    {
        // statement -> READLN '(' variable_list ')'
        if(error_flag)
            break;
        if(!$3.variable_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in READLN",$1.line_num,$1.column_num);
        }
        $$ = new StatementNode(StatementNode::GrammarType::READLN_STATEMENT);
        $$->append_child($3.variable_list_node);
        delete $3.type_ptr_list;
    }
    |READLN '('  ')'
    {
        // statement -> READLN '('  ')'
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::READLN_STATEMENT);
    }
    |READLN
    {
        // statement -> READLN
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::READLN_STATEMENT);
    }
    |WRITE '(' expression_list ')'
    {
        // statement -> WRITE '(' expression_list ')'
        if(error_flag)
            break;
        if(!$3.expression_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in WRITE",$1.line_num,$1.column_num);
        }
        
        $$ = new StatementNode(StatementNode::GrammarType::WRITE_STATEMENT);
        $$->append_child($3.expression_list_node);
        delete $3.type_ptr_list;
        delete $3.is_lvalue_list;
    }
    |WRITELN'(' expression_list ')'
    {
        // statement -> WRITELN'(' expression_list ')'
        if(error_flag)
            break;
        if(!$3.expression_list_node->set_types($3.type_ptr_list)){
            semantic_error(real_ast,"basic type is expected in WRITELN",$1.line_num,$1.column_num);
        }
        $$ = new StatementNode(StatementNode::GrammarType::WRITELN_STATEMENT);
        $$->append_child($3.expression_list_node);
        delete $3.type_ptr_list;
        delete $3.is_lvalue_list;
    };
    | WRITELN '(' ')'
    {
        // statement -> WRITELN '(' ')'
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::WRITELN_STATEMENT);
    }
    | WRITELN
    {
        // statement -> WRITELN
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::WRITELN_STATEMENT);
    };


variable_list :
    variable
    { 
        // variable_list -> variable
        $$.type_ptr_list = new std::vector<TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE);
        $$.variable_list_node->append_child($1.variable_node);
        if($1.name) delete $1.name;
    } | variable_list ',' variable{
        // variable_list -> variable_list ',' variable
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE_LIST_VARIABLE);
        $$.variable_list_node->append_child($1.variable_list_node);
        $$.variable_list_node->append_child($3.variable_node);
        if($3.name) delete $3.name;
    };
variable:
    ID '('')'
    {
        // variable -> ID '('')'
        if (error_flag) break;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        $$.is_lvalue = false;
        if(tmp == nullptr){
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",$1.line_num,$1.column_num);
            break;
        }
        if(tmp->type() != nullptr && tmp->symbol_type() == ObjectSymbol::SYMBOL_TYPE::FUNCTION){
            if(!tmp->AssertParams()){
                string tn = $1.value.get<string>();
                string param = tmp->ParamName();
	        semantic_error(real_ast,"too few arguments to function '"+tn+"' (expected '("+param+")')",$1.line_num,$1.column_num);
                break;
            }
            $$.type_ptr = tmp->type();
            $$.name = new std::string($1.value.get<string>());
            real_ast->libs()->Call(*($$.name));
            string name = $1.value.get<string>()+"()";
            $$.variable_node = new VariableNode();
            LeafNode *id_node = new LeafNode(name);
            $$.variable_node->append_child(id_node);
        } else {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined function '"+tn+"'",$1.line_num,$1.column_num);
        }
         
    }
    | ID id_varparts
    {
        // variable -> ID id_varparts
        if(error_flag)
            break;
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.get<string>());
        string name = $1.value.get<string>();
        $$.name = new std::string($1.value.get<string>());
        $$.type_ptr = TYPE_ERROR;
        if(tmp == nullptr) {
            semantic_error(real_ast,"'"+name+ "' undeclared",$1.line_num,$1.column_num);
            break;
        } else {
            //类型检查
            $$.is_lvalue = true;
            if (ObjectSymbol::SYMBOL_TYPE::CONST == tmp->symbol_type()){
                tmp = dynamic_cast<ConstSymbol*>(tmp);
                $$.is_lvalue = false;
            } else if(ObjectSymbol::SYMBOL_TYPE::FUNCTION == tmp->symbol_type()){
                //函数调用 类型检查
                if (name!=table_set_queue.top()->tag()){
                    if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertParams()){
                        string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
                        semantic_error(real_ast,"too few arguments to function '"+name+"' (expected '("+param+")')",line_count,0);
                    }else{
                    	name += "()";
                    }
                }else{
                    name="__"+name+"__";
                }
                $$.is_lvalue = false;
                real_ast->libs()->Call(tmp->name());
            } else {
                if (tmp->type()->template_type() == TypeTemplate::TYPE::ARRAY && !error_flag){
                    std::vector<ArrayType::ArrayBound> bounds = dynamic_cast<ArrayType*>(tmp->type())->bounds();
                    $2.id_varparts_node->set_lb(bounds);
                }
            }
            $$.type_ptr = $2.AccessCheck(tmp->type());
            if($$.type_ptr==nullptr){
                string tn1 = type_name(tmp->type());
                if(tmp->symbol_type() == ObjectSymbol::SYMBOL_TYPE::FUNCTION ||
                   tmp->symbol_type() == ObjectSymbol::SYMBOL_TYPE::CONST ||
                   tmp->type() == TYPE_ERROR ||
                   tmp->type()->template_type() == TypeTemplate::TYPE::BASIC){
                    if($2.var_parts != nullptr && $2.var_parts->size() != 0){
                    	semantic_error(real_ast,"requested value is nither array nor record (have '"+tn1+"')",line_count,0);
                    }
                }else{
                    string tn2;
                    std::vector<VarParts>* vp = $2.var_parts;
                    if(vp == nullptr) tn2 += "error";
                    else{
                        for(int i=0;i<vp->size();i++){
                            if((*vp)[i].flag == 0){
                                tn2 += "[" + type_name(*((*vp)[i].subscript)) + "]";
                            }else{
                            	tn2 += (*vp)[i].name;
                            }
			    if(i != vp->size() - 1) tn2 += ",";
		        }
                    }
                    semantic_error(real_ast,"invalid request from '"+tn1+"' with '"+tn2+"'",line_count,0);
                }
            }
            if(tmp->is_ref()){
                name = "*("+name+")";
            }
        }

        $$.variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode(name);
        $$.variable_node->append_child(id_node);
        $$.variable_node->append_child($2.id_varparts_node);
        for (auto i : *($2.var_parts)){
            delete i.subscript;
        }
        delete $2.var_parts;
    };

id_varparts:
    {
        // id_varparts -> empty.
        if(error_flag)
            break;
        $$.var_parts = new std::vector<VarParts>();
        if(error_flag)
            break;
        $$.id_varparts_node = new IDVarPartsNode();
    }
    | id_varparts id_varpart
    {
        // id_varparts -> id_varparts id_varpart.
        if(error_flag)
            break;
        if($1.var_parts){
            $$.var_parts = $1.var_parts;
        } else {
            $$.var_parts = new std::vector<VarParts>();
        }
        
        $$.var_parts->push_back(*($2.var_part));

        $$.id_varparts_node = new IDVarPartsNode();
        $$.id_varparts_node->append_child($1.id_varparts_node);
        $$.id_varparts_node->append_child($2.id_varpart_node);
        delete $2.var_part;
    };

id_varpart:
    '[' expression_list ']'
    {   
        // id_varpart -> [expression_list].
        $$.var_part= new VarParts();
        $$.var_part->flag = 0;//数组
        $$.var_part->subscript = $2.type_ptr_list;
        if(error_flag)
            break;
        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::EXP_LIST);
        $$.id_varpart_node->append_child($2.expression_list_node);
        delete $2.is_lvalue_list;
    }
    | '.' ID
    {
        // id_varpart -> .id.
        $$.var_part= new VarParts();
        $$.var_part->flag = 1;//结构体
        $$.var_part->name = $2.value.get<string>();
        if(error_flag)
            break;
        $$.id_varpart_node = new IDVarPartNode(IDVarPartNode::GrammarType::_ID);
        LeafNode *id_node = new LeafNode($2.value);
        $$.id_varpart_node->append_child(id_node);
    };
else_part:
    {
        // else_part -> empty.
        if(error_flag)
            break;
        $$ = new ElseNode(ElseNode::GrammarType::EPSILON);
    }
    | ELSE statement
    {
        // else_part -> ELSE statement.
        if(error_flag)
            break;
        $$ = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        $$->append_child($2);
    } ;
case_body:
    {
        // case_body -> empty.
        $$.type_ptr= TYPE_ERROR;
        if(error_flag)
            break;
        $$.case_body_node = new CaseBodyNode();
    }
    | branch_list
    {
        // case_body -> branch_list.
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.case_body_node = new CaseBodyNode();
        $$.case_body_node->append_child($1.branch_list_node);
    };
branch_list:
    branch_list ';' branch
    {
        // branch_list -> branch_list branch.
        if(error_flag)
            break;        
        if($1.type_ptr!=$3.type_ptr){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"branch type conflict between '"+tn1+"' and '"+tn2+"'",line_count,0);
        }
        $$.type_ptr = $1.type_ptr;

        $$.branch_list_node = new BranchListNode();
        $$.branch_list_node->append_child($1.branch_list_node);
        $$.branch_list_node->append_child($3.branch_node);
    }
    | branch
    {
        // branch_list -> branch.
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.branch_list_node = new BranchListNode();
        $$.branch_list_node->append_child($1.branch_node);
    };
branch:
    const_list ':' statement
    {
        // branch -> const_list : statement.
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.branch_node = new BranchNode();
        $$.branch_node->append_child($1.const_list_node);
        $$.branch_node->append_child($3);
    };
const_list:
    const_list ',' const_variable
    {
        // const_list -> const_list , const_variable.
        if($1.type_ptr != $3.type_ptr) {
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"constlist type conflict between '"+tn1+"' and '"+tn2+"'",line_count,0);
        }
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.const_list_node = new ConstListNode();
        $$.const_list_node->append_child($1.const_list_node);
        $$.const_list_node->append_child($3.const_variable_node);
    }
    | const_variable
    {
        // const_list -> const_variable.
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        $$.const_list_node = new ConstListNode();
        $$.const_list_node->append_child($1.const_variable_node);
    };
updown:
    TO
    {
        // updown -> TO.
        if(error_flag)
            break;
        $$ = new UpdownNode(true);
    }
    | DOWNTO
    {
        // updown -> DOWNTO.
        if(error_flag)
            break;
        $$ = new UpdownNode(false);
    };
call_procedure_statement:
    ID '(' expression_list ')'
    {
        // call_procedure_statement -> id (expression_list).
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        }
        if(!tmp || !tmp->AssertParams(*($3.type_ptr_list),*($3.is_lvalue_list))){
            string tn = $1.value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*($3.type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        $$->append_child($3.expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        $3.expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete $3.is_lvalue_list;
        delete $3.type_ptr_list;
    };
    | ID
    {   
        // call_procedure_statement -> id.
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        } else {
            //函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertParams()){
                string tn = $1.value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too few arguments to procedure '"+tn+"' (expected '("+param+")')",line_count,0);
            }
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    };
    | ID '(' ')'
    {
        // call_procedure_statement -> id().
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            string tn = $1.value.get<string>();
            semantic_error(real_ast,"undefined procedure '"+tn+"'",$1.line_num,$1.column_num);
            break;
        } else {
            //函数调用 类型检查
            if(!dynamic_cast<FunctionSymbol*>(tmp)->AssertParams()){
                string tn = $1.value.get<string>();
                string param = dynamic_cast<FunctionSymbol*>(tmp)->ParamName();
	        semantic_error(real_ast,"too many arguments to procedure '"+tn+"' (expected '("+param+")')",$1.line_num,0);
            }
        }
        if(error_flag)
            break;
        $$ = new ProcedureCallNode(ProcedureCallNode::GrammarType::ID);
        LeafNode *id_node = new LeafNode($1.value);
        $$->append_child(id_node);
        real_ast->libs()->Call(tmp->name());
    };
expression_list:
    expression_list ',' expression
    {
        // expression_list -> expression_list ',' expression
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        $$.is_lvalue_list = $1.is_lvalue_list;
        $$.is_lvalue_list->push_back($3.is_lvalue);
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        $$.expression_list_node->append_child($1.expression_list_node);
        $$.expression_list_node->append_child($3.expression_node);
    }
    | expression
    {
        // expression_list -> expression
        $$.type_ptr_list = new std::vector<TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        $$.is_lvalue_list = new std::vector<bool>();
        $$.is_lvalue_list->push_back($1.is_lvalue);
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        $$.expression_list_node->append_child($1.expression_node);
    };
expression:
    simple_expression RELOP simple_expression
    {
        // expression -> simple_expression RELOP simple_expression.
        if(error_flag)
            break;
        // 类型检查
        //从这里开始进行运算检查
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr, $2.value.get<string>());
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
	    string tn2 = type_name($3.type_ptr);
	    string tn3 = $2.value.get<string>();
	    semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        
        std::string relop = $2.value.get<string>();
        if($2.value.get<string>() == "<>") {
            relop = "!=";
        }
        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue(relop));
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
    }
    | simple_expression '=' simple_expression
    {
        // expression -> simple_expression '=' simple_expression.
        // 类型检查
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr, "=");
        
        if(result==TYPE_ERROR){
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary = (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue("=="));
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
    }
    | simple_expression
    {
        // expression -> simple_expression.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        if($$.type_ptr && $$.type_ptr->template_type() == TypeTemplate::TYPE::ARRAY) {
            $$.expression_node = new ExpressionNode(ExpressionNode::TargetType::VAR_ARRAY);
        } else {
            $$.expression_node = new ExpressionNode();
        }
        
        $$.expression_node->append_child($1.simple_expression_node);
    }| str_expression
    {
        // expression -> str_expression.
        $$.type_ptr = $1.type_ptr;
        $$.length = $1.length;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.expression_node = new ExpressionNode(ExpressionNode::TargetType::CONST_STRING);
        $$.expression_node->append_child($1.str_expression_node);
    };

str_expression :
    STRING_ {
        // str_expression -> string.
        if(error_flag)
            break;
        $$.type_ptr = TYPE_STRINGLIKE;
        $$.length = $1.value.get<string>().length();
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.str_expression_node = new StrExpressionNode();
        LeafNode *string_node = new LeafNode($1.value);
        $$.str_expression_node->append_child(string_node);
    } | str_expression PLUS STRING_ {
        // str_expression -> str_expression + string.
        $$.type_ptr = TYPE_STRINGLIKE;
        $$.length = $1.length + $3.value.get<string>().length();
        $$.is_lvalue = false;
        if(error_flag)
	    break;
        $$.str_expression_node = new StrExpressionNode();
        $$.str_expression_node->append_child($1.str_expression_node);
        LeafNode *string_node = new LeafNode($3.value);
        $$.str_expression_node->append_child(string_node);
    } | str_expression PLUS CHAR  {
        // str_expression -> str_expression + char.
        $$.type_ptr = TYPE_STRINGLIKE;
        $$.length = $1.length + 1;
        char c = $3.value.get<char>();
        $3.value.set(std::string(1, c));
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.str_expression_node = new StrExpressionNode();
        $$.str_expression_node->append_child($1.str_expression_node);
        LeafNode *string_node = new LeafNode($3.value);
        $$.str_expression_node->append_child(string_node);
    }
simple_expression:
    term
    {   
        // simple_expression -> term.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.term_node);
    }
    |PLUS term
    {
        // simple_expression -> + term.
        //类型检查
        if(!is_basic($2.type_ptr)){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }

        auto result=compute((BasicType*)$2.type_ptr, "+");
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary plus",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($2.term_node);
    }
    |UMINUS term
    {
        // simple_expression -> - term.
        //类型检查
        if(!is_basic($2.type_ptr)){
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        auto result=compute((BasicType*)$2.type_ptr, "-");
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary minus",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($2.term_node);
    }
    | simple_expression ADDOP term
    {
        // simple_expression -> simple_expression or term.、
        //类型检查
        if($1.type_ptr!=$3.type_ptr){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary or (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = $1.type_ptr;

        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *addop_node = new LeafNode(ConstValue("||"));
        $$.simple_expression_node->append_child(addop_node);
        $$.simple_expression_node->append_child($3.term_node);
    }
    | simple_expression PLUS term
    { 
        // 类型检查
        // simple_expression -> simple_expression + term.
        $$.is_lvalue = false;
        if(error_flag)
            break;
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
           string tn1 = type_name($1.type_ptr);
           string tn2 = type_name($3.type_ptr);
           semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
           $$.type_ptr = $1.type_ptr;
        }
        else{
            auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,"+");
            if(result==TYPE_ERROR){
                 string tn1 = type_name($1.type_ptr);
                 string tn2 = type_name($3.type_ptr);
                 semantic_error(real_ast,"invalid operands to binary + (have '"+tn1+"' and '"+tn2+"')",line_count,0);
            }
            $$.type_ptr = result;
        }
        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *plus_node = new LeafNode(ConstValue("+"));
        $$.simple_expression_node->append_child(plus_node);
        $$.simple_expression_node->append_child($3.term_node);
    }
    | simple_expression UMINUS term
    {
        $$.is_lvalue = false;
        if(error_flag)
            break;
        // 类型检查
        // simple_expression -> simple_expression - term.
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,"-");
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            semantic_error(real_ast,"invalid operands to binary - (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.type_ptr = result;

        $$.simple_expression_node = new SimpleExpressionNode();
        $$.simple_expression_node->append_child($1.simple_expression_node);
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($3.term_node);
    };
term:
    factor
    {   
        // term -> factor.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.factor_node);
    }
    | term MULOP factor
    {  
        // term -> term mulop factor.
        // 类型检查
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,$2.value.get<string>());
        if(result==TYPE_ERROR){
            string tn1 = type_name($1.type_ptr);
            string tn2 = type_name($3.type_ptr);
            string tn3 = $2.value.get<string>();
            semantic_error(real_ast,"invalid operands to binary "+tn3+" (have '"+tn1+"' and '"+tn2+"')",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        
        std::string mulop = $2.value.get<string>();
        if(mulop == "/" && !error_flag){
            $1.term_node->set_op_div(true);
        } else if(mulop == "div"){
            mulop = "/";
        } else if (mulop == "mod"){
            mulop = "%";
        } else if (mulop == "and"){
            mulop = "&&";
        }
        if(error_flag)
            break;
        $$.term_node = new TermNode();
        $$.term_node->append_child($1.term_node);
        LeafNode *mulop_node = new LeafNode(ConstValue(mulop));
        $$.term_node->append_child(mulop_node);
        $$.term_node->append_child($3.factor_node);
    };
factor:
    unsigned_const_variable
    {   
        // factor -> unsigned_const_variable.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::UCONST_VAR);
        $$.factor_node->append_child($1.unsigned_constant_var_node);
    }
    | variable
    {   
        // factor -> variable.
        $$.type_ptr = $1.type_ptr;
        $$.is_lvalue = $1.is_lvalue;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::VARIABLE);
        $$.factor_node->append_child($1.variable_node);
        if($1.name) delete $1.name;
    }
    |ID '(' expression_list ')'
    {
        if(error_flag)
            break;
        $$.is_lvalue = false;
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            semantic_error(real_ast,"undefined function '" + $1.value.get<string>() + "'",$1.line_num,$1.column_num);
            break;
        }else if(!tmp->AssertParams(*($3.type_ptr_list),*($3.is_lvalue_list))){
            string tn = $1.value.get<string>();
            string param = tmp->ParamName();
            string input = type_name(*($3.type_ptr_list));
            semantic_error(real_ast,"invalid arguments '("+input+")' to function '"+tn+"' (expected '("+param+")')",line_count,0);
            break;
        }
        //if(error_flag)
        //   break;
        $$.type_ptr = tmp->type();
        $$.factor_node = new FactorNode(FactorNode::GrammarType::ID_EXP_LIST);
        LeafNode *id_node = new LeafNode($1.value);
        $$.factor_node->append_child(id_node);
        $$.factor_node->append_child($3.expression_list_node);
        auto ref_vec = tmp->ParamRefVec();
        auto ref_stack = new std::stack<bool>();
        for (auto i : ref_vec){
            ref_stack->push(i);
        }
        $3.expression_list_node->DynamicCast<ExpressionListNode>()->set_ref(ref_stack);
        delete ref_stack;
        real_ast->libs()->Call(tmp->name());
        delete $3.type_ptr_list;
        delete $3.is_lvalue_list;

    }
    | '(' expression ')'
    {
        // factor -> (expression).
        $$.type_ptr = $2.type_ptr;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::EXP);
        $$.factor_node->append_child($2.expression_node);
    }
    | NOT factor
    {   
        // factor -> not factor.
        // 类型检查
        if(!is_basic($2.type_ptr)){
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
        auto result=compute((BasicType*)$2.type_ptr, "not");
        
        if(result==TYPE_ERROR){
            semantic_error(real_ast,"wrong type argument to unary not",line_count,0);
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;

        if(error_flag)
            break;
        $$.factor_node = new FactorNode(FactorNode::GrammarType::NOT);
        $$.factor_node->append_child($2.factor_node);
    };
unsigned_const_variable :
    num
    {
        // unsigned_const_variable -> num
        $$.type_ptr = $1.type_ptr;
        if(error_flag)
            break;
        LeafNode *num_node = new LeafNode($1.value);
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        $$.unsigned_constant_var_node->append_child(num_node);
    };
    | CHAR
    {
        // unsigned_const_variable -> 'LETTER'
        $$.type_ptr = TYPE_CHAR;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode($1.value);
        $$.unsigned_constant_var_node->append_child(char_node);
    }
    |TRUE
    {
        // unsigned_const_variable -> true
        $$.type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(ConstValue(true));
        $$.unsigned_constant_var_node->append_child(true_node);
    }
    | FALSE
    {   
        // unsigned_const_variable -> false
        $$.type_ptr = TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(ConstValue(false));
        $$.unsigned_constant_var_node->append_child(false_node);
    };

/*---------------.
| Error handler  |
`---------------*/
/*--紧急恢复--*/
program:error
    {
        location_pointer_refresh();
        new_line_flag=false;
        yyerror(real_ast,"unrecoverable errors occurred");
        while (new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        while (yychar!= YYEOF){
            yychar = yylex();
        }        
    };
/*--短语级恢复--*/

    /*PROGRAM相关错误*/

program_head: PROGRAM error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected identifier before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }; 

program: program_head program_body error
    {
        location_pointer_refresh();
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        ProgramNode* node = new ProgramNode();
	node->append_child($1);
	node->append_child($2);
	real_ast->set_root(node);
	delete top_table_set;

        int length=cur_line_info.size();
        if(length==0){
            length = last_line_info.size();
            char msg[]="expected '.' at the end of the program";
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }else{        
            yyerror(real_ast,"expected '.' at the end of the program");
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    };

    /*定义语句相关*/

const_declarations: CONST error
    {
        if(yychar==TYPE || yychar==BEGIN_ || yychar==VAR || yychar==FUNCTION || yychar== PROCEDURE){
            char msg[] = "expected ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
            break;
        }
        else if(yychar==ID){
            char msg[] = "expected ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else{
            location_pointer_refresh();
            new_line_flag=false;
            if(yychar=='=')
                yyerror(real_ast,"expected identifier before '='");
            else
                yyerror(real_ast,"unrecoverable errors occurred in const_declarations");
            while (new_line_flag==false && yychar!= YYEOF){
                yychar = yylex();
            }
            if(new_line_flag){
                fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
                fprintf(stderr,"\t| %s",location_pointer);
            }
        }    
        while (yychar!=TYPE && yychar!= VAR && yychar!= YYEOF && yychar != FUNCTION && yychar!=PROCEDURE && yychar!=BEGIN_){
            yychar = yylex();
        }
    };

type_declarations: TYPE  error
    {
        if(yychar==BEGIN_ || yychar==VAR || yychar==FUNCTION || yychar== PROCEDURE){
            char msg[] = "expected ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
            break;
        }
        else if(yychar==ID){
            char msg[] = "expected ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else{     
            location_pointer_refresh();
            new_line_flag=false;
            if(yychar=='=')
                yyerror(real_ast,"expected identifier before '='");
            else
                yyerror(real_ast,"unrecoverable errors occurred in type_declarations");
            while (new_line_flag==false && yychar!= YYEOF){
                yychar = yylex();
            }
            if(new_line_flag){
                fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
                fprintf(stderr,"\t| %s",location_pointer);
            }
        }
        while (yychar!= VAR && yychar!= YYEOF && yychar != FUNCTION && yychar!=PROCEDURE && yychar!=BEGIN_){
            yychar = yylex();
        }
    };

var_declarations: VAR error
    {
        if( yychar==BEGIN_ || yychar==FUNCTION || yychar== PROCEDURE){
            char msg[] = "expcted ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
            break;
        }
        else if(yychar==ID){      
            char msg[] = "expected ';'";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else{
            location_pointer_refresh();
            new_line_flag=false;
            if(yychar==':')
                yyerror(real_ast,"expected identifier before ':'");
            else
                yyerror(real_ast,"unrecoverable errors occurred in var_declarations");
            while (new_line_flag==false && yychar!= YYEOF){
                yychar = yylex();
            }
            if(new_line_flag){
                fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
                fprintf(stderr,"\t| %s",location_pointer);
            }
        }
        while (yychar!= YYEOF && yychar != FUNCTION && yychar!=PROCEDURE && yychar!=BEGIN_){
           yychar = yylex();
        }
        
    };

const_declaration: ID const_variable
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected '=' before const value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

const_declaration: const_declaration ';' ID const_variable
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected '=' before const value");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };     

const_declaration: ID '=' error  
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected const value before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    }; 

const_declaration: ID ASSIGNOP 
    {
        yyerror(real_ast,"expected '=' (have ':=')");
        location_pointer_refresh();
    }
    const_variable 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };
    
const_declaration: const_declaration ';' ID ASSIGNOP
    {
        yyerror(real_ast,"expected '=' (have ':=')");
        location_pointer_refresh();
    }
    const_variable
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

const_declaration: ID ':' 
    {
        yyerror(real_ast,"expected '=' (have ':')");
        location_pointer_refresh();
    }
    const_variable 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };
    
const_declaration: const_declaration ';' ID ':'
    {
        yyerror(real_ast,"expected '=' (have ':')");
        location_pointer_refresh();
    }
    const_variable
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };    
     
const_declaration: const_declaration ';' ID '=' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected const value before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }; 

const_declaration: const_declaration ';'error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar=='=')
            yyerror(real_ast,"expected identifier before '='");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type_declaration: ID type
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected '=' before type");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type_declaration: type_declaration ';' ID type
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected '=' before type");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };    

type_declaration: ID '=' error  
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected type before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    }; 

type_declaration: ID ASSIGNOP 
    {
        yyerror(real_ast,"expected '=' (have ':=')");
        location_pointer_refresh();
    }
    type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type_declaration: type_declaration ';' ID ASSIGNOP 
    {
        yyerror(real_ast,"expected '=' (have ':=')");
        location_pointer_refresh();
    }
    type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type_declaration: ID ':' 
    {
        yyerror(real_ast,"expected '=' (have ':')");
        location_pointer_refresh();
    }
    type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type_declaration: type_declaration ';' ID ':' 
    {
        yyerror(real_ast,"expected '=' (have ':')");
        location_pointer_refresh();
    }
    type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };    
     
type_declaration: type_declaration ';' ID '=' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected type before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };     

type_declaration: type_declaration ';' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar=='=')
            yyerror(real_ast,"expected identifier before '='");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };
    
var_declaration:id_list ID_or_type
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected ':' before identifier or type");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: var_declaration ';' id_list ID_or_type
    {
        location_pointer_refresh();
        yyerror(real_ast,"expected ':' before identifier or type");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };     

var_declaration: id_list ':' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected type identifier before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    };

var_declaration: var_declaration ';' id_list ':' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected type identifier before ';'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: var_declaration ';' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==':')
            yyerror(real_ast,"expected identifier before ':'");
        else
            yyerror(real_ast,"Syntax error");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: id_list ASSIGNOP 
    {
        yyerror(real_ast,"expected ':' (have ':=')");
        location_pointer_refresh();
    }
    ID_or_type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: var_declaration ';' id_list ASSIGNOP 
    {
        yyerror(real_ast,"expected ':' (have ':=')");
        location_pointer_refresh();
    }
    ID_or_type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: ID '=' 
    {
        yyerror(real_ast,"expected ':' (have '=')");
        location_pointer_refresh();
    }
    ID_or_type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

var_declaration: var_declaration ';' ID '=' 
    {
        yyerror(real_ast,"expected ':' (have '=')");
        location_pointer_refresh();
    }
    ID_or_type 
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

ID_or_type:ID
    | type
    ;       
/*其他*/

type: ARRAY '[' periods ']' error
    {
        new_line_flag=false;
        location_pointer_refresh();
        yyerror(real_ast,"expected 'of' before type.");
        while(yychar!=';' && !new_line_flag)
            yychar=yylex();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type: ARRAY  error
    {
        new_line_flag=false;
        location_pointer_refresh();
        yyerror(real_ast,"Invaild periods.");
        while(yychar!=';' && !new_line_flag)
            yychar=yylex();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

type: ARRAY '[' periods ']' OF error
    {
        new_line_flag=false;
        location_pointer_refresh();
        yyerror(real_ast,"unknown error!");
        while(yychar!=';' && !new_line_flag)
            yychar=yylex();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };    

id_varpart: '[' error 
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';'){
            yyerror(real_ast,"expected ']' before ';'");
        }else if(yychar == ASSIGNOP){
            yyerror(real_ast,"expected ']' before ':='");
        }else
            yyerror(real_ast,"invalid expression");
        int left_num = 1;   // 括号匹配
        while (yychar!=';' && yychar!=ASSIGNOP && new_line_flag==false && yychar!= YYEOF ){
            if(yychar=='[') left_num++;
            if(yychar==']'&& left_num == 1) break; 
            yychar = yylex();
        }
        if(yychar==']'){
            yychar=yylex();
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(yychar==ASSIGNOP){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    };

type: ARRAY '[' periods ']' OF ID
    {
        yyerror(real_ast,"unsupported definition of array using customized type");
        location_pointer_refresh();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }; 

factor: '(' error 
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected ')' before ';'");
        else
            yyerror(real_ast,"invalid expression");
        int left_num = 1;   // 括号匹配
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF ){
            if(yychar=='(') left_num++;
            if(yychar==')'&& left_num == 1) break; 
            yychar = yylex();
        }
        if(yychar==')'){
            yychar=yylex();
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    };

subprogram_head: FUNCTION ID formal_parameter ':' error
    {
        new_line_flag=false;
        location_pointer_refresh();
        if(yychar==ARRAY||yychar==RECORD||yychar==ID)
        {
            yyerror(real_ast,"return type of function should be integer, real, boolean or char");
        }
        while(yychar!=';'&&!new_line_flag)
            yychar=yylex();
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
            yychar=yylex();
        }
    }    


/*statement相关*/
// compound_statement: BEGIN_ statement_list END
// IF expression THEN statement else_part
statement: IF error
    {
        new_line_flag=false;
        location_pointer_refresh();
        while(yychar!=THEN && !new_line_flag&&yychar!=';')
            yychar=yylex();
        if(yychar==THEN){
            yyerror(real_ast,"invalid expression");
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
            yychar=yylex();
        }
        else if(yychar==';'){
            char msg[] = "'THEN' might be missing";
            int length = last_line_info.size();
            fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
            memset(location_pointer,' ',length);
            memcpy(location_pointer+length,"^\n\0",3);
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else{
            yyerror(real_ast,"Syntax error");
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        while(yychar!=';'&&yychar!=END)
            yychar=yylex();
    }
    ;
// REPEAT statement_list UNTIL expression
statement: REPEAT error 
    {
        new_line_flag=false;
        if(yychar=='='||yychar==RELOP||yychar==END)
            yyerror(real_ast,"'UNTIL' might be missing");
        else
            yyerror(real_ast,"Syntax error");
        location_pointer_refresh();
        while(yychar!=';'&&!new_line_flag && yychar!=END)
            yychar=yylex();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

// statement:FOR ID ASSIGNOP expression updown expression DO statement 

// WHILE expression DO statement

statement: WHILE error
{
    new_line_flag=false;
    location_pointer_refresh();
    char msg[] = "'do' might be missing";
    int length = last_line_info.size();
    if(yychar==ID)
        fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,msg);   
    else{
        fprintf(stderr,"%d,%d:\033[01;31m \terror\033[0m : %s\n", last_line_count,length,"Syntax error");   
    }
    while(yychar!=';'&&!new_line_flag && yychar!=END)
        yychar=yylex();
    memset(location_pointer,' ',length);
    memcpy(location_pointer+length,"^\n\0",3);
    fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
    fprintf(stderr,"\t| %s",location_pointer);
};

statement:WHILE expression  DO error
{
    new_line_flag=false;
    yyerror(real_ast,"Syntax error");
    location_pointer_refresh();
    while(yychar!=';'&&!new_line_flag && yychar!=END)
        yychar=yylex();
    fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
    fprintf(stderr,"\t| %s",location_pointer);
}; 

statement:FOR ID ASSIGNOP expression  updown expression DO error
{
    new_line_flag=false;
    yyerror(real_ast,"Syntax error");
    location_pointer_refresh();
    while(yychar!=';'&& yychar!=END)
        yychar=yylex();
    if(new_line_flag){
        fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    else if(yychar==';'){
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
};

statement:FOR ID ASSIGNOP expression  updown error
{
    new_line_flag=false;
    yyerror(real_ast,"Syntax error");
    location_pointer_refresh();
    while(yychar!=';'&& yychar!=END)
        yychar=yylex();
    if(new_line_flag){
        fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    else if(yychar==';'){
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
};

statement:FOR ID ASSIGNOP error
{
    new_line_flag=false;
    if(yychar==INT_NUM)
        yyerror(real_ast,"'to' or 'downto' might be missing");
    else
        yyerror(real_ast,"Syntax error");
    location_pointer_refresh();
    while(yychar!=';'&& yychar!=END)
        yychar=yylex();
    if(new_line_flag){
        fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    else if(yychar==';'){
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
};


statement: variable ASSIGNOP type
    {
        yyerror(real_ast,"type identifier not allowed");
        location_pointer_refresh();
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

statement: variable ASSIGNOP error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"expected expression before ';'");
        else
            yyerror(real_ast,"invalid expression");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",last_line_count,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    };

statement: variable ':' 
    {
        yyerror(real_ast,"expected ':=' (have ':' and '=')");
        location_pointer_refresh();
    } '=' expression
    {
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    ;    

%%
 

void yyerror(ast::AST* real_ast,const char *msg){
    if(yydebug || strcmp(msg,"syntax error")!=0)   // 当非debug模式且传入的是默认报错时不输出 
        fprintf(stderr,"%d,%ld:\033[01;31m \terror\033[0m : %s\n", line_count,cur_line_info.size(),msg);   
    error_flag = 1;
    real_ast->set_root(nullptr);
}

void yynote(std::string msg ,int line){
    fprintf(stderr,"%d:\033[01;32m \tnote\033[0m : previous definition of \"%s\" was here\n", line, msg.c_str());
}

void yyerror_var(AST* real_ast,int line){
    fprintf(stderr,"%d:\033[01;31m \terror\033[0m : %s\n", line, "redifinition of variable");
    error_flag = 1;
    real_ast->set_root(nullptr);
}

void location_pointer_refresh(){
    int length = cur_line_info.size()-yyleng;
    if(length<0)
        length=0;
    memset(location_pointer,' ',length);
    memcpy(location_pointer+length,"^\n\0",3);
}
int yywrap(){
    return 1;
}
