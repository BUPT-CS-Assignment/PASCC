%{	
#include"parser.h"
using namespace ast;
using namespace pascal_symbol;
using namespace pascal_type;
using std::string;
extern "C"			
{					
    void yyerror(const char *s);
    extern int yylex(void);
    extern int line_count;
    extern bool new_line_flag;
    extern int yyleng;
}
extern std::string cur_line_info;
extern std::string last_line_info;
//AST real_ast;
//symbol_table::SymbolTable *real_symbol_table = new symbol_table::SymbolTable();
std::stack<symbol_table::TableSet*> table_set_queue;
symbol_table::TableSet* top_table_set = new symbol_table::TableSet("main",nullptr);
//table_set_queue.push(top_table_set);
int error_flag=0;
char location_pointer[256];
void location_pointer_refresh();

void yyerror(ast::AST* real_ast,const char *msg);
void yyerror_var(ast::AST* real_ast,int line);
void yynote(std::string msg,int line);
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
%token ARRAY TYPE CONST RECORD
%token IF THEN ELSE CASE OF WHILE DO FOR REPEAT UNTIL BEGIN_ END
%token ADDOP NOT PLUS UMINUS ASSIGNOP TRUE FALSE CONSTASSIGNOP READ WRITE WRITELN
%token<token_info> ID CHAR INT_NUM REAL_NUM BASIC_TYPE RELOP MULOP STRING_ VAR
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
        if(!error_flag){
            ProgramNode* node = new ProgramNode();
            node->append_child($1);
            node->append_child($2);
            real_ast->set_root(node);
        } else {
            real_ast->set_root(nullptr); 
        }
    };
program_head :
    PROGRAM ID ';' {
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
        $1.list_ref->push_back(std::make_pair($3.value.get<string>(),$3.line_num));
        $$.list_ref = $1.list_ref;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::MULTIPLE_ID);
        if(error_flag)
            break;
        LeafNode* leaf_node = new LeafNode($3.value);
        $$.id_list_node->append_child($1.id_list_node);
        $$.id_list_node->append_child(leaf_node);
    } | ID {
        $$.list_ref = new std::vector<std::pair<std::string,int>>();
        $$.list_ref->push_back(std::make_pair($1.value.get<string>(),$1.line_num));
        if(error_flag)
            break;
        $$.id_list_node = new IdListNode(IdListNode::GrammarType::SINGLE_ID);
        LeafNode* leaf_node = new LeafNode($1.value);
        $$.id_list_node->append_child(leaf_node);
    };
const_declarations :{
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode();
    }
    | CONST const_declaration ';'
    {   
        // const_declarations -> const const_declaration
        if(error_flag)
            break;
        $$ = new ConstDeclarationsNode(); 
        $$->append_child($2);
        
    };
const_declaration :
    const_declaration ';' ID '=' const_variable
    {
        if (!$5.is_right){
            break;
        }
        pascal_symbol::ConstSymbol *symbol = new ConstSymbol($3.value.get<string>(),$5.value,$3.line_num);

        if(!table_set_queue.top()->Insert<ConstSymbol>($3.value.get<string>(),symbol)){
            yyerror(real_ast,"The identifier has been declared.\n");
        } 
        else{
            if(error_flag)
                break;
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::DECLARATION,$5.type_ptr);
            $$->append_child($1);
            LeafNode* leaf_node = new LeafNode($3.value);
            $$->append_child(leaf_node);
            $$->append_child($5.const_variable_node);
            // const_declaration -> const_declaration ; id = const_variable.
        }
        
    }
    | ID '=' const_variable
    {   
        
        // const_declaration -> id = const_variable.
        //TODO 插入符号表
        if (!$3.is_right){
            break;
        }
        pascal_symbol::ConstSymbol *symbol = new ConstSymbol($1.value.get<string>(),$3.value,$1.line_num);
        if(!table_set_queue.top()->Insert<ConstSymbol>($1.value.get<string>(),symbol)){
            yyerror(real_ast,"The identifier has been declared.");
        } 
        else {
            if(error_flag)
                break;
            $$ = new ConstDeclarationNode(ConstDeclarationNode::GrammarType::VALUE,$3.type_ptr);
            LeafNode* leaf_node = new LeafNode($1.value);
            $$->append_child(leaf_node);
            $$->append_child($3.const_variable_node);
        }
    };
const_variable :
    PLUS ID
    {   
        // const_variable -> + id.
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.get<string>());
        $$.type_ptr = nullptr;
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.");
            $$.is_right = false;
        }else {
            // You cannot use variables to assign values to constants
            if (pascal_symbol::ObjectSymbol::SYMBOL_TYPE::CONST != symbol->symbol_type()){
                yyerror(real_ast,"The identifier is not a const variable.");
                $$.is_right = false;
                break;
            }
            $$.value = symbol->value();
            $$.type_ptr = symbol->type();
            if(error_flag)
                break;
            $$.const_variable_node = new LeafNode(ConstValue("+" + $2.value.get<string>()));
            
        }
    }
    | UMINUS ID
    {
        // const_variable -> - id. todo -
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($2.value.get<string>());
        $$.type_ptr = nullptr;
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.");
            $$.is_right = false;
        }else {
            // You cannot use variables to assign values to constants
            if (pascal_symbol::ObjectSymbol::SYMBOL_TYPE::CONST != symbol->symbol_type()){
                yyerror(real_ast,"The identifier is not a const variable.");
                $$.is_right = false;
                break;
            }
            $$.type_ptr = symbol->type();
            $$.value = symbol->value();
            //$$.const_value = -(symbol->value());
            if(error_flag)
                break;
            $$.const_variable_node = new LeafNode(ConstValue("-" + $2.value.get<string>()));
        }
    }
    | ID
    {
        // const_variable -> id.
        ConstSymbol *symbol = table_set_queue.top()->SearchEntry<ConstSymbol>($1.value.get<string>());
        $$.type_ptr = nullptr;
        if(!symbol){
            yyerror(real_ast,"The identifier has not been declared.");
            $$.is_right = false;
        } else {
            // You cannot use variables to assign values to constants
            if (pascal_symbol::ObjectSymbol::SYMBOL_TYPE::CONST != symbol->symbol_type()){
                yyerror(real_ast,"The identifier is not a const variable.");
                $$.is_right = false;
                break;
            }
            $$.type_ptr = symbol->type();
            $$.value = symbol->value();
        }
        if(error_flag)
            break;
        $$.const_variable_node = new LeafNode($1.value);
    }
    |UMINUS num
    {  
        //TODO
        // const_variable -> - num.
        $$.type_ptr = $2.type_ptr;
        $2.value.set_unimus();
        //$$.value = $$.value * ConstValue(-1, $2.type_ptr==pascal_type::TYPE_REAL);
        $$.value = $2.value;
        if(error_flag)
            break; 
        //$$.const_variable_node = new LeafNode($2.value * ConstValue(-1, $2.type_ptr==pascal_type::TYPE_REAL));
        $$.const_variable_node = new LeafNode($2.value);
    }
    | num
    {   
        // const_variable -> num.
        $$.type_ptr = $1.type_ptr;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);
    }
    |PLUS num
    {  
        // const_variable -> +num.
        $$.type_ptr = $2.type_ptr;
        $$.value = $2.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($2.value);
        // $$.const_variable_node = new ConstVariableNode();
        // LeafNode* leaf_node = new LeafNode($2.value.m_INT);//?
        // $$.const_variable_node->append_child(leaf_node);
    }
    | CHAR
    {
        // const_variable -> 'letter'.
        $$.type_ptr = pascal_type::TYPE_CHAR;
        $$.value = $1.value;
        if(error_flag)
            break; 
        $$.const_variable_node = new LeafNode($1.value);

    };

