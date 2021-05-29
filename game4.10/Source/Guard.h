namespace game_framework {
	class Guard : public Tower {
	public:
		Guard();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void SetData(int, int, clock_t, int*);
		void SetEnemies(vector<Enemy>&);
		void Attack(int);
		bool IsInRange(int, int, int, int);
	private:
		vector<int> durationCounter;
		vector<Enemy*> target2;
		Projectile projectile2;
	};
}