#pragma once

/*
* @date : 2018/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include "Regulation.h"

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditDataTypeDialog(QList<QString>, QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditDataTypeDialog(QList<QString>, QString, dataTypeInfo, QWidget *p = 0);
	//������
	~EditDataTypeDialog() {};

private slots:
	void pSlotAdd();
	void pSlotDelete();
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
	void createTemplate(QList<QString>);

	//�Ƿ����
	bool isAdd;

	//�޸�ʱ��¼֮ǰ��key
	QString preName;

	//��¼֮ǰ��data
	dataTypeInfo dInfo;

	//�ṹ����
	QLineEdit *nameText;

	//��ӵ���
	QLineEdit *nameContent;
	QComboBox *typeContent;

	//����
	QTableWidget *dataDetailList;
};
#endif // EDITDATATYPEDIALOG_H