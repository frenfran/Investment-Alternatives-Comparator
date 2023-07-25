#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cmath>

using namespace std;

#include "Functions.h"
#include "Globals.h"
#include "Alternative.h"
#include "Formulas.h"

bool includeDoNothing () {
    cout << "Is doing nothing a viable alternative? ";
    return getYesOrNo ();
}

int setNumOfAlternatives () {
    cout << "Enter the total number of mutually exclusive alternatives (excluding the 'do-nothing' approach): ";
    return getInteger();
}

bool getYesOrNo() {
    while (true) {
        string yesNo;
        stringstream lineStream = getStringStream();
        lineStream >> yesNo;
        
        for (int i = 0; i < (int) yesNo.length(); i++) // turn input into lower case
            yesNo[i] = tolower (yesNo[i]);
        
        if (lineStream.fail())
            cout << "Error, please try again: ";
        else if (yesNo == "y" || yesNo == "yes")
            return true;
        else if (yesNo == "n" || yesNo == "no")
            return false;
        else
            cout << "Error, please try again: ";
    }
}

int getInteger() {
    while (true) {
        int value;
        cin >> value;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error, please try again: ";
        }
        else if (value <= 0)
            cout << "Invalid input, please try again: ";
        else 
            return value;
    }
}

stringstream getStringStream() {
    string input;
    
    cin.sync();
    getline (cin, input);
    stringstream lineStream (input);
    
    return lineStream;
}

void populateProjectInfo (nodeProject* projectToCreate, nodeProject* firstProject) {    
    // accepting preliminary info
    getPreliminaryProjectInfo (projectToCreate, firstProject);
    cout << endl;
    
    // accepting info regarding cash flow
    int cashFlowType = obtainCashFlowType(REVENUES);
    getCashFlowInfo (REVENUES, cashFlowType, projectToCreate->alternative);
    cout << endl;
    cashFlowType = obtainCashFlowType(COSTS);
    getCashFlowInfo (COSTS, cashFlowType, projectToCreate->alternative);
}

void getPreliminaryProjectInfo (nodeProject* projectToCreate, nodeProject* firstProject) {
    bool userDone = false; // boolean variable to track whether a project's preliminary info has all been accepted
    bool inputs[4]; // boolean array to track which inputs the user still has not entered
    for (int i = 0; i < 4; i ++)
        inputs[i] = false;
        
    while (!userDone) {
        printProjectPreliminaryInfo(projectToCreate, inputs);  // print project preliminary info table
        
        string line;
        int rowNum;
        string rowValue;
        bool inputObtained = false;
        
        cout << "Please enter your input: ";
        while (inputObtained == false) {
            stringstream lineStream = getStringStream();
            lineStream >> rowNum >> rowValue;
            
            if (lineStream.fail())
                cout << "Error, please try again: ";
            else if (rowNum < 1 || rowNum > 4)
                cout << "Row does not exist, please try again: ";
            else { // user input is valid
                if (rowNum == 1) { // user is inputting the name
                    if (!isNameInUse(rowValue, projectToCreate, firstProject)) { // error checking if name is already in use
                        (projectToCreate->alternative).setName(rowValue);
                        inputObtained = true;
                    }
                }
                else { // user is inputting either economic life, first cost or depreciation rate
                    double value = convertStringToDouble(rowValue, inputObtained);
                
                    if (inputObtained) {
                        if (rowNum == 2) // user is setting economic life
                            (projectToCreate->alternative).setYears(value);
                        else if (rowNum == 3) // user is setting first cost
                            (projectToCreate->alternative).setFirstCost(value);
                        else // user is setting depreciation rate
                            (projectToCreate->alternative).setDepreciationRate(value);
                    }
                }
            }
            
            if (inputObtained)
                inputs[rowNum - 1] = true; // set array element associated with user's input to true 
        }
        
        // check if user has modified all rows at least once
        userDone = true;
        for (int i = 0; i < 4; i ++)
            if (inputs[i] == false)
                userDone = false;
    }
    
    printProjectPreliminaryInfo(projectToCreate, inputs); // final print (once all data have been inputted)
}

bool isNameInUse (string rowValue, nodeProject* projectBeingDesigned, nodeProject* firstProject) {
    bool nameInUse = false;
    nodeProject* helper = firstProject;
    while (helper != NULL) {
        if (helper != projectBeingDesigned) {
            if (!(helper->alternative).getName().compare(rowValue)) {
                nameInUse = true;
                cout << "Name already in use, please try again: ";
                break;
            }
        }
            
        helper = helper->next;
    }
    
    return nameInUse;
}

double convertStringToDouble (string toConvert, bool &valid) {
    char* input = new char[toConvert.length() + 1]; // input as a C string
    double valueAsDouble; // input as a double
    char* endptr;
    
    strcpy(input, toConvert.c_str()); // convert input to C string
    valueAsDouble = strtod(input, &endptr); // convert C string to double
    if (endptr == input || *endptr != '\0')
        cout << "Error, please try again: ";
    else if (valueAsDouble < 0)
        cout << "Invalid input, please try again: ";
    else
        valid = true;
        
    return valueAsDouble;
}

void printProjectPreliminaryInfo(nodeProject* project, const bool* inputs) {
    int maxColSize = 0, maxSizeRow = 0, nameSize = 0, lifeSize = 0, firstCostSize = 0, depreciationSize = 0;
    
    if (inputs[0] == true) nameSize = ((project->alternative).getName()).length();
    if (inputs[1] == true) lifeSize = (to_string((project->alternative).getYears())).length();
    if (inputs[2] == true) firstCostSize = (to_string((project->alternative).getFirstCost())).length();
    if (inputs[3] == true) depreciationSize = (to_string((project->alternative).getDepreciationRate())).length();
    
    maxColSize = nameSize;
    if (lifeSize > maxColSize) {
        maxColSize = lifeSize;
        maxSizeRow = 1;
    }
    if (firstCostSize > maxColSize) {
        maxColSize = firstCostSize;
        maxSizeRow = 2;
    }
    if (depreciationSize > maxColSize) {
        maxColSize = depreciationSize;
        maxSizeRow = 3;
    }
    
    for (int i = 0; i < 24 + maxColSize; i ++) cout << "-";
    cout << endl << "|        Name       | ";
    printTableElement (inputs[0], maxColSize, 0, maxSizeRow, (project->alternative).getName());
    cout << " | (1)" << endl;
    
    cout << "|   Economic Life   | ";
    printTableElement (inputs[1], maxColSize, 1, maxSizeRow, to_string((project->alternative).getYears()));
    cout << " | (2)" << endl;

    cout << "|     First Cost    | ";
    printTableElement (inputs[2], maxColSize, 2, maxSizeRow, to_string((project->alternative).getFirstCost()));
    cout << " | (3)" << endl;

    cout << "| Depreciation Rate | "; 
    printTableElement (inputs[3], maxColSize, 3, maxSizeRow, to_string((project->alternative).getDepreciationRate()));
    cout << " | (4)" << endl;
    for (int i = 0; i < 24 + maxColSize; i ++) cout << "-";
    cout << endl;
}

void printTableElement (bool elementExists, int maxColSize, int row, int maxSizeRow, string cellElement) {
    int numSpaces = maxColSize; // numSpaces will track how many spaces should be printed
    if (elementExists )
        numSpaces -= (cellElement).length();
        
    for (int i = 0; i < numSpaces / 2; i ++) cout << " ";
    if (elementExists == true) cout << cellElement;
    for (int i = 0; i < numSpaces / 2; i ++) cout << " ";
    if (numSpaces % 2 != 0) cout << " "; // extra space for elements with an odd number of characters
}

