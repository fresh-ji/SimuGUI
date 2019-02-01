#pragma once

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

//QSet不好用
#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditInterfaceDialog.h"

constexpr auto outputType = "output";
constexpr auto inputType = "input";

class InterfaceInfoStack :public MiniStack {

	Q_OBJECT

public:

	explicit InterfaceInfoStack(QWidget *p = 0);
	~InterfaceInfoStack() {};

public slots:
	//接收表格内模型改变
	void slotModelChange(QString);
public slots:
	void slotRefreshInterface(interfaceInfo, bool);
private slots:
	//添加输出，订阅输入，编辑，删除
	void pSlotAddInterface();
	void pSlotSubscribeInterface();
	void pSlotEditInterface();
	void pSlotDeleteInterface();
signals:
	void signalSendMessage(QString);
public slots:
	//从dialog来的，被转发到center
	void slotMessageFromDialog(QString);

private:
	QTableWidget *m_pInterfaceList;

	//被选中模型
	QString selectedModel;

	//core
	std::set<interfaceInfo> interfaceSet;

	//改发布接口对话框
	EditInterfaceDialog *dialog;
};
#endif // INTERFACEINFOSTACK_H