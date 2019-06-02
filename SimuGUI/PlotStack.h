#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef PLOTSTACK_H
#define PLOTSTACK_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

class PlotStack :public MiniStack {

	Q_OBJECT

public:

	explicit PlotStack(QWidget *p = 0);

	~PlotStack() {};
};

#endif // PLOTSTACK_H