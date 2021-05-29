/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
	// �o��class���C�����C���}�Y�e������
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
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
		Sleep(300);
		//
		// �}�l���J���
		//				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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
			GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// �K�Wlogo
		//
		/*logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
		logo.ShowBitmap();*/
		//
		// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
		//
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
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
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �̲׶i�׬�100%
		//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
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
		CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		//
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// ���ʭI���Ϫ��y��
		//
		// �P�_���l�O�_�I��y
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

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
		//
		// �}�l���J���
		//
		levels.LoadBitmap(IDB_LEVELS);
		//
		// ��������Loading�ʧ@�A�����i��
		//
		ShowInitProgress(50);
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �~����J��L���
		//
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
		//
		// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
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

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
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

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
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

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
	}

	void CGameStateRun::OnShow()
	{
		//
		//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
		//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
		//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
		//
		//
		//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
		//			// �K�W�I����
		//help.ShowBitmap();					// �K�W������
		//
		//  �K�W���W�Υk�U��������
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