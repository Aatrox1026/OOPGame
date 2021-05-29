#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "LossCounter.h"

namespace game_framework {
	LossCounter::LossCounter() {
		count_init = count = 3;
	}

	LossCounter::LossCounter(int c) {
		count_init = count = c;
	}

	void LossCounter::LoadBitmap() {
		int num[10] = { IDB_0, IDB_1, IDB_2, IDB_3, IDB_4, IDB_5, IDB_6, IDB_7, IDB_8, IDB_9 };
		for (int i = 0; i < 10; i++)
			pic[i].LoadBitmap(num[i], RGB(0, 0, 0));
	}

	void LossCounter::OnShow() {
		pic[count].SetTopLeft(315, 0);
		pic[count].ShowBitmap();
	}

	void LossCounter::Loss() {
		count--;
	}

	void LossCounter::CheckEnemies(vector<Enemy> &enemies) {
		count = count_init;
		for (int i = 0; i < enemies.size(); i++)
			if (enemies[i].index == 5 && enemies[i].hp > 0 && count > 0) {
				count--;
				enemies[i].hp = 0;
			}
	}

	bool LossCounter::CheckLost() {
		if (count == 0)
			return true;
		return false;
	}

	bool LossCounter::CheckWin(vector<Enemy> enemies) {
		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i].hp > 0)
				break;
			if (i == enemies.size() - 1)
				return true;
		}
		return false;
	}
}