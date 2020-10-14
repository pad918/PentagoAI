#include "Hash.h"

Hash128::Hash128(uint64_t largeWord, uint64_t smallWord)
{
	val[0] = largeWord;
	val[1] = smallWord;
}

int Hash128::compareHash(Hash128 inputHash)
{
	// 1 = larger
	// 0 = same
	//-1 = smaller
	if (val[0] > inputHash.val[0]) { return 1; }
	if (val[0] < inputHash.val[0]) { return -1; }
	if (val[1] > inputHash.val[1]) { return 1; }
	if (val[1] < inputHash.val[1]) { return -1; }
	return 0;
}

long HashTable::getIndexInTable(Hash128 hash)
{
	if (hashList.size() == 0) { return false; }
	int min = 0;
	int max = hashList.size() - 1;
	int lastPos = 0;
	int pos = (max - min) / 2;
	while (lastPos != pos) {
		testVal++;
		int cmp = hash.compareHash(hashList[pos]);
		if (cmp > 0) {/*öka pos*/ min = pos; }
		else if (cmp < 0) {/*minska cmp*/ max = pos; }
		else {/* HAR HITTAT EN MATCH*/ break; } //Should retrurn position of val...
		lastPos = pos;
		pos = min + ((max - min) / 2);
	}
	if (hash.compareHash(hashList[hashList.size() - 1]) > 0) { return (hashList.size()-1); }
	if (hash.compareHash(hashList[0]) < 0) { return -1; }
	return pos;
}

bool HashTable::isInTable(Hash128 inputHash)
{
	if (hashList.size() == 0) { return false; }
	int min = 0;
	int max = hashList.size() - 1;
	int lastPos = 0;
	int pos = (max - min) / 2;
	while (lastPos != pos) {
		testVal++;
		int cmp = inputHash.compareHash(hashList[pos]);
		if (cmp > 0) {/*öka pos*/ min = pos; }
		else if (cmp < 0) {/*minska cmp*/ max = pos; }
		else {/* HAR HITTAT EN MATCH*/ return true; } //Should retrurn position of val...
		lastPos = pos;
		pos = min + ((max - min) / 2);
	}
	if (inputHash.compareHash(hashList[hashList.size() - 1]) == 0) { return true; }
	return false;
}

void HashTable::addElement(Hash128 hash, int value)
{
	long position = getIndexInTable(hash) + 1;
	if (hashList.size() == 0) {
		position = 0;
	}
	hashList.insert(hashList.begin() + position, hash);
	valueList.insert(valueList.begin() + position, value);
	lowestDepth.insert(lowestDepth.begin() + position, value);
}

int HashTable::getVal(Hash128 hash)
{
	hash.val[1]++; // SILVRTEJP LÖSR ALLT >=)
	long pos = getIndexInTable(hash);
	return valueList[pos];
}
