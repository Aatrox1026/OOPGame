namespace game_framework {
	class LossCounter {
	public:
		LossCounter();
		LossCounter(int);
		void LoadBitmap();
		//void OnMove();
		void OnShow();
		void Loss();
		void CheckEnemies(vector<Enemy>&);
		bool CheckLost();
		bool CheckWin(vector<Enemy>);
		CMovingBitmap pic[10];
		int count_init,count;
	};
}