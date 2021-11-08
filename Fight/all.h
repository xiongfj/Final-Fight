#pragma once

#ifndef STR_CLA_H
#define STR_CLA_H

#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define MAX_MAP	5				// 地图数目
#define LEFT	0
#define RIGHT	1
#define UP		2
#define DOWN	3
#define R_U		4				// 右上
#define R_D		5				// 右下
#define L_U		6				// 左上
#define L_D		7				// 左下

#define MINX	-20				// 玩家活动左右范围
#define MAXX	440

enum THROW_DIR { FORWARD = 0x101, 
				 BACK = 0x201 };		// 玩家甩人方向

enum SKILL { LIGHT_WAVE,				// 光波
			 THUMP,						// 倒插
			 THROW_BACK,				// 背摔
			 NONE };					// 空

enum PE { PE_player, PE_emeny };		// 用于 Check_hole() 函数
#define _IN		true					// 进洞
#define _UNIN	false					// 没进


/////// 地图类 /////////////////////////////////////////////////////
class MAP
{
	int line_1[MAX_MAP][9];							// 每种地图八部分的 9 条分界线

	int line_2[MAX_MAP][8];							// 每部分里面的分界线

	IMAGE img[MAX_MAP];								// 储存每一种地图

	int width[MAX_MAP];								// 地图的宽不一样

	int xx, yy;
	int num;										// 函数外部数据


public:
	MAP();
	void Init();		// 替换构造函数中 loadimage，不然异常，IMAGE 不能太早使用？！

	bool Set_Map();								// 设置地图的变化
	bool Move_map( bool check = false );		// 移动地图，受主角移动控制
	bool Check( int );							// 检测人物能否移动到该高度

	bool move;									// 地图移动(不包含判断)
	bool die_out[MAX_MAP][8];					// 每种地图的各部分敌人是否死光，需关联另一个类
	int map_id;									// 当前地图 id，过关后增加
	int part[MAX_MAP];							// 每种地图显示在窗口的当前部分(0-7)
	int max_y[MAX_MAP][8], min_y[MAX_MAP][8];	// 每种地图的 8 个不同部分可以移动的上下宽度
	int x[MAX_MAP];								// 每种地图的左上角 x 坐标
	int y;										// 为了制作倒插地图震动效果

};


//////////////////////////////////////////////////////////////////////////////////
class Role
{
public:
	Role();
	virtual ~Role()	{}
	virtual bool Check_n_x	( int );											// 检测玩家是否处于跳跃、大招等不受伤害的状态
	virtual int  Getn_a()		{ return n_a + n_ja; }							// Kady不覆盖该函数
	virtual int  Get_wavex()	{ printf("Role基类\n出错");return 0; }
	virtual int  Get_wavey()	{ printf("Role基类2\n出错");return 0; }
	virtual IMAGE* GetImg( int, int, int ) { printf("Role基类储出错3\n出错");return 0; }
	virtual int Get_weapx()	{ MessageBox(NULL, _T("virtual int Get_weapx()出错"), _T("ds"), MB_OK); return 0; }
	virtual int Get_weapy()	{ MessageBox(NULL, _T("virtual int Get_weapy()出错"), _T("ds"), MB_OK); return 0; }

	virtual void Stand		( int dir, int player );					// 通用
	virtual void Walk		( int dir, int player ) = 0;				// 只是指引作用
	virtual void Walk_all	( int dir, int player, int* x, int* y );	// 三者通用
	virtual void Jump		( int dir, int player );					// 勉强通用
	virtual void Attack		( int dir, int player );					// Jack 子类不能使用
	virtual void Weapon_attack( int dir, int player ) = 0;				// 武器攻击
	virtual void Big_Blow	( int dir, int player );					// Jack 子类不能使用

