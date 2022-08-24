#include <iostream>
#include <fstream>
#include <typeinfo>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "common-headers.hh"

void Program::perform_ava()
{
    map<string, Procedure *>::iterator procedure;
    for (procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
    {
        (procedure->second)->perform_ava();
    }
}

void Program::print_av()
{
	ostream *file_buffer; // For printing Av analysis using file_buffer
	command_options.create_av_buffer();

	file_buffer = &(command_options.get_av_buffer());

	map<string, Procedure *>::iterator procedure;
	for (procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
	{
		CHECK_INVARIANT(procedure->second != NULL, "Procedure object pointer cannot be Null");
		procedure->second->print_av(*file_buffer);
	}
}

void Procedure::perform_ava()
{
    populate_symbol_table_temporary();
    list<TAC_Stmt *>::iterator tac_stmt_itr;
    for (tac_stmt_itr = tac_stmt_list->begin(); tac_stmt_itr != tac_stmt_list->end(); ++tac_stmt_itr)
    {
        map<string,Expression*> expression_string_to_reference = av_dfv_obj.get_expression_string_to_reference();
        if (typeid(**tac_stmt_itr) == typeid(Compute_TAC_Stmt))
        {
            if ((**tac_stmt_itr).get_opd1() != NULL && ( typeid(*((**tac_stmt_itr).get_opd1())) == typeid(Variable_TAC_Opd) || typeid(*((**tac_stmt_itr).get_opd1())) == typeid(Temporary_TAC_Opd)) && (**tac_stmt_itr).get_opd2() != NULL && (typeid(*((**tac_stmt_itr).get_opd2())) == typeid(Variable_TAC_Opd) || typeid(*((**tac_stmt_itr).get_opd2())) == typeid(Temporary_TAC_Opd))  && (**tac_stmt_itr).get_op() != and_3a && (**tac_stmt_itr).get_op() != or_3a && (**tac_stmt_itr).get_op() != not_3a && (**tac_stmt_itr).get_op() != lt_3a && (**tac_stmt_itr).get_op() != leq_3a && (**tac_stmt_itr).get_op() != gt_3a && (**tac_stmt_itr).get_op() != geq_3a && (**tac_stmt_itr).get_op() != eq_3a && (**tac_stmt_itr).get_op() != neq_3a)
            {
                Symbol_Table_Entry *op1, *op2;
                op1 = (**tac_stmt_itr).get_opd1()->get_symbol_entry();
                op2 = (**tac_stmt_itr).get_opd2()->get_symbol_entry();
                Compute_TAC_Stmt* ct = dynamic_cast<Compute_TAC_Stmt*>((*tac_stmt_itr));
                string opt = (*ct).op_name((**tac_stmt_itr).get_op());
                string name = op1->get_variable_name();
                name.append(opt);
                name.append(op2->get_variable_name());
                if (opt == "+" || opt == "*")
                    {
                        string rev = op2->get_variable_name(); 
                        rev.append(opt);
                        rev.append(op1->get_variable_name());
                        if(expression_string_to_reference.find(name) == expression_string_to_reference.end() && expression_string_to_reference.find(rev) == expression_string_to_reference.end())
                        {
                            Expression *ex = new Expression();
                            (*ex).exp_string = name;
                            (*ex).op1 = op1;
                            (*ex).op2 = op2;
                            (*ex).opt = opt; 
                            av_dfv_obj.set_expression_string_to_reference(name,ex);
                            av_dfv_obj.populate_expression_maps(ex);
                        }
                    }
                    else if (opt == "/" || opt == "-")
                        {
                            if(expression_string_to_reference.find(name) == expression_string_to_reference.end())
                            {
                                Expression *ex = new Expression();
                                (*ex).exp_string = name;
                                (*ex).op1 = op1;
                                (*ex).op2 = op2;
                                (*ex).opt = opt; 
                                av_dfv_obj.set_expression_string_to_reference(name,ex);
                                av_dfv_obj.populate_expression_maps(ex);
                            }
                        }
            }
        }
    }
    // av_dfv_obj.print_maps();
    list<Basic_Block *> bb_list = cfg->get_bb_list();
    list<Basic_Block *>::iterator bb_list_itr;
    int iter = 1;
    for (bb_list_itr = bb_list.begin(); bb_list_itr != bb_list.end(); ++bb_list_itr)
    {
        av_dfv_obj.compute_gen_kill(*bb_list_itr);
        av_dfv_obj.init(*bb_list_itr, iter++);
    }
    // av_dfv_obj.print_gen_kill_maps();
    bool converge = 0;
    int itr = 1;
    // av_dfv_obj.print_round_robin_iteration(itr++);
    while (!converge)
    {
        reverse_post_order_traversal(bb_list);
        // av_dfv_obj.print_round_robin_iteration(itr++);
        converge = av_dfv_obj.convergence(bb_list);
    }
}
void Procedure::print_av(ostream &file_buffer)
{
	file_buffer << "============================== AVAILABLE-EXPRESSION ANALYSIS GEN-KILL VALUES ================================================="<<endl;	
	av_dfv_obj.print_gen_kill_maps(file_buffer);
	file_buffer << "============================== FINAL DATA-FLOW VALUES ================================================="<<endl;
	av_dfv_obj.print_final_values(file_buffer);
}
