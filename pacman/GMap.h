#include"stdafx.h"
#pragma once

#include<list>

#define MAPLENTH 19//��ͼ��С
#define P_ROW 10//�ҷ�λ������
#define P_ARRAY 9
#define E_ROW 8//�з�λ������
#define E_ARRAY 9

using std::list;

//������GMap
class GMap
{
protected:
	static int LD;//�ϰ���ߴ�
	static int PD;//���ӵİ뾶
	void InitOP();//����˫�����ֵ�λ��û�ж��ӵĳ���
	bool mapData[MAPLENTH][MAPLENTH];//�ϰ����߼���ͼ����
	bool peaMapData[MAPLENTH][MAPLENTH];//�����߼���ͼ����
	COLORREF color;//��ͼ��ǽ����ɫ
public:
	void DrawMap(HDC &hdc);//���Ƶ�ͼ
	void DrawPeas(HDC &hdc);//���ƶ���
	virtual ~GMap();
	GMap() 
	{
		if (GetAsyncKeyState('B') & 0 * 8000)
		{
			MessageBoxA(NULL, "�������з������ؼ�", "", MB_OK);
			for (int i = 0; i < MAPLENTH; i++)
			{
				for (int j = 0; j < MAPLENTH; j++)
				{
					peaMapData[i][j] = false;
				}
			}
		}
	}
	friend class GObject;//����������ʹ��ֱ�ߵ������յ���Ϣ�������ײ
	friend class PacMan;//���������ʶ��ӵ�ͼ
};

//��һ��
class Stage_1 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];//��ͼ����
public:
	Stage_1();
};

//�ڶ���
class Stage_2 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];//��ͼ����
public:
	Stage_2();
};

//������
class Stage_3 :public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];//��ͼ����
public:
	Stage_3();
};