#include "Dataset.h"

Dataset::Dataset(std::string pathToData, int numOfDataBatches)
{
	std::cout << "Loading dataset...\n";
	for (int i = 0; i < numOfDataBatches; ++i) {
		std::string path = pathToData + std::to_string(i * 1000) + ".txt";
		std::string line="";
		int lineNum = 0;
		std::ifstream loadFile(path);
		//std::cout << "Loading from path: " << path << "\n";
		uint64_t boardKey = 0;
		int evaluations[288];

		//Read all data from the file 
		if (loadFile.is_open())
		{
			while (getline(loadFile, line))
			{
				
				if (lineNum % 289 == 0 || lineNum >= 28899) {
					bool endOfFile = false;

					if (lineNum >= 28899) {
						//close file
						endOfFile = true;
						evaluations[(lineNum % 289 - 1)] = std::stoi(line, nullptr);
					}

					//save data sample
					if (lineNum != 0) {
						DataSample sample;
						sample.boardKey = boardKey;
						std::copy(std::begin(evaluations), std::end(evaluations), std::begin(sample.evaluations));
						dataSamples.push_back(sample);
						if (endOfFile) {
							//std::cout << "Closed file\n";
							loadFile.close();
							break;
						}
					}

					
					//Load next board key:
					std::istringstream iss(line);
					if (!(iss >> boardKey)) {
						std::cout << "ERROR READING FILE WITH PATH: " << path << " on line " << lineNum << "\n";
					}
				}
				else {
					//load next evaluation
					evaluations[(lineNum % 289 - 1)] = std::stoi(line, nullptr);
				}

				++lineNum;
			}
		}
	}
	std::cout << "Loaded " << dataSamples.size() << " samples.\n";
}

uint64_t Dataset::getBoard(int i)
{
	i = i % dataSamples.size();
	return dataSamples[i].boardKey;
}

std::array<double, 288> Dataset::getTargets(int i)
{
	i = i % dataSamples.size();
	std::array<double, 288> targets;
	std::copy(std::begin(dataSamples[i].evaluations), std::end(dataSamples[i].evaluations), std::begin(targets));
	int maxEval = *std::max_element(targets.begin(), targets.end());
	int minEval = *std::min_element(targets.begin(), targets.end());
	for (double & target : targets) {
		if (maxEval == (int)target) {
			target = 1.0;
		}
		else {
			double dividor = (double)maxEval - (double)minEval;
			if (maxEval == minEval) { target = 1; }
			else {
				target = 0.5 * ((target - (double)minEval) / dividor);
			}
		}
	}
	return targets;
}

DataSample::DataSample()
{
	memset(&evaluations, 0, sizeof(int)*288);
}
