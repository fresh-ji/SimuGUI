
#include "ItemElement.h"

ItemElement::ItemElement(QPixmap *pixmap, QString name,
	QString type, int count, QWidget *p) : QLabel(p) {

	this->name = name;
	this->type = type;
	this->count = count;

	setFixedWidth(TOTALWIDTH);
	setFixedHeight(TEXTHEIGHT * 2 + WIDTH + EDGE * 4);
	setStyleSheet("border: 0px solid black;background: transparent;");

	//1
	topNameLabel = new QLabel(this);
	topNameLabel->setGeometry(
		0,
		0,
		TOTALWIDTH,
		TEXTHEIGHT);
	topNameLabel->setStyleSheet("border: 0px solid black;");
	topNameLabel->setAlignment(Qt::AlignCenter);
	topNameLabel->setText(name);

	//2
	bottomNameLabel = new QLabel(this);
	bottomNameLabel->setGeometry(
		0,
		TEXTHEIGHT + WIDTH + EDGE * 4,
		TOTALWIDTH,
		TEXTHEIGHT);
	bottomNameLabel->setStyleSheet("border: 0px solid black;");
	bottomNameLabel->setAlignment(Qt::AlignCenter);
	

	//3
	frameLabel = new QLabel(this);
	frameLabel->setGeometry(FRAMESHIFTX, FRAMESHIFTY,
		WIDTH, WIDTH + EDGE * 2);
	frameLabel->setStyleSheet("background: transparent #93D5FD;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	//4
	topAnchor = new QLabel(this);
	topAnchor->setGeometry(
		(TOTALWIDTH - SMALLANCHOR) / 2,
		TEXTHEIGHT + EDGE - (SMALLANCHOR / 2),
		SMALLANCHOR,
		SMALLANCHOR);
	topAnchor->setStyleSheet("background: #1890FF;"
		"border: 2px solid #1890FF;");

	//5
	bottomAnchor = new QLabel(this);
	bottomAnchor->setGeometry(
		(TOTALWIDTH - SMALLANCHOR) / 2,
		TEXTHEIGHT + EDGE * 3 + WIDTH - (SMALLANCHOR / 2),
		SMALLANCHOR,
		SMALLANCHOR);
	bottomAnchor->setStyleSheet("background: #1890FF;"
		"border: 2px solid #1890FF;");

	//6
	leftAnchor = new QLabel(this);
	leftAnchor->setGeometry(
		(TOTALWIDTH - WIDTH - SMALLANCHOR) / 2,
		TEXTHEIGHT + EDGE * 2 + ((WIDTH - SMALLANCHOR) / 2),
		SMALLANCHOR,
		SMALLANCHOR);
	leftAnchor->setStyleSheet("background: #1890FF;"
		"border: 2px solid #1890FF;");

	//7
	rightAnchor = new QLabel(this);
	rightAnchor->setGeometry(
		(TOTALWIDTH + WIDTH - SMALLANCHOR) / 2,
		TEXTHEIGHT + EDGE * 2 + ((WIDTH - SMALLANCHOR) / 2),
		SMALLANCHOR,
		SMALLANCHOR);
	rightAnchor->setStyleSheet("background: #1890FF;"
		"border: 2px solid #1890FF;");

	//8
	imageLabel = new QLabel(this);
	imageLabel->setGeometry(
		(TOTALWIDTH - WIDTH) / 2 + GAP,
		TEXTHEIGHT + EDGE * 2 + GAP,
		WIDTH - GAP * 2,
		WIDTH - GAP * 2);
	imageLabel->setStyleSheet("border:0px solid black;");
	pixmap->scaled(imageLabel->size(), Qt::KeepAspectRatio);
	imageLabel->setScaledContents(true);
	imageLabel->setPixmap(*pixmap);
}

void ItemElement::inactive() {

	frameLabel->setStyleSheet("background: transparent #E7F7FE;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	topAnchor->setVisible(false);
	bottomAnchor->setVisible(false);
	leftAnchor->setVisible(false);
	rightAnchor->setVisible(false);
}

void ItemElement::active() {

	frameLabel->setStyleSheet("background: transparent #93D5FD;"
		"border: 1.5px solid #1890FF; border-radius: 5px");

	topAnchor->setVisible(true);
	bottomAnchor->setVisible(true);
	leftAnchor->setVisible(true);
	rightAnchor->setVisible(true);
}

ItemElement::Anchor ItemElement::whichAnchor(QPoint offset) {
	if (topAnchor->geometry().contains(offset)) {
		return TOP;
	}
	if (bottomAnchor->geometry().contains(offset)) {
		return BOTTOM;
	}
	if (leftAnchor->geometry().contains(offset)) {
		return LEFT;
	}
	if (rightAnchor->geometry().contains(offset)) {
		return RIGHT;
	}
	return NONE;
}

QPoint ItemElement::getAnchorShift(Anchor anchor) {
	QPoint point;
	if (TOP == anchor) {
		point.setX(TOPANCHORSHIFTX);
		point.setY(TOPANCHORSHIFTY);
	}
	else if (BOTTOM == anchor) {
		point.setX(BOTTOMANCHORSHIFTX);
		point.setY(BOTTOMANCHORSHIFTY);
	}
	else if (LEFT == anchor) {
		point.setX(LEFTANCHORSHIFTX);
		point.setY(LEFTANCHORSHIFTY);
	}
	else if (RIGHT == anchor) {
		point.setX(RIGHTANCHORSHIFTX);
		point.setY(RIGHTANCHORSHIFTY);
	}
	return point;
}

QString ItemElement::getName() {
	return name;
}

void ItemElement::setName(QString name) {
	this->name = name;
}

QString ItemElement::getType() {
	return type;
}

int ItemElement::getCount() {
	return count;
}