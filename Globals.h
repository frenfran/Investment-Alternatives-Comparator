#pragma once

// int options associated with cash flow types
const int UNIFORM = 1, GRADIENT = 2, GEOMETRIC = 3, INCONSISTENT = 4, NONEXISTANT= 5;

// int options associated with revenue or cost cash flows
const int REVENUES = 1, COSTS = 2;

// int options associated with cash flow variables
const int A_REVENUE = 1, A1_REVENUE = 2, G_REVENUE = 3, J_REVENUE = 4, A_COST = 5, A1_COST = 6, G_COST = 7, J_COST = 8;

// int options associated with methods of analysis
const int PRESENT_WORTH = 1, ANNUAL_WORTH = 2, FUTURE_WORTH = 3, INTERNAL_RATE_OF_RETURN = 4;

// int options associated with values to sort by
const int NET_VALUE = 1, FIRST_COST = 2;

// linked list node definition for inconsistent cash flow series
typedef struct nodeCashFlow {
    double amount;
    nodeCashFlow* next;
} nodeCashFlow;