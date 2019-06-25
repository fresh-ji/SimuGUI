#pragma once

/*
* @date : 2019/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet������
#include <set>

#include "GlobalConstant.h"

#include "fancybutton.h"

#include "BusElement.h"
#include "ItemElement.h"
#include "OverviewDialog.h"
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
	//�϶�����ģ�Ͳ�����
	void signalAddModel(QString, QString);
public slots:
	//����ɾ��ģ��
	void slotDeleteModel(QString);
signals:
	//ģ��ѡ��ı䲢����
	void signalModelChange(QString);
public slots:
	//����ģ��ѡ��ı�
	void slotModelChange(QString);
signals:
	QMap<QString, dataTypeInfo> signalDataTypeOverview();
signals:
	QMap<QString, interfaceInfo> signalInterfaceOverview();
signals:
	//��ui������Ϣ
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

	//�����Ի���
	OverviewDialog *dialog;

private:
	QString iconPath = ICON_PATH;
};

#endif // DROPLABEL_H