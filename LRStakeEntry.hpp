#include "stdafx.h"
#include <iostream>
#include <vector>
#include <String>

using std::string;
using std::vector;

/*
 * author: Ruogu Gao
 * date: 4.28.2015
 * description: 
 *	1. the entry in LR(1) ananlysis stake
 *	2. format: classMacro: string; state: long;
 *	3. classMacro coresponding to the classMacro in class Token
 *	4. state indicates current LR(1) state number
 *	5. 
 */

class LRStakeEntry{
public:
	string classMacro;
	long state;
	
	LRStakeEntry():state(-1){};
	LRStakeEntry( const LRStakeEntry& );
	~LRStakeEntry();
private:

};

//implements
LRStakeEntry::LRStakeEntry( const LRStakeEntry& a ){
	classMacro = a.classMacro;
	state = a.state;
};
