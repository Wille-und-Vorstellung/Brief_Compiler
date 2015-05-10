#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "LRItem.hpp"

using std::vector;
using std::string;

/*
* author: Ruogu Gao
* date: 5.9.2015
* description:
*	1. structure to hold the LR(1) item cluster in the LR(1) DFA
*	2. containing:	cluster id
*					vector of LRItem
*/

class LRICluster {
public:
	long ID;
	vector<LRItem> LRIs;
	vector<Token> intrigers;
	vector<long> dest;

	//LRItem getLRI();
	bool containLRI( LRItem& );
	void pushBack( LRItem );
	long size();
	LRItem get(long);
	bool getTransfer( Token , long* );

	LRICluster():ID(0){
		LRIs.clear();
		intrigers.clear();
		dest.clear();
	};
	LRICluster( const LRICluster& );
	~LRICluster(){};
private:

};

LRICluster::LRICluster(const LRICluster& a){
	ID = a.ID;
	LRIs = a.LRIs;
	intrigers = a.intrigers;
	dest = a.dest;
}

bool LRICluster::containLRI( LRItem& a ){
	for (int i = 0; i < LRIs.size(); i++){
		if (LRIs[i] == a ){
			return true;
		}
	}
	return false;
}

void LRICluster::pushBack( LRItem a ){
	LRIs.push_back(a);
}

long LRICluster::size(){
	return LRIs.size();
}

LRItem LRICluster::get(long i){
	return LRIs[i];
}

bool LRICluster::getTransfer(Token t, long* index){
	for (int i = 0; i < intrigers.size(); i++){
		if (intrigers[i].classMarco == t.classMarco ){
			*index = i;
			return true;
		}
	}
	return false;
}