
#include "CentralLabel.h"

CentralLabel::CentralLabel(QWidget *p) : QLabel(p) {

	setStyleSheet("border: 5px solid gray;"
		"background-color: white;border-radius: 10px;");
	setAcceptDrops(true);
	//TODO：加格点功能

	//无被选中模型
	activeModel = NULL;

	//初始文件选择路径
	//TODO：绝对路径测试使用
	openPath = QDir::currentPath() + "\\FMI\\examples\\cs\\x64_arch";

	//fmi
	FMIsupport = new FMISupport();

	//添加功能栏
	addToolBar();
}

void CentralLabel::dragEnterEvent(QDragEnterEvent *e) {
	e->accept();
}

void CentralLabel::mousePressEvent(QMouseEvent *event) {
	//左键
	if (event->button() & Qt::LeftButton) {
		QPoint point = event->localPos().toPoint();

		int isInItem = mousePressPreprocess(point);

		if (1 == isInItem) {
			//点中模型，准备开始模型拖动
			moveStartPoint = event->pos();

			QDrag *dg = new QDrag(activeModel);
			QMimeData *md = new QMimeData;
			md->setText(FMU_DRAG_MOVE);
			dg->setMimeData(md);
			dg->exec();
		}
		else if (0 == isInItem) {
			//未点中模型
			activeModel = NULL;
		}
	}
}

/*
 * 该函数为鼠标左键按下时的提前处理函数
 * 主要判断是否点到了模型或当前选中模型的锚点
 * 返回值0，代表未点中任何模型
 * 返回值1，代表点中模型
 */
int CentralLabel::mousePressPreprocess(QPoint pressPoint) {
	//优先当前模型
	if (activeModel) {
		//判断当前模型中其他点
		QRect rect(
			activeModel->geometry().x() + FMUFRAMESHIFTX,
			activeModel->geometry().y() + FMUFRAMESHIFTY,
			FMUWIDTH,
			FMUWIDTH + FMUEDGE * 2);
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
	for (FMUElement* ie : modelMap.values()) {
		QRect rect(
			ie->geometry().x() + FMUFRAMESHIFTX,
			ie->geometry().y() + FMUFRAMESHIFTY,
			FMUWIDTH,
			FMUWIDTH + FMUEDGE * 2);
		if (rect.contains(pressPoint)) {
			activeModel = ie;
			activeModel->active();
			activeModel->raise();
			//点中了某个模型，存在了activeModel里
			QString modelName = modelMap.key(ie);
			FMUInfo info = modelRepo.value(modelName);
			emit signalModelDetail(&info);
			return 1;
		}
	}
	//未点中模型
	emit signalModelDetail(NULL);
	return 0;
}

void CentralLabel::dragMoveEvent(QDragMoveEvent *event) {
	if (FMU_DRAG_MOVE == event->mimeData()->text()) {
		//移动距离
		int moveX = event->pos().x() - moveStartPoint.x();
		int moveY = event->pos().y() - moveStartPoint.y();
		//模型跟着跑
		QRect rect(moveX + activeModel->geometry().x(),
			moveY + activeModel->geometry().y(),
			activeModel->size().width(), activeModel->size().height());
		activeModel->setGeometry(rect);
		//动态移动起始点
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
	openButton->setIcon(QIcon(iconPath + "function/open"));
	connect(openButton, SIGNAL(clicked()), this, SLOT(slotOpen()));

	QToolButton *undoButton = new QToolButton(this);
	undoButton->setGeometry(70, 5, 50, 50);
	undoButton->setCursor(Qt::PointingHandCursor);
	undoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	undoButton->setIcon(QIcon(iconPath + "function/undo"));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(slotUndo()));

	QToolButton *redoButton = new QToolButton(this);
	redoButton->setGeometry(120, 5, 50, 50);
	redoButton->setCursor(Qt::PointingHandCursor);
	redoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	redoButton->setIcon(QIcon(iconPath + "function/redo"));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(slotRedo()));

	QToolButton *gridButton = new QToolButton(this);
	gridButton->setGeometry(170, 5, 50, 50);
	gridButton->setCursor(Qt::PointingHandCursor);
	gridButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	gridButton->setIcon(QIcon(iconPath + "function/grid"));
	connect(gridButton, SIGNAL(clicked()), this, SLOT(slotGrid()));

	QToolButton *deleteButton = new QToolButton(this);
	deleteButton->setGeometry(220, 5, 50, 50);
	deleteButton->setCursor(Qt::PointingHandCursor);
	deleteButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	deleteButton->setIcon(QIcon(iconPath + "function/delete"));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));

	QToolButton *infoButton = new QToolButton(this);
	infoButton->setGeometry(270, 5, 50, 50);
	infoButton->setStyleSheet("QToolButton{border: none;border-radius: 25px;}");
	infoButton->setIcon(QIcon(iconPath + "function/info"));
	infoButton->setToolTip("Only FMI-2.0 and WIN64 are supported");
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

		//看是不是正确的FMU
		FMUInfo info = FMIsupport->loadFMU(
			modelPath.toLocal8Bit().constData(),
			QUuid::createUuid().toString().toStdString());
		if (!info.isSuccess) {
			emit signalSendMessage(QString::fromStdString(info.message));
			return;
		}
		else {
			emit signalWriteLog("load " + name + " success");
		}
		modelRepo.insert(modelPath, info);

		int inputNumber = 0;
		int outputNumber = 0;
		for (FMIVariable* fv : info.variableInfo) {
			if ("input" == fv->causality) {
				inputNumber++;
			}
			if ("output" == fv->causality) {
				outputNumber++;
			}
		}

		FMUElement *item = new FMUElement(name, inputNumber, outputNumber, this);
		//TODO：做一个灵活定位的
		item->setGeometry(100, 100, 1, 1);
		item->show();

		if (activeModel) {
			activeModel->inactive();
		}
		activeModel = item;

		modelMap.insert(modelPath, item);

		//详情显示
		emit signalModelDetail(&info);
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

