/* BEGIN_BANNER */
/*******************************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

       About:

       Originally implemented by Tanu Kanvar and Uday Khedker (http://www.cse.iitb.ac.in/~uday)
       for the courses cs302+cs316 Language Processors (theory and lab) at IIT Bombay.

       Initial release date Jan 15, 2013.

       Updated by N. Venkatesh in Jan 2019. The main updates included
       - Changing the input language from a basic-block-based GCC GIMPLE dump to a subset of C.
       - Replacing the flexc++ and bisonc++ scripts by flex and bison.
       - Supporting print operation for variables.
       
       Updated by Uday Khedker with the help of Nisha Biju and Saari Rajan in Jan 2020. The main
       updates include
       - Introduction of three address code (TAC) phase between abstract syntax tree (AST)
         and Register Transfer Language (RTL) which was originally called icode.
       - Introduction of string data type with constant strings.
       - Support for read operation for integer and floating point variables, and print
         operation for all variables (including string variables) and string constants. 
       - Significant changes in command line switches for printing each representation (tokens,
         AST, TAC, RTL, and assembly) as well as stopping after each phase (scanning, parsing,
	 AST construction (i.e. semantic analysis), TAC generation and RTL generation).
       - Substantial clean up of the code including removal of AST to icode translation,
         and interpretation (which was buggy and had not been tested thorougly after the front
	 end and the input langauge changed in 2019). 

       Copyrights reserved  by Uday Khedker. This  implemenation has been made available purely  
       for academic purposes without any warranty of any kind.

*******************************************************************************************************/
/* END_BANNER */

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include"tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"
#include"class.hh"

int Ast::labelCounter;

Ast::Ast()
{}

Ast::~Ast()
{}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::valid_operand()
{
	stringstream msg;
	msg << "No valid_operand() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


Ast * Ast::get_opd1()
{
	stringstream msg;
	msg << "No get_opd1() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Ast * Ast::get_opd2()
{
	stringstream msg;
	msg << "No get_opd2() function for " << typeid(*this).name();
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

TAC_For_Ast * Ast::gen_tac()
{
	stringstream msg;
	msg << "No gen_tac() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

TAC_Op Ast::get_tac_operator_for_rel_operator()
{
	stringstream msg;
	msg << "No get_tac_operator_for_rel_operator() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	node_data_type = void_data_type;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

void Assignment_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Mismatch between symbol entry of variable "+name+" and variable name");

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

void Name_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type dt, int line)
{
	constant = number;
	node_data_type = dt;

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
void Number_Ast<DATA_TYPE>::print_ast(ostream & file_buffer)
{
	file_buffer << "Num : " << constant;
}

///////////////////////////////////////////////////////////////////////////////

String_Constant_Ast::String_Constant_Ast(int key, Data_Type dt, int line)
{
	str_key = key;
	node_data_type = dt;

	lineno = line;
}

String_Constant_Ast::~String_Constant_Ast() 
{}

void String_Constant_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "String : \"" << program_object.get_string(str_key) << "\"";
}

Data_Type String_Constant_Ast::get_data_type()
{
	return node_data_type;
}

void String_Constant_Ast::set_data_type(Data_Type dt)
{
	node_data_type = dt;
}

bool String_Constant_Ast::is_null_string()
{
	string str_value = program_object.get_string(str_key);
	return str_value.empty();
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

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_data_type() == int_data_type || 
		            lhs->get_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in PLUS expression", line);

	CHECK_INPUT_AND_ABORT(l->get_data_type() == r->get_data_type(), "Type mismatch in PLUS expression", line);
	node_data_type = l->get_data_type();
	lineno = line;
}

void Plus_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: PLUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

Ast * Plus_Ast::get_opd1()	
{ 
	return lhs;
}

Ast * Plus_Ast::get_opd2()
{
	return rhs;
}

bool Plus_Ast::valid_operand()
{
	return true;
}

        
/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_data_type() == int_data_type || 
		            lhs->get_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in MINUS expression", line);


	CHECK_INPUT_AND_ABORT(l->get_data_type() == r->get_data_type(), "Type mismatch in MINUS expression", line);
	node_data_type = l->get_data_type();

	lineno = line;
}

void Minus_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MINUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_data_type() == int_data_type || 
		            lhs->get_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in MULT expression", line);

	CHECK_INPUT_AND_ABORT(l->get_data_type() == r->get_data_type(), "Type mismatch in MULT expression", line);
	node_data_type = l->get_data_type();

	lineno = line;
}

void Mult_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: MULT";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_data_type() == int_data_type || 
		            lhs->get_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in DIVIDE expression", line);


	CHECK_INPUT_AND_ABORT(l->get_data_type() == r->get_data_type(), "Type mismatch in DIVIDE expression", line);
	node_data_type = l->get_data_type();

	lineno = line;
}

void Divide_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: DIV";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	CHECK_INVARIANT(l!=NULL, "L operand for Unary Minus cannot be NULL");
	CHECK_INVARIANT(r==NULL, "R operand for Unary Minus must be NULL");
	bool correct_type = lhs->get_data_type() == int_data_type ||
		            lhs->get_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in UMINUS expression", line);

	node_data_type = l->get_data_type();

	lineno = line;
}

