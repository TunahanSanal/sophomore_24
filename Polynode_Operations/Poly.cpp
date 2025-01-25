#include <stdio.h>	//Definitions of needed libraries
#include "Poly.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

PolyNode *CreatePoly(char *expr){		//Definition of  polynomial creation
	PolyNode* start = 0;
	istringstream flow(expr);
	string t;	//Temporary string for holding to variables
	double coef = 0;	//Holding the coefficient of variables
	int exp = 0;	//Keeping the exponential coefficient of variables

	while (flow >> t) {
		if (t == "+") continue;	//Detection of positivity or negativity
		if (t == "-") {
			flow >> t;
			t = "-" + t;
		}
		
		if (sscanf_s(t.c_str(), "%lfx^%d", &coef, &exp) == 2) {//Catching the coefficient and exponential variables
			start = AddNode(start, coef, exp);
		}
		else if (sscanf_s(t.c_str(), "%lfx", &coef) == 1) {//Catching only the X coefficient
			start = AddNode(start, coef, 1);
		}
		else if (sscanf_s(t.c_str(), "%lf", &coef) == 1) {//Catching the ordinary numberial coefficient
			start = AddNode(start, 0, 0);
		}
	}
	return start;
} 


void DeletePoly(PolyNode* poly) {
	PolyNode* now = poly; //Creation of newest poly for holding the elements
	while (now != 0) {
		PolyNode* new_temp = now;//Delete operations for the PolyNode
		now = now->next;
		delete new_temp;
	}
} 
	

PolyNode* AddNode(PolyNode *head, double coef, int exp){
	if (head == NULL) {
		head = new PolyNode{ coef, exp, 0 };
		return head;
	}
	PolyNode* it = head;
	PolyNode* last = NULL;
	while (it != NULL && it->exp > exp) {//Controlling of the variable's coefficient for correct point
		last = it;
		it = it->next;
	}
	if (it != NULL && it->exp == exp) {
		it->coef += coef;	//If we have a new variable to same exponential value, sum it
		if (it->coef == NULL) {
			if (last == NULL) {
				PolyNode* temp = head;
				head = head->next;
				delete temp;
			}
			else {
				last->next = it->next;
				delete it;
			}
		}
	}
	else {
		PolyNode* new_n = new PolyNode{ coef, exp, it };	//Creating new node
		if (last == NULL) {	//Desicion of first node for deleting or not
			head = new_n;
		}
		else {
			last->next = new_n;
		}
	}
	return head;
} 


PolyNode *Add(PolyNode *poly1, PolyNode *poly2){	//Addition Operations
	PolyNode* resultant = 0;
	PolyNode* p_1 = poly1;
	PolyNode* p_2 = poly2;
	while (p_1 != 0 || p_2 != 0) {
		if (p_1 != 0 && (p_2 == 0 || p_1->exp > p_2->exp)) {
			resultant = AddNode(resultant, p_1->coef, p_1->exp);
			p_1 = p_1->next;
		}
		else if (p_2 != 0 && (p_1 == 0 || p_2->exp > p_1->exp)) {
			resultant = AddNode(resultant, p_2->coef, p_2->exp);
			p_2 = p_2->next;
		}
		else {
			resultant = AddNode(resultant, p_1->coef + p_2->coef, p_1->exp);
			p_1 = p_1->next;
			p_2 = p_2->next;
		}
	}
	return resultant;
} 


PolyNode *Subtract(PolyNode *poly1, PolyNode *poly2){	//Substraction Operations
	PolyNode* negative_Poly2 = 0;
	PolyNode* p_2 = poly2;
	while (p_2 != 0) {
		negative_Poly2 = AddNode(negative_Poly2, -p_2->coef, p_2->exp);
		p_2 = p_2->next;
	}
	PolyNode* q = Add(poly1, negative_Poly2);
	DeletePoly(negative_Poly2);
	return q;
} 


PolyNode *Multiply(PolyNode *poly1, PolyNode *poly2){	//Multiplication Operations
	PolyNode* resultant = 0;
	for (PolyNode* p = poly1; p != 0; p = p->next) {
		for (PolyNode* t = poly2; t != 0; t = t->next) {
			double new_c = p->coef * t->coef;
			int new_e = p->exp + t->exp;
			resultant = AddNode(resultant, new_c, new_e);
		}
	}
	return resultant;
} 


double Evaluate(PolyNode *poly, double x){	//Evaluation Operations
	double r = 0.0;
	PolyNode* now = poly;
	while (now != 0) {
		r += now->coef * pow(x, now->exp);
		now = now->next;
	}
	return r;
} 


PolyNode *Derivative(PolyNode *poly){	//Derivation Operations
	PolyNode* result_d = 0;
	PolyNode* now = poly;
	while (now != 0) {
		if (now->exp != 0) {
			double newCoef = now->coef * now->exp;
			int newExp = now->exp - 1;
			result_d = AddNode(result_d, newCoef, newExp);
		}
		now = now->next;
	}
	return result_d;
} 


void Plot(PolyNode *poly, int x1, int x2){	//Plot Operations

	int y, x;
	double t;
	
	for (y = 12; y >= -12; y--) { 
		for (x = x1; x <= x2; x++) { 
			t = Evaluate(poly, x);
			if (static_cast <int> (t) == y) {
				printf("*");
			}
			else if (y == NULL && x == NULL) {
				printf("+"); 
			}
			else if (y == NULL) {
				printf("-"); 
			}
			else if (x == 0) {
				printf("|"); 
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
} 
