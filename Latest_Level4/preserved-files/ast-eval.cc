
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Originally implemented in 2012 by Tanu Kanvar and Uday Khedker 
           (http://www.cse.iitb.ac.in/~uday) for the courses cs302+cs316
           Language Processors (theory and lab) at IIT Bombay.

           Initial release date Jan 15, 2013.

           Updated by N. Venkatesh in Jan 2019.

           Updated by Uday Khedker with the help of Nisha Biju and Saari
           Rajan in Jan 2020.

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

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	stringstream msg;
	msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	stringstream msg;
	msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print(file_buffer);

	lhs->print_value(eval_env, file_buffer);

	return result;
}

/////////////////////////////////////////////////////////////////

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		if (loc_var_val->get_result_enum() == double_result)
			file_buffer << loc_var_val->get_double_value() << "\n";
		else if (loc_var_val->get_result_enum() == int_result)
			file_buffer << loc_var_val->get_int_value() << "\n";
		else
			CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Result type can only be int / float");
	}

	else
	{
		CHECK_INVARIANT(((glob_var_val->get_result_enum() == int_result) ||
			(glob_var_val->get_result_enum() == double_result)),
			"Result type can only be int and float");

		if (glob_var_val == NULL)
			file_buffer << "0\n";
		else if (glob_var_val->get_result_enum() == double_result)
			file_buffer << glob_var_val->get_double_value() << "\n";
		else
			file_buffer << glob_var_val->get_int_value() << "\n";
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	CHECK_INVARIANT(variable_symbol_entry, "Variable symbol entry cannot be null");

	string variable_name = variable_symbol_entry->get_variable_name();

	if (eval_env.does_variable_exist(variable_name))
	{
		CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true), 
					"Variable should be defined before its use", lineno);

		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true), 
				"Variable should be defined before its use", lineno);

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	CHECK_INVARIANT(result, "Result cannot be null");

	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result * i;

	CHECK_INVARIANT((variable_symbol_entry != NULL), "Variable symbol entry cannot be null");

	string variable_name = variable_symbol_entry->get_variable_name();
	if (get_data_type() == int_data_type)
		i = new Eval_Result_Value_Int();
	else if (get_data_type() == double_data_type)
		i = new Eval_Result_Value_Double();
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value(result.get_int_value());
	else if (result.get_result_enum() == double_result)
	 	i->set_value(result.get_double_value());
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

        //cout<<variable_name;
	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value(constant);

		return result;
	}
	else if (node_data_type == double_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Double();
		result.set_value(constant);

		return result;
	}
}


///////////////////////////////////////////////////////////////////////////////

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;

	if (node_data_type == int_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Int();
		condition_eval_result->set_result_enum(int_result);
	}
	else if (node_data_type == double_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Double();
		condition_eval_result->set_result_enum(double_result);
	}
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Plus_Ast can have only float / int data type", lineno);

	Eval_Result & lhs_eval = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_eval = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of arithmetic expression", lineno);

	CHECK_INPUT_AND_ABORT(rhs_eval.is_variable_defined(), 
		"Variable should be defined to be on rhs of arithmetic expression", lineno);

	if (lhs_eval.get_result_enum() == double_result) 
		condition_eval_result->set_value(lhs_eval.get_double_value() + rhs_eval.get_double_value());
	else
		condition_eval_result->set_value(lhs_eval.get_int_value() + rhs_eval.get_int_value());
	
	return *condition_eval_result;
}

///////////////////////////////////////////////////////////////////////////////

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;

	if (node_data_type == int_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Int();
		condition_eval_result->set_result_enum(int_result);
	}
	else if (node_data_type == double_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Double();
		condition_eval_result->set_result_enum(double_result);
	}
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Minus_Ast can have only float / int data type", lineno);

	Eval_Result & lhs_eval = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_eval = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of arithmetic expression", lineno);

	CHECK_INPUT_AND_ABORT(rhs_eval.is_variable_defined(), 
		"Variable should be defined to be on rhs of arithmetic expression", lineno);

	if (lhs_eval.get_result_enum() == double_result) 
		condition_eval_result->set_value(lhs_eval.get_double_value() - rhs_eval.get_double_value());
	else
		condition_eval_result->set_value(lhs_eval.get_int_value() - rhs_eval.get_int_value());
	
	return *condition_eval_result;
}

//////////////////////////////////////////////////////////////////

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;

	if (node_data_type == int_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Int();
		condition_eval_result->set_result_enum(int_result);
	}
	else if (node_data_type == double_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Double();
		condition_eval_result->set_result_enum(double_result);
	}
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Mult_Ast can have only float / int data type", lineno);

	Eval_Result & lhs_eval = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_eval = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of arithmetic expression", lineno);

	CHECK_INPUT_AND_ABORT(rhs_eval.is_variable_defined(), 
		"Variable should be defined to be on rhs of arithmetic expression", lineno);

	if (lhs_eval.get_result_enum() == double_result) 
		condition_eval_result->set_value(lhs_eval.get_double_value() * rhs_eval.get_double_value());
	else
		condition_eval_result->set_value(lhs_eval.get_int_value() * rhs_eval.get_int_value());

	return *condition_eval_result;
}

