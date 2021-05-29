namespace game_framework {
	class Sniper : public Tower {
	public:
		Sniper();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void SetData(int, int, clock_t, int*);
		bool IsInRange(int, int, int, int);
	private:
		vector<int> skillHit;
	};
}