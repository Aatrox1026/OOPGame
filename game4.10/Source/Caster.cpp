#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tower.h"
#include "Caster.h"
#include <string>

namespace game_framework {
	Caster::Caster() : Tower() {
		atk_speed = 1;
		MaxSp = 5;
		iconX = 544;
		cost = 21;
		projectile.SetPicNumber(IDB_CASTER_PROJECTILE);
		projectile.SetDmg(10);
	}

	void Caster::LoadBitmap() {
		icon.LoadBitmap(IDB_EYJAFJALLA, RGB(255, 255, 255));
		char_l.LoadBitmap(IDB_EYJAFJALLA_L, RGB(255, 255, 255));
		char_r.LoadBitmap(IDB_EYJAFJALLA_R, RGB(255, 255, 255));
		projectile.LoadBitmap();
		Tower::IconReset();
	}

	void Caster::OnMove() {
		if (IsActive) {
			for (int i = 0; i < time.size(); i++) {
				if (target[i] != nullptr && IsInRange(pos[i].first, pos[i].second, target[i]->x, target[i]->y) && target[i]->isAlive()) {
					if (sp[i] == MaxSp) {
						projectile.SetPicNumber(IDB_CASTER_PROJECTILE_SKILL);
						Attack(i);
						for (int j = 0; j < 5; j++) {
							if (Range(enemies[j]->x, target[i]->x, enemies[j]->y, target[i]->y) < 48 * 2) {
								enemies[j]->hp -= 30;
							}
						}
						projectile.SetPicNumber(IDB_CASTER_PROJECTILE);
						sp[i] = 0;
						//OutputDebugString(_T((to_string(skill[i].first) + " ").c_str()));
					}
					else if (Attackable(i, 1000 / atk_speed)) {
						Attack(i);
						sp[i]++;
						//OutputDebugString(_T((to_string(skill[i].first) + " ").c_str()));
					}
				}
			}
			projectile.OnMove();
		}
	}

	void Caster::OnShow() {
		icon.ShowBitmap();
		Tower::OnShow();
	}

	bool Caster::IsInRange(int selfX, int selfY, int targetX, int targetY) {
		switch (int(direction))
		{
		case 0:
			if ((targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY - 84 && targetY <= selfY + 60) || (targetX >= selfX && targetX <= selfX + 48 && targetY >= selfY - 132 && targetY <= selfY - 84))
				return true;
			break;
		case 1:
			if ((targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY + 12 && targetY <= selfY + 156) || (targetX >= selfX && targetX <= selfX + 48 && targetY >= selfY + 156 && targetY <= selfY + 204))
				return true;
			break;
		case 2:
			if ((targetX >= selfX - 96 && targetX <= selfX + 48 && targetY >= selfY - 36 && targetY <= selfY + 108) || (targetX >= selfX - 144 && targetX <= selfX - 96 && targetY >= selfY + 12 && targetY <= selfY + 60))
				return true;
			break;
		case 3:
			if ((targetX >= selfX && targetX <= selfX + 144 && targetY >= selfY - 36 && targetY <= selfY + 108) || (targetX >= selfX + 144 && targetX <= selfX + 192 && targetY >= selfY + 12 && targetY <= selfY + 48))
				return true;
			break;
		default:
			break;
		}
		return false;
	}
}