num :
    INT_NUM
    {
        // num -> int_num.
        $$.type_ptr = pascal_type::TYPE_INT;
        $$.value = $1.value;
    }
    | REAL_NUM
    {   
        // num -> real_num.
        $$.type_ptr = pascal_type::TYPE_REAL;
        $$.value = $1.value;
    };
type_declarations : 
    {
    if(error_flag)
            break;
        // type_declarations -> empty.
        $$ = new TypeDeclarationsNode();
    }
    | TYPE type_declaration ';'
    {
        if(error_flag)
            break;
        $$ = new TypeDeclarationsNode();
        $$->append_child($2);
        // type_declarations -> type type_declaration.
    };
type_declaration :
    type_declaration ';' ID '=' type
    {
        // TODO
        // type_declaration -> type_declaration ; id = type.
        if ($5.main_type == TypeAttr::BASIC) {
            //pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>($5.type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>($3.value.get<string>(),dynamic_cast<BasicType*>($5.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            }
        } else if ($5.main_type == TypeAttr::ARRAY) {
            //pascal_type::ArrayType *array_type = new pascal_type::ArrayType($5.type_ptr,*($5.bounds));
            if (!table_set_queue.top()->Insert<ArrayType>($3.value.get<string>(),dynamic_cast<ArrayType*>($5.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            } 
        } else if ($5.record_info) {
            //pascal_type::RecordType *record_type = new pascal_type::RecordType(*($5.record_info));
            if (!table_set_queue.top()->Insert<RecordType>($3.value.get<string>(),dynamic_cast<RecordType*>($5.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            } 
        }

        if(error_flag)
            break;

        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::MULTIPLE_DECL);
        $$->append_child($1);
        LeafNode *leaf_node = new LeafNode($3.value);
        $$->append_child(leaf_node);
        $$->append_child($5.type_node);
    }
    | ID '=' type
    {
        // TODO!
        // type_declaration -> id = type.
        if ($3.main_type == TypeAttr::BASIC) {
            //pascal_type::BasicType *basic_type = new pascal_type::BasicType(dynamic_cast<BasicType*>($3.type_ptr)->type());
            if (!table_set_queue.top()->Insert<BasicType>($1.value.get<string>(),dynamic_cast<BasicType*>($3.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            } 
        } else if ($3.main_type == TypeAttr::ARRAY) {
            //pascal_type::ArrayType *array_type = new pascal_type::ArrayType($3.array_type_ptr,*($3.bounds));
            if (!table_set_queue.top()->Insert<ArrayType>($1.value.get<string>(),dynamic_cast<ArrayType*>($3.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            } 
        } else if ($3.record_info) {
            //pascal_type::RecordType *record_type = new pascal_type::RecordType(*($3.record_info));
            if (!table_set_queue.top()->Insert<RecordType>($1.value.get<string>(),dynamic_cast<RecordType*>($3.type_ptr))){
                yyerror(real_ast,"Error: redefinition of type.");
            } 
        }

        if(error_flag)
            break;
        $$ = new TypeDeclarationNode(TypeDeclarationNode::GrammarType::SINGLE_DECL);
        LeafNode *leaf_node = new LeafNode($1.value);
        $$->append_child(leaf_node);
        $$->append_child($3.type_node);
    };
type :
    standrad_type
    {
        // type -> standrad_type.
        $$.main_type = (TypeAttr::MainType)0;
        $$.type_ptr = $1.type_ptr;
        //if ($$.type_ptr == pascal_type::TYPE_BOOL) std::cout<<"666"<<std::endl;
        if(error_flag)
            break;
        $$.type_node = new TypeNode(TypeNode::GrammarType::BASIC_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->set_base_type_node($$.type_node);
        $$.type_node->append_child($1.standard_type_node);
    }
    | ARRAY '[' periods ']' OF type
    {
        // TODO
        // type -> array [periods] of stype.
        $$.main_type = (TypeAttr::MainType)1;
        $$.base_type_node = $6.base_type_node;
        $$.bounds = $3.bounds;
        if ($3.bounds){
            // connect $3.bounds and $6.bounds
            auto merged_bounds = new std::vector<ArrayType::ArrayBound>();
            for (auto i : *($3.bounds)){
                merged_bounds->push_back(i);
            }
            auto basic_type = $6.type_ptr;
            if($6.type_ptr->template_type() == pascal_type::TypeTemplate::TYPE::ARRAY) {
                for (auto i : *($6.bounds)){
                    merged_bounds->push_back(i);
                }
                basic_type = $6.type_ptr->DynamicCast<pascal_type::ArrayType>()->base_type();
            }

            $$.type_ptr = new pascal_type::ArrayType(basic_type, *merged_bounds);
            delete merged_bounds;
        }
        
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::ARRAY);
        $$.type_node->set_base_type_node($6.base_type_node);
        $$.type_node->append_child($3.periods_node);
        $$.type_node->append_child($6.type_node);
    }
    | RECORD record_body END
    {
        // TODO
        $$.main_type = (TypeAttr::MainType)2;
        $$.record_info = $2.record_info;
        if ($2.record_info){
            $$.type_ptr = new pascal_type::RecordType(*($2.record_info));
        } else{
             $$.type_ptr = new pascal_type::RecordType();
        }
        if(error_flag)
            break; 
        $$.type_node = new TypeNode(TypeNode::GrammarType::RECORD_TYPE);
        $$.base_type_node = $$.type_node;
        $$.type_node->append_child($2.record_body_node);
        $$.type_node->set_base_type_node($$.type_node);
        // type -> record record_body end.
    };
record_body :
    {
        // record_body -> empty.
        $$.record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
    }
    | var_declaration
    {
        // record_body -> var_declaration.
        $$.record_info = $1.record_info;
        if(error_flag)
            break;
        $$.record_body_node = new RecordBodyNode();
        $$.record_body_node->append_child($1.variable_declaration_node);
    };
standrad_type :
    BASIC_TYPE
    {
        // standrad_type -> int|real|bool|char.
        //std::cout<<"$1.value.get<string>():"<<$1.value.get<string>()<<std::endl;
        string typestr = $1.value.get<string>();
        if (typestr == "integer"){
            $$.type_ptr = pascal_type::TYPE_INT;
        } else if(typestr == "real"){
            $$.type_ptr = pascal_type::TYPE_REAL;
        } else if(typestr == "boolean"){
            $$.type_ptr = pascal_type::TYPE_BOOL;
        } else{
            $$.type_ptr = pascal_type::TYPE_CHAR;
        }
        if(error_flag)
            break;
        $$.standard_type_node = new BasicTypeNode();
        $$.standard_type_node->set_type(dynamic_cast<pascal_type::BasicType*>($$.type_ptr));
    };
periods :
    periods ',' period
    {
        // periods -> periods,period.
        $$.bounds = $1.bounds;
        $$.bounds->push_back(*($3.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.periods_node);
        $$.periods_node->append_child($3.period_node);
    }
    | period
    {
        // periods -> period.
        // TODO fix with type
        $$.bounds = new std::vector<ArrayType::ArrayBound>();
        $$.bounds->push_back(*($1.bound));
        if(error_flag)
            break;
        $$.periods_node = new PeriodsNode();
        $$.periods_node->append_child($1.period_node);
    };
period :
    const_variable SUBCATALOG const_variable
    {
        // test type ID 
        // period -> const_variable .. const_variable.
        
        int arr_len=0;
        // TODO fix with bound_type
        $$.bound = new pascal_type::ArrayType::ArrayBound();
        if ($1.type_ptr == pascal_type::TYPE_INT&&$3.type_ptr == pascal_type::TYPE_INT){
            arr_len = std::max(0,($3.value - $1.value).get<int>());
            $$.bound-> type_ = pascal_type::TYPE_INT;
            $$.bound->lb_ = $1.value.get<int>();
            $$.bound->ub_ = $3.value.get<int>();
        } else if($1.type_ptr == pascal_type::TYPE_CHAR&&$3.type_ptr == pascal_type::TYPE_CHAR){
            arr_len = std::max(0,(int)($3.value - $1.value).get<char>());
            $$.bound-> type_ = pascal_type::TYPE_CHAR;
            $$.bound->lb_ = int($1.value.get<int>());
            $$.bound->ub_ = int($3.value.get<int>());
        } else {
            yyerror(real_ast,"array bound must be integer or char");
        }
        if(arr_len < 0){
            yyerror(real_ast,"array bound must be positive");
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
        if(error_flag)
            break;
        // var_declarations -> empty.
        $$ = new VariableDeclarationsNode();
    }
    | VAR var_declaration ';'
    {
        if(error_flag)
            break;
        for (auto i : *($2.record_info)){
            ObjectSymbol *obj = new ObjectSymbol(i.first, i.second,10);//TODO
            if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first,obj)){
                yyerror_var(real_ast,$1.line_num);
                yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
            }
        }
        // var_declarations -> var var_declaration.
        $$ = new VariableDeclarationsNode();
        $$->append_child($2.variable_declaration_node);
    };
var_declaration :
    var_declaration ';' id_list ':' type 
    {
        
        // var_declaration -> var_declaration ; id_list : type.
        $$.record_info = $1.record_info;
        for (auto i : *($3.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $5.type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");
            }
        }
        if(error_flag)
            break;
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        $$.variable_declaration_node->append_child($5.type_node);
    }
    | id_list ':' type 
    {
        $$.record_info = new std::unordered_map<std::string, pascal_type::TypeTemplate*>();
        for (auto i : *($1.list_ref)){
            auto res = $$.record_info->insert(make_pair(i.first, $3.type_ptr));
            if (!res.second){
             yyerror(real_ast,"redefinition of variable");
             }
        }
        // var_declaration -> id : type.
        if(error_flag)
           break;
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::TYPE);
        $$.variable_declaration_node->append_child($1.id_list_node);
        $$.variable_declaration_node->append_child($3.type_node);
    }
    |var_declaration ';' id_list ':' ID
    {
        $$.record_info = $1.record_info;
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($5.value.get<string>());
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
        if(error_flag)
            break;
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::MULTIPLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.variable_declaration_node);
        $$.variable_declaration_node->append_child($3.id_list_node);
        LeafNode *leaf_node = new LeafNode($5.value);
        $$.variable_declaration_node->append_child(leaf_node);
    }
    |id_list ':' ID
    {
        TypeTemplate *tmp = table_set_queue.top()->SearchEntry<TypeTemplate>($3.value.get<string>());
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
        if(error_flag)
            break;
        $$.variable_declaration_node = new VariableDeclarationNode(VariableDeclarationNode::GrammarType::SINGLE_DECL,VariableDeclarationNode::ListType::ID);
        $$.variable_declaration_node->append_child($1.id_list_node);
        LeafNode *leaf_node = new LeafNode($3.value);
        $$.variable_declaration_node->append_child(leaf_node);
    };
subprogram_declarations : 
    {
        if(error_flag)
            break;
        // subprogram_declarations -> empty.
        $$ = new SubprogramDeclarationsNode();
    }
    | subprogram_declarations subprogram_declaration ';'
    {
        if(error_flag)
            break;
        // subprogram_declarations -> subprogram_declarations subprogram_declaration.
        $$ = new SubprogramDeclarationsNode();
        $$->append_child($1);
        $$->append_child($2);
        table_set_queue.pop();        
    };
subprogram_declaration :
    subprogram_head subprogram_body
    {
        if(error_flag)
            break;
        // subprogram_declaration -> subprogram_head program_body.
        $$ = new SubprogramDeclarationNode();
        $$->append_child($1);
        $$->append_child($2);
    };
subprogram_body :
    const_declarations type_declarations var_declarations compound_statement
    {
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
        // subprogram_head -> function id formal_parametert : standrad_type.
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), $5.type_ptr, $2.line_num);
        }
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp)){
            yyerror(real_ast,"redefinition of function");
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet($2.value.get<string>(), table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp);
        if ($3.parameters){
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if (i.second.second == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                    yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
                }
            }
        }
        if(error_flag)
            break;
        $$ = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::FUNCTION);
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
        $$->append_child($5.standard_type_node);
    }
    | PROCEDURE ID formal_parameter ';'
    {
        // subprogram_head -> procedure id formal_parametert.
        FunctionSymbol* tmp ;
        if($3.parameters){
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num, *$3.parameters);
        } else {
            tmp = new FunctionSymbol($2.value.get<string>(), nullptr, $2.line_num);
        }
        
        if (!table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp)){
            yyerror(real_ast,"redefinition of function");
            yynote($2.value.get<string>(),table_set_queue.top()->SearchEntry<FunctionSymbol>($2.value.get<string>())->decl_line());
        } 

        symbol_table::TableSet* now_table_set = new symbol_table::TableSet($2.value.get<string>(),table_set_queue.top());
        table_set_queue.push(now_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        table_set_queue.top()->Insert<FunctionSymbol>($2.value.get<string>(), tmp);
        if ($3.parameters){
            for (auto i : *($3.parameters)){
                ObjectSymbol *tmp = new ObjectSymbol(i.first, i.second.first, $2.line_num);
                if (i.second.second == FunctionSymbol::PARAM_MODE::REFERENCE){
                    tmp->set_ref(true);
                }
                if(!table_set_queue.top()->Insert<ObjectSymbol>(i.first, tmp)){
                    yyerror(real_ast,"redefinition of variable");
                    yynote(i.first,table_set_queue.top()->SearchEntry<ObjectSymbol>(i.first)->decl_line());
                }
            }
        }
        
        if(error_flag)
            break;
        $$ = new SubprogramHeadNode(ast::SubprogramHeadNode::GrammarType::PROCEDURE);
        LeafNode *leaf_node = new LeafNode($2.value);
        $$->append_child(leaf_node);
        $$->append_child($3.formal_parameter_node);
    };
