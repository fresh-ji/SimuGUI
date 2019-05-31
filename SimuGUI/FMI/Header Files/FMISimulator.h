#pragma once

/*
* @date : 2018/12/27
* @author : jihang
*/

#ifndef FMISIMULATOR_H
#define FMISIMULATOR_H

#include "imode.h"
#include "fancytabwidget.h"
#include "fancynavbar.h"

#include "cwidgets.h"

class FMISimulator : public IMode {

	Q_OBJECT

public:
	explicit FMISimulator(QWidget *parent = 0);

private:
	void createWindow();
	//1
	void createCentralPanel();
	//2
	void createDetailStack();
	//3
	void createPlotStack();

	void createConnects();

public slots:
	void test();
	void DisplayFMIMsg(QString);
	//日志区随动
	void slotResizable(bool resizable);

private:
	//QTextBrowser *textBrowser;

	//
	QStatusBar *m_logBar;
	//
	QTextBrowser *m_logBrowser;
};
#endif // FMISIMULATOR_H