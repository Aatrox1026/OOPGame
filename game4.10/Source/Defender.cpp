#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tower.h"
#include "Defender.h"
#include <string>

namespace game_framework {
	Defender::Defender() : Tower() {
		atk_speed = 1;
		MaxSp = 5;
		iconX = 592;
		cost = 23;
		projectile.SetPicNumber(IDB_PROJECTILE);
		projectile.SetDmg(5);
		MaxBlockCount = 3;
		MaxHp = 500;
	}

	void Defender::LoadBitmap() {
		icon.LoadBitmap(IDB_HOSHIKUMA, RGB(255, 255, 255));
		char_l.LoadBitmap(IDB_HOSHIKUMA_L, RGB(255, 255, 255));
		char_r.LoadBitmap(IDB_HOSHIKUMA_R, RGB(255, 255, 255));
		projectile.LoadBitmap();
		Tower::IconReset();
		hpBar.LoadBitmap();
	}

	void Defender::OnMove() {
		if (IsActive) {
			for (int i = 0; i < time.size(); i++) {
				if (hp[i] > 0) {
					if (target[i] != nullptr && IsInRange(pos[i].first, pos[i].second, target[i]->x, target[i]->y) && target[i]->isAlive()) {
						if (Attackable(i, 1000 / atk_speed)) {
							Attack(i);
							hp[i] -= 10 * blockCount[i];
							for (int j = 0; j < blockedEnemies.size(); j++)
								blockedEnemies[j]->hp -= 5;
							OutputDebugString(_T((to_string(blockCount[i]) + " ").c_str()));
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
		}
	}

	void Defender::OnShow() {
		icon.ShowBitmap();
		for (int i = 0; i < pos.size(); i++) {
			hpBar.Set(pos[i].first, pos[i].second, hp[i], MaxHp);
			hpBar.OnShow();
		}
		Tower::OnShow();
	}

	bool Defender::IsInRange(int selfX, int selfY, int targetX, int targetY) {
		switch (int(direction))
		{
		case 0:
			if (targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY - 132 && targetY <= selfY + 60)
				return true;
			break;
		case 1:
			if (targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY + 12 && targetY <= selfY + 192)
				return true;
			break;
		case 2:
			if (targetX >= selfX - 144 && targetX <= selfX + 48 && targetY >= selfY - 36 && targetY <= selfY + 108)
				return true;
			break;
		case 3:
			if (targetX >= selfX && targetX <= selfX + 192 && targetY >= selfY - 36 && targetY <= selfY + 108)
				return true;
			break;
		default:
			break;
		}
		return false;
	}
}