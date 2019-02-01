
/*
* @date : 2018/01/15
* @author : jihang
*/

#include "DropLabel.h"

DropLabel::DropLabel(QWidget *p) : QLabel(p) {

	//初始化命名map
	std::set<int> CNameSet;
	CNameSet.insert(1);
	namingMap.insert(CTYPE, CNameSet);
	std::set<int> MatlabNameSet;
	MatlabNameSet.insert(1);
	namingMap.insert(MATLABTYPE, MatlabNameSet);
	std::set<int> AdamsNameSet;
	AdamsNameSet.insert(1);
	namingMap.insert(ADAMSTYPE, AdamsNameSet);

	//无被选中模型
	selectedLabel = NULL;

	//直接画总线
	principalLabel = new QLabel(this);
	principalLabel->setStyleSheet("border:2px solid black;""background-color:orange;""font:16pt");
	principalLabel->setText("CoSimulation Bus");
	principalLabel->setGeometry(100, 200, 500, 40);
	principalLabel->setAlignment(Qt::AlignCenter);
	principalLabel->show();
}

void DropLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void DropLabel::dropEvent(QDropEvent *e) {

	if (DRAG_COPY == e->mimeData()->text()) {

		modelInfo model = getModel(e->mimeData()->objectName());

		QLabel *label = new QLabel(this);
		label->setStyleSheet("border:3px solid black;");
		label->setGeometry(e->pos().x(), e->pos().y(), 60, 60);

		//默认选中，在Table中也默认，但不是关联逻辑
		if (selectedLabel && selectedLabel != principalLabel) {
			selectedLabel->setStyleSheet("border:1px solid gray;");
		}
		selectedLabel = label;

		QPixmap pixmap = QPixmap::fromImage(e->mimeData()->imageData().value<QImage>());
		pixmap.scaled(label->size(), Qt::KeepAspectRatio);
		label->setScaledContents(true);
		label->setPixmap(pixmap);
		label->show();

		model.label = label;
		modelList.append(model);
		emit signalAddModel(model.name, model.type);
	}
}

void DropLabel::mousePressEvent(QMouseEvent *event) {

	if (event->button() & Qt::LeftButton) {

		QPoint point = event->localPos().toPoint();
		QList<modelInfo>::iterator itor;

		//取消选中
		if (selectedLabel && selectedLabel != principalLabel) {
			selectedLabel->setStyleSheet("border:1px solid gray;");
		}

		if (principalLabel->geometry().contains(point)) {
			selectedLabel = principalLabel;
			emit signalModelChange(NULL);
			QDrag *dg = new QDrag(selectedLabel);
			QMimeData *md = new QMimeData;
			md->setText(DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
			return;
		}

		//遍历
		for (itor = modelList.begin(); itor != modelList.end(); itor++) {
			if (itor->label->geometry().contains(point)) {
				break;
			}
		}
		if (itor != modelList.end()) {
			selectedLabel = itor->label;
			selectedLabel->setStyleSheet("border:3px solid black;");
			emit signalModelChange(itor->name);

			QDrag *dg = new QDrag(selectedLabel);
			QMimeData *md = new QMimeData;
			//md->setProperty("label", QVariant::fromValue(selectedLabel));
			md->setText(DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
		}
		else {
			//点空白取消选择
			selectedLabel = NULL;
			emit signalModelChange(NULL);
		}
	}
}

void DropLabel::dragMoveEvent(QDragMoveEvent *event) {

	if (DRAG_MOVE == event->mimeData()->text()) {

		//之前的传label方式
		//QLabel *label = event->mimeData()->property("label").value<QLabel*>();

		QPoint point = event->pos();
		QRect rect(event->pos(), selectedLabel->size());
		selectedLabel->setGeometry(rect);
	}
}

void DropLabel::slotDeleteModel(QString name) {
	QList<modelInfo>::iterator itor;
	for (itor = modelList.begin(); itor != modelList.end(); itor++) {
		if (itor->name == name) {
			//判断是否被选中
			if (selectedLabel == itor->label) {
				selectedLabel = NULL;
			}
			//删除label
			QLabel *label = itor->label;
			delete label;
			//插入可用数字
			namingMap[itor->type].insert(itor->count);
			//列表删除项
			modelList.erase(itor++);
			return;
		}
	}
	emit signalSendMessage("delete model name not found");
}

void DropLabel::slotModelChange(QString name) {
	//NULL的情况为：dropLabel给model发null，null给interface的null
	//但是这里也接受到了,信息多余,所以直接返回
	if (name == NULL) {
		return;
	}
	if (selectedLabel) {
		selectedLabel->setStyleSheet("border:1px solid gray;");
	}
	QList<modelInfo>::iterator itor;
	for (itor = modelList.begin(); itor != modelList.end(); itor++) {
		if (itor->name == name) {
			selectedLabel = itor->label;
			selectedLabel->setStyleSheet("border:3px solid black;");
			return;
		}
	}
	emit signalSendMessage("change model name not found");
}

modelInfo DropLabel::getModel(QString type) {
	modelInfo model;
	model.type = type;
	int next = *namingMap[type].begin();
	namingMap[type].erase(next);
	if (namingMap[type].empty()) {
		namingMap[type].insert(next + 1);
	}
	model.count = next;
	model.name = type + "Model_" + QString::number(next);
	return model;
}