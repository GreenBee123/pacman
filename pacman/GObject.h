#include"stdafx.h"
#include<time.h>
#pragma once

#include"GMap.h"

#define PLAYERSPEED 6 //玩家速度
#define ENERMYSPEED 4 //敌人速度
#define LEGCOUNTS 5//敌人腿的数量
#define DISTANCE 10//图形范围
#define BLUE_ALERT 8//蓝色警戒范围
#define D_OFFSET 2//绘图误差
#define RD (DISTANCE + D_OFFSET)//绘图范围

enum TWARDS//方向枚举
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	OVER,//游戏结束
};

class GObject//物体类：大嘴和敌人的父类
{
public:
	GObject(int Row, int Array)
	{
		m_nFrame = 1;//帧数
		pStage = NULL;//当前关卡
		this->m_nRow = Row;//行
		this->m_nArray = Array;//组数
		//中心位置
		this->m_ptCenter.x = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.y = m_nArray * pStage->LD + pStage->LD / 2;

		this->m_nX = m_ptCenter.x;
		this->m_nY = m_ptCenter.y;
	}

	void SetPosition(int Row, int Array);//设置位置
	void DrawBlank(HDC &hdc);//画空白
	void virtual Draw(HDC &hdc) = 0;//绘制对象
	void virtual action() = 0;//数据变更的表现

	int GetRow();
	int GetArray();

	static GMap *pStage;//指向地图类的指针，设置为静态，使所有子类对象都能够使用相同的地图

protected:
	int m_nX;
	int m_nY;
	TWARDS m_cmd;//指令缓存
	POINT m_ptCenter;//中心坐标
	int m_nRow;//逻辑横坐标
	int m_nArray;//逻辑纵坐标
	int m_nSpeed;//速度
	TWARDS m_dir;//朝向
	int m_nFrame;//帧数
	bool Achive();//判断物体是否到达逻辑坐标位置
	bool Collision();//碰撞检测，将物体摆放到合理的位置
	int PtTransform(int k);//将实际坐标转换为逻辑坐标
	virtual void AchiveCtrl();//到达逻辑点后更新数据
};

class PacMan :public GObject//玩家对象
{
protected:
	virtual void AchiveCtrl();
public:
	POINT GetPos();
	bool IsOver();//游戏是否结束
	bool IsWin();//玩家是否赢得游戏
	void Draw(HDC &hdc);//负责绘制自己
	void SetTwCommand(TWARDS command);//设置玩家下一步指令
	PacMan(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = PLAYERSPEED;//设置玩家速度
		m_cmd = m_dir = LEFT;//设置朝向
	}
	void action();//玩家动作函数
	void SetOver();//设置游戏结束函数
};

class Enermy :public GObject
{
protected:
	void Catch();//是否抓住玩家
	void virtual MakeDecision(bool b) = 0;//AI实现，确定方向
	COLORREF color;
public:
	static std::shared_ptr<PacMan>player;
	void virtual Draw(HDC &hdc);//绘制自己
	Enermy(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = ENERMYSPEED;//设置敌军速度
		m_dir = LEFT;//设置朝向
		m_cmd = UP;//设置移动方向
	}
	void virtual action();//负责行为
};

class RedOne :public Enermy//随机移动s
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	RedOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(255, 0, 0);
	}
};

class BlueOne :public RedOne//守卫
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	BlueOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(0, 0, 255);
	}
};

class YellowOne :public RedOne//守卫
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC &hdc);
	YellowOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(200, 200, 100);
	}
};