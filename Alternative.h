#pragma once

#include <string>

#include "Globals.h"

class Alternative {
    private:
        // preliminary information present for all alternatives
        string name;
        int years;
        double firstCost;
        double depreciationRate;
        
        // variables to determine cash flow type for revenues/costs
        int revenueCashFlowType;
        int costCashFlowType;
        
        // variables required for each cash flow type for revenues/costs
        double ARevenue;
        double A1Revenue;
        double GRevenue;
        double jRevenue;
        double ACost;
        double A1Cost;
        double GCost;
        double jCost;
        
        // linked list for inconsistent cash flow series
        struct nodeCashFlow* cashFlowHeadRevenue;
        struct nodeCashFlow* cashFlowHeadCost;
        
        // variable to contain this alternative's PW, AW, FW or individual IRR
        double netValue;
    public:
        // Constructor
        Alternative ();
        
        // Mutators
        void setName (string alternativeName);
        void setYears (int economicLife);
        void setFirstCost (double initialCost);
        void setDepreciationRate (double rateOfDepreciation);
        
        void setUniform (int option, double annuity);
        void setGradient (int option, double base, double gradient);
        void setGeometric (int option, double base, double rateOfChange);
        void setInconsistent (int option, nodeCashFlow* headCashFlow);
        void setNonexistant (int option);
        
        void setNetValue (double value);
        
        // Accessors
        string getName ();
        int getYears ();
        double getFirstCost ();
        double getDepreciationRate ();
        
        int getRevenueCashFlowType ();
        int getCostCashFlowType ();
        
        double getCashFlowVariable (int option);
        nodeCashFlow* getInconsistent (int option);
        double getNetValue();
        
        // Print (debugging)
        void printAlternative ();
};