/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */
#include "stdio.h"
#include <string.h>

#include "./slp.h"
#include "./prog1.h"
#include "./util.h"

typedef struct table *Table_;
struct table {
        string id;
        int value;
        Table_ tail;
};

struct IntAndTable {
        int i;
        Table_ t;
};

Table_ interStm(A_stm stm, Table_ table);

struct IntAndTable interpExp(A_exp exp, Table_ t);

int lookup(string key, Table_ t) {
        for (;t;) {
                if(strcmp(key, t->id) == 0)
                        return t->value;
                t = t->tail;
        }
        return -1;
}


struct IntAndTable IntAndTable_(int i, Table_ t) {
        struct IntAndTable it;
        it.i = i;
        it.t = t;
        return it;
}

struct IntAndTable calcOpExp(A_exp exp, Table_ t) {
        struct IntAndTable it1, it2;
        it1 = interpExp(exp->u.op.left, t);
        it2 = interpExp(exp->u.op.right, t);

        switch (exp->u.op.oper) {
        case A_plus:
                return IntAndTable_(it1.i + it2.i, t);
        case A_minus:
                return IntAndTable_(it1.i - it2.i, t);
        case A_times:
                return IntAndTable_(it1.i * it2.i, t);
        case A_div:
                return IntAndTable_(it1.i / it2.i, t);
        }
}

struct IntAndTable interpExp(A_exp exp, Table_ t) {
        switch (exp->kind) {
        case A_idExp:
                return IntAndTable_(lookup(exp->u.id, t), t);
        case A_numExp:
                return IntAndTable_(exp->u.num, t);
        case A_opExp:
                return calcOpExp(exp, t);
        case A_eseqExp:
                interStm(exp->u.eseq.stm, t);
                return interpExp(exp->u.eseq.exp, t);
        }
}

struct IntAndTable interpExpList(A_expList exps, Table_ t) {
        switch (exps->kind) {
        case A_pairExpList:
                interpExp(exps->u.pair.head, t);
                return interpExpList(exps->u.pair.tail, t);
        case A_lastExpList:
                return interpExp(exps->u.last, t);
        }
}

Table_ Table(string id, int value, Table_ tail) {
        Table_ t = checked_malloc(sizeof(*t));
        t->id = id;
        t->value = value;
        t->tail = tail;
        return t;
}


int main(int argc, char *argv[])
{
        Table_ tab = interStm(prog(), NULL);
        Table_ t;
        for (t = tab; t; t = t->tail) {
                printf("id: %s => value: %d\n", t->id, t->value);
        }
        return 0;
}

Table_ interStm(A_stm stm, Table_ table) {
        Table_ tab;
        struct IntAndTable int_tab;
        switch (stm->kind) {
        case A_compoundStm:
                return interStm(stm->u.compound.stm2,
                         interStm(stm->u.compound.stm1, table));
        case A_assignStm:
                tab = Table(stm->u.assign.id, 0, table);
                struct IntAndTable int_tab = interpExp(stm->u.assign.exp, tab);
                tab->value = int_tab.i;
                return tab;
        case A_printStm:
                int_tab = interpExpList(stm->u.print.exps, table);
                return int_tab.t;
        }
        return NULL;
}
