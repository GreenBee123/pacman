#include "stdafx.h"
#include "GObject.h"

//GObject��Ա����
GMap *GObject::pStage = NULL;

int GObject::GetRow()//������
{
	return m_nRow;
}

int GObject::GetArray()//���������׵�ַ
{
	return m_nArray;
}

int GObject::PtTransform(int k)//����ת������
{
	return (k - (pStage->LD / 2) / pStage->LD);
}

//�ж������Ƿ񵽴��߼�����λ��
bool GObject::Achive()
{
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;//����x���������
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD;//����y���������
	bool l = (n == 0 && k == 0);
	return l;
}

//�����߼�����������
void GObject::AchiveCtrl()
{

}