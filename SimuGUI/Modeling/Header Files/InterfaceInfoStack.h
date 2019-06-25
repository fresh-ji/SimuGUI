#pragma once

/*
* @date : 2019/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

#include "EditOutputDialog.h"
#include "EditInputDialog.h"

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
	void slotRefreshInput(QSet<QString>, QSet<QString>);

public slots:
	//接收表格内模型改变
	void slotModelChange(QString);

public slots:
	//接收自定义数据类型更新
	void slotRefreshTypeSet(QString, QString);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

public slots:
	QMap<QString, interfaceInfo> slotGetOverview();

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

	//目前的自定义数据类型
	QList<QString> types;

private:
	//core
	QMap<QString, interfaceInfo> interfaceMap;

private:
	QString iconPath = ICON_PATH;
};
#endif // INTERFACEINFOSTACK_H