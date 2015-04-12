#include"stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.9.2015
 * description: 
 *	1. tuple in the tokenTable
 *	2. format(classMacro, dvalue, svalue, domain)
 */

class ListTuple{
public:
	//const double INVALID_VALUE = 1;
	//const long INVALID_DOMAIN = -1;

	string classMacro;
	double dvalue;
	string svalue;
	long domain;

	ListTuple():dvalue(-1), domain(-1){
	};
	ListTuple(string cl, double dv, string sv, long idomain){
		classMacro = cl; 
		dvalue = dv; 
		svalue = sv;
		domain = idomain;
	};
	ListTuple(const ListTuple &input){
		classMacro = input.classMacro;
		dvalue = input.dvalue;
		svalue = input.svalue;
		domain = input.domain;
	};
	~ListTuple(){
	};
private:
	

};