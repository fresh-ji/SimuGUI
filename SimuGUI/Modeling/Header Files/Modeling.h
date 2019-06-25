#pragma once

/*
* @date : 2019/01/04
* @author : jihang
*/

#ifndef MODELING_H
#define MODELING_H

#include "GlobalConstant.h"

#include "imode.h"
#include "fancytabwidget.h"
#include "fancynavbar.h"

#include "ToolDragStack.h"
#include "DropLabel.h"
#include "DataTypeStack.h"
#include "ModelInfoStack.h"
#include "InterfaceInfoStack.h"

class Modeling : public IMode {

	Q_OBJECT

public:
	explicit Modeling(QWidget *parent = 0);

private:
	void createWindow();
	//1
	void createToolDragStack();
	//2
	void createDropLabel();
	//3
	void createDateTypeStack();
	//4
	void createModelInfoStack();
	//5
	void createInterfaceInfoStack();

	void createConnects();

public slots:
	//��������ʹ��
	void slotReceiveMessage(QString);

private slots:
	//��־���涯
	void slotResizable(bool);

private:
	//1
	ToolDragStack *m_pToolDragStack;
	//2
	DropLabel *m_pDropLabel;
	//3
	DataTypeStack *m_pDataTypeStack;
	//4
	ModelInfoStack *m_pModelInfoStack;
	//5
	InterfaceInfoStack *m_pInterfaceInfoStack;
	//6
	QStatusBar *m_logBar;
	//7
	QTextBrowser *m_logBrowser;

	//8
	QSplitter *splitterMain;
	QSplitter *splitterTop;
	QSplitter *splitterDown;

private:
	QString iconPath = ICON_PATH;
};
#endif // MODELING_H