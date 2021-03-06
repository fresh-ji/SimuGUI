#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef DETAILSTACK_H
#define DETAILSTACK_H

#include "GlobalConstant.h"

#include "ministack.h"
#include "fancybutton.h"

#include "FMISupport.h"

class DetailStack :public MiniStack {

	Q_OBJECT

public:
	explicit DetailStack(QWidget *p = 0);
	~DetailStack() {}

public slots:
	void slotModelDetail(FMUInfo*);

private slots:
	void slotGo();
	void slotPlot();
private slots:
	void sort(int);
signals:
	void signalRunSimulation(double, double, double);
signals:
	void signalPlot(QString);
signals:
	//向ui发送消息
	void signalSendMessage(QString);
signals:
	//向ui写日志
	void signalWriteLog(QString);

private:
	//基本信息
	QWidget *infoWidget;
	QGridLayout *infoLayout;
	//变量信息
	QTableWidget *variableTable;
	//运行信息
	QLineEdit *startTime;
	QLineEdit *stopTime;
	QLineEdit *stepSize;

private:
	FMUInfo* activeInfo;
	void refreshInfo();
	void refreshVariable();

private:
	QString iconPath = ICON_PATH;
};

#endif // DETAILSTACK_H