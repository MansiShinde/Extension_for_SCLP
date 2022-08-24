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
#include<fstream>
#include<iostream>
#include<typeinfo>
#include <string>
#include <vector>
#include <list>
#include <map>

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"symbol-table.hh"
#include "tac.hh"
#include"ast.hh"
#include"rtl.hh"
#include"procedure.hh"
#include"program.hh"
#include "reg-alloc.hh"


using namespace std;

TAC_Stmt::TAC_Stmt() {}

TAC_Stmt::~TAC_Stmt() {}


////////////////////////////////////////      TAC_Opd subclasses  ////////////////////////////////////////

Variable_TAC_Opd::Variable_TAC_Opd(Symbol_Table_Entry * se)
{
	symbol_entry = se;
}
 
void Variable_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << symbol_entry->get_variable_name();
}

Variable_TAC_Opd & Variable_TAC_Opd::operator=(const Variable_TAC_Opd & rhs)
{
	this->symbol_entry = rhs.symbol_entry;
	return *this;
}

Symbol_Table_Entry * Variable_TAC_Opd::get_symbol_entry()
{
	return symbol_entry;
}

Data_Type Variable_TAC_Opd::get_data_type()
{
	return symbol_entry->get_data_type();
}
 
Int_Const_TAC_Opd::Int_Const_TAC_Opd (int n)
{
	num = n;
}
 
void Int_Const_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << num;
}

Data_Type Int_Const_TAC_Opd::get_data_type()
{
	return int_data_type;
}

Int_Const_TAC_Opd & Int_Const_TAC_Opd::operator=(const Int_Const_TAC_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

Double_Const_TAC_Opd::Double_Const_TAC_Opd (double n)
{
	num = n;
	// cout << "Number is " << n << endl; // TEST
}
 
void Double_Const_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << num;
}

Data_Type Double_Const_TAC_Opd::get_data_type()
{
	return double_data_type;
}

Double_Const_TAC_Opd & Double_Const_TAC_Opd::operator=(const Double_Const_TAC_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

int Temporary_TAC_Opd::tac_temp_count=0;
 
Str_Const_TAC_Opd::Str_Const_TAC_Opd(int key)
{
     str_key = key;
}

int Str_Const_TAC_Opd::get_string_key()
{
     return str_key;
}
 
 
void Str_Const_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << "\"" << program_object.get_string(str_key) << "\"";
}

Data_Type Str_Const_TAC_Opd::get_data_type()
{
	return string_data_type;
}

Temporary_TAC_Opd::Temporary_TAC_Opd(Data_Type dt)
{
	temp_number = tac_temp_count++;
	data_type_of_temp = dt;
}

Register_Descriptor * Temporary_TAC_Opd::get_register_of_temporary()
{
	return register_for_temporary;
}

void Temporary_TAC_Opd::set_register_of_temporary(Register_Descriptor * reg)
{
	register_for_temporary = reg;
}


void Temporary_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << "temp" << temp_number << "_";
}

void Temporary_TAC_Opd::print_opd_with_register(ostream & file_buffer)
{
	file_buffer << "temp" << temp_number << "_ has register " << register_for_temporary->get_name() << endl;
}

Data_Type Temporary_TAC_Opd::get_data_type()
{
	return data_type_of_temp;
}

Temporary_TAC_Opd & Temporary_TAC_Opd::operator=(const Temporary_TAC_Opd & rhs)
{
	this->temp_number = rhs.temp_number;
	return *this;
}


//////////////////////////////      Copy_TAC_Stmt     ////////////////////////////////

Copy_TAC_Stmt::Copy_TAC_Stmt(TAC_Opd * lval, TAC_Opd * rval)
{
	tac_operator = copy_3a;
	opd1 = rval;   
	result = lval; 
}

TAC_Opd * Copy_TAC_Stmt::get_result()
{
	return result; 
}

TAC_Opd * Copy_TAC_Stmt::get_opd1()
{
	return opd1;   
}


void Copy_TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	opd1 = opd;   
}

void Copy_TAC_Stmt::set_result(TAC_Opd * res)
{
	result = res; 
}