formal_parameter :
    {   
        // formal_parameter -> empty.
        $$.parameters = new std::vector<std::pair<std::string, std::pair<BasicType*,FunctionSymbol::PARAM_MODE>>>();
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();

    }
    | '(' parameter_lists ')'
    {
        // formal_parameter -> (parameter_lists).
        $$.parameters = $2.parameters;
        if(error_flag)
            break;
        $$.formal_parameter_node = new FormalParamNode();
        $$.formal_parameter_node->append_child($2.param_lists_node);
    };
parameter_lists :
    parameter_lists ';' parameter_list
    {   
        // parameter_lists -> parameter_lists ; parameter_list.
        $$.parameters = $1.parameters;
        $$.parameters->insert($$.parameters->end(), $3.parameters->begin(), $3.parameters->end());
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::MULTIPLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_lists_node);
        $$.param_lists_node->append_child($3.param_list_node);
    }
    | parameter_list
    {  
        // parameter_lists -> parameter_list.
        $$.parameters = $1.parameters;
        if(error_flag)
            break;
        $$.param_lists_node = new ParamListsNode(ParamListsNode::GrammarType::SINGLE_PARAM_LIST);
        $$.param_lists_node->append_child($1.param_list_node);
    };
parameter_list :
    var_parameter
    {   
        // parameter_list -> var_parameter.
        $$.parameters = $1.parameters;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.var_parameter_node);
    }
    | value_parameter
    {   
        // parameter_list -> value_parameter.
        $$.parameters = $1.parameters;
        if(error_flag)
            break;
        $$.param_list_node = new ParamListNode();
        $$.param_list_node->append_child($1.value_parameter_node);
    };
