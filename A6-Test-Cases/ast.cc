
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs316: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Initial release  date  Jan  15, 2013.  

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

int Ast::labelCounter;

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_value_zero()
{
	stringstream msg;
	msg << "No is_value_zero() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	/*else if (((typeid(*rhs) == typeid(Number_Ast<int>)) 
		|| (typeid(*rhs) == typeid(Number_Ast<double>)))
		&& rhs->is_value_zero())
	// value 0 is treated both as float and as int
	{
		node_data_type = lhs->get_data_type();
		return true;
	}*/	

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible", lineno);
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

	ast_num_child = zero_arity;
	node_data_type = void_data_type;
	lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt)
{
	variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

template <class DATA_TYPE>
bool Number_Ast<DATA_TYPE>::is_value_zero()
{
	if (constant == 0 || constant == 0.0)
		return true;

	return false;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << "Num : " << constant;
}

///////////////////////////////////////////////////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	if (rhs == NULL)
	{
		CHECK_INPUT((ast_num_child == unary_arity), "Only unary operator can have null right child", lineno);

		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	/*else if (((typeid(*rhs) == typeid(Number_Ast<int>)) 
		|| (typeid(*rhs) == typeid(Number_Ast<double>))) 
		&& rhs->is_value_zero())
	// value 0 is treated both as float and as int
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	else if (((typeid(*lhs) == typeid(Number_Ast<int>)) 
		|| (typeid(*lhs) == typeid(Number_Ast<double>))) 
		&& lhs->is_value_zero())
	// value 0 is treated both as float and as int
	{
		node_data_type = rhs->get_data_type();
		return true;
	}*/

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Arithmetic statement data type not compatible", lineno);
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

void Plus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: PLUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

void Minus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MINUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

void Mult_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MULT";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

void Divide_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: DIV";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	ast_num_child = unary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: UMINUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
///////////////////////////////////////////////////////////////////////////////

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line)
{
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	node_data_type = int_data_type;

	ast_num_child = binary_arity;

	lineno = line;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs_condition;
	delete rhs_condition;
}

bool Relational_Expr_Ast::check_ast()
{
	CHECK_INVARIANT((lhs_condition != NULL), "Lhs of Relational_Expr_Ast cannot be null");
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs of Relational_Expr_Ast cannot be null");

	if (lhs_condition->get_data_type() == rhs_condition->get_data_type())
	{
		node_data_type = int_data_type;  //0 or 1 assumed int type
		return true;
	}

	/*else if (((typeid(*rhs_condition) == typeid(Number_Ast<int>)) 
		|| (typeid(*rhs_condition) == typeid(Number_Ast<double>))) 
		&& rhs_condition->is_value_zero())
	// value 0 is treated both as float and as int
	{
		node_data_type = int_data_type;
		return true;
	}

	else if (((typeid(*lhs_condition) == typeid(Number_Ast<int>)) 
		|| (typeid(*lhs_condition) == typeid(Number_Ast<double>))) 
		&& lhs_condition->is_value_zero())
	// value 0 is treated both as float and as int
	{
		node_data_type = int_data_type;
		return true;
	}*/

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Relational statement data type not compatible", lineno);
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return node_data_type;
}

//redundant
void Relational_Expr_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: ";
	switch(rel_op)
	{
	case less_equalto: file_buffer << "LE";
				break;
	case less_than: file_buffer << "LT";
				break;
	case greater_equalto: file_buffer << "GE";
				break;
	case greater_than: file_buffer << "GT";
				break;
	case equalto: file_buffer << "EQ";
				break;
	case not_equalto: file_buffer << "NE";
				break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Cannot recognize relational operator");
	}

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs_condition->print(file_buffer);
	file_buffer << ")";
}
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////--asgn 3
Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line)
{
	this->cond = cond;
	lhs = l;
	rhs = r;
	ast_num_child = ternary_arity;
	node_data_type = lhs->get_data_type();
	get_new_label();
	lineno = line;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast()
{
	//ADD CODE HERE
	delete cond;

}

/*Data_Type Conditional_Expression_Ast::get_data_type()
{
  return node_data_type;

}
void Conditional_Expression_Ast::set_data_type(Data_Type dt)
{
    node_data_type = dt;
}*/

void Conditional_Expression_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << "\n" << AST_SPACE << "Cond:";
	file_buffer << "\n" << AST_NODE_SPACE << "IF_ELSE";
	cond->print(file_buffer);
	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";
	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line){

get_new_label();
lineno = line;

}

Sequence_Ast::~Sequence_Ast()
{

}


void Sequence_Ast::print(ostream & file_buffer)
{
	list<Ast *>::iterator t;
	for (t = statement_list.begin(); t != statement_list.end(); ++t) {
	file_buffer<<"\n"<<AST_NODE_SPACE;
	(*t)->print(file_buffer);
}
}

void Sequence_Ast::ast_push_back(Ast * ast){
	
	statement_list.push_back (ast);}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line)
{
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	lineno = line;
	node_data_type = cond->get_data_type();
	ast_num_child = binary_arity;
}

Selection_Statement_Ast::~Selection_Statement_Ast()
{
	delete cond;
	delete then_part;
	delete else_part;
}

