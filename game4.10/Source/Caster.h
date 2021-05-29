namespace game_framework {
	class Caster : public Tower {
	public:
		Caster();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		bool IsInRange(int, int, int, int);
	};
}