var_parameter :
    VAR value_parameter
    {   
        // var_parameter -> var value_parameter.
        int para_len = $2.parameters->size();
        for (int i = 0; i < para_len; i++){
            $2.parameters->at(i).second.second = FunctionSymbol::PARAM_MODE::REFERENCE;
        }
        $$.parameters = $2.parameters;
        if(error_flag)
            break;
        $$.var_parameter_node = new VarParamNode();
        $$.var_parameter_node->append_child($2.value_parameter_node);
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
        
        if(error_flag)
            break;
        $$.value_parameter_node = new ValueParamNode();
        $$.value_parameter_node->append_child($1.id_list_node);
        $$.value_parameter_node->append_child($3.standard_type_node);
    };
compound_statement :
    BEGIN_ statement_list END {
        if(error_flag)
            break;
        // compound_statement -> begin statement_list end.
        $$ = new CompoundStatementNode();
        $$->append_child($2);
    };
statement_list :
    statement_list ';' statement
    {
        if(error_flag)
            break;
        // statement_list -> statement_list ; statement.
        $$ = new StatementListNode();
        $$->append_child($1);
        $$->append_child($3);
    } | statement
    {
        if(error_flag)
            break;
        // statement_list -> statement.
        $$ = new StatementListNode();
        $$->append_child($1);
    };
