#pragma once

/*
* @date : 2019/01/07
* @author : jihang
*/

#ifndef TOOLDRAGSTACK_H
#define TOOLDRAGSTACK_H

#include "GlobalConstant.h"

#include "ministack.h"
#include "fancybutton.h"

class ToolDragStack :public MiniStack {

	Q_OBJECT

public:

	explicit ToolDragStack(QWidget *p = 0);

	~ToolDragStack() {}

	virtual void mousePressEvent(QMouseEvent *e);

private:
	QLabel* m_cLabel;
	QLabel* m_matlabLabel;
	QLabel* m_adamsLabel;
	QLabel* m_simulinkLabel;

	QLabel* m_dragLabel;

private:
	void putImage(QString, QLabel *);

private:
	QString iconPath = ICON_PATH;
};

#endif // TOOLDRAGSTACK_H