	virtual void Light_wave	( int dir, int player ) = 0;				// 三者不宜
	virtual void Dragon_fist( int dir, int player ) = 0;				// 三者不宜
	virtual void Throw_role	( int dir, int player );					// Jack 不宜
	virtual void Jump_forward(int dir, int player );					// 勉强通用
	virtual void Jump_attack( int dir, int player );					// 通用
	virtual void Jump_knee	( int dir, int player );					// Jack 不宜
	virtual void Knee_attack( int dir, int player );					// 通用
	virtual void Kick_down	( int dir, int player );					// 通用
	virtual void Revive		( int dir, int player );					// 通用
	virtual void Be_kiss	( int dir, int player );					// 通用，第三关被 boss kiss

	int x, y;
	int temp_y;
	byte Dir : 1;

	bool have_weapon;			// 当前是否获得武器
	byte weapon_counter : 4;	// 攻击 15 次后武器消失

	int w, a, j, ja, bb, jk, jf, lw, kd, df, tr, cm, ka, r, bk;
	int n_w, n_a, n_j, n_ja, n_bb, n_jk, n_jf, n_lw, n_kd, n_df, n_tr, n_cm, n_ka, n_r, n_bk;

protected:
	int X, Y;
	int Drtx;
	bool big;
	int temp_x;
};

//、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
class Kady : public Role
{
	int wave_x, wave_y;			// 冲击波左上角坐标，需覆盖基类的函数来获取

	IMAGE walk[2][4][2], attack[2][2][2], jump[2][3][2], jump_attack[2][4][2];
	IMAGE b_blow[2][4][2], jump_knee[2][3][2], jump_forward[2][7][2], light_wave[2][10][2];
	IMAGE kick_down[2][6][2], dragon_fist[2][8][2], throw_role[2][5][2], catch_attack[2][6][2];
	IMAGE knife[2][3][2];		// 小刀

public:
	Kady();
	bool shoot_knife;			// 小刀攻击次数用光，就发射小刀

	int Get_wavex()	{ return wave_x; }
	int Get_wavey()	{ return wave_y; }

	IMAGE* GetImg( int dir, int n, int kind )	{ return &kick_down[dir][n][kind]; }

	void Stand		( int dir, int player );
	void Walk		( int dir, int player );
	void Jump		( int dir, int player );
	void Attack		( int dir, int player );
	void Weapon_attack( int dir, int player );
	void Big_Blow	( int dir, int player );

	void Light_wave	( int dir, int player );
	void Dragon_fist( int dir, int player );
	void Throw_role	( int dir, int player );
	void Jump_forward(int dir, int player );
	void Jump_attack( int dir, int player );
	void Jump_knee	( int dir, int player );
	void Knee_attack( int dir, int player );
	void Kick_down	( int dir, int player );
	void Revive		( int dir, int player );
	void Be_kiss	( int dir, int player );
};

//、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
class Heye : public Role
{
	IMAGE walk[2][4][2], attack[2][7][2], jump[2][3][2], jump_attack[2][3][2];
	IMAGE b_blow[2][4][2], jump_knee[2][3][2], jump_forward[2][7][2], light_wave[2][7][2];
	IMAGE kick_down[2][6][2], dragon_fist[2][7][2], throw_role[2][7][2], catch_attack[2][6][2];
	IMAGE weapon[2][3][2];		// 飞镖攻击

public:
	Heye();

	int weapx, weapy;			// 飞镖坐标

	int Getn_a()		{ return (n_a + n_lw + n_ja); }
	int Get_weapx()		{ return weapx; }
	int Get_weapy()		{ return weapy; }
	IMAGE* GetImg( int dir, int n, int kind )	{ return &kick_down[dir][n][kind]; }

	void Stand		( int dir, int player );
	void Walk		( int dir, int player );
	void Jump		( int dir, int player );
	void Attack		( int dir, int player );
	void Weapon_attack( int dir, int player );
	void Big_Blow	( int dir, int player );

	void Light_wave	( int dir, int player );	// 连环腿
	void Dragon_fist( int dir, int player );	// 倒钩脚
	void Throw_role	( int dir, int player );
	void Jump_forward(int dir, int player );
	void Jump_attack( int dir, int player );
	void Jump_knee	( int dir, int player );
	void Knee_attack( int dir, int player );
	void Kick_down	( int dir, int player );
	void Revive		( int dir, int player );
	void Be_kiss	( int dir, int player );
};

