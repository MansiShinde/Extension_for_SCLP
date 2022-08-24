#include <bits/stdc++.h>
#include "common-headers.hh"
using namespace std;

void LV_DFV::populate_maps(TAC_Opd *variable)
{
    if (typeid(*variable) != typeid(Int_Const_TAC_Opd) && typeid(*variable) != typeid(Double_Const_TAC_Opd) && typeid(*variable) != typeid(Label_TAC_Opd) && typeid(*variable) != typeid(Str_Const_TAC_Opd))
    {
        if (variable_to_index_mapping.find(variable->get_symbol_entry()) == variable_to_index_mapping.end())
        {
            variable_to_index_mapping.insert(make_pair(variable->get_symbol_entry(), ind));
            reverse_variable_to_index_mapping.insert(make_pair(ind++, variable->get_symbol_entry()));
        }
    }
}

void LV_DFV::populate_gen_map(int bb_id, int idx)
{
    Gen[bb_id].insert(idx);
}
void LV_DFV::populate_kill_map(int bb_id, int idx)
{
    Kill[bb_id].insert(idx);
}

map<Symbol_Table_Entry *, int> LV_DFV ::get_v_to_i()
{
    return variable_to_index_mapping;
}

map<int, Symbol_Table_Entry *> LV_DFV::get_i_to_v()
{
    return reverse_variable_to_index_mapping;
}

void LV_DFV ::compute_gen_kill_bb(Basic_Block *currBB)
{
    list<TAC_Stmt *> tac_stmts = currBB->get_tac_stmts();
    Gen[currBB->get_bb_id()] = {};
    Kill[currBB->get_bb_id()] = {};
    list<TAC_Stmt *>::iterator tac_stmt_itr;
    for (tac_stmt_itr = tac_stmts.begin(); tac_stmt_itr != tac_stmts.end(); tac_stmt_itr++)
    {
        if (typeid(**tac_stmt_itr) == typeid(IO_TAC_Stmt))
        {
            if (((**tac_stmt_itr).get_opd1()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd1()->get_symbol_entry()];
                    if ((**tac_stmt_itr).get_op() == write_3a)
                    {

                        if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                            populate_gen_map(currBB->get_bb_id(), idx);
                    }
                    else
                    {
                        if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end())
                            populate_kill_map(currBB->get_bb_id(), idx);
                    }
                }
            }
        }
        else if (typeid(**tac_stmt_itr) == typeid(Asgn_TAC_Stmt))
        {
            if (((**tac_stmt_itr).get_opd1()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd1()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                        populate_gen_map(currBB->get_bb_id(), idx);
                }
            }
            if (((**tac_stmt_itr).get_result()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_result())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_result()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end())
                        populate_kill_map(currBB->get_bb_id(), idx);
                }
            }
        }
        else if (typeid(**tac_stmt_itr) == typeid(Compute_TAC_Stmt))
        {
            if (((**tac_stmt_itr).get_opd1()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd1()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                        populate_gen_map(currBB->get_bb_id(), idx);
                }
            }
            if (((**tac_stmt_itr).get_opd2()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_opd2())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd2())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd2())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd2())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd2()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                        populate_gen_map(currBB->get_bb_id(), idx);
                }
            }
            if (((**tac_stmt_itr).get_result()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_result())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_result()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end())
                        populate_kill_map(currBB->get_bb_id(), idx);
                }
            }
        }
        else if (typeid(**tac_stmt_itr) == typeid(If_Goto_TAC_Stmt))
        {

            if (((**tac_stmt_itr).get_opd1()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd1()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                        populate_gen_map(currBB->get_bb_id(), idx);
                }
            }
        }
        else if (typeid(**tac_stmt_itr) == typeid(Return_TAC_Stmt))
        {
            if (typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_opd1())) != typeid(Str_Const_TAC_Opd))
            {
                int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_opd1()->get_symbol_entry()];
                if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                    populate_gen_map(currBB->get_bb_id(), idx);
            }
        }
        else if (typeid(**tac_stmt_itr) == typeid(Call_TAC_Stmt))
        {
            if (((**tac_stmt_itr).get_result()) != NULL)
            {
                if (typeid(*((**tac_stmt_itr).get_result())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_itr).get_result())) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*tac_stmt_itr)->get_result()->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end())
                        populate_kill_map(currBB->get_bb_id(), idx);
                }
            }
            Call_TAC_Stmt *cts = dynamic_cast<Call_TAC_Stmt *>(*tac_stmt_itr);
            list<TAC_Opd *> param_list = (*cts).get_actual_param_list();
            for (auto it = param_list.begin(); it != param_list.end(); it++)
            {
                if (typeid(**it) != typeid(Int_Const_TAC_Opd) && typeid(**it) != typeid(Double_Const_TAC_Opd) && typeid(**it) != typeid(Label_TAC_Opd) && typeid(**it) != typeid(Str_Const_TAC_Opd))
                {
                    int idx = variable_to_index_mapping[(*it)->get_symbol_entry()];
                    if (Kill[currBB->get_bb_id()].find(idx) == Kill[currBB->get_bb_id()].end() && Gen[currBB->get_bb_id()].find(idx) == Gen[currBB->get_bb_id()].end())
                        populate_gen_map(currBB->get_bb_id(), idx);
                }
            }
        }
    }
}