statement:
    variable ASSIGNOP expression
    {   
        // 类型检查
        //此处赋值存在多种情况，结构体、数组等需要之后一一检查
        // statement -> variable assignop expression.
        //基本情况
        if(!($1.type_ptr==TYPE_REAL&&$3.type_ptr==TYPE_INT)&&!is_same($1.type_ptr,$3.type_ptr)){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"statement -> variable assignop expression\n");
        }
        std::string func_name = table_set_queue.top()->tag();
        if(error_flag)
            break;
        if(func_name == *$1.name){
            $$ = new StatementNode(StatementNode::GrammarType::FUNC_ASSIGN_OP_EXP);
        }else{
            $$ = new StatementNode(StatementNode::GrammarType::VAR_ASSIGN_OP_EXP);
            if (!$1.is_lvalue){
                yyerror(real_ast,"You can only assign values to lvalues");
            }
        }
        if(error_flag)
            break;
        $$->append_child($1.variable_node);
        $$->append_child($3.expression_node);
    }
    | call_procedure_statement
    {
        if(error_flag)
            break;
        // statement -> call_procedure_statement.
        // TODO check
        $$ = new StatementNode(StatementNode::GrammarType::PROCEDURE_CALL);
        $$->append_child($1);
    }
    | compound_statement
    {
        if(error_flag)
            break;
        // statement -> compound_statement.
        $$ = new StatementNode(StatementNode::GrammarType::COMPOUND_STATEMENT);
        $$->append_child($1);
    }
    | IF expression THEN statement else_part
    {   
        if(error_flag)
            break;
        //类型检查
        if(!is_same($2.type_ptr,TYPE_BOOL)){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"IF expression THEN statement else_part\n");
        }
        // statement -> if expression then statement else_part.
        $$ = new StatementNode(StatementNode::GrammarType::IF_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);
        $$->append_child($5);
    }
    | CASE expression OF case_body END
    {
        if(error_flag)
            break;
        //类型检查
        if(!is_same($4.type_ptr,TYPE_ERROR)){
            if((!is_same($2.type_ptr,$4.type_ptr))||is_same($2.type_ptr,TYPE_REAL)){
                yyerror(real_ast,"Type check failed\n");
                yyerror(real_ast,"CASE expression OF case_body END\n");
            }
        }
        // statement -> case expression of case_body end.
        $$ = new StatementNode(StatementNode::GrammarType::CASE_STATEMET);
        $$->append_child($2.expression_node);
        $$->append_child($4.case_body_node);
    }
    | WHILE expression DO statement
    {
        if(error_flag)
            break;
        //类型检查
        if(!is_same($2.type_ptr,TYPE_BOOL)){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"WHILE expression DO statement\n");
        }
        // statement -> while expression do if_statement_1.
        $$ = new StatementNode(StatementNode::GrammarType::WHILE_STATEMENT);
        $$->append_child($2.expression_node);
        $$->append_child($4);

    } 
    | REPEAT statement_list UNTIL expression
    {
        if(error_flag)
            break;
        //类型检查
        if(!is_same($4.type_ptr,TYPE_BOOL)){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"REPEAT statement_list UNTIL expression\n");
        }
        // statement -> repeat statement_list until expression.
        $$ = new StatementNode(StatementNode::GrammarType::REPEAT_STATEMENT);
        $$->append_child($2);
        $$->append_child($4.expression_node);
    }
    | FOR ID ASSIGNOP expression updown expression DO statement
    {
        if(error_flag)
            break;
        //类型检查
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($2.value.get<string>());
        if((!is_basic(tmp->type()))||(!is_same(tmp->type(),$4.type_ptr))){
            yyerror(real_ast,"Type check failed\n");
                yyerror(real_ast,"FOR ID ASSIGNOP expression updown expression DO statement\n");
        }

        if((!is_same($4.type_ptr,$6.type_ptr))||(is_same($4.type_ptr,TYPE_REAL))){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"FOR ID ASSIGNOP expression updown expression DO statement\n");
        }
        // statement -> for id assignop expression updown expression do statement.
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
        if(error_flag)
            break;
        // statement -> empty.
        $$ = new StatementNode(StatementNode::GrammarType::EPSILON);
    }
    |READ '(' variable_list ')'
    {
        $3.variable_list_node->GetType($3.basic_types);
        if(error_flag)
            break;
        $$ = new StatementNode(StatementNode::GrammarType::READ_STATEMENT);
        $$->append_child($3.variable_list_node);
    }
    |WRITE '(' expression_list ')'
    {
        if(error_flag)
            break;
        if(!$3.expression_list_node->GetType($3.type_ptr_list)){
            yyerror(real_ast,"BasicType is expexted in WRITE\n");
        }
        $$ = new StatementNode(StatementNode::GrammarType::WRITE_STATEMENT);
        $$->append_child($3.expression_list_node);
    }
    |WRITELN'(' expression_list ')'
    {
        if(error_flag)
            break;
        if(!$3.expression_list_node->GetType($3.type_ptr_list)){
            yyerror(real_ast,"BasicType is expexted in WRITELN\n");
        }
        $$ = new StatementNode(StatementNode::GrammarType::WRITELN_STATEMENT);
        $$->append_child($3.expression_list_node);
    };

variable_list :
    variable
    { 
        $$.basic_types = new std::vector<BasicType*>();
        if($1.type_ptr != nullptr){
            if ($1.type_ptr==pascal_type::TYPE_INT ||
                $1.type_ptr==pascal_type::TYPE_CHAR ||
                $1.type_ptr==pascal_type::TYPE_REAL){
                $$.basic_types->push_back(dynamic_cast<BasicType*>($1.type_ptr));
            } else{
                yyerror(real_ast,"It should be INT BOOL or CHAR\n");
            }
            
        }
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE);
        $$.variable_list_node->append_child($1.variable_node);
    } | variable_list ',' variable{
        $$.basic_types = $1.basic_types;
        if($3.type_ptr != nullptr){
            if ($3.type_ptr==pascal_type::TYPE_INT ||
                $3.type_ptr==pascal_type::TYPE_CHAR ||
                $3.type_ptr==pascal_type::TYPE_REAL){
                $$.basic_types->push_back(dynamic_cast<BasicType*>($3.type_ptr));
            } else{
                yyerror(real_ast,"It should be INT BOOL or CHAR\n");
            }
        }
        if(error_flag)
            break;
        $$.variable_list_node = new VariableListNode(VariableListNode::GrammarType::VARIABLE_LIST_VARIABLE);
        $$.variable_list_node->append_child($1.variable_list_node);
        $$.variable_list_node->append_child($3.variable_node);
    };
