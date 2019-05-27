
/*
* @date : 2019/01/15
* @author : jihang
*/

#include "DropLabel.h"

DropLabel::DropLabel(QWidget *p) : QLabel(p) {

	setStyleSheet("border: 5px solid gray;"
		"background-color: white;border-radius: 10px;");
	setAcceptDrops(true);
	//TODO：加格点功能

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
	activeModel = NULL;

	//bus
	bus = new BusElement(this);
	bus->setGeometry(200, 300, 1, 1);
	bus->show();

	//添加功能栏
	addToolBar();
}

void DropLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void DropLabel::dropEvent(QDropEvent *e) {

	//drop完成后统一变为箭头
	setCursor(Qt::ArrowCursor);

	if (DRAG_COPY == e->mimeData()->text()) {

		//获取四项资料
		QPixmap pixmap = QPixmap::fromImage(e->mimeData()->imageData().value<QImage>());

		QString type = e->mimeData()->objectName();

		int next = *namingMap[type].begin();
		namingMap[type].erase(next);
		if (namingMap[type].empty()) {
			namingMap[type].insert(next + 1);
		}
		int count = next;

		QString name = type + "Model_" + QString::number(next);

		ItemElement *item = new ItemElement(&pixmap, name, type, count, this);
		item->setGeometry(e->pos().x() - FRAMESHIFTX, e->pos().y() - FRAMESHIFTY, 1, 1);
		item->show();

		//默认选中，在Table中也默认，但不是关联逻辑
		if (activeModel) {
			activeModel->inactive();
		}
		activeModel = item;

		modelList.append(item);
		emit signalAddModel(name, type);
	}
	else if ((DRAG_CONNECT == e->mimeData()->text())) {
		QString modelName = e->mimeData()->objectName();
		if (bus->geometry().contains(e->pos())) {
			//恢复bus颜色
			bus->effectOff();
			//记录比例点，用于拉伸时动态变化
			double part = e->pos().x() - bus->geometry().x();
			double total = bus->width();
			double ratio = part / total;
			connectInfo info = connectMap.value(modelName);
			info.busRatio = ratio;
			connectMap.insert(modelName, info);
			update();
			//TODO：连接点那块可以再加效果，直接处理endpoint
		}
		else {
			connectMap.remove(modelName);
			update();
		}
	}
}

void DropLabel::mousePressEvent(QMouseEvent *event) {

	//左键
	if (event->button() & Qt::LeftButton) {
		QPoint point = event->localPos().toPoint();

		int isInItem = mousePressPreprocess(point);

		if (1 == isInItem) {
			//点中模型，准备开始模型拖动
			moveStartPoint = event->pos();

			QDrag *dg = new QDrag(activeModel);
			QMimeData *md = new QMimeData;
			md->setText(DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
		}
		else if (0 == isInItem) {
			//未点中模型
			activeModel = NULL;
			emit signalModelChange(NULL);
			//最后查看是否点中bus
			QRect rect = bus->geometry();
			if (rect.contains(point)) {
				//如果在bus内，优先查看是否点中bus拉伸区
				rect.setX(rect.x() + rect.width() - STRETCHWIDTH);
				rect.setWidth(STRETCHWIDTH);
				if (rect.contains(point)) {
					//将要拉伸
					setCursor(Qt::SizeHorCursor);
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_STRETCH);
					dg->setMimeData(md);
					dg->exec();
				}
				else {
					//将要平拖
					moveStartPoint = event->pos();
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_MOVE);
					dg->setMimeData(md);
					dg->exec();
				}
			}
		}
		//默认其他情况为返回2，即点中锚点，直接开始连线
	}
}

/*
 * 该函数为鼠标左键按下时的提前处理函数
 * 主要判断是否点到了模型或当前选中模型的锚点
 * 返回值0，代表未点中任何模型
 * 返回值1，代表点中模型
 * 返回值2，代表点中当前选中模型的锚点
 */
