#include "MyMath.h"

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
