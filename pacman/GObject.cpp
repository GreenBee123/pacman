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
	return (k - (pStage->LD / 2) / pStage->LD);
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

				break;
			}
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
		}
		break;
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
		}
		break;
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
			y2 = y1 = m_ptCenter.y - offsetY;
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
			x2 = x1 = m_ptCenter.x - offsetX;
			y1 = m_ptCenter.y - offsetY;
			y2 = m_ptCenter.y + offsetY;
			break;
		}
		default:
			break;
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
			m_ptCenter.x + DISTANCE, m_ptCenter.y - DISTANCE);
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
			y2 = y1 = m_ptCenter.y ;
			break;
		}
		case DOWN:
		{
			x1 = m_ptCenter.x + DISTANCE;
			x2 = m_ptCenter.x - DISTANCE;
			y2 = y1 = m_ptCenter.y ;
			break;
		}
		case LEFT:
		{
			x2 = x1 = m_ptCenter.x ;
			y1 = m_ptCenter.y + DISTANCE;
			y2 = m_ptCenter.y - DISTANCE;
			break;
		}
		case RIGHT:
		{
			x2 = x1 = m_ptCenter.x ;
			y1 = m_ptCenter.y - DISTANCE;
			y2 = m_ptCenter.y + DISTANCE;
			break;
		}
		default:
			break;
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