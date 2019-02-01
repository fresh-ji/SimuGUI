

#include "InterfaceInfoStack.h"

InterfaceInfoStack::InterfaceInfoStack(QWidget *p) : MiniStack(p) {
	m_pInterfaceList = new QTableWidget();

	//表头
	m_pInterfaceList->setColumnCount(3);
	m_pInterfaceList->horizontalHeader()->setSectionsClickable(false);
	m_pInterfaceList->setColumnWidth(0, 70);
	m_pInterfaceList->setColumnWidth(1, 90);
	m_pInterfaceList->setColumnWidth(2, 90);

	QFont font;
	font.setBold(true);
	m_pInterfaceList->horizontalHeader()->setFont(font);
	m_pInterfaceList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header;
	header << "I/O" << "Name" << "Type";
	m_pInterfaceList->setHorizontalHeaderLabels(header);

	//列头
	m_pInterfaceList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pInterfaceList->verticalHeader()->setVisible(false);

	//选择模式
	m_pInterfaceList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pInterfaceList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pInterfaceList->setStyleSheet("selection-background-color:lightblue;");

	//滚动条
	m_pInterfaceList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	m_pInterfaceList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	FancyButton *addButton = new FancyButton();
	addButton->setIcon(QIcon("./Icon/tools/add"));
	addButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addButton, SIGNAL(clicked()), this, SLOT(pSlotAddInterface()));

	FancyButton *subscribeButton = new FancyButton();
	subscribeButton->setIcon(QIcon("./Icon/tools/subscribe"));
	subscribeButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(subscribeButton, SIGNAL(clicked()), this, SLOT(pSlotSubscribeInterface()));

	FancyButton *editButton = new FancyButton();
	editButton->setIcon(QIcon("./Icon/tools/edit"));
	editButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editButton, SIGNAL(clicked()), this, SLOT(pSlotEditInterface()));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/tools/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteInterface()));

	QWidget *buttons = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(addButton);
	layout->addWidget(subscribeButton);
	layout->addWidget(editButton);
	layout->addWidget(deleteButton);
	buttons->setLayout(layout);

	addTab(tr("interfaceList"), m_pInterfaceList, buttons);
}

void InterfaceInfoStack::slotModelChange(QString modelName) {

	if (selectedModel == modelName) {
		return;
	}

	//有可能是NULL
	selectedModel = modelName;

	m_pInterfaceList->setRowCount(0);
	m_pInterfaceList->clearContents();

	if (selectedModel != NULL) {
		for (interfaceInfo i : interfaceSet) {
			if (i.publisher == modelName) {
				int row = m_pInterfaceList->rowCount();
				m_pInterfaceList->insertRow(row);

				QTableWidgetItem *item;

				item = new QTableWidgetItem(outputType);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInterfaceList->setItem(row, 0, item);

				item = new QTableWidgetItem(i.iName);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInterfaceList->setItem(row, 1, item);

				item = new QTableWidgetItem(i.iDataType);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInterfaceList->setItem(row, 2, item);
			}
			//订阅关系
			//if (i.subscribers) {
			//
			//}
		}
	}
}

void InterfaceInfoStack::pSlotAddInterface() {
	if (selectedModel != NULL) {
		interfaceInfo iInfo;
		iInfo.publisher = selectedModel;
		dialog = new EditInterfaceDialog(iInfo, true);
		connect(dialog, SIGNAL(refreshInterface(interfaceInfo, bool)),
			this, SLOT(slotRefreshInterface(interfaceInfo, bool)));
		connect(dialog, SIGNAL(signalSendMessage(QString)),
			this, SLOT(slotMessageFromDialog(QString)));
		dialog->exec();
	}
}

void InterfaceInfoStack::pSlotSubscribeInterface() {

}

void InterfaceInfoStack::pSlotEditInterface() {

}

void InterfaceInfoStack::pSlotDeleteInterface() {

}

void InterfaceInfoStack::slotRefreshInterface(interfaceInfo iInfo, bool isAdd) {
	if (isAdd) {
		auto ret = interfaceSet.insert(iInfo);
		if (ret.second == 0) {
			emit signalSendMessage("name already used");
			dialog = new EditInterfaceDialog(iInfo, true);
			connect(dialog, SIGNAL(refreshInterface(interfaceInfo, bool)),
				this, SLOT(slotRefreshInterface(interfaceInfo, bool)));
			connect(dialog, SIGNAL(signalSendMessage(QString)),
				this, SLOT(slotMessageFromDialog(QString)));
			dialog->exec();
		}
		else {
			//添加成功
			int row = m_pInterfaceList->rowCount();
			m_pInterfaceList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(outputType);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pInterfaceList->setItem(row, 0, item);

			item = new QTableWidgetItem(iInfo.iName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pInterfaceList->setItem(row, 1, item);

			item = new QTableWidgetItem(iInfo.iDataType);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pInterfaceList->setItem(row, 2, item);
		}
	}
	else {
		//edit
	}
}

void InterfaceInfoStack::slotMessageFromDialog(QString message) {
	emit signalSendMessage(message);
}