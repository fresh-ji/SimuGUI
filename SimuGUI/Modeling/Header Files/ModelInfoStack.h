#pragma once

/*
* @date : 2019/01/25
* @author : jihang
*/

#ifndef MODELINFOSTACK_H
#define MODELINFOSTACK_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

class ModelInfoStack :public MiniStack {

	Q_OBJECT

public:
	explicit ModelInfoStack(QWidget *p = 0);
	~ModelInfoStack() {};

public slots:
	//��������ģ��
	void slotAddModel(QString, QString);
signals:
	//���ɾ��ģ��
	void signalDeleteModel(QString);
public slots:
	//����ģ�͵���ı�
	void slotModelChange(QString);
signals:
	//ģ�͸ı�
	void signalModelChange(QString);
private slots:
	void pSlotModelItemChanged();
	void pSlotEditModel();
	void pSlotDeleteModel();
signals:
	void signalSendMessage(QString);

private:
	QTableWidget *m_pModelList;
};
#endif // MODELINFOSTACK_H