//、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
class Jack : public Role
{
	bool thump;				// 倒插--重击

	IMAGE walk[2][4][2], attack[2][3][2], jump[2][3][2], jump_attack[2][3][2];
	IMAGE b_blow[2][4][2], jump_knee[2][3][2], jump_forward[2][3][2], light_wave[2][2][2];
	IMAGE kick_down[2][6][2], dragon_fist[2][5][2], throw_role[2][4][2], catch_attack[2][6][2];
	IMAGE hammer[2][3][2];	// 锤子攻击

public:
	Jack();

	int Getn_a()		{ return (n_a + n_lw + n_ja); }
	IMAGE* GetImg( int dir, int n, int kind )	{ return &kick_down[dir][n][kind]; }

	void Stand		( int dir, int player );
	void Walk		( int dir, int player );
	void Jump		( int dir, int player );
	void Attack		( int dir, int player );
	void Weapon_attack( int dir, int player );
	void Big_Blow	( int dir, int player );

	void Light_wave	( int dir, int player );	// 飞扑
	void Dragon_fist( int dir, int player );	// 双脚攻击
	void Throw_role	( int dir, int player );	// 背摔
	void Jump_forward(int dir, int player );
	void Jump_attack( int dir, int player );
	void Jump_knee	( int dir, int player );	// 泰山压顶
	void Knee_attack( int dir, int player );	// 用头攻击
	void Kick_down	( int dir, int player );
	void Revive		( int dir, int player );
	void Be_kiss	( int dir, int player );
};


/******************** 敌人类 *******************/
#define EMENY_NUM	6		// 地图每段最多出 6 人

////////////// 抽象基类 /////////////////////////////////////////
class BASE_CLASS
{
public:
	virtual ~BASE_CLASS()	{}

	virtual int  Getx	()		= 0;
	virtual int  Gety	()		= 0;
	virtual void Setxy( int _x, int _y )	= 0;
	virtual int  GetDir	()		= 0;
	virtual void SetDir	( int )	= 0;
	virtual void SetHp	( int& )	= 0;
	virtual void Out_Put( int id )  = 0;

	virtual void Stand		( int id, const int& player ) = 0;		// 偶尔静止，被玩家膝盖击中也静止
	virtual void Walk		( int id, const int& player ) = 0;
	virtual void Attack		( int id, const int& player ) = 0;
	virtual void Kick_away	( int id, const int& player ) = 0;		// 被击飞
	virtual void Throw_away	( int id, const int& player ) = 0;		// 被甩飞
	virtual void Be_catch	( int id, const int& player ) = 0;		// 被玩家抓住
	virtual void Die		( int id, const int& player ) = 0;		// 敌人被搞死
};

////////// 小不点猴仔 /////////////////////////////////////////////
class Small_Monkey : public BASE_CLASS
{
	int X, Y;				// 被击飞抛物线顶点
	int temp_x;				// 处理越界情况
	int Drtx[2];
	int attack_level[6];	// 6 个地图里面的攻击概率的 分母, rand() % 分母。

	int x, y, temp_y;		// img 左上角坐标
	int dx1, dy1;			// 倾斜运动分量
	int dx2;				// 水平运动分量，无 y
	int step;				// 当前移动的步数
	int role;				// 向该号玩家靠近
	double _a;				// 敌人相对玩家所在的象限
	byte Dir : 1;			// 左右方向

	bool start;				// 开始新的一段运功
	bool leave;				// 远离玩家

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;

	DWORD w_t1, w_t2;		// 侯仔移动速度
	DWORD w_dt[6];

