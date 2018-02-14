#include "stdafx.h"
#include "GObject.h"

//GObject成员定义
GMap *GObject::pStage = NULL;

int GObject::GetRow()//返回行
{
	return m_nRow;
}

int GObject::GetArray()//返回数组首地址
{
	return m_nArray;
}

int GObject::PtTransform(int k)//坐标转换函数
{
	return (k - (pStage->LD) / 2) / pStage->LD;
}

//判断物体是否到达逻辑坐标位置
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//计算x坐标的余数
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD;//计算y坐标的余数
	bool l = (n == 0 && k == 0);
	return l;
}

//到达逻辑点后更新数据
void GObject::AchiveCtrl()
{
	if (Achive())//如果达到了逻辑坐标
	{
		m_nArray = PtTransform(m_ptCenter.x);//更新列
		m_nRow = PtTransform(m_ptCenter.y);//更新行
	}
}

void GObject::DrawBlank(HDC &hdc)
{
	//申请资源，并交给智能指针处理
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255)); // 创建画刷，绘制矩型函数要求使用
	std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {	  // 把资源交给智能指针处理，自动释放
		DeleteObject(hbr); });	// 离开 DrawBlank函数时,会自动调用释放资源

	RECT rect;
	rect.top = m_nY - RD;
	rect.left = m_nX - RD;
	rect.right = m_nX + RD;
	rect.bottom = m_nY + RD;
	FillRect(hdc, &rect, *phbr);//填充矩形
}

//设中心位置
void GObject::SetPosition(int Row, int Array)
{
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}

//碰撞检测
bool GObject::Collision()
{
	bool b = false;

	//更新行和列的数据，如果是大嘴，则会执行PacMan重写的AchiveCtrl()函数消除豆子
	AchiveCtrl();
	//判断指令的有效性
	if (m_nArray<0 || m_nRow<0 || m_nArray>MAPLENTH - 1 || m_nRow>MAPLENTH - 1)
	{
		b = true;
	}
	else if (Achive())
	{
		switch (m_cmd)//判断进行的方向
		{
		case LEFT://如果朝向为左
		{
			//判断下一个格子是否能够同行
			if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1])
			{
				b = true;//撞墙了
			}
			break;
		}
		case RIGHT://如果朝向为右
		{
			//判断下一个格子是否能够同行
			if (m_nArray < MAPLENTH - 1 && !pStage->mapData[m_nRow][m_nArray + 1])
			{
				b = true;//撞墙了

			}
				break;
		}
		case UP://如果朝向为上
		{
			//判断下一个格子是否能够同行
			if (m_nRow > 0 && !pStage->mapData[m_nRow - 1][m_nArray])
			{
				b = true;//撞墙了
			}
			break;
		}
		case DOWN://如果朝向为下
		{
			//判断下一个格子是否能够同行
			if (m_nRow < MAPLENTH - 1 && !pStage->mapData[m_nRow + 1][m_nArray])
			{
				b = true;//撞墙了
			}
			break;
		}
		if (!b)
		{
			m_dir = m_cmd;//没有撞墙，指令是对的
		}
		}
	}
	//依照真实方向进行位移
	m_nX = m_ptCenter.x;
	m_nY = m_ptCenter.y;
	int MAX = pStage->LD*MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir)
	{
		//判断下一个格子是否能够通行
	case LEFT:
	{
		if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1])
		{
			b = true;
			break;//撞墙了
		}
		m_ptCenter.x -= m_nSpeed;
		if (m_ptCenter.x < MIN)
		{
			m_ptCenter.x = MAX;
		}
		break;
	}
	case RIGHT:
	{
		if (m_nArray < MAPLENTH - 1 && !pStage->mapData[m_nRow][m_nArray + 1])
		{
			b = true;//撞墙了
			break;
		}
		m_ptCenter.x += m_nSpeed;
		if (m_ptCenter.x > MAX)
		{
			m_ptCenter.x = MIN;
		}
		break;
	}

	case UP://如果朝向为上
	{
		//判断下一个格子是否能够同行
		if (m_nRow > 0 && !pStage->mapData[m_nRow - 1][m_nArray])
		{
			b = true;//撞墙了
			break;
		}
		m_ptCenter.y -= m_nSpeed;
		if (m_ptCenter.y < MIN)
		{
			m_ptCenter.y = MAX;
		}
		break;
	}

	case DOWN://如果朝向为下
	{
		//判断下一个格子是否能够同行
		if (m_nRow < MAPLENTH - 1 && !pStage->mapData[m_nRow + 1][m_nArray])
		{
			b = true;//撞墙了
			break;
		}
		m_ptCenter.y += m_nSpeed;
		if (m_ptCenter.y > MAX)
		{
			m_ptCenter.y = MIN;
		}
		break;
	}
	}
	return b;
}

