
#include "HLARunControl.h"

HLARunControl::HLARunControl(QWidget *parent) : IMode(parent) {

	setObjectName(QLatin1String("HLARunControl"));
	setDisplayName(tr("HLA\nRunControl"));
	setIcon(QIcon("./Icon/mode/hlarc"));

	createWindow();

	flowThread = new HLAFlowThread();
	connect(flowThread, SIGNAL(FlowSignal(QString)), this, SLOT(DisplayFlow(QString)));
	connect(flowThread, SIGNAL(StateSignal(int)), this, SLOT(DisplayState(int)));
	connect(flowThread->getInstance(), SIGNAL(postUIMsg(int, QString)), this, SLOT(DisplayHLAState(int, QString)));
}

void HLARunControl::createWindow() {

	QWidget *widget = new QWidget();

	QLabel *title = new QLabel();
	title->setText("HLA Run Control System");
	QFont font("Microsoft YaHei", 20, 75);
	title->setFont(font);

	QLCDNumber *lcdNumber = new QLCDNumber();
	lcdNumber->setSegmentStyle(QLCDNumber::Flat);
	lcdNumber->setFixedWidth(200);

	MiniStack *functionStack = new MiniStack();
	functionStack->setTitle("function area");
	functionStack->setFixedWidth(300);
	FancyButton *prepare = new FancyButton();
	prepare = addFunctionButton(prepare, "prepare");
	connect(prepare, SIGNAL(clicked()), this, SLOT(prepare()));
	FancyButton *ready = new FancyButton();
	ready = addFunctionButton(ready, "ready");
	FancyButton *stop = new FancyButton();
	stop = addFunctionButton(stop, "stop");
	FancyButton *start = new FancyButton();
	start = addFunctionButton(start, "start");
	FancyButton *pause = new FancyButton();
	pause = addFunctionButton(pause, "pause");
	QGridLayout *buttonLayout = new QGridLayout();
	buttonLayout->addWidget(prepare, 0, 0, 1, 1);
	buttonLayout->addWidget(ready, 0, 1, 1, 1);
	buttonLayout->addWidget(stop, 0, 2, 1, 1);
	buttonLayout->addWidget(start, 1, 0, 1, 1);
	buttonLayout->addWidget(pause, 1, 1, 1, 1);
	QWidget *buttonGroup = new QWidget();
	buttonGroup->setLayout(buttonLayout);
	functionStack->addTab(tr("function"), buttonGroup);

	MiniStack *stateStack = new MiniStack();
	stateStack->setTitle("state area");
	stateStack->setFixedWidth(300);
	stateLabel = new QLabel();
	stateLabel->setAlignment(Qt::AlignCenter);
	putImage("1", stateLabel);
	QHBoxLayout *stateLayout = new QHBoxLayout();
	stateLayout->addWidget(stateLabel);
	QWidget *stateWidget = new QWidget();
	stateWidget->setLayout(stateLayout);
	stateStack->addTab(tr("state"), stateWidget);

	MiniStack *bar2 = new MiniStack();
	bar2->setTitle("empty area");

	MiniStack *infoStack = new MiniStack();
	infoStack->setTitle("information area");
	info = new QTextBrowser();
	QHBoxLayout *infoLayout = new QHBoxLayout();
	infoLayout->addWidget(info);
	QWidget *infoWidget = new QWidget();
	infoWidget->setLayout(infoLayout);
	infoStack->addTab(tr("state"), infoWidget);

	QGridLayout *layout = new QGridLayout();
	layout->setMargin(10);
	layout->setSpacing(10);
	layout->addWidget(title, 0, 0, 1, 5, Qt::AlignCenter);
	layout->addWidget(lcdNumber, 1, 0, 1, 5, Qt::AlignRight);
	layout->addWidget(functionStack, 2, 0, 1, 1);
	layout->addWidget(stateStack, 3, 0, 1, 1);
	layout->addWidget(bar2, 2, 1, 2, 2);
	layout->addWidget(infoStack, 2, 3, 2, 2);

	widget->setLayout(layout);
	setWidget(widget);
}

void HLARunControl::putImage(QString path, QLabel *label) {
	QPixmap *pixmap = new QPixmap("./Icon/hlaState/" + path);
	pixmap->scaled(label->size(), Qt::KeepAspectRatio);
	label->setScaledContents(true);
	label->setPixmap(*pixmap);
}

FancyButton* HLARunControl::addFunctionButton(FancyButton* button, QString name) {
	QFont font("Microsoft YaHei", 15);
	button->setFixedHeight(60);
	button->setTextColor(Qt::black);
	button->setFont(font);
	button->setText(name);
	button->setHoverColor(Qt::green);
	return button;
}

void HLARunControl::DisplayFlow(QString msg) {
	info->append(msg);
}

void HLARunControl::DisplayState(int state) {
	if (state == StateMachineCode::getJoinedCode()) {
		putImage("2", stateLabel);
	}
	else if (state == StateMachineCode::getInitialedCode()) {
		putImage("3", stateLabel);
	}
	else if (state == StateMachineCode::getRegisPointCode()) {
		putImage("4", stateLabel);
	}
	else if (state == StateMachineCode::getSynchedCode()) {
		putImage("5", stateLabel);
	}
	else if (state == StateMachineCode::getPubAndSubCode()) {
		putImage("6", stateLabel);
	}
	else if (state == StateMachineCode::getRegisteredCode()) {
		putImage("7", stateLabel);
	}
	else if (state == StateMachineCode::getRunningCode()) {
		putImage("8", stateLabel);
	}
}

void HLARunControl::DisplayHLAState(int a, QString s) {
	info->append(s);
}

void HLARunControl::prepare() {
	//�ж�����״̬
	if (flowThread->getState() != StateMachineCode::getUnstartCode()) {
		QMessageBox::warning(NULL, "Warning", "Environment Already Established!!!");
		return;
	}
	//create���������߳�ִ�У�������������������
	info->append("Preparing Environment, Please Wait...");
	QCoreApplication::processEvents();
	InterfaceResponse<bool> *ir = flowThread->getInstance()->createFederation();
	if (!ir->isSuccessful()) {
		info->append("HERE IS AN ERROR");
	}
	info->append(ir->getMessage().c_str());
	flowThread->start();
}