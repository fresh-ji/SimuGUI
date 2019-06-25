#pragma once

/*
* @date : 2019/02/18
* @author : jihang
*/

#ifndef INTERFACEINFOSTACK_H
#define INTERFACEINFOSTACK_H

#include "EditOutputDialog.h"
#include "EditInputDialog.h"

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
	void slotRefreshInput(QSet<QString>, QSet<QString>);

public slots:
	//���ձ����ģ�͸ı�
	void slotModelChange(QString);

public slots:
	//�����Զ����������͸���
	void slotRefreshTypeSet(QString, QString);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

public slots:
	QMap<QString, interfaceInfo> slotGetOverview();

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

	//Ŀǰ���Զ�����������
	QList<QString> types;

private:
	//core
	QMap<QString, interfaceInfo> interfaceMap;

private:
	QString iconPath = ICON_PATH;
};
#endif // INTERFACEINFOSTACK_H