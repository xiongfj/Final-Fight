#pragma once

#ifndef WELCOME_H
#define WELCOME_H

#include "graphics.h"
#include <conio.h>

// 返回玩家数 1、2

int Welcome()
{
	loadimage( NULL, "./res/Images/w1.jpg", 500, 450 );
	int player_number = 1;

	setwritemode( R2_XORPEN );
	setfillcolor( BLUE );
	bar( 60, 250, 220, 300 );
	bool game = true;

	while ( game )
	{
		Sleep(10);

		if ( GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000 )
		{
			bar( 60, 250, 220, 300 );
			bar( 250, 250, 410, 300 );
			player_number = (++player_number > 2) ? 1 : 2;
			Sleep(165);
		}

		if ( GetAsyncKeyState(VK_RETURN) & 0x8000 )
			return player_number;
	}
	return 0;
}

struct select_rect
{
	byte index : 6;
	byte index_2 : 6;
}s_rect;

// 选角色，返回角色值。

int Chose_role()
{
	int play_1 = 0, play_2 = 0;
	bool done_1 = false, done_2 = false;
	int y[3] = { 145, 255, 370 };
	int yy[3] = {90, 203, 316};
	IMAGE bg_img;
	IMAGE img[2][2];
	loadimage(&img[0][0], "./res/Images/rect_1_0.gif", 104, 108);
	loadimage(&img[0][1], "./res/Images/rect_1_1.gif", 104, 108);
	loadimage(&img[1][0], "./res/Images/rect_2_0.gif", 104, 108);
	loadimage(&img[1][1], "./res/Images/rect_2_1.gif", 104, 108);
	s_rect.index = 0;
	s_rect.index_2 = 0;

	if ( Welcome() == 1 )
	{
		loadimage( &bg_img, "./res/Images/1player.jpg", 500, 450 );

		BeginBatchDraw();
		while ( !done_1 )
		{
			Sleep(10);
			if ( GetAsyncKeyState('W') & 0x8000 && !done_1 )
			{
				if ( --play_1 < 0 )						play_1 = 2;
				Sleep(93);
			}

			if ( GetAsyncKeyState('S') & 0x8000 && !done_1 )
			{
				if ( ++play_1 > 2 )						
					play_1 = 0;
				Sleep(93);
			}

			if ( GetAsyncKeyState('J') & 0x8000 && !done_1 )
			{
				done_1 = true;
				Sleep(155);
			}
			
			putimage(0, 0, &bg_img);		// 背景图片 
			// 交替闪烁 rect 选标
			putimage( 41, yy[play_1], &img[ (s_rect.index++/19)%2 ][0], SRCAND );
			putimage( 41, yy[play_1], &img[ (s_rect.index++/19)%2 ][1], SRCINVERT );
			FlushBatchDraw();
		}
		setwritemode( R2_COPYPEN );
		return ( 30 + play_1 );	// 如果返回值大于22，表示只有一个玩家，后面那个值就是角色值
	}
	else
	{
		loadimage( &bg_img, "./res/Images/2player.jpg", 500, 450 );

		setbkmode( TRANSPARENT );
		outtextxy(  60, 40, "player 1" );
		outtextxy( 300, 40, "player 2" );
		int player_1_count = 0, player_2_count = 0;	// 计数多少次按键响应一次

		BeginBatchDraw();
		while ( !done_1 || !done_2 )
		{
			Sleep(10);
			player_1_count++;
			player_2_count++;
			if ( GetAsyncKeyState('W') & 0x8000 && !done_1 && player_1_count > 10 )
			{
				player_1_count = 0;
				play_1 = ( --play_1 < 0 ) ? 2 : play_1;
			}

			if ( GetAsyncKeyState('S') & 0x8000 && !done_1 && player_1_count > 10 )
			{
				player_1_count = 0;
				play_1 = ( ++play_1 > 2 ) ? 0 : play_1;
			}

			if ( GetAsyncKeyState('J') & 0x8000 && !done_1 && player_1_count > 10 )
			{
				player_1_count = 0;
				done_1 = true;
			}
			putimage(0, 0, &bg_img);		// 背景图片 
			if ( !done_1 )					// 交替闪烁 rect 选标
			{
				putimage( 41, yy[play_1], &img[ (s_rect.index++/19) % 2 ][0], SRCAND );
				putimage( 41, yy[play_1], &img[ (s_rect.index++/19) % 2 ][1], SRCINVERT );
			}
			else
			{
				putimage( 41, yy[play_1], &img[0][0], SRCAND );
				putimage( 41, yy[play_1], &img[0][1], SRCINVERT );
			}
			FlushBatchDraw();

			/************************************************/

			if ( GetAsyncKeyState(VK_UP) & 0x8000 && !done_2 && player_2_count > 10 )
			{
				player_2_count = 0;
				play_2 = ( --play_2 < 0 ) ? 2 : play_2;
			}

			if ( GetAsyncKeyState(VK_DOWN) & 0x8000 && !done_2 && player_2_count > 10  )
			{
				player_2_count = 0;
				play_2 = ( ++play_2 > 2 ) ? 0 : play_2;
			}

			if ( GetAsyncKeyState(VK_NUMPAD1) & 0x8000 && !done_2 && player_2_count > 10  )
			{
				player_2_count = 0;
				done_2 = true;
			}

			if ( !done_2 )
			{
				putimage( 269, yy[play_2], &img[ (s_rect.index_2++/19)%2 ][0], SRCAND );
				putimage( 269, yy[play_2], &img[ (s_rect.index_2++/19)%2 ][1], SRCINVERT );
			}
			else
			{
				putimage( 269, yy[play_2], &img[0][0], SRCAND );
				putimage( 269, yy[play_2], &img[0][1], SRCINVERT );
			}
			FlushBatchDraw();
		}
		setwritemode( R2_COPYPEN );
		return ( play_1 * 10 + play_2 );	// 返回值前面代表玩家1，后面代表玩家2，
	}
}


#endif