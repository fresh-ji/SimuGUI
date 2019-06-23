#pragma once

/*
* @date : 2019/06/22
* @author : jihang
*/

#ifndef OVERVIEWDIALOG_H
#define OVERVIEWDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"
#include "Regulation.h"

class OverviewDialog :public FancyDialog {

	Q_OBJECT

public:
	explicit OverviewDialog(QMap<QString, dataTypeInfo>,
		QMap<QString, interfaceInfo>, QList<QString>, QWidget *p = 0);
	//Îö¹¹Æ÷
	~OverviewDialog() {}
};

#endif // OVERVIEWDIALOG_H