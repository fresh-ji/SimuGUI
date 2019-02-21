#pragma once

/*
* @date : 2018/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet������
#include <set>

#include "cwidgets.h"
#include "fancybutton.h"

constexpr auto CTYPE = "C";
constexpr auto MATLABTYPE = "Matlab";
constexpr auto ADAMSTYPE = "Adams";

constexpr auto DRAG_MOVE = "Drag_Move";
constexpr auto DRAG_COPY = "Drag_Copy";

struct modelInfo {
	//��ʼ��name = type + "Model_" + count
	QString name;
	QString type;
	int count;
	QLabel *label;
};

class DropLabel : public QLabel {

	Q_OBJECT

public:
	explicit DropLabel(QWidget *p = 0);

	//copy
	virtual void dragEnterEvent(QDragEnterEvent*);
	virtual void dropEvent(QDropEvent*);

	//move
	virtual void mousePressEvent(QMouseEvent*);
	virtual void dragMoveEvent(QDragMoveEvent*);

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
	modelInfo getModel(QString);
	QMap<QString, std::set<int>> namingMap;

	//ģ���б�
	QList<modelInfo> modelList;

	//��ѡ��ģ��
	QLabel *selectedLabel;

	//����
	QLabel *principalLabel;

	//�϶�ʱ���������
	int hOffset, vOffset;
};
#endif // DROPLABEL_H