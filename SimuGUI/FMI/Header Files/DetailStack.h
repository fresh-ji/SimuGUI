#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef DETAILSTACK_H
#define DETAILSTACK_H

#include <QMap>

#include "GlobalConstant.h"
#include "Regulation.h"

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
	//��ui������Ϣ
	void signalSendMessage(QString);
signals:
	//��uiд��־
	void signalWriteLog(QString);

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
	FMUInfo* activeInfo;
	void refreshInfo();
	void refreshVariable();
};

#endif // DETAILSTACK_H