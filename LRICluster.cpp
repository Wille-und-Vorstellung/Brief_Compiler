#pragma once
#include "stdafx.h"
#include "LRICluster.hpp"

//implements
LRICluster::LRICluster(const LRICluster& a){
	ID = a.ID;
	LRIs = a.LRIs;
	intrigers = a.intrigers;
	dest = a.dest;
}

bool LRICluster::containLRI(LRItem& a){
	for (int i = 0; i < LRIs.size(); i++){
		if (LRIs[i] == a){
			return true;
		}
	}
	return false;
}

void LRICluster::pushBack(LRItem a){
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
		if (intrigers[i].classMarco == t.classMarco){
			*index = i;
			return true;
		}
	}
	return false;
}