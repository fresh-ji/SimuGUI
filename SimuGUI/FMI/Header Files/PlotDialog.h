#pragma once

/*
* @date : 2019/06/22
* @author : jihang
*/

#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include "cwidgets.h"
#include "fancydialog.h"

class PlotDialog :public FancyDialog {

	Q_OBJECT

public:

	explicit PlotDialog(QWidget *p = 0);
	//Îö¹¹Æ÷
	~PlotDialog() {}
};

#endif // PLOTDIALOG_H