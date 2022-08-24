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

#include "common-headers.hh"

using namespace std;

TAC_Stmt::TAC_Stmt() {}

TAC_Stmt::~TAC_Stmt() {}

void TAC_Stmt :: print_tac_stmt(ostream &file_buffer) {}

bool TAC_Stmt :: is_leader_statement()
{
	return false;
}
bool TAC_Stmt :: has_target()
{
	return false;
}
bool TAC_Stmt :: is_label()
{
	return false;
}

bool TAC_Stmt :: needs_false_edge()
{
	return false;
}
bool TAC_Stmt :: needs_true_edge()
{
	return false;
}
bool TAC_Stmt :: needs_fall_through_edge()
{
	return false;
}
bool TAC_Stmt :: is_next_leader_statement()
{
	return false;
}
TAC_Op TAC_Stmt::get_op()
{
	return tac_operator;
}
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

Basic_Data_Type Variable_TAC_Opd::get_basic_data_type()
{
	return symbol_entry->get_basic_data_type();
}

string Variable_TAC_Opd::get_operand_string()
{
	//CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Function get_label_name() called for Variable_TAC_Opd");
	return symbol_entry->get_variable_name();
} 

string Temporary_TAC_Opd::get_operand_string()
{
	//CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Function get_label_name() called for Temporary_TAC_Opd");
	return "temp"+to_string(temp_number);
}


////////////   Int_Const_TAC_Opd   ////////////////
Int_Const_TAC_Opd::Int_Const_TAC_Opd (int n)
{
	num = n;
}

int Int_Const_TAC_Opd::get_int_num()
{
	return num;
}
 
void Int_Const_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << num;
}

Basic_Data_Type Int_Const_TAC_Opd::get_basic_data_type()
{
	return int_data_type;
}

string Int_Const_TAC_Opd::get_operand_string()
{
	return to_string(num);
}

Int_Const_TAC_Opd & Int_Const_TAC_Opd::operator=(const Int_Const_TAC_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

//////////////  Double_Const_TAC_Opd  //////////////////
Double_Const_TAC_Opd::Double_Const_TAC_Opd (double n)
{
	num = n;
	// cout << "Number is " << n << endl; // TEST
}

double Double_Const_TAC_Opd::get_double_num()
{
	return num;
}
 
void Double_Const_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << num;
}

Basic_Data_Type Double_Const_TAC_Opd::get_basic_data_type()
{
	return double_data_type;
}

string Double_Const_TAC_Opd::get_operand_string()
{
	return to_string(num);
}


Double_Const_TAC_Opd & Double_Const_TAC_Opd::operator=(const Double_Const_TAC_Opd & rhs)
{
	this->num = rhs.num;
	return *this;
}

int Temporary_TAC_Opd::tac_temp_count=0;


///////////  Str_Const_TAC_Opd ///////
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

Basic_Data_Type Str_Const_TAC_Opd::get_basic_data_type()
{
	return string_data_type;
}

string Str_Const_TAC_Opd::get_operand_string()
{
	return "\""+program_object.get_string(str_key)+"\"";
}

//////////  Temporary_TAC_Opd //////////////////
Temporary_TAC_Opd::Temporary_TAC_Opd(Basic_Data_Type dt)
{
	temp_number = tac_temp_count++;
	data_type_of_temp = dt;
}

void Temporary_TAC_Opd::set_symbol_entry(Symbol_Table_Entry* entry)
{
	symbol_entry = entry;
}

Symbol_Table_Entry  *Temporary_TAC_Opd::get_symbol_entry()
{
	return symbol_entry;
}

Register_Descriptor * Temporary_TAC_Opd::get_register_of_temporary()
{
	return register_for_temporary;
}

void Temporary_TAC_Opd::set_register_of_temporary(Register_Descriptor * reg)
{
	register_for_temporary = reg;
}

int Temporary_TAC_Opd::get_temp_number(){
	return temp_number;
}

void Temporary_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << "temp" << temp_number;
}

void Temporary_TAC_Opd::print_opd_with_register(ostream & file_buffer)
{
	file_buffer << "temp" << temp_number << "_ has register " << register_for_temporary->get_name() << endl;
}

Basic_Data_Type Temporary_TAC_Opd::get_basic_data_type()
{
	return data_type_of_temp;
}

