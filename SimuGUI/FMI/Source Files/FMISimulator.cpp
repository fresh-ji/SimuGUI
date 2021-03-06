
#include "FMISimulator.h"

FMISimulator::FMISimulator(QWidget *parent) : IMode(parent) {

	setObjectName(QLatin1String("FMISimulator"));
	setDisplayName(tr("FMI\nSimulator"));
	setIcon(QIcon(iconPath + "mode/fmi"));

	createWindow();
}

void FMISimulator::createWindow() {

	splitterMain = new QSplitter(Qt::Horizontal);

	createCentralLabel();
	createDetailStack();

	createConnects();

	FancyTabWidget *widget = new FancyTabWidget();

	//装配核心widget
	widget->insertTab(0, splitterMain, QIcon(), NULL, false);
	widget->closeSideEffect();

	//装配日志区域
	m_logBar = widget->statusBar();
	connect(this, SIGNAL(resizable(bool)), this, SLOT(slotResizable(bool)));

	FancyNavBar *navBar = new FancyNavBar();
	//设置点击之后的状态区在哪
	QSplitter *splitter = widget->addCornerWidget(navBar->panel(), FancyTabWidget::Bottom);
	navBar->setSplitter(splitter);

	m_logBrowser = new QTextBrowser();
	navBar->add(tr("log area"), m_logBrowser);
	//示例用法
	QTextBrowser *tb2 = new QTextBrowser();
	navBar->add(tr("invisible"), tb2, new QLineEdit("This item should not be seen"));
	m_logBar->addWidget(navBar);

	setWidget(widget);
}

void FMISimulator::createConnects() {

	//接收消息发送
	connect(m_pCentralLabel, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pDetailStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	//接受日志
	connect(m_pCentralLabel, SIGNAL(signalWriteLog(QString)),
		this, SLOT(slotWriteLog(QString)));
	connect(m_pDetailStack, SIGNAL(signalWriteLog(QString)),
		this, SLOT(slotWriteLog(QString)));


	//FMU详情发送
	connect(m_pCentralLabel, SIGNAL(signalModelDetail(FMUInfo*)),
		m_pDetailStack, SLOT(slotModelDetail(FMUInfo*)));

	//仿真FMU
	connect(m_pDetailStack, SIGNAL(signalRunSimulation(double, double, double)),
		m_pCentralLabel, SLOT(slotRunSimulation(double, double, double)));

	//画图
	connect(m_pDetailStack, SIGNAL(signalPlot(QString)),
		m_pCentralLabel, SLOT(slotPlot(QString)));
}

void FMISimulator::createCentralLabel() {
	m_pCentralLabel = new CentralLabel(splitterMain);
	m_pCentralLabel->setMinimumWidth(400);
}

void FMISimulator::createDetailStack() {
	m_pDetailStack = new DetailStack(splitterMain);
	m_pDetailStack->setTitle("Detail");
}

void FMISimulator::slotReceiveMessage(QString info) {
	QMessageBox::information(NULL, "Title", info, QMessageBox::Yes, QMessageBox::Yes);
}

void FMISimulator::slotWriteLog(QString info) {
	m_logBrowser->append(info);
}

void FMISimulator::slotResizable(bool resizable) {
	m_logBar->setSizeGripEnabled(resizable);
}