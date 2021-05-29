#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"
#include "Projectile.h"
#include <string>
#include <time.h>

namespace game_framework {
	Projectile::Projectile() {
	}

	void Projectile::LoadBitmap() {
		pic.LoadBitmap(picNumber, RGB(255, 255, 255));
	}

	void Projectile::OnMove() {
		for (int i = 0; i < pos.size(); i++) {
			double angle = (double)(target->y + 24 - pos[i].second) / (target->x + 24 - pos[i].first);
			if (Range(pos[i].first,target->x,pos[i].second,target->y) <= 48 * 4) {
				/*OutputDebugString(_T((to_string(pos[i].first) + " ").c_str()));
				OutputDebugString(_T((to_string(pos[i].second) + "\n").c_str()));
				OutputDebugString(_T((to_string(target->x + 24) + " ").c_str()));
				OutputDebugString(_T((to_string(target->y + 24) + "\n\n").c_str()));*/
				if (pow(target->x + 24 - pos[i].first, 2) + pow(target->y + 24 - pos[i].second, 2) <= 100) {
					DealDmg();
					pos.erase(pos.begin() + i);
					i -= 2;
				}
				else if (target->x + 24 == pos[i].first) {
					if (target->y + 24 - pos[i].second > 0)
						pos[i].second += 10;
					else
						pos[i].second += -10;
				}
				else if (target->x + 24 - pos[i].first < 0 && target->y + 24 == pos[i].second) {
					pos[i].first += -10;
				}
				else if (target->x + 24 - pos[i].first > 0 && target->y + 24 == pos[i].second) {
					pos[i].first += 10;
				}
				else if (target->x + 24 - pos[i].first < 0) {
					if (target->y + 24 - pos[i].second < 0)
						pos[i].second += -10 * abs(sin(angle));
					else
						pos[i].second += 10 * abs(sin(angle));
					pos[i].first += -10 * abs(cos(angle));
				}
				else {
					if (target->y + 24 - pos[i].second < 0)
						pos[i].second += -10 * abs(sin(angle));
					else
						pos[i].second += 10 * abs(sin(angle));
					pos[i].first += 10 * abs(cos(angle));
				}
			}
		}
	}

	void Projectile::OnShow() {
		for (int i = 0; i < pos.size(); i++)
			if(target->isAlive() && Range(pos[i].first, target->x, pos[i].second, target->y) <= 48 * 4 ) {
				pic.SetTopLeft(pos[i].first, pos[i].second);
				pic.ShowBitmap();
			}
	}

	void Projectile::SetTarget(Enemy& target) {
		this->target = &target;
	}

	void Projectile::SetTarget(vector<Enemy>& target) {
		for (int i = 0; i < target.size(); i++) {
			if (target[i].hp > 0) {
				this->target = &(target[i]);
				break;
			}
		}
	}

	void Projectile::SetPosition(int x, int y) {
		pos.push_back(make_pair(x, y));
	}

	void Projectile::DealDmg() {
		target->hp -= dmg;
	}

	double Projectile::Range(int x1,int x2,int y1,int y2) {
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}

	void Projectile::SetPicNumber(int n) {
		picNumber = n;
	}

	void Projectile::SetDmg(int dmg) {
		this->dmg = dmg;
	}
}