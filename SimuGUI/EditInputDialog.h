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
	//������
	~EditInputDialog() {};

private slots:
	void pSlotFinish();

signals:
	//��Ϣ�źţ�Ŀǰû�õ�
	void signalSendMessage(QString);
};
#endif // EDITINPUTDIALOG_H