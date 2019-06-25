#pragma once

/*
* @date : 2019/02/16
* @author : jihang
*/

#ifndef EDITDATATYPEDIALOG_H
#define EDITDATATYPEDIALOG_H

#include "GlobalConstant.h"
#include "Regulation.h"

class EditDataTypeDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditDataTypeDialog(QList<QString>, QWidget *p = 0);
	//修改一项的构造器
	explicit EditDataTypeDialog(QList<QString>, QString, dataTypeInfo, QWidget *p = 0);
	//析构器
	~EditDataTypeDialog() {};

private slots:
	void pSlotAdd();
	void pSlotDelete();
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
	void createTemplate(QList<QString>);

	//是否添加
	bool isAdd;

	//修改时记录之前的key
	QString preName;

	//记录之前的data
	dataTypeInfo dInfo;

	//结构体名
	QLineEdit *nameText;

	//详情项
	QTextEdit *noticeContent;

	//添加的项
	QLineEdit *itemContent;
	QComboBox *typeContent;

	//详情
	QTableWidget *dataDetailList;

private:
	QString iconPath = ICON_PATH;
};
#endif // EDITDATATYPEDIALOG_H