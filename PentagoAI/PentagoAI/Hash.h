#pragma once
#include <vector>
#include <iostream>

class Hash128 {
private:
	
public:
	Hash128(uint64_t largeWord, uint64_t smallWord);
	int compareHash(Hash128 inputHash);
	uint64_t val[2];
};

class HashTable {
private:
	
	
	
public:
	int testVal = 0;
	bool isInTable(Hash128 inputHash);
	long getIndexInTable(Hash128 hash);
	void addElement(Hash128 hash, int value);
	int getVal(Hash128 hash);
	std::vector<Hash128> hashList; 
	std::vector<int> valueList;
};