int obtainCashFlowType (int option) {
    cout << "Which of the below series of cash flows best models the annual ";
    if (option == REVENUES) cout << "revenues";
    else cout << "costs";
    cout << " of the project?" << endl;
    cout << "----------------------" << endl;
    cout << "| (1) -    Uniform   |" << endl;
    cout << "| (2) -   Gradient   |" << endl;
    cout << "| (3) -   Geometric  |" << endl;
    cout << "| (4) - Inconsistent |" << endl;
    cout << "| (5) -  Nonexistant |" << endl;
    cout << "----------------------" << endl;
    
    int cashFlowType;
    cout << "Please enter your input: ";
    while (true) {
        cashFlowType = getInteger();
        
        if (cashFlowType < 1 || cashFlowType > 5)
            cout << "Invalid input, please try again: ";
        else break;
    }
    
    return cashFlowType;
}

void getCashFlowInfo (int option, int cashFlowType, Alternative &alternative) {
    if (cashFlowType == UNIFORM) {
        cout << "Enter the value of the annuity: ";
        double annuity = getDouble();
        
        alternative.setUniform(option, annuity);
    }
    else if (cashFlowType == GRADIENT) {
        cout << "Enter the base amount (at EOY1): ";
        double baseAmount = getDouble();
        cout << "Enter the gradient value: ";
        double gradient = getDouble();
        
        alternative.setGradient(option, baseAmount, gradient);
    }
    else if (cashFlowType == GEOMETRIC) {
        cout << "Enter the initial cash flow amount (at EOY1): ";
        double baseAmount = getDouble();
        cout << "Enter the rate of change: ";
        double rate = getDouble();
        
        alternative.setGeometric(option, baseAmount, rate);
    }
    else if (cashFlowType == INCONSISTENT) { // special case for inconsistent cash flow
        nodeCashFlow* headCashFlow = NULL;
        createInconsistentCashFlowSeries(alternative, headCashFlow);
        alternative.setInconsistent(option, headCashFlow);
    }
    else // nonexistant cash flow
        alternative.setNonexistant(option);
}

void createInconsistentCashFlowSeries (Alternative alternative, nodeCashFlow* &head) {
    for (int i = 1; i <= alternative.getYears(); i ++) {
        nodeCashFlow* temp = new nodeCashFlow;

        cout << "Cash Flow for EOY " << i << ": ";
        temp->amount = getDouble();
        temp->next = NULL;
        
        if (head == NULL)
            head = temp;
        else {
            nodeCashFlow* helper = head;
            while (helper->next != NULL)
                helper = helper->next;
                
            helper->next = temp;
        }
    }
}

double getDouble () {
    string input;
    bool userDone = false;
    
    while (!userDone) {
        stringstream lineStream = getStringStream();
        lineStream >> input;
        
        if (lineStream.fail())
            cout << "Error, please try again: ";
        else {
            if (!validateInputAsDouble (input))
                cout << "Invalid input, please try again: ";
            else
                userDone = true;
        }
    }
    
    return convertStringToDouble(input, userDone); // userDone is used as a dummy variable here
}

bool validateInputAsDouble (string input) {
    char* inputAsCString = new char[input.length() + 1]; 
    strcpy(inputAsCString, input.c_str()); // convert input to a C string

    bool inputValid = true;
    bool decimalFound = false;
    for (int i = 0; i < (int) input.length(); i ++) {
        if ((inputAsCString[i] < 48 || inputAsCString[i] > 57) && inputAsCString[i] != '.')
            inputValid = false;
        if (inputAsCString[i] == '.') {
            if (decimalFound == true)
                inputValid = false;
            else
                decimalFound = true;
        }
    }
    
    if (decimalFound && input.length() == 1)
        inputValid = false;
    
    return inputValid;
}

void verifyPreliminaryInfo (nodeProject* nodeBeingChecked, nodeProject* firstNode) {
    bool dummy[4];
    for (int i = 0; i < 4; i ++) dummy[i] = true;
    bool userDone = false;
    
    while (!userDone) {
        printProjectPreliminaryInfo(nodeBeingChecked, dummy);
        printProjectCashFlowType(nodeBeingChecked);
        
        cout << "Is the above correct (y/n)? ";
        userDone = getYesOrNo();
        
        if (!userDone) {
            cout << "Enter the number of the row you would like to modify: ";
            int rowNum = getInteger();
            while (rowNum > 6) {
                cout << "Row does not exist, please try again: ";
                rowNum = getInteger();
            }
            
            if (rowNum < 5)
                cout << "Enter the correct input for this row: ";
            if (rowNum == 1) { // user wants to modify project name
                string name;
                bool nameValid = false;
                while (!nameValid) {
                    stringstream lineStream = getStringStream();
                    lineStream >> name;
                    
                    if (lineStream.fail())
                        cout << "Error: please try again: ";
                    else if (!isNameInUse(name, nodeBeingChecked, firstNode))
                        nameValid = true;
                }
                (nodeBeingChecked->alternative).setName(name);
            }
            else if (rowNum < 5) { // user wants to modify either economic life, first cost or depreciation rate
                double value = getDouble();
                if (rowNum == 2) {
                    (nodeBeingChecked->alternative).setYears(value);
                    
                    if ((nodeBeingChecked->alternative).getRevenueCashFlowType() == INCONSISTENT) {
                        nodeCashFlow* headCashFlow = NULL;
                        
                        cout << endl << "Please re-create the cash flow series for revenues: " << endl;
                        destroyCashFlowLinkedList((nodeBeingChecked->alternative).getInconsistent(REVENUES));
                        createInconsistentCashFlowSeries((nodeBeingChecked->alternative), headCashFlow);
                        (nodeBeingChecked->alternative).setInconsistent(REVENUES, headCashFlow);
                    }
                    if ((nodeBeingChecked->alternative).getCostCashFlowType() == INCONSISTENT) {
                        nodeCashFlow* headCashFlow = NULL;
                        
                        cout << endl << "Please re-create the cash flow series for costs: " << endl;
                        destroyCashFlowLinkedList((nodeBeingChecked->alternative).getInconsistent(COSTS));
                        createInconsistentCashFlowSeries((nodeBeingChecked->alternative), headCashFlow); 
                        (nodeBeingChecked->alternative).setInconsistent(COSTS, headCashFlow);
                    }
                }
                else if (rowNum == 3) (nodeBeingChecked->alternative).setFirstCost(value);
                else if (rowNum == 4) (nodeBeingChecked->alternative).setDepreciationRate(value);
            }
            else { // user wants to modify the cash flow type for either revenues or costs
                int cashFlowType = obtainCashFlowType(rowNum - 4);
                getCashFlowInfo(rowNum - 4, cashFlowType, (nodeBeingChecked->alternative));
            }
        }
    }
}

void destroyCashFlowLinkedList (nodeCashFlow* head) {
    while (head != NULL) {
        nodeCashFlow* helper = head;
        head = head->next;
        delete helper;
    }
}

