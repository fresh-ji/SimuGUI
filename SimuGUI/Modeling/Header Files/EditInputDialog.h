#pragma once

#ifndef EDITINPUTDIALOG_H
#define EDITINPUTDIALOG_H

#include "Regulation.h"

class EditInputDialog : public FancyDialog {

	Q_OBJECT

public:
	explicit EditInputDialog(QString, QSet<inputInfo>, QWidget *p = 0);
	//������
	~EditInputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void refreshInput(QSet<QString>, QSet<QString>);

signals:
	//��Ϣ�źţ�Ŀǰû�õ�
	void signalSendMessage(QString);

private:
	//ģ����
	QString modelName;

	//֮ǰ�Ķ���
	QSet<QString> preSet;

	//core
	QTreeWidget *treeWidget;
};
#endif // EDITINPUTDIALOG_H