void Copy_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" ;
	result->print_opd(file_buffer);
	file_buffer << " = " ;
	opd1->print_opd(file_buffer);
	file_buffer << "\n";
}

//////////////////////////////      Compute_TAC_Stmt     ////////////////////////////////

Compute_TAC_Stmt::Compute_TAC_Stmt(TAC_Op op, TAC_Opd * arg_opd1, TAC_Opd * arg_opd2, TAC_Opd * res, Data_Type dt)
{
	tac_operator = op;
	opd1 = arg_opd1;
	opd2 = arg_opd2;
	result = res; 
	tac_stmt_type = dt;
}


TAC_Opd * Compute_TAC_Stmt::get_result()
{
	return result; 
}

TAC_Opd * Compute_TAC_Stmt::get_opd1()
{
	return opd1;   
}

TAC_Opd * Compute_TAC_Stmt::get_opd2()
{
	return opd2;   
}


void Compute_TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	opd1 = opd;   
}

void Compute_TAC_Stmt::set_opd2(TAC_Opd * opd)
{
	opd2 = opd;   
}

void Compute_TAC_Stmt::set_result(TAC_Opd * res)
{
	result = res; 
}

Data_Type Compute_TAC_Stmt::get_data_type()
{
	return tac_stmt_type;
}

void Compute_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" ;
	result->print_opd(file_buffer);
	file_buffer << " = " ;
	if (is_binary_op(tac_operator))
	{
		opd1->print_opd(file_buffer);
		file_buffer << " " << op_name(tac_operator) << " " ;
		opd2->print_opd(file_buffer);
	}
	else
	{
		file_buffer << " " << op_name(tac_operator) << " " ;
		opd1->print_opd(file_buffer);
	}
	file_buffer << "\n";
}

string Compute_TAC_Stmt::op_name(TAC_Op op)
{
	string name;

	switch (op)
	{
		case (add_3a):    name = "+";  break;
		case (sub_3a):    name = "-";  break;
		case (mult_3a):   name = "*";  break;
		case (div_3a):     name = "/";  break;
		case (uminus_3a): name = "-";  break;
		case (and_3a):    name = "&&";  break;
		case (or_3a):     name = "||";  break;
		case (not_3a):    name = "!";  break;
		case (lt_3a):     name = "<";  break;
		case (leq_3a):    name = "<=";  break;
		case (gt_3a):     name = ">";  break;
		case (geq_3a):    name = ">=";  break;
		case (eq_3a):     name = "==";  break;
		case (neq_3a ):   name = "!=";  break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator in Compute TAC");
	}
	return name;
}