////////////////////////////////////////////////////////////////////

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;

	if (node_data_type == int_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Int();
		condition_eval_result->set_result_enum(int_result);
	}
	else if (node_data_type == double_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Double();
		condition_eval_result->set_result_enum(double_result);
	}
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Divide_Ast can have only float / int data type", lineno);

	Eval_Result & lhs_eval = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_eval = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of arithmetic expression", lineno);

	CHECK_INPUT_AND_ABORT(rhs_eval.is_variable_defined(), 
		"Variable should be defined to be on rhs of arithmetic expression", lineno);

	if (lhs_eval.get_result_enum() == double_result) 
	{
		CHECK_INPUT_AND_ABORT((rhs_eval.get_double_value() != 0.0), "Divide by 0", lineno);
		condition_eval_result->set_value(lhs_eval.get_double_value() / rhs_eval.get_double_value());
	}
	else
	{
		CHECK_INPUT_AND_ABORT((rhs_eval.get_int_value() != 0), "Divide by 0", lineno);
		condition_eval_result->set_value(lhs_eval.get_int_value() / rhs_eval.get_int_value());
	}


	return *condition_eval_result;
}

//////////////////////////////////////////////////////////////////////

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;

	if (node_data_type == int_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Int();
		condition_eval_result->set_result_enum(int_result);
	}
	else if (node_data_type == double_data_type)
	{
		condition_eval_result = new Eval_Result_Value_Double();
		condition_eval_result->set_result_enum(double_result);
	}
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Uminus Ast can have only float / int data type", lineno);

	Eval_Result & lhs_eval = lhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of arithmetic expression", lineno);

	if (lhs_eval.get_result_enum() == double_result) 
		condition_eval_result->set_value(-lhs_eval.get_double_value());
	else
		condition_eval_result->set_value(-lhs_eval.get_int_value());

	return *condition_eval_result;
}

/////////////////////////////////////////////////////////////////////////////////

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * condition_eval_result;


	Eval_Result & lhs_eval = lhs_condition->evaluate(eval_env, file_buffer);
	Eval_Result & rhs_eval = rhs_condition->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhs_eval.is_variable_defined(), 
		"Variable should be defined to be on lhs of Relational expression", lineno);

	CHECK_INPUT_AND_ABORT(rhs_eval.is_variable_defined(), 
		"Variable should be defined to be on rhs of Relational expression", lineno);

   condition_eval_result = new Eval_Result_Value_Int();
   condition_eval_result->set_result_enum(int_result);

   switch(rel_op)
	{
	case less_equalto: if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value() <= rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value() <= rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }



				break;
	case less_than: if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value() < rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value() < rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   } 
				break;
	case greater_equalto: if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value() >= rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value() >= rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
				break;
	case greater_than: if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value()> rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value()> rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
				break;
	case equalto: if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value() == rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value() == rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
				break;
	case not_equalto:  if(lhs_eval.get_result_enum() == int_result) {
                           if(lhs_eval.get_int_value() != rhs_eval.get_int_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
                      else if(lhs_eval.get_result_enum() == double_result) {
                           if(lhs_eval.get_double_value() != rhs_eval.get_double_value())
                                     condition_eval_result->set_value(1);
                           else
                                     condition_eval_result->set_value(0);  
	                   }
				break;
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Cannot recognize relational operator");
	}

	
	return *condition_eval_result;
}
/////////////////////////////////////////////////////////////////////////////////
Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) 
{

	Eval_Result * condition_eval_result;



   condition_eval_result = new Eval_Result_Value_Int();
   condition_eval_result->set_result_enum(int_result);

switch(bool_op)
	{
		case _logical_and : {
                                Eval_Result & lhs_eval = lhs_op->evaluate(eval_env, file_buffer);
	                            Eval_Result & rhs_eval = rhs_op->evaluate(eval_env, file_buffer);

		                  if(lhs_eval.get_int_value() == 1 && rhs_eval.get_int_value() == 1)
                                     condition_eval_result->set_value(1);
                            else 
                                       condition_eval_result->set_value(0);
                             }
		                      break;

		case _logical_or :  {
                             Eval_Result & lhs_eval = lhs_op->evaluate(eval_env, file_buffer);
	                            Eval_Result & rhs_eval = rhs_op->evaluate(eval_env, file_buffer);

                             if(lhs_eval.get_int_value() == 1  || rhs_eval.get_int_value() == 1)
                                     condition_eval_result->set_value(1);
                            else 
                                       condition_eval_result->set_value(0);
                              }
		                      break;
		case _logical_not :  {
		                  Eval_Result & rhs_eval = rhs_op->evaluate(eval_env, file_buffer);


                            if(rhs_eval.get_int_value()==0)
                            	condition_eval_result->set_value(1);
                            else
                            	condition_eval_result->set_value(0);
                        }
		               break;
	}

	return *condition_eval_result;

}

