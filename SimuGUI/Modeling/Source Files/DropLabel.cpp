
/*
* @date : 2019/01/15
* @author : jihang
*/

#include "DropLabel.h"

DropLabel::DropLabel(QWidget *p) : QLabel(p) {

	setStyleSheet("border: 5px solid gray;"
		"background-color: white;border-radius: 10px;");
	setAcceptDrops(true);
	//TODO���Ӹ�㹦��

	//��ʼ������map
	std::set<int> CNameSet;
	CNameSet.insert(1);
	namingMap.insert(CTYPE, CNameSet);
	std::set<int> MatlabNameSet;
	MatlabNameSet.insert(1);
	namingMap.insert(MATLABTYPE, MatlabNameSet);
	std::set<int> AdamsNameSet;
	AdamsNameSet.insert(1);
	namingMap.insert(ADAMSTYPE, AdamsNameSet);

	//�ޱ�ѡ��ģ��
	activeModel = NULL;

	//bus
	bus = new BusElement(this);
	bus->setGeometry(200, 300, 1, 1);
	bus->show();

	//��ӹ�����
	addToolBar();
}

void DropLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void DropLabel::dropEvent(QDropEvent *e) {

	//drop��ɺ�ͳһ��Ϊ��ͷ
	setCursor(Qt::ArrowCursor);

	if (DRAG_COPY == e->mimeData()->text()) {

		//��ȡ��������
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

		//Ĭ��ѡ�У���Table��ҲĬ�ϣ������ǹ����߼�
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
			//�ָ�bus��ɫ
			bus->effectOff();
			//��¼�����㣬��������ʱ��̬�仯
			double part = e->pos().x() - bus->geometry().x();
			double total = bus->width();
			double ratio = part / total;
			connectInfo info = connectMap.value(modelName);
			info.busRatio = ratio;
			connectMap.insert(modelName, info);
			update();
			//TODO�����ӵ��ǿ�����ټ�Ч����ֱ�Ӵ���endpoint
		}
		else {
			connectMap.remove(modelName);
			update();
		}
	}
}

void DropLabel::mousePressEvent(QMouseEvent *event) {

	//���
	if (event->button() & Qt::LeftButton) {
		QPoint point = event->localPos().toPoint();

		int isInItem = mousePressPreprocess(point);

		if (1 == isInItem) {
			//����ģ�ͣ�׼����ʼģ���϶�
			moveStartPoint = event->pos();

			QDrag *dg = new QDrag(activeModel);
			QMimeData *md = new QMimeData;
			md->setText(DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
		}
		else if (0 == isInItem) {
			//δ����ģ��
			activeModel = NULL;
			emit signalModelChange(NULL);
			//���鿴�Ƿ����bus
			QRect rect = bus->geometry();
			if (rect.contains(point)) {
				//�����bus�ڣ����Ȳ鿴�Ƿ����bus������
				rect.setX(rect.x() + rect.width() - STRETCHWIDTH);
				rect.setWidth(STRETCHWIDTH);
				if (rect.contains(point)) {
					//��Ҫ����
					setCursor(Qt::SizeHorCursor);
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_STRETCH);
					dg->setMimeData(md);
					dg->exec();
				}
				else {
					//��Ҫƽ��
					moveStartPoint = event->pos();
					QDrag *dg = new QDrag(bus);
					QMimeData *md = new QMimeData;
					md->setText(DRAG_MOVE);
					dg->setMimeData(md);
					dg->exec();
				}
			}
		}
		//Ĭ���������Ϊ����2��������ê�㣬ֱ�ӿ�ʼ����
	}
}

/*
 * �ú���Ϊ����������ʱ����ǰ������
 * ��Ҫ�ж��Ƿ�㵽��ģ�ͻ�ǰѡ��ģ�͵�ê��
 * ����ֵ0������δ�����κ�ģ��
 * ����ֵ1���������ģ��
 * ����ֵ2��������е�ǰѡ��ģ�͵�ê��
 */
int DropLabel::mousePressPreprocess(QPoint pressPoint) {

	//���ȵ�ǰģ��
	if (activeModel) {
		//��ǰģ��������ê��
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
			//������ê��
			return 2;
		}
		//�жϵ�ǰģ����������
		QRect rect(
			activeModel->geometry().x() + FRAMESHIFTX,
			activeModel->geometry().y() + FRAMESHIFTY,
			WIDTH,
			WIDTH + EDGE * 2);
		if (rect.contains(pressPoint)) {
			//�����˵�ǰģ��
			return 1;
		}
		else {
			//�е�ǰģ�ͣ���û����
			activeModel->inactive();
		}
	}

	//��������ģ�Ͳ��жϣ�����������active��δ��active��������ᵽ��
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
			//������ĳ��ģ�ͣ�������activeModel��
			return 1;
		}
	}
	//δ����ģ��
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
			//��һ����ʾ�����ӵĽ���Ч��
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
	//�ƶ�����
	int moveX = dragPoint.x() - moveStartPoint.x();
	int moveY = dragPoint.y() - moveStartPoint.y();

	if (activeModel) {
		//ģ�͸�����
		QRect rect(moveX + activeModel->geometry().x(),
			moveY + activeModel->geometry().y(),
			activeModel->size().width(), activeModel->size().height());
		activeModel->setGeometry(rect);
		//�߸����ܣ���Ե�������㣬ֻ��һ��
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
		//bus������
		QRect rect(moveX + bus->geometry().x(),
			moveY + bus->geometry().y(),
			bus->size().width(), bus->size().height());
		bus->setGeometry(rect);
		//�߸����ܣ���Ե������յ㣬ÿһ��
		QMap<QString, connectInfo>::iterator iter;
		for (iter = connectMap.begin(); iter != connectMap.end(); ++iter) {
			connectInfo info = *iter;
			info.end.setX(info.end.x() + moveX);
			info.end.setY(info.end.y() + moveY);
			connectMap.insert(iter.key(), info);
		}
		update();
	}
	//��̬�ƶ���ʼ��
	moveStartPoint = dragPoint;
}

void DropLabel::mouseDragStretch(QPoint dragPoint) {
	//��¼bus���
	int x0 = bus->geometry().x();
	int y0 = bus->geometry().y();
	//bus����
	int distance = dragPoint.x() - x0;
	if (distance > 250) {
		bus->setFixedWidth(distance);
		bus->adjustStretchZone();
		//�߸������죬��Ե������յ㣬ÿһ��
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
	//TODO��ɾ���������б����߼���Ҫ���б�����ȫ�ж�
	//����ֻ������ɾ����ʾitem�����߼���
	/*
	QList<modelInfo>::iterator itor;
	for (itor = modelList.begin(); itor != modelList.end(); itor++) {
		if (itor->name == name) {
			//�ж��Ƿ�ѡ��
			if (selectedLabel == itor->label) {
				selectedLabel = NULL;
			}
			//ɾ��label
			QLabel *label = itor->label;
			delete label;
			//�����������
			namingMap[itor->type].insert(itor->count);
			//�б�ɾ����
			modelList.erase(itor++);
			return;
		}
	}
	emit signalSendMessage("delete model name not found");
	*/
}

void DropLabel::slotModelChange(QString name) {
	//NULL�����Ϊ��dropLabel��model��null��null��interface��null
	//��������Ҳ���ܵ���,��Ϣ����,����ֱ�ӷ���
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
	//����dialog����ʾ���

}