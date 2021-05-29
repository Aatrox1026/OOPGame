#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Icon.h"

namespace game_framework {
	void Icon::LoadBitmap() {
		Warfarin.LoadBitmap(IDB_WARFARIN,RGB(255,255,255));
		Reset();
	}

	void Icon::OnShow() {
		Warfarin.ShowBitmap();
	}

	void Icon::Reset() {
		Warfarin.SetTopLeft(496, 432);
	}
}