int DropLabel::mousePressPreprocess(QPoint pressPoint) {

	//优先当前模型
	if (activeModel) {
		//当前模型中优先锚点
		QPoint offset;
		offset.setX(pressPoint.x() - activeModel->geometry().x());
		offset.setY(pressPoint.y() - activeModel->geometry().y());
		ItemElement::Anchor anchor = activeModel->whichAnchor(offset);
		if (anchor != ItemElement::Anchor::NONE) {

			connectInfo info;
			QPoint point, shift;
			shift = activeModel->getAnchorShift(anchor);
			point.setX(activeModel->geometry().x() + shift.x());
			point.setY(activeModel->geometry().y() + shift.y());
			info.start = point;
			info.anchor = anchor;
			QString modelName = activeModel->getName();
			connectMap.insert(modelName, info);

			QDrag *dg = new QDrag(this);
			QMimeData *md = new QMimeData;
			md->setText(DRAG_CONNECT);
			md->setObjectName(modelName);
			dg->setMimeData(md);
			dg->exec();
			//点中了锚点
			return 2;
		}
		//判断当前模型中其他点
		QRect rect(
			activeModel->geometry().x() + FRAMESHIFTX,
			activeModel->geometry().y() + FRAMESHIFTY,
			WIDTH,
			WIDTH + EDGE * 2);
		if (rect.contains(pressPoint)) {
			//点中了当前模型
			return 1;
		}
		else {
			//有当前模型，但没点中
			activeModel->inactive();
		}
	}

	//遍历其他模型并判断，包括本身无active和未点active两种情况会到这
	QList<ItemElement*>::iterator itor;
	for (itor = modelList.begin(); itor != modelList.end(); itor++) {
		QRect rect(
			(*itor)->geometry().x() + FRAMESHIFTX,
			(*itor)->geometry().y() + FRAMESHIFTY,
			WIDTH,
			WIDTH + EDGE * 2);
		if (rect.contains(pressPoint)) {
			activeModel = (*itor);
			activeModel->active();
			activeModel->raise();
			emit signalModelChange((*itor)->getName());
			//点中了某个模型，存在了activeModel里
			return 1;
		}
	}
	//未点中模型
	return 0;
}

void DropLabel::dragMoveEvent(QDragMoveEvent *event) {
	if (DRAG_MOVE == event->mimeData()->text()) {
		mouseDragMove(event->pos());
	}
	else if (DRAG_STRETCH == event->mimeData()->text()) {
		mouseDragStretch(event->pos());
	}
	else if (DRAG_CONNECT == event->mimeData()->text()) {
		QString modelName = event->mimeData()->objectName();
		if (bus->geometry().contains(event->pos())) {
			//有一个提示可连接的进入效果
			bus->effectOn();
		}
		else {
			bus->effectOff();
		}
		connectInfo info = connectMap.value(modelName);
		info.end = event->pos();
		connectMap.insert(modelName, info);
		update();
	}
}

void DropLabel::mouseDragMove(QPoint dragPoint) {
	//移动距离
	int moveX = dragPoint.x() - moveStartPoint.x();
	int moveY = dragPoint.y() - moveStartPoint.y();

	if (activeModel) {
		//模型跟着跑
		QRect rect(moveX + activeModel->geometry().x(),
			moveY + activeModel->geometry().y(),
			activeModel->size().width(), activeModel->size().height());
		activeModel->setGeometry(rect);
		//线跟着跑，针对的是线起点，只有一根
		QString modelName = activeModel->getName();
		if (connectMap.contains(modelName)) {
			connectInfo info = connectMap.value(modelName);
			info.start.setX(info.start.x() + moveX);
			info.start.setY(info.start.y() + moveY);
			connectMap.insert(modelName, info);
			update();
		}
	}
	else {
		//bus跟着跑
		QRect rect(moveX + bus->geometry().x(),
			moveY + bus->geometry().y(),
			bus->size().width(), bus->size().height());
		bus->setGeometry(rect);
		//线跟着跑，针对的是线终点，每一根
		QMap<QString, connectInfo>::iterator iter;
		for (iter = connectMap.begin(); iter != connectMap.end(); ++iter) {
			connectInfo info = *iter;
			info.end.setX(info.end.x() + moveX);
			info.end.setY(info.end.y() + moveY);
			connectMap.insert(iter.key(), info);
		}
		update();
	}
	//动态移动起始点
	moveStartPoint = dragPoint;
}

