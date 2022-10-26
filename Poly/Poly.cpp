#include <stdio.h>
#include "Poly.h"

//-------------------------------------------------
// Creates a polynomial from an expression.
// Returns a pointer to the first PolyNode in the list (head of the list)
//
// Expression will be of the following sort:
// Ex1: 2.3x^4 + 5x^3 - 2.64x - 4
// Ex2: -4.555x^10 - 45.44
// Ex3: x^6 + 2x^4 - x^3 - 6.3x + 4.223
// Ex4: 34
// Ex5: -x+1
// Ex6: -3x^4    +   4x
// Ex7: -2x  - 5
//
PolyNode *CreatePoly(char *expr)
{
	PolyNode *newPoly = new PolyNode();
	PolyNode *head = newPoly;

	std::string exprTemp;
	for (int i = 0; expr[i] != '\0'; i++)
		if (expr[i] != ' ')
			exprTemp.push_back(expr[i]);

	for (int i = 0; i < exprTemp.size(); i++)
	{
		std::string coef;

		while (exprTemp[i] != 'x' && exprTemp[i] != '\0')
			coef.push_back(exprTemp[i]), i++;
		// Calculates coef
		if (coef[0] == '-' && coef.size() == 1)
			newPoly->coef = -1.0;
		else if (coef.empty() || (coef[0] == '+' && coef.size() == 1))
			newPoly->coef = 1;
		else
			newPoly->coef = stod(coef);

		coef = "";

		std::string exp;

		newPoly->exp = 0;

		if (!(exprTemp[i] == '\0'))
		{

			i++;
			if (exprTemp[i] == '^')
			{
				i++;
				while (exprTemp[i] != '+' && exprTemp[i] != '-' && exprTemp[i] != '\0')
					exp.push_back(exprTemp[i]), i++;
				newPoly->exp = stoi(exp);
			}
			else if (exprTemp[i - 1] == 'x')
				newPoly->exp = 1;

			exp = "";
		}
		if (exprTemp[i] == '-' || exprTemp[i] == '+')
		{
			PolyNode *next = new PolyNode();
			newPoly->next = next;
			newPoly = newPoly->next;
			i--;
		}
	}

	return head;
} // end-CreatePoly

/// -------------------------------------------------
/// Walk over the poly nodes & delete them
///
void DeletePoly(PolyNode *poly)
{

	while (poly != NULL)
	{
		PolyNode *next = (PolyNode *)malloc(sizeof(PolyNode));
		next = poly;
		poly = poly->next;
		delete next;
	}

} // end-DeletePoly

//-------------------------------------------------
// Adds a node (coefficient, exponent) to poly. If there already
// is a node with the same exponent, then you simply add the coefficient
// to the existing coefficient. If not, you add a new node to polynomial
// Returns a pointer to the possibly new head of the polynomial.
//
PolyNode *AddNode(PolyNode *head, double coef, int exp)
{
	PolyNode *next = new PolyNode();
	PolyNode *ptr = new PolyNode();
	ptr = head;
	next = head;
	if (head == NULL)
	{
		PolyNode *newPoly = new PolyNode();
		newPoly->coef = coef, newPoly->exp = exp;
		return newPoly;
	}
	else
	{
		if (next->exp < exp)
		{
			PolyNode *newPoly = new PolyNode();
			newPoly->coef = coef, newPoly->exp = exp, newPoly->next = next;
			return newPoly;
		}
		else if (next->exp == exp)
		{
			next->coef += coef;
			if (next->coef == 0)
			{
				delete next;
				return head->next;
			}
		}
		else
			while (head != NULL)
			{
				if (next->next == NULL)
				{
					PolyNode *newPoly = new PolyNode();
					newPoly->coef = coef, newPoly->exp = exp, next->next = newPoly;
					break;
				}
				else if (next->next->exp == exp)
				{
					next->next->coef += coef;
					if (next->next->coef == 0)
					{
						PolyNode *nextTemp = new PolyNode();
						nextTemp = next->next;
						next->next = next->next->next;
						delete nextTemp;
					}
					break;
				}
				else if (next->next->exp < exp)
				{
					PolyNode *newPoly = new PolyNode();
					newPoly->coef = coef, newPoly->exp = exp,
					newPoly->next = next->next, next->next = newPoly;
					break;
				}

				head = head->next;
				next = head;
			}
	}

	return ptr;
} // end-AddNode

