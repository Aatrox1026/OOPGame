#include "stdafx.h"
#include "resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include "Enemy.h"


namespace game_framework {

	Enemy::Enemy() {
		picType = minion;
		x = 32 + 48 * (path[0].first - 1);
		y = 48 * (path[0].second - 1);
		hp = 100;
		index = 0;
		isBlocked = false;
	}

	Enemy::Enemy(int type) {
		picType = PicType(type);
		x = 32 + 48 * (path[0].first - 1);
		y = 48 * (path[0].second - 1);
		hp = 100;
		index = 0;
		isBlocked = false;
	}

	void Enemy::LoadBitmap() {
		switch (picType)
		{
		case minion: {
			char *bmp[12] = { ".\\bitmaps\\man1.bmp", ".\\bitmaps\\man2.bmp", ".\\bitmaps\\man3.bmp", ".\\bitmaps\\man4.bmp", ".\\bitmaps\\man5.bmp", ".\\bitmaps\\man6.bmp", ".\\bitmaps\\man7.bmp", ".\\bitmaps\\man8.bmp", ".\\bitmaps\\man9.bmp", ".\\bitmaps\\man10.bmp", ".\\bitmaps\\man11.bmp", ".\\bitmaps\\man12.bmp" };
			for (int i = 0; i < 12; i++)
			pic.AddBitmap(bmp[i], RGB(255, 255, 255));
			speed = 1;
		}
			break;
		case dog: {
			char *bmp[] = { ".\\RES\\dog.bmp" };
			for (int i = 0; i < 1; i++)
				pic.AddBitmap(bmp[i], RGB(255, 255, 255));
			speed = 2;
			break;
		}
		}
		hpBar.LoadBitmap();
	}

	void Enemy::OnMove() {
		if (!isBlocked) {
			if (x == 32 + 48 * (path[index + 1].first - 1) && y == 48 * (path[index + 1].second - 1) && index != 5) {
				x = 32 + 48 * (path[index + 1].first - 1);
				y = 48 * (path[index + 1].second - 1);
				index++;
			}
			if (index != 5) {
				if (y != 48 * (path[index + 1].second - 1)) {
					if (y > 48 * (path[index + 1].second - 1))
						y -= speed;
					else
						y += speed;
				}
				else if (x != 32 + 48 * (path[index + 1].first - 1)) {
					if (x > 48 * (path[index + 1].first - 1))
						x -= speed;
					else
						x += speed;
				}
			pic.OnMove();
			}
		}
		pic.SetTopLeft(x, y);
		hpBar.Set(x, y, hp, 100);
	}

	void Enemy::OnShow() {
		if (isAlive()) {
			pic.OnShow();
			hpBar.OnShow();
		}
	}

	bool Enemy::isAlive() {
		if (hp > 0)
			return true;
		return false;
	}
}