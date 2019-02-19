#pragma once

/*
* @date : 2018/02/13
* @author : jihang
*/

#ifndef DATATYPESTACK_H
#define DATATYPESTACK_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "EditDataTypeDialog.h"

class DataTypeStack : public MiniStack {

	Q_OBJECT

public:
	explicit DataTypeStack(QWidget *p = 0);
	~DataTypeStack() {};

private slots:
	//添加，编辑，删除
	void pSlotAddDataType();
	void pSlotEditDataType();
	void pSlotDeleteDataType();

public slots:
	void slotNameCheck(bool, int, QString);
signals:
	void nameValid();
public slots:
	void slotRefreshDataType(bool, int, dataTypeInfo);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

private:
	//主表
	QTableWidget *m_pDataTypeList;

	//改数据类型对话框
	EditDataTypeDialog *dialog;

public:
	//core
	std::set<dataTypeInfo> dataTypeSet;
};

#endif //  DATATYPESTACK_H