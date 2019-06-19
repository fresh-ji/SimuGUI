#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef DETAILSTACK_H
#define DETAILSTACK_H

#include <QMap>

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

#include "FMISupport.h"

class DetailStack :public MiniStack {

	Q_OBJECT

public:
	explicit DetailStack(QWidget *p = 0);
	~DetailStack() {};

public slots:
	void slotModelDetail(FMUInfo*);

private:
	QWidget *infoWidget;
	QGridLayout *infoLayout;
};

#endif // DETAILSTACK_H