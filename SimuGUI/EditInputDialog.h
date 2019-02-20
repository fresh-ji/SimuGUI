#pragma once

#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

#include "EditOutputDialog.h"

class EditInputDialog : public FancyDialog {

	Q_OBJECT

public:
	explicit EditInputDialog(QString, QWidget *p = 0);
	//析构器
	~EditInputDialog() {};

private slots:
	void pSlotFinish();

signals:
	//消息信号，目前没用到
	void signalSendMessage(QString);
};
#endif // EDITINPUTDIALOG_H