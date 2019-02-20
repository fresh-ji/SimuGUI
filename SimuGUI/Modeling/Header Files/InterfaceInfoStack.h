#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditOutputDialog.h"
#include "EditInputDialog.h"

struct interfaceInfo {
	QString dataType;
	QString publisher;
	std::set<QString> subscribers;
};

class InterfaceInfoStack : public MiniStack {

	Q_OBJECT

public:
	explicit InterfaceInfoStack(QWidget *p = 0);
	~InterfaceInfoStack() {};

private slots:
	//���������������룬�༭��ɾ��
	void pSlotAddOutput();
	void pSlotEditOutput();
	void pSlotDeleteOutput();
	void pSlotSubscribeInput();
	void pSlotDeleteInput();

public slots:
	void slotNameCheck(QString);
signals:
	void nameValid();
public slots:
	void slotRefreshOutput(bool, QString, outputInfo);
	//void slotRefreshInput(interfaceInfo, bool);

public slots:
	//���ձ����ģ�͸ı�
	void slotModelChange(QString);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

private:
	//����
	QTableWidget *m_pOutputList;
	QTableWidget *m_pInputList;

	//�ķ����ӿڶԻ���
	EditOutputDialog *dialog;

	//�Ķ��ĽӿڶԻ���
	EditInputDialog *dialog_in;

	//��ѡ��ģ��
	QString selectedModel;

public:
	//core
	QMap<QString, interfaceInfo> interfaceMap;
};
#endif // INTERFACEINFOSTACK_H