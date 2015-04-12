#pragma once
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
 *	1. Lexer of the  breif compuler
 *	2. receive the input file in String
 *	3. output the token vector of input source code and the tokenTable
 *  4. another class "MatchTuple" to hold the temporay match result.
 */

class MatchTuple;

class Lexer {
public:
	typedef vector<Token> TOKEN_SEQUENCE;
	typedef vector<ListTuple> TOKEN_TABLE;
	typedef vector<char> SOURCE_STRING;

	void activate( SOURCE_STRING sourceInput );
	const TOKEN_SEQUENCE getResult();
	const TOKEN_TABLE getTokenTable();
	const vector<long> getErrorRecord();

	Lexer():tableIndex(-1), scanIndex(-1), currentDomain(0), maximumDomain(currentDomain), 
		errorFlag(false), lineCounter(1), EOFFlag(false){

	};
	~Lexer(){
	};
private:
	void initiateLexicon();//contains the info for reserved words

	long setTokenTable( MatchTuple );
	void writeResult( string classMacro, long tableIndex);
	long nextScanIndex();
	void resetIndexes();
	void updateScanIndex( bool matchConfirm, bool matchCom, MatchTuple );
	bool matchCoordination(bool , bool , bool , bool );
	bool hazardDealing();
	bool isRelevant(char);
	const char sourceFileEntry(long );

	bool matchReservedWord( MatchTuple &currentMatch );
	bool matchIdentifier( MatchTuple &currentMatch );
	bool matchOtherTerminater( MatchTuple &currentMatch, bool &);
	//contain special treatment on { and } to record domain number
	bool matchReservedWord_Aux( long );

	SOURCE_STRING sourceFile;
	TOKEN_SEQUENCE outputSequence;
	TOKEN_SEQUENCE tokenLexicon;
	TOKEN_TABLE tokenTable;
	vector<long> errorRecord;
	long tableIndex;//points to the last entry in token table
	long scanIndex;//points to the next entry of input,denote here, the NEXT ONE!
	long currentDomain, maximumDomain;
	bool errorFlag;
	bool EOFFlag;
	long lineCounter;
};


class MatchTuple{
public:
	string classMacro;
	
	string svalue;
	double dvalue;
	long length;

	MatchTuple(){};
	~MatchTuple(){};

};