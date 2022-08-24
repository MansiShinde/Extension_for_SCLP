#ifndef LV_DFV_HH
#define LV_DFV_HH

#include <set>
#include <map>
using namespace std;
class LV_DFV
{

    int ind = 1;
    map<int, set<int>> Gen;
    map<int, set<int>> Kill;
    map<int, set<int>> in_curr;
    map<int, set<int>> out_curr;
    map<int, set<int>> in_prev;
    map<int, set<int>> out_prev;
    map<Symbol_Table_Entry *, int> variable_to_index_mapping;
    map<int, Symbol_Table_Entry *> reverse_variable_to_index_mapping;

public:
    void populate_maps(TAC_Opd *);
    map<Symbol_Table_Entry *, int> get_v_to_i();
    map<int, Symbol_Table_Entry *> get_i_to_v();
    map<int, set<int>> get_GenMap();
    map<int, set<int>> get_KillMap();
    void print_round_robin_iteration(int);
    void print_final_values(ostream&);
    void print_maps();
    void print_gen_kill(ostream &);
    void compute_gen_kill_bb(Basic_Block *);
    void populate_gen_map(int, int);
    void populate_kill_map(int, int);
    void compute_in_out_bb(Basic_Block *);
    bool convergence(list<Basic_Block *>);
};
#endif