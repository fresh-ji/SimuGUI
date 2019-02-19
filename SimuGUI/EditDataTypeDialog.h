#pragma once

/*
* @date : 2018/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include <set>

#include "cwidgets.h"
#include "fancydialog.h"

struct dataTypeInfo {
	QString dName;
	//TODO:����������

	bool operator < (const dataTypeInfo& d) const {
		return this->dName < d.dName;
	}
};

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditDataTypeDialog(QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditDataTypeDialog(dataTypeInfo, int, QWidget *p = 0);
	//������
	~EditDataTypeDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(bool, int, QString);
public slots:
	void slotNameValid();
signals:
	void refreshDataType(bool, int, dataTypeInfo);
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
	QLineEdit *nameText;
};
#endif // EDITDATATYPEDIALOG_H