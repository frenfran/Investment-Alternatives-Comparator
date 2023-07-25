#include <iostream>

using namespace std;

#include "Globals.h"
#include "Alternative.h"

// Constructor
Alternative::Alternative() {
    name = "";
    years = 0;
    firstCost = 0;
    depreciationRate = 0;
    
    revenueCashFlowType = NONEXISTANT, costCashFlowType = NONEXISTANT;
    ARevenue = 0, A1Revenue = 0, GRevenue = 0, jRevenue = 0, ACost = 0, A1Cost = 0, GCost = 0, jCost = 0;
    
    cashFlowHeadRevenue = NULL, cashFlowHeadCost = NULL;
}

// Mutators
void Alternative::setName (string alternativeName) {
    name = alternativeName;
}

void Alternative::setYears (int economicLife) {
    years = economicLife;
}

void Alternative::setFirstCost (double initialCost) {
    firstCost = initialCost;
}

void Alternative::setDepreciationRate (double rateOfDepreciation) {
    depreciationRate = rateOfDepreciation;
}

void Alternative::setUniform (int option, double annuity) {
    if (option == 1) { // set uniform cash flow for revenue
        revenueCashFlowType = UNIFORM;
        ARevenue = annuity;
    }
    else { // set uniform cash flow for cost
        costCashFlowType = UNIFORM;
        ACost = annuity;
    }
}

void Alternative::setGradient (int option, double base, double gradientValue) {
    if (option == 1) { // set gradient cash flow for revenue
        revenueCashFlowType = GRADIENT;
        A1Revenue = base;
        GRevenue = gradientValue;
    }
    else { // set gradient cash flow for cost
        costCashFlowType = GRADIENT;
        A1Cost = base;
        GCost = gradientValue;
    }
}

void Alternative::setGeometric (int option, double base, double rateOfChange) {
    if (option == 1) { // set geometric cash flow for revenue
        revenueCashFlowType = GEOMETRIC;
        A1Revenue = base;
        jRevenue = rateOfChange;
    }
    else { // set geometric cash flow for cost
        costCashFlowType = GEOMETRIC;
        A1Cost = base;
        jCost = rateOfChange;
    }
}

void Alternative::setInconsistent (int option, nodeCashFlow* headCashFlow) {
    if (option == 1) { // set inconsistent cash flow for revenue
        revenueCashFlowType = INCONSISTENT;
        cashFlowHeadRevenue = headCashFlow;
    }
    else { // set inconsistent cash flow for cost
        costCashFlowType = INCONSISTENT;
        cashFlowHeadCost = headCashFlow;
    }
}

void Alternative::setNonexistant(int option) {
    if (option == 1) // set nonexistant cash flow for revenue
        revenueCashFlowType = NONEXISTANT;
    else // set nonexistant cash flow for cost
        costCashFlowType = NONEXISTANT;
}

void Alternative::setNetValue (double value) {
    netValue = value;
}

// Accessors
string Alternative::getName () {
    return name;
}

int Alternative::getYears () {
    return years;
}

double Alternative::getFirstCost () {
    return firstCost;
}

double Alternative::getDepreciationRate () {
    return depreciationRate;
}

int Alternative::getRevenueCashFlowType () {
    return revenueCashFlowType;
}

int Alternative::getCostCashFlowType () {
    return costCashFlowType;
}

double Alternative::getCashFlowVariable (int option) {
    switch(option) {
        case A_REVENUE: // A for revenue
        return ARevenue;
        
        case A1_REVENUE: // A1 for revenue
        return A1Revenue;
        
        case G_REVENUE: // G for revenue
        return GRevenue;
        
        case J_REVENUE: // j for revenue
        return jRevenue;
        
        case A_COST: // A for cost
        return ACost;
        
        case A1_COST: // A1 for cost
        return A1Cost;
        
        case G_COST: // G for cost
        return GCost;
        
        case J_COST: // j for cost
        return jCost;
        
        default: // an error occurred
        return 0;
    }
}

nodeCashFlow* Alternative::getInconsistent(int option) {
    if (option == REVENUES) // get inconsistent cash flow for revenue
        return cashFlowHeadRevenue;
    else // get inconsistent cash flow for revenue
        return cashFlowHeadCost;
}

double Alternative::getNetValue() {
    return netValue;
}

void Alternative::printAlternative () {
    cout << endl << "BEGIN-------------------------------------------" << endl;
    
    cout << "Name: " << name << endl;
    cout << "Economic life: " << years << endl;
    cout << "First Cost: " << firstCost << endl;
    cout << "Depreciation Rate: " << depreciationRate << endl;
    
    cout << "Revenue Cash Flow Type: " << revenueCashFlowType << endl;
    cout << "Cost Cash Flow Type: " << costCashFlowType << endl;
    
    cout << "A for revenue: " << ARevenue << endl;
    cout << "A1 for revenue: " << A1Revenue << endl;
    cout << "G for revenue: " << GRevenue << endl;
    cout << "j for revenue: " <<  jRevenue << endl;
    cout << "A for cost: " << ACost << endl;
    cout << "A1 for cost: " << A1Cost << endl;
    cout << "G for cost: " << GCost << endl;
    cout << "j for cost: " << jCost << endl;
    
    cout << "cash flow series (if inconsistent) for revenue: ";
    nodeCashFlow* helper = cashFlowHeadRevenue;
    while (helper != NULL) {cout << helper->amount << " "; helper = helper->next;}
    cout << endl << "cash flow series (if inconsistent) for cost: ";
    helper = cashFlowHeadCost;
    while (helper != NULL) {cout << helper->amount << " "; helper = helper->next;}
    cout << endl;
    
    cout << "END---------------------------------------------" << endl;
}
