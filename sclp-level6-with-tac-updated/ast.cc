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

#include "common-headers.hh"

int Ast::labelCounter;

Ast::Ast()
{}

Ast::~Ast()
{}

int Ast::count_indirection_level()
{
#if 0
	stringstream msg;
	msg << "No count_indirection_level() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
#endif
	return 0;
}

bool Ast::is_a_scalar()
{
	stringstream msg;
	msg << "No is_a_scalar() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Ast::is_an_array()
{
	return false;
}

bool Ast::is_a_pointer()
{
	return false;
}

Basic_Data_Type Ast::get_basic_data_type()
{
	stringstream msg;
	msg << "No get_basic_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Ast::get_lineno()
{
	return lineno;
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

TAC_For_Ast * Ast::gen_tac_aggregate()
{
	stringstream msg;
	msg << "No gen_tac_aggregate() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_data_type(Basic_Data_Type dt)
{
	stringstream msg;
	msg << "No set_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

TAC_Op Ast::get_tac_operator_for_rel_op_in_ast()
{
	stringstream msg;
	msg << "No get_tac_operator_for_rel_op_in_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Ast * Ast::get_offset_ast()
{
	stringstream msg;
	msg << "No get_offset_ast() function for " << typeid(*this).name();
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

void Assignment_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
        file_buffer << "Asgn:";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "LHS (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "RHS (";
	rhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Mismatch between symbol entry of variable "+name+" and variable name");

	node_data_type = variable_symbol_entry->get_basic_data_type();
	lineno = line;
}

Name_Ast::~Name_Ast()
{}

Basic_Data_Type Name_Ast::get_basic_data_type()
{
	return variable_symbol_entry->get_basic_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Basic_Data_Type dt)
{
	variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print_ast(int sc, ostream & file_buffer)
{
	// Ignore the space count sc
	
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
	
	if(printable_data_type(variable_symbol_entry->get_basic_data_type()) == "class")
	{
		Class_Type_Expr * cte = variable_symbol_entry->get_class_type_info();
		
		file_buffer << "<" << printable_data_type(variable_symbol_entry->get_basic_data_type())<<" - "<<cte->get_class_name() << ">";
	}
	else
	file_buffer << "<" << printable_data_type(variable_symbol_entry->get_basic_data_type()) << ">";
}

Ast * Name_Ast::get_offset_ast()
{
	return NULL;
}
	
bool Name_Ast::is_a_scalar()
{
	Symbol_Table_Entry & se = get_symbol_entry();
	return se.is_a_scalar();
}

bool Name_Ast::is_an_array()
{
	Symbol_Table_Entry & se = get_symbol_entry();
	Decl_Type_Info * declared_type_info = se.get_decl_type_info();
	Type_Expr * declared_type_expr = declared_type_info->get_derived_type();
	return	declared_type_expr->is_array_type();
}

bool Name_Ast::is_a_pointer()
{
	Symbol_Table_Entry & se = get_symbol_entry();
	Decl_Type_Info * declared_type_info = se.get_decl_type_info();
	Type_Expr * declared_type_expr = declared_type_info->get_derived_type();
	return	declared_type_expr->is_pointer_type();
}

int Name_Ast::count_indirection_level()
{
	if (is_a_scalar())
		return 0;
	else
	{
		Symbol_Table_Entry & se = get_symbol_entry();
		Decl_Type_Info * ti = se.get_decl_type_info();
		Type_Expr * te = ti->get_derived_type();
		if (te->is_pointer_type())
			return ti->get_max_depth();
		else 
			return 1;
	}
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Basic_Data_Type dt, int line)
{
	constant = number;
	node_data_type = dt;

	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Basic_Data_Type Number_Ast<DATA_TYPE>::get_basic_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::set_data_type(Basic_Data_Type dt)
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
void Number_Ast<DATA_TYPE>::print_ast(int sc, ostream & file_buffer)
{
	// Ignore the space count sc
	
	file_buffer << "Num : " << constant;
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";
}

///////////////////////////////////////////////////////////////////////////////

String_Constant_Ast::String_Constant_Ast(int key, Basic_Data_Type dt, int line)
{
	str_key = key;
	node_data_type = dt;

	lineno = line;
}

String_Constant_Ast::~String_Constant_Ast() 
{}

void String_Constant_Ast::print_ast(int sc, ostream & file_buffer)
{
	// Ignore the space count sc
	
	file_buffer << "String : \"" << program_object.get_string(str_key) << "\"";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";
}

Basic_Data_Type String_Constant_Ast::get_basic_data_type()
{
	return node_data_type;
}

void String_Constant_Ast::set_data_type(Basic_Data_Type dt)
{
	node_data_type = dt;
}

bool String_Constant_Ast::is_null_string()
{
	string str_value = program_object.get_string(str_key);
	return str_value.empty();
}

///////////////////////////////////////////////////////////////////////////////

Basic_Data_Type Arithmetic_Expr_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Basic_Data_Type dt)
{
	node_data_type = dt;
}

/////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in PLUS expression", line);

	CHECK_INPUT_AND_ABORT(l->get_basic_data_type() == r->get_basic_data_type(), "Type mismatch in PLUS expression", line);
	node_data_type = l->get_basic_data_type();
	lineno = line;
}

void Plus_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Arith: Plus";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "R_Opd (";
	rhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

/////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in MINUS expression", line);


	CHECK_INPUT_AND_ABORT(l->get_basic_data_type() == r->get_basic_data_type(), "Type mismatch in MINUS expression", line);
	node_data_type = l->get_basic_data_type();

	lineno = line;
}

void Minus_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Arith: Minus";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "R_Opd (";
	rhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in MULT expression", line);

	CHECK_INPUT_AND_ABORT(l->get_basic_data_type() == r->get_basic_data_type(), "Type mismatch in MULT expression", line);
	node_data_type = l->get_basic_data_type();

	lineno = line;
}

void Mult_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Arith: Mult";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "R_Opd (";
	rhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in DIVIDE expression", line);


	CHECK_INPUT_AND_ABORT(l->get_basic_data_type() == r->get_basic_data_type(), "Type mismatch in DIVIDE expression", line);
	node_data_type = l->get_basic_data_type();

	lineno = line;
}

void Divide_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Arith: Div";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "R_Opd (";
	rhs->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

//////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line)
{
	lhs = l;
	rhs = r;

	CHECK_INVARIANT(l!=NULL, "L operand for Unary Minus cannot be NULL");
	CHECK_INVARIANT(r==NULL, "R operand for Unary Minus must be NULL");
	bool correct_type = lhs->get_basic_data_type() == int_data_type ||
		            lhs->get_basic_data_type() == double_data_type;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in UMINUS expression", line);

	node_data_type = l->get_basic_data_type();

	lineno = line;
}

void UMinus_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Arith: Uminus";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs->print_ast(sc+2*INDENT_INCR, file_buffer);
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

	/*
	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type ||
		            (rop == equalto && lhs->get_basic_data_type() == bool_data_type) ||
		            (rop == not_equalto && lhs->get_basic_data_type() == bool_data_type)
			    ;
	*/
	bool correct_type = lhs->get_basic_data_type() == int_data_type || 
		            lhs->get_basic_data_type() == double_data_type
			    ;

	CHECK_INPUT_AND_ABORT(correct_type, "Wrong type of operand in Relational expression", line);
	CHECK_INPUT_AND_ABORT(lhs->get_basic_data_type() == rhs->get_basic_data_type(), "Type mismatch between operands in Relational expression", line);
	node_data_type = bool_data_type;

	lineno = line;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs_condition;
	delete rhs_condition;
}

Basic_Data_Type Relational_Expr_Ast::get_basic_data_type()
{
	return node_data_type;
}

//redundant
void Relational_Expr_Ast::set_data_type(Basic_Data_Type dt)
{
	node_data_type = dt;
}

void Relational_Expr_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Condition: ";
	file_buffer << op_name();
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "L_Opd (";
	lhs_condition->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
	if (rhs_condition != NULL)
	{
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "R_Opd (";
		rhs_condition->print_ast(sc+2*INDENT_INCR, file_buffer);
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

TAC_Op Relational_Expr_Ast::get_tac_operator_for_rel_op_in_ast()
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

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, Symbol_Table_Entry * se, int line)
{
	this->cond = cond;
	lhs = l;
	rhs = r;
	result_temp_symbol = se;

	CHECK_INPUT_AND_ABORT(cond->get_basic_data_type() == bool_data_type, "Wrong type of Condition in Ternary Expression", cond->get_lineno());
	CHECK_INPUT_AND_ABORT(l->get_basic_data_type() == r->get_basic_data_type(), "Type mismatch in Ternary expression", line);
	node_data_type = lhs->get_basic_data_type();
	lineno = line;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast()
{
	delete cond;

}

Symbol_Table_Entry * Conditional_Expression_Ast::get_result_sym_entry()
{
        return result_temp_symbol;
}
/*
void Conditional_Expression_Ast::set_data_type(Basic_Data_Type dt)
{
    node_data_type = dt;
}*/

void Conditional_Expression_Ast::print_ast(int sc, ostream & file_buffer)
{
	cond->print_ast(sc+2*INDENT_INCR, file_buffer);
	//file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+2*INDENT_INCR, file_buffer);
	file_buffer << "True_Part (";
	lhs->print_ast(sc+3*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+2*INDENT_INCR, file_buffer);
	file_buffer  << "False_Part (";
	rhs->print_ast(sc+3*INDENT_INCR, file_buffer);
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


void Sequence_Ast::print_ast(int sc, ostream & file_buffer)
{
	list<Ast *>::iterator t;
	for (t = seq_ast_statement_list.begin(); t != seq_ast_statement_list.end(); ++t) 
		(*t)->print_ast(sc, file_buffer);
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

	CHECK_INPUT_AND_ABORT(cond->get_basic_data_type() == bool_data_type, "Wrong type of Condition in IF statement", cond->get_lineno());

	node_data_type = cond->get_basic_data_type();
}

Selection_Statement_Ast::~Selection_Statement_Ast()
{
	delete cond;
	delete then_part;
	delete else_part;
}

Basic_Data_Type Selection_Statement_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Basic_Data_Type dt)
{
	node_data_type = dt;
}

void Selection_Statement_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "If: ";
        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Condition (";
	cond->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Then (";
	then_part->print_ast(sc+2*INDENT_INCR,file_buffer);
	file_buffer << ")";

	if(else_part!=NULL) 
	{
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "Else (";
		else_part->print_ast(sc+2*INDENT_INCR,file_buffer);
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

	CHECK_INPUT_AND_ABORT(cond->get_basic_data_type() == bool_data_type, "Wrong type of Condition in WHILE statement", cond->get_lineno());
}

Iteration_Statement_Ast::~Iteration_Statement_Ast()
{
	delete cond;
	delete body;
}

Basic_Data_Type Iteration_Statement_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Basic_Data_Type dt)
{
	node_data_type = dt;
}

void Iteration_Statement_Ast::print_ast(int sc, ostream & file_buffer)
{
	if (is_do_form)
	{
        	print_spaces_on_new_line (sc, file_buffer);
		file_buffer << "Do:";
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "Body (";
		body->print_ast(sc+2*INDENT_INCR,file_buffer);
		file_buffer << ")";
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "While Condition (";
		cond->print_ast(sc+2*INDENT_INCR,file_buffer);
		file_buffer << ")";
	}
	else
	{
        	print_spaces_on_new_line (sc, file_buffer);
		file_buffer << "While: ";
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "Condition (";
		cond->print_ast(sc+2*INDENT_INCR,file_buffer);
		file_buffer << ")";
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "Body (";
		body->print_ast(sc+2*INDENT_INCR,file_buffer);
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
		CHECK_INPUT_AND_ABORT(lhs->get_basic_data_type() == rhs->get_basic_data_type(), "Type mismatch in Logical expression", line);
	CHECK_INPUT_AND_ABORT(lhs->get_basic_data_type() == bool_data_type, "Wrong type for operand in Logical expression", line);
	
	node_data_type = bool_data_type;
}

Logical_Expr_Ast::~Logical_Expr_Ast()
{
	if (lhs_op != NULL)
		delete lhs_op;
	delete rhs_op;
}

Basic_Data_Type Logical_Expr_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Basic_Data_Type dt)
{
	
	node_data_type = dt;
}

void Logical_Expr_Ast::print_ast(int sc, ostream & file_buffer)
{
	string bop;
	switch(bool_op)
	{
		case and_op : bop = "AND"; break;
		case or_op : bop = "OR"; break;
		case not_op : bop = "NOT"; break;
	}
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Condition: " << bop;
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";
	if(lhs_op != NULL)
	{
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer  << "L_Opd (";
		lhs_op->print_ast(sc+2*INDENT_INCR, file_buffer);
		file_buffer << ")";
	}
	if (rhs_op != NULL)
	{
        	print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
		file_buffer << "R_Opd (";
		rhs_op->print_ast(sc+2*INDENT_INCR, file_buffer);
		file_buffer << ")";
	}
}

///////////////////////////////////////////////////////////////////////////////

TAC_Op Logical_Expr_Ast::get_tac_operator_for_logical_op_in_ast()
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
	Basic_Data_Type dt = r->get_basic_data_type();
	CHECK_INPUT_AND_ABORT(dt!=bool_data_type, "A bool variable is not allowed in a Print Stmt", line);
}

Write_Ast::~Write_Ast()
{}

void Write_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Write: ";
	var->print_ast(sc+INDENT_INCR, file_buffer);
}

////////////////////////////////////////////////////////////////////////////////////////
Read_Ast::Read_Ast(Ast *r, int line)
{
	lineno = line;
	var = r;
	CHECK_INVARIANT(r!=NULL, "Null AST Pointer in Read_Ast");
	Basic_Data_Type dt = r->get_basic_data_type();
	CHECK_INPUT_AND_ABORT(dt==int_data_type||dt==double_data_type, "Only Int and Float variables are allowed in a Read Stmt", line);
}

Read_Ast::~Read_Ast()
{}

void Read_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Read: ";
	var->print_ast(sc+INDENT_INCR, file_buffer);
}


///////////////////////////////////////////////////////////////


Call_Ast::Call_Ast(string name, int line)
{
	procedure_name = name;
	lineno = line;

	Procedure * proc = program_object.get_procedure_entry(procedure_name);
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

Basic_Data_Type Call_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list)
{
	actual_param_list = param_list;
}

void Call_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "FN CALL: ";

	file_buffer << procedure_name << "(";

	list<Ast *>::iterator i;
	for(i = actual_param_list.begin(); i != actual_param_list.end(); i++)
	{
        	print_spaces_on_new_line (sc, file_buffer);
		(*i)->print_ast(sc+INDENT_INCR, file_buffer);
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
		node_data_type = ret_val->get_basic_data_type();
	else
		node_data_type = void_data_type;

	CHECK_INPUT_AND_ABORT(node_data_type==(program_object.get_procedure_entry(name))->get_return_type(), "Return type does not match",line);

}

Return_Ast::~Return_Ast()
{}

Basic_Data_Type Return_Ast::get_basic_data_type()
{
	return node_data_type;
}

void Return_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Return: ";

	if (return_value != NULL)
		return_value->print_ast(sc+INDENT_INCR, file_buffer);
	else
		file_buffer << "<NOTHING>";
}

void Ast::print_spaces_on_new_line (int sc, ostream & file_buffer)
{
	file_buffer << "\n";
	for (int i=0; i<sc; i++)
		file_buffer << " ";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

Address_Of_Ast::Address_Of_Ast(Ast * bn)
{
	CHECK_INVARIANT(bn!=NULL, "Base of Address_Of AST cannot be NULL");

	base = bn;
}

Address_Of_Ast::~Address_Of_Ast() { }

Basic_Data_Type Address_Of_Ast::get_basic_data_type()
{
	return base->get_basic_data_type();
}
	
void Address_Of_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Address_Of: ";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Base (";
	base->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

Ast * Address_Of_Ast::get_base_ast()
{
	return base;
}
	
int Address_Of_Ast::count_indirection_level()
{
	CHECK_INVARIANT ((typeid(*base) == typeid(Name_Ast)), "Wrong type of child in Address_Of Ast");
	int base_ind = base->count_indirection_level() ;

	// cout << "Base Ind in Addr_ASt = " << base_ind << endl; TEST
	// base->print_ast(20,cout); TEST
	return base_ind + 1;
}

bool Address_Of_Ast::is_a_scalar()
{
	return false;
}

Symbol_Table_Entry & Address_Of_Ast::get_symbol_entry()
{
	CHECK_INVARIANT(base != NULL, "Base cannot be NULL in Array AST");
	CHECK_INVARIANT((typeid(*base) == typeid(Name_Ast)), "Wrong type of child in Address_Of Ast");
	return base->get_symbol_entry();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

Array_Access_Ast::Array_Access_Ast(Ast * bn,  Ast * off)
{
	CHECK_INVARIANT(bn!=NULL, "Base Ast of Array_Access AST cannot be NULL");
	CHECK_INVARIANT(off!=NULL, "Offset Ast of Array_Access AST cannot be NULL");
	CHECK_INVARIANT(typeid(*bn)==typeid(Name_Ast), "Base of Array_Access AST can only be a Name_Ast");

	base = bn;
	offset = off;
}

Array_Access_Ast::~Array_Access_Ast() { }

Basic_Data_Type Array_Access_Ast::get_basic_data_type()
{
	return base->get_basic_data_type();
}
	
void Array_Access_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Array_Access: ";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Base (";
	base->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Offset (";
	offset->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

Ast * Array_Access_Ast::get_base_ast()
{
	return base;
}

Ast * Array_Access_Ast::get_offset_ast()
{
	return offset;
}
	
int Array_Access_Ast::count_indirection_level()
{
	// In case of arrays, the indirection level actually represents 
	// (the number of dimensions - 1). The idea is that if there are K dimensions,
	// when all the dimensions are present in an array access, we get the base value
	// whose indirection level is 0.
	//
	
	CHECK_INVARIANT((typeid(*base) == typeid(Name_Ast)), "Wrong type of child in Array_Access_Ast Ast");
	int base_ind_level = base->count_indirection_level();

	return base_ind_level - 1;
}

bool Array_Access_Ast::is_a_scalar()
{
	return false;
}

bool Array_Access_Ast::is_an_array()
{
	return true;
}


Symbol_Table_Entry & Array_Access_Ast::get_symbol_entry()
{
	CHECK_INVARIANT(base != NULL, "Base cannot be NULL in Array AST");
	CHECK_INVARIANT((typeid(*base) == typeid(Name_Ast)), "Wrong type of child in Array_Access_Ast Ast");
	return base->get_symbol_entry();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

Pointer_Deref_Ast::Pointer_Deref_Ast(Ast * bn)
{
	CHECK_INVARIANT(bn!=NULL, "Base Ast of Pointer_Deref_AST cannot be NULL");

	opd = bn;
}

Pointer_Deref_Ast::~Pointer_Deref_Ast() { }

Basic_Data_Type Pointer_Deref_Ast::get_basic_data_type()
{
	return opd->get_basic_data_type();
}
	
void Pointer_Deref_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Pointer_Deref: ";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Operand (";
	opd->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

Ast * Pointer_Deref_Ast::get_opd_ast()
{
	return opd;
}

int Pointer_Deref_Ast::count_indirection_level()
{
	CHECK_INVARIANT(((typeid(*opd) == typeid(Name_Ast)) || 
			 (typeid(*opd) == typeid(Pointer_Deref_Ast)) ||
			 (typeid(*opd) == typeid(Address_Of_Ast))), "Wrong type of child in Pointer_Deref Ast");
	int opd_ind_level = opd->count_indirection_level();

	return opd_ind_level - 1;
}

bool Pointer_Deref_Ast::is_a_scalar()
{
	return false;
}

Symbol_Table_Entry & Pointer_Deref_Ast::get_symbol_entry()
{
	CHECK_INVARIANT(opd != NULL, "Base cannot be NULL in Array AST");
	return opd->get_symbol_entry();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
Field_Access_Ast::Field_Access_Ast(Ast * bn,  Ast * fld, Class_Type_Expr & class_type)
{
	CHECK_INVARIANT(bn!=NULL, "Base of Field Access AST cannot be NULL");
	CHECK_INVARIANT(fld!=NULL, "Offset of Field Access AST cannot be NULL");

	base = bn;
	field = fld;
	cte = &class_type;
}

*/

Field_Access_Ast::Field_Access_Ast(Ast * bn,  Ast * fld)
{
	CHECK_INVARIANT(bn!=NULL, "Base of Field Access AST cannot be NULL");
	CHECK_INVARIANT(fld!=NULL, "Offset of Field Access AST cannot be NULL");

	base = bn;
	field = fld;

}


Field_Access_Ast::~Field_Access_Ast() { }


Basic_Data_Type Field_Access_Ast:: get_basic_data_type()
{
	return field->get_basic_data_type();
}

/*
string Field_Access_Ast::get_class_type()
{
	return cte->get_class_name();	
}
*/	
void Field_Access_Ast::print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Field_Access: ";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Base (";
	base->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Offset (";
	field->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";
}

bool Field_Access_Ast::is_a_scalar()
{
	return true;
}

Ast * Field_Access_Ast::get_base_ast()
{
	return base;
}

Ast * Field_Access_Ast::get_field_ast()
{
	return field;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

Method_Access_Ast :: Method_Access_Ast(Ast * bname, Ast * m_name, Basic_Data_Type ret_ty)
{
	CHECK_INVARIANT(bname!=NULL, "Base of Field Access AST cannot be NULL");
	CHECK_INVARIANT(m_name!=NULL, "Offset of Field Access AST cannot be NULL");

	base = bname;
	method = m_name;
	return_type = ret_ty;
}

Method_Access_Ast::~Method_Access_Ast() { }


Basic_Data_Type Method_Access_Ast :: get_basic_data_type()
{
	return return_type;
}

void Method_Access_Ast:: print_ast(int sc, ostream & file_buffer)
{
        print_spaces_on_new_line (sc, file_buffer);
	file_buffer << "Method_Access: ";
	file_buffer << "<" << printable_data_type(get_basic_data_type()) << ">";

        print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	file_buffer << "Base (";
	base->print_ast(sc+2*INDENT_INCR, file_buffer);
	file_buffer << ")";

        //print_spaces_on_new_line (sc+INDENT_INCR, file_buffer);
	method->print_ast(sc+2*INDENT_INCR, file_buffer);
}


