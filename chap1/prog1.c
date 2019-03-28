#include "util.h"
#include "slp.h"
#include "prog1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

A_stm prog(void) {

return
 A_CompoundStm(A_AssignStm("a",
                 A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
  A_CompoundStm(A_AssignStm("b",
      A_EseqExp(A_PrintStm(A_PairExpList(A_IdExp("a"),
                 A_LastExpList(A_OpExp(A_IdExp("a"), A_minus,
                                       A_NumExp(1))))),
              A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
   A_PrintStm(A_LastExpList(A_IdExp("b")))));

}

int maxargs(A_stm stm) {
    assert(stm);
    switch(stm->kind) {
        case A_compoundStm:
            return max(maxargs(stm->u.compound.stm1), maxargs(stm->u.compound.stm2));
        case A_assignStm:
            return maxargs_exp(stm->u.assign.exp);
        case A_printStm:
            {
            A_expList tail = stm->u.print.exps;
            int exp_count = 1;
            while(tail->kind == A_pairExpList) {
                tail = tail->u.pair.tail;
                exp_count++;
            }
            return max(maxargs_list(stm->u.print.exps),exp_count);
            }
    }
}

int maxargs_exp(A_exp exp) {
    assert(exp);
    switch(exp->kind) {
        case A_idExp:
            return 0;
        case A_numExp:
            return 0;
        case A_opExp:
            return max(maxargs_exp(exp->u.op.left), maxargs_exp(exp->u.op.right));
        case A_eseqExp:
            return max(maxargs(exp->u.eseq.stm), maxargs_exp(exp->u.eseq.exp));
    }
}

int maxargs_list(A_expList list) {
    assert(list);
    switch(list->kind) {
        case A_pairExpList:
            return max(maxargs_exp(list->u.pair.head), maxargs_list(list->u.pair.tail));
        case A_lastExpList:
            return maxargs_exp(list->u.last);
    }
}

int max(int x, int y) {
    return x > y ? x : y;
}

Table_ Table(string id, int value, struct table *tail) {
    Table_ t = malloc(sizeof *t);
    t->id=id;
    t->value=value;
    t->tail=tail;
    return t;
}

void interp(A_stm stm) {
    assert(stm);
    interpStm(stm,NULL);
}


Table_ interpStm(A_stm s, Table_ t) {
    assert(s);
    switch(s->kind) {
        case A_compoundStm:
            {
            t = interpStm(s->u.compound.stm1, t);
            return interpStm(s->u.compound.stm2, t);
            }
        case A_assignStm:
            {
            struct IntAndTable tab = interpExp(s->u.assign.exp, t);
            int value = tab.i;
            string id = s->u.assign.id;
            t = tab.t;
            return Table(id,value,t);
            }
        case A_printStm:
            {
            int i;
            struct IntAndTable tab;
            A_expList tail = s->u.print.exps;

            // while not last $(exp)
            while(tail->kind == A_pairExpList) {
                // evaluate
                tab = interpExp(tail->u.pair.head, t);
                i = tab.i;

                // print evaluation
                printf("%d ", i);

                // update
                t = tab.t;
                tail = tail->u.pair.tail;
            }

            // evaluate last and update local var(s)
            tab = interpExp(tail->u.last, t);

            // print last $(exp)
            printf("%d\n",tab.i);
            return tab.t;
            }
    }

}
struct IntAndTable interpExp(A_exp e, Table_ t) {
    assert(e);
    struct IntAndTable res;
    switch(e->kind) {
        case A_idExp:
            res.i = lookup(t,e->u.id);
            res.t = t;
            return res;
        case A_numExp:
            res.i = e->u.num;
            res.t = t;
            return res;
        case A_opExp:
            {
            struct IntAndTable left = interpExp(e->u.op.left, t);
            int l = left.i;
            t = left.t;
            struct IntAndTable right = interpExp(e->u.op.right, t);
            int r = right.i;
            t = right.t;

            int result;
            // perform operation between left and right exp
            switch(e->u.op.oper) {
                case A_plus:
                    result = l + r;
                    break;
                case A_minus:
                    result = l - r;
                    break;
                case A_times:
                    result = l * r;
                    break;
                case A_div:
                    {
                    if(!r) {
                        printf("ayy nigga, you can't divide by 0");
                        exit(1);
                    }
                    result = l / r;
                    break;
                    }
            }
            res.i = result;
            res.t = t;
            return res;
            }
        case A_eseqExp:
            {
            t = interpStm(e->u.eseq.stm, t);
            return interpExp(e->u.eseq.exp, t);
            }
    }
}

int lookup(Table_ t, string key) {
    // while not last node, and not $(key)
    while(t != NULL && strcmp(t->id,key))
        t = t->tail;
    if(t == NULL) {
        printf("lookup error");
        exit(1);
    }
    return t->value;
}

//// 1.1.a
//T_tree Tree(T_tree l, string k, T_tree r) {
//    T_tree t = checked_malloc(sizeof(*t));
//    t->left=l;
//    t->key=k;
//    t->right=r;
//    return t;
//}
//T_tree insert(string key, T_tree t) {
//    if(t == NULL)
//        return Tree(NULL,key,NULL);
//    else if(strcmp(key,t->key) < 0)
//        return Tree(insert(key,t->left),t->key,t->right);
//    else if(strcmp(key,t->key) > 0)
//        return Tree(t->left,t->key,insert(key,t->right));
//    else
//        return Tree(t->left,key,t->right);
//}
//
//int member(T_tree t, string key) {
//    if(t == NULL)
//        return FALSE;
//    int x = strcmp(t->key,key);
//    if(x < 0)
//        return member(t->left, key);
//    else if(x > 0)
//        return member(t->right, key);
//    else
//        return TRUE;
//}
//
// 1.2.b
//T_tree Tree(T_tree l, string k, void *binding, T_tree r) {
//    T_tree t = checked_malloc(sizeof(*t));
//    t->left=l;
//    t->key=k;
//    t->binding=binding;
//    t->right=r;
//    return t;
//}
//T_tree insertTree(string key, void *binding, T_tree t) {
//    if(t == NULL)
//        return Tree(NULL,key, binding, NULL);
//    else if(strcmp(key,t->key) < 0)
//        return Tree(insertTree(key,binding,t->left),t->key,t->binding,t->right);
//    else if(strcmp(key,t->key) > 0)
//        return Tree(t->left,t->key,t->binding,insertTree(key,binding,t->right));
//    else
//        return Tree(t->left,key,binding,t->right);
//}
//
//
//void *lookupTree(string key, T_tree t) {
//    if(t == NULL)
//        return NULL;
//    int x = strcmp(t->key,key);
//    if(x < 0)
//        return lookupTree(key,t->left);
//    else if(x > 0)
//        return lookupTree(key,t->right);
//    else
//        return t->binding;
//}
