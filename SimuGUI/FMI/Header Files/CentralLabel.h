#pragma once

/*
* @date : 2019/06/01
* @author : jihang
*/

#ifndef CENTRALLABEL_H
#define CENTRALLABEL_H

#include "cwidgets.h"
#include "fancybutton.h"
#include "ItemElement.h"

class CentralLabel : public QLabel {

	Q_OBJECT

public:
	explicit CentralLabel(QWidget *p = 0);

	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);
	virtual void dropEvent(QDropEvent*);

private:
	//������������һЩԤ������ֹ��������
	int mousePressPreprocess(QPoint);

private:
	//��ӹ�������ť
	void addToolBar();

private slots:
	void slotOpen();
	void slotUndo();
	void slotRedo();
	void slotGrid();
	void slotDelete();
	void slotGo();
signals:
	//��ui������Ϣ
	void signalSendMessage(QString);

private:
	//ģ���б�
	QMap<QString, ItemElement*> modelMap;
	//��ѡ��ģ��
	ItemElement *activeModel;

	//�϶�ʱ�������ʼ����
	QPoint moveStartPoint;
};

#endif // CENTRALLABEL_H