	static IMAGE walk[2][4][2], attack[2][2][2], kick_away[2][6][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Small_Monkey();
	int  Getx	()			{ return x;		}
	int  Gety	()			{ return y;		}
	int	 GetDir	()			{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir	( int dir )	{ Dir = dir;	};
	void SetHp	( int& hp );
	void Out_Put(int id){ UNREFERENCED_PARAMETER(id); }

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};

/////// 小呆 ///////////////////////
class Small_Dai : public BASE_CLASS
{
	int X, Y;		// 被击飞抛物线顶点
	int temp_x;		// 处理越界情况
	int Drtx[2];
	int attack_level[6];

	int x, y, temp_y;
	int dx1, dy1;
	int dx2;
	int step;
	int role;
	double _a;
	byte Dir : 1;

	bool start;
	bool leave;

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;
	DWORD w_t1, w_t2;
	DWORD w_dt[6];

	static IMAGE walk[2][5][2], attack[2][2][2], kick_away[2][5][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Small_Dai();
	int Getx()	{ return x; }
	int Gety()	{ return y; }
	int GetDir()	{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir( int dir )	{ Dir = dir; };
	void SetHp	( int& hp );
	void Out_Put(int id){ UNREFERENCED_PARAMETER(id); }

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};

////// 大呆 ///////////////////////
class Big_Dai : public BASE_CLASS
{
	int X, Y;		// 被击飞抛物线顶点
	int temp_x;		// 处理越界情况
	int Drtx[2];
	int attack_level[6];

	int x, y, temp_y;
	int dx1, dy1;
	int dx2;
	int step;
	int role;
	double _a;
	byte Dir : 1;

	bool start;
	bool leave;
	bool resist;		// 抵挡

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;
	DWORD w_t1, w_t2;
	DWORD w_dt[6];

	static IMAGE walk[2][5][2], attack[2][2][2], kick_away[2][6][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Big_Dai();
	int Getx()		{ return x;		}
	int Gety()		{ return y;		}
	int GetDir()	{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir( int dir )	{ Dir = dir; };
	void SetHp	( int& hp );
	void Out_Put(int id){ UNREFERENCED_PARAMETER(id); }

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};

//// 妹子 //////////////////////////////
class Girl : public BASE_CLASS
{
	int X, Y, Drtx[2], temp_x;
	int x, y, temp_y;
	int dx1, dy1;
	int dx2;
	int attack_level[6];

	int step;
	int role;
	double _a;
	byte Dir : 1;
	byte attack_kind : 1;	// 攻击方式

	bool start;
	bool leave;

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;
	DWORD w_t1, w_t2;
	DWORD w_dt[6];

	static IMAGE walk[2][4][2], attack[2][8][2], kick_away[2][5][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Girl();
	int Getx()		{ return x;		}
	int Gety()		{ return y;		}
	int GetDir()	{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir( int dir )	{ Dir = dir; };
	void SetHp	( int& hp );
	void Out_Put(int id){ UNREFERENCED_PARAMETER(id); }

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};

//// 廋老头 ///////////////////////
class Thin_Old_Man : public BASE_CLASS
{
	int x, y;
	int temp_y;					// 空中 y 值，用于跳跃性质的运动
	int knife_x, knife_y;		// 飞刀坐标
	int k_dir;					// 飞刀方向

	int X, Y;
	int temp_x;					// 处理越界情况
	int dx1, dy1;
	int dx2;
	int Drtx[2];
	int attack_level[6];
	int step;
	int role;
	double _a;
	byte Dir : 1;
	int attack_kind;	

	bool start;
	bool leave;
	bool turn_dir;				// 飞刀被玩家击中反向，可攻击敌人

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;
	DWORD w_t1, w_t2;
	DWORD w_dt[6];

	static IMAGE walk[2][5][2], attack[2][10][2], kick_away[2][5][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Thin_Old_Man();
	int Getx()		{ return x;		}
	int Gety()		{ return y;		}
	int GetDir()	{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir( int dir )	{ Dir = dir; };
	void SetHp	( int& hp );
	void Out_Put(int id);

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};

///// 大个子野人 //////////////////////////
class Big_Savage : public BASE_CLASS
{
	int X, Y, temp_x;
	int x, y, temp_y;
	int dx1, dy1;
	int dx2;

	int Drtx[2];
	int attack_level[6];
	int step;
	int role;
	double _a;
	byte Dir : 1;
	byte attack_kind : 1;

	bool start;
	bool leave;

	int w, a, ka, ta, s, bc, d;
	int n_w, n_a, n_ka, n_ta, n_s, n_bc, n_d;
	DWORD w_t1, w_t2;
	DWORD w_dt[6];

	static IMAGE walk[2][5][2], attack[2][4][2], kick_away[2][5][2], throw_away[2][5][2], be_catch[2][5][2];
	static int Hp[6][7];

public:
	Big_Savage();
	int Getx()		{ return x;		}
	int Gety()		{ return y;		}
	int GetDir()	{ return Dir;	}
	void Setxy( int _x, int _y )	{ x = _x; y = _y; }
	void SetDir( int dir )	{ Dir = dir; }
	void SetHp	( int& hp );
	void Out_Put(int id){ UNREFERENCED_PARAMETER(id); }

	void Stand		( int id, const int& player );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Be_catch	( int id, const int& player );
	void Die		( int id, const int& player );

	static void Load();
};


/************* boss 类 **********************/
#define BOSS_NUM	2			// boss 数量

class BASE_BOSS
{
public:
	BASE_BOSS( int* p, RECT* rec );								// 使用一个数组参数初始化各关 boss
	virtual ~BASE_BOSS		(							)	{}
	virtual void Walk		( int id, const int& player );		// 个别可共用
	virtual void Attack		( int id, const int& player ) = 0;	// 不可共用
	virtual void Kick_away	( int id, const int& player );		// 通用
	virtual void Throw_away	( int id, const int& player );		// 通用
	virtual void Throw_back	( int id, const int& player );		// 通用，胖子背摔
	virtual void Die		( int id, const int& player );		// 通用

	int x, y;
	int temp_y;
	byte Dir : 1;			// boss 正面朝向，走路时永远朝向玩家

protected:					// 保护型可被公有派生类成员函数直接访问
	int X, Y;
	int Drtx[2];
	int temp_x;

	int a_kind;				// boss 攻击种类

	int w, a, ka, ta, tb, d;
	int n_w, n_a, n_ka, n_ta, n_tb, n_d;

	bool start;				// 开始新一段追击
	RECT* rec;				// boss 运动范围
	byte move_dir : 2;		// boss 运动方向
	DWORD w_t1, w_t2, w_dt;
};

//
class B_ONE : public BASE_BOSS
{
	IMAGE walk[2][4][2], attack[2][6][2], kick_away[2][5][2], throw_away[2][5][2];//, throw_back[2][5][2];

public:
	B_ONE( int* p, RECT* rec );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Throw_back	( int id, const int& player );
	void Die		( int id, const int& player );
};

//
class B_TWO : public BASE_BOSS
{
	IMAGE walk[2][4][2], attack[2][3][2], kick_away[2][5][2], throw_away[2][5][2];

public:
	B_TWO( int* p, RECT* rec );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Throw_back	( int id, const int& player );
	void Die		( int id, const int& player );
};

//
class B_THREE : public BASE_BOSS
{
	bool kiss;		// 当前是否 kiss 着玩家
	bool leave;
	double _a;
	int dx1, dx2, dy1;
	int step;
	IMAGE walk[2][4][2], attack[2][12][2], kick_away[2][5][2], throw_away[2][5][2];

public:
	B_THREE( int* p, RECT* rec );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Throw_back	( int id, const int& player );
	void Die		( int id, const int& player );
};

//
class B_FOUR : public BASE_BOSS
{
	double _a;
	// 与 boss1 共用图片
	IMAGE walk[2][4][2], attack[2][9][2], kick_away[2][5][2], throw_away[2][5][2];

public:
	B_FOUR( int* p, RECT* rec );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Throw_back	( int id, const int& player );
	void Die		( int id, const int& player );
};

//
class B_FIVE : public BASE_BOSS
{
	IMAGE walk[2][4][2], kick_away[2][5][2], throw_away[2][5][2];
public:
	static IMAGE attack[2][14][2];		// 供 LINK 类的 PutImg() 使用
	static IMAGE bullet_away[3][2];		// 子弹被击飞

public:
	B_FIVE( int* p, RECT* rec );
	void Walk		( int id, const int& player );
	void Attack		( int id, const int& player );
	void Kick_away	( int id, const int& player );
	void Throw_away	( int id, const int& player );
	void Throw_back	( int id, const int& player );
	void Die		( int id, const int& player );
};



////
////	链表
////
//// 最后一关 boss 发射拳头的链表类

class LINK
{
public:
	LINK( int _x = 0, int _y = 0, byte r = LEFT ):
	  x(_x),y(_y),n_a(0),a(10),dir(r)
	  {}
	  LINK( bool flag, int xx=0, int yy=0, byte r=LEFT ):
	  b_x(xx), b_y(yy), b_r(r), n_a(0), a(0)
	  { UNREFERENCED_PARAMETER(flag); }

	void PutImg();
	void Kick_down();

private:
	int b_x, b_y;			// 拳头被击落
	int x, y;				// 铁拳图片左上角坐标
	int n_a, a;				// a 为 IMAGE 下标
public:
	LINK* pP;				// 指向上一个对象
	LINK* p_B;
	byte dir : 1;
	byte b_r : 1;			// 拳头被弹飞方向

	int X, Y;
};

struct L
{
	static LINK* pn;
	static LINK* pnem;		// 用于连接链表

	static LINK* p_b;
	static LINK* p_temp;
};


/************ BOSS 结构 ***************/

struct BOSS
{
	byte role;
	BASE_BOSS* Bp;
	IMAGE* ph;			//

	int x, y;			//
	int hp;
	int max_hp;			//
	int be_kick_counter;

	bool die;			//
	bool appear;		//
	bool start_fly;
	bool lying;

	static IMAGE head[5];	//	貌似有 7 个 boss
	static int Boss_num;

}Boss[BOSS_NUM];

// 敌人结构体 ///////////////////////////////////////
struct EMENY
{
	BASE_CLASS* Mp;			// 敌人类型
	byte role : 1;			// 当前敌人关联的玩家
	IMAGE* phead;			// 指向敌人的头像

	int x, y;				// 头像左上角坐标
	static IMAGE head[6];	// 敌人头像
	static IMAGE papaw[2];		// 泡泡语
	static int emeny_num;	// 最多六个;

	bool stand;				// 用于标记被玩家跳膝攻击后静止状态
	bool attack;
	bool lying;				// 当前敌人是否躺着，不受伤害
	bool be_catch;			// 是否被抓住
	bool start_fly;			// 敌人开始播放被击飞画面
	bool appear[8];			// 该敌人在地图每段中是否出现

	int live[MAX_MAP][7];	// 该敌人在地图每段中的生命 最大值 2 ！！！！
	int be_kick_counter;	// 记录被击中次数，第 4 次被击飞

	int hp;					// 敌人当前血量
	int max_hp;				// 用于记录当前满血量

}Emeny[EMENY_NUM];

////////////////////////////////////////
struct PLAYER
{
	Role* Rp;						// 玩家所选的角色
	THROW_DIR throw_dir;			// 玩家甩人方向
	SKILL skill;					// 用于检测当前玩家释放何种技能
	IMAGE* ph;						// 指向玩家头像
	byte level : 3;					// 玩家级别，最高 7 级

	bool be_kiss;					// 用于 boss 被击飞释放正被 kiss 的玩家
	bool lying;						// 用与 boss 判断攻击生效与否
	bool appear;					// 玩家生命用光后不出现
	bool die;						// 玩家是否死翘翘
	bool catch_emeny;				// 是否抓着敌人
	bool bol[6];					// 控制 6 个按键当前能否按下 0~5 依次对应：左/右/上/下，攻/跳
	void Set_bol( int val );		// 设置 6 个按键

	int x, y;						// 头像坐标
	int dir;						// 两个玩家当前方向
	int be_kick_counter;			// 记录被攻击次数，3 次为一循环(被搞飞)
	int emeny;						// 玩家绑定的敌人
	int boss;						// 用于 Be_kiss() 函数
	int role_kind;					// 记录玩家选择的角色

	int experience;					// 玩家当前经验
	int hp;							// 血量
	int max_hp;						// 用于画血框
	int life;						// 玩家生命

	static int exp_lev[7];			// 每个级别所需经验
	static int player_num;			// 当前玩家数量 1 - 2
	static IMAGE light[2];			// 攻击闪光效果
	static IMAGE blood[2][6][2];	// 制作喷血效果
	static IMAGE head[3];			// 玩家头像

}Player[2];

// 游戏级别信息结构
struct LEVEL
{
	static bool next_lev;			// 播放进入下一关的画面
	static double run_fly[8];		// jack 八个飞扑级别加成
	static double wave[8];			// kady 八个级别冲击波加成
	static int a_level;				// 敌人攻击概率
	static int walk_dt;				// 敌人移动间隔时间
	static int harm[8];				// 玩家攻击力基础值，敌人攻击力不增加,

	static void Level_Up();
};

// 电梯效果
struct OTHER
{
	static bool emeny;					// true 敌人的 y > 玩家的 y， 遮住玩家，false 玩家遮住敌人
	static bool boss;
	static IMAGE lift_up_down[2];		// 制作电梯上下移动效果
	static int y[3];					// 电梯方块左上角 y 坐标
};

// 锤子，小刀，飞镖
struct WEAPON
{
	static bool out;			// 当前地图是否出现锤子，出现则检测玩家是否获得锤子
	static int X;				// 锤子出现时相对于 Map.x[Map.map_id] 的坐标，用于保持武器跟随地图运动 X = Emeny[i].x - Map.x[...]
	static int x, y;			// 锤子坐标，x = X + Map.x[..]
	static IMAGE weapon[3][2];	// 一次为：小刀、飞镖、锤子,对应 PLAYER 结构的 role_kind
	static int weapon_kind;		// 武器种类，玩家获取时 role_kind == weapon 才能获取
	static int time;			// 武器已经出现的时间，一定时间后消失
};

// 鸡腿
struct FOOD
{
	static bool out;
	static IMAGE img[2];
	static int X;
	static int x, y;
	static int time;
};



/********* 一些零散函数 ********/

void SetSound( const TCHAR*, const TCHAR*, const TCHAR* );						// 设置声音
bool Check_position( const int& e_dir, const int& e_x, const int& p_x );	// 检测玩家与敌人的方位
bool Check_hole( PE kind, int id );											// 检测玩家、敌人是否掉进第三关的洞里
bool Check_xy( int xx, int yy );											// 契合上一个函数
bool Check_lift( PE kind, int id );											// 检测当前位置是否掉下电梯
bool Check_lift_xy( const int& x );											// 另一种检测，用于廋老头铲地攻击或其他
void Weapon_Out( const int&, const int&, const int& id, const int& player );// 敌人死后随机出现玩家对应的武器
int Revise_xy( int id, const int& x, const int& y, int& dx, int& dy );		// 用于第三关的 坑，修改敌人走进坑里的位移量
void Revise_dispatch( int& );												// 修正电梯敌人出现的 x 位置
bool Revise_lift(const int&, const int& );									// 修正电梯中的运动分量
int SetDir( const int& b_x, const int& p_x );								// 判断 boss 与 player 的 x 坐标方位，返回 boss 朝向( LEFT/RIGHT )
int Change_move_dir( const int& e_x, const int& e_y );						// boss 靠近边界后重新获取移动方向
bool Check_kiss_x( const int& x, const int& lef, const int& rig );			// 检测 boss 所处范围是否会 kiss 玩家越界
void Dispatch_five_boss( int part );										// 产生第五关的前两个 boss


#endif