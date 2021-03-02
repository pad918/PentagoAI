#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <sstream>
#include <algorithm>
#include <iterator>
#include <array>

class DataSample {
public:
	DataSample();
	uint64_t boardKey;
	int evaluations[288];
};

class Dataset {
private:
	std::vector<DataSample> dataSamples;
public:
	Dataset(std::string pathToData, int numOfDataBatches);
	uint64_t getBoard(int i);
	std::array<double, 288> getTargets(int i);
};