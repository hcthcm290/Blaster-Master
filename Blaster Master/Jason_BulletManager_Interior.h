#pragma once 

#include "Bullet_Jason_Interior.h"

class Jason_BulletManager {
private:
	int level = 1;
	vector<BulletJasonInterior*> bullets;

	const int MAX_BULLET[9] =
	{
		-1, //level 0 - dont mind
		1,	//1
		2,	//2
		3,	//3
		3,	//4
		3,	//5
		3,	//6
		5,	//7
		10,	//8
	};
public:
	Jason_BulletManager();
	int GetLevel() { return level; }
	void SetLevel(int l) { level = l; }
	void CheckBullet();
	void Fire(int x,int y,int dx, int dy);

	//for easy test
	void CheckCheat();
};