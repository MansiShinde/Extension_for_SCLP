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
#include"rtl.hh"
#include"tac.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"procedure.hh"
#include"program.hh"

////////////////////////////////////////////////////////////////

RTL_For_TAC & Copy_TAC_Stmt::gen_rtl()
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



	RTL_For_TAC & load_stmt = opd1->create_load_stmt();

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	load_register->set_use_for_expr_result();

	RTL_For_TAC store_stmt = result->create_store_stmt(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null");
	load_register->reset_use_for_expr_result();


	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (load_stmt.get_rtl_list().empty() == false)
		rtl_list = load_stmt.get_rtl_list();

	if (store_stmt.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(), store_stmt.get_rtl_list());

	RTL_For_TAC * assign_stmt;
	if (rtl_list.empty() == false)
		assign_stmt = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	/* After a assignment statement, the local register is freed. 
           So we set no_register as the result register for RTL_FOR_TAC.
	   CHECK: This will change when we do global register allocation.
        */

	return *assign_stmt;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & If_Goto_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((opd1 != NULL), "Rhs cannot be null");

	Register_Descriptor * dest_register_id = opd1->get_register_of_temporary();
	RTL_Opd * dest_reg_opd = new Register_Opd(dest_register_id);

	RTL_Opd * reg2_opd = NULL;

	RTL_Stmt * if_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_bgtz, dest_reg_opd, reg2_opd, result->get_label_name());

	dest_register_id->reset_use_for_expr_result();

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
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

	RTL_For_TAC * goto_stmt;
	if (rtl_list.empty() == false)
		goto_stmt = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return * goto_stmt;
	//exit(1);
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Label_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((label_opd != NULL), "Label cannot be null");
	RTL_Stmt * label_rtl_stmt = new Label_RTL_Stmt(rtl_label, label_opd->get_label_name()); 
	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(label_rtl_stmt);

	RTL_For_TAC * label_stmt;
	if (rtl_list.empty() == false)
		label_stmt = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return * label_stmt;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & Return_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((opd1 != NULL), "Rhs cannot be null");
	CHECK_INVARIANT((proc_name != ""), "Procedure name cannot be null");
	Procedure * proc = program_object.get_procedure_prototype(proc_name);
	CHECK_INVARIANT((proc->get_return_type() != void_data_type), "Return TAC statement in a void procedure");

	RTL_For_TAC & load_stmt = opd1->create_load_stmt(proc->get_return_register());

	Register_Descriptor * load_register = load_stmt.get_reg();
	CHECK_INVARIANT(load_register, "Load register cannot be null");
	load_register->set_use_for_expr_result();


	RTL_Opd *result_opd = new Register_Opd(load_register);

	CHECK_INVARIANT((load_register != NULL), "Load register cannot be null");
	load_register->reset_use_for_expr_result();

	RTL_Opd * reg2_opd = NULL;

	RTL_Stmt * return_rtl_stmt = new Control_Flow_RTL_Stmt(rtl_return, result_opd, reg2_opd, proc_name);

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (load_stmt.get_rtl_list().empty() == false)
		rtl_list.splice(rtl_list.end(),load_stmt.get_rtl_list());
	
	rtl_list.push_back(return_rtl_stmt);
	
	RTL_For_TAC * return_stmt;
	if (rtl_list.empty() == false)
		return_stmt = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	CHECK_INVARIANT(return_stmt!=NULL, "Return statement cannot be NULL");
	
	return * return_stmt;
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
	Procedure * proc = program_object.get_procedure_prototype(proc_name);

	list<TAC_Opd *>::reverse_iterator i;
	for(i = actual_param_list.rbegin(); i != actual_param_list.rend(); i++)
	{
		if((*i)->get_data_type() == int_data_type)
			size = size + 4;
		else if((*i)->get_data_type() == double_data_type)
			size = size + 8;
		else if((*i)->get_data_type() == string_data_type)
			size = size + 4;
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "wrong data type");
		
		// A param may be a constant too hence by default we generate a load statement.

		RTL_For_TAC & load_stmt = (*i)->create_load_stmt();

		param_register_id = load_stmt.get_reg();
		CHECK_INVARIANT(param_register_id, "Load register cannot be null");
		param_register_id->set_use_for_expr_result();
		param_opd = new Register_Opd(param_register_id);

		if (load_stmt.get_rtl_list().empty() == false)
			rtl_list.splice(rtl_list.end(),load_stmt.get_rtl_list());

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

	RTL_For_TAC * call_rtl_code;
	if (rtl_list.empty() == false)
		call_rtl_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return *call_rtl_code;
}


