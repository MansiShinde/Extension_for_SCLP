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

TAC_For_Ast * Assignment_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// Get the code = (list,result) for RHS 
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of assignment cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();

	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	// Get the code = (list,result) for LHS 
	TAC_For_Ast  * lhs_code;
	if (lhs->is_a_scalar())
		lhs_code = lhs->gen_tac();
	else
		lhs_code = lhs->gen_tac_aggregate();
	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Create new assignment statement 
	TAC_Opd * lval = lhs_res;
	TAC_Opd * rval = rhs_res;
	TAC_Stmt * asgn_stmt = new Asgn_TAC_Stmt(lval, rval); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list = rhs_code->get_tac_list();

        // Append LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *lhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(asgn_stmt);

	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}


/////////////////////////////////////////////////////////////////
TAC_For_Ast * Selection_Statement_Ast::gen_tac()
{
	CHECK_INVARIANT((cond != NULL), "If condition cannot be null");
	CHECK_INVARIANT((then_part != NULL), "Then part of if_else cannot be null");

	TAC_For_Ast * cond_code = cond->gen_tac();
	TAC_For_Ast * then_code = then_part->gen_tac();
	TAC_For_Ast * else_code = NULL;

	TAC_Opd * lhs_res = cond_code->get_result();
	TAC_Opd * not_rhs_res = NULL;
	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	TAC_Stmt * not_cond_stmt = new Compute_TAC_Stmt(not_3a, lhs_res, not_rhs_res, res_temp, get_basic_data_type());	// 1 stmt 

	TAC_Opd * not_result = res_temp;
	TAC_Stmt * if_else_stmt;
	TAC_Opd * rhs_res, * if_goto;
	if_goto = new Label_TAC_Opd();
	TAC_Stmt * label1, * label2;

	if (else_part != NULL)
	{
		rhs_res = new Label_TAC_Opd();
		if_else_stmt = new If_Goto_TAC_Stmt(not_result, rhs_res);	// 2 stmt
	}
	else
	{
		
		if_else_stmt = new If_Goto_TAC_Stmt(not_result, if_goto);	// 2 stmt
	}
	
	TAC_Stmt * if_goto_stmt = new Goto_TAC_Stmt(if_goto);	// 3 stmt

	if (else_part != NULL)
	{
		label1 = new Label_TAC_Stmt(rhs_res);	// 4 stmt
		else_code = else_part->gen_tac();
		
	}

	label2 = new Label_TAC_Stmt(if_goto);	// 5 stmt

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

	// Copy Cond list to the new list 
	if (cond_code->get_tac_list() != NULL)
		tac_list = cond_code->get_tac_list();

	// Create new code 
	tac_list->push_back(not_cond_stmt);
	tac_list->push_back(if_else_stmt);

	// Append then list to the new list 
	if (then_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *then_code->get_tac_list());

	tac_list->push_back(if_goto_stmt);

	if (else_part != NULL)
	{
		tac_list->push_back(label1);
		if (else_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *else_code->get_tac_list());
	}

	tac_list->push_back(label2);

	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;

}


/////////////////////////////////////////////////////////////////
TAC_For_Ast * Iteration_Statement_Ast::gen_tac()
{
	CHECK_INVARIANT((cond != NULL), "Loop condition cannot be null");
	CHECK_INVARIANT((body != NULL), "Body of loop cannot be null");

	TAC_For_Ast * cond_code = cond->gen_tac();
	TAC_For_Ast * body_code = body->gen_tac();

	TAC_Opd * lhs_res = cond_code->get_result();
	TAC_Opd * rhs_res = body_code->get_result();

	TAC_Opd * label_opd1, * label_opd2;
	TAC_Opd * not_rhs_res = NULL, * res_temp, * not_result;
	label_opd1 = new Label_TAC_Opd();
	TAC_Stmt * label1, * label2;
	label1 = new Label_TAC_Stmt(label_opd1);

	TAC_Stmt * if_do_while, * not_cond_stmt, * if_while, * while_goto;

	if(is_do_form)
	{
		if_do_while = new If_Goto_TAC_Stmt(lhs_res, label_opd1);
	}
	else
	{
		res_temp = new Temporary_TAC_Opd(get_basic_data_type());
		not_cond_stmt = new Compute_TAC_Stmt(not_3a, lhs_res, not_rhs_res, res_temp, get_basic_data_type());
		not_result = res_temp;
		label_opd2 = new Label_TAC_Opd();
		if_while = new If_Goto_TAC_Stmt(not_result, label_opd2);
		while_goto = new Goto_TAC_Stmt(label_opd1);
		label2 = new Label_TAC_Stmt(label_opd2);
	}

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

	tac_list->push_back(label1);
	if(is_do_form)
	{
		if (body_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *body_code->get_tac_list());
		if (cond_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *cond_code->get_tac_list());
		tac_list->push_back(if_do_while);
	}
	else
	{
		if (cond_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *cond_code->get_tac_list());
		tac_list->push_back(not_cond_stmt);
		tac_list->push_back(if_while);
		if (body_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *body_code->get_tac_list());
		tac_list->push_back(while_goto);
		tac_list->push_back(label2);
	}
	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;

}


