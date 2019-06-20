#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef DETAILSTACK_H
#define DETAILSTACK_H

#include <QMap>

#include "cwidgets.h"
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

private slots :
	void slotGo();
signals:
	void signalRunSimulation(double, double, double);
signals:
	void signalSendMessage(QString);

private:
	//������Ϣ
	QWidget *infoWidget;
	QGridLayout *infoLayout;
	//������Ϣ
	QTableWidget *variableTable;
	//������Ϣ
	QLineEdit *startTime;
	QLineEdit *stopTime;
	QLineEdit *stepSize;

private:
	FMUInfo *info;
	void refreshInfo();
	void refreshVariable();
};

#endif // DETAILSTACK_H