#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

//QSet不好用
#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditOutputDialog.h"

//constexpr auto outputType = "output";
//constexpr auto inputType = "input";

class InterfaceInfoStack : public MiniStack {

	Q_OBJECT

public:
	explicit InterfaceInfoStack(QWidget *p = 0);
	~InterfaceInfoStack() {};

private slots:
	//添加输出，订阅输入，编辑，删除
	void pSlotAddOutput();
	void pSlotEditOutput();
	void pSlotDeleteOutput();
	void pSlotSubscribeInput();
	void pSlotDeleteInput();

public slots:
	void slotNameCheck(bool, int, QString);
signals:
	void nameValid();
public slots:
	void slotRefreshOutput(bool, int, interfaceInfo);
	//void slotRefreshInput(interfaceInfo, bool);

public slots:
	//接收表格内模型改变
	void slotModelChange(QString);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

private:
	//主表
	QTableWidget *m_pOutputList;
	QTableWidget *m_pInputList;

	//改发布接口对话框
	EditOutputDialog *dialog;

	//被选中模型
	QString selectedModel;

public:
	//core
	std::set<interfaceInfo> interfaceSet;
};
#endif // INTERFACEINFOSTACK_H