/////////////////////////////////////////////////////////////////
TAC_For_Ast * Conditional_Expression_Ast::gen_tac()
{
	CHECK_INVARIANT((cond != NULL), "Ternary condition cannot be null");
	CHECK_INVARIANT((lhs != NULL), "True part of ternary cannot be null");
	CHECK_INVARIANT((rhs != NULL), "False part of ternary cannot be null");

	TAC_For_Ast * cond_code = cond->gen_tac();
	TAC_For_Ast * lhs_code = lhs->gen_tac();
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	TAC_Opd * cond_res = cond_code->get_result();
	TAC_Opd * lhs_res = lhs_code->get_result();
	TAC_Opd * rhs_res = rhs_code->get_result();

	TAC_Opd * not_rhs_res = NULL;
	TAC_Opd * cond_res_temp = new Temporary_TAC_Opd(get_basic_data_type());
	TAC_Stmt * not_cond_stmt = new Compute_TAC_Stmt(not_3a, cond_res, not_rhs_res, cond_res_temp, get_basic_data_type());
	TAC_Opd * not_result = cond_res_temp;
	TAC_Opd * label_opd1 = new Label_TAC_Opd();

	TAC_Stmt * if_cond_stmt = new If_Goto_TAC_Stmt(not_result, label_opd1);

	//TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());  // FIX: This should become Variable_TAC_Opd with a new name.
	TAC_Opd * res_temp = new Variable_TAC_Opd(get_result_sym_entry());
	TAC_Stmt * true_stmt = new Asgn_TAC_Stmt(res_temp, lhs_res);

	TAC_Opd * label_opd2 = new Label_TAC_Opd();
	TAC_Stmt * endif_goto_stmt = new Goto_TAC_Stmt(label_opd2);

	TAC_Stmt * label1 = new Label_TAC_Stmt(label_opd1);
	TAC_Stmt * false_stmt = new Asgn_TAC_Stmt(res_temp, rhs_res);

	TAC_Stmt * label2 = new Label_TAC_Stmt(label_opd2);

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

	if (cond_code->get_tac_list() != NULL)
		tac_list = cond_code->get_tac_list();
	tac_list->push_back(not_cond_stmt);
	tac_list->push_back(if_cond_stmt);
	if (lhs_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *lhs_code->get_tac_list());
	tac_list->push_back(true_stmt);
	tac_list->push_back(endif_goto_stmt);
	tac_list->push_back(label1);
	if (rhs_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());
	tac_list->push_back(false_stmt);
	tac_list->push_back(label2);

	TAC_Opd * list_result = res_temp;
	CHECK_INVARIANT(list_result!=NULL, "Result of ternary TAC cannot be NULL");
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;

}


/////////////////////////////////////////////////////////////////
TAC_For_Ast * Sequence_Ast::gen_tac()
{
	TAC_For_Ast * t_code;

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

	list<Ast *>::iterator t;
	for (t = seq_ast_statement_list.begin(); t != seq_ast_statement_list.end(); ++t)
	{
		t_code = (*t)->gen_tac();
		if (t_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *t_code->get_tac_list());
	}
	
	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}


/////////////////////////////////////////////////////////////////

