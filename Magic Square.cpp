/*
* Magic Square.cpp : a programe that fills numbers in a given range into
* a square so that square becomes a magic square.
* created on: 9/2/2015 - Mon.
* by: Bassam Yassin
* thunderwiring.wordpress.com 
*/
#include "stdafx.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;
/*****************************************************************/
/*exception classes:*/
class invalidDimntions {}; /*if the data size was less/equal 0*/
class indexOutOfrange  {}; /*if neagtive index wanted in operator()*/
class rangeTooShort    {}; /*if the range of numbers to fill in data-
						    *is less than the dimention specified*/
/*****************************************************************/
/*DEFINES:*/
const int EMPTY = 0;
const int OFFSET = 3;
const int LOW_BOUND = 0;
const int START_POSITION = 0;
const int NEXT = 1;

const char SEPARATE = '\t';
const char YES = 'y';
const char NO = 'n';
/*****************************************************************/
/*typedefs:*/
typedef int* Data;
/*****************************************************************/
/*
 * The object of the class holds the data in a grid implemented by
 * 2D array.
 */
class Square {
private:
	int dim, bottomRange, topRange, initialValue;
	Data data;
	bool checkRowsAndColumns() const;
	bool checkDiagnals() const;
	const Data getData() const;
	int initValue() const;
	int size() const;
	int dimention() const;
	bool checkValid() const;
	bool valueUsed(int num) const;
	int getEmptyCellNumber() const;
	bool aux_solve(int position);
	bool checkCurrentElements(int currentPosition) const;
	int getEqualSum() const;
public:
	/*class constructor:*/
	Square(int dim, int bottom = EMPTY) :
	  dim(dim), bottomRange(bottom) {		  
		if(dim <= 0) {
			throw invalidDimntions();			
		} 		
		/* this guards the program from empty range*/
		if(bottomRange == EMPTY) {			
			bottomRange = 1;
			topRange = dim * dim;
		} else {
			topRange = bottomRange + (dim * dim) - 1;
		}		
		data = new int[dim*dim];
		initialValue = bottomRange - OFFSET;		
		for(int i = 0; i < (dim*dim); ++i) {
			data[i] = initialValue;			
		}	
	}
	/*class destructor:*/
	~Square() {
		delete data;
	  }
	/*class functions:*/		
	void print() const;
	void solve();	
}; /*end class Square*/
/*****************************************************************/
/* Implementation of calss 'Square' private methods:*/
/*****************************************************************/
/*
* returns the value that equals to the sum of numbers in each
* row, column and both diagonals
*/
int Square::getEqualSum() const {
	int sum(EMPTY);
	for(int i = bottomRange; i <= topRange; ++i) {
		sum += i;
	}
	return (sum / dim);
}
/*****************************************************************/
/*
* returns true if the passed value is already in the square
*/
bool Square::valueUsed(int num) const {
	for(int i = 0; i < size(); ++i) {
		if(data[i] == num) {
			return true;
		}
	}
	return false;
}
/*****************************************************************/
/*
* getEmptyCellNumber - private:
* returns the number of cells that still has NOT been filled with-
* numbers from the given range(set at initialization)
*/
int Square::getEmptyCellNumber() const {
	int count(0);
	for(int i = 0; i < size(); ++i) {		
			if(data[i] == initialValue) {
				count++;
			}		
	}	
	return count;
}
/*****************************************************************/
/*
* this method is used to cut-off branches from te recursion calls.
* It checks if the current filled rows fullfill equallibrium, means-
* sum of number in each filled row is equal to 'getEqualSum()'
*/
bool Square::checkCurrentElements(int currentPosition) const {	
	int sumRow(EMPTY);
	int filledRows = (currentPosition + 1) / dim;	
	for(int i = 0; i < filledRows; ++i) {
		for(int j = i * dim; j < ((i * dim) + dim); ++j) {
			sumRow += data[j];			
		}
		if(sumRow != getEqualSum()) {
			return false;
		}
		sumRow = EMPTY;
	}			
	return true;
}
/*****************************************************************/
/*
* checks whether the sum of numbers in all rows are the same and 
* in all columns.
*/
bool Square::checkRowsAndColumns() const {
	int sumRow(EMPTY), sumCol(EMPTY);	
	for(int column = 0; column < dim; ++column) {
		for(int row = 0; row < dim; ++row) {
			sumRow += data[column + row * dim];
			sumCol += data[row + column * dim];			
		}
		if((sumRow != getEqualSum()) || (sumCol != getEqualSum())) {	
			//cout << "false\n";
			return false;
		}
		sumRow = EMPTY;
		sumCol = EMPTY;
	}
	//cout << "true\n";
	return true;
}
/*****************************************************************/
/*
* checks whether the sum of numbers in both diagonals are the same
*/
bool Square::checkDiagnals() const {
	int sumMainDiag(EMPTY), sumSecondryDiag(EMPTY);
	int refMainDiag(initialValue),
		refSecondryDiag(initialValue);
	for(int i = 0; i < dim; ++i) {
		sumMainDiag += data[i + i * (dim)];
		sumSecondryDiag += data[(dim -1)*(i+1)];
	}	
	return ((sumMainDiag == getEqualSum()) && 
			(sumSecondryDiag == getEqualSum()));
}
/*****************************************************************/
/* Implementation of calss 'Square' functions:*/
/*****************************************************************/
const Data Square::getData() const {
	return this->data;
}
/*****************************************************************/
int Square::initValue() const {
	return this->initialValue;
}
/*****************************************************************/
int Square::size() const {
	return (this->dim) * (this->dim);
}
/*****************************************************************/
int Square::dimention() const {
	return this->dim;
}
/*****************************************************************/
/*
* return 'true' if the sum of numbers in all ROWS and all COLUMNS 
* and both DIAGONALS are equal, else, return 'false'
*/
bool Square::checkValid() const {
	return (checkRowsAndColumns() && checkDiagnals());
}
/*****************************************************************/
/*
* print the contents of the squre in a matrix format
*/
void Square::print() const {
	for(int i = 0; i < dim; ++i) {
		for(int j = 0; j < dim; ++j) {
			cout << data[i * dim + j] << SEPARATE;
		}
		cout << endl;
	}
}
/*****************************************************************/
void Square::solve() {
	if(aux_solve(START_POSITION)) {		
		cout << "The solved square:" << endl;
		print();
	} else {
		cout << "Cannot solve the square with the given range." << endl;
		cout << "Please try again, later!" << endl;		
	}
	cout << "***********************************************" << endl;
}
/*****************************************************************/
/*
* aux_solve - private:
* An auxillary method for the public class function 'solve()'
* that recursively filles the data container with-
* values from a given pre-set range of values in an order so that we 
* get a magic squre of numbers.
*/
bool Square::aux_solve(int position) {	
	if(getEmptyCellNumber() == EMPTY) {
		return checkValid();
	}
	for(int i = bottomRange; i <= topRange; ++i) {
		if(valueUsed(i)) {
			continue;
		}
		data[position] = i;		
		if(position >= (dim - 1)) {
			if(checkCurrentElements(position) == false) {
				data[position] = initialValue;
				continue;
			}
		} 
		if(aux_solve(position + NEXT)) {
			return true;
		}
		data[position] = initialValue;			
	}
	data[position] = initialValue;
	return false;
}
/*****************************************************************/
int main()
{
	int dim(EMPTY), bottom(EMPTY);
	char answer(EMPTY);
	cout << "***********************************************" << endl;
	cout << "custom run? (y/n) ";
	cin >> answer;
	if(answer == NO) {
		dim = 3;
		cout << "***********************************************" << endl;
		Square sqr = Square(dim,bottom);
		sqr.solve();
		return 0;
	}
	cout  << endl;
	cout << "square dimention(positive non-zer): ";
	cin >> dim;
	cout  << endl;
	cout << "choose value range? (y/n) ";
	cin >> answer;
	cout  << endl;
	if(answer == YES) {
		cout << "start at: ";
		cin >> bottom;
		cout  << endl;
	}
	cout << "***********************************************" << endl;
	cout << "solving..." << endl;
	Square sqr = Square(dim,bottom);
	sqr.solve();
	return 0;
}

