int maxargs(A_stm stm) {
    assert(stm);
    switch(stm->kind) {
        case A_compoundStm:
            int a = maxargs(stm->u.compound.stm1);
            int b = maxargs(stm->u.compound.stm2);
            return b > a ? b : a;
        case A_assignStm:
            return maxargs(stm->u.assign.exp);

        // TODO
        case A_printStm:
            int count = 0;
            int max = 0;
            A_expList tail = stm->u.print.exps;
            while(tail->kind == A_pairExpList) {
                int temp = maxargs(tail);
                max = max > temp ? max : temp;
                tail = tail->u.pair.tail;
                count++;
            }
            count++;
            return max > count ? max : count;
    }
}

int maxargs(A_exp exp) {
    assert(exp);
    switch(exp->kind) {
        case A_idExp:
            return maxargs(exp->u.compound.stm1);
        case A_numExp:
            return 0;
        case A_opExp:
            int a = maxargs(exp->u.op.left);
            int b = maxargs(exp->u.op.right);
            return b > a ? b : a;
        case A_eseqExp:
            int a = maxargs(exp->u.eseq.stm);
            int b = maxargs(exp->u.eseq.exp);
            return b > a ? b : a;
    }
}
int maxargs(A_expList list) {
    assert(list);
    switch(list->kind) {
        case A_pairExpList:
            int a = maxargs(list->u.pair.head);
            int b = maxargs(list->u.pair.last);
            return b > a ? b : a;
        case A_lastExpList:
            return maxargs(list->u.last);
    }
}
