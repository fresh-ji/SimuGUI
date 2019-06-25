#pragma once

/*
* @date : 2019/02/13
* @author : jihang
*/

#ifndef DATATYPESTACK_H
#define DATATYPESTACK_H

#include "ministack.h"
#include "fancybutton.h"

#include "EditDataTypeDialog.h"

class DataTypeStack : public MiniStack {

	Q_OBJECT

public:
	explicit DataTypeStack(QWidget *p = 0);
	~DataTypeStack() {}

private slots:
	//��ӣ��༭��ɾ��
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
	//����
	QTableWidget *m_pDataTypeList;

	//���������ͶԻ���
	EditDataTypeDialog *dialog;

private:
	//core
	QMap<QString, dataTypeInfo> dataTypeMap;

private:
	QString iconPath = ICON_PATH;
};

#endif //  DATATYPESTACK_H