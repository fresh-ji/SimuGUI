
/*
* @date : 2019/01/05
* @author : jihang
*/

#include "Modeling.h"

Modeling::Modeling(QWidget *parent) : IMode(parent) {
	setObjectName(QLatin1String("Modeling"));
	setDisplayName(tr("Modeling"));
	setIcon(QIcon("./Icon/mode/modeling"));

	createWindow();
}

void Modeling::createWindow() {

	createToolDragStack();
	createDropLabel();
	createDateTypeStack();
	createModelInfoStack();
	createInterfaceInfoStack();

	createConnects();

	QGridLayout *layout = new QGridLayout();
	layout->setMargin(10);
	layout->setSpacing(10);

	//TODO������ڸ����ڲ��ܹ������Ͳ�����Щ�ˣ���Ҫ�ӵ���
	//layout->setColumnStretch(0, 2);
	//layout->setColumnStretch(1, 5);
	//layout->setColumnStretch(2, 5);
	//layout->setRowStretch(0, 2);
	//layout->setRowStretch(1, 1);
	m_pDataTypeStack->setMaximumHeight(300);
	m_pModelInfoStack->setMaximumHeight(300);
	m_pInterfaceInfoStack->setMaximumHeight(300);

	layout->addWidget(m_pToolDragStack, 0, 0);
	layout->addWidget(m_pDropLabel, 0, 1, 1, 2);
	layout->addWidget(m_pDataTypeStack, 1, 0);
	layout->addWidget(m_pModelInfoStack, 1, 1);
	layout->addWidget(m_pInterfaceInfoStack, 1, 2);

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
	navBar->add(tr("LOG AREA"), m_logBrowser);
	m_logBar->addWidget(navBar);

	setWidget(widget);
}

void Modeling::createConnects() {
	//������Ϣ����
	connect(m_pDropLabel, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pDataTypeStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pModelInfoStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pInterfaceInfoStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));


	//���ģ�ͣ��϶�����ʱ��ģ���б��з���
	connect(m_pDropLabel, SIGNAL(signalAddModel(QString, QString)),
		m_pModelInfoStack, SLOT(slotAddModel(QString, QString)));
	//ɾ��ģ�ͣ�ģ���б�ɾ��ʱ�����ķ���
	connect(m_pModelInfoStack, SIGNAL(signalDeleteModel(QString)),
		m_pDropLabel, SLOT(slotDeleteModel(QString)));
	//�ı�ѡ���������ģ���б���
	connect(m_pDropLabel, SIGNAL(signalModelChange(QString)),
		m_pModelInfoStack, SLOT(slotModelChange(QString)));
	//�����б���ı�
	connect(m_pModelInfoStack, SIGNAL(signalModelChange(QString)),
		m_pDropLabel, SLOT(slotModelChange(QString)));
	//ֻ�л��ڱ��ĸı�ᱻ����interface�㣬������interface��Ļ���ת
	connect(m_pModelInfoStack, SIGNAL(signalModelChange(QString)),
		m_pInterfaceInfoStack, SLOT(slotModelChange(QString)));

	//�Զ����������͸ı䷢��interface
	connect(m_pDataTypeStack, SIGNAL(refreshTypeSet(QString, QString)),
		m_pInterfaceInfoStack, SLOT(slotRefreshTypeSet(QString, QString)));
}

void Modeling::createToolDragStack() {
	m_pToolDragStack = new ToolDragStack();
	m_pToolDragStack->setTitle("Instance");
}

void Modeling::createDropLabel() {
	m_pDropLabel = new DropLabel();
}

void Modeling::createDateTypeStack() {
	m_pDataTypeStack = new DataTypeStack();
	m_pDataTypeStack->setTitle("DATA");
}

void Modeling::createModelInfoStack() {
	m_pModelInfoStack = new ModelInfoStack();
	m_pModelInfoStack->setTitle("Model Info");
}

void Modeling::createInterfaceInfoStack() {
	m_pInterfaceInfoStack = new InterfaceInfoStack();
	m_pInterfaceInfoStack->setTitle("I/O Info");
}

void Modeling::slotReceiveMessage(QString info) {
	QMessageBox::information(NULL, "Title", info, QMessageBox::Yes, QMessageBox::Yes);
}

void Modeling::slotResizable(bool resizable) {
	m_logBar->setSizeGripEnabled(resizable);
}