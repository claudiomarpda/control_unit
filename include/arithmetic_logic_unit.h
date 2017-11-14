//
// Created by mz on 07/09/17.
//

#ifndef CONTROLUNIT_ARITHMETIC_LOGIC_UNIT_H
#define CONTROLUNIT_ARITHMETIC_LOGIC_UNIT_H

int add(int op1, int op2);

int subtract(int op1, int op2);

int multiply(int op1, int op2);

int divide(int op1, int op2);

/**
 * Compares two values. The possible results are:
 * 1 for op1 greater than op2
 * -1 for op1 lower than op2
 * 0 for equal value
 *
 * @return an integer value according to comparison
 */
int compare(int op1, int op2);

#endif //CONTROLUNIT_ARITHMETIC_LOGIC_UNIT_H
