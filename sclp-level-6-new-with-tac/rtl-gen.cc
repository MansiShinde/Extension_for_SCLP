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

////////////////////////////////////////////////////////////////

RTL_For_TAC & Asgn_TAC_Stmt::gen_rtl()
{
	/* 
		An assignment x = y where y is a variable is 
		compiled as a combination of load and store statements:
		(load) R <- y 
		(store) x <- R
		If y is a numeric constant, the statement is compiled as:
		(imm_Load) R <- y 
		(store) x <- R
		where imm_Load denotes the load immediate operation.
		If y is a string constant, the statement is compiled as:
		(load_address) R <- y 
		(store) x <- R
		where load_address denotes the load address operation.
	*/
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((opd1 != NULL), "Rhs cannot be null");


	//print_tac_stmt(cout);		// FIX: Only for testing. TEST

	RTL_For_TAC & load_code = opd1->create_load_rtl_list();

	//load_stmt.print_rtl_stmt_list(cout); // FIX: Only for testing. TEST

	Register_Descriptor * load_register = load_code.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	load_register->set_use_for_expr_result();

	RTL_For_TAC store_code = result->create_store_rtl_list(load_register);
	//store_stmt.print_rtl_stmt_list(cout); // FIX: Only for testing. TEST

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null");

	//CHECK_INVARIANT (result->is_a_variable(), "We expect the result of COPY_TAC to be variable, always");
	
	load_register->reset_use_for_expr_result();


	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (load_code.get_rtl_list().empty() == false)
		rtl_list = load_code.get_rtl_list();

	if (store_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), store_code.get_rtl_list());
	
	RTL_For_TAC * assign_code;
	if (rtl_list.empty() == false)
		assign_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	/* After a assignment statement, the local register is freed. 
           So we set no_register as the result register for RTL_FOR_TAC.
	   CHECK: This will change when we do global register allocation.
        */


	return *assign_code;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & If_Goto_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((opd1 != NULL), "Rhs cannot be null");

	RTL_For_TAC & load_code = opd1->create_load_rtl_list(); 

	list<RTL_Stmt *> & rtl_list = load_code.get_rtl_list();

	Register_Descriptor * dest_register_id = load_code.get_reg();
	RTL_Opd * dest_reg_opd = new Register_Opd(dest_register_id);

	RTL_Opd * reg2_opd = NULL;

	RTL_Stmt * if_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_bgtz, dest_reg_opd, reg2_opd, result->get_label_name());

	dest_register_id->reset_use_for_expr_result();

	rtl_list.push_back(if_rtl_stmt);

	RTL_For_TAC * if_stmt;
	if (rtl_list.empty() == false)
		if_stmt = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return *if_stmt;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Goto_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	RTL_Opd * dest_reg_opd = NULL, * reg2_opd =NULL;
	RTL_Stmt * goto_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_goto, dest_reg_opd, reg2_opd, result->get_label_name());

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(goto_rtl_stmt);

	RTL_For_TAC * goto_code;
	if (rtl_list.empty() == false)
		goto_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return * goto_code;
	//exit(1);
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Label_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((label_opd != NULL), "Label cannot be null");
	RTL_Stmt * label_rtl_stmt = new Label_RTL_Stmt(rtl_label, label_opd->get_label_name()); 
	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(label_rtl_stmt);

	RTL_For_TAC * label_code;
	if (rtl_list.empty() == false)
		label_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return * label_code;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Return_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((opd1 != NULL), "Rhs cannot be null");
	CHECK_INVARIANT((proc_name != ""), "Procedure name cannot be null");
	Procedure * proc = program_object.get_procedure_entry(proc_name);
	CHECK_INVARIANT((proc->get_return_type() != void_data_type), "Return TAC statement in a void procedure");

	RTL_For_TAC & load_code = opd1->create_load_rtl_list(proc->get_return_register());

	Register_Descriptor * load_register = load_code.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	load_register->set_use_for_expr_result();


	RTL_Opd *result_opd = new Register_Opd(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null");
	load_register->reset_use_for_expr_result();

	RTL_Opd * reg2_opd = NULL;

	RTL_Stmt * return_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_return, result_opd, reg2_opd, proc_name);

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (load_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(),load_code.get_rtl_list());
	
	rtl_list.push_back(return_rtl_stmt);
	
	RTL_For_TAC * return_code;
	if (rtl_list.empty() == false)
		return_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	CHECK_INVARIANT(return_code!=NULL, "Return statement cannot be NULL");
	
	return * return_code;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Call_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((proc_name != ""), "Procedure name cannot be null");
	CHECK_INVARIANT((result != NULL), "Result cannot be null");

	Register_Descriptor * param_register_id, * result_register_id;
	RTL_Opd * param_opd, * result_opd;
	RTL_Stmt * push_rtl_stmt, * call_rtl_stmt;
	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	int size;
	size = 0;
	Procedure * proc = program_object.get_procedure_entry(proc_name);

	list<TAC_Opd *>::reverse_iterator i;
	for(i = actual_param_list.rbegin(); i != actual_param_list.rend(); i++)
	{
		Basic_Data_Type dt = (*i)->get_basic_data_type();
		switch (dt)
		{
			case int_data_type:
			case string_data_type:
			case bool_data_type:
				size = size + 4;
				break;
			case double_data_type:
				size = size + 8;
				break;
			default:
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "wrong data type");
				break;
		}
		
		// A param may be a constant too hence by default we generate a load statement.

		RTL_For_TAC & load_code = (*i)->create_load_rtl_list();

		param_register_id = load_code.get_reg();
		CHECK_INVARIANT(param_register_id, "Load register cannot be null");
		param_register_id->set_use_for_expr_result();
		param_opd = new Register_Opd(param_register_id);

		if (load_code.get_rtl_list().empty() == false)
			rtl_list.splice(rtl_list.end(),load_code.get_rtl_list());

		push_rtl_stmt = new Move_RTL_Stmt(rtl_push, param_opd, NULL);
		rtl_list.push_back(push_rtl_stmt);
	}

	if(proc->get_return_type() != void_data_type)
	{
		result_register_id = proc->get_return_register();
		result->set_register_of_temporary(result_register_id);
		result_opd = new Register_Opd(result_register_id);
	}
	else 
		result_opd = NULL;


	call_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_call, result_opd, NULL, proc_name, size);
	if (result_opd)
		result_register_id->reset_use_for_expr_result();
	rtl_list.push_back(call_rtl_stmt);

	RTL_For_TAC * call_code;
	if (rtl_list.empty() == false)
		call_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return *call_code;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & IO_TAC_Stmt::gen_rtl()
{

	if (tac_operator == read_3a)
	{ 
		//cout << "found a read I/O TAC " << endl; // TEST
		RTL_For_TAC & i_o_code = gen_rtl_for_read();
		return i_o_code;
	}
	else if (tac_operator == write_3a)
	{
		//cout << "found a write I/O TAC " << endl; // TEST
		RTL_For_TAC & i_o_code = gen_rtl_for_write();
		return i_o_code;
	}
	else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong operator in IO TAC");
}

