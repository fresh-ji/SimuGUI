
/*
* @date : 2018/01/15
* @author : jihang
*/

#include "DropLabel.h"

DropLabel::DropLabel(QWidget *p) : QLabel(p) {

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
	selectedLabel = NULL;

	//ֱ�ӻ�����
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

		//Ĭ��ѡ�У���Table��ҲĬ�ϣ������ǹ����߼�
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

		//ȡ��ѡ��
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

		//����
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
			//��հ�ȡ��ѡ��
			selectedLabel = NULL;
			emit signalModelChange(NULL);
		}
	}
}

void DropLabel::dragMoveEvent(QDragMoveEvent *event) {

	if (DRAG_MOVE == event->mimeData()->text()) {

		//֮ǰ�Ĵ�label��ʽ
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
}

void DropLabel::slotModelChange(QString name) {
	//NULL�����Ϊ��dropLabel��model��null��null��interface��null
	//��������Ҳ���ܵ���,��Ϣ����,����ֱ�ӷ���
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