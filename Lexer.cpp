#pragma once
#include "stdafx.h"
#include"Lexer.hpp"
#define RESERVED_WORD_RANGE 24

void Lexer::activate( SOURCE_STRING sourceInput){
	//check input
	if ( sourceInput.empty() ){
		cout << "error: empty input file." << endl;
		return;
	}

	//initiate scan , tokentable and lexicon
	resetIndexes();
	initiateLexicon();
	sourceFile = sourceInput;

	//start scan input source
	bool matchRW, matchID, matchOT, matchCom, matchConfirm;
	long tempIndex;
	MatchTuple currentMatch;
	matchRW = matchID = matchOT = matchCom = false;
	matchConfirm = false;
	while ( nextScanIndex() < (long)sourceFile.size() ){
		//dubugging
		char currentChar = sourceFile[scanIndex];
		//jump through any irrelevant character
		if ( !isRelevant( sourceFile[scanIndex] ) ){
			if ( sourceFile[scanIndex] == '\n' ){
				lineCounter++;
			}
			scanIndex++;
			continue;
		}
		//Here comes the interesting matching part
		matchRW = matchReservedWord( currentMatch );
		if ( !matchRW ){//ad-hoc
			matchID = matchIdentifier( currentMatch );
		}
		matchOT = matchOtherTerminater( currentMatch, matchCom );//domain number may change here

	
		//set tokenTable and outputSequence
		matchConfirm = matchCoordination( matchRW, matchID, matchOT, matchCom );
		if ( matchConfirm ){//correctly matched indeed
			tempIndex = setTokenTable( currentMatch);
			writeResult( currentMatch.classMacro, tempIndex );
		}		

		//update scan index and counter-hazard measure
		updateScanIndex( matchConfirm, matchCom, currentMatch );
		//debugging
		outputSequence.size();
		tokenTable.size();
		//reset states
		matchRW = matchID = matchOT = matchCom = false;
	};
	return;
};

//nasty part 
bool Lexer::matchReservedWord( MatchTuple &currentMatch ){
	long tempIndex = scanIndex;
	bool tempFlag = false;
	unsigned int i=0;
	char checkPoint = NULL;
	if ( !isdigit( sourceFile[tempIndex] ) ) {
		for (i=0; i < tokenLexicon.size(); i++){
			tempFlag = matchReservedWord_Aux(i);
			if ( tempFlag )
				break;
		}
		if ( tempFlag ){//a reserved word spotted, might be a prefix though notice that it could also just be an operator like '('
			//check if it's prefix of an identifier
			//ad-hoc start
			//measures in matchReservedWord_Aux prevents (tempIndex + tokenLexicon[i].attribute.size() -1 < sourceFile.size() )from happening given tempFlag == true
			if ( tempIndex + tokenLexicon[i].attribute.size() < sourceFile.size() ) {//need to check prefix
				checkPoint = sourceFile[tempIndex + tokenLexicon[i].attribute.size()];
				if ( i <= RESERVED_WORD_RANGE && !isdigit(checkPoint) && !isalpha(checkPoint) ){//is reserved word and is not prefix: all reserved word index are between 0-13 in tokenLexicon
					//construct currentMatch
					currentMatch.classMacro = tokenLexicon[i].classMarco;
					currentMatch.length = tokenLexicon[i].attribute.size();
					currentMatch.dvalue = -1;
					currentMatch.svalue = "";
				}
				else if ( i > RESERVED_WORD_RANGE ){//operator spotted
					currentMatch.classMacro = tokenLexicon[i].classMarco;
					currentMatch.length = tokenLexicon[i].attribute.size();
					currentMatch.dvalue = -1;
					currentMatch.svalue = "";
				}
				else
					tempFlag = false;
			}
			else { //tempIndex + tokenLexicon[i].attribute.size() == sourceFile.size() + 1, thus no need to check prefix
				currentMatch.classMacro = tokenLexicon[i].classMarco;
				currentMatch.length = tokenLexicon[i].attribute.size();
				currentMatch.dvalue = -1;
				currentMatch.svalue = "";
			}
			
			
		}
	}
	if (tempFlag) 
		return true;
	return false;
};

