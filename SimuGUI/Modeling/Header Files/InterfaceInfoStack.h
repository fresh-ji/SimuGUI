#pragma once

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

//QSet������
#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditInterfaceDialog.h"

constexpr auto outputType = "output";
constexpr auto inputType = "input";

class InterfaceInfoStack :public MiniStack {

	Q_OBJECT

public:

	explicit InterfaceInfoStack(QWidget *p = 0);
	~InterfaceInfoStack() {};

public slots:
	//���ձ����ģ�͸ı�
	void slotModelChange(QString);
public slots:
	void slotRefreshInterface(interfaceInfo, bool);
private slots:
	//���������������룬�༭��ɾ��
	void pSlotAddInterface();
	void pSlotSubscribeInterface();
	void pSlotEditInterface();
	void pSlotDeleteInterface();
signals:
	void signalSendMessage(QString);
public slots:
	//��dialog���ģ���ת����center
	void slotMessageFromDialog(QString);

private:
	QTableWidget *m_pInterfaceList;

	//��ѡ��ģ��
	QString selectedModel;

	//core
	std::set<interfaceInfo> interfaceSet;

	//�ķ����ӿڶԻ���
	EditInterfaceDialog *dialog;
};
#endif // INTERFACEINFOSTACK_H