void printProjectCashFlowType(nodeProject* project) {
    int maxColSize = 0;
    int maxSizeRow = 0;
    
    // determining largest cell element
    if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT || (project->alternative).getCostCashFlowType() == INCONSISTENT) {
        maxColSize = strlen("Inconsistent");
        if ((project->alternative).getCostCashFlowType() == INCONSISTENT) 
            maxSizeRow = 1;
    }
    else if ((project->alternative).getRevenueCashFlowType() == NONEXISTANT || (project->alternative).getCostCashFlowType() == NONEXISTANT) {
        maxColSize = strlen("Nonexistant");
        if ((project->alternative).getCostCashFlowType() == NONEXISTANT)
            maxSizeRow = 1;
    }
    else if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC || (project->alternative).getCostCashFlowType() == GEOMETRIC) {
        maxColSize = strlen("Geometric");
        if ((project->alternative).getCostCashFlowType() == GEOMETRIC)
            maxSizeRow = 1;
    }
    else if ((project->alternative).getRevenueCashFlowType() == GRADIENT || (project->alternative).getCostCashFlowType() == GRADIENT) {
        maxColSize = strlen("Gradient");
        if ((project->alternative).getCostCashFlowType() == GRADIENT)
            maxSizeRow =1;
    }
    else {
        maxColSize = strlen("Uniform");
        if ((project->alternative).getCostCashFlowType() == UNIFORM)
            maxSizeRow = 1;
    }
    
    // printing table
    cout << endl;
    for (int i = 0; i < maxColSize + 34; i ++) cout << "-";
    cout << endl << "| Cash Flow Type for Revenues | ";
    printTableElement(true, maxColSize, 0, maxSizeRow, setCashFlowType((project->alternative).getRevenueCashFlowType()));
    cout << " | (5)" << endl << "|   Cash Flow Type for Costs  | ";
    printTableElement(true, maxColSize, 1, maxSizeRow, setCashFlowType((project->alternative).getCostCashFlowType()));
    cout << " | (6)" << endl;
    for (int i = 0; i < maxColSize + 34; i ++) cout << "-";
    cout << endl << endl;
}

string setCashFlowType(int cashFlowTypeAsInt) {
    string cashFlowType;
    
    switch(cashFlowTypeAsInt) {
        case UNIFORM:
            cashFlowType = "Uniform";
            break;
        
        case GRADIENT:
            cashFlowType = "Gradient";
            break;
        
        case GEOMETRIC:
            cashFlowType = "Geometric";
            break;
        
        case INCONSISTENT:
            cashFlowType = "Inconsistent";
            break;
        
        case NONEXISTANT:
            cashFlowType = "Nonexistant";
            break;
    }
    
    return cashFlowType;
}

void verifyCashFlowInfo (nodeProject* nodeBeingChecked) {
    if ((nodeBeingChecked->alternative).getRevenueCashFlowType() == NONEXISTANT && (nodeBeingChecked->alternative).getCostCashFlowType() == NONEXISTANT)
        return; // nothing to check
    
    bool userDone = false;
    while (!userDone) {
        int numOptions = printProjectCashFlowInfo(nodeBeingChecked);
        cout << "Is the above correct (y/n)? ";
        userDone = getYesOrNo();
        
        if (!userDone) {
            cout << "Enter the number of the row you would like to modify: ";
            int selectedOption = getInteger();
            while (selectedOption > numOptions) {
                cout << "Row does not exist, please try again: ";
                selectedOption = getInteger();
            }
            
            cout << "Enter the correct input for this row: ";
            double newValue = getDouble();
            
            // find row associated with the newly inputted value and set it to its new value
            setNewCashFlowData ((nodeBeingChecked->alternative), selectedOption, newValue);
        }
    }
}

void setNewCashFlowData (Alternative &alternative, int option, double value) {
    if (alternative.getRevenueCashFlowType() == UNIFORM) {
        if (option == 1)
            alternative.setUniform(REVENUES, value);
        else {
            option -= 1;
            setCashFlowDataForCosts (alternative, option, value);
        }
    }
    else if (alternative.getRevenueCashFlowType() == GRADIENT) {
        if (option == 1)
            alternative.setGradient(REVENUES, value, alternative.getCashFlowVariable(G_REVENUE));
        else if (option == 2)
            alternative.setGradient(REVENUES, alternative.getCashFlowVariable(A1_REVENUE), value);
        else {
            option -= 2;
            setCashFlowDataForCosts(alternative, option, value);
        }
    }
    else if (alternative.getRevenueCashFlowType() == GEOMETRIC) {
        if (option == 1)
            alternative.setGeometric(REVENUES, value, alternative.getCashFlowVariable(J_REVENUE));
        else if (option == 2)
            alternative.setGeometric(REVENUES, alternative.getCashFlowVariable(A1_REVENUE), value);
        else {
            option -= 2;
            setCashFlowDataForCosts(alternative, option, value);
        }
    }
    else if (alternative.getRevenueCashFlowType() == INCONSISTENT) {
        bool optionInRevenue = false;
        nodeCashFlow* helper = alternative.getInconsistent(REVENUES);
        int rowCounter = 1;
        
        while (helper != NULL) {
            if (option == rowCounter) {
                helper->amount = value;
                optionInRevenue = true;
            }
            
            rowCounter ++;
            helper = helper->next;
        }
        
        if (!optionInRevenue) {
            option -= (rowCounter - 1); // extra 1 to remove last increment on rowCounter from while loop
            setCashFlowDataForCosts(alternative, option, value);
        }
    }
    else if (alternative.getRevenueCashFlowType() == NONEXISTANT)
        setCashFlowDataForCosts(alternative, option, value);
}

void setCashFlowDataForCosts (Alternative &alternative, int option, double value) {
    if (alternative.getCostCashFlowType() == UNIFORM) {
        if (option == 1)
            alternative.setUniform(COSTS, value);
    }
    else if (alternative.getCostCashFlowType() == GRADIENT) {
        if (option == 1)
            alternative.setGradient(COSTS, value, alternative.getCashFlowVariable(G_COST));
        else if (option == 2)
            alternative.setGradient(COSTS, alternative.getCashFlowVariable(A1_COST), value);
    }
    else if (alternative.getCostCashFlowType() == GEOMETRIC) {
        if (option == 1)
            alternative.setGeometric(COSTS, value, alternative.getCashFlowVariable(J_COST));
        else if (option == 2)
            alternative.setGeometric(COSTS, alternative.getCashFlowVariable(A1_COST), value);
    }
    else if (alternative.getCostCashFlowType() == INCONSISTENT) {
        nodeCashFlow* helper = alternative.getInconsistent(COSTS);
        int rowCounter = 1;
        
        while (helper != NULL) {
            if (option == rowCounter)
                helper->amount = value;
            
            rowCounter ++;
            helper = helper->next;
        }
    }
}

