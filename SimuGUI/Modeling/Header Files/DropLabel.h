#pragma once

/*
* @date : 2019/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet������
#include <set>

#include "cwidgets.h"
#include "fancybutton.h"
#include "BusElement.h"
#include "ItemElement.h"
#include "Regulation.h"

class DropLabel : public QLabel {

	Q_OBJECT

public:
	explicit DropLabel(QWidget *p = 0);

	//copy
	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void dropEvent(QDropEvent*);

	//move, stretch, connect
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);

	//repaint all connections
	virtual void paintEvent(QPaintEvent*);

private:
	//������������һЩԤ������ֹ��������
	int mousePressPreprocess(QPoint);
	//�϶����̷�������ֹ��������
	void mouseDragMove(QPoint);
	//bus������̷�������ֹ��������
	void mouseDragStretch(QPoint);
	//��ӹ�������ť
	void addToolBar();

signals:
	//�϶�����ģ�ͣ�ǰ�˽���
	void signalAddModel(QString, QString);
public slots:
	//����ǰ��ɾ��ģ��
	void slotDeleteModel(QString);
signals:
	//ģ��ѡ��ı䣬ǰ�˽���
	void signalModelChange(QString);
public slots:
	//����ǰ��ģ��ѡ��ı�
	void slotModelChange(QString);
signals:
	//��ǰ�˷�����Ϣ
	void signalSendMessage(QString);

private slots:
	void slotUndo();
	void slotRedo();
	void slotGrid();
	void slotCut();
	void slotGenerate();

private:
	//˽����������
	QMap<QString, std::set<int>> namingMap;

	//���ӹ���
	QMap<QString, connectInfo> connectMap;

	//ģ���б�
	QList<ItemElement*> modelList;
	//��ѡ��ģ��
	ItemElement *activeModel;
	//bus
	BusElement *bus;

	//�϶�ʱ�������ʼ����
	QPoint moveStartPoint;
};

#endif // DROPLABEL_H