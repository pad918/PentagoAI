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

long HashTable::getIndexInTable(uint64_t hash) //Finns ett fel här, kan aldrig ge sista elementet korrekt
{
	if (shortHashList.size() == 0) { return false; }
	int min = 0;
	int max = shortHashList.size() - 1;
	int lastPos = 0;
	int pos = (max - min) / 2;
	while (lastPos != pos) {
		testVal++;
		if (hash > shortHashList[pos]) {/*öka pos*/ min = pos; }
		else if (hash < shortHashList[pos]) {/*minska cmp*/ max = pos; }
		else {/* HAR HITTAT EN MATCH*/ break; } //Should retrurn position of val...
		lastPos = pos;
		pos = min + ((max - min) / 2);
	}
	if (hash == shortHashList.back()) { return (shortHashList.size() - 1); }
	if (hash < shortHashList.front()) { return -1; }
	return pos;
	return 0;
}

//bool HashTable::isInTable(Hash128 inputHash)
//{
//	if (hashList.size() == 0) { return false; }
//	int min = 0;
//	int max = hashList.size() - 1;
//	int lastPos = 0;
//	int pos = (max - min) / 2;
//	while (lastPos != pos) {
//		testVal++;
//		int cmp = inputHash.compareHash(hashList[pos]);
//		if (cmp > 0) {/*öka pos*/ min = pos; }
//		else if (cmp < 0) {/*minska cmp*/ max = pos; }
//		else {/* HAR HITTAT EN MATCH*/ return true; } //Should retrurn position of val...
//		lastPos = pos;
//		pos = min + ((max - min) / 2);
//	}
//	if (inputHash.compareHash(hashList[hashList.size() - 1]) == 0) { return true; }
//	return false;
//}

int HashTable::highestDepthOfHash(uint64_t inputHash) // inte testad
{
	if (shortHashList.size() == 0) { return -1; }
	int min = 0;
	int max = shortHashList.size() - 1;
	int lastPos = 0;
	int pos = (max - min) / 2;
	while (lastPos != pos) {
		testVal++;
		if (inputHash > shortHashList[pos]) {/*öka pos*/ min = pos; }
		else if (inputHash < shortHashList[pos]) {/*minska cmp*/ max = pos; }
		else {/* HAR HITTAT EN MATCH*/ return (highestDepth[pos]); } //Returns depth of hash
		lastPos = pos;
		pos = min + ((max - min) / 2);
	}
	if (inputHash == shortHashList.back()) { return (highestDepth[pos]); }
	return -1;
}

int HashTable::highestDepthOfHashNy(uint64_t inputHash)
{
	if (hashMap.find(inputHash) == hashMap.end()) { return -1; }
	return highestValuesMap.at(inputHash);
}

//void HashTable::addElement(Hash128 hash, int value)
//{
//	long position = getIndexInTable(hash) + 1;
//	if (hashList.size() == 0) {
//		position = 0;
//	}
//	hashList.insert(hashList.begin() + position, hash);
//	valueList.insert(valueList.begin() + position, value);
//	highestDepth.insert(highestDepth.begin() + position, value);
//
//}

void HashTable::addElement(uint64_t hash, int value, int depth)
{
	//Update depth value of hash
	if (highestDepthOfHash(hash) >= 0) {
		long position = getIndexInTable(hash);
		highestDepth[position] = depth;
		return;
	}

	//Insert hash and key into table
	long position = getIndexInTable(hash) + 1;
	if (shortHashList.size() == 0) {
		position = 0;
	}
	shortHashList.insert(shortHashList.begin() + position, hash);
	valueList.insert(valueList.begin() + position, value);
	highestDepth.insert(highestDepth.begin() + position, depth);

}

void HashTable::addElementNy(uint64_t hash, int value, int depth)
{
	//Update depth value of hash

	if (hashMap.find(hash) != hashMap.end() && depth > (highestValuesMap.at(hash))) {
		highestValuesMap[hash] = depth;
		return;
	}

	//Insert hash and key into table
	hashMap[hash] = value;
	highestValuesMap[hash] = depth;
}

int HashTable::getVal(uint64_t hash)
{
	hash++; // SILVRTEJP LÖSR ALLT >=)
	long pos = getIndexInTable(hash);
	return valueList[pos];
}

int HashTable::getValNy(uint64_t hash)
{
	return hashMap.at(hash);
}

void HashTable::clear()
{
	shortHashList.clear();
	highestDepth.clear();
	valueList.clear();
}

KeyHash::KeyHash(uint64_t largeWord, uint64_t smallWord, uint8_t d)
{
	val[0] = largeWord;
	val[1] = smallWord;
	depth = d;
}
