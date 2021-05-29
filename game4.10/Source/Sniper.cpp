#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Tower.h"
#include "Sniper.h"
#include <string>

namespace game_framework {
	Sniper::Sniper() : Tower() {
		atk_speed = 1.5;
		MaxSp = 5;
		iconX = 400;
		cost = 14;
		projectile.SetPicNumber(IDB_SNIPER_PROJECTILE);
		projectile.SetDmg(10);
	}

	void Sniper::LoadBitmap() {
		icon.LoadBitmap(IDB_EXUSIAI, RGB(255, 255, 255));
		char_l.LoadBitmap(IDB_EXUISI_L, RGB(255, 255, 255));
		char_r.LoadBitmap(IDB_EXUISI_R, RGB(255, 255, 255));
		projectile.LoadBitmap();
		Tower::IconReset();
	}

	void Sniper::OnMove() {
		if (IsActive) {
			for (int i = 0; i < time.size(); i++) {
				if (target[i] != nullptr && IsInRange(pos[i].first, pos[i].second, target[i]->x, target[i]->y) && target[i]->isAlive()) {
					if (sp[i] == MaxSp && Attackable(i, 50)) {
						Attack(i);
						skillHit[i]--;
						if (skillHit[i] == 0)
							sp[i] = 0;
						//OutputDebugString(_T((to_string(skill[i].first) + " ").c_str()));
					}
					else if (Attackable(i, 1000 / atk_speed)) {
						Attack(i);
						sp[i]++;
						skillHit[i] = 4;
						//OutputDebugString(_T((to_string(skill[i].first) + " ").c_str()));
					}
				}
			}
			projectile.OnMove();
		}
	}

	void Sniper::OnShow() {
		icon.ShowBitmap();
		Tower::OnShow();
	}

	bool Sniper::IsInRange(int selfX, int selfY, int targetX, int targetY) {
		switch (direction)
		{
		case Up:
			if (targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY - 132 && targetY <= selfY + 60)
				return true;
			break;
		case Down:
			if (targetX >= selfX - 48 && targetX <= selfX + 96 && targetY >= selfY + 12 && targetY <= selfY + 204)
				return true;
			break;
		case Left:
			if (targetX >= selfX-144 && targetX <= selfX + 48 && targetY >= selfY - 36 && targetY <= selfY + 108)
				return true;
			break;
		case Right:
			if (targetX >= selfX && targetX <= selfX + 192 && targetY >= selfY - 36 && targetY <= selfY + 108)
				return true;
			break;
		default:
			break;
		}
		return false;
	}

	void Sniper::SetData(int x, int y, clock_t t, int *cost) {
		Tower::SetData(x, y, t, cost);
		skillHit.push_back(4);
	}
}