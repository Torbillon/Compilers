A_stm prog(void);

/*
 * programming exercise
 * */
typedef struct table *Table_;
struct table {string id; int value; Table_ tail;};
Table_ Table(string id, int value, struct table *tail);
struct IntAndTable{int i; Table_ t;};
struct IntAndTable interpExp(A_exp e, Table_ t);
void interp(A_stm stm);

int maxargs(A_stm stm);
int maxargs_exp(A_exp exp);
int maxargs_list(A_expList list);
int max(int x, int y);
Table_ Table(string id, int value, struct table *tail);
void interp(A_stm stm);
Table_ interpStm(A_stm s, Table_ t);
struct IntAndTable interpExp(A_exp e, Table_ t);
int lookup(Table_ t, string key);

/*
 * Exercise 1.1
 * */

// a.
typedef  struct tree *T_tree;
//struct tree {T_tree left; string key; T_tree right;};
//T_tree Tree(T_tree l, string k, T_tree r);
//T_tree insert(string key, T_tree t);
//int member(T_tree t, string key);

// b.
struct tree {T_tree left; string key; void *binding; T_tree right;};
T_tree insertTree(string key, void *binding, T_tree t);
void *lookupTree(string key, T_tree t);
