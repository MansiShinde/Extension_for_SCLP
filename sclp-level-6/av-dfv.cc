#include <bits/stdc++.h>
#include "common-headers.hh"
using namespace std;

void AV_DFV::populate_expression_maps(Expression* exp)
{
    if (expression_to_index.find(exp) == expression_to_index.end())
    {
        expression_to_index[exp] = ind;
        index_to_expression[ind++] = exp;
    }
}
void AV_DFV::set_expression_string_to_reference(string name,Expression* ex)
{   
    expression_string_to_reference[name] = ex;
}
map<string,Expression*> AV_DFV::get_expression_string_to_reference()
{
    return expression_string_to_reference;
}
map<Expression*, int> AV_DFV::get_e_to_i()
{
    return expression_to_index;
}
map<int, Expression*> AV_DFV::get_i_to_e()
{
    return index_to_expression;
}
void AV_DFV::print_maps()
{
    cout << "______________________________________________________________________" << endl;
    cout << "************************Expression to Index map***********************\n"
         << endl;
    for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
    {
        cout << "Expression : " << ((*it).first)->exp_string << " Index : " << (*it).second << endl;
    }
    cout << "______________________________________________________________________\n"
         << endl;
    cout << "______________________________________________________________________" << endl;
    cout << "************************Index to Expression map***********************\n"
         << endl;

    for (auto it = index_to_expression.begin(); it != index_to_expression.end(); ++it)
    {
        cout << "Index : " << (*it).first << " Expression : " << ((*it).second)->exp_string << endl;
    }
    cout << "______________________________________________________________________" << endl;
    // cout << "______________________________________________________________________" << endl;
    // for(auto it = expression_string_to_reference.begin(); it!= expression_string_to_reference.end(); it++)
    // {
    //     cout<<(*it).first<<"===========>"<<(*it).second->op1->get_variable_name()<<endl;
    // }
    // cout << "______________________________________________________________________" << endl;
}
// string AV_DFV:get_op_name(TAC_Op op)
// {
//     string name;