////////////////////////////////////////////////////////////////

RTL_For_TAC & IO_TAC_Stmt::gen_rtl()
{

	if (tac_operator == read_3a)
	{ 
		//cout << "found a read I/O TAC " << endl; // TEST
		RTL_For_TAC & rtl_for_io = gen_rtl_for_read();
		return rtl_for_io;
	}
	else if (tac_operator == write_3a)
	{
		//cout << "found a write I/O TAC " << endl; // TEST
		RTL_For_TAC & rtl_for_io = gen_rtl_for_write();
		return rtl_for_io;
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

 	Move_RTL_Stmt * mov_stmt, * load_stmt;

	if(opd1->get_data_type() == int_data_type) 
        {
	    // Generate instruction to load syscall number 1 into register v0.

	    RTL_Opd * syscall_opd = new Int_Const_Opd(1);
            mov_stmt = new Move_RTL_Stmt(rtl_imm_load, syscall_opd, load_register);
	    mov_stmt->set_comment("Loading 1 in v0 to indicate syscall to print integter value");

	    // Generate instruction to move the variable into register a0.

	    if (typeid(*(opd1)) == typeid(Variable_TAC_Opd))
	    {
      	    	print_opd = new Mem_Opd(*(opd1->get_symbol_entry()));
            	Register_Descriptor * arg_register = new Register_Descriptor (a0, "a0", int_num, argument);
	    	RTL_Opd * register_opd = new Register_Opd(arg_register);
	    	load_stmt = new Move_RTL_Stmt(rtl_load, print_opd, register_opd);
	    }
	    load_stmt->set_comment("Moving the value to be printed into register a0");
        }
        else if(opd1->get_data_type() == double_data_type) 
        {
	    // Generate instruction to load syscall number 3 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(3);
            mov_stmt = new Move_RTL_Stmt(rtl_imm_load, syscall_opd, load_register);
	    mov_stmt->set_comment("Loading 3 in v0 to indicate syscall to print double value");

	    // Generate instruction to move the variable into register f12.

      	    print_opd = new Mem_Opd(*(opd1->get_symbol_entry()));
            Register_Descriptor * arg_register = new Register_Descriptor (f12, "f12", float_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);
	    load_stmt = new Move_RTL_Stmt(rtl_load_d, print_opd, register_opd);
	    load_stmt->set_comment("Moving the value to be printed into register f12");
        }
        else if(opd1->get_data_type() == string_data_type) 
        {
	    // Generate instruction to load syscall number 4 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(3);
            mov_stmt = new Move_RTL_Stmt(rtl_imm_load, syscall_opd, load_register);
	    mov_stmt->set_comment("Loading 4 in v0 to indicate syscall to print string value");

	    // Generate instruction to move the address of the string into register a0.

	    //If we have string constant, we need to create print_opd differently.
	    //Check the typeid of opd1. If it is string constant, then we need to
	    //create a load address insruction instead of move instruction.

            Register_Descriptor * arg_register = new Register_Descriptor (a0, "a0", int_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);

	    if (typeid(*(opd1)) == typeid(Variable_TAC_Opd))
	    {
      	    	print_opd = new Mem_Opd(*(opd1->get_symbol_entry()));
	    	load_stmt = new Move_RTL_Stmt(rtl_move, print_opd, register_opd);
	    	load_stmt->set_comment("Moving the address of the string to be printed into register a0");
	    }
	    else if (typeid(*(opd1)) == typeid(Str_Const_TAC_Opd))
	    {
      	    	print_opd = new Str_Const_Opd(opd1->get_string_key());
	    	load_stmt = new Move_RTL_Stmt(rtl_load_addr, print_opd, register_opd);
	    	load_stmt->set_comment("Moving the address of the string to be printed into register a0");
	    }
	    else
	    	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " incorrect operand of write instruction");
        }
        else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " print data type cannot be null");

	RTL_Stmt *write_stmt = new Write_RTL_Stmt();
	write_stmt->set_comment("This is where syscall will be made");

        rtl_list.push_back(mov_stmt);
	rtl_list.push_back(load_stmt);
        rtl_list.push_back(write_stmt);

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

 	Move_RTL_Stmt * mov_stmt, * load_stmt;

	if(opd1->get_data_type() == int_data_type) 
        {
	    // Generate instruction to load syscall number 5 into register v0.

	    RTL_Opd * syscall_opd = new Int_Const_Opd(5);
            mov_stmt = new Move_RTL_Stmt(rtl_imm_load, syscall_opd, load_register);
	    mov_stmt->set_comment("Loading 5 in v0 to indicate syscall to read integter value");

	    // Generate instruction to move the variable from register v0.

            Register_Descriptor * arg_register = new Register_Descriptor (v0, "v0", int_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);
	    load_stmt = new Move_RTL_Stmt(rtl_store, register_opd, read_opd);
	    load_stmt->set_comment("Moving the read value from register v0");
        }
        else if(opd1->get_data_type() == double_data_type) 
        {
	    // Generate instruction to load syscall number 7 into register v0.

            RTL_Opd * syscall_opd = new Int_Const_Opd(7);
            mov_stmt = new Move_RTL_Stmt(rtl_imm_load, syscall_opd, load_register);
	    mov_stmt->set_comment("Loading 7 in v0 to indicate syscall to read double value");

	    // Generate instruction to move the variable from register f0.

            Register_Descriptor * arg_register = new Register_Descriptor (f0, "f0", float_num, argument);
	    RTL_Opd * register_opd = new Register_Opd(arg_register);
	    load_stmt = new Move_RTL_Stmt(rtl_store_d, register_opd, read_opd);
	    load_stmt->set_comment("Moving the read value from register f0");
        }
        else
	    CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, " read data type cannot be null");

	RTL_Stmt *read_stmt = new Read_RTL_Stmt();
	read_stmt->set_comment("This is where syscall will be made");

	rtl_list.push_back(mov_stmt);
        rtl_list.push_back(read_stmt);
	rtl_list.push_back(load_stmt);

	RTL_For_TAC * read_code = new RTL_For_TAC(rtl_list, NULL);
	return * read_code;
}


