#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "HpBar.h"

namespace game_framework {
	HpBar::HpBar() {

	}

	void HpBar::LoadBitmap() {
		pic1.LoadBitmap(IDB_HEALTH1);
		pic2.LoadBitmap(IDB_HEALTH2);
	}

	void HpBar::Set(int x, int y, int hp, int maxhp) {
		this->x = x;
		this->y = y;
		this->hp = hp;
		this->maxhp = maxhp;
	}

	void HpBar::OnShow() {
		int correction = 0;
		int percentage = (hp * 100) / maxhp;
		for (int j = 10; j <= 100; j += 10) {
			if (j <= percentage) {
				pic1.SetTopLeft(x + 4 + correction, y - 8);
				pic1.ShowBitmap();
			}
			else {
				pic2.SetTopLeft(x + 4 + correction, y - 8);
				pic2.ShowBitmap();
			}
			correction += 4;
		}
	}
}