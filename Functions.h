#pragma once

#include "Alternative.h"

// linked list node definition for alternatives
typedef struct nodeProject {
    Alternative alternative;
    nodeProject* next;
} nodeProject;

// ask user whether the 'do-nothing' option is a viable alternative or not
// arguments: none
// returns true if the 'do-nothing' option is a viable alternative and false if not
bool includeDoNothing();

// ask user for total number of mutually exclusive alternatives (excluding the 'do-nothing' option)
// arguments: none
// returns the number of alternatives as an integer
// note that decimals user inputs will be floored
int setNumOfAlternatives();

// prompts the user for a yes or no input
// arguments: none
// returns true if input is a positive response and false if negative
// note that the function includes error-checking
bool getYesOrNo ();

// prompts the user for an integer input
// arguments: none
// returns the user's input as an integer
// note that the function includes error-checking for negative numbers
// note that the function continues to re-prompt the user until a valid input is obtained
int getInteger();

// prompts the user for a stringstream input
// arguments: none
// returns the user's input as a stringstream
stringstream getStringStream();

// populates all information regarding a project
// arguments: the project node for the newly-created project, the first project node in the list
// returns nothing 
void populateProjectInfo (nodeProject* projectToCreate, nodeProject* firstProject);

// populates the preliminary information for a newly created project
// arguments: the node containing the Alternative being created, the node containing the first Alternative in the list
// returns nothing
void getPreliminaryProjectInfo (nodeProject* projectToCreate, nodeProject* firstProject);

// determines whether a name is already used by another project in the list
// arguments: the name as entered by the user, the project node being created/modified, the first project node in the list
// returns true if the name is already being used and false if not
// note that the function will print the prompt for a new input from the user but will not accept the user's new input
bool isNameInUse(string rowValue, nodeProject* projectBeingDesigned, nodeProject* firstProject);

// converts user's input as a string to a doulbe
// arguments: the user's input as a string, the boolean variable used to track whether the input is valid or not (passed by reference)
// returns the user's input as a double (even if valid was not set to true)
// note that the function will print the prompt for a new input from the user but will not accept the user's new input
double convertStringToDouble(string toConvert, bool &valid);

// prints the prliminary information of a project in a formatted table
// arguments: pointer to the project node whose info is being printed, boolean array that tracks which inputs have been received
// returns nothing
void printProjectPreliminaryInfo(nodeProject* project, const bool* inputs);

// prints a single element in a dynamic table and re-formats the table if necessary
// arguments: whether the cell in question contains an element, the maximum element lenght in the table, the row of the cell in question,
// the row with the longest cell element, the element of the cell (to be printed) as a string
// returns nothing
// note that rows are zero-indexed
void printTableElement (bool elementExists, int maxColSize, int row, int maxSizeRow, string cellElement);

// prints table containing the five types of cash flows and returns the user's choice
// arguments: an integer variable specifying if this is for revenues or costs
// returns the user's choice as an integer
// note that the function includes error-checking
int obtainCashFlowType (int option);

// populates the cash flow information for an alternative object
// arguments: an integer variable specifying if this is for revenues or costs, the cash flow type and the alternative object itself
// returns nothing
void getCashFlowInfo (int option, int cashFlowType, Alternative &alternative);

// creates an inconsistent cash flow series by prompting the user for a cash flow value for each EOY until the alternative's economic life is reached
// arguments: the alternative object the ensuing inconsistent cash flow series will be created for,
// the head node of the ensuing cash flow series
// returns nothing
// note that the head node passed as the second argument will be the head of the ensuing cash flow series (passed by reference)
void createInconsistentCashFlowSeries (Alternative alternative, nodeCashFlow* &head);

// prompts the user for a double  input
// arguments: none
// returns the user's input as an double
// note that the function includes error-checking and re-prompts the user for an input if
// their input is either not a number or is a negative number
double getDouble();

// validates a C++ string input as a valid double
// arguments: the input as a C++ string
// returns true if the input is a valid double and false if not
bool validateInputAsDouble (string input);

// prompts the user to double check all preliminary information they entered with respect to a particular project node
// arguments: the project node being verified, the first project node in the list
// returns nothing
// note that the function only returns once the user confirms that all preliminary info is correct
void verifyPreliminaryInfo (nodeProject* project, nodeProject* firstProjectInList);