RTL_Op Compute_TAC_Stmt::get_rtl_operator_for_tac_operator()
{
	RTL_Op rtl_op;

	Data_Type dt = get_data_type();

	stringstream msg;
	msg << "Wrong type in tac->rtl operator mapping for tac operator " << op_name(tac_operator);

	switch (tac_operator)
	{
		case (add_3a):    
			switch (dt)
			{
				case int_data_type: 
						if (typeid(*this) == typeid (Int_Const_TAC_Opd))
							rtl_op = rtl_imm_add; 
						else
							rtl_op = rtl_add; 
						break;
				case double_data_type: rtl_op = rtl_add_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (sub_3a):  
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sub; break;
				case double_data_type: rtl_op = rtl_sub_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (mult_3a):   
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_mult; break;
				case double_data_type: rtl_op = rtl_mult_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (div_3a):    
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_div; break;
				case double_data_type: rtl_op = rtl_div_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (uminus_3a):
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_uminus; break;
				case double_data_type: rtl_op = rtl_uminus_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (and_3a):    rtl_op = rtl_and;  break;
		case (or_3a):     rtl_op = rtl_or;  break;
		case (not_3a):    rtl_op = rtl_not;  break;
		case (lt_3a):     
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_slt; break;
				case double_data_type: rtl_op = rtl_slt_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (leq_3a):    
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sle; break;
				case double_data_type: rtl_op = rtl_sle_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (gt_3a):     
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sgt; break;
				case double_data_type: rtl_op = rtl_sgt_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (geq_3a):    
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sge; break;
				case double_data_type: rtl_op = rtl_sge_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (eq_3a):     
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_seq; break;
				case double_data_type: rtl_op = rtl_seq_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (neq_3a ):   
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sne; break;
				case double_data_type: rtl_op = rtl_sne_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator in Compute TAC");
	}
	return rtl_op;
}

bool Compute_TAC_Stmt::is_binary_op(TAC_Op op)
{
	bool ret_val;

	switch (op)
	{
		case (add_3a):    ret_val = true; break;
		case (sub_3a):    ret_val = true; break;
		case (mult_3a):   ret_val = true; break;
		case (div_3a):    ret_val = true;  break;
		case (uminus_3a): ret_val = false; break;
		case (and_3a):    ret_val = true;  break;
		case (or_3a):     ret_val = true;  break;
		case (not_3a):    ret_val = false; break;
		case (lt_3a):     ret_val = true; break;
		case (leq_3a):    ret_val = true;  break;
		case (gt_3a):     ret_val = true; break;
		case (geq_3a):    ret_val = true;  break;
		case (eq_3a):     ret_val = true;  break;
		case (neq_3a ):   ret_val = true;  break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator in Compute TAC");
	}
	return ret_val;
}

/////////////////////////////////   If_Goto_TAC_Stmt   //////////////////////////////////////
If_Goto_TAC_Stmt::If_Goto_TAC_Stmt(TAC_Opd * opd, TAC_Opd * l)
{
	opd1 = opd;
	result = l;
}

TAC_Opd * If_Goto_TAC_Stmt::get_opd1()
{
	return opd1;
}

TAC_Opd * If_Goto_TAC_Stmt::get_result()
{
	return result;
}

void If_Goto_TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	opd1 = opd;
}

void If_Goto_TAC_Stmt::set_result(TAC_Opd * l)
{
	result = l;
}

void If_Goto_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" << "if(" ;
	opd1->print_opd(file_buffer);
	file_buffer << ")" ;
	file_buffer << " goto ";
	result->print_opd(file_buffer);
	//Label_TAC_Stmt * label1 = new Label_TAC_Stmt();
	//label1->print_tac_stmt(file_buffer);
	//file_buffer << Goto_TAC_Stmt(label);
}

/////////////////////////////////   Goto_TAC_Stmt   //////////////////////////////////////
Goto_TAC_Stmt::Goto_TAC_Stmt(TAC_Opd * l)
{
	result = l;
}

TAC_Opd * Goto_TAC_Stmt::get_result()
{
	return result;
}

void Goto_TAC_Stmt::set_result(TAC_Opd * l)
{
	result = l;
}

void Goto_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" << "goto " ;
	result->print_opd(file_buffer);
}

/////////////////////////////////   Label_TAC_Stmt   //////////////////////////////////////
Label_TAC_Stmt::Label_TAC_Stmt(TAC_Opd * l_opd)
{
     label_opd = l_opd;
}

int Label_TAC_Stmt::get_label()
{
	return label_opd->get_label();
}

string Label_TAC_Stmt::get_label_name()
{
	stringstream l;
	l << "Label" << label_opd->get_label() << ":";
	return l.str();
}

void Label_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "Label" << get_label() << ":" << endl;
}

/////////////////////////////////   Label_TAC_Opd   //////////////////////////////////////
int Label_TAC_Opd::tac_label_count=0;
 
Label_TAC_Opd::Label_TAC_Opd()
{
     label_number=tac_label_count++;
}

int Label_TAC_Opd::get_label()
{
	return label_number;
}

string Label_TAC_Opd::get_label_name()
{
	stringstream l;
	l << "Label" << get_label() << ":";
	return l.str();
}

void Label_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << "Label" << get_label() << endl;
}

/////////////////////////////////   TAC_For_Ast   //////////////////////////////////////
 
TAC_For_Ast::TAC_For_Ast() 
{ 
	list<TAC_Stmt *> * TAC_list = new list<TAC_Stmt *>; 
}

