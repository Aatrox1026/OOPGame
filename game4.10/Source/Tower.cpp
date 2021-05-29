#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tower.h"
#include <time.h>
#include <string>

namespace game_framework {

	Tower::Tower() {
		findTarget = false;
		IsActive = false;
		IsSkillActive = false;
	}

	void Tower::LoadBitmap() {
		char_r.LoadBitmap(IDB_EXUISI_R, RGB(255, 255, 255));
		projectile.LoadBitmap();
	}

	void Tower::OnMove() {

	}
	 
	void Tower::OnShow() {
		for (int i = 0; i < pos.size(); i++) {
			char_r.SetTopLeft(pos[i].first, pos[i].second);
			char_r.ShowBitmap();
		}
		projectile.OnShow();
	}

	void Tower::SetEnemies(vector<Enemy>& enemies) {
		for (int i = 0; i < pos.size(); i++) {
			for (int j = 0; j < enemies.size(); j++) {
				if (enemies[j].isAlive() && IsInRange(pos[i].first, pos[i].second, enemies[j].x, enemies[j].y)) {
					this->target[i] = &(enemies[j]);
					projectile.SetTarget(enemies[j]);
					break;
				}
			}
		}
		if (this->enemies.empty())
			for (int i = 0; i < enemies.size(); i++)
				this->enemies.push_back(&enemies[i]);
	}

	void Tower::SetData(int x, int y, clock_t t,int *cost) {
		pos.push_back(make_pair(x, y));
		time.push_back(t);
		sp.push_back(0);
		target.push_back(0);
		blockCount.push_back(0);
		hp.push_back(MaxHp);
		*cost -= this->cost;
	}

	void Tower::SetDirection(int dir) {
		switch (dir)
		{
		case 0:
			direction = Up;
			break;
		case 1:
			direction = Down;
			break;
		case 2:
			direction = Left;
			break;
		case 3:
			direction = Right;
			break;
		}
	}

	bool Tower::CheckEmpty(int x, int y) {
		for (int i = 0; i < pos.size(); i++)
			if (pos[i].first == x && pos[i].second == y)
				return false;
		return true;
	}

	double Tower::Range(int x1, int x2, int y1, int y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}

	bool Tower::Attackable(int i,int t) {
		return (clock() - time[i] >= t) ? true : false;
	}

	void Tower::Attack(int i){
		projectile.SetPosition(pos[i].first + 24, pos[i].second + 24);
		time[i] = clock();
	}

	void Tower::IconReset() {
		icon.SetTopLeft(iconX, 432);
	}
}