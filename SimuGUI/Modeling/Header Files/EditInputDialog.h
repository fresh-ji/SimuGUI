#pragma once

#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include "Regulation.h"

class EditInputDialog : public FancyDialog {

	Q_OBJECT

public:
	explicit EditInputDialog(QString, QSet<inputInfo>, QWidget *p = 0);
	//析构器
	~EditInputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void refreshInput(QSet<QString>, QSet<QString>);

signals:
	//消息信号，目前没用到
	void signalSendMessage(QString);

private:
	//模型名
	QString modelName;

	//之前的订阅
	QSet<QString> preSet;

	//core
	QTreeWidget *treeWidget;
};
#endif // EDITINPUTDIALOG_H