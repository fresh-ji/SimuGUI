
/*
* @date : 2018/01/07
* @author : jihang
*/

#include "ToolDragStack.h"

#include "DropLabel.h"

ToolDragStack::ToolDragStack(QWidget *p) : MiniStack(p) {

	m_cLabel = new QLabel();
	putImage("cm", m_cLabel);
	//m_cLabel->setStyleSheet("background-color:gray");
	m_cLabel->setCursor(QCursor(Qt::PointingHandCursor));
	m_cLabel->setFixedHeight(20);
	m_cLabel->setFixedWidth(175);

	m_matlabLabel = new QLabel();
	putImage("matlabm", m_matlabLabel);
	//m_matlabLabel->setStyleSheet("background-color:black");
	m_matlabLabel->setCursor(QCursor(Qt::PointingHandCursor));
	m_matlabLabel->setFixedHeight(20);
	m_matlabLabel->setFixedWidth(175);

	m_adamsLabel = new QLabel();
	putImage("adamsm", m_adamsLabel);
	//m_adamsLabel->setStyleSheet("background-color:gray");
	m_adamsLabel->setCursor(QCursor(Qt::PointingHandCursor));
	m_adamsLabel->setFixedHeight(20);
	m_adamsLabel->setFixedWidth(175);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_cLabel);
	layout->addWidget(m_matlabLabel);
	layout->addWidget(m_adamsLabel);

	QWidget *toolWidget = new QWidget();
	toolWidget->setLayout(layout);

	this->setFixedWidth(200);

	addTab(tr("tool"), toolWidget);

	setAcceptDrops(false);
}

void ToolDragStack::mousePressEvent(QMouseEvent *event) {
	if (event->button() & Qt::LeftButton) {

		QPoint point = event->localPos().toPoint();
		//TODO:magic number，默认的stack头高40
		point.setY(point.y() - 40);

		QString modelType;
		QPixmap *pixmap;
		if (m_cLabel->geometry().contains(point)) {
			m_dragLabel = m_cLabel;
			modelType = CTYPE;
			pixmap = new QPixmap("./Icon/simutool/c");
		}
		else if (m_matlabLabel->geometry().contains(point)) {
			m_dragLabel = m_matlabLabel;
			modelType = MATLABTYPE;
			pixmap = new QPixmap("./Icon/simutool/matlab");
		}
		else if (m_adamsLabel->geometry().contains(point)) {
			m_dragLabel = m_adamsLabel;
			modelType = ADAMSTYPE;
			pixmap = new QPixmap("./Icon/simutool/adams");
		}
		else {
			return;
		}

		QDrag *dg = new QDrag(m_dragLabel);
		dg->setPixmap(*pixmap);

		QMimeData *md = new QMimeData;
		md->setText(DRAG_COPY);
		md->setObjectName(modelType);
		md->setImageData(pixmap->toImage());

		dg->setMimeData(md);
		dg->exec();
	}
}

void ToolDragStack::putImage(QString path, QLabel *label) {
	QPixmap *pixmap = new QPixmap("./Icon/simutool/" + path);
	pixmap->scaled(label->size(), Qt::KeepAspectRatio);
	label->setScaledContents(true);
	label->setPixmap(*pixmap);
}