void UMinus_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Arith: UMINUS";

	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
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

	CHECK_INPUT_AND_ABORT(lhs->get_data_type() == rhs->get_data_type(), "Type mismatch in Relational expression", line);
	node_data_type = bool_data_type;

	lineno = line;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs_condition;
	delete rhs_condition;
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

void Relational_Expr_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: ";
	file_buffer << op_name();
	file_buffer << "\n" << AST_SUB_NODE_SPACE << "LHS (";
	lhs_condition->print_ast(file_buffer);
	file_buffer << ")";
	if (rhs_condition != NULL)
	{
		file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
		rhs_condition->print_ast(file_buffer);
		file_buffer << ")";
	}
}

string Relational_Expr_Ast::op_name()
{
	string op_string;

	switch(rel_op)
	{
		case less_equalto: op_string = "LE"; break;
		case less_than: op_string = "LT"; break;
		case greater_equalto: op_string = "GE"; break;
		case greater_than: op_string = "GT"; break;
		case equalto: op_string = "EQ"; break;
		case not_equalto: op_string = "NE"; break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Cannot recognize relational operator");
	}
	return op_string;

}
///////////////////////////////////////////////////////////////////////////////

TAC_Op Relational_Expr_Ast::get_tac_operator_for_rel_operator()
{
	TAC_Op tac_op;

	stringstream msg;
	msg << "Wrong type in rel->tac operator mapping for tac operator " << op_name();

	switch (rel_op)
	{
		case less_equalto:    tac_op = leq_3a;	break;
		case less_than:       tac_op = lt_3a;	break;
		case greater_than:    tac_op = gt_3a;	break;
		case greater_equalto: tac_op = geq_3a;	break;
		case equalto:         tac_op = eq_3a;	break;
		case not_equalto:     tac_op = neq_3a;	break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
	}
	return tac_op;
}

////////////////////////////////////////////////////////////////////////////--asgn 3

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line)
{
	this->cond = cond;
	lhs = l;
	rhs = r;

	CHECK_INPUT_AND_ABORT(cond->get_data_type() == bool_data_type, "Wrong type of Condition in Ternary Expression", line);
	CHECK_INPUT_AND_ABORT(l->get_data_type() == r->get_data_type(), "Type mismatch in Ternary expression", line);
	node_data_type = lhs->get_data_type();
	lineno = line;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast()
{
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

void Conditional_Expression_Ast::print_ast(ostream & file_buffer)
{
	//ADD CODE HERE
	file_buffer << "\n" << AST_SPACE << "Cond:";
	file_buffer << "\n" << AST_NODE_SPACE << "IF_ELSE";
	cond->print_ast(file_buffer);
	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")";
	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";
}

///////////////////////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line){

//get_new_label();
lineno = line;

}

Sequence_Ast::~Sequence_Ast()
{

}


void Sequence_Ast::print_ast(ostream & file_buffer)
{
	list<Ast *>::iterator t;
	for (t = seq_ast_statement_list.begin(); t != seq_ast_statement_list.end(); ++t) {
	file_buffer<<"\n"<<AST_NODE_SPACE;
	(*t)->print_ast(file_buffer);
}
}

void Sequence_Ast::ast_push_back(Ast * ast){
	
	seq_ast_statement_list.push_back (ast);}

////////////////////////////////////////////////////////////////////////////////////////

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line)
{
	this->cond = cond;
	this->then_part = then_part;
	this->else_part = else_part;
	lineno = line;

	node_data_type = cond->get_data_type();
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

void Selection_Statement_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "IF : ";
	file_buffer << "\n" << AST_SPACE << "CONDITION (";
	cond->print_ast(file_buffer);
	file_buffer << ")";
	file_buffer << "\n" << AST_SPACE << "THEN (";
	then_part->print_ast(file_buffer);
	file_buffer << ")";
	  if(else_part!=NULL) {
	file_buffer << "\n" << AST_SPACE << "ELSE (";
	else_part->print_ast(file_buffer);
	file_buffer << ")";
 }
}