//////////////////////////////////////////////////////////////////////////////
RTL_For_TAC & IO_TAC_Stmt::gen_rtl_for_write()
{
	// Write statement is compiled into a system call which requires a sequence of three statements:
	// The first statement loads the syscall number into register v0. The second statement moves the 
	// variable to be printed into register a0 for int and f12 for float. The third statement is
        // the actual syscall.

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	Register_Descriptor *result_register = new Register_Descriptor (v0, "v0", int_num, int_reg);
	RTL_Opd * load_register = new Register_Opd(result_register);
	RTL_Opd * print_opd;

 	Move_RTL_Stmt * mov_rtl_stmt;
	RTL_Stmt * load_rtl_stmt;

	if(opd1->get_basic_data_type() == int_data_type) 
        {
	    // Generate instruction to load syscall number 1 into register v0.

	    RTL_Opd * syscall_opd = new Int_Const_Opd(1);
            mov_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, syscall_opd, load_register);
	    mov_rtl_stmt->set_comment("Loading 1 in v0 to indicate syscall to print integter value");

	    // Generate instruction to move the variable into register a0.

            Register_Descriptor * arg_register = new Register_Descriptor (a0, "a0", int_num, argument);
	    load_rtl_stmt = (opd1->create_load_rtl_list(arg_register)).get_first_rtl_stmt();

	    load_rtl_stmt->set_comment("Moving the value to be printed into register a0");
        }
        else if(opd1->get_basic_data_type() == double_data_type) 
        {
	    // Generate instruction to load syscall number 3 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(3);
            mov_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, syscall_opd, load_register);
	    mov_rtl_stmt->set_comment("Loading 3 in v0 to indicate syscall to print double value");

	    // Generate instruction to move the variable into register f12.

            Register_Descriptor * arg_register = new Register_Descriptor (f12, "f12", float_num, argument);
	    load_rtl_stmt = (opd1->create_load_rtl_list(arg_register)).get_first_rtl_stmt();
	    load_rtl_stmt->set_comment("Moving the value to be printed into register f12");
        }
        else if(opd1->get_basic_data_type() == string_data_type) 
        {
	    // Generate instruction to load syscall number 4 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(4);
            mov_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, syscall_opd, load_register);
	    mov_rtl_stmt->set_comment("Loading 4 in v0 to indicate syscall to print string value");

	    // Generate instruction to move the address of the string into register a0.

            Register_Descriptor * arg_register = new Register_Descriptor (a0, "a0", int_num, argument);

	    load_rtl_stmt = (opd1->create_load_rtl_list(arg_register)).get_first_rtl_stmt();
        }
        else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " print data type cannot be null");

	RTL_Stmt *write_rtl_stmt = new Write_RTL_Stmt();
	write_rtl_stmt->set_comment("This is where syscall will be made");

        rtl_list.push_back(mov_rtl_stmt);
	rtl_list.push_back(load_rtl_stmt);
        rtl_list.push_back(write_rtl_stmt);

	RTL_For_TAC * write_code = new RTL_For_TAC(rtl_list, NULL);
	return * write_code;
}

////////////////////////////////////////////////////////////////

