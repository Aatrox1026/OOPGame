namespace game_framework {
	class Projectile {
	public:
		Projectile();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void SetTarget(Enemy&);
		void SetTarget(vector<Enemy>&);
		void SetPosition(int, int);
		void SetPicNumber(int);
		void SetDmg(int);
		void DealDmg();
		Enemy *target;
		int x, y, picNumber;
		CMovingBitmap pic;
		double angle;
		vector<pair<int, int>> pos;
	private:
		double Range(int, int, int, int);
		int dmg;
	};
}