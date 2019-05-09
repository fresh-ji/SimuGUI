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

	//
	virtual void paintEvent(QPaintEvent*);

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

private:
	//˽����������
	QMap<QString, std::set<int>> namingMap;

	//ģ���б�
	QList<ItemElement*> modelList;
	//item
	ItemElement *activeModel;

	//bus
	BusElement *bus;

	//�϶�ʱ���������
	int hOffset, vOffset;

	QPoint startPoint;
	QPoint endPoint;
};

#endif // DROPLABEL_H