//match one single reserved word given it's lexicon index
//return true if matched
bool Lexer::matchReservedWord_Aux( long index ){
	bool temp = true;
	//if the coresponding tokenLexicon entry's attribute is ""(ie nothinig) , should return false;
	if (tokenLexicon[index].attribute.size() == 0){
		return false;
	}
	//transform string into vector<char>
	vector<char> targetString;
	for ( unsigned int i=0; i<tokenLexicon[index].attribute.size(); i++ ){
		targetString.push_back(tokenLexicon[index].attribute[i]);//potential hazard: tranform string to char
	}
	//match
	//check if reached the end fo file
	if ( (unsigned long)(scanIndex + targetString.size() -1) > sourceFile.size() ){//the target word breached the end of file
		return false;
	}
	for ( unsigned int i=0; i<targetString.size(); i++ ){
		if ( targetString[i] != sourceFile[scanIndex + i] ){
			temp = false;
			break;
		}
	}
	return temp;
}

//nasty part 
bool Lexer::matchIdentifier( MatchTuple &currentMatch ){
	long tempIndex = scanIndex;
	int i=0;

	if ( isalpha( sourceFile[tempIndex] ) ){
		while ( tempIndex + i < sourceFile.size() && 
			isalpha( sourceFile[tempIndex+i]) || isdigit( sourceFile[tempIndex+i] ) 
			|| sourceFile[tempIndex+i] == '_' ){
			i++;
		}
		//construct currentmatch
		currentMatch.classMacro = "ID";
		currentMatch.length = i;
		currentMatch.dvalue = -1;
		currentMatch.svalue = "";
		return true;
	}
	return false;
};

//nasty part 
/*  recognize comment and constants
 *	once comments spotted, return true, set matchCom = true
 *	other token spotted, return true, matchCom = false
 *	otherwise, return false, matchCom = false
 */
bool Lexer::matchOtherTerminater( MatchTuple &currentMatch, bool &matchCom ){
	long tempIndex = scanIndex;
	//recognize commments
	if ( sourceFile[tempIndex] == '/' && sourceFile[tempIndex+1] == '*'){
		matchCom = true;
		return true;
	}
	//recognize string constant
	else if ( sourceFile[tempIndex] == '"' ){
		int i=1;
		while ( tempIndex + i < sourceFile.size() &&
			sourceFile[tempIndex+i] != '"' && i <= 117 ){//string constant maximum length allowed: 117
			i++;
		}
		if ( i > 117 ||	//if breached the ebd of the file, then tempIndex + i has to be sourceFile.size()
			(tempIndex + i >= sourceFile.size() && sourceFile[tempIndex+i-1] != '"') ){
			errorFlag = true;
			return false;
		}
		//construct currentMatch
		currentMatch.classMacro = "STRINGC";
		currentMatch.length = i;
		currentMatch.dvalue = -1;
		currentMatch.svalue = "";
		for (int j=1; j<i; j++){
			currentMatch.svalue.push_back( sourceFile[tempIndex+j] );
		}
		return true;
	}
	//recognize int and float constant
	else if ( isdigit( sourceFile[tempIndex] ) ){
		int k=1;
		while ( tempIndex+k < sourceFile.size() &&
			isdigit(sourceFile[tempIndex+k]) ){
			k++;
		}
		//end of the file check
		if ( tempIndex+k < sourceFile.size() && sourceFile[tempIndex+k] == '.' ){//float
			
			while ( tempIndex+k < sourceFile.size() && 
				isdigit(sourceFile[tempIndex+k+1]) ){
				k++;
			}
			k++;
			currentMatch.classMacro = "REALC";
		}
		else{//int
			currentMatch.classMacro = "INTC";
		}
		currentMatch.length = k;
		currentMatch.dvalue = -1;
		currentMatch.svalue = "";
		for (int j=0; j<k; j++){
			currentMatch.svalue.push_back( sourceFile[tempIndex+j] );
		}
		
		return true;
	}
	return false;
};

const Lexer::TOKEN_SEQUENCE Lexer::getResult(){
	return outputSequence;	
};

const Lexer::TOKEN_TABLE Lexer::getTokenTable(){
	return tokenTable;
};

