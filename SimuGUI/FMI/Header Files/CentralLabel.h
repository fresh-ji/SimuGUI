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
#include "FMISupport.h"

class CentralLabel : public QLabel {

	Q_OBJECT

public:
	explicit CentralLabel(QWidget *p = 0);

	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);
	virtual void dropEvent(QDropEvent*);

private:
	//������������һЩԤ��������ֹ��������
	int mousePressPreprocess(QPoint);

private:
	//���ӹ�������ť
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
signals:
	//��DetailStack����Ҫ��ʾ��
	void signalModelDetail();

private:
	//ģ�Ϳ�
	QMap<QString, FMUInfo> modelRepo;

	//ģ���б�
	QMap<QString, ItemElement*> modelMap;
	//��ѡ��ģ��
	ItemElement *activeModel;

	//�϶�ʱ�������ʼ����
	QPoint moveStartPoint;

	//ѡ���ļ�ʱ�ĳ�ʼ·��
	QString openPath;

	//��fmu��
	FMISupport *FMIsupport;
};

#endif // CENTRALLABEL_H