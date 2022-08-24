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

void Program::perform_lva()
{
    map<string, Procedure *>::iterator procedure;
    for (procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
    {
        (procedure->second)->perform_lva();
    }
}
void Program::print_lv()
{
	ostream *file_buffer; // For printing lv analysis using file_buffer
	command_options.create_lv_buffer();

	file_buffer = &(command_options.get_lv_buffer());

	map<string, Procedure *>::iterator procedure;
	for (procedure = procedure_map.begin(); procedure != procedure_map.end(); procedure++)
	{
		CHECK_INVARIANT(procedure->second != NULL, "Procedure object pointer cannot be Null");
		procedure->second->print_lv(*file_buffer);
	}
}

void Procedure::populate_symbol_table_temporary()
{
	set<string> temp_list;//Rnd Work // public data member converted to local variable (should work??)
    list<TAC_Stmt *>::iterator tac_stmt_list_itr;

    for (tac_stmt_list_itr = tac_stmt_list->begin(); tac_stmt_list_itr != tac_stmt_list->end(); tac_stmt_list_itr++)
    {

        if (typeid(**tac_stmt_list_itr) == typeid(IO_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
            {
                if(typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd1());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            entry->set_symbol_scope(local);
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        } 
                    }
            }
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Asgn_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                {
                    if(typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd1());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }       
                    }
                }
            if (((**tac_stmt_list_itr).get_result()) != NULL)
                {
                    if(typeid(*((**tac_stmt_list_itr).get_result())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_result());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
                }
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Compute_TAC_Stmt))
        {
            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
            {
                if(typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd1());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
            }
            if (((**tac_stmt_list_itr).get_opd2()) != NULL)
            {
                if(typeid(*((**tac_stmt_list_itr).get_opd2())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd2());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }       
                    }
            }
            if (((**tac_stmt_list_itr).get_result()) != NULL)
            {
                if(typeid(*((**tac_stmt_list_itr).get_result())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_result());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
            }
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(If_Goto_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                {
                    if(typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd1());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
                }
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Return_TAC_Stmt))
        {
            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                {
                    if(typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_opd1());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
                }
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Call_TAC_Stmt))
        {
            if ((**tac_stmt_list_itr).get_result() != NULL)
            {
                if(typeid(*((**tac_stmt_list_itr).get_result())) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>((**tac_stmt_list_itr).get_result());
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
            }

            Call_TAC_Stmt *cts = dynamic_cast<Call_TAC_Stmt *>(*tac_stmt_list_itr);
            list<TAC_Opd *> param_list = (*cts).get_actual_param_list();
            for (auto it = param_list.begin(); it != param_list.end(); it++)
            {
                if(typeid(**it) == typeid(Temporary_TAC_Opd))
                    {
                        Temporary_TAC_Opd *tp = dynamic_cast<Temporary_TAC_Opd *>(*it);
                        string name = "temp";
                        name.append(to_string((*tp).get_temp_number()));
                        if(temp_list.find(name) == temp_list.end())
                        {
                            temp_list.insert(name);
                            Symbol_Table_Entry* entry = new Symbol_Table_Entry(name,(*tp).get_basic_data_type());
                            (*tp).set_symbol_entry(entry);
                            list<Symbol_Table_Entry*> &sym_tab = temporary_variable_table.get_table();
                            sym_tab.push_back(entry);
                        }      
                    }
            }
        }
    }
}
void Procedure::print_lv(ostream &file_buffer)
{
	file_buffer << "============================== LIVE-VARIABLE ANALYSIS GEN-KILL VALUES ================================================="<<endl;
	lv_dfv_obj.print_gen_kill(file_buffer);
	file_buffer << "============================== FINAL DATA-FLOW VALUES ================================================="<<endl;
	lv_dfv_obj.print_final_values(file_buffer);
}
void Procedure::perform_lva()
{
    populate_symbol_table_temporary();
    list<TAC_Stmt *>::iterator tac_stmt_list_itr;

    for (tac_stmt_list_itr = tac_stmt_list->begin(); tac_stmt_list_itr != tac_stmt_list->end(); tac_stmt_list_itr++)
    {

        if (typeid(**tac_stmt_list_itr) == typeid(IO_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd1()));
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Asgn_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd1()));
            if (((**tac_stmt_list_itr).get_result()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_result()));    
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Compute_TAC_Stmt))
        {
            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd1()));
            if (((**tac_stmt_list_itr).get_opd2()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd2()));
            if (((**tac_stmt_list_itr).get_result()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_result()));
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(If_Goto_TAC_Stmt))
        {

            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd1()));
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Return_TAC_Stmt))
        {
            if (((**tac_stmt_list_itr).get_opd1()) != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_opd1()));
        }
        else if (typeid(**tac_stmt_list_itr) == typeid(Call_TAC_Stmt))
        {
            if ((**tac_stmt_list_itr).get_result() != NULL)
                lv_dfv_obj.populate_maps(((**tac_stmt_list_itr).get_result()));

            Call_TAC_Stmt *cts = dynamic_cast<Call_TAC_Stmt *>(*tac_stmt_list_itr);
            list<TAC_Opd *> param_list = (*cts).get_actual_param_list();
            for (auto it = param_list.begin(); it != param_list.end(); it++)
                lv_dfv_obj.populate_maps(*it);
        }
    }
    // dfs_numbering();
    // lv_dfv_obj.print_maps();

    list<Basic_Block *> bb_list = cfg->get_bb_list();
    list<Basic_Block *>::iterator bb_list_itr;
    for (bb_list_itr = bb_list.begin(); bb_list_itr != bb_list.end(); ++bb_list_itr)
    {
        lv_dfv_obj.compute_gen_kill_bb(*bb_list_itr);
    }
    // lv_dfv_obj.print_gen_kill();
    bool converge = 0;
    int itr = 1;
    while (!converge)
    {
        post_order_traversal(bb_list);
        // lv_dfv_obj.print_round_robin_iteration(itr++);
        converge = lv_dfv_obj.convergence(bb_list);
    }
}
void Procedure ::post_order_traversal(list<Basic_Block *> bb_list)
{
    int succ_pushed = 0;
    Basic_Block *entryBB = cfg->get_entry_block();
    Basic_Block *currBB = entryBB;

    set<int> visited;
    stack<int> stack_bbid;
    stack_bbid.push(currBB->get_bb_id());

    visited.insert(entryBB->get_bb_id());
    list<pair<int, string>>::iterator succ;
    // cout << "Post Order Traversal: " << endl;
    // cout << "         ";
    while (!stack_bbid.empty())
    {
        int curr_BB_Id = stack_bbid.top();
        succ_pushed = 0;
        currBB = cfg->get_bb_from_id(curr_BB_Id);
        list<pair<int, string>> succList = currBB->get_successors_list();
        succList.reverse();
        for (succ = succList.begin(); succ != succList.end(); ++succ)
        {
            if (!visited.count((*succ).first))
            {
                succ_pushed++;
                stack_bbid.push((*succ).first);
                currBB = cfg->get_bb_from_id(stack_bbid.top());
                visited.insert((*succ).first);
            }
        }
        if (succ_pushed == 0)
        {
            // cout << stack_bbid.top() << " ";
            lv_dfv_obj.compute_in_out_bb(cfg->get_bb_from_id(stack_bbid.top()));
            stack_bbid.pop();
        }
    }
    // cout << "\n\n";
}
void Procedure ::reverse_post_order_traversal(list<Basic_Block *> bb_list)
{
    int succ_pushed = 0;
    Basic_Block *entryBB = cfg->get_entry_block();
    Basic_Block *currBB = entryBB;

    set<int> visited;
    stack<int> stack_bbid;
    stack<int> reverse_order;
    stack_bbid.push(currBB->get_bb_id());

    visited.insert(entryBB->get_bb_id());
    list<pair<int, string>>::iterator succ;
    while (!stack_bbid.empty())
    {
        int curr_BB_Id = stack_bbid.top();
        succ_pushed = 0;
        currBB = cfg->get_bb_from_id(curr_BB_Id);
        list<pair<int, string>> succList = currBB->get_successors_list();
        succList.reverse();
        for (succ = succList.begin(); succ != succList.end(); ++succ)
        {
            if (!visited.count((*succ).first))
            {
                succ_pushed++;
                stack_bbid.push((*succ).first);
                currBB = cfg->get_bb_from_id(stack_bbid.top());
                visited.insert((*succ).first);
            }
        }
        if (succ_pushed == 0)
        {
            reverse_order.push(stack_bbid.top());
            stack_bbid.pop();
        }
    }
    // cout << "Reverse Post Order Traversal: " << endl;
    // cout << "         ";
    while(!reverse_order.empty())
    {
        // cout << reverse_order.top() << " ";
        av_dfv_obj.compute_in_out_bb(cfg->get_bb_from_id(reverse_order.top()));
        reverse_order.pop();
    }
    // cout << "\n\n";
}
