/*By Eric 22/12/2020 convert from infix to postfix expresion*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <math.h>  

using namespace std;
 
struct Node{
    string data;
	int precedence;       
    Node *next;
};

int top = -1;

//function prototypes
Node *createNode(string value, int precedence);
void sortList(string expression, Node *&infix);
void insertAtTheEnd(Node *&infix, string value, int precedence);

bool stackIsEmpty();
int getStackPrecedence(Node *node);
void insertValueToPila(Node *&stack, Node *node);
void unstackOperators(Node *&stack, Node *&posfija,Node *operatorNode);
void convertInfixToPostfix(Node *infija, Node *&postfija, Node *&stack);
void printPostfix(Node *infix, int count);

void evaluate(Node *posfija, Node *&stack);
void unstackOperands(Node *&stack, Node *operatorNode);
std::string convertIntToString (int number);
 
int main()
{
    Node *infix = NULL;
    Node *postfix = NULL;
    Node *stack = NULL;
    int option;
    string expression;
 
    do {
    	cout<< "\t..::Menu::..\n"<< endl;
    	cout<< "[1] Type Infix expression"<< endl;
    	cout<< "[2] Convert Infix to Postfix"<< endl;
    	cout<< "[3] Evaluate expression"<< endl;
    	cout<< "[4] Quit"<< endl;
 
    	cout<< "\nSelect: ";
        cin>> option;
        
        system("cls");
        switch(option) {
            case 1:
            	infix = NULL;
            	postfix = NULL; 				// clear all
            	stack = NULL;
            	
            	cout<< "Type an infix expression: ";
            	cin>> expression;
            	
                sortList(expression, infix);
            break;
            
            case 2:
            	stack = NULL; 					// clear the stack
            	top = -1;
            	
            	if (infix != NULL) {
            		convertInfixToPostfix(infix, postfix, stack);
                	printPostfix(postfix, 0);	
				} else {
					cout<< "You must type an infix expression first"<< endl;
				}
            break;
            
            case 3:
            	stack = NULL; 					// clear the stack
            	top = -1;
            	
            	if (postfix != NULL) {
            		evaluate(postfix, stack);
					
					postfix = NULL; 			// clear the lists
					infix = NULL;	
				} else {
					cout<< "You must convert from infix to postfix expression first"<< endl;
				}
            break;
            
            case 4:
				cout<< "\tThank you!!"<< endl; 
            	break;
        }
 
        cout<< "\n";
        system("pause");  
		system("cls");
 
    } while(option != 4);
 
   return 0;
}

Node *createNode(string value, int precedence)
{
	Node *newNode = new Node();
	
	newNode->data  = value;
    newNode->next = NULL;
    newNode->precedence = precedence;
    
	return newNode;
}
 
void insertAtTheEnd(Node *&infix, string value, int precedence)
{
	if (infix == NULL) {
		Node *newNode = createNode(value, precedence); 
		infix = newNode; 										// just add the new element to your list "list-> new_element"
		return;
	}
    															// if you have more than one element
    insertAtTheEnd(infix->next, value, precedence); 			// go to the end of the list
}
 
void printPostfix(Node *postfix, int count)
{
	if (postfix == NULL) {
		if (count == 0) {
			cout<< "\n\t**empty list**"<< endl;
		} else {
			cout<< "NULL"<< endl;
		}
		return;
	}
    
	cout<< "[ "<< count+1 <<" | " <<postfix->data << " ]"<< "  ->  ";
    printPostfix(postfix->next, count+1);
}

void sortList(string expression, Node *&infix)
{
	string number = "";
	string oprator = "";
	
	for (int i = 0; i < expression.length(); i++) { // iterate the entire string
		char c = expression[i];						// get character
		
		if (isdigit(c)) {							// is digit
     		number +=c; 							// get any number
     		if (!isdigit(expression[i+1])) {		// before one operator
     			insertAtTheEnd(infix, number, 0);	// insert node to list
				number = "";						// clear number
			}
		} else {									// is operator							
			oprator = c; 							// get operator
			
			switch(c){								// set precedence
				case '^':
					insertAtTheEnd(infix, oprator, 4);
					break;
					
				case '/':
					insertAtTheEnd(infix, oprator, 2);
					break;
					
				case '*':
					insertAtTheEnd(infix, oprator, 2);
					break;
					
				case '+':
					insertAtTheEnd(infix, oprator, 1);
					break;
					
				case '-':
					insertAtTheEnd(infix, oprator, 1);
					break;
					
				case '(':
					insertAtTheEnd(infix, oprator, 5);
					break;
					
				case ')':
					insertAtTheEnd(infix, oprator, -1);
					break;
			}
			
			oprator = "";							// clear oprator				
		}
	}
}

bool stackIsEmpty()
{
	if (top == -1) {
		return true;
	} else {
		return false;
	}
}

void insertValueToPila(Node *&stack, Node *node)
{
	if (node == NULL) {
		return;
	}
	
	if (stackIsEmpty()) {													// first node
		Node *newNode = createNode(node->data, getStackPrecedence(node));	// create node
		stack = newNode;													
		top++;
		
		return;
	}
	
	Node *newNode = createNode(node->data, getStackPrecedence(node));		// create node
	newNode->next = stack;  												// stack newNode
	stack = newNode; 														// overwrite stack
	
	top++;																	// top+1
}

int getStackPrecedence(Node *node)
{
	char c = node->data[0];
	int stackPrecedence = 0;
	
	switch(c) {
		case '^':
			stackPrecedence = 3;
			break;
					
		case '/':
			stackPrecedence = 2;
			break;
					
		case '*':
			stackPrecedence = 2;
			break;
					
		case '+':
			stackPrecedence = 1;
			break;
					
		case '-':
			stackPrecedence = 1;
			break;
					
		case '(':
			stackPrecedence = 0;
			break;
					
		case ')':
			stackPrecedence = -1;
			break;
	}
	
	return stackPrecedence;
}

void convertInfixToPostfix(Node *infix, Node *&postfix, Node *&stack)
{
	if (infix == NULL) {								// when infix ends
		unstackOperators(stack, postfix, infix);		// unstack last operators
		return;
	}
    
    if (isdigit(infix->data[0])) {						// is digit
    	insertAtTheEnd(postfix, infix->data, -1);		// add to postfix list precedence N/A = -1
	} else {
		unstackOperators(stack, postfix, infix);		// untack
	}
    
    convertInfixToPostfix(infix->next, postfix, stack);	// keep iterating
}

void unstackOperators(Node *&stack, Node *&postfix, Node *operatorNode)
{
	if (stackIsEmpty()) {							// first node
		insertValueToPila(stack, operatorNode);		// stack node
		return;	
	} else{
		Node *currentNode = stack;					//get current Node
		
		if (operatorNode != NULL) {										// when infix has not finished			 
			if (operatorNode->precedence > currentNode->precedence) { 	// if infix priority > stack priority 
				insertValueToPila(stack, operatorNode);					// stack operator
				return;
			}	
		}
		
		if (currentNode->data[0] != '(') {
			insertAtTheEnd(postfix, currentNode->data, -1); // precedence N/A = -1 in postfix	
		}	
		
		stack = currentNode->next; 					// unstack node and continue to next node
		top--;										// top -1;
		
		if (currentNode->data[0] == '(') {          // break in this case
			return;
		}
		
		unstackOperators(stack, postfix, operatorNode); // keep unstacking
	}
}

void evaluate(Node *postfix, Node *&stack)
{
	if (postfix == NULL) { 							// when postfix ends
		unstackOperands(stack, postfix);			// unstack last operands
		return;
	}
    
    if (isdigit(postfix->data[0])) {				// is digit 
    	insertValueToPila(stack, postfix);			// stack node
	} else {
		unstackOperands(stack, postfix);			// unstack
	}
    
    evaluate(postfix->next, stack);					// keep iterating
}

void unstackOperands(Node *&stack, Node *operatorNode)
{
	if (stackIsEmpty()) {							// first element
		insertValueToPila(stack, operatorNode);		// stack
		return;	
	} else {
		Node *currentNode = stack;					// get current node
		char op;
		int number1 = 0;
		int number2 = 0;
		int result = 0;
		string finalResult = "";
		
		if (currentNode->next != NULL) {			// as long as we have more than 2 nodes
			string num1 = currentNode->next->data;	// get first operand
			string num2 = currentNode->data;		// get second operand
			
			// convert from string to int
			istringstream(num2) >> number2;			
			istringstream(num1) >> number1;
			
			if (operatorNode == NULL) { 			// When an user forgot to put * in a multiplication 
				result = number1 * number2;			// example: 1(2+2) = 4;
			} else {
				op = operatorNode->data[0];			// get operator
			}
			
			// do operations
			switch(op) {
				case '^':
					result = pow(number1, number2); 
					break;
					
				case '/':
					result = number1 / number2; 
					break;
					
				case '*':
					result = number1 * number2; 
					break;
					
				case '+':
					result = number1 + number2; 
					break;
					
				case '-':
					result = number1 - number2; 
					break;
			}
			
			stack = currentNode->next->next; 
			top -= 2;								// unstack 2 node and top -2;
			
			// add final result to stack
			finalResult = convertIntToString(result);
			Node *resultNode = createNode(finalResult, -1); // precedence N/A = -1;
			insertValueToPila(stack, resultNode);
			
			if (operatorNode == NULL) { 					// if an user forgot to put * in a multiplication
				unstackOperands(stack, operatorNode);		// keep iterating
			}
		} else {
			stack = currentNode->next;						// untack last operand
			top--;
			
			cout<< "Resultado: "<< currentNode->data<< endl;// print value
		}	
		
	}
}

std::string convertIntToString (int number)
{
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}
