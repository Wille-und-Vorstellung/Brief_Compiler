#pragma once
#include"stdafx.h"

/*
 * author: Ruogu Gao
 * date: 4.9.2015
 * description: 
 *	1. 2-tuple for token storage/token encoding
 *	2. format:(classMacro, tableIndex) for token;(attribute, classMacro) for token encoding
 */
class Token{
public:
	//const long INVALID_INDEX = -1;

	string attribute;
	string classMarco;
	long tableIndex;

	Token& operator= ( const Token& );

	Token():tableIndex(-1){
	};
	Token(string at, string cl, long ta):tableIndex(-1){
		attribute = at;
		classMarco = cl;
		tableIndex = ta;
	};
	Token(const Token &input){
		attribute = input.attribute;
		classMarco = input.classMarco;
		tableIndex = input.tableIndex;
	};
	~Token();
private:

};