//-------------------------------------------------
// Adds two polynomials and returns a new polynomial that contains the result
// Computes: poly3 = poly1 + poly2 and returns poly3
//
PolyNode *Add(PolyNode *poly1, PolyNode *poly2)
{
	PolyNode *poly3 = new PolyNode();

	while (poly1 != NULL)
	{
		poly3 = AddNode(poly3, poly1->coef, poly1->exp);
		poly1 = poly1->next;
	}

	while (poly2 != NULL)
	{
		poly3 = AddNode(poly3, poly2->coef, poly2->exp);
		poly2 = poly2->next;
	}

	return poly3;
} // end-Add

//-------------------------------------------------
// Subtracts poly2 from poly1 and returns the resulting polynomial
// Computes: poly3 = poly1 - poly2 and returns poly3
//
PolyNode *Subtract(PolyNode *poly1, PolyNode *poly2)
{
	PolyNode *poly3 = new PolyNode();

	while (poly1 != NULL)
	{
		poly3 = AddNode(poly3, poly1->coef, poly1->exp);
		poly1 = poly1->next;
	}

	while (poly2 != NULL)
	{
		poly3 = AddNode(poly3, -poly2->coef, poly2->exp);
		poly2 = poly2->next;
	}

	return poly3;
} // end-Substract

//-------------------------------------------------
// Multiplies poly1 and poly2 and returns the resulting polynomial
// Computes: poly3 = poly1 * poly2 and returns poly3
//
PolyNode *Multiply(PolyNode *poly1, PolyNode *poly2)
{
	PolyNode *poly3 = new PolyNode();
	PolyNode *poly2Temp = new PolyNode();
	poly2Temp = poly2;
	while (poly1 != NULL)
	{
		while (poly2 != NULL)
		{
			poly3 = AddNode(poly3, poly1->coef * poly2->coef, poly1->exp + poly2->exp);
			poly2 = poly2->next;
		}
		poly2 = poly2Temp;
		poly1 = poly1->next;
	}

	return poly3;
} // end-Multiply

//-------------------------------------------------
// Evaluates the polynomial at a particular "x" value and returns the result
//
double Evaluate(PolyNode *poly, double x)
{
	double result = 0;
	int xTemp = 1;

	while (poly != NULL)
	{
		for (int i = 0; i < poly->exp; i++)
		{
			xTemp *= x;
		}
		result += poly->coef * xTemp;
		poly = poly->next;
		xTemp = 1;
	}

	return result;
} // end-Evaluate

//-------------------------------------------------
// Computes the derivative of the polynomial and returns it
// Ex: poly(x) = 3x^4 - 2x + 1-->Derivative(poly) = 12x^3 - 2
//
PolyNode *Derivative(PolyNode *poly)
{
	PolyNode *head = new PolyNode();
	head = poly;

	while (poly != NULL)
	{
		if (poly->next->exp == 0)
			delete poly->next, poly->next = NULL;

		poly->coef *= poly->exp;
		poly->exp -= 1;
		poly = poly->next;
	}

	return head;
} // end-Derivative

//-------------------------------------------------
// Plots the polynomial in the range [x1, x2].
// -39<=x1<x2<=39
// -12<=y<=12
// On the middle of the screen you gotta have x & y axis plotted
// During evaluation, if "y" value does not fit on the screen,
// then just skip it. Otherwise put a '*' char depicting the curve
//
void Plot(PolyNode *poly, int x1, int x2)
{
	for (int i = x1; i < 0; i++)
		std::cout << " ";
	std::cout << "y" << std::endl;
	for (int i = 12; i >= -12; i--)
	{
		for (int j = x1 - 1; j <= x2; j++)
		{
			if (j == x1 - 1 && i == 0)
				std::cout << "x";
			else if (j == x1 - 1)
				std::cout << " ";

			if (!(j == x1 - 1))
			{
				if (j == 0 && i != 0)
					std::cout << "|";
				else if (j == 0 && i == 0)
					std::cout << "+";
				else if (j != 0 && i == 0)
					std::cout << "-";
				else if ((int)Evaluate(poly, j) == i)
					std::cout << "*";
				else
					std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
} // end-Plot
