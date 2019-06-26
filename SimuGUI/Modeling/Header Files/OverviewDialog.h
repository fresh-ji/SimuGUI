#pragma once

/*
* @date : 2019/06/22
* @author : jihang
*/

#ifndef OVERVIEWDIALOG_H
#define OVERVIEWDIALOG_H

#include "GlobalConstant.h"

#include "fancydialog.h"

#include "Regulation.h"

class OverviewDialog :public FancyDialog {

	Q_OBJECT

public:
	explicit OverviewDialog(QMap<QString, dataTypeInfo>,
		QMap<QString, interfaceInfo>, QList<QString>, QWidget *p = 0);
	//Îö¹¹Æ÷
	~OverviewDialog() {}

private slots:
	void pSlotFinish();

private:
	QString iconPath = ICON_PATH;
};

#endif // OVERVIEWDIALOG_H