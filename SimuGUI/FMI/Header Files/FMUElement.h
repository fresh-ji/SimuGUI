#pragma once
#pragma once

/*
* @date : 2019/06/24
* @author : jihang
*/

#ifndef FMUELEMENT_H
#define FMUELEMENT_H

#include "GlobalConstant.h"

#include "cwidgets.h"

//���
constexpr auto FMUTOTALWIDTH = 200;
constexpr auto FMUTEXTHEIGHT = 20;
constexpr auto FMUWIDTH = 80;
constexpr auto FMUEDGE = 10;
constexpr auto FMUGAP = 15;
constexpr auto FMUSMALLANCHOR = 12;

constexpr auto FMUFRAMESHIFTX = (FMUTOTALWIDTH - FMUWIDTH) / 2;
constexpr auto FMUFRAMESHIFTY = FMUTEXTHEIGHT + FMUEDGE;

constexpr auto FMUINPUTANCHORSHIFTX = (FMUTOTALWIDTH - FMUWIDTH) / 2;
constexpr auto FMUINPUTANCHORSHIFTY = FMUTEXTHEIGHT + FMUEDGE * 2 + FMUWIDTH / 2;
constexpr auto FMUOUTPUTANCHORSHIFTX = (FMUTOTALWIDTH + FMUWIDTH) / 2;
constexpr auto FMUOUTPUTANCHORSHIFTY = FMUTEXTHEIGHT + FMUEDGE * 2 + FMUWIDTH / 2;

//��ק����
constexpr auto FMU_DRAG_MOVE = "FMU_Drag_Move";
constexpr auto FMU_DRAG_CONNECT = "FMU_Drag_Connect";

class FMUElement : public QLabel {

	Q_OBJECT

public:
	explicit FMUElement(QString, int, int, QWidget *p = 0);

	enum Anchor { NONE, INPUT, OUTPUT };

private:
	//ģ����
	QString name;

private:
	//���Ʊ�ǩ
	QLabel *topNameLabel;
	QLabel *bottomNameLabel;
	//��ʾ��
	QLabel *frameLabel;
	//���������
	QLabel *inputAnchor;
	QLabel *outputAnchor;

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
};

#endif // FMUELEMENT_H