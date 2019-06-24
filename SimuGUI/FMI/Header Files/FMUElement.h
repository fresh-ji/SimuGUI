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

//设计
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

//拖拽类型
constexpr auto FMU_DRAG_MOVE = "FMU_Drag_Move";
constexpr auto FMU_DRAG_CONNECT = "FMU_Drag_Connect";

class FMUElement : public QLabel {

	Q_OBJECT

public:
	explicit FMUElement(QString, int, int, QWidget *p = 0);

	enum Anchor { NONE, INPUT, OUTPUT };

private:
	//模型名
	QString name;

private:
	//名称标签
	QLabel *topNameLabel;
	QLabel *bottomNameLabel;
	//显示框
	QLabel *frameLabel;
	//输入输出点
	QLabel *inputAnchor;
	QLabel *outputAnchor;

public:
	//取消选中
	void inactive();
	//选中
	void active();
	//根据点击点返回锚点
	Anchor whichAnchor(QPoint offset);
	//获取锚点的连接精准点
	QPoint getAnchorShift(Anchor);

public:
	QString getName();
	void setName(QString name);
};

#endif // FMUELEMENT_H