#pragma once

/*
* @date : 2019/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet不好用
#include <set>

#include "GlobalConstant.h"

#include "fancybutton.h"

#include "BusElement.h"
#include "ItemElement.h"
#include "OverviewDialog.h"
#include "Regulation.h"

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

	//repaint all connections
	virtual void paintEvent(QPaintEvent*);

private:
	//点击鼠标左键后的一些预处理，防止函数过长
	int mousePressPreprocess(QPoint);
	//拖动过程方法，防止函数过长
	void mouseDragMove(QPoint);
	//bus拉伸过程方法，防止函数过长
	void mouseDragStretch(QPoint);
	//添加功能栏按钮
	void addToolBar();

signals:
	//拖动增加模型并发送
	void signalAddModel(QString, QString);
public slots:
	//接收删除模型
	void slotDeleteModel(QString);
signals:
	//模型选择改变并发送
	void signalModelChange(QString);
public slots:
	//接收模型选择改变
	void slotModelChange(QString);
signals:
	QMap<QString, dataTypeInfo> signalDataTypeOverview();
signals:
	QMap<QString, interfaceInfo> signalInterfaceOverview();
signals:
	//向ui发送消息
	void signalSendMessage(QString);

private slots:
	void slotUndo();
	void slotRedo();
	void slotGrid();
	void slotCut();
	void slotGenerate();

private:
	//私有命名引擎
	QMap<QString, std::set<int>> namingMap;

	//连接管理
	QMap<QString, connectInfo> connectMap;

	//模型列表
	QList<ItemElement*> modelList;
	//被选中模型
	ItemElement *activeModel;
	//bus
	BusElement *bus;

	//拖动时的相对起始坐标
	QPoint moveStartPoint;

	//概览对话框
	OverviewDialog *dialog;

private:
	QString iconPath = ICON_PATH;
};

#endif // DROPLABEL_H