RTL_For_TAC & IO_TAC_Stmt::gen_rtl_for_read()
{
	// A read statement is compiled into a system call which requires a sequence of three statements:
	// The first statement loads the syscall number into register v0. The second statement is the syscall
	// which reads the value register v0 (integer value) or f0 (double value). The third statement stores 
	// the value into the desired variables. 
	
	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	Register_Descriptor *result_register = new Register_Descriptor (v0, "v0", int_num, int_reg);
	RTL_Opd * load_register = new Register_Opd(result_register);
	RTL_Opd * read_opd = new Mem_Opd(*(opd1->get_symbol_entry()));

 	Move_RTL_Stmt * mov_rtl_stmt, * load_rtl_stmt;

	if(opd1->get_basic_data_type() == int_data_type) 
        {
	    // Generate instruction to load syscall number 5 into register v0.

	    RTL_Opd * syscall_opd = new Int_Const_Opd(5);
            mov_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, syscall_opd, load_register);
	    mov_rtl_stmt->set_comment("Loading 5 in v0 to indicate syscall to read integter value");

	    // Generate instruction to move the variable from register v0.

            Register_Descriptor * arg_register = new Register_Descriptor (v0, "v0", int_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);
	    load_rtl_stmt = new Move_RTL_Stmt(rtl_store, register_opd, read_opd);
	    load_rtl_stmt->set_comment("Moving the read value from register v0");
        }
        else if(opd1->get_basic_data_type() == double_data_type) 
        {
	    // Generate instruction to load syscall number 7 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(7);
            mov_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, syscall_opd, load_register);
	    mov_rtl_stmt->set_comment("Loading 7 in v0 to indicate syscall to read double value");

	    // Generate instruction to move the variable from register f0.

            Register_Descriptor * arg_register = new Register_Descriptor (f0, "f0", float_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);
	    load_rtl_stmt = new Move_RTL_Stmt(rtl_store_d, register_opd, read_opd);
	    load_rtl_stmt->set_comment("Moving the read value from register f0");
        }
        else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " read data type cannot be null");

	RTL_Stmt *read_rtl_stmt = new Read_RTL_Stmt();
	read_rtl_stmt->set_comment("This is where syscall will be made");

	rtl_list.push_back(mov_rtl_stmt);
        rtl_list.push_back(read_rtl_stmt);
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC * read_code = new RTL_For_TAC(rtl_list, NULL);
	return * read_code;
}


