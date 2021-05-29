namespace game_framework {
	class Vanguard : public Tower {
	public:
		Vanguard();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void SetCost(int*);
		bool IsInRange(int, int, int, int);
	private:
		int *currentCost;
	};
}