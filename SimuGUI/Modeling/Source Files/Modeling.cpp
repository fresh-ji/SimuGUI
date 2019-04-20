
/*
* @date : 2018/01/05
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

	QWidget *widget = new QWidget();

	//QLabel *title = new QLabel();
	//title->setText("Modeling");
	//QFont font("Microsoft YaHei", 20, 75);
	//title->setFont(font);

	createToolDragStack();
	createDropLabel();
	createDateTypeStack();
	createModelInfoStack();
	createInterfaceInfoStack();

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
	//只有基于表格的改变会被发到interface层
	//connect(m_pDropLabel, SIGNAL(signalModelChange(QString)),
	//	m_pInterfaceInfoStack, SLOT(slotModelChange(QString)));
	connect(m_pModelInfoStack, SIGNAL(signalModelChange(QString)),
		m_pInterfaceInfoStack, SLOT(slotModelChange(QString)));

	//自定义数据类型改变发给interface
	connect(m_pDataTypeStack, SIGNAL(refreshTypeSet(QString, QString)),
		m_pInterfaceInfoStack, SLOT(slotRefreshTypeSet(QString, QString)));

	QGridLayout *layout = new QGridLayout();
	layout->setMargin(10);
	layout->setSpacing(10);
	//TODO：如果在各类内部能够调整就不用这些了
	//layout->setColumnStretch(0, 2);
	//layout->setColumnStretch(1, 5);
	//layout->setColumnStretch(2, 5);
	//layout->setRowStretch(0, 2);
	//layout->setRowStretch(1, 1);
	//layout->addWidget(title, 0, 0, 1, 3, Qt::AlignCenter);

	layout->addWidget(m_pToolDragStack, 0, 0);
	layout->addWidget(m_pDropLabel, 0, 1, 1, 2);
	layout->addWidget(m_pDataTypeStack, 1, 0);
	layout->addWidget(m_pModelInfoStack, 1, 1);
	layout->addWidget(m_pInterfaceInfoStack, 1, 2);

	widget->setLayout(layout);
	setWidget(widget);
}

void Modeling::createToolDragStack() {
	m_pToolDragStack = new ToolDragStack();
	m_pToolDragStack->setTitle("Tools");
}

void Modeling::createDropLabel() {
	m_pDropLabel = new DropLabel();
	m_pDropLabel->setStyleSheet("border:2px solid black;background-color:white;");
	m_pDropLabel->setAcceptDrops(true);
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