variable:
    ID id_varparts
    {
        // variable -> id id_varparts.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<ObjectSymbol>($1.value.get<string>());
        string name = $1.value.get<string>();
        if(tmp == nullptr) {
            $$.type_ptr = nullptr;
            yyerror(real_ast,"variable not defined");
        } else {
            //类型检查
            // Convert to a detailed type
            $$.is_lvalue = true;
            if (pascal_symbol::ObjectSymbol::SYMBOL_TYPE::CONST == tmp->symbol_type()){
                tmp = dynamic_cast<ConstSymbol*>(tmp);
                $$.is_lvalue = false;
            } else if(pascal_symbol::ObjectSymbol::SYMBOL_TYPE::FUNCTION == tmp->symbol_type()){
                //TODO 函数调用 类型检查
                tmp = dynamic_cast<FunctionSymbol*>(tmp);
                name+="()";
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
                yyerror(real_ast,"Type check failed\n");
                yyerror(real_ast,"variable -> id id_varparts.\n");
            }
            if(tmp->is_ref()){
                name = "*("+name+")";
            }
            $$.name = new std::string($1.value.get<string>());
        }
        if(error_flag)
            break;
        $$.variable_node = new VariableNode();
        LeafNode *id_node = new LeafNode(name);
        $$.variable_node->append_child(id_node);
        $$.variable_node->append_child($2.id_varparts_node);
    };

id_varparts:
    {
        // id_varparts -> empty.
        $$.var_parts = new std::vector<VarParts>();
        if(error_flag)
            break;
        $$.id_varparts_node = new IDVarPartsNode();
    }
    | id_varparts id_varpart
    {
        // id_varparts -> id_varparts id_varpart.
        //if($$.var_parts)
        if($1.var_parts){
            $$.var_parts = $1.var_parts;
        } else {
            $$.var_parts = new std::vector<VarParts>();
        }
        
        $$.var_parts->push_back(*($2.var_part));
        if(error_flag)
            break;
        $$.id_varparts_node = new IDVarPartsNode();
        $$.id_varparts_node->append_child($1.id_varparts_node);
        $$.id_varparts_node->append_child($2.id_varpart_node);
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
        if(error_flag)
            break;
        // else_part -> empty.
        $$ = new ElseNode(ElseNode::GrammarType::EPSILON);
    }
    | ELSE statement 
    {
        if(error_flag)
            break;
        // else_part -> else statement.
        $$ = new ElseNode(ElseNode::GrammarType::ELSE_STATEMENT);
        $$->append_child($2);
    } ;
case_body:
    {
        // case_body -> empty.
        $$.type_ptr= nullptr;
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
        //todo 检查类型是否一致
        if(error_flag)
            break;        //对于某个branch_list，要求其内含的所有branch类型都一致，不需要存值
        if($1.type_ptr!=$3.type_ptr){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"branch_list -> branch_list branch.\n");
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
           yyerror(real_ast,"const_list type not match");
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
        if(error_flag)
            break;
        // updown -> to.
        $$ = new UpdownNode(true);
    }
    | DOWNTO
    {
        if(error_flag)
            break;
        // updown -> downto.
        $$ = new UpdownNode(false);
    };
call_procedure_statement:
    ID '(' expression_list ')'
    {
        //类型检查
        // call_procedure_statement -> id (expression_list).
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure");
        }
        if(!tmp->AssertParams(*($3.type_ptr_list),*($3.is_lvalue_list))){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"call_procedure_statement -> ID '(' expression_list ')'\n");
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
    };
    | ID
    {   
        //类型检查
        //todo 类型检查
        // call_procedure_statement -> id.
        ObjectSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            yyerror(real_ast,"call_procedure_statement: no such procedure\n");
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
        //类型检查 检查是否为INT or CHAR???
        $$.type_ptr_list = $1.type_ptr_list;
        $$.type_ptr_list->push_back($3.type_ptr);
        $$.is_lvalue_list = $1.is_lvalue_list;
        $$.is_lvalue_list->push_back($3.is_lvalue);
        // expression_list -> expression_list , expression.
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)1);
        $$.expression_list_node->append_child($1.expression_list_node);
        $$.expression_list_node->append_child($3.expression_node);
    }
    | expression
    {
        //类型检查 检查是否为INT or CHAR  ???  
        //这里应该是做不了的，但如果在声明的时候就有检查应该就不必做
        $$.type_ptr_list = new std::vector<pascal_type::TypeTemplate*>();
        $$.type_ptr_list->push_back($1.type_ptr);
        $$.is_lvalue_list = new std::vector<bool>();
        $$.is_lvalue_list->push_back($1.is_lvalue);
        // expression_list -> expression.
        if(error_flag)
            break;
        $$.expression_list_node = new ExpressionListNode((ExpressionListNode::GrammarType)0);
        $$.expression_list_node->append_child($1.expression_node);
    };