//PacMan的成员定义：
void PacMan::AchiveCtrl()
{
	GObject::AchiveCtrl();
	if (Achive())
	{
		if (m_nRow >= 0 && m_nRow < MAPLENTH&&m_nArray >= 0 && m_nArray < MAPLENTH)
		{
			if (pStage->peaMapData[m_nRow][m_nArray])//防止数组越界 
			{
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}

void PacMan::action()
{
	Collision();//碰撞检测
}

void PacMan::SetTwCommand(TWARDS command)
{
	m_cmd = command;//设置移动方向
}

bool PacMan::IsOver()
{
	return m_dir == OVER;
}

bool PacMan::IsWin()
{
	for (int i = 0; i <= MAPLENTH; i++)
	{
		for (int j = 0; j <= MAPLENTH; j++)
		{
			if (pStage->peaMapData[i][j] == true)
			{
				return false;//如果存在一个豆子也就是没有胜利
			}
		}
	}
	return true;//没有豆子 胜利
}

POINT PacMan::GetPos()
{
	return m_ptCenter;//返回对象中心位置
}

void PacMan::SetOver()
{
	m_dir = OVER;
}

void PacMan::Draw(HDC &memDC)
{
	if (m_dir == OVER)
	{

	}//游戏结束，我啥也不画
	else if (m_nFrame % 2 == 0)//第四帧和第二帧动画：张嘴形状
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + D_OFFSET;//弧弦焦点
		int offsetY = DISTANCE / 2 + D_OFFSET;//弧弦焦点
		switch (m_dir)
		{
		case UP:
		{
			x1 = m_ptCenter.x - offsetX;
			x2 = m_ptCenter.x + offsetX;
			y2 = y1 = m_ptCenter.y - offsetY;
			break;
		}
		case DOWN:
		{
			x1 = m_ptCenter.x + offsetX;
			x2 = m_ptCenter.x - offsetX;
			y2 = y1 = m_ptCenter.y + offsetY;
			break;
		}
		case LEFT:
		{
			x2 = x1 = m_ptCenter.x - offsetX;
			y1 = m_ptCenter.y + offsetY;
			y2 = m_ptCenter.y - offsetY;
			break;
		}
		case RIGHT:
		{
			x2 = x1 = m_ptCenter.x + offsetX;
			y1 = m_ptCenter.y - offsetY;
			y2 = m_ptCenter.y + offsetY;
			break;
		}
		}
		//画出弧弦
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1,
			x2, y2);
		//画直线
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}

	else if (m_nFrame % 3 == 0)//第三帧，也就是整个圆形大嘴
	{
		Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);
	}

	else//嘴完全张开的形状
	{
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		switch (m_dir)
		{
		case UP:
		{
			x1 = m_ptCenter.x - DISTANCE;
			x2 = m_ptCenter.x + DISTANCE;
			y2 = y1 = m_ptCenter.y;
			break;
		}
		case DOWN:
		{
			x1 = m_ptCenter.x + DISTANCE;
			x2 = m_ptCenter.x - DISTANCE;
			y2 = y1 = m_ptCenter.y;
			break;
		}
		case LEFT:
		{
			x2 = x1 = m_ptCenter.x;
			y1 = m_ptCenter.y + DISTANCE;
			y2 = m_ptCenter.y - DISTANCE;
			break;
		}
		case RIGHT:
		{
			x2 = x1 = m_ptCenter.x;
			y1 = m_ptCenter.y - DISTANCE;
			y2 = m_ptCenter.y + DISTANCE;
			break;
		}
		}
		//画出弧弦
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
			m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
			x1, y1,
			x2, y2);
		//画直线
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	m_nFrame++;//画下一帧
}