int printProjectCashFlowInfo (nodeProject* project) {
    // helper variables for finding largest cell elements
    int maxSizeInFirstCol = 0;
    int maxSizeInSecondCol = 0;
    int maxSizeRowInFirstCol = 0;
    int maxSizeRowInSecondCol = 0;
    int maxNumInSecondCol = 0;

    // determining info regarding largest cell element in left half of table
    if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC || (project->alternative).getCostCashFlowType() == GEOMETRIC) {
        maxSizeInFirstCol = strlen("Initial Cash Flow");
        if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC || (project->alternative).getRevenueCashFlowType() == NONEXISTANT)
            maxSizeRowInFirstCol = 1;
        else
            addRows(1, project, maxSizeRowInFirstCol);
    }
    else if ((project->alternative).getRevenueCashFlowType() == GRADIENT || (project->alternative).getCostCashFlowType() == GRADIENT) {
        maxSizeInFirstCol = strlen("Gradient Value");
        if ((project->alternative).getRevenueCashFlowType() == GRADIENT || (project->alternative).getRevenueCashFlowType() == NONEXISTANT)
            maxSizeRowInFirstCol = 2;
        else
            addRows (1, project, maxSizeRowInFirstCol);
    }
    else if ((project->alternative).getRevenueCashFlowType() == UNIFORM  || (project->alternative).getCostCashFlowType() == UNIFORM) {
        maxSizeInFirstCol = strlen("Annuity");
        if ((project->alternative).getRevenueCashFlowType() == UNIFORM || (project->alternative).getRevenueCashFlowType() == NONEXISTANT)
            maxSizeRowInFirstCol = 1;
        else {
            if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT)
                maxSizeRowInFirstCol = (project->alternative).getYears() + 3; // + 1 for "Costs" header row
            else if ((project->alternative).getRevenueCashFlowType() == GRADIENT || (project->alternative).getRevenueCashFlowType() == GEOMETRIC)
                maxSizeRowInFirstCol = 4;
        }
    }
    else {
        maxSizeInFirstCol = strlen("EOY");
        
        if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT)
            maxSizeRowInFirstCol = 1;
        else { // guarantee that cash flow type for revenues is nonexistant
            nodeCashFlow* helper = (project->alternative).getInconsistent(COSTS);
            int counter = 2;
            while (helper != NULL) {
                helper = helper->next;
                counter ++;
            }
            maxSizeRowInFirstCol = counter + 2;
        }
    }
    
    // determining info regarding largest cell element in right half of table
    if ((project->alternative).getCashFlowVariable(A_REVENUE) > maxNumInSecondCol) setHelperSizeVariables(project, A_REVENUE, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
    if ((project->alternative).getCashFlowVariable(A1_REVENUE) > maxNumInSecondCol) setHelperSizeVariables(project, A1_REVENUE, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
    if ((project->alternative).getCashFlowVariable(G_REVENUE) > maxNumInSecondCol) setHelperSizeVariables(project, G_REVENUE, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
    if ((project->alternative).getCashFlowVariable(J_REVENUE) > maxNumInSecondCol) setHelperSizeVariables(project, J_REVENUE, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
    if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT) {
        nodeCashFlow* helper = (project->alternative).getInconsistent(REVENUES);
        for (int i = 1; i <= (project->alternative).getYears(); i ++) {
            if (helper->amount > maxNumInSecondCol) {
                maxNumInSecondCol = helper->amount;
                maxSizeRowInSecondCol = i + 1;
                maxSizeInSecondCol = to_string(helper->amount).length();
            }
            
            helper = helper->next;
        }
    }
    if ((project->alternative).getCashFlowVariable(A_COST) > maxNumInSecondCol) {
        setHelperSizeVariables(project, A_COST, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
        addRows (2, project, maxSizeRowInSecondCol);
    }
    if ((project->alternative).getCashFlowVariable(A1_COST) > maxNumInSecondCol) {
        setHelperSizeVariables(project, A1_COST, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
        addRows (2, project, maxSizeRowInSecondCol);
    }
    if ((project->alternative).getCashFlowVariable(G_COST) > maxNumInSecondCol) {
        setHelperSizeVariables(project, G_COST, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
        addRows (2, project, maxSizeRowInSecondCol);
    }
    if ((project->alternative).getCashFlowVariable(J_COST) > maxNumInSecondCol) {
        setHelperSizeVariables(project, J_COST, maxNumInSecondCol, maxSizeInSecondCol, maxSizeRowInSecondCol);
        addRows (2, project, maxSizeRowInSecondCol);
    }
    if ((project->alternative).getCostCashFlowType() == INCONSISTENT) {
        nodeCashFlow* helper = (project->alternative).getInconsistent(COSTS);
        for (int i = 1; i <= (project->alternative).getYears(); i ++) {
            if (helper->amount > maxNumInSecondCol) {
                maxNumInSecondCol = helper->amount;
                maxSizeRowInSecondCol = i + 1;
                maxSizeInSecondCol = to_string(helper->amount).length();
                
                addRows(3, project, maxSizeRowInSecondCol);
            }
            
            helper = helper->next;
        }
    }
    
    // printing the table
    int rowCounter = 0;
    int optionCounter = 1;
    
    // print Revenues
    if ((project->alternative).getRevenueCashFlowType() != NONEXISTANT) {
        cout << endl;
        printHeader(REVENUES, maxSizeInFirstCol, maxSizeInSecondCol);
        rowCounter ++;
        
        if ((project->alternative).getRevenueCashFlowType() == UNIFORM)
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Annuity", to_string((project->alternative).getCashFlowVariable(A_REVENUE)));
        else if ((project->alternative).getRevenueCashFlowType() == GRADIENT) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Base Amount", to_string((project->alternative).getCashFlowVariable(A1_REVENUE)));
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Gradient Value", to_string((project->alternative).getCashFlowVariable(G_REVENUE)));
        }
        else if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Initial Cash Flow", to_string((project->alternative).getCashFlowVariable(A1_REVENUE)));
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Rate of Change", to_string((project->alternative).getCashFlowVariable(J_REVENUE)));
        }
        else if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, false, "EOY", "$");
            
            nodeCashFlow* helper = (project->alternative).getInconsistent(REVENUES);
            for (int i = 1; i <= (project->alternative).getYears(); i ++) {
                printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, to_string(i), to_string(helper->amount));
                helper = helper->next;
            }
        }
    }
    
    // print Costs
    if ((project->alternative).getCostCashFlowType() != NONEXISTANT) {
        printHeader(COSTS, maxSizeInFirstCol, maxSizeInSecondCol);
        rowCounter ++;
        
        if ((project->alternative).getCostCashFlowType() == UNIFORM)
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Annuity", to_string((project->alternative).getCashFlowVariable(A_COST)));
        else if ((project->alternative).getCostCashFlowType() == GRADIENT) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Base Amount", to_string((project->alternative).getCashFlowVariable(A1_COST)));
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Gradient Value", to_string((project->alternative).getCashFlowVariable(G_COST)));
        }
        else if ((project->alternative).getCostCashFlowType() == GEOMETRIC) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Initial Cash Flow", to_string((project->alternative).getCashFlowVariable(A1_COST)));
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, "Rate of Change", to_string((project->alternative).getCashFlowVariable(J_COST)));
        }
        else if ((project->alternative).getCostCashFlowType() == INCONSISTENT) {
            printRowForDualColumnTable(maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, false, "EOY", "$");
            
            nodeCashFlow* helper = (project->alternative).getInconsistent(COSTS);
            for (int i = 1; i <= (project->alternative).getYears(); i ++) {
                printRowForDualColumnTable (maxSizeInFirstCol, maxSizeInSecondCol, rowCounter, optionCounter, maxSizeRowInFirstCol, maxSizeRowInSecondCol, true, to_string(i), to_string(helper->amount));
                helper = helper->next;
            }
        }
    }
    
    for (int i = 0; i < maxSizeInFirstCol + maxSizeInSecondCol + 7; i ++)
        cout << "-";
    cout << endl << endl;
    
    return optionCounter - 1; // return total number of rows in table
}

void setHelperSizeVariables (nodeProject* project, int cashFlowVariable, int &maxNum, int &maxSize, int &maxSizeRow) {
    maxNum = (project->alternative).getCashFlowVariable(cashFlowVariable);
    maxSize = to_string((project->alternative).getCashFlowVariable(cashFlowVariable)).length();
    
    if (cashFlowVariable == A_REVENUE || cashFlowVariable == A1_REVENUE || cashFlowVariable == A_COST || cashFlowVariable == A1_COST)
        maxSizeRow = 1;
    else
        maxSizeRow = 2;
}

void addRows (int option, nodeProject* project, int &counter) {
    if (option == 1 || option == 2) {
        if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT)
            counter += (project->alternative).getYears() + 1; // + 1 for "Costs" header row
        else if ((project->alternative).getRevenueCashFlowType() == UNIFORM)
            counter += 2;
        else if ((project->alternative).getRevenueCashFlowType() != NONEXISTANT)
            counter += 3;
    }
    else { // special case for inconsistent cash flow series for costs
        if ((project->alternative).getRevenueCashFlowType() == UNIFORM)
            counter += 2;
        else if ((project->alternative).getRevenueCashFlowType() == GRADIENT || (project->alternative).getRevenueCashFlowType() == GEOMETRIC) 
            counter += 3;
        else if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT)
            counter += (project->alternative).getYears() + 2;
    }
        
    if (option == 1 && (project->alternative).getRevenueCashFlowType() == INCONSISTENT)
        counter ++;
}

