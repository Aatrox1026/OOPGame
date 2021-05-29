/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "string"
#include <time.h>

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	CGameMap::CGameMap() :x(32), y(0), w(48), h(48) {
		int map_init[10][12] = { {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 1},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4},
								{3, 2, 2, 2, 2, 2, 4, 4, 4, 2, 4, 4},
								{4, 4, 4, 4, 4, 2, 4, 4, 4, 2, 4, 4},
								{4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 4, 4},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
								{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4} };
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 12; j++)
				map[i][j] = map_init[i][j];
	}

	void CGameMap::LoadBitmap() {
		start.LoadBitmap(IDB_START, RGB(255, 255, 255));
		ground.LoadBitmap(IDB_GROUND);
		end.LoadBitmap(IDB_END, RGB(255, 255, 255));
		highground1.LoadBitmap(IDB_HIGHGROUND1);
		highground2.LoadBitmap(IDB_HIGHGROUND2);
	}

	void CGameMap::OnShow() {
		CMovingBitmap *bitmap;
		int correction;
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 12; j++) {
				correction = 0;
				switch (map[i][j])
				{
				case 0:
					break;
				case 1:
					bitmap = &start;
					correction = -12;
					break;
				case 2:
					bitmap = &ground;
					break;
				case 3:
					bitmap = &end;
					correction = -12;
					break;
				case 4:
					bitmap = &highground1;
					correction = -12;
					break;
				case 5:
					bitmap = &highground2;
					correction = -12;
					break;
				default:
					ASSERT(0);
				}
				bitmap->SetTopLeft(x + (w*j), y + (h*i) + correction);
				bitmap->ShowBitmap();
			}
	}

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
		Sleep(300);
		//
		// 開始載入資料
		//				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
		//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// 貼上logo
		//
		/*logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
		logo.ShowBitmap();*/
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
		//
		// 開始載入資料
		//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 最終進度為100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g), NUMBALLS(28)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
	}

	void CGameStateRun::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 10;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移動背景圖的座標
		//
		// 判斷擦子是否碰到球
		//
					//
		if (state == Game) {
			cost.OnMove();
			lossCounter.CheckEnemies(enemies);
			if (lossCounter.CheckLost() || lossCounter.CheckWin(enemies))
				GotoGameState(GAME_STATE_OVER);
			static int n = 0;
			static clock_t t = clock();
			if (clock() - t >= 1000 && n != 4) {
				t = clock();
				n++;
			}
			for (int i = 0; i <= n; i++)
				enemies[i].OnMove();
			Texas.SetEnemies(enemies);
			Texas.OnMove();
			Exusiai.SetEnemies(enemies);
			Exusiai.OnMove();
			Lappland.SetEnemies(enemies);
			Lappland.OnMove();
			Eyjafjalla.SetEnemies(enemies);
			Eyjafjalla.OnMove();
			Hoshikuma.SetEnemies(enemies);
			Hoshikuma.OnMove();
		}
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
		//
		// 開始載入資料
		//
		levels.LoadBitmap(IDB_LEVELS);
		//
		// 完成部分Loading動作，提高進度
		//
		ShowInitProgress(50);
		Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//
		// 繼續載入其他資料
		//
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
		//
		// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
		//
		gamemap.LoadBitmap();
		icon.LoadBitmap();
		cost = Cost(50);
		cost.LoadBitmap();
		lossCounter.LoadBitmap();
		Texas.LoadBitmap();
		Texas.SetCost(&cost.cost);
		Exusiai.LoadBitmap();
		Lappland.LoadBitmap();
		Eyjafjalla.LoadBitmap();
		Hoshikuma.LoadBitmap();
		for (int i = 0; i < 5; i++) {
			enemies[i].LoadBitmap();
		}
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		switch (state) {
		case Level:
			if (point.x >= 10 && point.x <= 90 && point.y >= 220 && point.y <= 240) {
				level = level1;
				state = Game;
			}
			break;
		case Game:
			clickPoint = point;
			icon.Reset();
			Texas.IconReset();
			Exusiai.IconReset();
			Lappland.IconReset();
			Eyjafjalla.IconReset();
			Hoshikuma.IconReset();
			static int index;
			if (point.x >= 352 && point.y >= 432) {
				index = (point.x - 352) / 48;
				switch (index) {
				case 5:
					Hoshikuma.icon.SetTopLeft(592, 422);
					break;
				case 4:
					Eyjafjalla.icon.SetTopLeft(544, 422);
					break;
				case 3:
					icon.Warfarin.SetTopLeft(496, 422);
					break;
				case 2:
					Lappland.icon.SetTopLeft(448, 422);
					break;
				case 1:
					Exusiai.icon.SetTopLeft(400, 422);
					break;
				case 0:
					Texas.icon.SetTopLeft(352, 422);
					break;
				}
				OutputDebugString(_T((to_string(index) + " ").c_str()));
			}
			else {
				//Exusiai.IsActive = true;
				//Eyjafjalla.IsActive = true;
				int temp_x = (point.x - 32) / 48, temp_y = (point.y + 12) / 48;
				int x = (48 * (temp_x)) + 32, y = (48 * (temp_y)) - 24;
				switch (index) {
				case 5:
					if (gamemap.map[temp_y][temp_x] == 2 && cost.cost >= Hoshikuma.cost && Hoshikuma.CheckEmpty(x, y)) {
						Hoshikuma.SetData(x, y, clock(), &cost.cost);
						towerSelected = (Tower*)(&Hoshikuma);
					}
					break;
				case 4:
					if (gamemap.map[temp_y][temp_x] == 4 && cost.cost >= Eyjafjalla.cost && Eyjafjalla.CheckEmpty(x, y)) {
						Eyjafjalla.SetData(x, y, clock(), &cost.cost);
						towerSelected = (Tower*)(&Eyjafjalla);
					}
					break;
				case 3:
					break;
				case 2:
					if (gamemap.map[temp_y][temp_x] == 2 && cost.cost >= Lappland.cost && Lappland.CheckEmpty(x, y)) {
						Lappland.SetData(x, y, clock(), &cost.cost);
						towerSelected = (Tower*)(&Lappland);
					}
					break;
				case 1:
					if (gamemap.map[temp_y][temp_x] == 4 && cost.cost >= Exusiai.cost && Exusiai.CheckEmpty(x, y)) {
						Exusiai.SetData(x, y, clock(), &cost.cost);
						towerSelected = (Tower*)(&Exusiai);
					}
					break;
				case 0:
					if (gamemap.map[temp_y][temp_x] == 2 && cost.cost >= Texas.cost && Texas.CheckEmpty(x, y)) {
						Texas.SetData(x, y, clock(), &cost.cost);
						towerSelected = (Tower*)(&Texas);
					}
					break;
				}
			}
			break;
		default:
			break;
		}

	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		if (towerSelected) {
			towerSelected->IsActive = true;
			int xOffset = point.x - clickPoint.x, yOffset = point.y - clickPoint.y;
			if (abs(xOffset) > abs(yOffset)) {
				if (xOffset >= 0)
					towerSelected->SetDirection(3);
				else
					towerSelected->SetDirection(2);
			}
			else {
				if (yOffset >= 0)
					towerSelected->SetDirection(1);
				else
					towerSelected->SetDirection(0);
			}
			towerSelected = nullptr;
			//OutputDebugString(_T(("\n" + to_string(towerSelected->IsActive) + "\n").c_str()));
		}
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//			// 貼上背景圖
		//help.ShowBitmap();					// 貼上說明圖
		//
		//  貼上左上及右下角落的圖
		//
		switch (state)
		{
		case Level:
			levels.SetTopLeft(0, 0);
			levels.ShowBitmap();
			break;
		case Game:
			gamemap.OnShow();
			Texas.OnShow();
			Exusiai.OnShow();
			Lappland.OnShow();
			Eyjafjalla.OnShow();
			Hoshikuma.OnShow();
			for (int i = 0; i < 5; i++) {
				enemies[i].OnShow();
			}
			icon.OnShow();
			cost.OnShow();
			lossCounter.OnShow();
			
			break;
		default:
			break;
		}
	}
}