#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include <set>

#include "cwidgets.h"
#include "fancydialog.h"

struct interfaceInfo {
	QString iName;
	QString iDataType;
	QString publisher;
	std::set<QString> subscribers;

	bool operator < (const interfaceInfo& i) const {
		return this->iName < i.iName;
	}
};

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditOutputDialog(interfaceInfo, QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditOutputDialog(interfaceInfo, int, QWidget *p = 0);
	//������
	~EditOutputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(bool, int, QString);
public slots:
	void slotNameValid();
signals:
	void refreshOutput(bool, int, interfaceInfo);
signals:
	//��Ϣ�źţ�Ŀǰû�õ�
	void signalSendMessage(QString);

private:
	//��������ģ��
	void createTemplate();

	//�Ƿ����
	bool isAdd;

	//�޸�ʱ��¼��set��λ��
	int setIndex;

	//������
	QString publisher;

	//������
	QLineEdit *nameText;

	//��������
	QComboBox *typeBox;
};
#endif // EDITOUTPUTDIALOG_H