void LV_DFV ::print_maps()
{
    //================================================================================================================================================//
    /*Iterating over the maps to get the variable to index mapping and vice versa*/
    map<Symbol_Table_Entry *, int>::iterator v_to_i;
    map<int, Symbol_Table_Entry *>::iterator i_to_v;
    cout << "_______________________________________________________________________" << endl;
    cout << "*******************Variable To Index Map*******************************" << endl;

    cout << "Map Size:" << (get_v_to_i()).size() << endl;
    map<Symbol_Table_Entry *, int> t_map = get_v_to_i();
    map<int, Symbol_Table_Entry *> t_rev_map = get_i_to_v();

    for (v_to_i = t_map.begin(); v_to_i != t_map.end(); v_to_i++)
    {
        cout << "Variable : " << ((*v_to_i).first)->get_variable_name() << " Index : " << (*v_to_i).second << endl;
    }
    cout << "_______________________________________________________________________" << endl;
    cout << "*****************************Reverse Map*******************************" << endl;

    cout << "Map Size:" << (get_i_to_v()).size() << endl;
    for (i_to_v = t_rev_map.begin(); i_to_v != t_rev_map.end(); i_to_v++)
    {
        cout << "Index : " << (*i_to_v).first << " Variable :" << ((*i_to_v).second)->get_variable_name() << endl;
    }
    cout << "_______________________________________________________________________" << endl;
}
// //======================================================================================================================================================//
// Print Analysis Related Information
map<int, set<int>> LV_DFV::get_GenMap()
{
    return Gen;
}

map<int, set<int>> LV_DFV::get_KillMap()
{
    return Kill;
}
// //=============================================================================================================================================//

