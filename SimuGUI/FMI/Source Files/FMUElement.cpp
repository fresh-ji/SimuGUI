
#include "FMUElement.h"

FMUElement::FMUElement(QString name, int inputNumber,
	int outputNumber, QWidget *p) : QLabel(p) {

	this->name = name;

	setFixedWidth(FMUTOTALWIDTH);
	setFixedHeight(FMUTEXTHEIGHT * 2 + FMUWIDTH + FMUEDGE * 4);
	setStyleSheet("border: 0px solid black;background: transparent;");

	//1
	topNameLabel = new QLabel(this);
	topNameLabel->setGeometry(
		0,
		0,
		FMUTOTALWIDTH,
		FMUTEXTHEIGHT);
	topNameLabel->setStyleSheet("border: 0px solid black;");
	topNameLabel->setAlignment(Qt::AlignCenter);
	topNameLabel->setText(name);

	//2
	bottomNameLabel = new QLabel(this);
	bottomNameLabel->setGeometry(
		0,
		FMUTEXTHEIGHT + FMUWIDTH + FMUEDGE * 4,
		FMUTOTALWIDTH,
		FMUTEXTHEIGHT);
	bottomNameLabel->setStyleSheet("border: 0px solid black;");
	bottomNameLabel->setAlignment(Qt::AlignCenter);


	//3
	frameLabel = new QLabel(this);
	frameLabel->setGeometry(FMUFRAMESHIFTX, FMUFRAMESHIFTY,
		FMUWIDTH, FMUWIDTH + FMUEDGE * 2);
	frameLabel->setStyleSheet("background: transparent #93D5FD;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	//4
	if (inputNumber > 0) {
		inputAnchor = new QLabel(this);
		inputAnchor->setGeometry(
			(FMUTOTALWIDTH - FMUWIDTH - FMUSMALLANCHOR) / 2,
			FMUTEXTHEIGHT + FMUEDGE * 2 + ((FMUWIDTH - FMUSMALLANCHOR) / 2),
			FMUSMALLANCHOR,
			FMUSMALLANCHOR);
		inputAnchor->setStyleSheet("background: #1890FF;"
			"border: 2px solid #1890FF;");
	}
	else {
		inputAnchor = NULL;
	}

	//5
	if (outputNumber > 0) {
		outputAnchor = new QLabel(this);
		outputAnchor->setGeometry(
			(FMUTOTALWIDTH + FMUWIDTH - FMUSMALLANCHOR) / 2,
			FMUTEXTHEIGHT + FMUEDGE * 2 + ((FMUWIDTH - FMUSMALLANCHOR) / 2),
			FMUSMALLANCHOR,
			FMUSMALLANCHOR);
		outputAnchor->setStyleSheet("background: #1890FF;"
			"border: 2px solid #1890FF;");
	}
	else {
		outputAnchor = NULL;
	}

	//6
	QLabel *imageLabel = new QLabel(this);
	imageLabel->setGeometry(
		(FMUTOTALWIDTH - FMUWIDTH) / 2 + FMUGAP,
		FMUTEXTHEIGHT + FMUEDGE * 2 + FMUGAP,
		FMUWIDTH - FMUGAP * 2,
		FMUWIDTH - FMUGAP * 2);
	imageLabel->setStyleSheet("border:0px solid black;");
	QPixmap *pixmap = new QPixmap(iconPath + "simutool/fmi");
	pixmap->scaled(imageLabel->size(), Qt::KeepAspectRatio);
	imageLabel->setScaledContents(true);
	imageLabel->setPixmap(*pixmap);
}

void FMUElement::inactive() {

	frameLabel->setStyleSheet("background: transparent #E7F7FE;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	if (inputAnchor != NULL) {
		inputAnchor->setVisible(false);
	}
	if (outputAnchor != NULL) {
		outputAnchor->setVisible(false);
	}
}

void FMUElement::active() {

	frameLabel->setStyleSheet("background: transparent #93D5FD;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	if (inputAnchor != NULL) {
		inputAnchor->setVisible(true);
	}
	if (outputAnchor != NULL) {
		outputAnchor->setVisible(true);
	}
}

FMUElement::Anchor FMUElement::whichAnchor(QPoint offset) {
	if (inputAnchor != NULL) {
		if (inputAnchor->geometry().contains(offset)) {
			return INPUT;
		}
	}
	if (outputAnchor != NULL) {
		if (outputAnchor->geometry().contains(offset)) {
			return OUTPUT;
		}
	}
	return NONE;
}

QPoint FMUElement::getAnchorShift(Anchor anchor) {
	QPoint point;
	if (INPUT == anchor) {
		point.setX(FMUINPUTANCHORSHIFTX);
		point.setY(FMUINPUTANCHORSHIFTY);
	}
	else if (OUTPUT == anchor) {
		point.setX(FMUOUTPUTANCHORSHIFTX);
		point.setY(FMUOUTPUTANCHORSHIFTY);
	}
	return point;
}

QString FMUElement::getName() {
	return name;
}

void FMUElement::setName(QString name) {
	this->name = name;
}