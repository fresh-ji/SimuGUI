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