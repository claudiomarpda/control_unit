//
// Created by mz on 07/09/17.
//

#include "../include/arithmetic_logic_unit.h"

int add(int op1, int op2) {
    return op1 + op2;
}

int subtract(int op1, int op2) {
    return op1 - op2;
}

int multiply(int op1, int op2) {
    return op1 * op2;
}

int divide(int op1, int op2) {
    return op1 / op2;
}

/**
 * Compares two values. The possible results are:
 * 1 for op1 greater than op2
 * -1 for op1 lower than op2
 * 0 for equal value
 *
 * @return an integer value according to comparison
 */
int compare(int op1, int op2) {

    // Operand 1 is greater
    if (op1 > op2) {
        return 1;
    }
        // Operand 1 is lower
    else if (op1 < op2) {
        return -1;
    }
    // Operands have equal value
    return 0;
}