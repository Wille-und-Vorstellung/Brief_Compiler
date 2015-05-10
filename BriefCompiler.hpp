#pragma once
#include"stdafx.h"
#include<iostream>
#include"SynSemantic.hpp"
using std::string;

/*
 * author: Ruogu Gao
 * date: 4.9.2015 - 
 * description: 
 *	1. main framework and the interface of the brief compiler.
 *	2. basic modules: sourceReader, Lexer, 
 */

class BriefCompiler{
public:
	typedef vector<Token> TOKEN_SEQUENCE;
	typedef vector<ListTuple> TOKEN_TABLE;
	typedef vector<Tuple4> OBJECT_CODE_SEQUENCE;

	void activateCompiler( string fileName );//compiled  code will be write to file
	const vector<ListTuple> getTokenTable();
	const vector<Token> getLexerResult();
	const vector<Tuple4> getObjectCode();
	const vector<LRItem> getReducedLRItem();
	const vector<long> getSSErrorRecord();
	
	BriefCompiler(){

	};
	~BriefCompiler(){
	};
private:

	vector<char> readSource(string fileName);
	TOKEN_SEQUENCE activateLexer(vector<char> sourceString);
	OBJECT_CODE_SEQUENCE activateSynSemantic( TOKEN_SEQUENCE inputSeuence );


	OBJECT_CODE_SEQUENCE objectCode;
	TOKEN_TABLE tokenTable;
	Lexer LexAnalyser;
	SynSemantic SSAnalyser;
};