void printHeader (int option, int maxSizeInFirstCol, int maxSizeInSecondCol) {
    for (int i = 0; i < maxSizeInFirstCol + maxSizeInSecondCol + 7; i ++)
        cout << "-";
        
    if (option == REVENUES) {
        cout << endl << "| Revenues ";
        for (int i = 0; i < maxSizeInFirstCol + maxSizeInSecondCol -  6; i ++)
            cout << " ";
    }
    else {
        cout << endl << "| Costs ";
        for (int i = 0; i < maxSizeInFirstCol + maxSizeInSecondCol - 3; i ++)
            cout << " ";
    }
    
    cout << " |" << endl;
    for (int i = 0; i < maxSizeInFirstCol + maxSizeInSecondCol + 7; i ++)
        cout << "-";
    cout << endl;
}

void printRowForDualColumnTable (int maxSizeInFirstCol, int maxSizeInSecondCol, int &rowCounter, int &optionCounter, int maxSizeRowInFirstCol, int maxSizeRowInSecondCol, bool includeOption, string firstColumnElement, string secondColumnElement) {
    cout << "| ";
    printTableElement(true, maxSizeInFirstCol, rowCounter, maxSizeRowInFirstCol, firstColumnElement);
    cout << " | ";
    printTableElement(true, maxSizeInSecondCol, rowCounter, maxSizeRowInSecondCol, secondColumnElement);
    cout << " |";
    
    if (includeOption)
        cout << " (" << optionCounter << ")";
        
    cout << endl;
    
    rowCounter ++;
    if (includeOption)
        optionCounter ++;
}

void verifyListOfAlternatives (nodeProject* &head, bool &doNothingViable) {
    bool userDone = false;
    
    while (!userDone) {
        int numOptions = printListOfAlternatives (head, doNothingViable);
        
        cout << "Is the above correct (y/n)? ";
        userDone = getYesOrNo();
        
        if (!userDone) {
            bool inputObtained = false;
            
            cout << "Enter the project number of the alternative you would like to remove, or enter \"New\" to add a new alternative: ";
            while (!inputObtained) {
                string input;
                stringstream lineStream = getStringStream();
                lineStream >> input;
                
                if (lineStream.fail())
                    cout << "Error, please try again: ";
                else {
                    bool isInputInteger = true;
                    
                    for (int i = 0; i < (int) input.length(); i ++) // check if user's input is a number
                        if (input[i] != '.' && (input[i] < 48 || input[i] > 57))
                            isInputInteger = false;
                    if (!isInputInteger) // user has entered a non-numerical input
                        for (int i = 0; i < (int) input.length(); i ++)
                            input[i] = tolower(input[i]);
                            
                    if (input == "new") {
                        // create new alternative
                        cout << endl << "Information about new project" << endl;
                        nodeProject* newProject = new nodeProject;
                        newProject->next = NULL;
                        populateProjectInfo(newProject, head);
                        verifyPreliminaryInfo(newProject, head);
                        verifyCashFlowInfo(newProject);
                        
                        if (head == NULL)
                            head = newProject;
                        else {
                            nodeProject* helper = head;
                            while (helper->next != NULL)
                                helper = helper->next;
                                
                            helper->next = newProject;
                        }
                        
                        inputObtained = true;
                    }
                    else if (isInputInteger) {
                        if (stoi(input) > 0 && stoi(input) <= numOptions) {
                            // delete alternative associated with option number
                            int i = 1;
                            if (doNothingViable)
                                i ++;
                                
                            if (doNothingViable && stoi(input) == 1) // delete the 'do-nothing' approach
                                doNothingViable = false;
                            else { // delete a project node from the alternatives list
                                nodeProject* nodeToDelete = head;
                                for (; i < numOptions; i ++) { // find node to delete
                                    if (i == stoi(input))
                                        break;
                                    
                                    nodeToDelete = nodeToDelete->next;
                                }
                                
                                if (head == nodeToDelete) // node to delete is head node
                                    head = nodeToDelete->next;
                                else {
                                    nodeProject* helper = head;
                                    while (helper->next != nodeToDelete) // find node right before node to delete
                                        helper = helper->next;
                                        
                                    helper->next = nodeToDelete->next;
                                }
                                delete nodeToDelete;
                            }
                            
                            inputObtained = true;
                        }
                        else
                            cout << "Row does not exist, please try again: ";
                    }
                    else
                        cout << "Invalid input, please try again: ";
                }
            }
        }
    }
}

int printListOfAlternatives (nodeProject* head, bool doNothingViable) {
    // list of alternatives is empty
    if (!doNothingViable && head == NULL) {
        cout << endl << "No alternatives to show..." << endl << endl;
        return 0;
    }
    
    // determine largest cell element in right half of table
    int maxColumnSize = strlen("Name");
    int maxSizeRow = 0;
    nodeProject* helper = head;
    int rowCounter = 1;
    
    if (doNothingViable) {
        maxColumnSize = strlen("Do Nothing");
        maxSizeRow = 1;
        rowCounter ++;
    }
    
    while (helper != NULL) {
        if ((int) ((helper->alternative).getName()).length() > maxColumnSize) {
            maxColumnSize = ((helper->alternative).getName()).length();
            maxSizeRow = rowCounter;
        }
        
        helper = helper->next;
        rowCounter ++;
    }
    
    // print table
    int rowNumber = 0;
    int projectNumber = 1;
    int optionNumber = 1;
    
    cout << endl;
    for (int i = 0; i < (int) (strlen("Project Number")) + maxColumnSize + 7; i ++)
        cout << "-";
    cout << endl;
    printRowForDualColumnTable(strlen("Project Number"), maxColumnSize, rowNumber, optionNumber, 0, maxSizeRow, false, "Project Number", "Name");
    for (int i = 0; i < (int) strlen("Project Number") + maxColumnSize + 7; i ++)
        cout << "-";
    cout << endl;
    
    if (doNothingViable) // include the 'do-nothing' alternative
        printRowForDualColumnTable(strlen("Project Number"), maxColumnSize, rowNumber, optionNumber, 0, maxSizeRow, true, to_string(projectNumber - 1), "Do Nothing");
    
    helper = head;
    while (helper != NULL) {
        printRowForDualColumnTable(strlen("Project Number"), maxColumnSize, rowNumber, optionNumber, 0, maxSizeRow, true, to_string(projectNumber), (helper->alternative).getName());
        
        helper = helper->next;
        projectNumber ++;
    }
    
    for (int i = 0; i < (int) (strlen("Project Number")) + maxColumnSize + 7; i ++)
        cout << "-";
    cout << endl << endl;
    
    return optionNumber - 1; // return number of modifiable rows
}