//Enermy成员定义
std::shared_ptr<PacMan>Enermy::player = nullptr;

//抓住！游戏结束
void Enermy::Catch()
{
	int DX = m_ptCenter.x - player->GetPos().x;
	int DY = m_ptCenter.y - player->GetPos().y;
	if ((-RD < DX&&DX < RD) && (-RD < DY&&DY < RD))
	{
		player->SetOver();
	}
}

//画自己
void Enermy::Draw(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldpen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y,
		m_ptCenter.x - DISTANCE, m_ptCenter.y);//画一个半圆形
	int const LEGLENTH = (DISTANCE) / (LEGCOUNTS);
	//根据帧数来绘制身体和腿部
	if (m_nFrame % 2 == 0)
	{
		//矩形的身子
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);

		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);

		//开始画jio
		for (int i = 0; i < LEGCOUNTS; i++)
		{
			Arc(hdc,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH
			);
		}
	}

	else
	{
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE);

		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);

		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE + LEGLENTH, m_ptCenter.y + DISTANCE - LEGLENTH);

		//开始画jio
		for (int i = 0; i < LEGCOUNTS; i++)
		{
			Arc(hdc,
				m_ptCenter.x - DISTANCE + (i + 1 * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 3 * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 3 * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH
			);
		}

		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE - LEGLENTH, m_ptCenter.y + DISTANCE - LEGLENTH);

		//根据方向画眼睛
		int R = DISTANCE / 5;
		switch (m_dir)
		{
		case UP:
		{
			Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y - 2 * R,
				m_ptCenter.x, m_ptCenter.y);//画左眼
			Ellipse(hdc, m_ptCenter.x, m_ptCenter.y - 2 * R,
				m_ptCenter.x + 2 * R, m_ptCenter.y);//画右眼
			break;
		}
		case DOWN:
		{
			Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y,
				m_ptCenter.x, m_ptCenter.y + 2 * R);//画左眼
			Ellipse(hdc, m_ptCenter.x, m_ptCenter.y,
				m_ptCenter.x + 2 * R, m_ptCenter.y + 2 * R);//画右眼
			break;
		}
		case LEFT:
		{
			Ellipse(hdc, m_ptCenter.x - 3 * R, m_ptCenter.y - R,
				m_ptCenter.x, m_ptCenter.y);//画左眼
			Ellipse(hdc, m_ptCenter.x - R, m_ptCenter.y - R,
				m_ptCenter.x + R, m_ptCenter.y + R);//画右眼
			break;
		}
		case RIGHT:
		{
			Ellipse(hdc, m_ptCenter.x - R, m_ptCenter.y - R,
				m_ptCenter.x, m_ptCenter.y);//画左眼
			Ellipse(hdc, m_ptCenter.x + R, m_ptCenter.y - R,
				m_ptCenter.x + 3 * R, m_ptCenter.y + R);//画右眼
			break;
		}
		}
	}

	m_nFrame++;//下一帧
	SelectObject(hdc, oldpen);//还原画笔
	DeleteObject(pen);//还原画笔对象
	return;
}

