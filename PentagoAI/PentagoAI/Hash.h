#pragma once
#include <vector>
#include <iostream>
#include <unordered_map>
#include "include/tsl/robin_map.h"
#include "include/tsl/robin_growth_policy.h"
#include "include/tsl/robin_hash.h"
#include "include/tsl/robin_set.h"



class Hash128 {
private:
	
public:
	Hash128(uint64_t largeWord, uint64_t smallWord);
	int compareHash(Hash128 inputHash);
	uint64_t val[2];
};

class KeyHash {
private:

public:
	KeyHash(uint64_t largeWord, uint64_t smallWord, uint8_t d);
	uint64_t val[2];
	uint8_t depth=0;
};

class HashTable {
private:
	
	
	
public:
	//Nya saker
	//std::unordered_map<uint64_t, long> hashMap; 
	tsl::robin_map<uint64_t, long> hashMap;
	tsl::robin_map<uint64_t, int> highestValuesMap;

	//Gamla saker
	int testVal = 0;
	int highestDepthOfHash(uint64_t inputHash);
	int highestDepthOfHashNy(uint64_t inputHash);
	long getIndexInTable(uint64_t hash);
	void addElement(uint64_t hash, int value, int depth);
	void addElementNy(uint64_t hash, int value, int depth);
	int getVal(uint64_t hash);
	int getValNy(uint64_t hash);
	std::vector<uint64_t> shortHashList;
	std::vector<int> highestDepth;
	std::vector<int> valueList;
	void clear();
};