//Print Gen-kill maps
void LV_DFV::print_gen_kill(ostream & file_buffer)
{
    map<int, set<int>>::iterator gen;
    map<int, set<int>>::iterator killitr;
    file_buffer << "__________________________________________________________________________" << endl;
    file_buffer << "******************************** Gen Map *********************************\n"
         << endl;
    map<int, set<int>> genMAP = get_GenMap();
    map<int, Symbol_Table_Entry *> iv = get_i_to_v();
    for (gen = genMAP.begin(); gen != genMAP.end(); gen++)
    {
        file_buffer << "Basic Block : " << (*gen).first << " ----> ";
        set<int>::iterator i;
        file_buffer << "{";
        for (i = ((*gen).second).begin(); i != ((*gen).second).end(); i++)
        {
            file_buffer << iv[(*i)]->get_variable_name() << ", ";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "_________________________________________________________________________" << endl;

    file_buffer << "_________________________________________________________________________" << endl;
    file_buffer << "******************************** Kill Map *******************************\n"
         << endl;
    map<int, set<int>> killMAP = get_KillMap();
    for (killitr = killMAP.begin(); killitr != killMAP.end(); killitr++)
    {
        file_buffer << "Basic Block : " << (*killitr).first << " ----> ";
        set<int>::iterator i;
        file_buffer << "{";
        for (i = ((*killitr).second).begin(); i != ((*killitr).second).end(); i++)
        {
            file_buffer << iv[(*i)]->get_variable_name() << ", ";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "_______________________________________________________________________\n"
         << endl;
}

void LV_DFV::compute_in_out_bb(Basic_Block *currBB)
{
    int bb_id = currBB->get_bb_id();
    in_prev[bb_id] = set<int>();
    in_prev[bb_id] = in_curr[bb_id];
    out_prev[bb_id] = set<int>();
    out_prev[bb_id] = out_curr[bb_id];
    list<pair<int, string>> succlist = currBB->get_successors_list();
    for (auto i = succlist.begin(); i != succlist.end(); ++i)
    {
        (out_curr[bb_id]).insert(in_curr[(*i).first].begin(), in_curr[(*i).first].end());
    }
    set_difference(out_curr[bb_id].begin(), out_curr[bb_id].end(), Kill[bb_id].begin(), Kill[bb_id].end(), inserter(in_curr[bb_id], in_curr[bb_id].end()));
    in_curr[bb_id].insert(Gen[bb_id].begin(), Gen[bb_id].end());
}

bool LV_DFV::convergence(list<Basic_Block *> bb_list)
{
    for (auto it = bb_list.begin(); it != bb_list.end(); it++)
    {
        if (in_prev[(*it)->get_bb_id()] == in_curr[(*it)->get_bb_id()] && out_prev[(*it)->get_bb_id()] == out_curr[(*it)->get_bb_id()])
            continue;
        else
        {
            return 0;
        }
    }
    return 1;
}
void LV_DFV::print_round_robin_iteration(int itr)
{
    cout << "_________________________Iteration :" << itr << "____________________________" << endl;
    cout << "__________________________________________________________________________" << endl;
    cout << "*******************Out Data-flow values Map*******************************\n"
         << endl;
    map<int, Symbol_Table_Entry *> iv = get_i_to_v();
    for (auto it = out_curr.begin(); it != out_curr.end(); ++it)
    {
        cout << "Basic Block : " << (*it).first << " ---->";
        set<int> out = (*it).second;
        cout << " {";
        for (auto i = out.begin(); i != out.end(); ++i)
        {
            cout << iv[*i]->get_variable_name() << " ,";
        }
        cout << "}" << endl;
    }
    cout << "___________________________________________________________________________\n"
         << endl;
    cout << "*******************In Data-flow values Map*******************************\n"
         << endl;
    for (auto it = in_curr.begin(); it != in_curr.end(); ++it)
    {
        cout << "Basic Block : " << (*it).first << " ---->";
        set<int> in = (*it).second;
        cout << " {";
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            cout << iv[*i]->get_variable_name() << " ,";
        }
        cout << "}" << endl;
    }
    cout << "__________________________________________________________________________\n"
         << endl;
}
void LV_DFV::print_final_values(ostream &file_buffer)
{
        file_buffer << "__________________________________________________________________________" << endl;
    file_buffer << "*******************Out Data-flow values Map*******************************\n"
         << endl;
    map<int, Symbol_Table_Entry *> iv = get_i_to_v();
    for (auto it = out_curr.begin(); it != out_curr.end(); ++it)
    {
        file_buffer << "Basic Block : " << (*it).first << " ---->";
        set<int> out = (*it).second;
        file_buffer << " {";
        for (auto i = out.begin(); i != out.end(); ++i)
        {
            file_buffer << iv[*i]->get_variable_name() << " ,";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "___________________________________________________________________________\n"
         << endl;
    file_buffer << "*******************In Data-flow values Map*******************************\n"
         << endl;
    for (auto it = in_curr.begin(); it != in_curr.end(); ++it)
    {
        file_buffer << "Basic Block : " << (*it).first << " ---->";
        set<int> in = (*it).second;
        file_buffer << " {";
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            file_buffer << iv[*i]->get_variable_name() << " ,";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "__________________________________________________________________________\n"
         << endl;    
}