/////////////////////////////////////////////////////////////////

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Variable_TAC_Opd::create_load_rtl_list()		
{
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	/* Four steps:
	 * (a) Get a new register_id 
	 * (b) Create a new destination register operand 
	 * (c) Create a source operand for the variable 
	 * (d) Create a load instruction with the destination 
	 *     register operand and source operand
	*/

	Basic_Data_Type data_type = symbol_entry->get_basic_data_type();

	Register_Descriptor * dest_register_id = NULL;

	//cout << "Seeking register for creating load statement " << endl; // TEST

	switch(data_type)
	{
		// The destination register type is same for both string variable (which contains
		// the address of the string) and an integer number.
		case int_data_type:
		case string_data_type:
		case bool_data_type:
			dest_register_id = machine_desc_object.get_new_register<int_reg>();
			break;
	
		case double_data_type: 
			dest_register_id = machine_desc_object.get_new_register<float_reg>();
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	CHECK_INVARIANT((dest_register_id != NULL), "Result register cannot be null");

	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Mem_Opd(*symbol_entry);

	RTL_Stmt * load_rtl_stmt = NULL;

	switch(data_type)
	{
	case int_data_type:
	case string_data_type:
	case bool_data_type:
		load_rtl_stmt = new Move_RTL_Stmt(rtl_load, source_opd, dest_register_opd);
		break;

	case double_data_type:
		load_rtl_stmt = new Move_RTL_Stmt(rtl_load_d, source_opd, dest_register_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	load_rtl_stmt->set_asm_comment("Source:" + get_operand_string());

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}


// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions.
// It differs from the previous version in that it takes a register id as an argument.

RTL_For_TAC & Variable_TAC_Opd::create_load_rtl_list(Register_Descriptor *dest_register_id)
{
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.
	
	Basic_Data_Type data_type = symbol_entry->get_basic_data_type();

	CHECK_INVARIANT((dest_register_id != NULL), "Result register cannot be null");

	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Mem_Opd(*symbol_entry);

	RTL_Stmt * load_rtl_stmt = NULL;

	switch(data_type)
	{
	case int_data_type:
	case string_data_type:
	case bool_data_type:
		load_rtl_stmt = new Move_RTL_Stmt(rtl_load, source_opd, dest_register_opd);
		break;

	case double_data_type:
		load_rtl_stmt = new Move_RTL_Stmt(rtl_load_d, source_opd, dest_register_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");
	load_rtl_stmt->set_asm_comment("Source:" + get_operand_string());

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Variable_TAC_Opd::create_store_rtl_list(Register_Descriptor * source_register)
{
	CHECK_INVARIANT((source_register != NULL), "Store register cannot be null");
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	RTL_Opd * source_register_opd = new Register_Opd(source_register);
	RTL_Opd * dest_opd = new Mem_Opd(*symbol_entry);

	Basic_Data_Type data_type = symbol_entry->get_basic_data_type();

	RTL_Stmt * store_rtl_stmt = NULL;
	switch(data_type)
	{
	case int_data_type:
	case bool_data_type:
	case string_data_type:
		store_rtl_stmt = new Move_RTL_Stmt(rtl_store, source_register_opd, dest_opd);
		break;

	case double_data_type:
		store_rtl_stmt = new Move_RTL_Stmt(rtl_store_d, source_register_opd, dest_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((store_rtl_stmt != NULL), "Store statement cannot be null");
	store_rtl_stmt->set_asm_comment("Dest: " + get_operand_string());

	symbol_entry->update_register(source_register);
	
	// We cannot reset the register useage as soon as a variable is loaded because then
	// the register may be used for some other operand within the same expression. Hence 
	// a register should be reset only when it's use is over which would happen in an assignment
	// statement. In other words, assignment to temporaries is not an assignment after which
	// registers can be feed.
	// source_register->reset_use_for_expr_result();

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(store_rtl_stmt);

	RTL_For_TAC & name_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return name_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Array_Access_TAC_Opd::create_load_rtl_list()		
{
	/* Steps:
	 * (a) load the address of base which must be a Name_Ast (check using is_a_variable(). 
	 *     Call it reg_base. Must be of type int_num.
	 * (b) Generate the code to load the offset into a register. 
	 *     Call it reg_offset. Must be of type int_num.
	 * (c) reg_address = reg_base + reg_offset
	 * (d) Load indirect from reg_address into dest_reg (which should be of the data_type ot 
	 * (e) Join all the statements generated in steps (a) to (d).
	 *     Return the result as dest_reg.
	*/

	//// Step (a)
	Register_Descriptor * reg_base_id = machine_desc_object.get_new_register<int_reg>();
	RTL_Opd * reg_base_opd = new Register_Opd(reg_base_id);
	reg_base_id->set_use_for_expr_result();

	CHECK_INVARIANT(base->is_a_variable(), "The operand of Address_Of must be a variable");
	RTL_Opd * read_opd = new Mem_Opd(*(base->get_symbol_entry()));

	RTL_Stmt * base_addr_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, read_opd, reg_base_opd);
	base_addr_load_rtl_stmt->set_asm_comment("Loading the address of base:" + base->get_operand_string());
	
	//// Step (b) 
	RTL_For_TAC & offset_code = offset->create_load_rtl_list();
	Register_Descriptor * reg_offset = offset_code.get_reg();
	RTL_Opd * reg_offset_opd = new Register_Opd(reg_offset);

	//// Step (c)

        RTL_Stmt * add_rtl_stmt = new Compute_RTL_Stmt(rtl_add, reg_base_opd, reg_offset_opd, reg_base_opd);  // reusing base register
	add_rtl_stmt->set_asm_comment("Address of Opd1:" + base->get_operand_string() + ", offset:" + offset->get_operand_string());
	

	///// Step (d)
	// Get result register depending upon the type of reg1.
	
	Register_Descriptor * dest_reg_id = NULL;
	RTL_Opd * dest_reg_opd = NULL;
	RTL_Stmt * final_load_rtl_stmt = NULL;
	Basic_Data_Type data_type = base->get_basic_data_type();
	switch(data_type)
	{
		case int_data_type:
		case string_data_type:
		case bool_data_type:
			dest_reg_id = machine_desc_object.get_new_register<int_reg>();
			CHECK_INVARIANT((dest_reg_id != NULL), "Result register cannot be null");
			dest_reg_opd = new Register_Opd(dest_reg_id);
			final_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind, reg_base_opd, dest_reg_opd);
			break;
	
		case double_data_type: 
			dest_reg_id = machine_desc_object.get_new_register<float_reg>();
			CHECK_INVARIANT((dest_reg_id != NULL), "Result register cannot be null");
			dest_reg_opd = new Register_Opd(dest_reg_id);
			final_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind_d, reg_base_opd, dest_reg_opd);
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	final_load_rtl_stmt->set_asm_comment("Indirect load of the final result");


	list<RTL_Stmt *> rtl_list;

	rtl_list.push_back(base_addr_load_rtl_stmt); // From step (a)
	if (offset_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), offset_code.get_rtl_list()); //From step (b)
	rtl_list.push_back(add_rtl_stmt); // From step (c)
	rtl_list.push_back(final_load_rtl_stmt); // From step (d)

	RTL_For_TAC & array_access_code = *new RTL_For_TAC(rtl_list, dest_reg_id);

	return array_access_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Field_Access_TAC_Opd::create_load_rtl_list()		
{
	// This code is exactly identical to create_load_rtl_list for Array_Access_Opd. There is
	// no difference in the code other than this comment.

	/* Steps:
	 * (a) load the address of base which must be a Name_Ast (check using is_a_variable(). 
	 *     Call it reg_base. Must be of type int_num.
	 * (b) Generate the code to load the offset into a register. 
	 *     Call it reg_offset. Must be of type int_num.
	 * (c) reg_address = reg_base + reg_offset
	 * (d) Load indirect from reg_address into dest_reg (which should be of the data_type ot 
	 * (e) Join all the statements generated in steps (a) to (d).
	 *     Return the result as dest_reg.
	*/

	//// Step (a)
	Register_Descriptor * reg_base_id = machine_desc_object.get_new_register<int_reg>();
	RTL_Opd * reg_base_opd = new Register_Opd(reg_base_id);
	reg_base_id->set_use_for_expr_result();

	CHECK_INVARIANT(base->is_a_variable(), "The operand of Address_Of must be a variable");
	RTL_Opd * read_opd = new Mem_Opd(*(base->get_symbol_entry()));

	RTL_Stmt * base_addr_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, read_opd, reg_base_opd);
	base_addr_load_rtl_stmt->set_asm_comment("Loading the address of base:" + base->get_operand_string());
	
	//// Step (b) 
	RTL_For_TAC & offset_code = offset->create_load_rtl_list();
	Register_Descriptor * reg_offset = offset_code.get_reg();
	RTL_Opd * reg_offset_opd = new Register_Opd(reg_offset);

	//// Step (c)

        RTL_Stmt * add_rtl_stmt = new Compute_RTL_Stmt(rtl_add, reg_base_opd, reg_offset_opd, reg_base_opd);  // reusing base register
	add_rtl_stmt->set_asm_comment("Address of Opd1:" + base->get_operand_string() + ", offset:" + offset->get_operand_string());
	

	///// Step (d)
	// Get result register depending upon the type of reg1.
	
	Register_Descriptor * dest_reg_id = NULL;
	RTL_Opd * dest_reg_opd = NULL;
	RTL_Stmt * final_load_rtl_stmt = NULL;
	Basic_Data_Type data_type = base->get_basic_data_type(); ///////////////////


	switch(data_type)
	{
		case int_data_type:
		case string_data_type:
		case bool_data_type:
			dest_reg_id = machine_desc_object.get_new_register<int_reg>();
			CHECK_INVARIANT((dest_reg_id != NULL), "Result register cannot be null");
			dest_reg_opd = new Register_Opd(dest_reg_id);
			final_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind, reg_base_opd, dest_reg_opd);
			break;
	
		case double_data_type: 
			dest_reg_id = machine_desc_object.get_new_register<float_reg>();
			CHECK_INVARIANT((dest_reg_id != NULL), "Result register cannot be null");
			dest_reg_opd = new Register_Opd(dest_reg_id);
			final_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind_d, reg_base_opd, dest_reg_opd);
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	final_load_rtl_stmt->set_asm_comment("Indirect load of the final result");


	list<RTL_Stmt *> rtl_list;

	rtl_list.push_back(base_addr_load_rtl_stmt); // From step (a)
	if (offset_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), offset_code.get_rtl_list()); //From step (b)
	rtl_list.push_back(add_rtl_stmt); // From step (c)
	rtl_list.push_back(final_load_rtl_stmt); // From step (d)

	RTL_For_TAC & array_access_code = *new RTL_For_TAC(rtl_list, dest_reg_id);

	return array_access_code;
}


// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Pointer_Deref_TAC_Opd::create_store_rtl_list(Register_Descriptor * source_register)
{
	CHECK_INVARIANT((source_register != NULL), "Store register cannot be null");
	RTL_Opd * source_register_opd = new Register_Opd(source_register);

	RTL_Opd * dest_opd;

	Basic_Data_Type data_type = base->get_basic_data_type();

	RTL_Op rtl_op;
	switch(data_type)
	{
		case int_data_type:
		case bool_data_type:
		case string_data_type:
			rtl_op = rtl_store_ind; break;
		case double_data_type:
			rtl_op = rtl_store_ind_d; break;
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	// This where we do not know the register of the temporary.
	// We need to load it in a new register and use that register as dest_Opd.
	RTL_For_TAC & load_code = base->create_load_rtl_list();

	Register_Descriptor * load_register = load_code.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	dest_opd = new Register_Opd(load_register);
	
	// load_register->set_use_for_expr_result();
	// No need to keep the register busy it will be used in the (single) instruction that is generation
	// right after this if statement. It is guaranteed that no instruction is generated between this
	// point and the point of generation of store instruction below.

	RTL_Stmt * store_rtl_stmt = NULL;
	store_rtl_stmt = new Move_RTL_Stmt(rtl_op, source_register_opd, dest_opd);

	CHECK_INVARIANT((store_rtl_stmt != NULL), "Store statement cannot be null");
	store_rtl_stmt->set_asm_comment("Indirect store into Dest: " + get_operand_string());

	
	// We cannot reset the register useage as soon as a variable is loaded because then
	// the register may be used for some other operand within the same expression. Hence 
	// a register should be reset only when it's use is over which would happen in an assignment
	// statement. In general, assignment to temporaries is not an assignment after which
	// registers can be feed. However, in this case, this is an indirect assignment and is
	// a part of the assignment statement, hence the register can be freed.

	source_register->reset_use_for_expr_result();

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (load_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), load_code.get_rtl_list()); 

	rtl_list.push_back(store_rtl_stmt);

	RTL_For_TAC & store_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return store_code;
}


RTL_For_TAC & Array_Access_TAC_Opd::create_store_rtl_list(Register_Descriptor * source_register)
{
	// This is same as create_load_rtl_list except that step (d) is different.
	// Instead of load, it uses store and uses the source_register as the source.

	/* Steps:
	 * (a) load the address of base which must be a Name_Ast (check using is_a_variable(). 
	 *     Call it reg_base. Must be of type int_num.
	 * (b) Generate the code to load the offset into a register. 
	 *     Call it reg_offset. Must be of type int_num.
	 * (c) reg_address = reg_base + reg_offset
	 * (d) Store indirect from source register into reg_address 
	 * (e) Join all the statements generated in steps (a) to (d).
	 *     Return the result as no_reg.
	*/

	//// Step (a)
	Register_Descriptor * reg_base_id = machine_desc_object.get_new_register<int_reg>();
	RTL_Opd * reg_base_opd = new Register_Opd(reg_base_id);
	reg_base_id->set_use_for_expr_result();

	CHECK_INVARIANT(base->is_a_variable(), "The operand of Address_Of must be a variable");
	RTL_Opd * read_opd = new Mem_Opd(*(base->get_symbol_entry()));

	RTL_Stmt * base_addr_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, read_opd, reg_base_opd);
	base_addr_load_rtl_stmt->set_asm_comment("Loading the address of base:" + base->get_operand_string());
	
	//// Step (b) 
	RTL_For_TAC & offset_code = offset->create_load_rtl_list();
	Register_Descriptor * reg_offset = offset_code.get_reg();
	RTL_Opd * reg_offset_opd = new Register_Opd(reg_offset);

	//// Step (c)

        RTL_Stmt * add_rtl_stmt = new Compute_RTL_Stmt(rtl_add, reg_base_opd, reg_offset_opd, reg_base_opd);  // reusing base register
	add_rtl_stmt->set_asm_comment("Address of Opd1:" + base->get_operand_string() + ", offset:" + offset->get_operand_string());
	

	///// Step (d)
	
	RTL_Opd * dest_reg_opd = reg_base_opd;
	RTL_Opd * source_reg_opd = new Register_Opd(source_register);

	RTL_Stmt * store_rtl_stmt = NULL;
	Basic_Data_Type data_type = base->get_basic_data_type();
	switch(data_type)
	{
		case int_data_type:
		case string_data_type:
		case bool_data_type:
			store_rtl_stmt = new Move_RTL_Stmt(rtl_store_ind, source_reg_opd, dest_reg_opd);
			break;
	
		case double_data_type: 
			store_rtl_stmt = new Move_RTL_Stmt(rtl_store_ind_d, source_reg_opd, dest_reg_opd);
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	store_rtl_stmt->set_asm_comment("Indirect store into Dest: " + get_operand_string());

	//// Step (e)

	list<RTL_Stmt *> rtl_list;

	rtl_list.push_back(base_addr_load_rtl_stmt); // From step (a)
	if (offset_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), offset_code.get_rtl_list()); //From step (b)
	rtl_list.push_back(add_rtl_stmt); // From step (c)
	rtl_list.push_back(store_rtl_stmt); // From step (d)

	RTL_For_TAC & store_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return store_code;
}

RTL_For_TAC & Field_Access_TAC_Opd::create_store_rtl_list(Register_Descriptor * source_register)
{
	// This code is exactly identical to create_load_rtl_list for Array_Access_Opd. There is
	// no difference in the code other than this comment.

	// This is same as create_load_rtl_list except that step (d) is different.
	// Instead of load, it uses store and uses the source_register as the source.

	/* Steps:
	 * (a) load the address of base which must be a Name_Ast (check using is_a_variable(). 
	 *     Call it reg_base. Must be of type int_num.
	 * (b) Generate the code to load the offset into a register. 
	 *     Call it reg_offset. Must be of type int_num.
	 * (c) reg_address = reg_base + reg_offset
	 * (d) Store indirect from source register into reg_address 
	 * (e) Join all the statements generated in steps (a) to (d).
	 *     Return the result as no_reg.
	*/

	//// Step (a)
	Register_Descriptor * reg_base_id = machine_desc_object.get_new_register<int_reg>();
	RTL_Opd * reg_base_opd = new Register_Opd(reg_base_id);
	reg_base_id->set_use_for_expr_result();

	CHECK_INVARIANT(base->is_a_variable(), "The operand of Address_Of must be a variable");
	RTL_Opd * read_opd = new Mem_Opd(*(base->get_symbol_entry()));

	RTL_Stmt * base_addr_load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, read_opd, reg_base_opd);
	base_addr_load_rtl_stmt->set_asm_comment("Loading the address of base:" + base->get_operand_string());
	
	//// Step (b) 
	RTL_For_TAC & offset_code = offset->create_load_rtl_list();
	Register_Descriptor * reg_offset = offset_code.get_reg();
	RTL_Opd * reg_offset_opd = new Register_Opd(reg_offset);

	//// Step (c)

        RTL_Stmt * add_rtl_stmt = new Compute_RTL_Stmt(rtl_add, reg_base_opd, reg_offset_opd, reg_base_opd);  // reusing base register
	add_rtl_stmt->set_asm_comment("Address of Opd1:" + base->get_operand_string() + ", offset:" + offset->get_operand_string());
	

	///// Step (d)
	
	RTL_Opd * dest_reg_opd = reg_base_opd;
	RTL_Opd * source_reg_opd = new Register_Opd(source_register);

	RTL_Stmt * store_rtl_stmt = NULL;
	Basic_Data_Type data_type = base->get_basic_data_type();
	switch(data_type)
	{
		case int_data_type:
		case string_data_type:
		case bool_data_type:
			store_rtl_stmt = new Move_RTL_Stmt(rtl_store_ind, source_reg_opd, dest_reg_opd);
			break;
	
		case double_data_type: 
			store_rtl_stmt = new Move_RTL_Stmt(rtl_store_ind_d, source_reg_opd, dest_reg_opd);
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}
	store_rtl_stmt->set_asm_comment("Indirect store into Dest: " + get_operand_string());

	//// Step (e)

	list<RTL_Stmt *> rtl_list;

	rtl_list.push_back(base_addr_load_rtl_stmt); // From step (a)
	if (offset_code.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), offset_code.get_rtl_list()); //From step (b)
	rtl_list.push_back(add_rtl_stmt); // From step (c)
	rtl_list.push_back(store_rtl_stmt); // From step (d)

	RTL_For_TAC & store_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return store_code;
}


///////////////////////////////////////////////////////////////////////////////

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Int_Const_TAC_Opd::create_load_rtl_list()
{
	Register_Descriptor * dest_register_id;
	
	dest_register_id = machine_desc_object.get_new_register<int_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Int_Const_Opd(num);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("Loading integer number " + to_string(num));

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	load_rtl_stmt->set_asm_comment("Source:" + get_operand_string());

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Int_Const_TAC_Opd::create_load_rtl_list(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.
	
	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Int_Const_Opd(num);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("Loading integer number " + to_string(num));


	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}


///////////////////////////////////////////////////////////////////////////////

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Double_Const_TAC_Opd::create_load_rtl_list()
{
	Register_Descriptor * dest_register_id;
	dest_register_id = machine_desc_object.get_new_register<float_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Double_Const_Opd(num);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm_d, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("Loading float number " + to_string(num));

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Double_Const_TAC_Opd::create_load_rtl_list(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Double_Const_Opd(num);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_imm_d, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("Loading float number " + to_string(num));

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}


///////////////////////////////////////////////////////////////////////////////

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Str_Const_TAC_Opd::create_load_rtl_list()
{
	Register_Descriptor * dest_register_id;
	
	dest_register_id = machine_desc_object.get_new_register<int_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Str_Const_Opd(str_key);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("String = \"" + program_object.get_string(str_key) + "\"");

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & str_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return str_code;
}


// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Str_Const_TAC_Opd::create_load_rtl_list(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Str_Const_Opd(str_key);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, source_opd, dest_register_opd);
	load_rtl_stmt->set_comment("String = \"" + program_object.get_string(str_key) + "\"");

	CHECK_INVARIANT((load_rtl_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & str_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return str_code;
}

///////////////////////////////////////////////////////////////////////////////////////


// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Temporary_TAC_Opd::create_load_rtl_list()
{
	// No need to load a temporary in a new register. It is already in a register.
	
	Register_Descriptor * dest_register_id = get_register_of_temporary();

	CHECK_INVARIANT((dest_register_id != NULL), "Result register cannot be null");

	RTL_Stmt * load_rtl_stmt = NULL;

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}


// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Temporary_TAC_Opd::create_load_rtl_list(Register_Descriptor *dest_register_id)
{
	// No need to load a temporary in a new register. It is already in a register.

	Register_Descriptor * source_register_id = get_register_of_temporary();

	CHECK_INVARIANT((source_register_id != NULL), "Result register cannot be null");

	RTL_Opd * source_opd = new Register_Opd(source_register_id);
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Stmt * load_rtl_stmt = new Move_RTL_Stmt(rtl_move, source_opd, dest_register_opd);
	load_rtl_stmt->set_asm_comment("Source:" + get_operand_string());

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_rtl_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}

// This creates only a single RTL statement but we keep the list for uniformity gen_rtl functions

RTL_For_TAC & Temporary_TAC_Opd::create_store_rtl_list(Register_Descriptor * source_register)
{
	CHECK_INVARIANT((source_register != NULL), "Store register cannot be null");

	RTL_Stmt * store_rtl_stmt = NULL;

	set_register_of_temporary(source_register);

	// CHECK: Reset may have to be changed if a temporary is used multiple times.

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	// There is no statement in RTL_For_TAC for a temporary.
	// Also there is no register either because register is stored
	// in the object corresponding to the temporary.
	
	rtl_list.push_back(store_rtl_stmt);

	RTL_For_TAC & name_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return name_code;
}


///////////////////////////////////////////////////////////////////////////////
RTL_For_TAC & NOP_TAC_Stmt :: gen_rtl()
{
	RTL_For_TAC * nop_code;
	list<RTL_Stmt *> & rtl_list =  *new list<RTL_Stmt *>;
	rtl_list.push_back(new NOP_RTL_Stmt());
	nop_code = new RTL_For_TAC(rtl_list, NULL);
}

RTL_For_TAC & Compute_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((opd1 != NULL), "The first operand of Rhs cannot be null");

	RTL_For_TAC & opd1_code = opd1->create_load_rtl_list();
	Register_Descriptor * reg1 = opd1_code.get_reg();
	Register_Descriptor * reg2 = NULL;
	CHECK_INVARIANT(reg1, "Load register cannot be null");
	RTL_Opd * reg1_opd = new Register_Opd(reg1);

	// Get result register depending upon the type of reg1.
	
	Register_Val_Type reg_type = reg1->get_register_type();

	Register_Descriptor * dest_register_id = NULL;

	switch (reg_type)
	{
		case int_num:
			dest_register_id = machine_desc_object.get_new_register<int_reg>();
			break;
		case float_num:
			dest_register_id = machine_desc_object.get_new_register<float_reg>();
			break;
		case void_reg_type:
		default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong register type option");
			 break;
	}
	dest_register_id->set_use_for_expr_result();
	
		// The result of the TAC is a temporary and we are not goint to generate
		// a store statement for it. Hene it should record the register used 
		// to store the result so that the register is available as an operand
		// in other instructions that use the temporary as the operand.
		
	result->set_register_of_temporary(dest_register_id);	


	RTL_Opd * dest_reg_opd = new Register_Opd(dest_register_id);

	RTL_Stmt * compute_rtl_stmt;

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (opd1_code.get_rtl_list().empty() == false)
		rtl_list = opd1_code.get_rtl_list();

	string comment = "Result:" + result->get_operand_string();

	if (handles_aggregate(tac_operator))
	{
		// This is where as deal with deref_3a, copy_3a, and addr_3a operators that work on aggregates
		if (tac_operator == deref_3a) // FIX
		{
			if (reg_type == int_num)
				compute_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind, reg1_opd, dest_reg_opd);
			else if (reg_type == float_num)
				compute_rtl_stmt = new Move_RTL_Stmt(rtl_load_ind_d, reg1_opd, dest_reg_opd);
			else
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong register type option");
			compute_rtl_stmt->set_asm_comment(comment + ", Indirect load of Opd1:" + opd1->get_operand_string());
		}
		else if (tac_operator == copy_3a)
		{
			if (reg_type == int_num)
				compute_rtl_stmt = new Move_RTL_Stmt(rtl_move, reg1_opd, dest_reg_opd);
			else if (reg_type == float_num)
				compute_rtl_stmt = new Move_RTL_Stmt(rtl_move_d, reg1_opd, dest_reg_opd);
			else
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong register type option");
			compute_rtl_stmt->set_asm_comment(comment + ", Register move of Opd1:" + opd1->get_operand_string());
		}
		else if (tac_operator == addr_3a)
		{
			// We need to create mem_opd here instead of loading opd1 into a register
			// Hence we first free the register and remove the last statement from
			// the rtl list (which was the load statement)

			list<RTL_Stmt *>::iterator it;
			it = --rtl_list.end();
			rtl_list.erase(it);
  
			reg1->reset_use_for_expr_result();

			// Then we create a Mem Opd for the value to be loaded.
			CHECK_INVARIANT(opd1->is_a_variable(), "The operand of Address_Of must be a variable");
			RTL_Opd * read_opd = new Mem_Opd(*(opd1->get_symbol_entry()));

			compute_rtl_stmt = new Move_RTL_Stmt(rtl_load_addr, read_opd, dest_reg_opd);
			compute_rtl_stmt->set_asm_comment(comment + ", Loading the address of Opd1:" + opd1->get_operand_string());
		}
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong tac operator for handing aggregate data");
	}
	else
	{
		RTL_Op rtl_op = get_rtl_operator_for_scalar_tac_operator(); 
		if (is_binary_op(tac_operator))
		{
			CHECK_INVARIANT((opd2 != NULL), "The second operand of Rhs cannot be null");
			RTL_For_TAC & opd2_code = opd2->create_load_rtl_list();
			reg2 = opd2_code.get_reg();
			CHECK_INVARIANT(reg2, "Load register cannot be null");
			RTL_Opd * reg2_opd = new Register_Opd(reg2);
	
			if (opd2_code.get_rtl_list().empty() == false)
				rtl_list.splice(rtl_list.end(), opd2_code.get_rtl_list());
		        	
			// Generate an RTL statement with opd1_register and opd2_register
		        compute_rtl_stmt = new Compute_RTL_Stmt(rtl_op, reg1_opd, reg2_opd, dest_reg_opd); 
			compute_rtl_stmt->set_asm_comment(comment + ", Opd1:" + opd1->get_operand_string() + ", Opd2:" + opd2->get_operand_string());
	
		}
		else
		{
		        compute_rtl_stmt = new Compute_RTL_Stmt(rtl_op, reg1_opd, NULL, dest_reg_opd); 
			compute_rtl_stmt->set_asm_comment(comment + ", Opd1:" + opd1->get_operand_string());
		}
	}

	//print_tac_stmt(cout);		// FIX: Only for testing.
	//compute_rtl_stmt->print_rtl_stmt(cout); // FIX: Only for testing.

	rtl_list.push_back(compute_rtl_stmt);

	reg1->reset_use_for_expr_result();
	if (is_binary_op(tac_operator))
		reg2->reset_use_for_expr_result();

	RTL_For_TAC * compute_code;
	if (rtl_list.empty() == false)
		compute_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return *compute_code;
}

RTL_Opd * Variable_TAC_Opd::gen_rtl_opd()
{
	return new Mem_Opd(*get_symbol_entry());
}

RTL_Opd * Temporary_TAC_Opd::gen_rtl_opd()
{
	Register_Descriptor * register_id = get_register_of_temporary();
	return new Register_Opd(register_id);
}

RTL_Opd * Int_Const_TAC_Opd::gen_rtl_opd()
{
	return new Int_Const_Opd(num);
}

RTL_Opd * Double_Const_TAC_Opd::gen_rtl_opd()
{
	return new Double_Const_Opd(num);
}

RTL_Opd * Str_Const_TAC_Opd::gen_rtl_opd()
{
	return new Str_Const_Opd(str_key);
}

