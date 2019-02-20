#pragma once

/*
* @date : 2018/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

struct dataTypeInfo {
	int count;
	//TODO:内容待定
};

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditDataTypeDialog(QWidget *p = 0);
	//修改一项的构造器
	explicit EditDataTypeDialog(QString, dataTypeInfo, QWidget *p = 0);
	//析构器
	~EditDataTypeDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(QString);
public slots:
	void slotNameValid();
signals:
	void refreshDataType(bool, QString, QString, dataTypeInfo);
signals:
	//消息信号，目前没用到
	void signalSendMessage(QString);

private:
	//创建背景模板
	void createTemplate();

	//是否添加
	bool isAdd;

	//修改时记录之前的key
	QString preName;

	//数据名
	QLineEdit *nameText;
};
#endif // EDITDATATYPEDIALOG_H