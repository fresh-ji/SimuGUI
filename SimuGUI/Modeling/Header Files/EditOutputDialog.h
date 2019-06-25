#pragma once

/*
* @date : 2019/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include "GlobalConstant.h"
#include "Regulation.h"

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditOutputDialog(QList<QString>, QString, QWidget *p = 0);
	//修改一项的构造器
	explicit EditOutputDialog(QList<QString>, outputInfo, QWidget *p = 0);
	//析构器
	~EditOutputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(QString);
public slots:
	void slotNameValid();
signals:
	void refreshOutput(bool, QString, outputInfo);
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

	//发布者
	QString publisher;

	//数据名
	QLineEdit *nameText;

	//数据类型
	QComboBox *typeBox;

	//详情项
	QTextEdit *noticeContent;

private:
	QString iconPath = ICON_PATH;
};
#endif // EDITOUTPUTDIALOG_H