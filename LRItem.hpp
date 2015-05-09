#include "stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.27.2015
 * description: 
 *	1. structure to hold the LR(1) item along with it's lookahead symbol
 *	2. containing:	the left and right of the producer: char vector<Token>
 *					dotposition : int
 *					lopkahead : Token
 *	3. currently there appears not to be the need of distinguishing empty producer
 *	4. overload the == and != operator
 *	5. only attribute 'classMacro ' in Token is used here.
 */

class LRItem {
public:
	int dotPosition;
	Token leftSide;
	vector<Token> rightSide;
	Token lookAhead;
	long semanticActionID;

	friend bool operator== ( const LRItem & , const LRItem & );
	friend bool operator!= ( const LRItem & , const LRItem &);

	LRItem():dotPosition(-1){};
	LRItem( const LRItem&  );
	~LRItem();

	
private:

};


//implements below
LRItem::LRItem( const LRItem& a ){
	leftSide = a.leftSide;
	rightSide = a.rightSide;
	dotPosition = a.dotPosition;
	lookAhead = a.lookAhead;
	semanticActionID  = a.semanticActionID;
};

bool operator== ( const LRItem& a, const LRItem& b ){
	if ( a.leftSide.classMarco != b.leftSide.classMarco || 
		a.dotPosition != b.dotPosition || 
		a.lookAhead.classMarco != b.lookAhead.classMarco ||
		a.rightSide.size() != b.rightSide.size() ){
		return false;
	}
	for (int i=0; i<a.rightSide.size(); i++){
		if ( a.rightSide[i].classMarco != b.rightSide[i].classMarco ){
			return false;
		}
	}
	return true;
};


bool operator!= ( const LRItem& a, const LRItem& b ){
	return !( a == b );
}