void DropLabel::mouseDragStretch(QPoint dragPoint) {
	//记录bus起点
	int x0 = bus->geometry().x();
	int y0 = bus->geometry().y();
	//bus拉伸
	int distance = dragPoint.x() - x0;
	if (distance > 250) {
		bus->setFixedWidth(distance);
		bus->adjustStretchZone();
		//线跟着拉伸，针对的是线终点，每一根
		QMap<QString, connectInfo>::iterator iter;
		for (iter = connectMap.begin(); iter != connectMap.end(); ++iter) {
			connectInfo info = *iter;
			info.end.setX(x0 + (distance * info.busRatio));
			connectMap.insert(iter.key(), info);
		}
		update();
	}
}

void DropLabel::paintEvent(QPaintEvent *event) {

	QPen pen;
	pen.setColor(QColor("#1890FF"));
	pen.setWidth(3);
	QPainter painter(this);
	painter.setPen(pen);

	QLine line;;
	QMap<QString, connectInfo>::iterator iter;
	for (iter = connectMap.begin(); iter != connectMap.end(); ++iter) {
		line.setPoints((*iter).start, (*iter).end);
		painter.drawLine(line);
	}

}

void DropLabel::slotDeleteModel(QString name) {
	//TODO：删除部分由列表发起，逻辑需要在列表里完全判断
	//这里只用无脑删除显示item和连线即可
	/*
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
	*/
}

void DropLabel::slotModelChange(QString name) {
	//NULL的情况为：dropLabel给model发null，null给interface的null
	//但是这里也接受到了,信息多余,所以直接返回
	if (name == NULL) {
		return;
	}
	if (activeModel) {
		activeModel->inactive();
	}
	QList<ItemElement*>::iterator itor;
	for (itor = modelList.begin(); itor != modelList.end(); itor++) {
		if ((*itor)->getName() == name) {
			activeModel = *itor;
			(*itor)->active();
			return;
		}
	}
	emit signalSendMessage("change model name not found");
}

void DropLabel::addToolBar() {
	QToolButton *undoButton = new QToolButton(this);
	undoButton->setGeometry(20, 5, 50, 50);
	undoButton->setCursor(Qt::PointingHandCursor);
	undoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	undoButton->setIcon(QIcon("./Icon/function/undo"));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(slotUndo()));

	QToolButton *redoButton = new QToolButton(this);
	redoButton->setGeometry(70, 5, 50, 50);
	redoButton->setCursor(Qt::PointingHandCursor);
	redoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	redoButton->setIcon(QIcon("./Icon/function/redo"));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(slotRedo()));

	QToolButton *gridButton = new QToolButton(this);
	gridButton->setGeometry(120, 5, 50, 50);
	gridButton->setCursor(Qt::PointingHandCursor);
	gridButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gridButton->setIcon(QIcon("./Icon/function/grid"));
	connect(gridButton, SIGNAL(clicked()), this, SLOT(slotGrid()));

	QToolButton *generateButton = new QToolButton(this);
	generateButton->setGeometry(170, 5, 50, 50);
	generateButton->setCursor(Qt::PointingHandCursor);
	generateButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	generateButton->setIcon(QIcon("./Icon/function/generate"));
	connect(generateButton, SIGNAL(clicked()), this, SLOT(slotGenerate()));
}

void DropLabel::slotUndo() {

}

void DropLabel::slotRedo() {

}

void DropLabel::slotGrid() {

}

void DropLabel::slotGenerate() {
	if (0 == modelList.size()) {
		return;
	}
	if (connectMap.size() != modelList.size()) {
		emit signalSendMessage("check model link!");
	}
	//弹出dialog，显示结果

}