// 	switch (op)
// 	{
// 	case (add_3a):
// 		name = "+";
// 		break;
// 	case (sub_3a):
// 		name = "-";
// 		break;
// 	case (mult_3a):
// 		name = "*";
// 		break;
// 	case (div_3a):
// 		name = "/";
// 		break;
// 	case (uminus_3a):
// 		name = "-";
// 		break;
// 	case (and_3a):
// 		name = "&&";
// 		break;
// 	case (or_3a):
// 		name = "||";
// 		break;
// 	case (not_3a):
// 		name = "!";
// 		break;
// 	case (lt_3a):
// 		name = "<";
// 		break;
// 	case (leq_3a):
// 		name = "<=";
// 		break;
// 	case (gt_3a):
// 		name = ">";
// 		break;
// 	case (geq_3a):
// 		name = ">=";
// 		break;
// 	case (eq_3a):
// 		name = "==";
// 		break;
// 	case (neq_3a):
// 		name = "!=";
// 		break;
// 	}
// 	return name;
// }
void AV_DFV::compute_gen_kill(Basic_Block *currBB)
{
    int bb_id = currBB->get_bb_id();
    Gen[bb_id] = {};
    Kill[bb_id] = {};
    list<TAC_Stmt *> tac_stmt_list = currBB->get_tac_stmts();
    tac_stmt_list.reverse();
    list<TAC_Stmt *>::iterator tac_stmt_list_itr;
    for (tac_stmt_list_itr = tac_stmt_list.begin(); tac_stmt_list_itr != tac_stmt_list.end(); ++tac_stmt_list_itr)
    {
        if (typeid(**tac_stmt_list_itr) == typeid(Label_TAC_Stmt)||typeid(**tac_stmt_list_itr) == typeid(NOP_TAC_Stmt))
            continue;
        if (typeid(**tac_stmt_list_itr) == typeid(Compute_TAC_Stmt))
        {
            Symbol_Table_Entry *op1, *op2;
            string opt;
            if ((**tac_stmt_list_itr).get_opd1() != NULL && ( typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Variable_TAC_Opd) || typeid(*((**tac_stmt_list_itr).get_opd1())) == typeid(Temporary_TAC_Opd)) && (**tac_stmt_list_itr).get_opd2() != NULL && (typeid(*((**tac_stmt_list_itr).get_opd2())) == typeid(Variable_TAC_Opd) || typeid(*((**tac_stmt_list_itr).get_opd2())) == typeid(Temporary_TAC_Opd))  && (**tac_stmt_list_itr).get_op() != and_3a && (**tac_stmt_list_itr).get_op() != or_3a && (**tac_stmt_list_itr).get_op() != not_3a && (**tac_stmt_list_itr).get_op() != lt_3a && (**tac_stmt_list_itr).get_op() != leq_3a && (**tac_stmt_list_itr).get_op() != gt_3a && (**tac_stmt_list_itr).get_op() != geq_3a && (**tac_stmt_list_itr).get_op() != eq_3a && (**tac_stmt_list_itr).get_op() != neq_3a)
            {
                op1 = (**tac_stmt_list_itr).get_opd1()->get_symbol_entry();
                op2 = (**tac_stmt_list_itr).get_opd2()->get_symbol_entry();

                Compute_TAC_Stmt* ct = dynamic_cast<Compute_TAC_Stmt*>((*tac_stmt_list_itr));
                string opt = (*ct).op_name((**tac_stmt_list_itr).get_op());
                string name = op1->get_variable_name();
                name.append(opt);
                name.append(op2->get_variable_name());
                string rev = op2->get_variable_name(); 
                rev.append(opt);
                rev.append(op1->get_variable_name());
                Expression *exp;
                if ((opt == "+" || opt == "*"))
                {
                    if (expression_string_to_reference.find(name) != expression_string_to_reference.end())
                    {
                        exp = expression_string_to_reference[name];
                    }
                    else if (expression_string_to_reference.find(rev) != expression_string_to_reference.end())
                    {
                        exp = expression_string_to_reference[rev];
                    }
                    if (Gen[bb_id].find(expression_to_index[exp]) == Gen[bb_id].end() && Kill[bb_id].find(expression_to_index[exp]) == Kill[bb_id].end())
                        populate_gen_map(bb_id, expression_to_index[exp]);
                }
                else if ((opt == "-" || opt == "/"))
                {
                    exp = expression_string_to_reference[name];
                    if (Gen[bb_id].find(expression_to_index[exp]) == Gen[bb_id].end() && Kill[bb_id].find(expression_to_index[exp]) == Kill[bb_id].end())
                        populate_gen_map(bb_id, expression_to_index[exp]);
                }
            }
            if ((**tac_stmt_list_itr).get_result() != NULL)
            {
                string symbol = (**tac_stmt_list_itr).get_result()->get_symbol_entry()->get_variable_name();
                for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
                {
                    string search = (*it).first->exp_string;
                    if (search.find(symbol) != string::npos)
                    {
                        if (Kill[bb_id].find((*it).second) == Kill[bb_id].end())
                            populate_kill_map(bb_id, (*it).second);
                    }
                }
            }
        }
        else if (typeid(**tac_stmt_list_itr) != typeid(IO_TAC_Stmt) && typeid(*((**tac_stmt_list_itr).get_result())) != typeid(Int_Const_TAC_Opd) && typeid(*((**tac_stmt_list_itr).get_result())) != typeid(Double_Const_TAC_Opd) && typeid(*((**tac_stmt_list_itr).get_result())) != typeid(Label_TAC_Opd) && typeid(*((**tac_stmt_list_itr).get_result())) != typeid(Str_Const_TAC_Opd) && typeid(*((**tac_stmt_list_itr).get_result())) != typeid(Basic_Block_TAC_Opd))
        {
            if ((**tac_stmt_list_itr).get_result() != NULL)
            {
                string symbol = (**tac_stmt_list_itr).get_result()->get_symbol_entry()->get_variable_name();
                for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
                {
                    string search = (*it).first->exp_string;
                    if (search.find(symbol) != string::npos)
                    {
                        if (Kill[bb_id].find((*it).second) == Kill[bb_id].end())
                            populate_kill_map(bb_id, (*it).second);
                    }
                }
            }
        }
        else if (typeid((**tac_stmt_list_itr)) == typeid(IO_TAC_Stmt))
        {
            if ((**tac_stmt_list_itr).get_op() == read_3a)
            {
                string symbol = (**tac_stmt_list_itr).get_opd1()->get_symbol_entry()->get_variable_name();
                for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
                {
                    string search = (*it).first->exp_string;
                    if (search.find(symbol) != string::npos)
                    {
                        if (Kill[bb_id].find((*it).second) == Kill[bb_id].end())
                            populate_kill_map(bb_id, (*it).second);
                    }
                }
            }
        }
    }
}
void AV_DFV::populate_gen_map(int bb_id, int idx)
{
    Gen[bb_id].insert(idx);
}
void AV_DFV::populate_kill_map(int bb_id, int idx)
{
    Kill[bb_id].insert(idx);
}
map<int, set<int>> AV_DFV::get_GenMap()
{
    return Gen;
}