expression:
    simple_expression RELOP simple_expression
    {
        // 类型检查
        //从这里开始进行运算检查
        // expression -> simple_expression relop simple_expression.
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
            yyerror(real_ast,"Type check failed. Complex type in basic operation.\n");
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr, $2.value.get<string>());
        if(result==TYPE_ERROR){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"expression -> simple_expression relop simple_expression\n");
        }
        $$.is_lvalue = false;
        $$.type_ptr = result;
        
        std::string relop = $2.value.get<string>();
        if($2.value.get<string>() == "<>") {
            relop = "!=";
        }
        if(error_flag)
            break;
        $$.expression_node = new ExpressionNode();
        $$.expression_node->append_child($1.simple_expression_node);
        LeafNode *relop_node = new LeafNode(ConstValue(relop));
        $$.expression_node->append_child(relop_node);
        $$.expression_node->append_child($3.simple_expression_node);
    }
    | simple_expression '=' simple_expression
    {
        // 类型检查
        if((!is_basic($1.type_ptr))||(!is_basic($3.type_ptr))){
            yyerror(real_ast,"Type check failed. Complex type in basic operation.\n");
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr, "=");
        
        if(result==TYPE_ERROR){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"expression -> simple_expression '=' simple_expression\n");
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
        //std::cout<<$$.type_ptr<<std::endl;
        if(error_flag)
            break;
        if($$.type_ptr && $$.type_ptr->template_type() == pascal_type::TypeTemplate::TYPE::ARRAY) {
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
        $$.type_ptr = pascal_type::TYPE_STRINGLIKE;
        $$.length = $1.value.get<string>().length();
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.str_expression_node = new StrExpressionNode();
        LeafNode *string_node = new LeafNode($1.value);
        $$.str_expression_node->append_child(string_node);
    } | str_expression PLUS STRING_ {
        // str_expression -> str_expression + string.
        $$.type_ptr = pascal_type::TYPE_STRINGLIKE;
        $$.length = $1.length + $3.value.get<string>().length();
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.str_expression_node = new StrExpressionNode();
        $$.str_expression_node->append_child($1.str_expression_node);
        LeafNode *string_node = new LeafNode($3.value);
        $$.str_expression_node->append_child(string_node);
    };
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
        $$.type_ptr = $2.type_ptr;
        $$.is_lvalue = false;
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
        $$.type_ptr = $2.type_ptr;
        $$.is_lvalue = false;
        if(error_flag)
            break;
        $$.simple_expression_node = new SimpleExpressionNode();
        LeafNode *minus_node = new LeafNode(ConstValue("-"));
        $$.simple_expression_node->append_child(minus_node);
        $$.simple_expression_node->append_child($2.term_node);
    }
    | simple_expression ADDOP term
    {
        // simple_expression -> simple_expression or term.
        if($1.type_ptr!=$3.type_ptr){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"simple_expression -> simple_expression or term\n");
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
            yyerror(real_ast,"Type check failed. Complex type in basic operation.\n");
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,"+");
        if(result==TYPE_ERROR){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"expression -> simple_expression '+' simple_expression\n");
        }
        $$.type_ptr = result;

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
            yyerror(real_ast,"Type check failed. Complex type in basic operation.\n");
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,"-");
        if(result==TYPE_ERROR){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"expression -> simple_expression '-' simple_expression\n");
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
            yyerror(real_ast,"Type check failed. Complex type in basic operation.\n");
        }
        auto result=compute((BasicType*)$1.type_ptr, (BasicType*)$3.type_ptr,$2.value.get<string>());
        if(result==TYPE_ERROR){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"term -> term mulop factor\n");
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
    }
    |ID '(' expression_list ')'
    {
        // factor -> id (expression_list).
        // 类型检查
        FunctionSymbol *tmp = table_set_queue.top()->SearchEntry<FunctionSymbol>($1.value.get<string>());
        if(tmp == nullptr) {
            yyerror(real_ast,"factor -> no such procedure\n");
        }
        if(!tmp->AssertParams(*($3.type_ptr_list),*($3.is_lvalue_list))){
            yyerror(real_ast,"Type check failed\n");
            yyerror(real_ast,"call_procedure_statement -> ID '(' expression_list ')'\n");
        }
        $$.is_lvalue = false;
        $$.type_ptr = tmp->type();
        if(error_flag)
            break;
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
        //需要吗？
        $$.is_lvalue = false;
        $$.type_ptr = $2.type_ptr;
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
        //$$.unsigned_constant_var_node->append_child($1.const_variable_node);

//        if($1.type_ptr==pascal_type::TYPE_INT){
//            num_node = new LeafNode($1.value.m_INT);
//        } else {
//            num_node = new LeafNode($1.value.m_REAL);
//        }
        $$.unsigned_constant_var_node->append_child(num_node);
    };
    | CHAR
    {
        // unsigned_const_variable -> 'LETTER'
        $$.type_ptr = pascal_type::TYPE_CHAR;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *char_node = new LeafNode($1.value);
        $$.unsigned_constant_var_node->append_child(char_node);
    }
    |TRUE
    {
        // unsigned_const_variable -> true
        $$.type_ptr = pascal_type::TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *true_node = new LeafNode(ConstValue(true));
        $$.unsigned_constant_var_node->append_child(true_node);
    }
    | FALSE
    {   
        // unsigned_const_variable -> false
        $$.type_ptr = pascal_type::TYPE_BOOL;
        if(error_flag)
            break;
        $$.unsigned_constant_var_node = new UnsignConstVarNode();
        LeafNode *false_node = new LeafNode(ConstValue(false));
        $$.unsigned_constant_var_node->append_child(false_node);
    };

/*---------------.
| Error handler  |
`---------------*/
/*紧急恢复*/
program:error
    {
        location_pointer_refresh();
        new_line_flag=false;
        yyerror(real_ast,"There are unrecoverable errors. Please check the code carefully!");
        while (new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        fprintf(stderr,"abort!\n");
        while (yychar!= YYEOF){
            yychar = yylex();
        }        
    };
/*短语级恢复*/
    program_head: PROGRAM error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"An identifier is expected.");
        else
            yyerror(real_ast,"unknown error!");
        while (new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }; 

    const_declaration: ID '=' error  
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==TRUE||yychar==FALSE)
            yyerror(real_ast,"Const value cannot be Boolean.");
        else
            yyerror(real_ast,"unknown error!");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    }; 
     
    const_declaration: const_declaration ';' ID '=' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==TRUE||yychar==FALSE)
            yyerror(real_ast,"Const value cannot be Boolean.");
        else
            yyerror(real_ast,"unknown error!");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }; 

    type_declaration: type_declaration ';' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar=='=')
            yyerror(real_ast,"A type definition must begin with an identifier");
        else
            yyerror(real_ast,"unknown error!");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';')
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        else
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    }
    /*todo
    目前这是一种保底的恢复方法,这里会抛弃所有type定义;
    目标希望做到恢复到type_declaration级别;
    */
    type_declarations: TYPE  error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar=='=')
            yyerror(real_ast,"A type definition must begin with an identifier");
        else
            yyerror(real_ast,"There are unrecoverable errors in type_declarations. Please check the code carefully!");
        while (new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        while (yychar!= VAR && yychar!= YYEOF && yychar != FUNCTION && yychar!=PROCEDURE && yychar!=BEGIN_){
            yychar = yylex();
        }
    }