TAC_For_Ast * Name_Ast::gen_tac()
{
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null");

	TAC_Opd * list_result = new Variable_TAC_Opd(variable_symbol_entry);

	CHECK_INVARIANT(list_result!=NULL, "Variable operand of TAC cannot be NULL");
	list <TAC_Stmt *> * tac_list = NULL;    // No TAC statement for Name_Ast 
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	CHECK_INVARIANT(ast_code!=NULL, "Code generated for variable operand of TAC cannot be NULL");
	return ast_code;
}

TAC_For_Ast * Name_Ast::gen_tac_aggregate()
{
	return gen_tac();
}
///////////////////////////////////////////////////////////////////////////////////////////

template class Number_Ast<double>;
template class Number_Ast<int>;

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
TAC_For_Ast * Number_Ast<DATA_TYPE>::gen_tac()
{
	TAC_Opd * list_result=NULL;

	switch (get_basic_data_type())
	{
		case int_data_type:
			list_result = new Int_Const_TAC_Opd(constant);
			break;
		case double_data_type:
			list_result = new Double_Const_TAC_Opd(constant);
			break;
		default:
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Wrong type in Number_Ast");
			break;
	}

	CHECK_INVARIANT(list_result!=NULL, "Number operand of TAC cannot be NULL");
	list <TAC_Stmt *> * tac_list = NULL;    // No TAC statement for Number_Ast 
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);
	CHECK_INVARIANT(ast_code!=NULL, "Code generated for number operand of TAC cannot be NULL");

	return ast_code;
}

///////////////////////////////////////////////////////////////////////////////

TAC_For_Ast * String_Constant_Ast::gen_tac()
{
	TAC_Opd * list_result = new Str_Const_TAC_Opd(str_key);
	list <TAC_Stmt *> * tac_list = NULL;    // No TAC statement for string 
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);
	CHECK_INVARIANT(ast_code!=NULL, "Code generated for string operand of TAC cannot be NULL");
	return ast_code;
}

