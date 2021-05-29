namespace game_framework {
	class HpBar {
	public:
		HpBar();
		void LoadBitmap();
		void Set(int, int, int, int);
		void OnShow();
		CMovingBitmap pic1, pic2;
		int x, y, hp, maxhp = 100;
	};
}