const vector<long> Lexer::getErrorRecord(){
	return errorRecord;	
};

long Lexer::setTokenTable( MatchTuple cMatch ){
	ListTuple temp(cMatch.classMacro, -1, "", currentDomain);
	
	//constant
	if ( cMatch.classMacro == "REALC" || cMatch.classMacro == "STRINGC" || cMatch.classMacro == "INTC" ){
		temp.dvalue = cMatch.dvalue;
		temp.svalue = cMatch.svalue;
		tokenTable.push_back(temp);
		tableIndex++;
	}
	//identifier
	else if ( cMatch.classMacro == "ID" ){
		//Just saying that what if we push all declaration into tokentable and before push any ID
		//we check if it's been declared ??
		temp.dvalue = cMatch.dvalue;
		temp.svalue = cMatch.svalue;
		tokenTable.push_back(temp);
		tableIndex++;
	}
	//Reserved word, opreator, separator 
	else {
		//do nothing
	}
	//just check for sure
	if ( tokenTable.size() != tableIndex+1){
		exit(10);
	}
	return tableIndex;
};

void Lexer::writeResult( string classMacro, long tableIndex){
	long tempIndex = ( classMacro == "ID" || classMacro == "REALC" || classMacro == "INTC" || classMacro == "STRINGC" )?tableIndex:0;
	Token temp( "", classMacro, tempIndex);
	outputSequence.push_back( temp );
	return;
};

bool Lexer::hazardDealing(){
	if ( errorFlag ){
		scanIndex++;//ignore current character
		errorRecord.push_back(lineCounter);//record the line number of spotted  error
		errorFlag = false;
		return true;
	}
	return false;
};

bool Lexer::matchCoordination( bool matchRW, bool matchID, bool matchOT, bool matchCom ){
	//check match existence
	if ( !(matchRW || matchID || matchOT)  ){
		errorFlag = true;
		return false;
	}
	//check multiple match
	if ( ( matchRW == true && matchID == true && matchOT == false ) 
		|| ( matchRW == true && matchID == false && matchOT == true && matchCom == false)	//if comments spotted, it would be
		|| ( matchRW == false && matchID == true && matchOT == true )						//like: true false true true,
		|| ( matchRW == true && matchID == true && matchOT == true )) {						//but it's obviously not multi-match error		
		errorFlag = true;																	
		return false;
	}
	else if ( matchOT && matchCom ){//comments matched, do not change the tokentable and ouputSequence
		return false;
	}
	else
		return true;
};

long Lexer::nextScanIndex(){
	return scanIndex;
};

void Lexer::updateScanIndex( bool matchConfirm, bool matchCom, MatchTuple cMatch ){
	if ( hazardDealing() )//error occured and counter-hazard measure has deployed
		return;
	else if ( !matchConfirm && matchCom ){//comments spotted
		long offset = 0, i=0;
		//search for the end of this line, notice that the scanIndex has not been modified in this round
		//and still points to the incoming char
		while ( scanIndex + i + 2 < sourceFile.size() &&
			 !(sourceFile[scanIndex + i] == '*' && 
			 sourceFile[scanIndex + i+1] == '/') ){//first CR(13) then LF(10)
			 i++;
		 };
		 scanIndex += i+2;
	}
	else {//change scan index according to the match
		scanIndex += cMatch.length;
	}
	return;
};

void Lexer::resetIndexes(){
	scanIndex = 0;
	tableIndex = -1;
};

const char Lexer::sourceFileEntry( long index ){
	//boundary check
	if ( index >= sourceFile.size() ){
		cerr << "error: index out of range accessing sourceFile." << endl;
		return ' ';
	}
	return sourceFile[index];
}

const vector<Token> Lexer::getTokenLexicon(){
	return tokenLexicon;
}

bool Lexer::isRelevant( char ichar){
	if ( isalpha(ichar) || isdigit(ichar) || ichar == '_' || 
			ichar == '+' || ichar == '-' || ichar == '*' || ichar == '/' || ichar == '=' ||
			ichar == '!' || ichar == '>' || ichar == '<' || ichar == '&' || ichar == '|' ||
			ichar == ';' || ichar == ',' || ichar == '{' || ichar == '}' || ichar == '(' || 
			ichar == ')' || ichar == '[' || ichar == ']' || ichar == '.' || ichar == '\"' ){
		return true;
	}
	return false;
}

