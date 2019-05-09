#pragma once

/*
* @date : 2019/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet不好用
#include <set>

#include "cwidgets.h"
#include "fancybutton.h"
#include "BusElement.h"
#include "ItemElement.h"

class DropLabel : public QLabel {

	Q_OBJECT

public:
	explicit DropLabel(QWidget *p = 0);

	//copy
	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void dropEvent(QDropEvent*);

	//move, stretch, connect
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);

	//
	virtual void paintEvent(QPaintEvent*);

signals:
	//拖动增加模型，前端接收
	void signalAddModel(QString, QString);
public slots:
	//接收前端删除模型
	void slotDeleteModel(QString);
signals:
	//模型选择改变，前端接收
	void signalModelChange(QString);
public slots:
	//接收前端模型选择改变
	void slotModelChange(QString);
signals:
	//像前端发送消息
	void signalSendMessage(QString);

private:
	//私有命名引擎
	QMap<QString, std::set<int>> namingMap;

	//模型列表
	QList<ItemElement*> modelList;
	//item
	ItemElement *activeModel;

	//bus
	BusElement *bus;

	//拖动时的相对坐标
	int hOffset, vOffset;

	QPoint startPoint;
	QPoint endPoint;
};

#endif // DROPLABEL_H