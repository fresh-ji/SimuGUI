#pragma once

/*
* @date : 2019/05/05
* @author : jihang
*/

#ifndef ITEMELEMENT_H
#define ITEMELEMENT_H

#include "cwidgets.h"

//设计
constexpr auto TOTALWIDTH = 200;
constexpr auto TEXTHEIGHT = 20;
constexpr auto WIDTH = 80;
constexpr auto EDGE = 10;
constexpr auto GAP = 15;
constexpr auto SMALLANCHOR = 8;
constexpr auto BIGANCHOR = 20;

constexpr auto FRAMESHIFTX = (TOTALWIDTH - WIDTH) / 2;
constexpr auto FRAMESHIFTY = TEXTHEIGHT + EDGE;

//模型类型
constexpr auto CTYPE = "C";
constexpr auto MATLABTYPE = "Matlab";
constexpr auto ADAMSTYPE = "Adams";

//拖拽类型
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
	//模型名，初始化name = type + "Model_" + count
	QString name;
	//模型类型
	QString type;
	//模型编号
	int count;

	//锚点
	Anchor activeAnchor;

private:
	//名称标签
	QLabel *topNameLabel;
	QLabel *bottomNameLabel;
	//显示框
	QLabel *frameLabel;
	//连接点
	QLabel *topAnchor;
	QLabel *bottomAnchor;
	QLabel *leftAnchor;
	QLabel *rightAnchor;
	//图片
	QLabel *imageLabel;

public:
	void inactive();
	void active();
	//根据点击点返回锚点
	Anchor getAvaiableAnchor(QPoint offset);

public:
	QString getName();
	void setName(QString name);
	QString getType();
	int getCount();
	Anchor getActiveAnchor();
	void setActiveAnchor(Anchor activeAnchor);
};

#endif // ITEMELEMENT_H