/////////////////////////////////////////////////////////////////

RTL_For_TAC & Variable_TAC_Opd::create_load_stmt()
{
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	/* Four steps:
	 * (a) Get a new register_id 
	 * (b) Create a new destination register operand 
	 * (c) Create a source operand for the variable 
	 * (d) Create a load instruction with the destination 
	 *     register operand and source operand
	*/

	Data_Type data_type = symbol_entry->get_data_type();

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

	RTL_Stmt * load_stmt = NULL;

	switch(data_type)
	{
	case int_data_type:
	case string_data_type:
	case bool_data_type:
		load_stmt = new Move_RTL_Stmt(rtl_load, source_opd, dest_register_opd);
		break;

	case double_data_type:
		load_stmt = new Move_RTL_Stmt(rtl_load_d, source_opd, dest_register_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}


RTL_For_TAC & Variable_TAC_Opd::create_load_stmt(Register_Descriptor *dest_register_id)
{
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.
	
	Data_Type data_type = symbol_entry->get_data_type();

	CHECK_INVARIANT((dest_register_id != NULL), "Result register cannot be null");

	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Mem_Opd(*symbol_entry);

	RTL_Stmt * load_stmt = NULL;

	switch(data_type)
	{
	case int_data_type:
	case string_data_type:
	case bool_data_type:
		load_stmt = new Move_RTL_Stmt(rtl_load, source_opd, dest_register_opd);
		break;

	case double_data_type:
		load_stmt = new Move_RTL_Stmt(rtl_load_d, source_opd, dest_register_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}

RTL_For_TAC & Variable_TAC_Opd::create_store_stmt(Register_Descriptor * source_register)
{
	CHECK_INVARIANT((source_register != NULL), "Store register cannot be null");
	CHECK_INVARIANT(symbol_entry, "Variable symbol entry cannot be null");

	RTL_Opd * source_register_opd = new Register_Opd(source_register);
	RTL_Opd * dest_opd = new Mem_Opd(*symbol_entry);

	Data_Type data_type = symbol_entry->get_data_type();

	RTL_Stmt * store_stmt = NULL;
	switch(data_type)
	{
	case int_data_type:
	case bool_data_type:
	case string_data_type:
		store_stmt = new Move_RTL_Stmt(rtl_store, source_register_opd, dest_opd);
		break;

	case double_data_type:
		store_stmt = new Move_RTL_Stmt(rtl_store_d, source_register_opd, dest_opd);
		break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Illegal option");
	}

	CHECK_INVARIANT((store_stmt != NULL), "Store statement cannot be null");

	symbol_entry->update_register(source_register);
	
	// We cannot reset the register useage as soon as a variable is loaded because then
	// the register may be used for some other operand within the same expression. Hence 
	// a register should be reset only when it's use is over which would happen in an assignment
	// statement. In other words, assignment to temporaries is not an assignment after which
	// registers can be feed.
	// source_register->reset_use_for_expr_result();

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(store_stmt);

	RTL_For_TAC & name_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return name_code;
}

///////////////////////////////////////////////////////////////////////////////

RTL_For_TAC & Int_Const_TAC_Opd::create_load_stmt()
{
	Register_Descriptor * dest_register_id;
	
	dest_register_id = machine_desc_object.get_new_register<int_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Int_Const_Opd(num);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_imm_load, source_opd, dest_register_opd);
	load_stmt->set_comment("Loading integer number " + to_string(num));

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}

RTL_For_TAC & Int_Const_TAC_Opd::create_load_stmt(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.
	
	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Int_Const_Opd(num);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_imm_load, source_opd, dest_register_opd);
	load_stmt->set_comment("Loading integer number " + to_string(num));


	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}


///////////////////////////////////////////////////////////////////////////////

RTL_For_TAC & Double_Const_TAC_Opd::create_load_stmt()
{
	Register_Descriptor * dest_register_id;
	dest_register_id = machine_desc_object.get_new_register<float_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Double_Const_Opd(num);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_imm_load_d, source_opd, dest_register_opd);
	load_stmt->set_comment("Loading float number " + to_string(num));

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}

RTL_For_TAC & Double_Const_TAC_Opd::create_load_stmt(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);
	RTL_Opd * source_opd = new Double_Const_Opd(num);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_imm_load_d, source_opd, dest_register_opd);
	load_stmt->set_comment("Loading float number " + to_string(num));

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}