void Enermy::action()
{
	bool b = Collision();//是否发生发生碰撞
	MakeDecision(b);//设定方向
	Catch();//开始抓捕
}

//RedOne成员
void RedOne::Draw(HDC&hdc)
{
	Enermy::Draw(hdc);
}

void RedOne::MakeDecision(bool b)
{
	//srand(time(0));
	int i = rand();
	if (b) {  													// 撞到墙壁,改变方向
		if (i % 4 == 0) { 											// 逆时针转向
			m_dir == UP ? m_cmd = LEFT : m_cmd = UP;				// 面向上，向左拐
		}
		else if (i % 3 == 0) {
			m_dir == DOWN ? m_cmd = RIGHT : m_cmd = DOWN;			// 面向下，向右拐
		}
		else if (i % 2 == 0) {
			m_dir == RIGHT ? m_cmd = UP : m_cmd = RIGHT;			// 面向右，向上拐
		}
		else {
			m_dir == LEFT ? m_cmd = DOWN : m_cmd = LEFT;			// 面向左，向下拐
		}
		return;													// 提前结束函数，返回
	}

	// 程序运行到这里，说明没有撞墙，继续处理
	if (i % 4 == 0) {
		m_cmd != UP ? m_dir == DOWN : m_cmd == UP;		// 非向上移动则使之面向下，否则面向上
	}
	else if (i % 3 == 0) {
		m_dir != DOWN ? m_cmd = UP : m_cmd = DOWN;		// 非向下移动则使之面向上，否则面向下
	}
	else if (i % 2 == 0) {
		m_dir != RIGHT ? m_cmd = LEFT : m_cmd = RIGHT;	// 非向右移动则使之面向左，否则面向右
	}
	else {
		m_dir != LEFT ? m_cmd = RIGHT : m_cmd = LEFT;	// 非向左移动则使之面向右，否则面向左
	}
}

//BlueOne成员定义

void BlueOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);
}

void BlueOne::MakeDecision(bool b)
{

	const int DR = this->m_nRow - player->GetRow();
	const int DA = this->m_nArray - player->GetArray();
	if (!b && DR == 0) {
		if (DA <= BLUE_ALERT && DA > 0) {  // 玩家在左侧边警戒范围s
			m_cmd = LEFT;                   // 向左移动
			return;
		}
		if (DA < 0 && DA >= -BLUE_ALERT) {  // 右侧警戒范围
			m_cmd = RIGHT;                   // 向右移动
			return;
		}
	}
	if (!b && DA == 0) {
		if (DR <= BLUE_ALERT && DR > 0) {   // 下方警戒范围
			m_cmd = UP;                      // 向上移动
			return;
		}
		if (DR < 0 && DR >= -BLUE_ALERT) {  // 上方警戒范围
			m_cmd = DOWN;                    // 向下移动
			return;
		}
	}
	RedOne::MakeDecision(b);  //不在追踪模式时RED行为相同
}

//YellowOne成员定义
void YellowOne::MakeDecision(bool b)
{
	const int DR = this->m_nRow - player->GetRow();
	const int DA = this->m_nArray - player->GetArray();
	if (!b) {
		if (DR * DR > DA * DA) {
			if (DA > 0) {  		// 玩家在左侧边警戒范围
				m_cmd = LEFT;  	// 向左移动
				return;
			}
			else if (DA < 0) {  	// 右侧警戒范围
				m_cmd = RIGHT;	// 向右移动
				return;
			}
		}
		else {
			if (DR > 0) {  		// 下方警戒范围
				m_cmd = UP;		// 向上移动
				return;
			}
			if (DR < 0) {  		// 上方警戒范围
				m_cmd = DOWN;		// 向下移动
				return;
			}
		}
	}
	RedOne::MakeDecision(b);		// 调用红色对象的函数，实现随机移动功能
}
void YellowOne::Draw(HDC &hdc)
{
	Enermy::Draw(hdc);			// 绘制自身
}
