#pragma once
#include <vector>
#include <iostream>
#include <chrono>
#include "Pentago.h"
#include "Hash.h"
#include "Minimax.h"
#include "NeuralNetwork.h"

void benchmarkBoardEvaluation() {
	/*RESULTS:
	
		Eval1:
			10000 * 100: 1060ms

		Eval2:
			10000 * 100: 
	*/
	mm::Minimax minmax;
	Dataset data("dataset/data3/", 142, true);
	std::vector<ptg::PentagoGame> games;
	for (int i = 0; i < 10000; i++) {
		ptg::PentagoGame tmp;
		tmp.loadBoardFromHash(data.getBoard(i));
		games.push_back(tmp);
	}
	
	//benchmark this
	
	long sum = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (int b = 0; b < 10; ++b) {
		for (int j = 0; j < 100; ++j) {
			for (int i = 0; i < 10000; ++i)
			{
				sum += minmax.evaluate(games[i], 1);
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Eval 1 = " << duration1.count() << "ms | sum = " << sum << "\n";

	sum = 0;
	start = std::chrono::high_resolution_clock::now();
	for (int b = 0; b < 10; ++b) {
		for (int j = 0; j < 100; ++j) {
			for (int i = 0; i < 10000; ++i)
			{
				sum += minmax.evaluate2(games[i], 1);
			}
		}
	}
	end = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Eval 2 = " << duration2.count() << "ms | sum = " << sum << "\n";
}