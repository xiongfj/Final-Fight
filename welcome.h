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


// 选角色，返回角色值。

int Chose_role()
{
	int play_1 = 0, play_2 = 0;
	bool done_1 = false, done_2 = false;
	int y[3] = { 145, 255, 370 };

	if ( Welcome() == 1 )
	{
		loadimage( NULL, "./res/Images/1player.jpg", 500, 450 );

		fillcircle(  35, y[play_1], 8 );

		while ( !done_1 )
		{
			Sleep(10);

			if ( GetAsyncKeyState('W') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				if ( --play_1 < 0 )						play_1 = 2;
				fillcircle( 35, y[play_1], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState('S') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				if ( ++play_1 > 2 )						play_1 = 0;
				fillcircle( 35, y[play_1], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState('J') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				done_1 = true;
				Sleep(155);
			}
		}
		setwritemode( R2_COPYPEN );
		return ( 30 + play_1 );	// 如果返回值大于22，表示只有一个玩家，后面那个值就是角色值
	}
	else
	{
		loadimage( NULL, "./res/Images/2player.jpg", 500, 450 );

		setbkmode( TRANSPARENT );
		outtextxy(  60, 40, "player 1" );
		outtextxy( 300, 40, "player 2" );

		fillcircle(  35, y[play_1], 8 );
		fillcircle( 265, y[play_2], 8 );

		while ( !done_1 || !done_2 )
		{
			Sleep(10);

			if ( GetAsyncKeyState('W') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				play_1 = ( --play_1 < 0 ) ? 2 : play_1;
				fillcircle( 35, y[play_1], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState('S') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				play_1 = ( ++play_1 > 2 ) ? 0 : play_1;
				fillcircle( 35, y[play_1], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState('J') & 0x8000 && !done_1 )
			{
				fillcircle( 35, y[play_1], 8 );
				done_1 = true;
				Sleep(155);
			}

			/************************************************/

			if ( GetAsyncKeyState(VK_UP) & 0x8000 && !done_2 )
			{
				fillcircle( 265, y[play_2], 8 );
				play_2 = ( --play_2 < 0 ) ? 2 : play_2;
				fillcircle( 265, y[play_2], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState(VK_DOWN) & 0x8000 && !done_2 )
			{
				fillcircle( 265, y[play_2], 8 );
				play_2 = ( ++play_2 > 2 ) ? 0 : play_2;
				fillcircle( 265, y[play_2], 8 );
				Sleep(155);
			}

			if ( GetAsyncKeyState(VK_NUMPAD1) & 0x8000 && !done_2 )
			{
				fillcircle( 265, y[play_2], 8 );
				done_2 = true;
				Sleep(155);
			}
		}
		setwritemode( R2_COPYPEN );
		return ( play_1 * 10 + play_2 );	// 返回值前面代表玩家1，后面代表玩家2，
	}
}


#endif