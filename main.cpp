#include  "stdafx.h"
#include  "BriefCompiler.hpp"

/*
 * author: Ruogu Gao
 * date: 4.11.2015
 * description: 
 *	1. main test frame for the brief compiler
 *	2. 
 */


int _tmain(){
	//fibericating 
	BriefCompiler testCompiller;
	vector<ListTuple> lexResult;
	vector<Token> tokenTable;
	lexResult.clear();
	tokenTable.clear();

	string sourceFilePath("../testSource.txt");
	testCompiller.activateCompiler(sourceFilePath);

	cout << "the Lexer output:" << endl;
	for ( unsigned int i=0; i < testCompiller.getLexerResult().size(); i++ ){
		cout << "< " << testCompiller.getLexerResult()[i].classMarco << ", " << testCompiller.getLexerResult()[i].tableIndex << " >" << endl;
	}
	cout << "------------------------" << endl;
	cout << "the token table:" << endl;
	for ( unsigned int j=0; j<testCompiller.getTokenTable().size(); j++ ){
		cout << j << "\'th row: " << testCompiller.getTokenTable()[j].classMacro << " | " << testCompiller.getTokenTable()[j].dvalue << " | " << testCompiller.getTokenTable()[j].svalue << endl;
	}

	cout << "about to end" << endl;
	
	return 117;
}