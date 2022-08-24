
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
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

int flag=1;
Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile()
{
	/* 
		An assignment x = y where y is a variable is 
		compiled as a combination of load and store statements:
		(load) R <- y 
		(store) x <- R
		If y is a constant, the statement is compiled as:
		(imm_Load) R <- y 
		(store) x <- R
		where imm_Load denotes the load immediate operation.
	*/
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	load_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null");
	load_register->reset_use_for_expr_result();

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	if ((typeid(*rhs) == typeid(Name_Ast)) || (typeid(*rhs) == typeid(Number_Ast<int>)) || (typeid(*rhs) == typeid(Number_Ast<double>)))
	{
		switch(node_data_type)
		{
		case int_data_type:
			lra.optimize_lra<int_reg>(mc_2m, lhs, rhs);
			break;
		case double_data_type:
			lra.optimize_lra<float_reg>(mc_2m, lhs, rhs);
			break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}
	Code_For_Ast load_stmt = rhs->compile_and_optimize_ast(lra);
	Register_Descriptor * result_register = load_stmt.get_reg();
	CHECK_INVARIANT(result_register, "Result register cannot be null");
	result_register->set_use_for_expr_result();

	Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);

	// Store the statements into the list

	list<Icode_Stmt *> ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT(assign_stmt, "Assignment statement cannot be null");
	return *assign_stmt;
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile()
{
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null");

	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

	Register_Descriptor * result_register = NULL;
	switch(get_data_type())
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type: 
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	switch(get_data_type())
	{
	case int_data_type:
		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
		break;

	case double_data_type:
		load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null");

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

	Icode_Stmt * store_stmt = NULL;
	switch(get_data_type())
	{
	case int_data_type:
		store_stmt = new Move_IC_Stmt(store, register_opd, opd);
		break;

	case double_data_type:
		store_stmt = new Move_IC_Stmt(store_d, register_opd, opd);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((store_stmt != NULL), "Store statement cannot be null");

	/*
		This option is removed now. Uday.
	if (command_options.is_do_lra_selected() == false)
		variable_symbol_entry->free_register(store_register);

	else
	*/
	{
		variable_symbol_entry->update_register(store_register);
		store_register->reset_use_for_expr_result();
	}

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);

	return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	bool load_needed = lra.is_load_needed();

	Register_Descriptor * result_register = lra.get_register();
	CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	if (load_needed)
	{
		Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

		load_stmt = NULL;
		switch(get_data_type())
		{
		case int_data_type:
			load_stmt = new Move_IC_Stmt(load, opd, register_opd);
			break;

		case double_data_type:
			load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
			break;

		defualt: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}

		CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

		ic_list.push_back(load_stmt);
	}

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Register_Descriptor * result_register;
	switch(node_data_type)
	{
		case int_data_type:
			result_register = machine_desc_object.get_new_register<int_reg>();
			break;

		case double_data_type:
			result_register = machine_desc_object.get_new_register<float_reg>();
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Ics_Opd * opd = new Const_Opd<DATA_TYPE>(constant);

	Icode_Stmt * load_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
		break;

	case double_data_type:
		load_stmt = new Move_IC_Stmt(imm_load_d, opd, load_register);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);

	return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");

	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd = new Const_Opd<DATA_TYPE>(constant);

	Icode_Stmt * load_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
		break;

	case double_data_type:
		load_stmt = new Move_IC_Stmt(imm_load_d, opd, load_register);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// lhs

	Code_For_Ast & lhs_code = lhs->compile();
	Register_Descriptor * lhs_result_reg = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs register cannot be null");

	lhs_result_reg->set_use_for_expr_result();
	Ics_Opd * left_result_reg = new Register_Addr_Opd(lhs_result_reg);

	// rhs
	
	Code_For_Ast & rhs_code = rhs->compile();
	Register_Descriptor * rhs_result_reg = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Rhs register cannot be null");

	rhs_result_reg->set_use_for_expr_result();
	Ics_Opd * right_result_reg = new Register_Addr_Opd(rhs_result_reg);

	// target

	Register_Descriptor * result_register = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * add_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		add_stmt = new Compute_IC_Stmt(add, left_result_reg, right_result_reg,target_reg);
		break;

	case double_data_type:
		add_stmt = new Compute_IC_Stmt(add_d, left_result_reg, right_result_reg, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((add_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (add_stmt != NULL)
		ic_list.push_back(add_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs result register cannot be null");
		
	lhs_result_reg->reset_use_for_expr_result();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Lhs result register cannot be null");
		
	rhs_result_reg->reset_use_for_expr_result();

	return *arith_stmt;
}

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// lhs
	
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	Lra_Outcome left_lra;
	if ((typeid(*lhs) == typeid(Name_Ast)) || (typeid(*lhs) == typeid(Number_Ast<int>)) || (typeid(*lhs) == typeid(Number_Ast<double>)))
	{
		switch(lhs->get_data_type())
		{
		case int_data_type:
			left_lra.optimize_lra<int_reg>(mc_2r, NULL, lhs);
			break;

		case double_data_type:
			left_lra.optimize_lra<float_reg>(mc_2r, NULL, lhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * lhs_result_register = left_lra.get_register();

	Code_For_Ast & lhs_code = lhs->compile_and_optimize_ast(left_lra);
	lhs_result_register = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_register != NULL), "Left register cannot be null");

	lhs_result_register->set_use_for_expr_result();
	Ics_Opd * left_result_register = new Register_Addr_Opd(lhs_result_register);

	// rhs

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Lra_Outcome right_lra;
	if ((typeid(*rhs) == typeid(Name_Ast)) || (typeid(*rhs) == typeid(Number_Ast<int>)) || (typeid(*rhs) == typeid(Number_Ast<double>)))
	{
		switch(rhs->get_data_type())
		{
		case int_data_type:
			right_lra.optimize_lra<int_reg>(mc_2r, NULL, rhs);
			break;

		case double_data_type:
			right_lra.optimize_lra<float_reg>(mc_2r, NULL, rhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * rhs_result_register = right_lra.get_register();

	Code_For_Ast & rhs_code = rhs->compile_and_optimize_ast(right_lra);
	rhs_result_register = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_register != NULL), "Right register cannot be null");

	rhs_result_register->set_use_for_expr_result();
	Ics_Opd * right_result_register = new Register_Addr_Opd(rhs_result_register);

	// target

	Register_Descriptor * result_register = NULL;

	switch(node_data_type)
	{
	case int_data_type: 
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;
	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * add_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		add_stmt = new Compute_IC_Stmt(add, left_result_register, right_result_register, target_reg);
		break;

	case double_data_type:
		add_stmt = new Compute_IC_Stmt(add_d, left_result_register, right_result_register, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((add_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (add_stmt != NULL)
		ic_list.push_back(add_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	if (lhs_result_register)
	{
		lhs_result_register->reset_use_for_expr_result();
	}

	if (rhs_result_register)
	{
		rhs_result_register->reset_use_for_expr_result();
	}

	return *arith_stmt;
}

/////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// lhs

	Code_For_Ast & lhs_code = lhs->compile();
	Register_Descriptor * lhs_result_reg = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs register cannot be null");

	lhs_result_reg->set_use_for_expr_result();
	Ics_Opd * left_result_reg = new Register_Addr_Opd(lhs_result_reg);

	// rhs
	
	Code_For_Ast & rhs_code = rhs->compile();
	Register_Descriptor * rhs_result_reg = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Rhs register cannot be null");

	rhs_result_reg->set_use_for_expr_result();
	Ics_Opd * right_result_reg = new Register_Addr_Opd(rhs_result_reg);

	// target

	Register_Descriptor * result_register = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * minus_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		minus_stmt = new Compute_IC_Stmt(sub, left_result_reg, right_result_reg, target_reg);
		break;

	case double_data_type:
		minus_stmt = new Compute_IC_Stmt(sub_d, left_result_reg, right_result_reg, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((minus_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (minus_stmt != NULL)
		ic_list.push_back(minus_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs result register cannot be null");
		
	lhs_result_reg->reset_use_for_expr_result();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Lhs result register cannot be null");
		
	rhs_result_reg->reset_use_for_expr_result();

	return *arith_stmt;
}

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// lhs
	
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	Lra_Outcome left_lra;
	if ((typeid(*lhs) == typeid(Name_Ast)) || (typeid(*lhs) == typeid(Number_Ast<int>)) || (typeid(*lhs) == typeid(Number_Ast<double>)))
	{
		switch(lhs->get_data_type())
		{
		case int_data_type:
			left_lra.optimize_lra<int_reg>(mc_2r, NULL, lhs);
			break;

		case double_data_type:
			left_lra.optimize_lra<float_reg>(mc_2r, NULL, lhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * lhs_result_register = left_lra.get_register();
	Code_For_Ast & lhs_code = lhs->compile_and_optimize_ast(left_lra);
	lhs_result_register = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_register != NULL), "Left register cannot be null");

	lhs_result_register->set_use_for_expr_result();
	Ics_Opd * left_result_register = new Register_Addr_Opd(lhs_result_register);

	// rhs

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Lra_Outcome right_lra;
	if ((typeid(*rhs) == typeid(Name_Ast)) || (typeid(*rhs) == typeid(Number_Ast<int>)) || (typeid(*rhs) == typeid(Number_Ast<double>)))
	{
		switch(rhs->get_data_type())
		{
		case int_data_type:
			right_lra.optimize_lra<int_reg>(mc_2r, NULL, rhs);
			break;

		case double_data_type:
			right_lra.optimize_lra<float_reg>(mc_2r, NULL, rhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * rhs_result_register = right_lra.get_register();
	Code_For_Ast & rhs_code = rhs->compile_and_optimize_ast(right_lra);
	rhs_result_register = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_register != NULL), "Right register cannot be null");

	rhs_result_register->set_use_for_expr_result();
	Ics_Opd * right_result_register = new Register_Addr_Opd(rhs_result_register);

	// target

	Register_Descriptor * result_register = NULL;

	switch(node_data_type)
	{
	case int_data_type: 
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;
	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * minus_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		minus_stmt = new Compute_IC_Stmt(sub, left_result_register, right_result_register, target_reg);
		break;

	case double_data_type:
		minus_stmt = new Compute_IC_Stmt(sub_d, left_result_register, right_result_register, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((minus_stmt != NULL), "Minus statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (minus_stmt != NULL)
		ic_list.push_back(minus_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	if (lhs_result_register)
	{
		lhs_result_register->reset_use_for_expr_result();
	}

	if (rhs_result_register)
	{
		rhs_result_register->reset_use_for_expr_result();
	}

	return *arith_stmt;
}

//////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// lhs

	Code_For_Ast & lhs_code = lhs->compile();
	Register_Descriptor * lhs_result_reg = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs register cannot be null");

	lhs_result_reg->set_use_for_expr_result();
	Ics_Opd * left_result_reg = new Register_Addr_Opd(lhs_result_reg);

	// rhs
	
	Code_For_Ast & rhs_code = rhs->compile();
	Register_Descriptor * rhs_result_reg = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Rhs register cannot be null");

	rhs_result_reg->set_use_for_expr_result();
	Ics_Opd * right_result_reg = new Register_Addr_Opd(rhs_result_reg);

	// target

	Register_Descriptor * result_register = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * mult_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		mult_stmt = new Compute_IC_Stmt(mult, left_result_reg, right_result_reg, target_reg);
		break;

	case double_data_type:
		mult_stmt = new Compute_IC_Stmt(mult_d, left_result_reg, right_result_reg, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((mult_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (mult_stmt != NULL)
		ic_list.push_back(mult_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs result register cannot be null");
		
	lhs_result_reg->reset_use_for_expr_result();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Lhs result register cannot be null");
		
	rhs_result_reg->reset_use_for_expr_result();

	return *arith_stmt;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// lhs
	
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	Lra_Outcome left_lra;
	if ((typeid(*lhs) == typeid(Name_Ast)) || (typeid(*lhs) == typeid(Number_Ast<int>)) || (typeid(*lhs) == typeid(Number_Ast<double>)))
	{
		switch(lhs->get_data_type())
		{
		case int_data_type:
			left_lra.optimize_lra<int_reg>(mc_2r, NULL, lhs);
			break;

		case double_data_type:
			left_lra.optimize_lra<float_reg>(mc_2r, NULL, lhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * lhs_result_register = left_lra.get_register();

	Code_For_Ast & lhs_code = lhs->compile_and_optimize_ast(left_lra);
	lhs_result_register = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_register != NULL), "Left register cannot be null");

	lhs_result_register->set_use_for_expr_result();
	Ics_Opd * left_result_register = new Register_Addr_Opd(lhs_result_register);

	// rhs

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Lra_Outcome right_lra;
	if ((typeid(*rhs) == typeid(Name_Ast)) || (typeid(*rhs) == typeid(Number_Ast<int>)) || (typeid(*rhs) == typeid(Number_Ast<double>)))
	{
		switch(rhs->get_data_type())
		{
		case int_data_type:
			right_lra.optimize_lra<int_reg>(mc_2r, NULL, rhs);
			break;

		case double_data_type:
			right_lra.optimize_lra<float_reg>(mc_2r, NULL, rhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * rhs_result_register = right_lra.get_register();

	Code_For_Ast & rhs_code = rhs->compile_and_optimize_ast(right_lra);
	rhs_result_register = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_register != NULL), "Right register cannot be null");

	rhs_result_register->set_use_for_expr_result();
	Ics_Opd * right_result_register = new Register_Addr_Opd(rhs_result_register);

	// target

	Register_Descriptor * result_register = NULL;

	switch(node_data_type)
	{
	case int_data_type: 
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;
	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * mult_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		mult_stmt = new Compute_IC_Stmt(mult, target_reg, left_result_register, right_result_register);
		break;

	case double_data_type:
		mult_stmt = new Compute_IC_Stmt(mult_d, target_reg, left_result_register, right_result_register);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((mult_stmt != NULL), "Mult statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (mult_stmt != NULL)
		ic_list.push_back(mult_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	if (lhs_result_register)
	{
		lhs_result_register->reset_use_for_expr_result();
	}

	if (rhs_result_register)
	{
		rhs_result_register->reset_use_for_expr_result();
	}

	return *arith_stmt;
}

////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// lhs

	Code_For_Ast & lhs_code = lhs->compile();
	Register_Descriptor * lhs_result_reg = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs register cannot be null");

	lhs_result_reg->set_use_for_expr_result();
	Ics_Opd * left_result_reg = new Register_Addr_Opd(lhs_result_reg);

	// rhs
	
	Code_For_Ast & rhs_code = rhs->compile();
	Register_Descriptor * rhs_result_reg = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Rhs register cannot be null");

	rhs_result_reg->set_use_for_expr_result();
	Ics_Opd * right_result_reg = new Register_Addr_Opd(rhs_result_reg);

	// target

	Register_Descriptor * result_register = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * divide_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		divide_stmt = new Compute_IC_Stmt(divd, left_result_reg, right_result_reg, target_reg);
		break;

	case double_data_type:
		divide_stmt = new Compute_IC_Stmt(div_d, left_result_reg, right_result_reg, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((divide_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (divide_stmt != NULL)
		ic_list.push_back(divide_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs result register cannot be null");
		
	lhs_result_reg->reset_use_for_expr_result();

	CHECK_INVARIANT((rhs_result_reg != NULL), "Lhs result register cannot be null");
		
	rhs_result_reg->reset_use_for_expr_result();

	return *arith_stmt;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// lhs
	
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	Lra_Outcome left_lra;
	if ((typeid(*lhs) == typeid(Name_Ast)) || (typeid(*lhs) == typeid(Number_Ast<int>)) || (typeid(*lhs) == typeid(Number_Ast<double>)))
	{
		switch(lhs->get_data_type())
		{
		case int_data_type:
			left_lra.optimize_lra<int_reg>(mc_2r, NULL, lhs);
			break;

		case double_data_type:
			left_lra.optimize_lra<float_reg>(mc_2r, NULL, lhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * lhs_result_register = left_lra.get_register();

	Code_For_Ast & lhs_code = lhs->compile_and_optimize_ast(left_lra);
	lhs_result_register = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_register != NULL), "Left register cannot be null");

	lhs_result_register->set_use_for_expr_result();
	Ics_Opd * left_result_register = new Register_Addr_Opd(lhs_result_register);

	// rhs

	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Lra_Outcome right_lra;
	if ((typeid(*rhs) == typeid(Name_Ast)) || (typeid(*rhs) == typeid(Number_Ast<int>)) || (typeid(*rhs) == typeid(Number_Ast<double>)))
	{
		switch(rhs->get_data_type())
		{
		case int_data_type:
			right_lra.optimize_lra<int_reg>(mc_2r, NULL, rhs);
			break;

		case double_data_type:
			right_lra.optimize_lra<float_reg>(mc_2r, NULL, rhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * rhs_result_register = right_lra.get_register();

	Code_For_Ast & rhs_code = rhs->compile_and_optimize_ast(right_lra);
	rhs_result_register = rhs_code.get_reg();

	CHECK_INVARIANT((rhs_result_register != NULL), "Right register cannot be null");

	rhs_result_register->set_use_for_expr_result();
	Ics_Opd * right_result_register = new Register_Addr_Opd(rhs_result_register);

	// target

	Register_Descriptor * result_register = NULL;

	switch(node_data_type)
	{
	case int_data_type: 
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;
	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * divide_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		divide_stmt = new Compute_IC_Stmt(divd, left_result_register, right_result_register, target_reg);
		break;

	case double_data_type:
		divide_stmt = new Compute_IC_Stmt(div_d, left_result_register, right_result_register, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((divide_stmt != NULL), "Divide statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (rhs_code.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_code.get_icode_list());

	if (divide_stmt != NULL)
		ic_list.push_back(divide_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	if (lhs_result_register)
	{
		lhs_result_register->reset_use_for_expr_result();
	}

	if (rhs_result_register)
	{
		rhs_result_register->reset_use_for_expr_result();
	}

	return *arith_stmt;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	// lhs

	Code_For_Ast & lhs_code = lhs->compile();
	Register_Descriptor * lhs_result_reg = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs register cannot be null");

	lhs_result_reg->set_use_for_expr_result();
	Ics_Opd * left_result_reg = new Register_Addr_Opd(lhs_result_reg);


	// rhs

	CHECK_INVARIANT((rhs == NULL), "Rhs should be null for uminus");

	// target

	Register_Descriptor * result_register = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;

	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * uminus_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		uminus_stmt = new Compute_IC_Stmt(uminus, left_result_reg,NULL,target_reg);
		break;

	case double_data_type:
		uminus_stmt = new Compute_IC_Stmt(uminus_d, left_result_reg, NULL,target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((uminus_stmt != NULL), "Add statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (uminus_stmt != NULL)
		ic_list.push_back(uminus_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	CHECK_INVARIANT((lhs_result_reg != NULL), "Lhs result register cannot be null");
		
	lhs_result_reg->reset_use_for_expr_result();

	return *arith_stmt;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// lhs
	
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	Lra_Outcome left_lra;
	if ((typeid(*lhs) == typeid(Name_Ast)) || (typeid(*lhs) == typeid(Number_Ast<int>)) || (typeid(*lhs) == typeid(Number_Ast<double>)))
	{
		switch(lhs->get_data_type())
		{
		case int_data_type:
			left_lra.optimize_lra<int_reg>(mc_2r, NULL, lhs);
			break;

		case double_data_type:
			left_lra.optimize_lra<float_reg>(mc_2r, NULL, lhs);
			break;

		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
		}
	}

	Register_Descriptor * lhs_result_register = left_lra.get_register();

	Code_For_Ast & lhs_code = lhs->compile_and_optimize_ast(left_lra);
	lhs_result_register = lhs_code.get_reg();

	CHECK_INVARIANT((lhs_result_register != NULL), "Left register cannot be null");

	lhs_result_register->set_use_for_expr_result();
	Ics_Opd * left_result_register = new Register_Addr_Opd(lhs_result_register);

	// rhs

	CHECK_INVARIANT((rhs == NULL), "Rhs of uminus should be null");

	// target

	Register_Descriptor * result_register = NULL;

	switch(node_data_type)
	{
	case int_data_type: 
		result_register = machine_desc_object.get_new_register<int_reg>();
		break;
	case double_data_type:
		result_register = machine_desc_object.get_new_register<float_reg>();
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");

	Ics_Opd * target_reg = new Register_Addr_Opd(result_register);
	result_register->set_use_for_expr_result();

	Icode_Stmt * uminus_stmt = NULL;
	switch(node_data_type)
	{
	case int_data_type:
		uminus_stmt = new Compute_IC_Stmt(uminus, left_result_register, NULL, target_reg);
		break;

	case double_data_type:
		uminus_stmt = new Compute_IC_Stmt(uminus_d, left_result_register, NULL, target_reg);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((uminus_stmt != NULL), "Uminus statement cannot be null");

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_code.get_icode_list().empty() == false)
		ic_list = lhs_code.get_icode_list();

	if (uminus_stmt != NULL)
		ic_list.push_back(uminus_stmt);

	Code_For_Ast * arith_stmt;
	if (ic_list.empty() == false)
		arith_stmt = new Code_For_Ast(ic_list, result_register);

	CHECK_INVARIANT((arith_stmt != NULL), "Arithmetic statement cannot be null");

	if (lhs_result_register)
	{
		lhs_result_register->reset_use_for_expr_result();
	}
	return *arith_stmt;
}

///////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Relational_Expr_Ast::compile()
{
	CHECK_INVARIANT((lhs_condition != NULL) && (rhs_condition != NULL), "Lhs/rhs cannot be null in relational expression");
	Code_For_Ast & lhs_stmt = lhs_condition->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->set_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(lhs_register);

	Code_For_Ast & rhs_stmt = rhs_condition->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->set_use_for_expr_result();
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(rhs_register);

	machine_desc_object.clear_local_register_mappings();
	// get a new register to store the result
	Register_Descriptor * load_register = machine_desc_object.get_new_register<int_reg>();;
	Tgt_Op op;
	
     

	switch(rel_op)
	{
		case less_equalto : 
                              if(lhs_condition->get_data_type()==int_data_type)
		                            op = sle; 
		                        else if(lhs_condition->get_data_type()==double_data_type)
		                        	op= sle_d;

		                  break;
		case less_than : if(lhs_condition->get_data_type()==int_data_type)
		                            op = slt; 
		                        else if(lhs_condition->get_data_type()==double_data_type)
		                        	op= slt_d;

		                   break;
		case greater_than : if(lhs_condition->get_data_type()==int_data_type)
		                            op = sgt; 
		                        else if(lhs_condition->get_data_type()==double_data_type){
		                        	op= sgt_d;
		                         	flag=0;
		                         }

		          break;
		case greater_equalto : if(lhs_condition->get_data_type()==int_data_type)
		                            op = sge; 
		                        else if(lhs_condition->get_data_type()==double_data_type){
		                        	op= sge_d;
		                        	flag=0;
		                        }
		                        	 break;
		case equalto : if(lhs_condition->get_data_type()==int_data_type)
		                            op = seq; 
		                        else if(lhs_condition->get_data_type()==double_data_type)
		                        	op= seq_d;
		                        	 break;
		case not_equalto : if(lhs_condition->get_data_type()==int_data_type)
		                            op = sne; 
		                        else if(lhs_condition->get_data_type()==double_data_type){
		                        	op= sne_d;
		                        	flag=0;
		                        }
	}


	Register_Addr_Opd * result = new Register_Addr_Opd(load_register);


	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

   Icode_Stmt * ics;

           if(lhs_condition->get_data_type()==int_data_type)
	                ics = new Compute_IC_Stmt(op, opd1, opd2, result);
           else if(lhs_condition->get_data_type()==double_data_type )
           {
           	        //if(flag==1)
                    ics = new Compute_IC_Stmt(op, opd1, opd2, NULL);
                   // else
                   //ics = new Compute_IC_Stmt(op, opd2, opd1, NULL);
           }
	ic_list.push_back(ics);


        Code_For_Ast * load_stmt;
 if(lhs_condition->get_data_type()==int_data_type)
	 load_stmt = new Code_For_Ast(ic_list, load_register);
 else if(lhs_condition->get_data_type()==double_data_type)
 	load_stmt = new Code_For_Ast(ic_list, NULL);

	return * load_stmt;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Logical_Expr_Ast::compile()
{
	CHECK_INVARIANT((rhs_op != NULL), "Lhs/rhs cannot be null in boolean expression");

	Tgt_Op op;
	switch(bool_op)
	{
		case _logical_and : op = and_t; break;
		case _logical_or : op = or_t; break;
		case _logical_not : op = not_t;
						   lhs_op = new Number_Ast<int>(1, int_data_type, lineno);	// Create a dummy lhs to store value 1
	}

	CHECK_INVARIANT((lhs_op != NULL), "Lhs/rhs cannot be null in boolean expression");
	Code_For_Ast & lhs_stmt = lhs_op->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->set_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(lhs_register); 

	Code_For_Ast & rhs_stmt = rhs_op->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->set_use_for_expr_result();
	Register_Addr_Opd * opd2 = new Register_Addr_Opd(rhs_register);

	machine_desc_object.clear_local_register_mappings();
	// get a new register to store the result
	Register_Descriptor * result_register = machine_desc_object.get_new_register<int_reg>();
	//result_register->set_use_for_expr_result();
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();
	
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	Icode_Stmt * ics;
	if(bool_op == _logical_not)
		ics = new Compute_IC_Stmt(op, opd2, opd1, result);
	else
		ics = new Compute_IC_Stmt(op, opd1, opd2, result);
	
	ic_list.push_back(ics);
	Code_For_Ast * load_stmt = new Code_For_Ast(ic_list, result_register);

	if(bool_op == _logical_not) {
		delete lhs_op;
		lhs_op = NULL;
	}

	return * load_stmt;
}
///////////////////////////////////////////////////////////////////////
  

Code_For_Ast & Selection_Statement_Ast::compile()
{
	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();


	  if(cond_register==NULL)
	  {

    // cout<<"xyzx \n";

	  	Tgt_Op op;

	  	   if(flag==1)
         op = bc1f;

          else {
        op = bc1t;
         flag=1;
     }


      Code_For_Ast & then_stmt = then_part->compile();

        string label1 = get_new_label();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

        
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(op, NULL,NULL,label1);
	ic_list.push_back(ics);

	if (then_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), then_stmt.get_icode_list());

     string label2;
   if(else_part!=NULL) {
	label2 = get_new_label();
	op = j;
	Icode_Stmt * goto_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(goto_ics);
     }
	op = label;
	Icode_Stmt * lbl_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl_ics);
	
       
        if(else_part!=NULL) {
	Code_For_Ast & else_stmt = else_part->compile();
	if (else_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), else_stmt.get_icode_list());
	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);
  }
  

	//cond_register->reset_use_for_expr_result();

	Code_For_Ast * if_stmt = new Code_For_Ast(ic_list, NULL);
	return * if_stmt;





	  }

	cond_register->set_use_for_expr_result();
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(cond_register);

	Tgt_Op op = beq;

	Code_For_Ast & then_stmt = then_part->compile();
	     
	
	string label1 = get_new_label();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

        
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(op, opd1,NULL, label1);
	ic_list.push_back(ics);

	if (then_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), then_stmt.get_icode_list());

     string label2;
   if(else_part!=NULL) {
	label2 = get_new_label();
	op = j;
	Icode_Stmt * goto_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(goto_ics);
     }
	op = label;
	Icode_Stmt * lbl_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl_ics);
	
       
        if(else_part!=NULL) {
	Code_For_Ast & else_stmt = else_part->compile();
	if (else_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), else_stmt.get_icode_list());
	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);
  }
  

	cond_register->reset_use_for_expr_result();

	Code_For_Ast * if_stmt = new Code_For_Ast(ic_list, NULL);
	return * if_stmt;
}

///////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile()
{

	Code_For_Ast & cond_stmt = cond->compile();
Register_Descriptor * cond_register = cond_stmt.get_reg();

       if(cond_register==NULL)
	  {

    // cout<<"xyzx \n";

	  	Tgt_Op op1;

	  	   if(flag==1)
         op1 = bc1t;
          else {
        op1 = bc1f;
         flag=1;
     }
        string label1 = get_new_label();
	string label2 = get_new_label();
	Code_For_Ast & body_stmt = body->compile();
	Register_Descriptor * body_register = body_stmt.get_reg();

	//cond_register->set_use_for_expr_result();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Tgt_Op op = j;

	if(!is_do_form) {
		Icode_Stmt * ics = new Label_IC_Stmt(op, label2);
		ic_list.push_back(ics);
	}

	op = label;
	Icode_Stmt * lbl1_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl1_ics);

	if (body_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), body_stmt.get_icode_list());

	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(),cond_stmt.get_icode_list());

	//op1 = bne;
	Icode_Stmt * cond_ics = new Control_Flow_IC_Stmt(op1, NULL,NULL, label1);
	ic_list.push_back(cond_ics);

	//cond_register->reset_use_for_expr_result();

	Code_For_Ast * iter_stmt = new Code_For_Ast(ic_list, NULL);
	return * iter_stmt;


   }


	
	Register_Addr_Opd * opd1 = new Register_Addr_Opd(cond_register);

	string label1 = get_new_label();
	string label2 = get_new_label();
	Code_For_Ast & body_stmt = body->compile();
	Register_Descriptor * body_register = body_stmt.get_reg();

	cond_register->set_use_for_expr_result();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Tgt_Op op = j;

	if(!is_do_form) {
		Icode_Stmt * ics = new Label_IC_Stmt(op, label2);
		ic_list.push_back(ics);
	}

	op = label;
	Icode_Stmt * lbl1_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl1_ics);

	if (body_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), body_stmt.get_icode_list());

	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(),cond_stmt.get_icode_list());

	op = bne;
	Icode_Stmt * cond_ics = new Control_Flow_IC_Stmt(op, opd1,NULL, label1);
	ic_list.push_back(cond_ics);

	cond_register->reset_use_for_expr_result();

	Code_For_Ast * iter_stmt = new Code_For_Ast(ic_list, NULL);
	return * iter_stmt;
}

///////////////////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;
////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Expression_Ast::compile()
{
	Code_For_Ast & cond_stmt = cond->compile();
	Register_Descriptor * cond_register = cond_stmt.get_reg();
	  if(cond_register==NULL)
	  {

    // cout<<"xyzx \n";

	  	Tgt_Op op;

	  	   if(flag==1)
         op = bc1f;

          else {
        op = bc1t;
         flag=1;
     }


	string label1 = get_new_label();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	Icode_Stmt * ics = new Control_Flow_IC_Stmt(op, NULL,NULL, label1);
	ic_list.push_back(ics);

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();

	Tgt_Op mov_op = move_d;
	machine_desc_object.clear_local_register_mappings();
	// get a new register to store the result
	Register_Descriptor * result_register;
	if(node_data_type == int_data_type) {
		result_register = machine_desc_object.get_new_register<int_reg>();
	}
	else if(node_data_type == double_data_type) {
		result_register = machine_desc_object.get_new_register<float_reg>();
	}
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Arithmetic expression data type cannot be null");

	// Create a zero register
	Spim_Register reg = zero;
	string nam = "zero";
	Register_Descriptor * zero = new Register_Descriptor(reg, nam, int_num, fixed_reg); 
	Register_Addr_Opd * zero_reg = new Register_Addr_Opd(zero);

	Register_Addr_Opd * opd1 = NULL;
	opd1 = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Icode_Stmt * mov_ics = new Compute_IC_Stmt(mov_op, opd1, NULL, result);
	ic_list.push_back(mov_ics);
	result_register->set_use_for_expr_result();

	string label2 = get_new_label();
	op = j;
	Icode_Stmt * goto_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(goto_ics);

	op = label;
	Icode_Stmt * lbl_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl_ics);

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	opd1 = new Register_Addr_Opd(rhs_register);
	mov_ics = new Compute_IC_Stmt(mov_op, opd1, NULL, result);
	ic_list.push_back(mov_ics);

	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);

	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();
	result_register->reset_use_for_expr_result();

	Code_For_Ast * conditional_stmt = new Code_For_Ast(ic_list, result_register);
	return * conditional_stmt;





	  }



	cond_register->set_use_for_expr_result();

	Register_Addr_Opd * opd1 = new Register_Addr_Opd(cond_register);
	string label1 = get_new_label();

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (cond_stmt.get_icode_list().empty() == false)
		ic_list = cond_stmt.get_icode_list();

	Tgt_Op op = beq;
	Icode_Stmt * ics = new Control_Flow_IC_Stmt(op, opd1,NULL, label1);
	ic_list.push_back(ics);

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), lhs_stmt.get_icode_list());

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();

	Tgt_Op mov_op = or_t;
	machine_desc_object.clear_local_register_mappings();
	// get a new register to store the result
	Register_Descriptor * result_register;
	if(node_data_type == int_data_type) {
		result_register = machine_desc_object.get_new_register<int_reg>();
	}
	else if(node_data_type == double_data_type) {
		result_register = machine_desc_object.get_new_register<float_reg>();
	}
	else
		CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Arithmetic expression data type cannot be null");

	// Create a zero register
	Spim_Register reg = zero;
	string nam = "zero";
	Register_Descriptor * zero = new Register_Descriptor(reg, nam, int_num, fixed_reg); 
	Register_Addr_Opd * zero_reg = new Register_Addr_Opd(zero);

	opd1 = new Register_Addr_Opd(lhs_register);
	Register_Addr_Opd * result = new Register_Addr_Opd(result_register);
	Icode_Stmt * mov_ics = new Compute_IC_Stmt(mov_op, opd1, zero_reg, result);
	ic_list.push_back(mov_ics);
	result_register->set_use_for_expr_result();

	string label2 = get_new_label();
	op = j;
	Icode_Stmt * goto_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(goto_ics);

	op = label;
	Icode_Stmt * lbl_ics = new Label_IC_Stmt(op, label1);
	ic_list.push_back(lbl_ics);

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	opd1 = new Register_Addr_Opd(rhs_register);
	mov_ics = new Compute_IC_Stmt(mov_op, opd1, zero_reg, result);
	ic_list.push_back(mov_ics);

	Icode_Stmt * lbl2_ics = new Label_IC_Stmt(op, label2);
	ic_list.push_back(lbl2_ics);

	cond_register->reset_use_for_expr_result();
	lhs_register->reset_use_for_expr_result();
	rhs_register->reset_use_for_expr_result();
	result_register->reset_use_for_expr_result();

	Code_For_Ast * conditional_stmt = new Code_For_Ast(ic_list, result_register);
	return * conditional_stmt;
}


////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile()
{
	for (list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end(); ++it)
	{
		Code_For_Ast & ast_icode = (*it)->compile();
		sa_icode_list.splice(sa_icode_list.end(), ast_icode.get_icode_list());
	}
	Code_For_Ast * seq_icode = new Code_For_Ast(sa_icode_list, NULL);
	return *seq_icode;
}

void Sequence_Ast::print_assembly(ostream & file_buffer)
{
	for (list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
		(*it)->print_assembly(file_buffer);
}

void Sequence_Ast::print_icode(ostream & file_buffer)
{
	for (list<Icode_Stmt *>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); ++it)
		(*it)->print_icode(file_buffer);
}

//////////////////////////////////////////////////////////////////////////////
Code_For_Ast & Print_Ast::compile()
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	//Code_For_Ast & print_stmt= var->compile();

	Register_Descriptor *result_register = new Register_Descriptor (v0, "v0", int_num, int_reg);
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);

 Move_IC_Stmt * mov_stmt;
	if(var->get_data_type() == int_data_type) {

		Ics_Opd * opd = new Const_Opd<int>(1);
            mov_stmt= new Move_IC_Stmt(imm_load, opd, load_register);
      }
      else if(var->get_data_type() == double_data_type) {
           Ics_Opd * opd = new Const_Opd<int>(3);
           mov_stmt = new Move_IC_Stmt(imm_load, opd, load_register);
      }
      else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " print data type cannot be null");



//CHECK_INVARIANT(var->get_symbol_entry() , "Variable symbol entry cannot be null");

	Ics_Opd * opd = new Mem_Addr_Opd(var->get_symbol_entry());


       Register_Descriptor * arg_register;

	if(var->get_data_type() == int_data_type) {
		     arg_register = new Register_Descriptor (a0, "a0", int_num, argument);
		    
		}
		else if(var->get_data_type() == double_data_type) {
			arg_register = new Register_Descriptor (f12, "f12", float_num, argument);
		}
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " print data type cannot be null");
	

	Ics_Opd * register_opd = new Register_Addr_Opd(arg_register);

	Icode_Stmt * load_stmt = NULL;
	switch(var->get_data_type())
	{
	case int_data_type:
		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
		break;

	case double_data_type:
		load_stmt = new Move_IC_Stmt(load_d, opd, register_opd);
		break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	

	Icode_Stmt *print_stmt = new Print_IC_Stmt();

      ic_list.push_back(mov_stmt);
	ic_list.push_back(load_stmt);
     ic_list.push_back(print_stmt);


	Code_For_Ast * load_code = new Code_For_Ast(ic_list, NULL);
	return * load_code;
}
/////////////////////////////////////////////////////////////////////////////////////////

Code_For_Ast & Call_Ast::compile()
{
	// Place actual parameters at $sp position and increment $sp
	int size = 0;
	int count = 0;
	Code_For_Ast * ast_code = NULL;
	Register_Descriptor * target_register = NULL;
	Icode_Stmt * mov_stmt = NULL;
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>; 

	list<Ast *>::iterator j;
	for(j = actual_param_list.begin(); j != actual_param_list.end(); j++, count++);

	list<Ast *>::reverse_iterator i;
	for(i = actual_param_list.rbegin(); i != actual_param_list.rend(); i++, count--)
	{
		ast_code = &((*i)->compile());
		list<Icode_Stmt *> & ic_list_temp = ast_code->get_icode_list();

		if (ic_list.empty() != true)
			ic_list.splice(ic_list.end(), ic_list_temp);
		else{
			ic_list = ic_list_temp;
		}
		// get ast_code target register
		target_register = ast_code->get_reg();
		CHECK_INVARIANT(target_register, "Target register cannot be null");
		target_register->set_use_for_expr_result();

		// store its content to size($sp)
		Data_Type dt = (*i)->get_data_type();
		Procedure * proc = program_object.get_procedure_prototype(procedure_name);
		string name = proc->get_variable_in_formal_list(count);
		Symbol_Table_Entry * arg_entry = new Symbol_Table_Entry(name, dt, lineno, sp_ref);
		arg_entry->set_symbol_scope(formal);
		arg_entry->set_start_offset(size);

		Mem_Addr_Opd * arg_mem = new Mem_Addr_Opd(*arg_entry);
		Register_Addr_Opd * result_reg = new Register_Addr_Opd(target_register);

		Move_IC_Stmt * store_stmt = NULL;
		if (target_register->get_use_category() == int_reg)
			store_stmt = new Move_IC_Stmt(store, result_reg, arg_mem);
		else if (target_register->get_use_category() == float_reg)
			store_stmt = new Move_IC_Stmt(store_d, result_reg, arg_mem);
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");

		ic_list.push_back(store_stmt);

		// if target register is int_reg, then size = size - 4
		// else size = size - 8
		// store_stmt->print_assembly();
		if (target_register->get_use_category() == int_reg)
			size = size - 4;
		else if (target_register->get_use_category() == float_reg)
			size = size - 8;
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Target register can be only int_reg or float_reg");

		target_register->reset_use_for_expr_result();
	}

	machine_desc_object.clear_local_register_mappings();
	machine_desc_object.validate_init_local_register_mapping_before_fn_call();

	Control_Flow_IC_Stmt * call_fn = new Control_Flow_IC_Stmt(jal, NULL, NULL, procedure_name, -size);

	if (node_data_type != void_data_type)
	{
		// prepare target register
		target_register = NULL;
		if (node_data_type == int_data_type)
			target_register = machine_desc_object.get_new_register<int_reg>();
		else if (node_data_type == double_data_type)
			target_register = machine_desc_object.get_new_register<float_reg>();
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");

		target_register->set_use_for_expr_result();
		target_register->set_used_for_fn_return();

		Ics_Opd * result_reg = new Register_Addr_Opd(target_register);

		// prepare source register
		Register_Descriptor * src_reg = NULL;
		if (node_data_type == int_data_type)
			src_reg = machine_desc_object.spim_register_table[v1];
		else if (node_data_type == double_data_type)
			src_reg = machine_desc_object.spim_register_table[f0];
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");

		src_reg->reset_used_for_fn_return();

		set_register(src_reg);

		Ics_Opd * source_reg = new Register_Addr_Opd(src_reg);

		// prepare move statement
		if (node_data_type == int_data_type)
			mov_stmt = new Move_IC_Stmt(mov, source_reg, result_reg);
		else if (node_data_type == double_data_type)
			mov_stmt = new Move_IC_Stmt(move_d, source_reg, result_reg);
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");

		src_reg->reset_use_for_expr_result();
		target_register->reset_use_for_expr_result();
	}

	// Prepare code_for_ast

	if (ast_code == NULL)
		ast_code = new Code_For_Ast();

	if (ic_list.empty() == true)
		ic_list = ast_code->get_icode_list();

	if (call_fn != NULL)
		ic_list.push_back(call_fn);
	if (mov_stmt != NULL)
		ic_list.push_back(mov_stmt);

	ast_code->set_reg(target_register);
	ast_code->set_icode_list(&ic_list);

	return *ast_code;

}


Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Return_Ast::compile()
{
	Code_For_Ast & ret_code = *new Code_For_Ast();


	if (return_value != NULL)
	{
		ret_code = return_value->compile();
		Register_Descriptor * reg_old = ret_code.get_reg();
		Ics_Opd * source_reg = new Register_Addr_Opd(reg_old);

		Register_Descriptor * target_reg = NULL;
		if (reg_old->get_use_category() == int_reg)
			target_reg = machine_desc_object.spim_register_table[v1];
		else if (reg_old->get_use_category() == float_reg)
			target_reg = machine_desc_object.spim_register_table[f0];

		Ics_Opd * return_reg = new Register_Addr_Opd(target_reg);

		Icode_Stmt * mov_stmt = NULL;
		if (node_data_type == int_data_type)
			mov_stmt = new Move_IC_Stmt(mov, source_reg, return_reg);
		else if (node_data_type == double_data_type)
			mov_stmt = new Move_IC_Stmt(move_d, source_reg, return_reg);
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");

		list<Icode_Stmt *> & ic_list = ret_code.get_icode_list();
		ic_list.push_back(mov_stmt);

		reg_old->reset_use_for_expr_result();
		target_reg->reset_use_for_expr_result();
		target_reg->set_used_for_fn_return();
	}

	string new_label = "epilogue_" + proc_name;
        Ics_Opd * zero_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
        Icode_Stmt * label_stmt = new Control_Flow_IC_Stmt(j, NULL, zero_opd, new_label);

	list<Icode_Stmt *> & ic_list = ret_code.get_icode_list();
	ic_list.push_back(label_stmt);

	return ret_code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}
