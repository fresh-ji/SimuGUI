#pragma once

/*
* @date : 2018/12/27
* @author : jihang
*/

#ifndef FMISIMULATOR_H
#define FMISIMULATOR_H

#include "GlobalConstant.h"

#include "imode.h"
#include "fancytabwidget.h"
#include "fancynavbar.h"

#include "CentralLabel.h"
#include "DetailStack.h"

class FMISimulator : public IMode {

	Q_OBJECT

public:
	explicit FMISimulator(QWidget *parent = 0);

private:
	void createWindow();
	//1
	void createCentralLabel();
	//2
	void createDetailStack();

	void createConnects();

public slots:
	//仅作弹窗使用
	void slotReceiveMessage(QString);
	//写日志
	void slotWriteLog(QString);

private slots:
	//日志区随动
	void slotResizable(bool resizable);

private:
	//1
	CentralLabel *m_pCentralLabel;
	//2
	DetailStack *m_pDetailStack;
	//3
	QSplitter *splitterMain;

	//4
	QStatusBar *m_logBar;
	//5
	QTextBrowser *m_logBrowser;

private:
	QString iconPath = ICON_PATH;
};

#endif // FMISIMULATOR_H