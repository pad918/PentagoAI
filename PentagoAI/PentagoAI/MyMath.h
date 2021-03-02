#pragma once
#include <stdint.h>
#include <fstream>
#include <string>
#include <thread>
#include <random>

namespace mth {
	class rand {

	public:
		int intRand(const int & min, const int & max);
	};

	class Vector2 {
	public:
		int x, y;
		Vector2(int xPos, int yPos);
		Vector2();
	};
	class Vector3 {
	public:
		int x, y, z;
		Vector3(int xPos, int yPos, int zPos);
		Vector3();
	};
	class PentagoMove {
	public:
		Vector2 marblePos;
		Vector2 rotation;
		PentagoMove(Vector2 mP, Vector2 rot);
		PentagoMove();
		uint64_t getHash();
		
	};
}