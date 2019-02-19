#pragma once

/*
* @date : 2018/02/18
* @author : jihang
*/

#ifndef EDITOUTPUTDIALOG_H
#define EDITOUTPUTDIALOG_H

#include <set>

#include "cwidgets.h"
#include "fancydialog.h"

struct interfaceInfo {
	QString iName;
	QString iDataType;
	QString publisher;
	std::set<QString> subscribers;

	bool operator < (const interfaceInfo& i) const {
		return this->iName < i.iName;
	}
};

class EditOutputDialog : public FancyDialog {

	Q_OBJECT

public:
	//增加一项的构造器
	explicit EditOutputDialog(interfaceInfo, QWidget *p = 0);
	//修改一项的构造器
	explicit EditOutputDialog(interfaceInfo, int, QWidget *p = 0);
	//析构器
	~EditOutputDialog() {};

private slots:
	void pSlotFinish();
signals:
	void nameCheck(bool, int, QString);
public slots:
	void slotNameValid();
signals:
	void refreshOutput(bool, int, interfaceInfo);
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

	//发布者
	QString publisher;

	//数据名
	QLineEdit *nameText;

	//数据类型
	QComboBox *typeBox;
};
#endif // EDITOUTPUTDIALOG_H