Temporary_TAC_Opd & Temporary_TAC_Opd::operator=(const Temporary_TAC_Opd & rhs)
{
	this->temp_number = rhs.temp_number;
	return *this;
}


//////////////////////////////      Asgn_TAC_Stmt     ////////////////////////////////

Asgn_TAC_Stmt::Asgn_TAC_Stmt(TAC_Opd * lval, TAC_Opd * rval)
{
	tac_operator = asgn_3a;
	opd1 = rval;   
	result = lval; 
}

TAC_Opd * Asgn_TAC_Stmt::get_result()
{
	return result; 
}

TAC_Opd * Asgn_TAC_Stmt::get_opd1()
{
	return opd1;   
}


void Asgn_TAC_Stmt::set_opd1(TAC_Opd * opd)
{
	opd1 = opd;   
}

void Asgn_TAC_Stmt::set_result(TAC_Opd * res)
{
	result = res; 
}


void Asgn_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" ;
	result->print_opd(file_buffer);
	file_buffer << " = " ;
	opd1->print_opd(file_buffer);
	file_buffer << "\n";
}

//////////////////////////////      Compute_TAC_Stmt     ////////////////////////////////

Compute_TAC_Stmt::Compute_TAC_Stmt(TAC_Op op, TAC_Opd * arg_opd1, TAC_Opd * arg_opd2, TAC_Opd * res, Basic_Data_Type dt)
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

Basic_Data_Type Compute_TAC_Stmt::get_basic_data_type()
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
		file_buffer << op_name(tac_operator);
		opd2->print_opd(file_buffer);
	}
	else
	{
		string ops = op_name(tac_operator);
		if (!ops.empty())
			file_buffer << op_name(tac_operator) ;
		opd1->print_opd(file_buffer);
	}
	file_buffer << "\n";
}

string Compute_TAC_Stmt::op_name(TAC_Op op)
{
	string name;

	switch (op)
	{
		case (add_3a):    name = " + ";  break;
		case (sub_3a):    name = " - ";  break;
		case (mult_3a):   name = " * ";  break;
		case (div_3a):     name = " / ";  break;
		case (uminus_3a): name = "- ";  break;
		case (and_3a):    name = " && ";  break;
		case (or_3a):     name = " || ";  break;
		case (not_3a):    name = "! ";  break;
		case (lt_3a):     name = " < ";  break;
		case (leq_3a):    name = " <= ";  break;
		case (gt_3a):     name = " > ";  break;
		case (geq_3a):    name = " >= ";  break;
		case (eq_3a):     name = " == ";  break;
		case (neq_3a ):   name = " != ";  break;
		case (copy_3a ):   name = "";  break;
		case (deref_3a ):   name = "* ";  break;
		case (addr_3a ):   name = "& ";  break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator in Compute TAC");
	}
	return name;
}

RTL_Op Compute_TAC_Stmt::get_rtl_operator_for_scalar_tac_operator()
{
	RTL_Op rtl_op;

	Basic_Data_Type dt = get_basic_data_type();

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
				case bool_data_type: rtl_op = rtl_seq; break;
				case double_data_type: rtl_op = rtl_seq_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		case (neq_3a ):   
			switch (dt)
			{
				case int_data_type: rtl_op = rtl_sne; break;
				case bool_data_type: rtl_op = rtl_sne; break;
				case double_data_type: rtl_op = rtl_sne_d; break;
				default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
			}
			break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong scalar TAC operator in Compute TAC");
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
		case (copy_3a):   ret_val = false; break;
		case (deref_3a):   ret_val = false; break;
		case (addr_3a):   ret_val = false; break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong TAC operator in Compute TAC");
	}
	return ret_val;
}