map<int, set<int>> AV_DFV::get_KillMap()
{
    return Kill;
}
void AV_DFV::print_gen_kill_maps(ostream &file_buffer)
{
    map<int, set<int>>::iterator gen;
    map<int, set<int>>::iterator killitr;
    file_buffer << "_________________________________________________________________________" << endl;
    file_buffer << "****************************** Gen Map **********************************\n"
         << endl;
    map<int, set<int>> genMAP = get_GenMap();
    map<int, Expression *> iv = get_i_to_e();
    for (gen = genMAP.begin(); gen != genMAP.end(); gen++)
    {
        file_buffer << "Basic Block : " << (*gen).first << " ----> ";
        set<int>::iterator i;
        file_buffer << "{";
        for (i = ((*gen).second).begin(); i != ((*gen).second).end(); i++)
        {
            file_buffer << iv[(*i)]->exp_string << ", ";
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
            file_buffer << iv[(*i)]->exp_string << ", ";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "_______________________________________________________________________\n"
         << endl;
}
void AV_DFV::init(Basic_Block *currBB, int iter)
{
    int bb_id = currBB->get_bb_id();
    if (iter == 1)
    {
        in_prev[bb_id] = set<int>();
        in_curr[bb_id] = set<int>();
        for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
        {
            out_prev[bb_id].insert((*it).second);
            out_curr[bb_id].insert((*it).second);
        }
    }
    else
    {
        for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
        {
            in_prev[bb_id].insert((*it).second);
            in_curr[bb_id].insert((*it).second);
            out_prev[bb_id].insert((*it).second);
            out_curr[bb_id].insert((*it).second);
        }
    }
}
void AV_DFV::compute_in_out_bb(Basic_Block *currBB)
{
    int bb_id = currBB->get_bb_id();
    in_prev[bb_id] = in_curr[bb_id];
    out_prev[bb_id] = out_curr[bb_id];
    list<int> predlist = currBB->get_predecessors_list();
    set<int> U;
    set<int> r = {};
    for (auto it = expression_to_index.begin(); it != expression_to_index.end(); ++it)
    {
        U.insert((*it).second);
    }
    for (auto it = predlist.begin(); it != predlist.end(); ++it)
    {
        set_intersection(U.begin(), U.end(), out_curr[(*it)].begin(), out_curr[(*it)].end(), inserter(r, r.end()));
        U = r;
        r = {};
        in_curr[bb_id] = U;
    }

    set<int> tmp = {};
    set_difference(in_curr[bb_id].begin(), in_curr[bb_id].end(), Kill[bb_id].begin(), Kill[bb_id].end(), inserter(tmp, tmp.end()));
    tmp.insert(Gen[bb_id].begin(), Gen[bb_id].end());
    out_curr[bb_id] = tmp;
}
bool AV_DFV::convergence(list<Basic_Block *> bb_list)
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
void AV_DFV::print_round_robin_iteration(int itr)
{
    if (itr != 1)
    {
        cout << "_________________________Iteration :" << itr - 1 << "____________________________________" << endl;
        cout << "*******************In Data-flow values Map*******************************\n"
             << endl;
    }
    else
    {
        cout << "_________________________Initialization :_____________________________________" << endl;
        cout << "*******************In Data-flow values Map*******************************\n"
             << endl;
    }
    map<int, Expression *> ie = get_i_to_e();
    for (auto it = in_curr.begin(); it != in_curr.end(); ++it)
    {
        cout << "Basic Block : " << (*it).first << " ---->";
        set<int> in = (*it).second;
        cout << " {";
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            cout << ie[*i]->exp_string << ",";
        }
        cout << "}" << endl;
    }
    cout << "__________________________________________________________________________\n"
         << endl;
    cout << "__________________________________________________________________________" << endl;
    cout << "*******************Out Data-flow values Map*******************************\n"
         << endl;
    for (auto it = out_curr.begin(); it != out_curr.end(); ++it)
    {
        cout << "Basic Block : " << (*it).first << " ---->";
        set<int> out = (*it).second;
        cout << " {";
        for (auto i = out.begin(); i != out.end(); ++i)
        {
            cout << ie[*i]->exp_string << ",";
        }
        cout << "}" << endl;
    }
    cout << "___________________________________________________________________________\n"
         << endl;
}
void AV_DFV::print_final_values(ostream &file_buffer)
{
    file_buffer << "__________________________________________________________________________\n"
         << endl;
    file_buffer << "*******************In Data-flow values Map*******************************\n"
             << endl;
    map<int, Expression *> ie = get_i_to_e();
    for (auto it = in_curr.begin(); it != in_curr.end(); ++it)
    {
        file_buffer << "Basic Block : " << (*it).first << " ---->";
        set<int> in = (*it).second;
        file_buffer << " {";
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            file_buffer << ie[*i]->exp_string << ",";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "__________________________________________________________________________\n"
         << endl;
    file_buffer << "__________________________________________________________________________" << endl;
    file_buffer << "*******************Out Data-flow values Map*******************************\n"
         << endl;
    for (auto it = out_curr.begin(); it != out_curr.end(); ++it)
    {
        file_buffer << "Basic Block : " << (*it).first << " ---->";
        set<int> out = (*it).second;
        file_buffer << " {";
        for (auto i = out.begin(); i != out.end(); ++i)
        {
            file_buffer << ie[*i]->exp_string << ",";
        }
        file_buffer << "}" << endl;
    }
    file_buffer << "___________________________________________________________________________\n"
         << endl;
}