/*
    该产生式会导致program_body的FOLLOW集中出现error
    程序中只会出现一次program_body应该不会引起冲突
*/
program: program_head program_body error
    {
        location_pointer_refresh();
        table_set_queue.push(top_table_set);
        real_ast->libs()->Preset(table_set_queue.top()->symbols());
        yyerror(real_ast,"A dot is expected at the end of the program !");
        fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
        fprintf(stderr,"\t| %s",location_pointer);
    };

    var_declaration: id_list ':' error
    {
        location_pointer_refresh();
        new_line_flag=false;
        if(yychar==';')
            yyerror(real_ast,"A type identifier is expected here.");
        else
            yyerror(real_ast,"unknown error!");
        while (yychar!=';' && new_line_flag==false && yychar!= YYEOF){
            yychar = yylex();
        }
        if(yychar==';'){
            fprintf(stderr,"%d:\t| %s\n",line_count,cur_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
        else if(new_line_flag){
            fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
            fprintf(stderr,"\t| %s",location_pointer);
        }
    }
// program_head:  error  
//     {
//         location_pointer_refresh();
//         new_line_flag=false;
//         if(yychar==ID)
//             yyerror(real_ast,"Every program must begin with the symbol program!");
//         else
//             yyerror(real_ast,"unknown error!");
//         while (new_line_flag==false && yychar!= YYEOF){
//             yychar = yylex();
//         }
//         fprintf(stderr,"%d:\t| %s\n",line_count-1,last_line_info.c_str());
//         fprintf(stderr,"\t| %s",location_pointer);
//     };
 

// program_body: error
//     {
        
//     };

// const_declarations: CONST error
//     {

//     };

// type_declarations: TYPE error
//     {

//     };

// var_declarations: VAR error
//     {

//     };

// subprogram_declaration: FUNCTION error
//     {

//     };

// subprogram_declaration: PROCEDURE error
//     {

//     };  

// statement : error
//     {

//     };
/* A colon is expected. In declarations, the colon is followed by a type.*/
// var_declaration:
//     var_declaration ';' id_list error type_or_ID
//     {
//         yyerror(real_ast, "A colon is expected. In declarations, the colon is followed by a type.");
//     }
//     | var_declaration ';' error ':' type_or_ID
//     {
//         yyerror(real_ast, "An identifier is expected.");
//     }
//     | error ':' type_or_ID
//     {
//         yyerror(real_ast, "An identifier is expected.");
//     }
//     | id_list error type_or_ID
//     {
//         yyerror(real_ast, "A colon is expected. In declarations, the colon is followed by a type.");
//     };
// type_or_ID:
//     type
//     |ID;

// /* The symbol of is expected.*/
// statement:
//     CASE error END
//     {
//         yyerror(real_ast,"The symbol of is expected");
//     };
// type:
//     ARRAY '[' periods ']' error type
//     {
//         yyerror(real_ast,"The symbol of is expected");
//     };

// /* An opening parenthesis is expected.*/
// // program_head: PROGRAM ID error ';'
// //     {
// //         yyerror(real_ast,"An opening parenthesis is expected.");
// //     };
// // formal_parameter: error ')'
// //     {
// //         yyerror(real_ast,"An opening parenthesis is expected.");
// //     };
// statement:
//     READ error variable_list ')'
//     {
//         yyerror(real_ast,"An opening parenthesis is expected.");
//     }
//     | WRITE error expression_list ')'
//     {
//         yyerror(real_ast,"An opening parenthesis is expected.");
//     }
//     | WRITELN error expression_list ')'
//     {
//         yyerror(real_ast,"An opening parenthesis is expected.");
//     };
// // factor:error expression ')'
// //      {
// //           yyerror(real_ast,"An opening parenthesis is expected.");
// //      }
//    | ID error ')'
//    {
//        yyerror(real_ast,"An opening parenthesis is expected.");
//    }
//     ;

//     /* An opening bracket is expected ([).*/
// type: ARRAY error periods ']' OF type
//     {
//         yyerror(real_ast,"An opening bracket is expected ([).");
//     };

//     /* A closing bracket is expected (]).*/
// type: ARRAY '[' periods error OF type
//     {
//         yyerror(real_ast,"An closing bracket is expected (]).");
//     };
// // id_varpart: '[' expression_list error
// //     {
// //        yyerror(real_ast,"An closing bracket is expected (]).");
// //     };

//     /* A dot is expected at the end of the program. Check corresponding begin and end symbols!*/


//     /* Every program must begin with the symbol program.*/
// // program_head: error ID '(' id_list ')' ';'
// //     {
// //         table_set_queue.push(top_table_set);
// //         real_ast->libs()->Preset(table_set_queue.top()->symbols());
// //         yyerror(real_ast,"Every program must begin with the symbol program.");
// //     };

//     /* The symbol then is expected.*/
// // statement: IF error statement else_part
// //     {
// //         yyerror(real_ast,"The symbol then is expected.");
// //     };
//     /* The symbol until is expected.*/
// // statement: REPEAT statement_list error expression
// //     {
// //         yyerror(real_ast,"The symbol until is expected.");
// //     };

//     /* The symbol do is expected.*/
// statement: WHILE error statement
//     {
//         yyerror(real_ast,"The symbol do is expected.");
//     }
//     | FOR ID ASSIGNOP expression updown error statement
//     {
//         yyerror(real_ast,"The symbol do is expected.");
//     };

//     /* The symbol to (or downto) is expected.*/
// statement: FOR ID ASSIGNOP error expression DO statement
//     {
//         yyerror(real_ast,"The symbol to (or downto) is expected.");
//     };

//     /* The symbol begin is expected.*/
// // compound_statement: error statement_list END
// //     {
// //         yyerror(real_ast,"The symbol begin is expected.");
// //     };

//     /* The symbol and is expected.*/

//     /* The symbol := is expected. */
// statement: FOR ID error expression updown expression DO statement
//     {
//         yyerror(real_ast,"The symbol := is expected.");
//     };

// statement: ID error ';'
//     {
//         yychar = ';';
//         yyerror(real_ast,"Syntax error, ';' expected .");
//     };

%%
 

void yyerror(ast::AST* real_ast,const char *msg){
if(!yydebug)
    if(strcmp(msg,"syntax error")==0)
        return;

    fprintf(stderr,"%d,%ld:\033[01;31m \terror\033[0m : %s\n", line_count,cur_line_info.size(),msg);
        
    error_flag = 1;
    real_ast->set_root(nullptr);
}

void yynote(std::string msg ,int line){
    fprintf(stderr,"%d:\033[01;32m \tnote\033[0m : previous definition of \"%s\" was here\n", line, msg.c_str());
}

void yyerror_var(ast::AST* real_ast,int line){
    fprintf(stderr,"%d:\033[01;31m \terror\033[0m : %s\n", line, "redifinition of variable");
    error_flag = 1;
    real_ast->set_root(nullptr);
}

void location_pointer_refresh(){
    auto length = cur_line_info.size()-yyleng;
    memset(location_pointer,' ',length);
    location_pointer[length]='^';
    location_pointer[length+1]='\n';
}
// int main(){
//     AST *real_ast = new AST();
//     if(!yyparse(real_ast)&&error_flag){
//         printf("successful analysis\n");
//         Compiler compiler;
//         compiler.Compile(real_ast,"basic_test");
//     }
// }

int yywrap(){
    return 1;
}
