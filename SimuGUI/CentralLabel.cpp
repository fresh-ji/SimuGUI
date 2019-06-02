
/*
* @date : 2019/06/01
* @author : jihang
*/

#include "CentralLabel.h"

CentralLabel::CentralLabel(QWidget *p) : QLabel(p) {

	setStyleSheet("border: 5px solid gray;"
		"background-color: white;border-radius: 10px;");
	//TODO：加格点功能

	//无被选中模型
	activeModel = NULL;

	//添加功能栏
	addToolBar();
}

void CentralLabel::mousePressEvent(QMouseEvent *event) {

}

void CentralLabel::dragMoveEvent(QDragMoveEvent *event) {

}

void CentralLabel::dropEvent(QDropEvent *event) {

}

void CentralLabel::addToolBar() {
	QToolButton *openButton = new QToolButton(this);
	openButton->setGeometry(20, 5, 50, 50);
	openButton->setCursor(Qt::PointingHandCursor);
	openButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	openButton->setIcon(QIcon("./Icon/function/open"));
	connect(openButton, SIGNAL(clicked()), this, SLOT(slotOpen()));

	QToolButton *undoButton = new QToolButton(this);
	undoButton->setGeometry(70, 5, 50, 50);
	undoButton->setCursor(Qt::PointingHandCursor);
	undoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	undoButton->setIcon(QIcon("./Icon/function/undo"));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(slotUndo()));

	QToolButton *redoButton = new QToolButton(this);
	redoButton->setGeometry(120, 5, 50, 50);
	redoButton->setCursor(Qt::PointingHandCursor);
	redoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	redoButton->setIcon(QIcon("./Icon/function/redo"));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(slotRedo()));

	QToolButton *gridButton = new QToolButton(this);
	gridButton->setGeometry(170, 5, 50, 50);
	gridButton->setCursor(Qt::PointingHandCursor);
	gridButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gridButton->setIcon(QIcon("./Icon/function/grid"));
	connect(gridButton, SIGNAL(clicked()), this, SLOT(slotGrid()));

	QToolButton *cutbutton = new QToolButton(this);
	cutbutton->setGeometry(220, 5, 50, 50);
	cutbutton->setCursor(Qt::PointingHandCursor);
	cutbutton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	cutbutton->setIcon(QIcon("./Icon/function/cut"));
	connect(cutbutton, SIGNAL(clicked()), this, SLOT(slotCut()));

	QToolButton *gobutton = new QToolButton(this);
	gobutton->setGeometry(270, 5, 50, 50);
	gobutton->setCursor(Qt::PointingHandCursor);
	gobutton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gobutton->setIcon(QIcon("./Icon/function/go"));
	connect(gobutton, SIGNAL(clicked()), this, SLOT(slotGo()));
}

void CentralLabel::slotOpen() {

}

void CentralLabel::slotUndo() {

}

void CentralLabel::slotRedo() {

}

void CentralLabel::slotGrid() {

}

void CentralLabel::slotCut() {

}

void CentralLabel::slotGo() {

}