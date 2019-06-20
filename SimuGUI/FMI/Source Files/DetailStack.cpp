
#include "DetailStack.h"
#include "Regulation.h"

DetailStack::DetailStack(QWidget *p) : MiniStack(p) {

	setTitleHeight(40);
	info = NULL;

	//基本信息
	infoWidget = new QWidget();
	infoWidget->setStyleSheet("border: 2px solid black;");
	infoLayout = new QGridLayout();
	infoWidget->setLayout(infoLayout);
	addTab(tr("Info"), infoWidget);

	//变量信息
	variableTable = new QTableWidget();
	//设置
	variableTable->setColumnCount(7);
	QStringList header;
	header << "Name" << "valueReference" << "description"
		<< "causality" << "variability" << "initial" << "canHandleMSPTI";
	variableTable->setHorizontalHeaderLabels(header);
	tableStandardize(variableTable);

	FancyButton *plotButton = new FancyButton();
	plotButton->setIcon(QIcon("./Icon/function/plot"));
	plotButton->setCursor(QCursor(Qt::PointingHandCursor));
	//connect(plotButton, SIGNAL(clicked()), this, SLOT());

	addTab(tr("Variables"), variableTable, plotButton);

	//运行信息
	QWidget *simuInfoStack = new QWidget();
	QGridLayout *simuLayout = new QGridLayout();

	QLabel *startTimeLabel = new QLabel();
	startTimeLabel->setText("start time : ");
	simuLayout->addWidget(startTimeLabel, 0, 0);
	startTime = new QLineEdit();
	simuLayout->addWidget(startTime, 0, 1);
	startTime->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	QLabel *stopTimeLabel = new QLabel();
	stopTimeLabel->setText("stop time : ");
	simuLayout->addWidget(stopTimeLabel, 1, 0);
	stopTime = new QLineEdit();
	simuLayout->addWidget(stopTime, 1, 1);
	stopTime->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	QLabel *stepSizeLabel = new QLabel();
	stepSizeLabel->setText("step size : ");
	simuLayout->addWidget(stepSizeLabel, 2, 0);
	stepSize = new QLineEdit();
	simuLayout->addWidget(stepSize, 2, 1);
	stepSize->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	simuInfoStack->setLayout(simuLayout);

	FancyButton *goButton = new FancyButton();
	goButton->setIcon(QIcon("./Icon/function/go"));
	goButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(goButton, SIGNAL(clicked()), this, SLOT(slotGo()));

	addTab(tr("Simulation"), simuInfoStack, goButton);
}

void DetailStack::slotModelDetail(FMUInfo* info) {

	//删除之前布局
	QLayoutItem *child;
	while ((child = infoLayout->takeAt(0)) != 0) {
		infoLayout->removeWidget(child->widget());
		child->widget()->setParent(0);
		delete child;
	}
	//删除之前表格
	variableTable->clearContents();
	variableTable->setRowCount(0);

	this->info = info;

	if (info != NULL) {
		refreshInfo();
		refreshVariable();
	}
}

void DetailStack::refreshInfo() {
	//装填基本信息
	QLabel *typeLabel = new QLabel();
	typeLabel->setText("FMI type");
	infoLayout->addWidget(typeLabel, 0, 0);
	QLabel *type = new QLabel();
	type->setText(QString::fromStdString(info->simuType));
	typeLabel->setStyleSheet("border: 0px;");
	type->setStyleSheet("border: 0px;");
	infoLayout->addWidget(type, 0, 1);

	map<string, string>::iterator it;
	int count;
	for (count = 1, it = info->basicInfo.begin(); it != info->basicInfo.end(); ++it, ++count) {
		QLabel *title = new QLabel();
		title->setText(QString::fromStdString(it->first));
		infoLayout->addWidget(title, count, 0);
		QLabel *content = new QLabel();
		content->setText(QString::fromStdString(it->second));
		if (count % 2 == 1) {
			title->setStyleSheet("background-color: lightgray;" "border: 0px;");
			content->setStyleSheet("background-color: lightgray;" "border: 0px;");
		}
		else {
			title->setStyleSheet("border: 0px;");
			content->setStyleSheet("border: 0px;");
		}
		infoLayout->addWidget(content, count, 1);
	}
}

void DetailStack::refreshVariable() {
	//装填变量信息
	for (FMIVariable* fv : info->variableInfo) {
		int row = variableTable->rowCount();
		variableTable->insertRow(row);

		QTableWidgetItem *item;

		item = new QTableWidgetItem(QString::fromStdString(fv->name));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 0, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->valueReference));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 1, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->description));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 2, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->causality));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 3, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->variability));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 4, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->initial));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 5, item);

		item = new QTableWidgetItem(QString::fromStdString(fv->canHandleMSPTI));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		variableTable->setItem(row, 6, item);
	}
}

void DetailStack::slotGo() {
	if (info == NULL) {
		signalSendMessage("No model Select!");
		return;
	}

	if (startTime->text().isEmpty() ||
		stopTime->text().isEmpty() ||
		stepSize->text().isEmpty()) {

		signalSendMessage("Empty items!");
		return;
	}

	double start = startTime->text().toDouble();
	double stop = stopTime->text().toDouble();
	double step = stepSize->text().toDouble();

	if (start >= stop) {
		signalSendMessage("Stop time wrong!");
		return;
	}

	signalRunSimulation(start, stop, step);

}