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

}