///////////////////////////////////////////////////////////////////////////////

RTL_For_TAC & Str_Const_TAC_Opd::create_load_stmt()
{
	Register_Descriptor * dest_register_id;
	
	dest_register_id = machine_desc_object.get_new_register<int_reg>();

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Str_Const_Opd(str_key);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_load_addr, source_opd, dest_register_opd);
	load_stmt->set_comment("String = \"" + program_object.get_string(str_key) + "\"");

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}


RTL_For_TAC & Str_Const_TAC_Opd::create_load_stmt(Register_Descriptor * dest_register_id)
{

	// Type checking has already been done so it is assumed that the dest_register_id
	// supplied to this call is of the right type.

	CHECK_INVARIANT((dest_register_id != NULL), "Destination register cannot be null");
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Opd * source_opd = new Str_Const_Opd(str_key);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_load_addr, source_opd, dest_register_opd);
	load_stmt->set_comment("String = \"" + program_object.get_string(str_key) + "\"");

	CHECK_INVARIANT((load_stmt != NULL), "Load statement cannot be null");

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & num_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////////////


RTL_For_TAC & Temporary_TAC_Opd::create_load_stmt()
{
	// No need to load a temporary in a new register. It is already in a register.
	
	Register_Descriptor * dest_register_id = get_register_of_temporary();

	CHECK_INVARIANT((dest_register_id != NULL), "Result register cannot be null");

	RTL_Stmt * load_stmt = NULL;

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}