Data_Type Selection_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt)
{

	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast()
{
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Boolean statement data type not compatible", lineno);
}

void Selection_Statement_Ast::print(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << "\n" << AST_SPACE << "IF : ";
	file_buffer << "\n" << AST_SPACE << "CONDITION (";
	cond->print(file_buffer);
	file_buffer << ")";
	file_buffer << "\n" << AST_SPACE << "THEN (";
	then_part->print(file_buffer);
	file_buffer << ")";
	  if(else_part!=NULL) {
	file_buffer << "\n" << AST_SPACE << "ELSE (";
	else_part->print(file_buffer);
	file_buffer << ")";
 }
}

////////////////////////////////////////////////////////////////////////////////////////

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
	this->cond = cond;
	this->body = body;
	lineno = line;
	ast_num_child = binary_arity;
	is_do_form = do_form;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast()
{
	delete cond;
	delete body;
}

Data_Type Iteration_Statement_Ast::get_data_type()
{
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast()
{
	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Boolean statement data type not compatible", lineno);
}

void Iteration_Statement_Ast::print(ostream & file_buffer)
{
	if (is_do_form)
	{
		file_buffer << "\n" << AST_SPACE << "DO (";
		body->print(file_buffer);
		file_buffer << ")";
		file_buffer << "\n" << AST_SPACE << "WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
	}
	else
	{
		file_buffer << "\n" << AST_SPACE << "WHILE : ";
		file_buffer << "\n" << AST_SPACE << "CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "BODY (";
		body->print(file_buffer);
		file_buffer << ")";
	}
}


Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs,Logical_Op bop, Ast * rhs, int line)
{
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	node_data_type = int_data_type;
	ast_num_child = binary_arity;
}

Logical_Expr_Ast::~Logical_Expr_Ast()
{
	if (lhs_op != NULL)
		delete lhs_op;
	delete rhs_op;
}

Data_Type Logical_Expr_Ast::get_data_type()
{
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt)
{
	
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast()
{
       if(bool_op!=_logical_not)
	CHECK_INVARIANT((lhs_op != NULL), "Lhs of Logical_Expr_Ast cannot be null");
	
	CHECK_INVARIANT((rhs_op != NULL), "Rhs of Logical_Expr_Ast cannot be null");

   if(bool_op != _logical_not) {
	if (lhs_op->get_data_type() == int_data_type && rhs_op->get_data_type()==int_data_type)
	{
		node_data_type = int_data_type;  //0 or 1 assumed int type
		return true;
	}
	}
   else {
   	if (rhs_op->get_data_type()==int_data_type)
	{
		node_data_type = int_data_type;  //0 or 1 assumed int type
		return true;
	}
   }

	CHECK_INPUT(CONTROL_SHOULD_NOT_REACH, "Boolean statement data type not compatible", lineno);
}

void Logical_Expr_Ast::print(ostream & file_buffer)
{
	string bop;
	switch(bool_op)
	{
		case _logical_and : bop = "AND"; break;
		case _logical_or : bop = "OR"; break;
		case _logical_not : bop = "NOT"; break;
	}
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << bop << endl;
	if(lhs_op != NULL)
	{
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")\n";
	}
	file_buffer << AST_SUB_NODE_SPACE << "RHS (";
	rhs_op->print(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////////////////////////
Print_Ast::Print_Ast(Ast *r, int line)
{
	lineno = line;
	var = r;
	//node_data_type = void_data_type;
	ast_num_child = unary_arity;
}
Print_Ast::~Print_Ast()
{}

void Print_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Print :\n";
	file_buffer << AST_SUB_NODE_SPACE << "(";
	var->print(file_buffer);
	file_buffer << ")";
}

///////////////////////////////////////////////////////////////


Call_Ast::Call_Ast(string name, int line)
{
	procedure_name = name;
	lineno = line;

	Procedure * proc = program_object.get_procedure_prototype(procedure_name);
	CHECK_INPUT((proc != NULL), "Procedure prototype cannot be null", lineno);
	node_data_type = proc->get_return_type();
	return_value_reg = NULL;
}

Call_Ast::~Call_Ast()
{}

void Call_Ast::set_register(Register_Descriptor * reg)
{
	return_value_reg = reg;
}

Data_Type Call_Ast::get_data_type()
{
	return node_data_type;
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list)
{
	actual_param_list = param_list;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_list)
{
	list<Symbol_Table_Entry *> & formal_param_list = formal_list.get_table();

	list<Ast *>::iterator i;
	list<Symbol_Table_Entry *>::iterator j;

	for (i = actual_param_list.begin(), j = formal_param_list.begin(); i != actual_param_list.end() && j != formal_param_list.end(); i++, j++)
	{
		Data_Type type1 = (*i)->get_data_type();
		Data_Type type2 = (*j)->get_data_type();

		CHECK_INPUT((type1 == type2), "Actual and formal parameters data types are not matching", lineno);
	}

	CHECK_INPUT((j == formal_param_list.end() && i == actual_param_list.end()),
			"Actual and formal parameter count do not match", lineno);
}

void Call_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "FN CALL: ";

	file_buffer << procedure_name << "(";

	list<Ast *>::iterator i;
	for(i = actual_param_list.begin(); i != actual_param_list.end(); i++)
	{
		file_buffer << "\n" << AST_NODE_SPACE;
		(*i)->print(file_buffer);
	}

	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * ret_val, string name, int line)
{
	lineno = line;
	node_data_type = void_data_type;
	ast_num_child = zero_arity;

	return_value = ret_val;
	proc_name = name;

	if(return_value != NULL)
		node_data_type = ret_val->get_data_type();
	else
		node_data_type = void_data_type;

CHECK_INVARIANT( node_data_type==(program_object.get_procedure_prototype(name))->get_return_type(), "Return type not matching");

}

Return_Ast::~Return_Ast()
{}

Data_Type Return_Ast::get_data_type()
{
	return node_data_type;
}

void Return_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "RETURN ";

	if (return_value != NULL)
		return_value->print(file_buffer);
	else
		file_buffer << "<NOTHING>";

	file_buffer << "\n";
}
