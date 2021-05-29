#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include "Cost.h"

namespace game_framework {
	Cost::Cost() {
		cost = 10;
		costCounter = 0;
	}

	Cost::Cost(int initCost) {
		cost = initCost;
		costCounter = 0;
	}

	void Cost::LoadBitmap() {
		int num[10] = { IDB_0, IDB_1, IDB_2, IDB_3, IDB_4, IDB_5, IDB_6, IDB_7, IDB_8, IDB_9 };
		for (int i = 0; i < 10; i++)
			pic[i].LoadBitmap(num[i], RGB(0, 0, 0));
	}

	void Cost::OnMove() {
		if (++costCounter == 30) {
			cost++;
			costCounter = 0;
			//OutputDebugString(_T((to_string(cost) + " ").c_str()));
		}
	}

	void Cost::OnShow() {
		for(int i=0;i<10;i++)
			pic[i].SetTopLeft(620, 403);
		pic[cost / 10].ShowBitmap();
		for(int i=0;i<10;i++)
			pic[i].SetTopLeft(630, 403);
		pic[cost % 10].ShowBitmap();
	}
}