// destroy an entire linked list of cash flow nodes 
// arguments: the head of the linked list to be destroyed
// returns nothing
void destroyCashFlowLinkedList(nodeCashFlow* head);

// prints the information regarding the cash flow type for both the revenues and the costs of a project in a formatted table
// arguments: the project node for which you want to print the cash flow types
// returns nothing
void printProjectCashFlowType(nodeProject* project);

// returns the cash flow type as a string given the cash flow type as an integer
// arguments: the cash flow type as an integer
// returns the associated cash flow type as a string (i.e., its actual name)
string setCashFlowType(int cashFlowTypeAsInt);

// prompts the user to double check all cash flow information they entered with respect to a particular project node
// arguments: the project node being verified
// returns nothing
// note that the function only returns once the user confirms that all cash flow info is correct
void verifyCashFlowInfo (nodeProject* project);

// sets cash flow data to newly inputted value (if requested by user)
// arguments: the alternative whose data the user wishes to modify, the row number the user wishes to modify,
// the new value for this row as inputted by the user
// returns nothing
// note that the alternative is passed by reference so the function can modify its appropriate cash flow data directly
void setNewCashFlowData (Alternative &alternative, int rowNum, double value);

// sets cash flow data for costs to newly inputted value
// arguments: the alternative whose data the user wishes to modify, the row number the user wishes to modify,
// the new value for this row as inputted by the user
// returns nothing
// note that the alternative is passed by reference so the function can modify its appropriate cash flow data directly
// note that the option variable ignores the number of options there were in revenues and is reset to start from 1
void setCashFlowDataForCosts (Alternative &alternative, int option, double value);

// prints a project's cash flow info as a formatted table
// arguments: the node project whose alternative's cash flow info you wish to print
// returns total number of options from table as an integer (i.e., the number of rows user can modify)
int printProjectCashFlowInfo(nodeProject* project);

// determines values for the maximum number, the string length of the maximum number and the row in which the maximum number is found
// arguments: the project node being assessed, the cash flow variable that translates to which cash flow variable you wish to get from the project being assessed,
// the maximum number in the column thus far, the string length of the maximum number, the row in which the maximum number is found
// returns nothing
void setHelperSizeVariables(nodeProject* project, int cashFlowVariable, int &maxNum, int &maxSize, int &maxSizeRow);

// adds an integer value to a counter variable to update the row containing the longest element
// arguments: an option variable, the node project being assessed, the counter variable that tracks the row number
// returns nothing
// note that: option = 1 means first column, option = 2 means second column, option = 3 means special case for inconsistent cash flow series for costs
void addRows (int option, nodeProject* project, int &counter);

// prints the header in a project's cash flow info table
// arguments: an option variable, the maximum element size in the first column, the maximum element size in the second column
// returns nothing
// note that the function also prints dashes in the lines prior and after the header
// note that: option = 1 means print "Revenues", option = 2 means print "Costs"
void printHeader (int option, int firstMaxSize, int secondMaxSize);

// prints a single row in a dual-column table
// arguments: the maximum size in the first column, the maximum size in the second column, the row counter, the option counter,
// the row containing the maximum sized element from the first column, the row containing the maximum sized element from the second column,
// a boolean value indicating whether the row has an associated option variable,
// the element (to be printed) from the first column as a string, the element (to be printed) from the second column as a string
// returns nothing
// note that the row and option counters are passed by reference so that the function can increment these variables if necessary
// note that if the row has an associated option variable, that option variable is printed at the end of the row within parantheses
void printRowForDualColumnTable (int maxSizeInFirstCol, int maxSizeInSecondCol, int &rowCounter, int &optionCounter, int maxSizeRowInFirstCol, int maxSizeRowInSecondCol, bool includeOption, string firstColumnElement, string secondColumnElement);

// prompts the user to double check all alternatives that they've entered (user can either delete existing alternatives or create new ones) 
// arguments: the head project node, a boolean variable indicating whether the 'do-nothing' approach was a viable option
// returns nothing
// note that the function only returns once the user confirms that the resulting list of project nodes is clTabCtrlcorrect
// note that the boolean variable indicating whether the 'do-nothing' approach is a viable option is passed by reference to be reused
void verifyListOfAlternatives(nodeProject* &head, bool &doNothingViable);

