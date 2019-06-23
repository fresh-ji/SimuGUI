#pragma once

/*
* @date : 2019/02/13
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
	void slotNameCheck(QString);
signals:
	void nameValid();
public slots:
	void slotRefreshDataType(bool, QString, QString, dataTypeInfo);

signals:
	void refreshTypeSet(QString, QString);

signals:
	void signalSendMessage(QString);
public slots:
	void slotMessageFromDialog(QString);

public slots:
	QMap<QString, dataTypeInfo> slotGetOverview();

private:
	//主表
	QTableWidget *m_pDataTypeList;

	//改数据类型对话框
	EditDataTypeDialog *dialog;

private:
	//core
	QMap<QString, dataTypeInfo> dataTypeMap;
};

#endif //  DATATYPESTACK_H