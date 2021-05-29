#ifndef HPBAR_H

#include "HpBar.h"

#endif
namespace game_framework {
	class Enemy {
	public:
		Enemy();
		Enemy(int);
		vector<pair<int, int> > path{ make_pair(12,3),make_pair(10,3),make_pair(10,7),make_pair(6,7),make_pair(6,5),make_pair(1,5) };
		void LoadBitmap();
		void OnMove();
		void OnShow();
		bool isAlive();
		CAnimation pic;
		int x, y, hp, index, speed;
		enum PicType { minion, dog };	PicType picType;
		bool isBlocked;
		HpBar hpBar;

		/*class HealthBar {
		public:
			//HealthBar(int, int, int, int);
			void LoadBitmap();
			void Set(int, int, int, int);
			void OnShow();
			CMovingBitmap pic1, pic2;
			int x, y, hp, maxhp=100;
		} hpBar;*/
	};
}