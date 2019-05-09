
/*
* @date : 2019/01/15
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
	activeModel = NULL;

	//bus
	bus = new BusElement(this);
	bus->setGeometry(200, 300, 1, 1);
	bus->show();
}

void DropLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void DropLabel::dropEvent(QDropEvent *e) {

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
		item->setGeometry(e->pos().x() - FRAMESHIFTX,
			e->pos().y() - FRAMESHIFTY, 1, 1);
		item->show();

		//默认选中，在Table中也默认，但不是关联逻辑
		if (activeModel) {
			activeModel->inactive();
		}
		activeModel = item;

		modelList.append(item);
		emit signalAddModel(name, type);
	}
}

//TODO:这个函数太长了
void DropLabel::mousePressEvent(QMouseEvent *event) {

	//左键
	if (event->button() & Qt::LeftButton) {
		QPoint point = event->localPos().toPoint();
		bool blank = true;

		//优先当前
		if (activeModel) {
			//先判断是否连线
			QPoint offset;
			offset.setX(point.x() - activeModel->geometry().x());
			offset.setY(point.y() - activeModel->geometry().y());
			ItemElement::Anchor anchor = activeModel->getAvaiableAnchor(offset);
			if (anchor != ItemElement::Anchor::NONE) {

				//setCursor(Qt::SizeAllCursor);

				//QDrag *dg = new QDrag(this);
				//QMimeData *md = new QMimeData;
				//md->setText(DRAG_CONNECT);
				//md->setProperty("point", point);

				//md->setProperty("anchor", anchor);
				//dg->setMimeData(md);
				//dg->exec();
				//return;
			}
			//其他点
			QRect rect(
				activeModel->geometry().x() + FRAMESHIFTX,
				activeModel->geometry().y() + FRAMESHIFTY,
				WIDTH,
				WIDTH + EDGE * 2);
			if (rect.contains(point)) {
				blank = false;
			}
			else {
				activeModel->inactive();
			}
		}

		//其次遍历，包括本身无active和未点active两种情况
		if (blank) {
			QList<ItemElement*>::iterator itor;
			for (itor = modelList.begin(); itor != modelList.end(); itor++) {
				QRect rect(
					(*itor)->geometry().x() + FRAMESHIFTX,
					(*itor)->geometry().y() + FRAMESHIFTY,
					WIDTH,
					WIDTH + EDGE * 2);
				if (rect.contains(point)) {
					activeModel = (*itor);
					activeModel->active();
					activeModel->raise();
					emit signalModelChange((*itor)->getName());
					blank = false;
					break;
				}
			}
		}

		//有item
		if (!blank) {
			hOffset = event->pos().x() - activeModel->geometry().x();
			vOffset = event->pos().y() - activeModel->geometry().y();

			QDrag *dg = new QDrag(activeModel);
			QMimeData *md = new QMimeData;
			md->setText(DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
		}
		else {
			activeModel = NULL;
			emit signalModelChange(NULL);
			//还有可能是bus
			QRect rect = bus->geometry();
			if (rect.contains(point)) {
				hOffset = event->pos().x() - rect.x();
				vOffset = event->pos().y() - rect.y();

				rect.setX(rect.x() + rect.width() - STRETCHWIDTH);
				rect.setWidth(STRETCHWIDTH);

				if (rect.contains(point)) {
					setCursor(Qt::SizeHorCursor);
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_STRETCH);
					dg->setMimeData(md);
					dg->exec();
				}
				else {
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_MOVE);
					dg->setMimeData(md);
					dg->exec();
				}
			}
		}
	}
}

void DropLabel::dragMoveEvent(QDragMoveEvent *event) {
	if (DRAG_MOVE == event->mimeData()->text()) {
		int hPos = event->pos().x() - hOffset;
		int vPos = event->pos().y() - vOffset;
		if (activeModel) {
			QRect rect(hPos, vPos, activeModel->size().width(), activeModel->size().height());
			activeModel->setGeometry(rect);
		}
		else {
			QRect rect(hPos, vPos, bus->size().width(), bus->size().height());
			bus->setGeometry(rect);
		}
	}
	else if (DRAG_STRETCH == event->mimeData()->text()) {
		int offset = event->pos().x() - bus->geometry().x();
		if (offset > 200) {
			bus->setFixedWidth(offset);
			bus->adjustAnchor();
		}
	}
	else if (DRAG_CONNECT == event->mimeData()->text()) {
		//startPoint = qvariant_cast<QPoint>(event->mimeData()->property("point"));
		//endPoint = event->pos();
		//update();
	}
}

void DropLabel::paintEvent(QPaintEvent *event) {
	/*
	QLine line;
	QPoint p1(startPoint);
	QPoint p2(endPoint);
	line.setPoints(p1, p2);

	QPainter painter(this);
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(3);
	painter.setPen(pen);
	painter.drawLine(line);
	*/
}

void DropLabel::slotDeleteModel(QString name) {
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