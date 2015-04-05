/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */

#include <stdio.h>

#include "./slp.h"
#include "./util.h"
#include "./prog1.h"

int maxargs_exp(A_exp exp);
int maxargs_stm(A_stm stm);
int maxargs_explist(A_expList exps);

#define max(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char *argv[])
{
        printf("maxargs: %d\n", maxargs_stm(prog()));
        return 0;
}

int maxargs_exp(A_exp exp) {
        switch (exp->kind) {
        case A_idExp:
        case A_numExp:
                return 0;
        case A_opExp:
                return max(maxargs_exp(exp->u.op.left),
                           maxargs_exp(exp->u.op.right));
        case A_eseqExp:
                return max(maxargs_stm(exp->u.eseq.stm),
                           maxargs_exp(exp->u.eseq.exp));
        }
}

int current_args(A_expList exps) {
        switch (exps->kind) {
        case A_pairExpList:
                return 1 + current_args(exps->u.pair.tail);
        case A_lastExpList:
                return 1;
        }
}

int maxargs_explist(A_expList exps) {
        switch (exps->kind) {
        case A_pairExpList:
                return max(current_args(exps),
                           max(maxargs_exp(exps->u.pair.head),
                               maxargs_explist(exps->u.pair.tail)));
        case A_lastExpList:
                return max(1, maxargs_exp(exps->u.last));
        }
}

int maxargs_stm(A_stm stm) {
        switch (stm->kind) {
        case A_compoundStm:
                return max(maxargs_stm(stm->u.compound.stm1),
                           maxargs_stm(stm->u.compound.stm2));
        case A_assignStm:
                return maxargs_exp(stm->u.assign.exp);
        case A_printStm:
                return maxargs_explist(stm->u.print.exps);
        }
        return 0;
}

