#pragma once

/*
* @date : 2018/01/04
* @author : jihang
*/

#ifndef MODELING_H
#define MODELING_H

#include "imode.h"

#include "cwidgets.h"
#include "ministack.h"
#include "DropLabel.h"
#include "ToolDragStack.h"

class Modeling : public IMode {

	Q_OBJECT

public:
	explicit Modeling(QWidget *parent = 0);

private:
	void createWindow();
	void createToolStack();
	void createModelStack();
	void createModelList();
	void createInterfaceList();

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
public slots:
	void slotReceiveMessage(QString);

private slots:
	void pSlotDeleteOne();
	void pSlotModelItemChanged();

private:
	ToolDragStack *m_pToolStack;
	DropLabel *m_pModelLabel;
	QTableWidget *m_pModelList;
	QTableWidget *m_pInterfaceList;
};
#endif // MODELING_H