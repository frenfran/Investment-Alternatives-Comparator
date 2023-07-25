#include <cmath>

double P_given_F (double F, double i, int n) {
    return F * (pow(1 + i, -n));
}

double P_given_A (double A, double i, int n) {
    return A * ((pow(1 + i, n) - 1) / (i * pow(1 + i, n)));
}

double P_given_G (double G, double i, int n) {
    return G * ((1 - (1 + n * i) * pow(1 + i, -n)) / pow(i, 2));
}

double P_given_A1 (double A1, double i, double j, int n) {
    if (i == j)
        return A1 * n / (1 + i);
    else
        return A1 * ((1 - pow(1 + j, n) * pow(1 + i, -n)) / (i - j));
}

double A_given_P (double P, double i, int n) {
    return P * ((i * pow(1 + i, n)) / (pow(1 + i, n) - 1));
}

double A_given_F (double F, double i, int n) {
    return F * (i / (pow(1 + i, n) - 1));
}

double A_given_G (double G, double i, int n) {
    return G * ((pow(1 + i, n) - (1 + n * i)) / (i * (pow(1 + i, n) - 1)));
}

double F_given_P (double P, double i, int n) {
    return P * (pow(1 + i, n));
}

double F_given_A (double A, double i, int n) {
    return A * ((pow(1 + i, n) - 1) / i);
}

double F_given_A1 (double A1, double i, double j, int n) {
    if (i == j)
        return A1 * n / (1 + i) * pow(1 + i, n);
    else
        return A1 * ((pow(1 + i, n) - pow(1 + j, n)) / (i - j));
}