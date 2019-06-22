
#include "DetailStack.h"
#include "Regulation.h"

DetailStack::DetailStack(QWidget *p) : MiniStack(p) {

	setTitleHeight(40);
	activeInfo = NULL;

	//1.基本信息
	infoWidget = new QWidget();
	infoWidget->setStyleSheet("border: 2px solid black;");
	infoLayout = new QGridLayout();
	infoWidget->setLayout(infoLayout);
	addTab(tr("Info"), infoWidget);

	//2.变量信息
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
	plotButton->setFixedWidth(50);
	connect(plotButton, SIGNAL(clicked()), this, SLOT(slotPlot()));

	addTab(tr("Variables"), variableTable, plotButton);

	//3.运行信息
	QWidget *simuInfoStack = new QWidget();
	QGridLayout *simuLayout = new QGridLayout();

	QLabel *startTimeLabel = new QLabel();
	startTimeLabel->setText("simulation start time : ");
	startTimeLabel->setFixedWidth(200);
	simuLayout->addWidget(startTimeLabel, 0, 0);
	startTime = new QLineEdit();
	startTime->setFixedWidth(100);
	startTime->setText("0.0");
	simuLayout->addWidget(startTime, 0, 1);
	startTime->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	QLabel *stopTimeLabel = new QLabel();
	stopTimeLabel->setText("simulation stop time : ");
	stopTimeLabel->setFixedWidth(200);
	simuLayout->addWidget(stopTimeLabel, 1, 0);
	stopTime = new QLineEdit();
	stopTime->setFixedWidth(100);
	stopTime->setText("10.0");
	simuLayout->addWidget(stopTime, 1, 1);
	stopTime->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	QLabel *stepSizeLabel = new QLabel();
	stepSizeLabel->setText("simulation step size : ");
	stepSizeLabel->setFixedWidth(200);
	simuLayout->addWidget(stepSizeLabel, 2, 0);
	stepSize = new QLineEdit();
	stepSize->setFixedWidth(100);
	stepSize->setText("0.1");
	simuLayout->addWidget(stepSize, 2, 1);
	stepSize->setValidator(new QDoubleValidator(0.0, 1024.0, 10));

	simuLayout->setAlignment(Qt::AlignTop);
	simuInfoStack->setLayout(simuLayout);

	FancyButton *goButton = new FancyButton();
	goButton->setIcon(QIcon("./Icon/function/go"));
	goButton->setCursor(QCursor(Qt::PointingHandCursor));
	goButton->setFixedWidth(50);
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

	this->activeInfo = info;

	if (info != NULL) {
		refreshInfo();
		refreshVariable();
	}
}

void DetailStack::refreshInfo() {
	//装填基本信息
	QLabel *typeLabel = new QLabel();
	typeLabel->setText("FMI type");
	typeLabel->setStyleSheet("border: 0px;");
	infoLayout->addWidget(typeLabel, 0, 0);
	QLabel *type = new QLabel();
	type->setText(QString::fromStdString(activeInfo->simuType));
	type->setStyleSheet("border: 0px;");
	infoLayout->addWidget(type, 0, 1);

	map<string, string>::iterator it;
	int count;
	for (count = 1, it = activeInfo->basicInfo.begin(); it != activeInfo->basicInfo.end(); ++it, ++count) {
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
			title->setStyleSheet("border: 0px solid black;");
			content->setStyleSheet("border: 0px solid black;");
		}
		infoLayout->addWidget(content, count, 1);
	}
	infoLayout->setAlignment(Qt::AlignTop);
}

void DetailStack::refreshVariable() {
	//装填变量信息
	//TODO：变量过多怎么办，变量分级怎么显示
	for (FMIVariable* fv : activeInfo->variableInfo) {
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

	if (activeInfo == NULL) {
		emit signalSendMessage("no active model");
		return;
	}

	if (startTime->text().isEmpty() || stopTime->text().isEmpty() ||
		stepSize->text().isEmpty()) {
		emit signalSendMessage("time setting wrong");
		return;
	}

	double start = startTime->text().toDouble();
	double stop = stopTime->text().toDouble();
	double step = stepSize->text().toDouble();

	if (start >= stop) {
		emit signalSendMessage("stop not behind start");
		return;
	}

	signalRunSimulation(start, stop, step);
}

void DetailStack::slotPlot() {
	if (!variableTable->currentItem()) {
		emit signalSendMessage("no variable selected");
		return;
	}
	if (variableTable->currentItem()->isSelected()) {
		int row = variableTable->currentItem()->row();
		emit signalPlot(variableTable->item(row, 0)->text());
	}
}