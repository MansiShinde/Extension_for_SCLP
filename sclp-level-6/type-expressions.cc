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

#include<string>
#include<iostream>

using namespace std;

#include "common-headers.hh"

Type_Expr::Type_Expr(){}
Type_Expr::~Type_Expr(){}

int Type_Expr::get_dim_count ()
{
	stringstream msg;
	msg << "No get_dim_count() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

bool Type_Expr::is_pointer_type ()
{
	return false;
}

bool Type_Expr::is_array_type ()
{
	return false;
}

bool Type_Expr::is_dummy_leaf ()
{
	return false;
}

Type_Expr * Type_Expr::get_child()
{
	stringstream msg;
	msg << "No get_child() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


int Type_Expr::compute_cumulative_size_of_remaining_dims()
{
	stringstream msg;
	msg << "No compute_cumulative_size_of_remaining_dims() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Type_Expr::compute_total_size()
{
	stringstream msg;
	msg << "No compute_total_size() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Type_Expr::assign_dimension_number(int d)
{
	stringstream msg;
	msg << "No compute_total_size() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


#if 0
Type_Expr * Type_Expr::get_type_node_at_depth(int d)
{
	stringstream msg;
	msg << "No get_type_node_at_depth() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
#endif

int Type_Expr::get_cumulative_size_of_remaining_dims()
{
	stringstream msg;
	msg << "No get_cumulative_size_of_remaining_dims() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Type_Expr::get_cumulative_size_of_array_dim_from_end(int d)
{
	stringstream msg;
	msg << "No get_cumulative_size_of_array_dim_from_end() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


int Type_Expr::get_total_size()
{
	stringstream msg;
	msg << "No get_total_size() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Type_Expr::count_indirection_levels()
{
	stringstream msg;
	msg << "No count_indirection_levels() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


Pointer_Type_Expr::Pointer_Type_Expr(Type_Expr * o)
{
	CHECK_INVARIANT(o!=NULL, "Child Node of Pointer Type Expression cannot be NULL");
	opd = o;
}

bool Type_Expr::match_type(Type_Expr * te)
{
	bool match = false;

	if ((is_dummy_leaf()) && (te->is_dummy_leaf()))
		match = true;
	else if (((is_array_type()) && (te->is_array_type())) ||
		    ((is_pointer_type()) && (te->is_pointer_type())))
		match = (get_child())->match_type(te->get_child());
	else 
		match = false;
	return match;
}

void Pointer_Type_Expr::print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt)
{
	CHECK_INVARIANT (opd!=NULL, "Child Node of Pointer Type Expression cannot be NULL in printing");
	file_buffer << "Pointer (";
        opd->print_type_expression(sc, file_buffer, bt);
	file_buffer << ")";
}

bool Pointer_Type_Expr::is_pointer_type()
{
	return true;
}

int Pointer_Type_Expr::count_indirection_levels ()
{
	if (!(opd->is_pointer_type()))
		return 1;
	else
		return opd->count_indirection_levels() + 1;
}

Type_Expr * Pointer_Type_Expr::get_child()
{
	return opd;
}

Array_Type_Expr::Array_Type_Expr(int s, Type_Expr * o, int d)
{
	dim_size = s;
	CHECK_INVARIANT(o!=NULL, "Child Node of Array Type Expression cannot be NULL");
	opd = o;
	dim_count = d;
}

Array_Type_Expr::~Array_Type_Expr(){}

void Array_Type_Expr::print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt)
{
	CHECK_INVARIANT (opd!=NULL, "Child Node of Array Type Expression cannot be NULL in printing");
	file_buffer << "Array (DS=" << dim_size << " [DN=" << get_dim_count() << " TS=" << total_size << " CSRD=" << cumulative_size_of_remaining_dims << "],";
	opd->print_type_expression(sc, file_buffer, bt);
	file_buffer << ")";
}

int Array_Type_Expr::get_dim_count()
{
	return dim_count;
}

int Array_Type_Expr::compute_total_size()
{
	if (opd->is_array_type())
		total_size = dim_size;
	else
		total_size = dim_size * compute_cumulative_size_of_remaining_dims();
	return total_size;
}

void Array_Type_Expr::assign_dimension_number(int d)
{
	dimension_number = d;
	if (opd->is_array_type())
		opd->assign_dimension_number(d+1);
}

Type_Expr * Array_Type_Expr::get_child()
{
	return opd;
}

int Array_Type_Expr::get_total_size()
{
	return total_size;
}

int Array_Type_Expr::compute_cumulative_size_of_remaining_dims()
{
	if (!(opd->is_array_type()))
		cumulative_size_of_remaining_dims = 1;
	else
		cumulative_size_of_remaining_dims = opd->compute_total_size();
	return cumulative_size_of_remaining_dims;
}

int Array_Type_Expr::get_cumulative_size_of_remaining_dims()
{
	return cumulative_size_of_remaining_dims;
}

int Array_Type_Expr::get_cumulative_size_of_array_dim_from_end(int d)
{
	CHECK_INVARIANT (d >= 0, "Negative dimension number in Array_Type_Expr");
	if (d==dimension_number)
		return cumulative_size_of_remaining_dims;
	else
	{
		CHECK_INVARIANT (opd->is_array_type(), "Wrong dimension number in  Array_Type_Expr");
		return opd->get_cumulative_size_of_array_dim_from_end(d);
	}

}

bool Array_Type_Expr::is_array_type()
{
	return true;
}



Dummy_Type_Expr_Leaf::Dummy_Type_Expr_Leaf(){}
Dummy_Type_Expr_Leaf::~Dummy_Type_Expr_Leaf(){}

void Dummy_Type_Expr_Leaf::print_type_expression(int sc, ostream & file_buffer, Basic_Data_Type bt)
{
	file_buffer << printable_data_type(bt);
}

bool Dummy_Type_Expr_Leaf::is_dummy_leaf()
{
	return true;
}

Decl_Type_Info::Decl_Type_Info(Basic_Data_Type bt, Type_Expr * te, int s, int n)
{
	bdt = bt;
	ddt = te;
	size = s;
	lineno = n;
	name = "__Error:Name_not_initialized__";
	if (te->is_array_type())
	{
		te->compute_total_size();
		te->compute_cumulative_size_of_remaining_dims();
		te->assign_dimension_number(1);
		max_depth = te->get_dim_count();
	}
	else if (te->is_pointer_type())
		max_depth = te->count_indirection_levels();
	else if (te->is_dummy_leaf())
		max_depth = 1;
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong Type Of Derived Data Type in Type Info");
}

Decl_Type_Info::~Decl_Type_Info() { }

Basic_Data_Type Decl_Type_Info::get_base_data_type()
{
	return bdt;
}

void Decl_Type_Info::set_base_data_type(Basic_Data_Type bt)
{
	bdt = bt;
}

Type_Expr * Decl_Type_Info::get_derived_type()
{
	return ddt;
}

void Decl_Type_Info::set_derived_type(Type_Expr * te)
{
	ddt = te;
}

int Decl_Type_Info::get_size()
{
	return size;
}

void Decl_Type_Info::set_size(int s)
{
	size = s;
}

string Decl_Type_Info::get_name()
{
	return name;
}

void Decl_Type_Info::set_name(string nm)
{
	name = nm;
}

void Decl_Type_Info::print_decl_type_info(int sc, ostream & file_buffer)
{
	file_buffer << "Variable " << get_name() << " Type: "; 
	Type_Expr * dt = get_derived_type();
	Basic_Data_Type bt = get_base_data_type();
	dt->print_type_expression(4,file_buffer,bt);
	file_buffer << " [Line Number: " << get_line_number() << "]";
	file_buffer << " [Max Depth: " << get_max_depth() << "]";
	file_buffer << endl;
}

bool Decl_Type_Info::is_derived_type()
{
	return !((get_derived_type())->is_dummy_leaf());
}

int Decl_Type_Info::get_line_number()
{
	return lineno;
}

void Decl_Type_Info::set_line_number(int n)
{
	lineno = n;
}

int Decl_Type_Info::get_max_depth()
{
	return max_depth;
}


Access_Type_Info::Access_Type_Info (Ast * b, Decl_Type_Info * dti, Ast * d_ast, int d)
{
	declared_type = dti;
	base = b;
	curr_depth = d;
	deref_ast = d_ast;
	max_depth = dti->get_max_depth();
}

Access_Type_Info::~Access_Type_Info() { }

int Access_Type_Info::get_curr_depth()
{
	return curr_depth;
}

int Access_Type_Info::get_max_depth()
{
	return max_depth;
}

Ast * Access_Type_Info::get_deref_ast()
{
	return deref_ast;
}


Decl_Type_Info * Access_Type_Info::get_declared_type()
{
	return declared_type;
}

void Access_Type_Info::set_curr_depth(int d)
{
	curr_depth = d;
}

int Access_Type_Info::get_cumulative_size_of_array_dim_from_end(int d)
{
	int size;
	Type_Expr * ddt = declared_type->get_derived_type();
	if (ddt->is_array_type())
		size = ddt->get_cumulative_size_of_array_dim_from_end(d);
	else if (ddt->is_pointer_type())
		size = 1;
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong Type Of Derived Data Type in Access Type Info");
	return size;
}

Ast * Access_Type_Info::get_name_ast()
{
	return base;
}

void Access_Type_Info::check_depth_consistency()
{
	if (curr_depth == max_depth)
		return;

	stringstream msg;
	Decl_Type_Info * dti = get_declared_type();
	string name = dti->get_name();
	Type_Expr * ddt = dti->get_derived_type();

	if (ddt->is_array_type())
	{
		if (curr_depth > max_depth)
			msg << "Number of array dimensions exceeded than the total number of dimensions for variable " + name;
		else
			msg << "Fewer dimensions than the total number of dimensions for array variable " + name;
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, msg.str(),NO_FILE_LINE);
	}
	else if (ddt->is_pointer_type())
	{
		if (curr_depth > max_depth)
		{
			msg << "Indirection level exceeded for pointer variable " + name;
			CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, msg.str(),NO_FILE_LINE);
		}
		// lower indirection level is fine for pointers
	}
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong Type Of Derived Data Type in Access Type Info");

}

bool Access_Type_Info::match_type(Access_Type_Info * ati)
{
	bool derived_match = false;

	Decl_Type_Info * dti1 = get_declared_type();
	Decl_Type_Info * dti2 = ati->get_declared_type();

	Type_Expr *te1 = dti1->get_derived_type();
	Type_Expr *te2 = dti2->get_derived_type();

	if ((dti1->is_derived_type()) && (dti2->is_derived_type()))
		derived_match = te1->match_type(te2);

	Basic_Data_Type b1 = dti1->get_base_data_type();
	Basic_Data_Type b2 = dti2->get_base_data_type();

	return derived_match && (b1 == b2);
}

///////////////////////////////////////////////////////////////////////////////////////

Class_Type_Expr::Class_Type_Expr(string classname, int line)
{
	
	class_name = classname;
	lineno = line;

	class_is_defined = false;
}

Class_Type_Expr::~Class_Type_Expr() { }


bool Class_Type_Expr::is_class_defined()
{
	return class_is_defined;
}


void Class_Type_Expr::set_class_is_defined()
{
	class_is_defined = true;
}


string Class_Type_Expr::get_class_name()
{
	return class_name;
}


void Class_Type_Expr::set_local_class_list(Symbol_Table & new_list)
{
	local_class_symbol_table = new_list;
	local_class_symbol_table.set_table_scope(local);
}

void Class_Type_Expr::set_global_class_list(Symbol_Table & new_list)
{
	global_class_symbol_table = new_list;
	global_class_symbol_table.set_table_scope(global);
}

Symbol_Table Class_Type_Expr::get_local_class_list()
{

	return local_class_symbol_table;
}

Symbol_Table Class_Type_Expr::get_global_class_list()
{

	return global_class_symbol_table;
}


bool Class_Type_Expr::variable_in_local_symbol_list_check(string variable)
{
	return local_class_symbol_table.variable_in_symbol_list_check(variable);
}


bool Class_Type_Expr::variable_in_global_class_list_check(string variable)
{
	return global_class_symbol_table.variable_in_symbol_list_check(variable);
}


Symbol_Table_Entry & Class_Type_Expr::get_local_class_symbol_table_entry(string variable_name)
{
	return local_class_symbol_table.get_symbol_table_entry(variable_name);
}


Symbol_Table_Entry & Class_Type_Expr::get_global_class_symbol_table_entry(string variable_name)
{
	return global_class_symbol_table.get_symbol_table_entry(variable_name);
}

/////////////////////////////////////////////////////////////////////////////////


