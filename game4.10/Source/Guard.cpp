#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tower.h"
#include "Guard.h"
#include <string>

namespace game_framework {
	Guard::Guard() : Tower() {
		atk_speed = 1;
		MaxSp = 2;
		projectile.SetPicNumber(IDB_LAPPLAND_PROJECTILE);
		projectile2.SetPicNumber(IDB_LAPPLAND_PROJECTILE);
		iconX = 448;
		cost = 18;
		projectile.SetDmg(10);
		projectile2.SetDmg(10);
		MaxBlockCount = 2;
		MaxHp = 200;
	}

	void Guard::LoadBitmap() {
		icon.LoadBitmap(IDB_LAPPLAND, RGB(255, 255, 255));
		char_l.LoadBitmap(IDB_LAPPLAND_L, RGB(255, 255, 255));
		char_r.LoadBitmap(IDB_LAPPLAND_R, RGB(255, 255, 255));
		projectile.LoadBitmap();
		projectile2.LoadBitmap();
		hpBar.LoadBitmap();
		Tower::IconReset();
	}

	void Guard::OnMove() {
		if (IsActive) {
			for (int i = 0; i < time.size(); i++) {
				if (hp[i] > 0) {
					if (target[i] != nullptr && IsInRange(pos[i].first, pos[i].second, target[i]->x, target[i]->y) && target[i]->isAlive()) {
						if (sp[i] == MaxSp) {
							Attack(i);
							sp[i] = 0;
						}
						else if (Attackable(i, 1000 / atk_speed)) {
							Tower::Attack(i);
							sp[i]++;
							hp[i] -= 10 * blockCount[i];
						}
					}

					for (int j = 0; j < blockedEnemies.size(); j++) {
						if (!blockedEnemies[j]->isAlive()) {
							enemies[j]->isBlocked = false;
							blockedEnemies.erase(blockedEnemies.begin() + j);
							j--;
							blockCount[i]--;
						}
					}
					for (int j = 0; j < enemies.size(); j++) {
						if (blockCount[i] < MaxBlockCount && enemies[j]->isAlive() && !enemies[j]->isBlocked && Range(pos[i].first, enemies[j]->x, pos[i].second, enemies[j]->y) < 48) {
							enemies[j]->isBlocked = true;
							blockedEnemies.push_back(enemies[j]);
							blockCount[i]++;
						}
					}
				}
				else {
					for (int j = 0; j < blockedEnemies.size(); j++)
						blockedEnemies[j]->isBlocked = false;
					blockedEnemies.clear();
					for (int j = 0; j < blockCount.size(); j++)
						blockCount[j] = 0;
				}
			}
			projectile.OnMove();
			projectile2.OnMove();
		}
	}

	void Guard::OnShow() {
		icon.ShowBitmap();
		projectile2.OnShow();
		for (int i = 0; i < pos.size(); i++) {
			hpBar.Set(pos[i].first, pos[i].second, hp[i], MaxHp);
			hpBar.OnShow();
		}
		Tower::OnShow();
	}

	void Guard::SetData(int x, int y, clock_t t, int *cost) {
		target2.push_back(0);
		Tower::SetData(x, y, t, cost);
	}

	void Guard::SetEnemies(vector<Enemy>& enemies) {
		for (int i = 0; i < pos.size(); i++) {
			for (int j = 0; j < enemies.size();  j++) {
				if (enemies[j].isAlive() && IsInRange(pos[i].first, pos[i].second, enemies[j].x, enemies[j].y)) {
					this->target[i] = &(enemies[j]);
					projectile.SetTarget(enemies[j]);
					for (int n = j + 1; n < enemies.size(); n++)
						if (enemies[n].isAlive() && IsInRange(pos[i].first, pos[i].second, enemies[n].x, enemies[n].y)) {
							this->target2[i] = &(enemies[n]);
							projectile2.SetTarget(enemies[n]);
							break;
						}
					break;
				}
			}
		}
		if (this->enemies.empty())
			for (int i = 0; i < enemies.size(); i++)
				this->enemies.push_back(&enemies[i]);
	}

	void Guard::Attack(int i) {
		projectile2.SetPosition(pos[i].first + 24, pos[i].second + 24);
		Tower::Attack(i);
	}

	bool Guard::IsInRange(int selfX, int selfY, int targetX, int targetY) {
		switch (int(direction))
		{
		case 0:
			if ((targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY - 36 && targetY <= selfY + 60) || (targetX >= selfX && targetX <= selfX + 48 && targetY >= selfY - 132 && targetY <= selfY + 60))
				return true;
			break;
		case 1:
			if ((targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY + 12 && targetY <= selfY + 108) || (targetX >= selfX && targetX <= selfX + 48 && targetY >= selfY + 12 && targetY <= selfY + 204))
				return true;
			break;
		case 2:
			if ((targetX >= selfX - 48 && targetX <= selfX + 48 && targetY >= selfY - 36 && targetY <= selfY + 108) || (targetX >= selfX - 144 && targetX <= selfX - 48 && targetY >= selfY -12 && targetY <= selfY + 60))
				return true;
			break;
		case 3:
			if ((targetX >= selfX && targetX <= selfX + 96 && targetY >= selfY - 36 && targetY <= selfY + 108) || (targetX >= selfX && targetX <= selfX + 192 && targetY >= selfY + 12 && targetY <= selfY + 60))
				return true;
			break;
		default:
			break;
		}
		return false;
	}
}