void compareAlternatives (nodeProject* head, bool doNothingViable) {
    int methodOfAnalysis = obtainMethodOfAnalysis();
    int planningPeriod = obtainPlanningPeriod(head);
    int doNothingRank = 0; // initialized to zero in the event that the 'do-nothing' approach is not a valid alternative
    
    if (methodOfAnalysis != INTERNAL_RATE_OF_RETURN) { // user wants to use either the PW, AW or FW method of analysis
        cout << "Enter the cost of capital: ";
        double costOfCapital = getDouble();
    
        nodeProject* helper = head;
        while (helper != NULL) { // compute net value for every alternative
            calculateNetValue (methodOfAnalysis, helper, planningPeriod, costOfCapital);
            helper = helper->next;
        }
        sortAlternatives(NET_VALUE, head); // sort alternatives linked list from largest netValue to smallest
        
        // find ranking for the 'do-nothing' approach if it is a valid alternative
        if (doNothingViable) {
            int counter = 1;
            doNothingRank = 1;
            helper = head;
            
            while (helper->next != NULL) {
                if ((head->alternative).getNetValue() < 0) // the 'do-nothing' approach is the best alternative
                    break;
                if ((helper->next->next == NULL) && ((helper->alternative).getNetValue() >= 0) && (((helper->next)->alternative).getNetValue() >= 0)) { // the 'do-nothing' approach is the worst alternative
                    doNothingRank = counter + 2;
                    break;
                }
                if ((helper->alternative).getNetValue() >= 0 && ((helper->next)->alternative).getNetValue() < 0) { // the 'do-nothing' approach is somewhere in the middle
                    doNothingRank = counter + 1;
                    break;
                }
                
                helper = helper->next;
                counter ++;
            }
        }
    }
    else { // user wants to use the internal rate of return method
        cout << "Enter the minimum attractive rate of return: ";
        double minimumAttractiveRateOfReturn = getDouble();
        sortAlternatives(FIRST_COST, head); // first sort alternatives linked list from smallest first cost to largest
        
        // apply incremental IRR algorithm
        IIRRA (head, minimumAttractiveRateOfReturn, planningPeriod, doNothingViable, doNothingRank);
        
        // compute individual IRR for every alternative
        nodeProject* helper = head;
        while (helper != NULL) {
            calculateIRR (helper, minimumAttractiveRateOfReturn, planningPeriod);
            helper = helper->next;
        }
    }
    
    // set header name for third column in ensuing table of ranked alternatives
    string columnThreeHeader;
    switch (methodOfAnalysis) {
        case PRESENT_WORTH:
            columnThreeHeader = "Present Worth";
            break;
        case ANNUAL_WORTH:
            columnThreeHeader = "Annual Worth";
            break;
        case FUTURE_WORTH:
            columnThreeHeader = "Future Worth";
            break;
        case INTERNAL_RATE_OF_RETURN:
            columnThreeHeader = "Individual IRR";
            break;
    }
    
    cout << endl << "The following table summarizes the rank for each alternative" << endl;
    printAlternativesRanked (head, columnThreeHeader, doNothingRank);
}

int obtainMethodOfAnalysis () {
    cout << "Which of the below methods of analysis would you like to use to compare the above alternatives?" << endl;
    cout << "---------------------------------"<< endl;
    cout << "| (1) -      Present Worth      |" << endl;
    cout << "| (2) -      Annual Worth       |" << endl;
    cout << "| (3) -      Future Worth       |" << endl;
    cout << "| (4) - Internal Rate of Return |" << endl;
    cout << "---------------------------------" << endl;
    
    int methodOfAnalysis;
    cout << endl << "Enter the number associated with the desired analysis method: ";
    while (true) {
        methodOfAnalysis = getInteger();
        
        if (methodOfAnalysis < 1 || methodOfAnalysis > 4)
            cout << "Invalid input, please try again: ";
        else break;
    }
    
    return methodOfAnalysis;
}

int obtainPlanningPeriod (nodeProject* head) {
    cout << endl << "What would you like to set the planning period to be? A few suggestions are shown below:" << endl;
    
    cout << "The shortest project life is " << findShortestProjectLife (head) << " years." << endl;
    cout << "The longest project life is " << findLongestProjectLife (head) << " years." << endl;
    cout << "The least common multiple of lives is " << findProjectLivesLCM (head) << " years." << endl;
    
    cout << endl << "Enter the planning period you would like to use: ";
    return getInteger();
}

int findShortestProjectLife (nodeProject* head) {
    if (head == NULL)
        return 0;
    
    nodeProject* helper = head;
    int shortestProjectLife = (head->alternative).getYears();
    
    while (helper != NULL) {
        if ((helper->alternative).getYears() < shortestProjectLife)
            shortestProjectLife = (helper->alternative).getYears();
            
        helper = helper->next;
    }
    
    return shortestProjectLife;
}

int findLongestProjectLife (nodeProject* head) {
    nodeProject* helper = head;
    int longestProjectLife = 0;
    
    while (helper != NULL) {
        if ((helper->alternative).getYears() > longestProjectLife)
            longestProjectLife = (helper->alternative).getYears();
            
        helper = helper->next;
    }
    
    return longestProjectLife;
}

int findProjectLivesLCM (nodeProject* head) {
    nodeProject* helper = head;
    int leastCommonMultipleSoFar = findLongestProjectLife(head);
    
    bool leastCommonMultipleFound = false;
    while (!leastCommonMultipleFound) {
        leastCommonMultipleFound = true;
        while (helper != NULL) {
            if (leastCommonMultipleSoFar % (helper->alternative).getYears() != 0) {
                leastCommonMultipleFound = false;
                leastCommonMultipleSoFar += findLongestProjectLife(head);
                break;
            }
            
            helper = helper->next;
        }
        
        helper = head;
    }
    
    return leastCommonMultipleSoFar;
}

