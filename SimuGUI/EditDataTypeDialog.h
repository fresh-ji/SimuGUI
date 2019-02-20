#pragma once

/*
* @date : 2018/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

struct dataTypeInfo {
	int count;
	//TODO:���ݴ���
};

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditDataTypeDialog(QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditDataTypeDialog(QString, dataTypeInfo, QWidget *p = 0);
	//������
	~EditDataTypeDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(QString);
public slots:
	void slotNameValid();
signals:
	void refreshDataType(bool, QString, QString, dataTypeInfo);
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
	QLineEdit *nameText;
};
#endif // EDITDATATYPEDIALOG_H