
/*
* @date : 2019/01/01
* @author : jihang
*/

#include "FMISimulator.h"
//#include "FMISupport.h"

FMISimulator::FMISimulator(QWidget *parent) : IMode(parent) {
	setObjectName(QLatin1String("FMISimulator"));
	setDisplayName(tr("FMI\nSimulator"));
	setIcon(QIcon("./Icon/mode/fmi"));

	createWindow();
}

void FMISimulator::createWindow() {

	createCentralLabel();
	createDetailStack();
	createPlotStack();

	createConnects();

	QGridLayout *layout = new QGridLayout();
	layout->setMargin(10);
	layout->setSpacing(10);
	
	//TODO����Ҫ�ӵ���
	m_pDetailStack->setMaximumWidth(600);
	m_pPlotStack->setMaximumWidth(600);
	layout->addWidget(m_pCentralLabel, 0, 0, 2, 1);
	layout->addWidget(m_pDetailStack, 0, 1);
	layout->addWidget(m_pPlotStack, 1, 1);

	QWidget *carryWidget = new QWidget();
	carryWidget->setLayout(layout);

	FancyTabWidget *widget = new FancyTabWidget();

	//װ�����widget
	widget->insertTab(0, carryWidget, QIcon(), NULL, false);
	widget->closeSideEffect();

	//װ����־����
	m_logBar = widget->statusBar();
	connect(this, SIGNAL(resizable(bool)), this, SLOT(slotResizable(bool)));

	FancyNavBar *navBar = new FancyNavBar();
	//���õ��֮���״̬������
	QSplitter *splitter = widget->addCornerWidget(navBar->panel(), FancyTabWidget::Bottom);
	navBar->setSplitter(splitter);

	m_logBrowser = new QTextBrowser();
	navBar->add(tr("log area"), m_logBrowser);
	//ʾ���÷�
	QTextBrowser *tb2 = new QTextBrowser();
	navBar->add(tr("invisible"), tb2, new QLineEdit("This item should not be seen"));
	m_logBar->addWidget(navBar);

	setWidget(widget);
}

void FMISimulator::createConnects() {
	//������Ϣ����
	connect(m_pCentralLabel, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pDetailStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	//������־
	connect(m_pCentralLabel, SIGNAL(signalWriteLog(QString)),
		this, SLOT(slotWriteLog(QString)));
	connect(m_pDetailStack, SIGNAL(signalWriteLog(QString)),
		this, SLOT(slotWriteLog(QString)));


	//FMU���鷢��
	connect(m_pCentralLabel, SIGNAL(signalModelDetail(FMUInfo*)),
		m_pDetailStack, SLOT(slotModelDetail(FMUInfo*)));

	//����FMU
	connect(m_pDetailStack, SIGNAL(signalRunSimulation(double, double, double)),
		m_pCentralLabel, SLOT(slotRunSimulation(double, double, double)));
	
}

void FMISimulator::createCentralLabel() {
	m_pCentralLabel = new CentralLabel();
}

void FMISimulator::createDetailStack() {
	m_pDetailStack = new DetailStack();
	m_pDetailStack->setTitle("Detail");
}

void FMISimulator::createPlotStack() {
	m_pPlotStack = new PlotStack();
	m_pPlotStack->setTitle("Plot");
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