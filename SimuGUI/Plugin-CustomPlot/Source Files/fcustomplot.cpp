/****************************************************************************
**
** Copyright (C) 2014 dragondjf
**
** QFramer is a frame based on Qt5.3, you will be more efficient with it.
** As an Qter, Qt give us a nice coding experience. With user interactive experience(UE)
** become more and more important in modern software, deveployers should consider business and UE.
** So, QFramer is born. QFramer's goal is to be a mature solution
** which you only need to be focus on your business but UE for all Qters.
**
** Version	: 0.2.5.0
** Author	: dragondjf
** Website	: https://github.com/dragondjf
** Project	: https://github.com/dragondjf/QCFramer
** Blog		: http://my.oschina.net/dragondjf/home/?ft=atme
** Wiki		: https://github.com/dragondjf/QCFramer/wiki
** Lincence: LGPL V2
** QQ: 465398889
** Email: dragondjf@gmail.com, ding465398889@163.com, 465398889@qq.com
**
****************************************************************************/

#include "fcustomplot.h"
#include <QLinearGradient>


FCustomPlot::FCustomPlot(QWidget *parent) :
	QCustomPlot(parent)
{
	QLinearGradient linearGrad1(QPointF(0, 0), QPointF(200, 200));
	linearGrad1.setColorAt(0, QColor(33, 50, 70));
	linearGrad1.setColorAt(1, QColor(255, 255, 255));
	linearGrad1.setSpread(QGradient::PadSpread);
	setBackground(QBrush(linearGrad1));
}