// prints the complete list of all alternatives to be compared in a formatted table
// arguments: the head project node, a boolean variable indicating whether the 'do-nothing' approach is a viable option
// returns total number of options from table as an integer (i.e., the number of rows user can modify)
int printListOfAlternatives (nodeProject* head, bool doNothingViable);

void compareAlternatives (nodeProject* head, bool doNothingViable);

// prints the four different methods of analysis and prompts the user to select one of the four
// arguments: none
// returns method of analysis as selected by the user as an integer
int obtainMethodOfAnalysis ();

// prompts the user to enter a planning period for ensuing analysis
// arguments: the head project node
// returns the planning period as an integer
int obtainPlanningPeriod (nodeProject* head);

// finds the shortest project life among all project nodes in alternatives list
// arguments: the head project node
// returns the shortest project life as an integer
int findShortestProjectLife (nodeProject* head);

// finds the longest project life among all project nodes in alternatives list
// arguments: the head project node
// returns the shortest project life as an integer
int findLongestProjectLife (nodeProject* head);

// computes the least common multiple among the project lives from the alternatives list
// arguments: the head project node
// returns the least common multiple of project lives as an integer
int findProjectLivesLCM (nodeProject* head);

// sets the net value for a project node according to the specified method of analysis
// arguments: the specified method of analysis, the project node for which the net value is being calculated,
// the planning period in years, the cost of capital as a decimal
// returns nothing
// note that the project is passed by reference as the function sets the net value member of the project at the end of the function
void calculateNetValue (int methodOfAnalysis, nodeProject* &project, int planningPeriod, double costOfCaptial);

// sorts a linked list of project nodes based on either the net value or the first cost value in each project node's alternative
// arguments: an option specifying which value to sort the linked list by, the head project node of the list (passed by reference)
// returns nothing
// note that the two options to sort the linked list by are net value and first cost
void sortAlternatives (int option, nodeProject* &head);

// executes the swapping mechanism between two neighbouring nodes in a linked list
// arguments: the head project node, the current project node, the next project node, the previous project node
// returns nothing
// note that all arguments are passed by reference
void swapAlgorithm (nodeProject* &head, nodeProject* &currentNode, nodeProject* &nextNode, nodeProject* &previousNode);

// the incremental internal rate of return algorithm that shuffles around the project nodes in a list, ultimately sorting them from best to worst
// arguments: the head project node (passed by reference), the minimum attractive rate of return, the planning period,
// a boolean variable indicating whether the 'do-nothing' approach is viable or not and the rank for the 'do-nothing' alternative
// note that if the 'do-nothing' approach is a viable alternative, the rank for the 'do-nothing' alternative will be passed back into the last parameter (by reference)
void IIRRA (nodeProject* &head, double MARR, int planningPeriod, bool doNothingViable, int &doNothingRank);

// algorithm for computing the individual IRR value for a project node's alternative
// arguments: the project node, the minimum attractive rate of return, the planning period
// returns nothing
// note that the function uses linear interpolation in the end to refine the project's IRR value
// note that the function sets the alternative's NetValue to the resulting IRR (done through calling the linear interpolation function)
void calculateIRR (nodeProject* project, double MARR, int planningPeriod);

// computes and sets the refined IRR value for a project node alternative using the two neighbouring cost of capital values via the linear interpolation method
// arguments: the project node, the larger cost of capital (upper bound), the smaller cost of capital (lower bound), the planning period
// returns nothing
// note that the function overwrites the project node alternative's net value in the end with the refined IRR value
void linearInterpolation (nodeProject* project, int largerCostOfCapital, int smallerCostOfCapital, int planningPeriod);

// prints the list of alternatives in a formatted table
// arguments: the head project node, the header of the third column as a string, the ranking of the 'do-nothing' approach if it is a viable alternative
// returns nothing
// note that the function does not perform any ranking of the alternatives
// note that the function automatically knows whether the 'do-nothing' approach is a viable alternative or not based on its rank (the rank would be zero if the 'do-nothing' approach was not valid)
void printAlternativesRanked (nodeProject* head, string columnThreeHeader, int doNothingRank);