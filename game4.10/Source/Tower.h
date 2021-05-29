namespace game_framework {
	class Tower {
	public:
		Tower();
		void LoadBitmap();
		virtual void OnMove();
		virtual void OnShow();
		void SetEnemies(vector<Enemy>&);
		void SetData(int, int, clock_t, int*);
		void SetDirection(int);
		void Attack(int);
		void IconReset();
		bool CheckEmpty(int, int);
		bool Attackable(int,int);
		virtual bool IsInRange(int ,int ,int ,int) = 0;
		double Range(int, int, int, int);
		CMovingBitmap icon, char_l, char_r;
		Projectile projectile;
		vector<pair<int, int>> pos;
		vector<int> sp, blockCount, hp;
		vector<clock_t> time;
		vector<Enemy*> target, enemies, blockedEnemies;
		bool findTarget, IsActive, IsSkillActive;
		int MaxSp, iconX, cost, MaxBlockCount,MaxHp;
		double atk_speed;
		enum Direction { Up, Down, Left, Right };
		Direction direction;
		HpBar hpBar;
	};
}