///////////////////////////////////////////////////////////////////////////////
Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {

   

	Eval_Result & cond_eval = cond->evaluate(eval_env, file_buffer);
	
    if(cond_eval.get_int_value()==1) {
    Eval_Result & then_result= then_part->evaluate(eval_env, file_buffer);
        return then_result;
    }
    else {
    	      if(else_part!=NULL) {
    	Eval_Result & else_result = else_part->evaluate(eval_env, file_buffer);
    	return else_result;
         }
    }

}
////////////////////////////////////////////////////////////////////////////////
Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
      

       if(is_do_form)  do {  body->evaluate(eval_env,file_buffer); } while(cond->evaluate(eval_env, file_buffer).get_int_value());
       else
         while(cond->evaluate(eval_env, file_buffer).get_int_value()) {
             body->evaluate(eval_env,file_buffer);          
         }

         //Eval_Result & result=body->evaluate(eval_env,file_buffer);
     return cond->evaluate(eval_env, file_buffer);
}
///////////////////////////////////////////////////////////////////////////////////
Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {

	Eval_Result * result = NULL;

        list <Ast *>::iterator i;
	for (i = seq_ast_statement_list.begin(); i != seq_ast_statement_list.end(); i++)
	{
		CHECK_INVARIANT(((*i) != NULL), "Ast pointer seems to be NULL");
		result = &((*i)->evaluate(eval_env, file_buffer)); 
	}

      return *result;

}
////////////////////////////////////////////////////////////////////////////////////
Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {



     Eval_Result & cond_eval = cond->evaluate(eval_env, file_buffer);
	
    if(cond_eval.get_int_value()==1) {
    Eval_Result & then_result= lhs->evaluate(eval_env, file_buffer);
        return then_result;
    }
    else {
    	Eval_Result & else_result = rhs->evaluate(eval_env, file_buffer);
    	return else_result;
    }

}
////////////////////////////////////////////////////////////////////////////////////
Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Procedure * proc = program_object.get_procedure_prototype(procedure_name);
	CHECK_INVARIANT((proc != NULL), "Procedure being called cannot be null");
	Symbol_Table formal_param_list = proc->get_formal_param_list();

	// check formal and actual parameters
	check_actual_formal_param(formal_param_list);

	// create a local interpreter table
	// store the actual param values of the corresponding variables from eval_env

	Local_Environment param_table;
	list<Symbol_Table_Entry *> formal_list = formal_param_list.get_table();

	list<Symbol_Table_Entry *>::iterator j;
	list<Ast *>::iterator i;
	for(i = actual_param_list.begin(), j = formal_list.begin(); i != actual_param_list.end() && j != formal_list.end(); i++, j++)
	{
		Eval_Result & result = (*i)->evaluate(eval_env, file_buffer);
		param_table.put_variable_value(result, (*j)->get_variable_name());
	}
	
	// call evaluate of the function proc_name
	// receive the result in an eval_result
	// return eval_result

	Eval_Result & return_result = proc->evaluate(param_table, file_buffer);

	return return_result;
}

//////////////////////////////////////////////////////////////////////////////

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	//Eval_Result & result = *new Eval_Result_BB();
	//result.set_result_enum(next_bb_result);
	//result.set_next_bb(END_OF_PROG);

	Eval_Result * return_eval = NULL;

	switch(node_data_type)
	{
	case int_data_type:
		return_eval = new Eval_Result_Value_Int();
		return_eval->set_value(0);
		return_eval->set_result_enum(int_result);
		break;

	case double_data_type:
		return_eval = new Eval_Result_Value_Double();
		return_eval->set_value(0.0);
		return_eval->set_result_enum(double_result);
		break;
	}

	if(return_eval != NULL)
	{
		eval_env.put_variable_value(*return_eval, "return");
		if (return_value != NULL)
		{
			Eval_Result & inter_result = return_value->evaluate(eval_env, file_buffer);
			eval_env.put_variable_value(inter_result, "return");
		}
	}

	// print return ast
	print(file_buffer);

	return *return_eval;
}

////////////////////////////////////////////////////////////////////////


template class Number_Ast<double>;
template class Number_Ast<int>;


////////////////////////////////////////////////////////////////////////////////////////

Eval_Result & Write_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No evaluate() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Read_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No evaluate() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & String_Constant_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No evaluate() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

