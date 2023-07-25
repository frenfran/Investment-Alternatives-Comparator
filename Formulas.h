#pragma once

// present value given future value
double P_given_F (double futureValue, double costOfCapital, int numberOfYears);

// present value given annuity
double P_given_A (double A, double costOfCapital, int numberOfYears);

// present value given gradient
double P_given_G (double G, double costOfCapital, int numberOfYears);

// present value given initial cash flow & rate of change
double P_given_A1 (double A1, double costOfCapital, double rateOfChange, int numberOfYears);

// annuity given present value
double A_given_P (double presentValue, double costOfCapital, int numberOfYears);

// annuity given future value
double A_given_F (double futureValue, double costOfCapital, int numberOfYears);

// annuity given gradient
double A_given_G (double G, double costOfCapital, int numberOfYears);

// future value given present value
double F_given_P (double presentValue, double costOfCapital, int numberOfYears);

// future value given annuity
double F_given_A (double A, double costOfCapital, int numberOfYears);

// future value given initial cash flow & rate of change
double F_given_A1 (double A1, double costOfCapital, double rateOfChange, int numberOfYears);