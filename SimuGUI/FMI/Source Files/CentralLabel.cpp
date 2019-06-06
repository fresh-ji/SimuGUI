
/*
* @date : 2019/06/01
* @author : jihang
*/

#include "CentralLabel.h"

CentralLabel::CentralLabel(QWidget *p) : QLabel(p) {

	setStyleSheet("border: 5px solid gray;"
		"background-color: white;border-radius: 10px;");
	setAcceptDrops(true);
	//TODO���Ӹ�㹦��

	//�ޱ�ѡ��ģ��
	activeModel = NULL;

	//��ʼ�ļ�ѡ��·��
	openPath = QDir::currentPath();

	//fmi
	FMIsupport = new FMISupport();

	//��ӹ�����
	addToolBar();
}

void CentralLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void CentralLabel::mousePressEvent(QMouseEvent *event) {
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
		}
	}
}

/*
 * �ú���Ϊ����������ʱ����ǰ������
 * ��Ҫ�ж��Ƿ�㵽��ģ�ͻ�ǰѡ��ģ�͵�ê��
 * ����ֵ0������δ�����κ�ģ��
 * ����ֵ1���������ģ��
 */
int CentralLabel::mousePressPreprocess(QPoint pressPoint) {
	//���ȵ�ǰģ��
	if (activeModel) {
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
	for (ItemElement* ie : modelMap.values()) {
		QRect rect(
			ie->geometry().x() + FRAMESHIFTX,
			ie->geometry().y() + FRAMESHIFTY,
			WIDTH,
			WIDTH + EDGE * 2);
		if (rect.contains(pressPoint)) {
			activeModel = ie;
			activeModel->active();
			activeModel->raise();
			//������ĳ��ģ�ͣ�������activeModel��
			return 1;
		}
	}
	//δ����ģ��
	return 0;
}

void CentralLabel::dragMoveEvent(QDragMoveEvent *event) {
	if (DRAG_MOVE == event->mimeData()->text()) {
		//�ƶ�����
		int moveX = event->pos().x() - moveStartPoint.x();
		int moveY = event->pos().y() - moveStartPoint.y();
		//ģ�͸�����
		QRect rect(moveX + activeModel->geometry().x(),
			moveY + activeModel->geometry().y(),
			activeModel->size().width(), activeModel->size().height());
		activeModel->setGeometry(rect);
		//��̬�ƶ���ʼ��
		moveStartPoint = event->pos();
	}
}

void CentralLabel::dropEvent(QDropEvent *event) {

}

void CentralLabel::addToolBar() {
	QToolButton *openButton = new QToolButton(this);
	openButton->setGeometry(20, 5, 50, 50);
	openButton->setCursor(Qt::PointingHandCursor);
	openButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	openButton->setIcon(QIcon("./Icon/function/open"));
	connect(openButton, SIGNAL(clicked()), this, SLOT(slotOpen()));

	QToolButton *undoButton = new QToolButton(this);
	undoButton->setGeometry(70, 5, 50, 50);
	undoButton->setCursor(Qt::PointingHandCursor);
	undoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	undoButton->setIcon(QIcon("./Icon/function/undo"));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(slotUndo()));

	QToolButton *redoButton = new QToolButton(this);
	redoButton->setGeometry(120, 5, 50, 50);
	redoButton->setCursor(Qt::PointingHandCursor);
	redoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	redoButton->setIcon(QIcon("./Icon/function/redo"));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(slotRedo()));

	QToolButton *gridButton = new QToolButton(this);
	gridButton->setGeometry(170, 5, 50, 50);
	gridButton->setCursor(Qt::PointingHandCursor);
	gridButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gridButton->setIcon(QIcon("./Icon/function/grid"));
	connect(gridButton, SIGNAL(clicked()), this, SLOT(slotGrid()));

	QToolButton *deleteButton = new QToolButton(this);
	deleteButton->setGeometry(220, 5, 50, 50);
	deleteButton->setCursor(Qt::PointingHandCursor);
	deleteButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	deleteButton->setIcon(QIcon("./Icon/function/delete"));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));

	QToolButton *gobutton = new QToolButton(this);
	gobutton->setGeometry(270, 5, 50, 50);
	gobutton->setCursor(Qt::PointingHandCursor);
	gobutton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gobutton->setIcon(QIcon("./Icon/function/go"));
	connect(gobutton, SIGNAL(clicked()), this, SLOT(slotGo()));
}

void CentralLabel::slotOpen() {
	QString title = "select FMU file";
	QString filter = "FMU File(*.fmu)";

	QString modelPath = QFileDialog::getOpenFileName(this, title, openPath, filter);
	openPath = modelPath;

	if (!modelPath.isEmpty()) {
		QString name = modelPath.section('/', -1);

		if (modelMap.contains(modelPath)) {
			emit signalSendMessage("Model Already Imported");
			return;
		}

		//���ǲ�����ȷ��FMU
		FMUInfo info = FMIsupport->loadFMU(modelPath);
		if (!info.isSuccess) {
			signalSendMessage(info.message);
			return;
		}
		modelRepo.insert(modelPath, info);

		QPixmap *pixmap = new QPixmap("./Icon/simutool/fmi");
		ItemElement *item = new ItemElement(pixmap, name, "FMU", 0, this);
		//TODO����һ����λ��
		item->setGeometry(100, 100, 1, 1);
		item->show();

		if (activeModel) {
			activeModel->inactive();
		}
		activeModel = item;

		modelMap.insert(modelPath, item);
	}
}

void CentralLabel::slotUndo() {

}

void CentralLabel::slotRedo() {

}

void CentralLabel::slotGrid() {

}

void CentralLabel::slotDelete() {

}

void CentralLabel::slotGo() {

}