TAC_For_Ast::TAC_For_Ast(list<TAC_Stmt *> * tac_list, TAC_Opd * res_temp)
{
     	TAC_list = tac_list;
     	result_temp = res_temp;
}
 
void TAC_For_Ast::append_tac_stmts(TAC_Stmt * tacs)
{
	TAC_list->push_back(tacs);
}

void TAC_For_Ast::print_tac_stmt_list(ostream & file_buffer)
{
	list<TAC_Stmt *>::iterator i;
		
	if (TAC_list != NULL)
		for (i=TAC_list->begin(); i!= TAC_list->end(); i++)
			(*i)->print_tac_stmt(file_buffer);
}

list<TAC_Stmt *> * TAC_For_Ast::get_tac_list()
{
	return TAC_list;
}

void TAC_For_Ast::set_TAC_list(list<TAC_Stmt *> * tac_list_new)
{
     	TAC_list = tac_list_new;
}
 
TAC_Opd * TAC_For_Ast::get_result()
{
     	return result_temp;
}

void TAC_For_Ast::set_result(TAC_Opd * result)
{
     	result_temp = result;
}

TAC_For_Ast & TAC_For_Ast::operator= (const TAC_For_Ast & rhs)
{
	this->TAC_list = rhs.TAC_list;
	this->result_temp = rhs.result_temp;
	return *this;
}

///////////////////////////////                  I/O TAC Statements            /////////////////////////////////

IO_TAC_Stmt::IO_TAC_Stmt(TAC_Op op, TAC_Opd * opd)
{
	tac_operator = op;
	opd1 = opd;
}

TAC_Opd * IO_TAC_Stmt::get_opd1()
{
	return opd1;
}

void IO_TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	opd1 = opd;
}
 	
void IO_TAC_Stmt::print_tac_stmt(ostream & file_buffer) 
{
	string op_name;
	if (tac_operator == write_3a)
		op_name = "write";	
	else if (tac_operator == read_3a)
		op_name = "read";	
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator");

	file_buffer <<  "\t" << op_name << "  " ;
	opd1->print_opd(file_buffer);
	file_buffer << "\n";
}


/////////////////////////////        TAC_Stmt Default Virtual Function Members       ///////////////////////////////

TAC_Opd * TAC_Stmt::get_opd1()
{
	stringstream msg;
	msg << "No get_opd1() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

TAC_Opd * TAC_Stmt::get_opd2()
{
	stringstream msg;
	msg << "No get_opd2() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

TAC_Opd * TAC_Stmt::get_result()
{
	stringstream msg;
	msg << "No get_result() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

string TAC_Stmt::get_comment()
{
	stringstream msg;
	msg << "No get_comment() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	stringstream msg;
	msg << "No set_opd1() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Stmt::set_opd2(TAC_Opd * opd)
{
	stringstream msg;
	msg << "No set_opd2() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Stmt::set_result(TAC_Opd * opd)
{
	stringstream msg;
	msg << "No set_result() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Stmt::set_comment(string s)
{
	stringstream msg;
	msg << "No set_comment() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

//////////////////////////////// TAC_Opd Default Virtual Function Members ///////////////////////////////////

RTL_For_TAC & TAC_Opd::create_load_stmt() 
{
	stringstream msg;
	msg << "No create_load_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

RTL_For_TAC & TAC_Opd::create_store_stmt(Register_Descriptor *rd) 
{
	stringstream msg;
	msg << "No create_store_stmt()function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type TAC_Opd::get_data_type() 
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Opd::set_register_of_temporary(Register_Descriptor *rd)
{
	stringstream msg;
	msg << "No set_register_of_temporary() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry * TAC_Opd::get_symbol_entry() 
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int TAC_Opd::get_string_key() 
{
	stringstream msg;
	msg << "No get_string_key() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int TAC_Opd::get_label()
{
	stringstream msg;
	msg << "No get_label() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

string TAC_Opd::get_label_name()
{
	stringstream msg;
	msg << "No get_label_name() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
/* void TAC_Opd::print_tac_label(ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_tac_label() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
} */