void Lexer::initiateLexicon(){
	tokenLexicon.push_back(Token("", "INTC", -1));
	tokenLexicon.push_back(Token("", "REALC", -1));
	tokenLexicon.push_back(Token("", "STRINGC", -1));
	tokenLexicon.push_back( Token( "int", "INT", -1) );
	tokenLexicon.push_back( Token( "float", "FLOAT", -1) );
	tokenLexicon.push_back( Token( "do", "DO", -1) );
	tokenLexicon.push_back( Token( "while", "WHILE", -1) );
	tokenLexicon.push_back( Token( "if", "IF", -1) );
	tokenLexicon.push_back( Token( "else", "ELSE", -1) );
	tokenLexicon.push_back( Token( "for", "FOR", -1) );
	tokenLexicon.push_back( Token( "bool", "BOOL", -1) );
	tokenLexicon.push_back( Token( "true", "BOOLC", -1) );
	tokenLexicon.push_back( Token( "false", "BOOLC", -1) );
	tokenLexicon.push_back( Token( "null", "NULL", -1) );
	tokenLexicon.push_back( Token( "string", "STRING", -1) );
	tokenLexicon.push_back( Token( "print", "PRINT", -1) );
	tokenLexicon.push_back( Token( "scanf", "SCANF", -1) );
	tokenLexicon.push_back(Token("endif", "ENDIF", -1));
	tokenLexicon.push_back(Token("given", "GIVEN", -1));
	tokenLexicon.push_back(Token("endgiven", "ENDGIVEN", -1));
	tokenLexicon.push_back(Token("repeat", "REPEAT", -1));
	tokenLexicon.push_back(Token("endrepeat", "ENDREPEAT", -1));
	tokenLexicon.push_back(Token("then", "THEN", -1));
	tokenLexicon.push_back(Token("until", "UNTIL", -1));
	tokenLexicon.push_back(Token("endwhile", "ENDWHILE", -1));
	tokenLexicon.push_back( Token( "+", "PLUS", -1) );
	tokenLexicon.push_back( Token( "-", "MINUS", -1) );
	tokenLexicon.push_back( Token( "*", "MULTI", -1) );
	tokenLexicon.push_back( Token( "/", "RDIV", -1) );
	tokenLexicon.push_back( Token( "==", "EQ", -1) );
	tokenLexicon.push_back( Token( "!=", "NEQ", -1) );
	tokenLexicon.push_back( Token( "=", "AEQ", -1) );
	tokenLexicon.push_back( Token( "!", "NOT", -1) );
	tokenLexicon.push_back( Token( ">=", "GE", -1) );
	tokenLexicon.push_back( Token( "<=", "LE", -1) );
	tokenLexicon.push_back( Token( ">", "GT", -1) );
	tokenLexicon.push_back( Token( "<", "LT", -1) );
	tokenLexicon.push_back( Token( "&&", "AND", -1) );
	tokenLexicon.push_back( Token( "||", "OR", -1) );
	tokenLexicon.push_back( Token( ";", "SEP01", -1) );
	tokenLexicon.push_back( Token( ",", "SEP02", -1) );
	tokenLexicon.push_back( Token( "{", "SEP03", -1) );
	tokenLexicon.push_back( Token( "}", "SEP04", -1) );
	tokenLexicon.push_back( Token( "(", "SEP05", -1) );
	tokenLexicon.push_back( Token( ")", "SEP06", -1) );
	tokenLexicon.push_back( Token( "[", "SEP07", -1) );
	tokenLexicon.push_back( Token( "]", "SEP08", -1) );
	//tokenLexicon.push_back( Token( "\"", "", -1) );
	//tokenLexicon.push_back( Token( ".", "", -1) );
	//tokenLexicon.push_back( Token( "", "", -1) );
	//newly added to the change in SynSemantic Analysis
	

	tokenLexicon.push_back(Token("#", "#", -1));
	tokenLexicon.push_back(Token("", "VOID", -1));
	tokenLexicon.push_back(Token("", "ID", -1));
};