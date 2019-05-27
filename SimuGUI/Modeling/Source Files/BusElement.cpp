
/*
* @date : 2019/04/20
* @author : jihang
*/

#include "BusElement.h"

BusElement::BusElement(QWidget *p) : QLabel(p) {

	setFixedWidth(BUSWIDTH);
	setFixedHeight(BUSHEIGHT);
	setText("CoSimulation Bus");
	setAlignment(Qt::AlignCenter);
	setStyleSheet("background: transparent #F49D2A;"
		"border: 3px solid #F49D2A; border-radius: 10px;"
		"font: 16pt;");

	stretchZone = new QLabel(this);
	stretchZone->setGeometry(this->width() - STRETCHWIDTH, 0,
		STRETCHWIDTH, BUSHEIGHT);
	stretchZone->setStyleSheet("background: transparent #a8918a;"
		"border: 0px solid black; border-radius: 10px;");
}

void BusElement::adjustStretchZone() {
	stretchZone->setGeometry(this->width() - STRETCHWIDTH, 0,
		STRETCHWIDTH, BUSHEIGHT);
}

void BusElement::effectOn() {
	setStyleSheet("background: transparent #a8918a;"
		"border: 3px solid #a8918a; border-radius: 10px;"
		"font: 16pt;");
}

void BusElement::effectOff() {
	setStyleSheet("background: transparent #F49D2A;"
		"border: 3px solid #F49D2A; border-radius: 10px;"
		"font: 16pt;");
}