namespace game_framework {
	class Defender : public Tower {
	public:
		Defender();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		bool IsInRange(int, int, int, int);
	};
}