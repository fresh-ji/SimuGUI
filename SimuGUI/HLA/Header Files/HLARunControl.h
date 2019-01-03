#pragma once

/*
* �������GUI
* @date : 2018/12/26
* @author : jihang
*/

#ifndef HLARUNCONTROL_H
#define HLARUNCONTROL_H

#include "imode.h"

#include "cwidgets.h"

#include "tinytabwidget.h"
#include "ministack.h"
#include "stateprogressbar.h"
#include "fancybutton.h"

#include "HLAFlowThread.h"
#include "StateMachineCode.h"

class	HLARunControl : public IMode {

	Q_OBJECT

public:
	explicit HLARunControl(QWidget *parent = 0);

signals:

public slots:
	//�����̴߳��ص��ź���Ϣ
	void DisplayFlow(QString);
	//�����̴߳��ص�״̬��Ϣ
	void DisplayState(int);
	//����HLA״̬��Ϣ
	void DisplayHLAState(QString);
	//����HLAʱ����Ϣ
	void DisplayHLATime(double);

private slots:
	void prepare();
	void ready();
	void start();
	void pause();
	void end();

private:
	void createWindow();
	void putImage(QString, QLabel*);
	FancyButton* addFunctionButton(FancyButton*, QString);

private:
	//�̶߳���
	HLAFlowThread *flowThread;
	//�ı���Ӷ���
	QTextBrowser *info;
	//״̬ͼ����
	QLabel *stateLabel;
	//��ʱ����
	QLCDNumber *lcdNumber;
};

#endif // HLARUNCONTROL_H