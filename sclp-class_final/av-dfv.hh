#ifndef AV_DFV_HH
#define AV_DFV_HH

// #include "tac.hh"

struct Expression{
    Symbol_Table_Entry* op1;
    Symbol_Table_Entry* op2;
    string opt;
    string exp_string; 
};

using namespace std;

class AV_DFV
{
    int ind = 1;
    map<Expression*, int> expression_to_index;
    map<int, Expression*> index_to_expression;
    map<int, set<int>> in_curr;
    map<int, set<int>> out_curr;
    map<int, set<int>> in_prev;
    map<int, set<int>> out_prev;
    map<int, set<int>> Gen;
    map<int, set<int>> Kill;
    map<string,Expression*> expression_string_to_reference;     // public data member 

public:
    map<string,Expression*> get_expression_string_to_reference();
    void set_expression_string_to_reference(string,Expression*);
    void populate_expression_maps(Expression*);
    map<Expression*, int> get_e_to_i();
    map<int, Expression*> get_i_to_e();
    map<int, set<int>> get_GenMap();
    map<int, set<int>> get_KillMap();
    // string get_op_name(TAC_Op op);
    void print_round_robin_iteration(int);
    void print_final_values(ostream &);
    bool convergence(list<Basic_Block *>);
    void compute_in_out_bb(Basic_Block *);
    void compute_gen_kill(Basic_Block *);
    void populate_gen_map(int, int);
    void populate_kill_map(int, int);
    void print_maps();
    void print_gen_kill_maps(ostream &);
    void init(Basic_Block *, int);
};
#endif