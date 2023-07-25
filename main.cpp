#include <iostream>

using namespace std;

#include "Globals.h"
#include "Alternative.h"
#include "Functions.h"

int main () {    
    bool doNothingViable = includeDoNothing(); // determine whether or not to include the 'do-nothing' approach
    int numOfAlternatives = setNumOfAlternatives(); // determine the total number of alternatives less the 'do-nothing' approach
    nodeProject* head = NULL; // head node for ensuing linked list of alternatives
    
    // creation of individual project alternatives
    for (int i = 1; i <= numOfAlternatives; i ++) {
        nodeProject* temp = new nodeProject;
        temp->next = NULL;
        cout << endl << "Information about project " << i << endl;
        
        if (head == NULL)
            head = temp;
        else {
            nodeProject* helper = head;
            while (helper->next != NULL) // find most recently created node
                helper = helper->next;
                
            helper->next = temp;
        }
        
        populateProjectInfo(temp, head);
    }
    
    // verification of individual project alternatives
    nodeProject* helper = head;
    for (int i = 1; i <= numOfAlternatives; i ++) {
        cout << endl << "Information about project " << i << ":" << endl << endl; 
        
        verifyPreliminaryInfo (helper, head);
        verifyCashFlowInfo (helper);
        
        helper = helper->next;
    }
    
    // verification of project alternatives list (as a whole)
    cout << endl << "The following table lists all alternatives to be compared" << endl;
    verifyListOfAlternatives (head, doNothingViable);
    
    // analysis of alternatives
    compareAlternatives (head, doNothingViable);
}