#pragma once

/*
* @date : 2018/01/14
* @author : jihang
*/

#ifndef DROPLABEL_H
#define DROPLABEL_H

//QSet不好用
#include <set>

#include "cwidgets.h"
#include "fancybutton.h"

constexpr auto CTYPE = "C";
constexpr auto MATLABTYPE = "Matlab";
constexpr auto ADAMSTYPE = "Adams";

constexpr auto DRAG_MOVE = "Drag_Move";
constexpr auto DRAG_COPY = "Drag_Copy";

struct modelInfo {
	//初始化name = type + "Model_" + count
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
	//拖动增加模型，前端接收
	void signalAddModel(QString, QString);
public slots:
	//接收前端删除模型
	void slotDeleteModel(QString);
signals:
	//模型选择改变，前端接收
	void signalModelChange(QString);
public slots:
	//接收前端模型选择改变
	void slotModelChange(QString);
signals:
	//像前端发送消息
	void signalSendMessage(QString);

private:
	//私有命名引擎
	modelInfo getModel(QString);
	QMap<QString, std::set<int>> namingMap;

	//模型列表
	QList<modelInfo> modelList;

	//被选中模型
	QLabel *selectedLabel;

	//总线
	QLabel *principalLabel;

	//拖动时的相对坐标
	int hOffset, vOffset;
};
#endif // DROPLABEL_H