#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef CENTRALLABEL_H
#define CENTRALLABEL_H

#include "GlobalConstant.h"

#include "fancybutton.h"

#include "FMUElement.h"
#include "FMISupport.h"
#include "PlotDialog.h"

class CentralLabel : public QLabel {

	Q_OBJECT

public:
	explicit CentralLabel(QWidget *p = 0);

	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);
	virtual void dropEvent(QDropEvent*);

private:
	//点击鼠标左键后的一些预处理，防止函数过长
	int mousePressPreprocess(QPoint);

private:
	//添加功能栏按钮
	void addToolBar();

private slots:
	void slotOpen();
	void slotUndo();
	void slotRedo();
	void slotGrid();
	void slotDelete();
signals:
	//向ui发送消息
	void signalSendMessage(QString);
signals:
	//向ui写日志
	void signalWriteLog(QString);
signals:
	//向DetailStack发送要显示的
	void signalModelDetail(FMUInfo*);
public slots:
	void slotRunSimulation(double, double, double);
public slots:
	void slotPlot(QString);

private:
	//模型库
	QMap<QString, FMUInfo> modelRepo;

	//模型列表
	QMap<QString, FMUElement*> modelMap;
	//被选中模型
	FMUElement *activeModel;

	//拖动时的相对起始坐标
	QPoint moveStartPoint;

	//选择文件时的初始路径
	QString openPath;

	//整fmu的
	FMISupport *FMIsupport;

	//作画框
	PlotDialog *dialog;

private:
	QString iconPath = ICON_PATH;
};

#endif // CENTRALLABEL_H