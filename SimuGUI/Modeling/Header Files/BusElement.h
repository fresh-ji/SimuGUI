#pragma once

/*
* @date : 2019/04/20
* @author : jihang
*/

#ifndef BUSELEMENT_H
#define BUSELEMENT_H

#include "cwidgets.h"

//…Ëº∆
constexpr auto BUSWIDTH = 500;
constexpr auto BUSHEIGHT = 30;
constexpr auto STRETCHWIDTH = 20;

class BusElement : public QLabel {

	Q_OBJECT

public:
	explicit BusElement(QWidget *p = 0);

private:
	QLabel *rightAnchor;

public:
	void adjustAnchor();
};

#endif // BUSELEMENT_H