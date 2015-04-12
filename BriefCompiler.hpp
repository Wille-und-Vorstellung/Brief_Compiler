#include"stdafx.h"
#include<iostream>
using std::string;

/*
 * author: Ruogu Gao
 * date: 4.9.2015
 * description: 
 *	1. main framework and the interface of the brief compiler.
 *	2. basic modules: sourceReader, Lexer, 
 */

class BriefCompiler{
public:
	typedef vector<Token> TOKEN_SEQUENCE;
	typedef vector<ListTuple> TOKEN_TABLE;

	void activateCompiler( string fileName );//compiled  code will be write to file
	const vector<ListTuple> getTokenTable();
	const vector<Token> getLexerResult();

	BriefCompiler(){

	};
	~BriefCompiler(){
	};
private:

	vector<char> readSource(string fileName);
	TOKEN_SEQUENCE activateLexer(vector<char> sourceString);


	TOKEN_TABLE tokenTable;
	Lexer LexAnalyser;
};