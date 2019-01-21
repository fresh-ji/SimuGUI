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
	//接收增加模型
	void slotAddModel(QString, QString);
signals:
	//点击删除模型
	void signalDeleteModel(QString);
public slots:
	//接收模型点击改变
	void slotModelChange(QString);
signals:
	//模型改变
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