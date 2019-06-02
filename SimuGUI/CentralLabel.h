#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef CENTRALLABEL_H
#define CENTRALLABEL_H

#include "cwidgets.h"
#include "fancybutton.h"
#include "ItemElement.h"

class CentralLabel : public QLabel {

	Q_OBJECT

public:
	explicit CentralLabel(QWidget *p = 0);

	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);
	virtual void dropEvent(QDropEvent*);

private:
	//添加功能栏按钮
	void addToolBar();

private slots:
	void slotOpen();
	void slotUndo();
	void slotRedo();
	void slotGrid();
	void slotCut();
	void slotGo();

private:
	//模型列表
	QList<ItemElement*> modelList;
	//被选中模型
	ItemElement *activeModel;

	//拖动时的相对起始坐标
	QPoint moveStartPoint;
};

#endif // CENTRALLABEL_H