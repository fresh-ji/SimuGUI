#pragma once

/*
* @date : 2018/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include <set>

#include "cwidgets.h"
#include "fancydialog.h"

struct dataTypeInfo {
	QString dName;
	//TODO:内容这块待定

	bool operator < (const dataTypeInfo& d) const {
		return this->dName < d.dName;
	}
};

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditDataTypeDialog(QWidget *p = 0);
	//修改一项的构造器
	explicit EditDataTypeDialog(dataTypeInfo, int, QWidget *p = 0);
	//析构器
	~EditDataTypeDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(bool, int, QString);
public slots:
	void slotNameValid();
signals:
	void refreshDataType(bool, int, dataTypeInfo);
signals:
	//消息信号，目前没用到
	void signalSendMessage(QString);

private:
	//创建背景模板
	void createTemplate();

	//是否添加
	bool isAdd;

	//修改时记录在set中位置
	int setIndex;

	//数据名
	QLineEdit *nameText;
};
#endif // EDITDATATYPEDIALOG_H