RTL_For_TAC & Temporary_TAC_Opd::create_load_stmt(Register_Descriptor *dest_register_id)
{
	// No need to load a temporary in a new register. It is already in a register.

	Register_Descriptor * source_register_id = get_register_of_temporary();

	CHECK_INVARIANT((source_register_id != NULL), "Result register cannot be null");

	RTL_Opd * source_opd = new Register_Opd(source_register_id);
	RTL_Opd * dest_register_opd = new Register_Opd(dest_register_id);

	RTL_Stmt * load_stmt = new Move_RTL_Stmt(rtl_move, source_opd, dest_register_opd);

	list<RTL_Stmt *> rtl_list;
	rtl_list.push_back(load_stmt);

	RTL_For_TAC & load_code = *new RTL_For_TAC(rtl_list, dest_register_id);

	return load_code;
}

RTL_For_TAC & Temporary_TAC_Opd::create_store_stmt(Register_Descriptor * source_register)
{
	CHECK_INVARIANT((source_register != NULL), "Store register cannot be null");

	RTL_Stmt * store_stmt = NULL;

	set_register_of_temporary(source_register);

	// CHECK: Reset may have to be changed if a temporary is used multiple times.

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	// There is no statement in RTL_For_TAC for a temporary.
	// Also there is no register either because register is stored
	// in the object corresponding to the temporary.
	
	rtl_list.push_back(store_stmt);

	RTL_For_TAC & name_code = *new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg());

	return name_code;
}


///////////////////////////////////////////////////////////////////////////////


RTL_For_TAC & Compute_TAC_Stmt::gen_rtl()
{
	CHECK_INVARIANT((result != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((opd1 != NULL), "The first operand of Rhs cannot be null");

	RTL_For_TAC & opd1_load_stmt = opd1->create_load_stmt();
	Register_Descriptor * reg1 = opd1_load_stmt.get_reg();
	CHECK_INVARIANT(reg1, "Load register cannot be null");
	reg1->reset_use_for_expr_result();
	RTL_Opd * reg1_opd = new Register_Opd(reg1);

	// Get result register depending upon the type of reg1.
	
	Register_Val_Type reg_type = reg1->get_register_type();
	RTL_Op rtl_op = get_rtl_operator_for_tac_operator(); 

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

	RTL_Stmt * compute_stmt;

	list<RTL_Stmt *> & rtl_list = *new list<RTL_Stmt *>;

	if (opd1_load_stmt.get_rtl_list().empty() == false)
		rtl_list = opd1_load_stmt.get_rtl_list();


	if (is_binary_op(tac_operator))
	{
		CHECK_INVARIANT((opd2 != NULL), "The second operand of Rhs cannot be null");
		RTL_For_TAC & opd2_load_stmt = opd2->create_load_stmt();
		Register_Descriptor * reg2 = opd2_load_stmt.get_reg();
		CHECK_INVARIANT(reg2, "Load register cannot be null");
		reg2->reset_use_for_expr_result();
		RTL_Opd * reg2_opd = new Register_Opd(reg2);

		if (opd2_load_stmt.get_rtl_list().empty() == false)
			rtl_list.splice(rtl_list.end(), opd2_load_stmt.get_rtl_list());
	        	
		// Generate an RTL statement with opd1_register and opd2_register
	        compute_stmt = new Compute_RTL_Stmt(rtl_op, reg1_opd, reg2_opd, dest_reg_opd); 

	}
	else
	        compute_stmt = new Compute_RTL_Stmt(rtl_op, reg1_opd, NULL, dest_reg_opd); 

	rtl_list.push_back(compute_stmt);

	RTL_For_TAC * compute_rtl_code;
	if (rtl_list.empty() == false)
		compute_rtl_code = new RTL_For_TAC(rtl_list, machine_desc_object.get_no_reg()); 

	return *compute_rtl_code;
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

