namespace game_framework {
	class Cost {
	public:
		Cost();
		Cost(int);
		void LoadBitmap();
		void OnMove();
		void OnShow();
		CMovingBitmap pic[10];
		int cost, costCounter;
	};
}