//////////////////////////////////////////////////////////////////////////////
TAC_For_Ast * Write_Ast::gen_tac()
{
	TAC_For_Ast * print_opd_code = var->gen_tac();
	TAC_Opd * var_to_print = print_opd_code->get_result();
	
	TAC_Stmt * stmt = new IO_TAC_Stmt(write_3a, var_to_print);
	list <TAC_Stmt *> * tac_list = new list<TAC_Stmt *>; 
	tac_list->push_back(stmt);

	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

//////////////////////////////////////////////////////////////////////////////
TAC_For_Ast * Read_Ast::gen_tac()
{
	TAC_For_Ast * print_opd_code = var->gen_tac();
	TAC_Opd * var_to_print = print_opd_code->get_result();
	
	TAC_Stmt * stmt = new IO_TAC_Stmt(read_3a, var_to_print);
	list <TAC_Stmt *> * tac_list = new list<TAC_Stmt *>; 
	tac_list->push_back(stmt);

	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

//////////////////////////////////////////////////////////////////////////////

TAC_For_Ast * Plus_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of Plus AST cannot be NULL");
	CHECK_INVARIANT(lhs_code!=NULL, "Code generated for LHS of Plus AST cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();
	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new plus statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = rhs_res;
	TAC_Stmt * plus_stmt = new Compute_TAC_Stmt(add_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

        // Append RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(plus_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Minus_Ast //////////////////////////////////

TAC_For_Ast * Minus_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of Minus AST cannot be NULL");
	CHECK_INVARIANT(lhs_code!=NULL, "Code generated for LHS of Minus AST cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();
	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new minus statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = rhs_res;
	TAC_Stmt * minus_stmt = new Compute_TAC_Stmt(sub_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

        // Append RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(minus_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Mult_Ast //////////////////////////////////

TAC_For_Ast * Mult_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of Must AST cannot be NULL");
	CHECK_INVARIANT(lhs_code!=NULL, "Code generated for LHS of Mult AST cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();

	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new mult statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = rhs_res;
	TAC_Stmt * mult_stmt = new Compute_TAC_Stmt(mult_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

        // Append RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(mult_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Divide_Ast //////////////////////////////////

TAC_For_Ast * Divide_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_For_Ast * rhs_code = rhs->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of Divide AST cannot be NULL");
	CHECK_INVARIANT(lhs_code!=NULL, "Code generated for LHS of Divide AST cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();

	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new divide statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = rhs_res;
	TAC_Stmt * divide_stmt = new Compute_TAC_Stmt(div_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

        // Append RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(divide_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// UMinus_Ast //////////////////////////////////

TAC_For_Ast * UMinus_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	// UMinus has only one operand
	
	// Get the code = (list,result) for LHS 
	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new uminus statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = NULL;
	TAC_Stmt * uminus_stmt = new Compute_TAC_Stmt(uminus_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

	// Create new code 
	tac_list->push_back(uminus_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Relational_Expr_Ast //////////////////////////////////

TAC_For_Ast * Relational_Expr_Ast::gen_tac()
{
	//cout << "Constructing TAC for relational expression ast " << endl;	// TEST

	CHECK_INVARIANT((lhs_condition != NULL), "Lhs_condition cannot be null");
	CHECK_INVARIANT((rhs_condition != NULL), "Rhs_condition cannot be null");

	TAC_For_Ast  * lhs_code = lhs_condition->gen_tac();
	TAC_For_Ast * rhs_code = rhs_condition->gen_tac();

	CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of Relational Expr AST cannot be NULL");
	CHECK_INVARIANT(lhs_code!=NULL, "Code generated for LHS of Relational Expr AST cannot be NULL");
	TAC_Opd * rhs_res = rhs_code->get_result();

	CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");

	// Get the code = (list,result) for LHS 
	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(bool_data_type);

	// Create new relational statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = rhs_res;
	TAC_Stmt * relational_stmt = new Compute_TAC_Stmt(get_tac_operator_for_rel_operator(), lopd, ropd, res_temp, lhs_condition->get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

        // Append RHS list to the new list 
	if (rhs_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());

	// Create new code 
	tac_list->push_back(relational_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Logical_Expr_Ast //////////////////////////////////

TAC_For_Ast * Logical_Expr_Ast::gen_tac()
{
	CHECK_INVARIANT((lhs_op != NULL), "Lhs_op cannot be null");

	// Get the code = (list,result) for LHS 
	TAC_For_Ast  * lhs_code = lhs_op->gen_tac();
	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");
	
	TAC_For_Ast * rhs_code = NULL;
	TAC_Opd * lopd, * ropd = NULL, * rhs_res = NULL;

	lopd = lhs_res;
	//if(rhs_op == NULL)
	//	cout << "RHS is null" ;
	
	if(bool_op != not_op)
	{
		CHECK_INVARIANT((rhs_op != NULL), "Rhs_op cannot be null");

		// Get the code = (list,result) for RHS 
		rhs_code = rhs_op->gen_tac();
		CHECK_INVARIANT(rhs_code!=NULL, "Code generated for RHS of logical expression cannot be NULL");

		rhs_res = rhs_code->get_result();
		CHECK_INVARIANT(rhs_res, "Result of TAC list cannot be null");
		ropd = rhs_res;
	}
	
	// Get a new temporary 
	TAC_Opd * res_temp = new Temporary_TAC_Opd(bool_data_type);

	TAC_Stmt * relational_stmt = new Compute_TAC_Stmt(get_tac_operator_for_logical_operator(), lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();
       	
	if(bool_op != not_op)
	{
		// Append RHS list to the new list 
		if (rhs_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *rhs_code->get_tac_list());
	}

	// Create new code 
	tac_list->push_back(relational_stmt);

	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Return_Ast //////////////////////////////////

TAC_For_Ast * Return_Ast::gen_tac()
{
	CHECK_INVARIANT((proc_name != ""), "Procedure name cannot be null");
	TAC_For_Ast * ret_val_code = NULL;
	TAC_Opd * ret_val_res = NULL;
	TAC_Stmt * ret_stmt = NULL;
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");
	TAC_Opd * return_temp, * label_opd1;
	TAC_Stmt * ret_asign_stmt, * ret_goto_stmt, * ret_label;
	Procedure * proc = program_object.get_procedure_entry(proc_name);

	if (return_value != NULL)	
	{
		return_temp = proc->get_return_temp();
		ret_val_code = return_value->gen_tac();
		ret_val_res = ret_val_code->get_result();
		ret_asign_stmt = new Asgn_TAC_Stmt(return_temp, ret_val_res);
		if (ret_val_code->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *ret_val_code->get_tac_list());
		tac_list->push_back(ret_asign_stmt);
		ret_goto_stmt = new Goto_TAC_Stmt(proc->get_return_label());
		tac_list->push_back(ret_goto_stmt);
	}

	TAC_Opd * list_result = NULL;
	TAC_For_Ast * ret_stmt_code = new TAC_For_Ast(tac_list, list_result);

	return ret_stmt_code;
}

///////////////////////////////// Call_Ast //////////////////////////////////

TAC_For_Ast * Call_Ast::gen_tac()
{
	CHECK_INVARIANT((procedure_name != ""), "Procedure name cannot be null");

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

	TAC_Opd * result = new Temporary_TAC_Opd(get_basic_data_type());
	TAC_Stmt * call_stmt = new Call_TAC_Stmt(procedure_name, result);
	
	list<Ast *>::iterator i;
	for(i = actual_param_list.begin(); i != actual_param_list.end(); i++)
	{
		TAC_For_Ast  * param_ast = (*i)->gen_tac();
		TAC_Opd * param_res = param_ast->get_result();
		call_stmt->set_actual_param_list(param_res);
		if (param_ast->get_tac_list() != NULL)
			tac_list->splice(tac_list->end(), *param_ast->get_tac_list());
	}
	
	tac_list->push_back(call_stmt);	

	TAC_Opd * list_result = result;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;

}

//////////////////////////////////////////////////////////////////////////////

	// Overall strategy
	// Add two functions for Array_Access_Ast: 
	// - gen_tac which generates the result in a temporary
	// - gen_tac_aggregate which generates the result as access of array into a single dimension
	//        (check: if we want the word size multiplication at the end or in the AST, i.e.
	//                if we want byte-sized array or not.
	// gen_tac is used for generating TAC for RHS whereas gen_tac_aggregate is used for generating
	// TAC for aggregate LHS where we want the result to go in an aggregate. Since the RHS of
	// Asgn_TAC_Stmt is ALWAYS a temporary, the LHS can be a aggregate.

TAC_For_Ast * Array_Access_Ast::gen_tac_aggregate()
{
	CHECK_INVARIANT((base != NULL), "Base cannot be null");
	CHECK_INVARIANT((offset != NULL), "Offset cannot be null");

	TAC_For_Ast  * base_code = base->gen_tac();
	TAC_For_Ast * offset_code = offset->gen_tac();

	CHECK_INVARIANT(base_code!=NULL, "Code generated for base of Array AST cannot be NULL");
	CHECK_INVARIANT(offset_code!=NULL, "Code generated for offset of Array AST cannot be NULL");
	TAC_Opd * offset_res = offset_code->get_result();
	CHECK_INVARIANT(offset_res, "Result of TAC list cannot be null");

	TAC_Opd * base_res = base_code->get_result();
	CHECK_INVARIANT(base_res, "Result of TAC list cannot be null");

	TAC_Opd * array_access_opd = new Array_Access_TAC_Opd(base_res, offset_res);

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (base_code->get_tac_list() != NULL)
		tac_list = base_code->get_tac_list();

        // Append RHS list to the new list 
	if (offset_code->get_tac_list() != NULL)
		tac_list->splice(tac_list->end(), *offset_code->get_tac_list());

	TAC_Opd * list_result = array_access_opd;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

TAC_For_Ast * Array_Access_Ast::gen_tac()
{


	TAC_For_Ast  * array_access_code = gen_tac_aggregate();

	CHECK_INVARIANT(array_access_code!=NULL, "Code generated for of Array AST cannot be NULL");


	TAC_Opd * array_opd = array_access_code->get_result();
	CHECK_INVARIANT(array_opd, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	TAC_Stmt * copy_stmt = new Compute_TAC_Stmt(copy_3a, array_opd, NULL, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (array_access_code->get_tac_list() != NULL)
		tac_list = array_access_code->get_tac_list();

	// Create new code 
	tac_list->push_back(copy_stmt);

	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

	// Overall strategy
	// Add two functions for Pointer_Deref_Ast: 
	// - gen_tac which generates the result in a temporary
	// - gen_tac_aggregate which generates the result as a pointer deref
	// gen_tac is used for generating TAC for RHS whereas gen_tac_aggregate is used for generating
	// TAC for aggregate LHS where we want the result to go in an aggregate. The RHS of
	// Asgn_TAC_Stmt is ALWAYS a temporary, the LHS can be a aggregate.
	// Unlike in the case of array, we cannot use gen_aggregate internally and generate a
	// copy statement within gen-tac because then we get a copy statement of the form
	// temp_5 = * *x where *x is generated by gen_aggregate. Hence in this case, we restrict
	// the use of gen_aggregate to generating code for LHS only.

TAC_For_Ast * Pointer_Deref_Ast::gen_tac_aggregate()
{
	Ast * base = opd;
	CHECK_INVARIANT((base != NULL), "Base cannot be null");

	TAC_For_Ast  * base_code = base->gen_tac();
	CHECK_INVARIANT(base_code!=NULL, "Code generated for base of Pointer Deref AST cannot be NULL");
	TAC_Opd * base_res = base_code->get_result();
	CHECK_INVARIANT(base_res, "Result of TAC list cannot be null");

	TAC_Opd * pointer_deref_access_opd = new Pointer_Deref_TAC_Opd(base_res);

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (base_code->get_tac_list() != NULL)
		tac_list = base_code->get_tac_list();

	TAC_Opd * list_result = pointer_deref_access_opd;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

TAC_For_Ast * Pointer_Deref_Ast::gen_tac()
{
	Ast * base = opd;
#if 0
	TAC_For_Ast  * pointer_deref_access_code = gen_tac_aggregate();

	CHECK_INVARIANT(pointer_deref_access_code!=NULL, "Code generated for of Array AST cannot be NULL");


	TAC_Opd * pointer_deref_opd = pointer_deref_access_code->get_result();
	CHECK_INVARIANT(pointer_deref_opd, "Result of TAC list cannot be null");
#endif
	TAC_For_Ast  * base_code = base->gen_tac();
	CHECK_INVARIANT(base_code!=NULL, "Code generated for base of Pointer Deref AST cannot be NULL");
	TAC_Opd * base_res = base_code->get_result();
	CHECK_INVARIANT(base_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	TAC_Stmt * copy_stmt = new Compute_TAC_Stmt(deref_3a, base_res, NULL, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (base_code->get_tac_list() != NULL)
		tac_list = base_code->get_tac_list();

	// Create new code 
	tac_list->push_back(copy_stmt);

	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}

///////////////////////////////// Address_Of_Ast //////////////////////////////////

TAC_For_Ast * Address_Of_Ast::gen_tac()
{
	Ast * lhs = base;
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");

	// Get the code = (list,result) for LHS 
	TAC_For_Ast  * lhs_code = lhs->gen_tac();
	TAC_Opd * lhs_res = lhs_code->get_result();
	CHECK_INVARIANT(lhs_res, "Result of TAC list cannot be null");

	// Get a new temporary 

	TAC_Opd * res_temp = new Temporary_TAC_Opd(get_basic_data_type());

	// Create new uminus statement 
	TAC_Opd * lopd = lhs_res;
	TAC_Opd * ropd = NULL;
	TAC_Stmt * addr_stmt = new Compute_TAC_Stmt(addr_3a, lopd, ropd, res_temp, get_basic_data_type()); 

	// Make new list 
	list<TAC_Stmt *> * tac_list = new list<TAC_Stmt *>;
	CHECK_INVARIANT(tac_list, "TAC list cannot be null");

        // Copy LHS list to the new list 
	if (lhs_code->get_tac_list() != NULL)
		tac_list = lhs_code->get_tac_list();

	// Create new code 
	tac_list->push_back(addr_stmt);


	TAC_Opd * list_result = res_temp;
	TAC_For_Ast * ast_code = new TAC_For_Ast(tac_list, list_result);

	return ast_code;
}
