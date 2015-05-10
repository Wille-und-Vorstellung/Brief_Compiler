#pragma once
#include"stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.19.2015
 * description: 
 *	1. 4-tuple that holds the object code sequence
 *	2. format(op, arg1, arg2, result);
 */

class Tuple4 {
public:
	string op;
	string arg1;
	string arg2;
	string result;

	Tuple4(){
	};
	~Tuple4(){
	};
private:


};