////////////////////////////////////////////////////////////////////////////////////////

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form)
{
	this->cond = cond;
	this->body = body;
	lineno = line;
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

void Iteration_Statement_Ast::print_ast(ostream & file_buffer)
{
	if (is_do_form)
	{
		file_buffer << "\n" << AST_SPACE << "DO (";
		body->print_ast(file_buffer);
		file_buffer << ")";
		file_buffer << "\n" << AST_SPACE << "WHILE CONDITION (";
		cond->print_ast(file_buffer);
		file_buffer << ")";
	}
	else
	{
		file_buffer << "\n" << AST_SPACE << "WHILE : ";
		file_buffer << "\n" << AST_SPACE << "CONDITION (";
		cond->print_ast(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SPACE << "BODY (";
		body->print_ast(file_buffer);
		file_buffer << ")";
	}
}


Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line)
{
	lhs_op = lhs;
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;
	if (bop != not_op)
		CHECK_INPUT_AND_ABORT(lhs->get_data_type() == rhs->get_data_type(), "Type mismatch in Logical expression", line);
	CHECK_INPUT_AND_ABORT(lhs->get_data_type() == bool_data_type, "Wrong type for operand in Logical expression", line);
	
	node_data_type = bool_data_type;
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

void Logical_Expr_Ast::print_ast(ostream & file_buffer)
{
	string bop;
	switch(bool_op)
	{
		case and_op : bop = "AND"; break;
		case or_op : bop = "OR"; break;
		case not_op : bop = "NOT"; break;
	}
	file_buffer << "\n" << AST_NODE_SPACE << "Condition: " << bop << endl;
	if(lhs_op != NULL)
	{
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs_op->print_ast(file_buffer);
		file_buffer << ")";
	}
	if (rhs_op != NULL)
	{
		file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
		rhs_op->print_ast(file_buffer);
		file_buffer << ")";
	}
}

///////////////////////////////////////////////////////////////////////////////

TAC_Op Logical_Expr_Ast::get_tac_operator_for_logical_operator()
{
	TAC_Op tac_op;

	switch (bool_op)
	{
		case not_op: tac_op = not_3a; break;
		case or_op:  tac_op = or_3a; break;
		case and_op: tac_op = and_3a; break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong logical operator");
	}
	return tac_op;
}
////////////////////////////////////////////////////////////////////////////////////////
Write_Ast::Write_Ast(Ast *r, int line)
{
	lineno = line;
	var = r;
	CHECK_INVARIANT(r!=NULL, "Null AST Pointer in Write_Ast");
	Data_Type dt = r->get_data_type();
	CHECK_INPUT_AND_ABORT(dt!=bool_data_type, "A bool variable is not allowed in a Print Stmt", line);
}

Write_Ast::~Write_Ast()
{}

void Write_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Write :\n";
	file_buffer << AST_SUB_NODE_SPACE << "(";
	var->print_ast(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////////////////////////
Read_Ast::Read_Ast(Ast *r, int line)
{
	lineno = line;
	var = r;
	CHECK_INVARIANT(r!=NULL, "Null AST Pointer in Read_Ast");
	Data_Type dt = r->get_data_type();
	CHECK_INPUT_AND_ABORT(dt==int_data_type||dt==double_data_type, "Only Int and Float variables are allowed in a Read Stmt", line);
}

Read_Ast::~Read_Ast()
{}

void Read_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Read :\n";
	file_buffer << AST_SUB_NODE_SPACE << "(";
	var->print_ast(file_buffer);
	file_buffer << ")";
}


///////////////////////////////////////////////////////////////


Call_Ast::Call_Ast(string name, int line)
{
	procedure_name = name;
	lineno = line;

	Procedure * proc = program_object.get_procedure_prototype(procedure_name);
	CHECK_INPUT_AND_ABORT((proc != NULL), "Procedure prototype cannot be null", lineno);
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

void Call_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "FN CALL: ";

	file_buffer << procedure_name << "(";

	list<Ast *>::iterator i;
	for(i = actual_param_list.begin(); i != actual_param_list.end(); i++)
	{
		file_buffer << "\n" << AST_NODE_SPACE;
		(*i)->print_ast(file_buffer);
	}

	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * ret_val, string name, int line)
{
	lineno = line;
	node_data_type = void_data_type;

	return_value = ret_val;
	proc_name = name;

	if(return_value != NULL)
		node_data_type = ret_val->get_data_type();
	else
		node_data_type = void_data_type;

	CHECK_INPUT_AND_ABORT(node_data_type==(program_object.get_procedure_prototype(name))->get_return_type(), "Return type not matching",line);

}

Return_Ast::~Return_Ast()
{}

Data_Type Return_Ast::get_data_type()
{
	return node_data_type;
}

void Return_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "RETURN ";

	if (return_value != NULL)
		return_value->print_ast(file_buffer);
	else
		file_buffer << "<NOTHING>";

	file_buffer << "\n";
}
///////////////////////////////////////////////////////////////////////

Addr_Ast::Addr_Ast(Ast * o, int line)
{
	opd = o;
	lineno = line;

	CHECK_INVARIANT(o!=NULL, "Operand cannot be NULL for Addr_Ast");

	node_data_type = int_data_type;
}

Data_Type Addr_Ast::get_data_type()
{
	return node_data_type;
}


void Addr_Ast::print_ast(ostream & file_buffer)
{
       // print_spaces_on_new_line (sc, file_buffer);

	//file_buffer << "\n" << AST_SPACE ;
	file_buffer << "Address_Of";
	//file_buffer << "<" << printable_data_type(get_data_type()) << ">";

     //   print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);

	//file_buffer << "\n" << AST_SPACE ;
	file_buffer << "(";
	opd->print_ast(file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////////////

Deref_Ast::Deref_Ast(Ast * o, int line)
{
	opd = o;
	lineno = line;

	CHECK_INVARIANT(o!=NULL, "Operand cannot be NULL for Deref_Ast");

	// valid_operand(); 

	node_data_type = opd->get_data_type();
}

Data_Type Deref_Ast::get_data_type()
{
	return node_data_type;
}


void Deref_Ast::print_ast(ostream & file_buffer)
{
      //  print_spaces_on_new_line (sc, file_buffer);
	
	//file_buffer << "\n" << AST_SPACE ;
	file_buffer << "Deref";
      // file_buffer << "<" << printable_data_type(get_data_type()) << ">";

	//file_buffer << "\n" << AST_SPACE ;
       // print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "(";
	opd->print_ast(file_buffer);
	file_buffer << ")";
}


bool Deref_Ast::valid_operand()
{
	// Two possibilities
	// Either opd is a Name_Ast
        // Or we need to validate opd using the following four steps.
	// Step 1. opd must be a Plus_Ast
	// Step 2. get lopd and ropd of Plus_Ast
	// Step 3. lopd must be a Name_Ast
	// Step 4. ropd must be a Number_Ast and its data type should be int.
	// return tru/false appropriately.

	//bool child_is_deref = typeid(*opd) == typeid(Deref_Ast);
	//bool child_is_name = typeid(*opd) == typeid(Name_Ast);

	bool child_is_plus = typeid(*opd) == typeid(Plus_Ast);
	bool opd1_is_name;
	bool opd2_is_int;

	CHECK_INVARIANT( child_is_plus, "Wrong child of Deref Ast");

	
	
	if (child_is_plus)
	{
		Ast * lopd = opd->get_opd1();	
		Ast * ropd = opd->get_opd2();	
		opd1_is_name = typeid(*lopd) == typeid(Addr_Ast);
		opd2_is_int = typeid(*ropd) == typeid(Number_Ast<int>);

		if (opd1_is_name && opd2_is_int)
			return true;
		else if (opd1_is_name) 
		{
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong right operand in Plus Child of Deref Ast");
		}
		else if (opd2_is_int) 
		{
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong left operand in Plus Child of Deref Ast");
		}
		else
		{
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong left and right operand in Plus Child of Deref Ast");
		}
	}
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong child in Deref Ast");
	return false;
}

// get_opd(int n) function
// write a virtual function for Ast that says Function not defined
// override it in Plus_Ast ro return lhs if n is 1, rhs if n is 2


void Deref_Ast:: set_data_type(Data_Type dt)
{
   node_data_type = dt;
}