void CentralLabel::slotRunSimulation(double start, double stop, double step) {
	if (activeModel != NULL) {
		QString key = modelMap.key(activeModel);
		FMUInfo FMUinfo = modelRepo.value(key);
		simuInfo info;
		if (FMUinfo.simuType == FMI_MODEL_EXCHANGE) {
			info = FMIsupport->simulateByMe(
				FMUinfo.fmu, FMUinfo.resultDir, start, stop, step, 0, NULL);
		}
		if (FMUinfo.simuType == FMI_COSIMULATION) {
			info = FMIsupport->simulateByCs(
				FMUinfo.fmu, FMUinfo.resultDir, start, stop, step, 0, NULL);
		}
		if (info.isSucess) {
			FMUinfo.resultFile = info.resultPath;
			modelRepo.insert(key, FMUinfo);
			emit signalSendMessage(QString::fromStdString(info.message));
			emit signalWriteLog(QString::fromStdString(info.message));
			emit signalWriteLog("reslut file is saved in : "
				+ QString::fromStdString(info.resultPath));
		}
	}
}

void CentralLabel::slotPlot(QString name) {
	if (activeModel != NULL) {
		FMUInfo FMUinfo = modelRepo.value(modelMap.key(activeModel));
		QFile file(QString::fromStdString(FMUinfo.resultFile));
		if (!file.exists()) {
			emit signalSendMessage("no relative result file");
			return;
		}
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream stream(&file);
			QStringList firstRow = stream.readLine().split(",");

			//检测第一列是否是时间
			if (firstRow.at(0) != "time") {
				emit signalSendMessage("first column is now time");
				return;
			}
			//检索列序
			int index = firstRow.indexOf(name);
			if (index < 0) {
				emit signalSendMessage("file not found target name");
				return;
			}
			int timeIndex = 0;

			QVector<double> timeLine;
			QVector<double> dataLine;
			while (true) {
				QString row = stream.readLine();
				if (row.size() == 0) {
					//结束
					break;
				}
				//填入数据
				QStringList line = row.split(",");
				QString time = line.at(timeIndex);
				QString data = line.at(index);
				timeLine << time.toDouble();
				dataLine << data.toDouble();
			}
			dialog = new PlotDialog(name, timeLine, dataLine);
			dialog->exec();
		}
		else {
			emit signalSendMessage("target file can not open");
		}
	}
}