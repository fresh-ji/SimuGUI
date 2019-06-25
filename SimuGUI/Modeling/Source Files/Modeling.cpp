
#include "Modeling.h"

Modeling::Modeling(QWidget *parent) : IMode(parent) {

	setObjectName(QLatin1String("Modeling"));
	setDisplayName(tr("Modeling"));
	
	setIcon(QIcon(iconPath + "mode/modeling"));

	createWindow();
}

void Modeling::createWindow() {

	splitterMain = new QSplitter(Qt::Vertical);
	splitterTop = new QSplitter(Qt::Horizontal, splitterMain);
	splitterDown = new QSplitter(Qt::Horizontal, splitterMain);

	createToolDragStack();
	createDropLabel();
	createDateTypeStack();
	createModelInfoStack();
	createInterfaceInfoStack();

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
	navBar->add(tr("LOG AREA"), m_logBrowser);
	m_logBar->addWidget(navBar);

	setWidget(widget);
}

void Modeling::createConnects() {
	//接收消息发送
	connect(m_pDropLabel, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pDataTypeStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pModelInfoStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));
	connect(m_pInterfaceInfoStack, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));


	//添加模型，拖动进入时向模型列表中发送
	connect(m_pDropLabel, SIGNAL(signalAddModel(QString, QString)),
		m_pModelInfoStack, SLOT(slotAddModel(QString, QString)));
	//删除模型，模型列表删除时向中心发送
	connect(m_pModelInfoStack, SIGNAL(signalDeleteModel(QString)),
		m_pDropLabel, SLOT(slotDeleteModel(QString)));
	//改变选择项，中心向模型列表发送
	connect(m_pDropLabel, SIGNAL(signalModelChange(QString)),
		m_pModelInfoStack, SLOT(slotModelChange(QString)));
	//发送列表项改变
	connect(m_pModelInfoStack, SIGNAL(signalModelChange(QString)),
		m_pDropLabel, SLOT(slotModelChange(QString)));
	//只有基于表格的改变会被发到interface层，中心与interface层的会中转
	connect(m_pModelInfoStack, SIGNAL(signalModelChange(QString)),
		m_pInterfaceInfoStack, SLOT(slotModelChange(QString)));

	//自定义数据类型改变发给interface
	connect(m_pDataTypeStack, SIGNAL(refreshTypeSet(QString, QString)),
		m_pInterfaceInfoStack, SLOT(slotRefreshTypeSet(QString, QString)));

	//drop统一获取overview信息
	connect(m_pDropLabel, SIGNAL(signalDataTypeOverview()),
		m_pDataTypeStack, SLOT(slotGetOverview()));
	connect(m_pDropLabel, SIGNAL(signalInterfaceOverview()),
		m_pInterfaceInfoStack, SLOT(slotGetOverview()));
}

void Modeling::createToolDragStack() {
	m_pToolDragStack = new ToolDragStack(splitterTop);
	m_pToolDragStack->setTitle("Instance");
}

void Modeling::createDropLabel() {
	m_pDropLabel = new DropLabel(splitterTop);
}

void Modeling::createDateTypeStack() {
	m_pDataTypeStack = new DataTypeStack(splitterDown);
	m_pDataTypeStack->setTitle("DATA");
}

void Modeling::createModelInfoStack() {
	m_pModelInfoStack = new ModelInfoStack(splitterDown);
	m_pModelInfoStack->setTitle("Model Info");
}

void Modeling::createInterfaceInfoStack() {
	m_pInterfaceInfoStack = new InterfaceInfoStack(splitterDown);
	m_pInterfaceInfoStack->setTitle("I/O Info");
}

void Modeling::slotReceiveMessage(QString info) {
	QMessageBox::information(NULL, "Title", info, QMessageBox::Yes, QMessageBox::Yes);
}

void Modeling::slotResizable(bool resizable) {
	m_logBar->setSizeGripEnabled(resizable);
}