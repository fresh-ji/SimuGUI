#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef DETAILSTACK_H
#define DETAILSTACK_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancybutton.h"

class DetailStack :public MiniStack {

	Q_OBJECT

public:

	explicit DetailStack(QWidget *p = 0);

	~DetailStack() {};
};

#endif // DETAILSTACK_H