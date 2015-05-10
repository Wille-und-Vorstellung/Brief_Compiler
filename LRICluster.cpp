#pragma once
#include "stdafx.h"
//#include "SynSemantic.hpp"
#include "LRICluster.hpp"

//implements
LRStakeEntry::LRStakeEntry(const LRStakeEntry& a){
	classMacro = a.classMacro;
	state = a.state;
};