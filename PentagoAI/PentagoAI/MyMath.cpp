#include "MyMath.h"
#include <iostream>

mth::Vector2::Vector2(int xPos, int yPos)
{
	x = xPos;
	y = yPos;
}

mth::Vector2::Vector2()
{
	x = 0;
	y = 0;
}

mth::Vector3::Vector3(int xPos, int yPos, int zPos)
{
	x = xPos;
	y = yPos;
	z = zPos;
}

mth::Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

mth::PentagoMove::PentagoMove(Vector2 mP, Vector2 rot)
{
	marblePos = mP;
	rotation = rot;
}

mth::PentagoMove::PentagoMove()
{

}

uint64_t mth::PentagoMove::getHash()
{
	uint64_t hash=0;
	hash += marblePos.x;
	hash += marblePos.y << 3;
	hash += rotation.x	<< 5;
	hash += rotation.y	<< 7;
	return hash;
}

//Den här funktionen är rakt stulen från stackoverflow: tack Konrad Rudolph.
int mth::rand::intRand(const int & min, const int & max) {
	static thread_local std::random_device generator;
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}
