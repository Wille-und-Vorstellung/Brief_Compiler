#pragma once
#include "stdafx.h"
#include "LRItem.hpp"

//implements below
LRItem::LRItem(const LRItem& a){
	leftSide = a.leftSide;
	rightSide = a.rightSide;
	dotPosition = a.dotPosition;
	lookAhead = a.lookAhead;
	semanticActionID = a.semanticActionID;
};

LRItem::LRItem(int dot, Token ls, vector<Token> rs, Token la, long said){
	dotPosition = dot;
	leftSide = ls;
	rightSide = rs;
	lookAhead = la;
	semanticActionID = said;
};

bool operator== (const LRItem& a, const LRItem& b){
	if (a.leftSide.classMarco != b.leftSide.classMarco ||
		a.dotPosition != b.dotPosition ||
		a.lookAhead.classMarco != b.lookAhead.classMarco ||
		a.rightSide.size() != b.rightSide.size()){
		return false;
	}
	for (int i = 0; i<a.rightSide.size(); i++){
		if (a.rightSide[i].classMarco != b.rightSide[i].classMarco){
			return false;
		}
	}
	return true;
};


bool operator!= (const LRItem& a, const LRItem& b){
	return !(a == b);
}