#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

struct outputInfo {
	QString oName;
	QString oDataType;
	QString oPublisher;
};

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditOutputDialog(QString, QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditOutputDialog(outputInfo, QWidget *p = 0);
	//������
	~EditOutputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(QString);
public slots:
	void slotNameValid();
signals:
	void refreshOutput(bool, QString, outputInfo);
signals:
	//��Ϣ�źţ�Ŀǰû�õ�
	void signalSendMessage(QString);

private:
	//��������ģ��
	void createTemplate();

	//�Ƿ����
	bool isAdd;

	//�޸�ʱ��¼֮ǰ��key
	QString preName;

	//������
	QString publisher;

	//������
	QLineEdit *nameText;

	//��������
	QComboBox *typeBox;
};
#endif // EDITOUTPUTDIALOG_H