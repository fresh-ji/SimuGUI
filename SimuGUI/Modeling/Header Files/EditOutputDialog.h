#pragma once

/*
* @date : 2019/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include "GlobalConstant.h"
#include "Regulation.h"

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//����һ��Ĺ�����
	explicit EditOutputDialog(QList<QString>, QString, QWidget *p = 0);
	//�޸�һ��Ĺ�����
	explicit EditOutputDialog(QList<QString>, outputInfo, QWidget *p = 0);
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
	void createTemplate(QList<QString>);

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

	//������
	QTextEdit *noticeContent;

private:
	QString iconPath = ICON_PATH;
};
#endif // EDITOUTPUTDIALOG_H