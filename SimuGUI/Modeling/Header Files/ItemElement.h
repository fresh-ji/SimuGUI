#pragma once

/*
* @date : 2019/05/05
* @author : jihang
*/

#ifndef ITEMELEMENT_H
#define ITEMELEMENT_H

#include "cwidgets.h"

//���
constexpr auto TOTALWIDTH = 200;
constexpr auto TEXTHEIGHT = 20;
constexpr auto WIDTH = 80;
constexpr auto EDGE = 10;
constexpr auto GAP = 15;
constexpr auto SMALLANCHOR = 8;
constexpr auto BIGANCHOR = 20;

constexpr auto FRAMESHIFTX = (TOTALWIDTH - WIDTH) / 2;
constexpr auto FRAMESHIFTY = TEXTHEIGHT + EDGE;

constexpr auto TOPANCHORSHIFTX = TOTALWIDTH / 2;
constexpr auto TOPANCHORSHIFTY = TEXTHEIGHT + EDGE;
constexpr auto BOTTOMANCHORSHIFTX = TOTALWIDTH / 2;
constexpr auto BOTTOMANCHORSHIFTY = TEXTHEIGHT + EDGE * 3 + WIDTH;
constexpr auto LEFTANCHORSHIFTX = (TOTALWIDTH - WIDTH) / 2;
constexpr auto LEFTANCHORSHIFTY = TEXTHEIGHT + EDGE * 2 + WIDTH / 2;
constexpr auto RIGHTANCHORSHIFTX = (TOTALWIDTH + WIDTH) / 2;
constexpr auto RIGHTANCHORSHIFTY = TEXTHEIGHT + EDGE * 2 + WIDTH / 2;

//ģ������
constexpr auto CTYPE = "C";
constexpr auto MATLABTYPE = "Matlab";
constexpr auto ADAMSTYPE = "Adams";

//��ק����
constexpr auto DRAG_MOVE = "Drag_Move";
constexpr auto DRAG_COPY = "Drag_Copy";
constexpr auto DRAG_STRETCH = "Drag_Stretch";
constexpr auto DRAG_CONNECT = "Drag_Connect";

class ItemElement : public QLabel {

	Q_OBJECT

public:
	explicit ItemElement(QPixmap*, QString, QString, int, QWidget *p = 0);

	enum Anchor { NONE, TOP, BOTTOM, LEFT, RIGHT };

private:
	//ģ��������ʼ��name = type + "Model_" + count
	QString name;
	//ģ������
	QString type;
	//ģ�ͱ��
	int count;

private:
	//���Ʊ�ǩ
	QLabel *topNameLabel;
	QLabel *bottomNameLabel;
	//��ʾ��
	QLabel *frameLabel;
	//���ӵ�
	QLabel *topAnchor;
	QLabel *bottomAnchor;
	QLabel *leftAnchor;
	QLabel *rightAnchor;
	//ͼƬ
	QLabel *imageLabel;

public:
	//ȡ��ѡ��
	void inactive();
	//ѡ��
	void active();
	//���ݵ���㷵��ê��
	Anchor whichAnchor(QPoint offset);
	//��ȡê������Ӿ�׼��
	QPoint getAnchorShift(Anchor);

public:
	QString getName();
	void setName(QString name);
	QString getType();
	int getCount();
};

#endif // ITEMELEMENT_H