#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

struct outputInfo {
	QString oName;
	QString oDataType;
	QString oPublisher;
};

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditOutputDialog(QString, QWidget *p = 0);
	//修改一项的构造器
	explicit EditOutputDialog(outputInfo, QWidget *p = 0);
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
	void createTemplate();

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
};
#endif // EDITOUTPUTDIALOG_H