void calculateNetValue (int option, nodeProject* &project, int planningPeriod, double costOfCapital) {
    double netValueFinal = 0;
    double netValueRevenues = 0;
    double netValueCosts = 0;
    double netFirstCost = 0;
    double netSalvageValue = 0;
    
    int yearsAhead = 0; // number of years into the future the current iteration of analysis is taking place
    int yearsRemaining = planningPeriod; // years left for analysis (initialized as the total planning period)
    int numYears; // the number of years to be used in the ensuing formulas of the current iteration 
    
    // first compute net revenues, net costs and net salvage value(s)
    while (yearsRemaining > 0) {
        if (yearsRemaining > (project->alternative).getYears()) numYears = (project->alternative).getYears();
        else numYears = yearsRemaining;
    
        if (option == PRESENT_WORTH || option == ANNUAL_WORTH) { // find PW for both PW and AW
            if ((project->alternative).getRevenueCashFlowType() == UNIFORM)
                netValueRevenues += P_given_F(P_given_A((project->alternative).getCashFlowVariable(A_REVENUE), costOfCapital, numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getRevenueCashFlowType() == GRADIENT)
                netValueRevenues += P_given_F(P_given_G((project->alternative).getCashFlowVariable(G_REVENUE), costOfCapital, numYears) + P_given_A((project->alternative).getCashFlowVariable(A1_REVENUE), costOfCapital, numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC)
                netValueRevenues += P_given_F(P_given_A1((project->alternative).getCashFlowVariable(A1_REVENUE), costOfCapital, (project->alternative).getCashFlowVariable(J_REVENUE), numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT) {
                nodeCashFlow* helper = (project->alternative).getInconsistent(REVENUES);
                double temp = 0;
                
                for (int counter = 1; counter <= numYears; counter ++) {
                    temp += P_given_F (helper->amount, costOfCapital, counter);
                    helper = helper->next;
                }
                netValueRevenues += P_given_F(temp, costOfCapital, yearsAhead);
            }
            
            if ((project->alternative).getCostCashFlowType() == UNIFORM)
                netValueCosts += P_given_F(P_given_A((project->alternative).getCashFlowVariable(A_COST), costOfCapital, numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getCostCashFlowType() == GRADIENT)
                netValueCosts += P_given_F(P_given_G((project->alternative).getCashFlowVariable(G_COST), costOfCapital, numYears) + P_given_A((project->alternative).getCashFlowVariable(A1_COST), costOfCapital, numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getCostCashFlowType() == GEOMETRIC)
                netValueCosts += P_given_F(P_given_A1((project->alternative).getCashFlowVariable(A1_COST), costOfCapital, (project->alternative).getCashFlowVariable(J_COST), numYears), costOfCapital, yearsAhead);
            else if ((project->alternative).getCostCashFlowType() == INCONSISTENT) {
                nodeCashFlow* helper = (project->alternative).getInconsistent(COSTS);
                double temp = 0;
                
                for (int counter = 1; counter <= numYears; counter ++) {
                    temp += P_given_F (helper->amount, costOfCapital, counter);
                    helper = helper->next;
                }
                netValueCosts += P_given_F(temp, costOfCapital, yearsAhead);
            }
            
            netSalvageValue += P_given_F((project->alternative).getFirstCost() * pow(1- (project->alternative).getDepreciationRate(), numYears), costOfCapital, numYears + yearsAhead);
        }
        else if (option == FUTURE_WORTH) {
            if ((project->alternative).getRevenueCashFlowType() == UNIFORM)
                netValueRevenues += F_given_P(F_given_A((project->alternative).getCashFlowVariable(A_REVENUE), costOfCapital, numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if ((project->alternative).getRevenueCashFlowType() == GRADIENT)
                netValueRevenues += F_given_P(F_given_P(P_given_G((project->alternative).getCashFlowVariable(G_REVENUE), costOfCapital, numYears), costOfCapital, numYears) + F_given_A((project->alternative).getCashFlowVariable(A1_REVENUE), costOfCapital, numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if ((project->alternative).getRevenueCashFlowType() == GEOMETRIC)
                netValueRevenues += F_given_P(F_given_A1((project->alternative).getCashFlowVariable(A1_REVENUE), costOfCapital, (project->alternative).getCashFlowVariable(J_REVENUE), numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if ((project->alternative).getRevenueCashFlowType() == INCONSISTENT) {
                nodeCashFlow* helper = (project->alternative).getInconsistent(REVENUES);
                double temp = 0;
                
                for (int counter = 1; counter <= numYears; counter ++) {
                    temp += F_given_P(helper->amount, costOfCapital, numYears - counter);
                    helper = helper->next;
                }
                netValueRevenues += F_given_P(temp, costOfCapital, planningPeriod - (yearsAhead + numYears));
            }
            
            if ((project->alternative).getCostCashFlowType() == UNIFORM)
                netValueCosts += F_given_P(F_given_A((project->alternative).getCashFlowVariable(A_COST), costOfCapital, numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if ((project->alternative).getCostCashFlowType() == GRADIENT)
                netValueCosts += F_given_P(F_given_P(P_given_G((project->alternative).getCashFlowVariable(G_COST), costOfCapital, numYears), costOfCapital, numYears) + F_given_A((project->alternative).getCashFlowVariable(A1_COST), costOfCapital, numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if ((project->alternative).getCostCashFlowType() == GEOMETRIC)
                netValueCosts += F_given_P(F_given_A1((project->alternative).getCashFlowVariable(A1_COST), costOfCapital, (project->alternative).getCashFlowVariable(J_COST), numYears), costOfCapital, planningPeriod - (yearsAhead + numYears));
            else if  ((project->alternative).getCostCashFlowType() == INCONSISTENT) {
                nodeCashFlow* helper = (project->alternative).getInconsistent(COSTS);
                double temp = 0;
                
                for (int counter = 1; counter <= numYears; counter ++) {
                    temp += F_given_P(helper->amount, costOfCapital, numYears - counter);
                    helper = helper->next;
                }
                netValueCosts += F_given_P(temp, costOfCapital, planningPeriod - (yearsAhead + numYears));
            }
            
            netSalvageValue += F_given_P((project->alternative).getFirstCost() * pow(1 - (project->alternative).getDepreciationRate(), numYears), costOfCapital, planningPeriod - (numYears + yearsAhead));
        }
        yearsRemaining -= (project->alternative).getYears();
        yearsAhead += (project->alternative).getYears();
    }
    if (option == ANNUAL_WORTH) { // spread present value for revenues, costs and salvage value(s) across entire planning period to find resulting annual value
        netValueRevenues = A_given_P(netValueRevenues, costOfCapital, planningPeriod);
        netValueCosts = A_given_P(netValueCosts, costOfCapital, planningPeriod);
        netSalvageValue = A_given_P(netSalvageValue, costOfCapital, planningPeriod);
    }
    
    // then factor in the first cost(s)
    if (option == PRESENT_WORTH)
        for (int i = 0; i < planningPeriod; i += (project->alternative).getYears())
            netFirstCost += P_given_F((project->alternative).getFirstCost(), costOfCapital, i); 
    else if (option == ANNUAL_WORTH) {
        double totalFutureFirstCosts = 0;
        for (int i = 0; i < planningPeriod; i += (project->alternative).getYears())
            totalFutureFirstCosts += F_given_P((project->alternative).getFirstCost(), costOfCapital, planningPeriod - i);
            
        netFirstCost = A_given_F(totalFutureFirstCosts, costOfCapital, planningPeriod);
    }
    else if (option == FUTURE_WORTH)
        for (int i = 0; i < planningPeriod; i += (project->alternative).getYears())
            netFirstCost += F_given_P((project->alternative).getFirstCost(), costOfCapital, planningPeriod - i);
    
    netValueFinal = netValueRevenues - netValueCosts + netSalvageValue - netFirstCost;
    (project->alternative).setNetValue(netValueFinal); // set netValue member variable of project to its calculated value
}

void sortAlternatives (int option, nodeProject* &head) {
    if (head == NULL || head->next == NULL) 
        return;
    
    // count the total number of nodes in the list
    nodeProject* helper = head;
    int numNodes = 0;
    while (helper != NULL) {
        numNodes ++;
        helper = helper->next;
    }
    
    // bubble sort
    for (int upperLimit = numNodes; upperLimit > 1; upperLimit --) {
        nodeProject* traversingNode = head;
        nodeProject* nodeBeforeTraversingNode = head;

        for (int i = 1; i < upperLimit; i ++) {
            nodeProject* temp = NULL;
            bool swapOccurred = false;
            if (option == NET_VALUE) { // sort based on netValue
                if (((traversingNode->next)->alternative).getNetValue() > (traversingNode->alternative).getNetValue()) {
                    swapAlgorithm(head, traversingNode, temp, nodeBeforeTraversingNode);
                    swapOccurred = true;
                }
            }
            else { // sort based on firstCost
                if (((traversingNode->next)->alternative).getFirstCost() < (traversingNode->alternative).getFirstCost()) {
                    swapAlgorithm (head, traversingNode, temp, nodeBeforeTraversingNode);
                    swapOccurred = true;
                }
            }
            
            if (!swapOccurred) {
                nodeBeforeTraversingNode = traversingNode;
                traversingNode = traversingNode->next;
            }
            else
                nodeBeforeTraversingNode = temp;
        }
    }
}

void swapAlgorithm (nodeProject* &head, nodeProject* &currentNode, nodeProject* &nextNode, nodeProject* &previousNode) {
    nextNode = currentNode->next;
    currentNode->next = nextNode->next;
    nextNode->next = currentNode;
    
    if (currentNode != head) // there is a node pointing to the current node
        previousNode->next = nextNode;
    else // temp node becomes the new head node
        head = nextNode;
}

void IIRRA (nodeProject* &head, double MARR, int planningPeriod, bool doNothingViable, int &doNothingRank) {
    if (head == NULL && doNothingViable) {
        doNothingRank = 1;
        return;
    }
    
    bool doNothingFound = false; // variable for remembering that the ranking for the 'do-nothing' approach has been determined
    int numAlternatives = 0;
    nodeProject* helper = head;
    while (helper != NULL) {
        helper = helper->next;
        numAlternatives ++;
    }
    if (doNothingViable) numAlternatives ++;
    int numIterations = numAlternatives - 1; // given that the IIRRA compares two alternatives per iteration, the total number of iterations should be one less than the total number of alternatives
    
    for (int iteration = 1; iteration <= numIterations; iteration ++)  {
        if (!doNothingViable || doNothingFound) {
            nodeProject* helperOne = head;
            nodeProject* helperTwo = head->next;
        
            nodeProject* lastNode = head;
            while (lastNode->next != NULL)
                lastNode = lastNode->next;
                
            calculateNetValue(PRESENT_WORTH, helperOne, planningPeriod, MARR);
            calculateNetValue(PRESENT_WORTH, helperTwo, planningPeriod, MARR);
            
            if ((helperTwo->alternative).getNetValue() - (helperOne->alternative).getNetValue() >= 0) { // move first alternative to end of list
                lastNode->next = head;
                head = head->next;
                lastNode->next->next = NULL;
            }
            else { // move second alternative to end of list
                lastNode->next = head->next;
                head->next = lastNode->next->next;
                lastNode->next->next = NULL;
            }
        }
        else {
            // continuously compare the first alternative in the list with the 'do-nothing' approach until the 'do-nothing' approach is the worst alternative of the two
            nodeProject* helper = head;
            calculateNetValue(PRESENT_WORTH, helper, planningPeriod, MARR);
            
            if ((helper->alternative).getNetValue() >= 0) { // figuratively move the 'do-nothing' alternative to end of list (actually just remembering the rank for the 'do-nothing' alternative)
                doNothingRank = iteration + 1; // always +1 as there will be a singular best alternative at the top of the resulting ranked list
                doNothingFound = true; // continue with the default incremental IRR method
            }
            else { // move head project node to end of list
                nodeProject* lastNode = head;
                while (lastNode->next != NULL)
                    lastNode = lastNode->next;

                lastNode->next = head;
                head = head->next;
                lastNode->next->next = NULL;
            }
        }
        
        if (doNothingViable && iteration == numIterations && !doNothingFound)
            doNothingRank = 1; // the 'do-nothing' approach must be the best alternative
    }
}

void calculateIRR (nodeProject* project, double MARR, int planningPeriod) {
    int percentageMARR = int (MARR * 100);
    calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(percentageMARR) / 100);
    
    int counter;
    for (counter = 0; counter < 1000; counter ++) { // PercentageMARR will hold the greater value for subsequent linear interpolation at the end of this loop        
        // first find whether we should be decreasing or increasing the cost of capital
        double oldNetValue = (project->alternative).getNetValue();
        if (percentageMARR + 1 != 0)
            calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(percentageMARR + 1) / 100);
        else
            calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(percentageMARR + 2) / 100);
        
        // then proceed with this iteration of the algorithm
        if ((oldNetValue > 0 && (project->alternative).getNetValue() < 0) || (oldNetValue < 0 && (project->alternative).getNetValue() > 0)) { // continue to linear interpolation
            percentageMARR ++;
            break;
        }
        else if (abs((project->alternative).getNetValue()) >= 0 && abs((project->alternative).getNetValue()) < 1) { // upper and lower bounds tight enough for linear interpolation
            percentageMARR ++;
            break;
        }
        else if (abs((project->alternative).getNetValue()) < abs(oldNetValue)) { // continue algorithm by increasing the cost of capital
            if (percentageMARR + 1 != 0)
                percentageMARR ++;
            else
                percentageMARR += 2;
                
            calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(percentageMARR) / 100);
        }
        else { // cost of capital should be decreasing for this iteration
            if (percentageMARR - 1 != 0)
                percentageMARR --;
            else
                percentageMARR -= 2;
                
            calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(percentageMARR) / 100);
        }
    }
    
    if (counter < 1000)
        linearInterpolation (project, percentageMARR, percentageMARR - 1, planningPeriod);
    else // algorithm failed to find IRR value that would give an NPV of zero (IRR must then be zero, indicating no return on this investment)
        (project->alternative).setNetValue(0);
}

void linearInterpolation (nodeProject* project, int largerCostOfCapital, int smallerCostOfCapital, int planningPeriod) {
    calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(smallerCostOfCapital) / 100);
    double firstNPV = (project->alternative).getNetValue();
    calculateNetValue(PRESENT_WORTH, project, planningPeriod, double(largerCostOfCapital) / 100);
    double secondNPV = (project->alternative).getNetValue();
    
    (project->alternative).setNetValue((smallerCostOfCapital - (firstNPV / ((firstNPV - secondNPV) / (smallerCostOfCapital - largerCostOfCapital)))) / 100);
}

void printAlternativesRanked (nodeProject* head, string columnThreeHeader, int doNothingRank) {
    if (head == NULL && doNothingRank == 0) {
        cout << endl << "No alternatives to show..." << endl << endl;
        return;
    }
    
    nodeProject* helper = head;
    int rank = 1;
    int rowNum = 1;
    
    // determine values for variables associated with largest cell element in second and third columns
    int maxSizeColTwo = strlen("Project Name");
    int maxSizeColThree = (columnThreeHeader).length();
    int rowWithMaxSizeColTwo = 0;
    int rowWithMaxSizeColThree = 0;
    
    while (helper != NULL) {
        if (int((helper->alternative).getName().length()) > maxSizeColTwo) {
            maxSizeColTwo = ((helper->alternative).getName()).length();
            rowWithMaxSizeColTwo = rowNum;
        }
        if (int(to_string((helper->alternative).getNetValue()).length()) > maxSizeColThree) {
            maxSizeColThree = to_string((helper->alternative).getNetValue()).length();
            rowWithMaxSizeColThree = rowNum;
        }
        
        helper = helper->next;
        rowNum ++;
    }
    
    // print column headers
    for (int i = 0; i < maxSizeColTwo + maxSizeColThree + int(strlen("Rank")) + 10; i ++)
        cout << "-";
    cout << endl << "| ";
    printTableElement(true, strlen("Rank"), 0, 0, "Rank");
    cout << " | ";
    printTableElement(true, maxSizeColTwo, 0, rowWithMaxSizeColTwo, "Project Name");
    cout << " | ";
    printTableElement(true, maxSizeColThree, 0, rowWithMaxSizeColThree, columnThreeHeader);
    cout << " |" << endl;
    for (int i = 0; i < maxSizeColTwo + maxSizeColThree + int(strlen("Rank")) + 10; i ++)
        cout << "-";
    cout << endl;
    
    // print rest of alternatives table
    helper = head;
    int numAlternatives = 0; // total number of alternatives (including the 'do-nothing' approach)
    while (helper != NULL) {
        numAlternatives ++;
        helper = helper->next;
    }
    if (doNothingRank > 0)
        numAlternatives ++;
    helper = head;
    
    while (helper != NULL || rank <= numAlternatives) {
        if (rank == doNothingRank) {
            cout << "| ";
            printTableElement(true, strlen("Rank"), rank, 0, to_string(rank));
            cout << " ";
            
            if (columnThreeHeader == "Individual IRR")
                printRowForDualColumnTable(maxSizeColTwo, maxSizeColThree, rank, rowNum, rowWithMaxSizeColTwo, rowWithMaxSizeColThree, false, "Do Nothing", "N/A"); // rowNum acts as a dummy variable
            else
                printRowForDualColumnTable(maxSizeColTwo, maxSizeColThree, rank, rowNum, rowWithMaxSizeColTwo, rowWithMaxSizeColThree, false, "Do Nothing", "0"); // rowNum acts as a dummy variable
        }
        else {
            cout << "| ";
            printTableElement(true, strlen("Rank"), rank, 0, to_string(rank));
            cout << " ";
            printRowForDualColumnTable(maxSizeColTwo, maxSizeColThree, rank, rowNum, rowWithMaxSizeColTwo, rowWithMaxSizeColThree, false, (helper->alternative).getName(), to_string((helper->alternative).getNetValue())); // rowNum acts as a dummy variable
        
            helper = helper->next;
        }
    }
    
    for (int i = 0; i < maxSizeColTwo + maxSizeColThree + int(strlen("Rank")) + 10; i ++)
        cout << "-";
    cout << endl;
}