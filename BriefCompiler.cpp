#include "stdafx.h"
#include"BriefCompiler.hpp"
#define MAXIMUM_FILE_LENGTH 30
#define TEST_ONLY

//--------------------------Lex analysis part
void BriefCompiler::activateCompiler( string fileName ){
	//error-check
	vector<char> inputString = readSource( fileName );
	//error-check
	TOKEN_SEQUENCE lexerResult;
	lexerResult = activateLexer( inputString );
#ifdef TEST_ONLY
	cout << "the Lexer output:" << endl;
	for (unsigned int i = 0; i < lexerResult.size(); i++){
		cout << "< " << lexerResult[i].classMarco << ", " << lexerResult[i].tableIndex << " >" << endl;
	}
	cout << "------------------------" << endl;
#endif
	//error-check
	OBJECT_CODE_SEQUENCE SSResult;
	SSResult = activateSynSemantic( lexerResult );
	objectCode = SSResult;
	//error-check
	//...
	return;
};

const BriefCompiler::TOKEN_TABLE BriefCompiler::getTokenTable(){
	tokenTable = LexAnalyser.getTokenTable();
	return tokenTable;
};

const BriefCompiler::TOKEN_SEQUENCE BriefCompiler::getLexerResult(){
	return LexAnalyser.getResult();
};

vector<char> BriefCompiler::readSource(string fileName){
	//boundary-check
	if ( (unsigned int)fileName.size() > MAXIMUM_FILE_LENGTH){
		cout << "file name is  too long." << endl;
		exit(00);
	}
	ifstream source( fileName.c_str() );
	if ( !source ){
		cout << "error: fail to open the source file" << endl;
		exit(01);
	}
	vector<char> sourceString;
	char temp;
	while ( source >> temp ) {
		sourceString.push_back(temp);
	}
	return sourceString;
};

BriefCompiler::TOKEN_SEQUENCE BriefCompiler::activateLexer(vector<char> sourceString){
	BriefCompiler::TOKEN_SEQUENCE result;
	LexAnalyser.activate( sourceString );	
	result = LexAnalyser.getResult();
	return result;
};
//---------------------------syntax & semantic part below
const BriefCompiler::OBJECT_CODE_SEQUENCE BriefCompiler::getObjectCode(){
	return objectCode;
};

BriefCompiler::OBJECT_CODE_SEQUENCE BriefCompiler::activateSynSemantic( BriefCompiler::TOKEN_SEQUENCE inputTokenSequence ){
	BriefCompiler::OBJECT_CODE_SEQUENCE temp;
	SSAnalyser.activate( inputTokenSequence, LexAnalyser.getTokenLexicon() );
	temp = SSAnalyser.getObjectCode();
	//error-check
	objectCode = temp;
	return objectCode;
};

const vector<LRItem> BriefCompiler::getReducedLRItem(){
	return SSAnalyser.getReducedLRItem();
};

const vector<long> BriefCompiler::getSSErrorRecord(){
	return SSAnalyser.getErrorRecord();
};