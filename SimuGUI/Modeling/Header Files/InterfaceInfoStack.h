#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditOutputDialog.h"
#include "EditInputDialog.h"

struct interfaceInfo {
	QString dataType;
	QString publisher;
	std::set<QString> subscribers;
};

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
	void slotNameCheck(QString);
signals:
	void nameValid();
public slots:
	void slotRefreshOutput(bool, QString, outputInfo);
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

	//改订阅接口对话框
	EditInputDialog *dialog_in;

	//被选中模型
	QString selectedModel;

public:
	//core
	QMap<QString, interfaceInfo> interfaceMap;
};
#endif // INTERFACEINFOSTACK_H