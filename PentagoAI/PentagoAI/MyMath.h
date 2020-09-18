#pragma once

namespace mth {

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
	};
}