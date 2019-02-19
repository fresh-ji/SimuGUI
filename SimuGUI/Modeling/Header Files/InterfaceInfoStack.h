#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

//QSet������
#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditOutputDialog.h"

//constexpr auto outputType = "output";
//constexpr auto inputType = "input";

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
	void slotNameCheck(bool, int, QString);
signals:
	void nameValid();
public slots:
	void slotRefreshOutput(bool, int, interfaceInfo);
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

	//��ѡ��ģ��
	QString selectedModel;

public:
	//core
	std::set<interfaceInfo> interfaceSet;
};
#endif // INTERFACEINFOSTACK_H