#pragma once

#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

struct inputInfo {
	QString iName;
	QString iDataType;
	QString iPublisher;
	bool isSubscribe;

	bool operator < (const inputInfo& i) const {
		return this->iName < i.iName;
	}

	bool operator == (const inputInfo& i) const {
		return this->iName == i.iName;
	}
};

class EditInputDialog : public FancyDialog {

	Q_OBJECT

public:
	explicit EditInputDialog(QString, QSet<inputInfo>, QWidget *p = 0);
	//������
	~EditInputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void refreshInput(QSet<QString>);

signals:
	//��Ϣ�źţ�Ŀǰû�õ�
	void signalSendMessage(QString);

private:
	//ģ����
	QString modelName;
};
#endif // EDITINPUTDIALOG_H