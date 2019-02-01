#pragma once

#ifndef ADDINTERFACEDIALOG_H
#define ADDINTERFACEDIALOG_H

//QSet不好用
#include <set>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"
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

class EditInterfaceDialog :public FancyDialog {

	Q_OBJECT

public:
	explicit EditInterfaceDialog(interfaceInfo, bool, QWidget *p = 0);
	~EditInterfaceDialog() {};

private slots:
	void pSlotFinish();
signals:
	void refreshInterface(interfaceInfo, bool);
signals:
	void signalSendMessage(QString);

private:
	//接口模型
	interfaceInfo iInfo;
	//是否添加
	bool isAdd;
	//数据名
	QLineEdit *nameText;
	//数据类型
	QComboBox *typeBox;
};
#endif // ADDINTERFACEDIALOG_H