bool Compute_TAC_Stmt::handles_aggregate(TAC_Op op)
{
	bool ret_val;

	switch (op)
	{
		case (add_3a):    ret_val = false; break;
		case (sub_3a):    ret_val = false; break;
		case (mult_3a):   ret_val = false; break;
		case (div_3a):    ret_val = false; break;
		case (uminus_3a): ret_val = false; break;
		case (and_3a):    ret_val = false; break;
		case (or_3a):     ret_val = false; break;
		case (not_3a):    ret_val = false; break;
		case (lt_3a):     ret_val = false; break;
		case (leq_3a):    ret_val = false; break;
		case (gt_3a):     ret_val = false; break;
		case (geq_3a):    ret_val = false; break;
		case (eq_3a):     ret_val = false; break;
		case (neq_3a ):   ret_val = false; break;
		case (copy_3a):   ret_val = true; break;
		case (deref_3a):  ret_val = true; break;
		case (addr_3a):   ret_val = true; break;
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

bool If_Goto_TAC_Stmt :: has_target()
{
	return true;
}
void If_Goto_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" << "if(" ;
	opd1->print_opd(file_buffer);
	file_buffer << ")" ;
	file_buffer << " goto ";
	result->print_opd(file_buffer);
}
bool If_Goto_TAC_Stmt :: is_next_leader_statement()
{
	return true;
}

bool If_Goto_TAC_Stmt :: needs_false_edge()
{
	return true;
}

bool If_Goto_TAC_Stmt :: needs_true_edge()
{
	return true;
}
bool If_Goto_TAC_Stmt :: needs_fall_through_edge()
{
	return false;
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
bool Goto_TAC_Stmt :: has_target()
{
	return true;
}

void Goto_TAC_Stmt::print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\t" << "goto " ;
	result->print_opd(file_buffer);
}

bool Goto_TAC_Stmt :: is_next_leader_statement()
{
	return false;
}
bool Goto_TAC_Stmt :: needs_false_edge()
{
	return false;
}
bool Goto_TAC_Stmt :: needs_true_edge()
{
	return false;
}
bool Goto_TAC_Stmt :: needs_fall_through_edge()
{
	return true;
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

bool Label_TAC_Stmt :: is_label()
{
	return true;
} 

bool Label_TAC_Stmt :: is_leader_statement()
{
	return true;
}
/////////////////////////////////   Label_TAC_Opd   //////////////////////////////////////
int Label_TAC_Opd::tac_label_count=0;
 
Label_TAC_Opd::Label_TAC_Opd()
{
     label_number=tac_label_count++;
}

int Label_TAC_Opd::get_label()
{
	//cout <<"REturning label num - "<<this->label_number<<endl;
	return this->label_number;
}

string Label_TAC_Opd::get_label_name()
{
	stringstream l;
	l << "Label" << get_label();
	return l.str();
}

void Label_TAC_Opd::print_opd(ostream & file_buffer)
{
	file_buffer << "Label" << get_label() << endl;
}
void Label_TAC_Opd :: set_label_number(Basic_Block* bb)
{
	//cout << "Label Number set to - "<<label_number<<endl;
	this->label_number = bb->get_bb_id();
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

///////////////////////////////                  Return TAC Statements            /////////////////////////////////

Return_TAC_Stmt::Return_TAC_Stmt(TAC_Opd * opd, string proc)
{
	opd1 = opd;
	proc_name = proc;
}

TAC_Opd * Return_TAC_Stmt::get_opd1()
{
	return opd1;
}

void Return_TAC_Stmt::print_tac_stmt(ostream & file_buffer) 
{
	file_buffer <<  "\t return ";
	if (opd1 != NULL)
		opd1->print_opd(file_buffer);
	file_buffer << "\n";
}


///////////////////////////////                  Call TAC Statements            /////////////////////////////////

Call_TAC_Stmt::Call_TAC_Stmt(string name, TAC_Opd * res)
{
	proc_name = name;
	result = res;
}

string Call_TAC_Stmt::get_proc_name()
{
	return proc_name;
}

TAC_Opd * Call_TAC_Stmt::get_result()
{
	return result;
}

list <TAC_Opd *> Call_TAC_Stmt::get_actual_param_list()
{
	return actual_param_list;
}

void Call_TAC_Stmt::set_opd1(string name)
{
	proc_name = name;
}

void Call_TAC_Stmt::set_result(TAC_Opd * opd)
{
	result = opd;
}

void Call_TAC_Stmt::set_actual_param_list(TAC_Opd * apl)
{
	actual_param_list.push_back(apl);
}

void Call_TAC_Stmt::print_tac_stmt(ostream & file_buffer) 
{
	Procedure * proc = program_object.get_procedure_entry(proc_name);
	file_buffer <<  "\t";
	if(proc->get_return_type()!=void_data_type)
	{
		result->print_opd(file_buffer);
		file_buffer <<  " = ";
	}
	file_buffer << proc_name + "(";
	int count = 0;
	list<TAC_Opd *>::iterator i;
	for(i = actual_param_list.begin(); i != actual_param_list.end(); i++, count++)
	{
		(*i)->print_opd(file_buffer);
		if(count!=actual_param_list.size()-1)
			file_buffer << ", ";
	}
	file_buffer << ")\n";
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
void TAC_Stmt::set_actual_param_list(TAC_Opd * apl)
{
	stringstream msg;
	msg << "No set_actual_param_list() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

//////////////////////////////// TAC_Opd Default Virtual Function Members ///////////////////////////////////

bool TAC_Opd::is_a_variable()
{
	return (typeid(*this) == typeid(Variable_TAC_Opd));
}

bool TAC_Opd::is_a_temporary()
{
	return (typeid(*this) == typeid(Temporary_TAC_Opd));
}

bool TAC_Opd::is_an_int_const()
{
	return (typeid(*this) == typeid(Int_Const_TAC_Opd));
}


RTL_For_TAC & TAC_Opd::create_load_rtl_list() 
{
	stringstream msg;
	msg << "No create_load_rtl_list() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

RTL_For_TAC & TAC_Opd::create_load_rtl_list(Register_Descriptor *rd) 
{
	stringstream msg;
	msg << "No create_load_rtl_list(Register_Descriptor *rd) function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}


RTL_For_TAC & TAC_Opd::create_store_rtl_list(Register_Descriptor *rd) 
{
	stringstream msg;
	msg << "No create_store_rtl_list()function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Basic_Data_Type TAC_Opd::get_basic_data_type() 
{
	stringstream msg;
	msg << "No get_basic_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void TAC_Opd::set_register_of_temporary(Register_Descriptor *rd)
{
	stringstream msg;
	msg << "No set_register_of_temporary() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Register_Descriptor * TAC_Opd::get_register_of_temporary()
{
	stringstream msg;
	msg << "No get_register_of_temporary() function for " << typeid(*this).name();
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
int TAC_Opd::get_int_num()
{
	stringstream msg;
	msg << "No get_int_num() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
double TAC_Opd::get_double_num()
{
	stringstream msg;
	msg << "No get_double_num() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
/*void TAC_Opd::print_opd_with_register(ostream & file_buffer)
{
	stringstream msg;
	msg << "No get_label_name() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
void TAC_Opd::print_tac_label(ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_tac_label() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
} */
Basic_Block_TAC_Opd :: Basic_Block_TAC_Opd(int target)
{
	this->target = target;
}

void Basic_Block_TAC_Opd :: print_opd(ostream & file_buffer) 
{
	file_buffer << "bb" << target <<"\n";

}

int Basic_Block_TAC_Opd::get_label()
{
	return target;
}

string Basic_Block_TAC_Opd :: get_label_name()
{
	return to_string(target);
}

void NOP_TAC_Stmt :: print_tac_stmt(ostream & file_buffer)
{
	file_buffer << "\tNOP\n" ;
}


string TAC_Opd::get_operand_string()
{
	stringstream msg;
	msg << "No get_operand_string() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Array_Access_TAC_Opd::Array_Access_TAC_Opd(TAC_Opd * b, TAC_Opd *o)
{
	CHECK_INVARIANT (b!=NULL, "The base of Access_TAC_Opd cannot be NULL");
	CHECK_INVARIANT (o!=NULL, "The offset of Access_TAC_Opd cannot be NULL");
	CHECK_INVARIANT (b->is_a_variable(), "The base of Access_TAC_Opd must be a variable");
	CHECK_INVARIANT (o->is_a_variable() ||
		         o->is_a_temporary() ||
		         o->is_an_int_const(),
				 "The offset of Access_TAC_Opd must be a variable or a temporary or int_const");
	base = b;
	offset = o;
}
Array_Access_TAC_Opd::~Array_Access_TAC_Opd() { }

TAC_Opd * Array_Access_TAC_Opd::get_base_opd()
{
	return base;
}

TAC_Opd * Array_Access_TAC_Opd::get_offset_opd()
{
	return offset;
}

void Array_Access_TAC_Opd :: print_opd(ostream & file_buffer) 
{
	// printing the base
	if (base->is_a_variable())
	{
		Variable_TAC_Opd * n_opd = dynamic_cast<Variable_TAC_Opd *>(base);
		n_opd->print_opd(file_buffer);
	}
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type of TAC_Opd");
	}
	file_buffer << "[";

	// printing the offset
	if (offset->is_a_variable())
	{
		Variable_TAC_Opd * n_opd = dynamic_cast<Variable_TAC_Opd *>(offset);
		n_opd->print_opd(file_buffer);
	}
	else if (offset->is_a_temporary())
	{
		Temporary_TAC_Opd * n_opd = dynamic_cast<Temporary_TAC_Opd *>(offset);
		n_opd->print_opd(file_buffer);
	}
	else if (offset->is_an_int_const())
	{
		Int_Const_TAC_Opd * n_opd = dynamic_cast<Int_Const_TAC_Opd *>(offset);
		n_opd->print_opd(file_buffer);
	}
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type of TAC_Opd");
	}
	file_buffer << "]";
}

string Array_Access_TAC_Opd::get_operand_string()
{
	stringstream msg;
	msg << base->get_operand_string() << "[" << offset->get_operand_string() << "]";
	return msg.str();
}

Pointer_Deref_TAC_Opd::Pointer_Deref_TAC_Opd(TAC_Opd * b)
{
	CHECK_INVARIANT (b!=NULL, "The base of Access_TAC_Opd cannot be NULL");
	CHECK_INVARIANT (b->is_a_variable() ||
		         b->is_a_temporary(),
				 "The base of Access_TAC_Opd must be a variable or a temporary.");
	base = b;
}

Pointer_Deref_TAC_Opd::~Pointer_Deref_TAC_Opd() { }

TAC_Opd * Pointer_Deref_TAC_Opd::get_base_opd()
{
	return base;
}

void Pointer_Deref_TAC_Opd :: print_opd(ostream & file_buffer) 
{
	file_buffer << "* ";
	// printing the base
	if (base->is_a_variable())
	{
		Variable_TAC_Opd * n_opd = dynamic_cast<Variable_TAC_Opd *>(base);
		n_opd->print_opd(file_buffer);
	}
	else if (base->is_a_temporary())
	{
		Temporary_TAC_Opd * n_opd = dynamic_cast<Temporary_TAC_Opd *>(base);
		n_opd->print_opd(file_buffer);
	}
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type of TAC_Opd");
	}
}

string Pointer_Deref_TAC_Opd::get_operand_string()
{
	stringstream msg;
	msg << "* " << base->get_operand_string() ;
	return msg.str();
}

Field_Access_TAC_Opd::Field_Access_TAC_Opd(TAC_Opd * b, TAC_Opd *o)
{
	CHECK_INVARIANT (b!=NULL, "The base of Access_TAC_Opd cannot be NULL");
	CHECK_INVARIANT (o!=NULL, "The offset of Access_TAC_Opd cannot be NULL");
	CHECK_INVARIANT (b->is_a_variable(), "The base of Access_TAC_Opd must be a variable");
	CHECK_INVARIANT (o->is_a_variable(), "The offset of Access_TAC_Opd must be a variable");
	base = b;
	offset = o;
}

Field_Access_TAC_Opd::~Field_Access_TAC_Opd() { }

TAC_Opd * Field_Access_TAC_Opd::get_base_opd()
{
	return base;
}

TAC_Opd * Field_Access_TAC_Opd::get_offset_opd()
{
	return offset;
}

void Field_Access_TAC_Opd :: print_opd(ostream & file_buffer) 
{
	// printing the base
	if (base->is_a_variable())
	{
		Variable_TAC_Opd * n_opd = dynamic_cast<Variable_TAC_Opd *>(base);
		n_opd->print_opd(file_buffer);
	}
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type of TAC_Opd");
	}
	file_buffer << ".";

	// printing the offset
	if (offset->is_a_variable())
	{
		Variable_TAC_Opd * n_opd = dynamic_cast<Variable_TAC_Opd *>(offset);
		n_opd->print_opd(file_buffer);
	}
	else
	{
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type of TAC_Opd");
	}
}

string Field_Access_TAC_Opd::get_operand_string()
{
	stringstream msg;
	msg << base->get_operand_string() << "." << offset->get_operand_string();
	return msg.str();
}
