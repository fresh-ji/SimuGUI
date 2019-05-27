#pragma once

/*
* @date : 2019/04/20
* @author : jihang
*/

#ifndef BUSELEMENT_H
#define BUSELEMENT_H

#include "cwidgets.h"

//���
constexpr auto BUSWIDTH = 500;
constexpr auto BUSHEIGHT = 30;
constexpr auto STRETCHWIDTH = 20;

class BusElement : public QLabel {

	Q_OBJECT

public:
	explicit BusElement(QWidget *p = 0);

private:
	//������
	QLabel *stretchZone;

public:
	//����ʱ��̬�ı�������
	void adjustStretchZone();

	//
	void effectOn();
	//
	void effectOff();
};

#endif // BUSELEMENT_H