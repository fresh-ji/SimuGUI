

#include "InterfaceInfoStack.h"

InterfaceInfoStack::InterfaceInfoStack(QWidget *p) : MiniStack(p) {

	//输出表
	m_pOutputList = new QTableWidget();

	//表头
	m_pOutputList->setColumnCount(2);
	m_pOutputList->horizontalHeader()->setSectionsClickable(false);
	m_pOutputList->setColumnWidth(0, 160);
	m_pOutputList->setColumnWidth(1, 160);

	QFont font;
	font.setBold(true);
	m_pOutputList->horizontalHeader()->setFont(font);
	m_pOutputList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header;
	header << "Name" << "Type";
	m_pOutputList->setHorizontalHeaderLabels(header);

	//列头
	m_pOutputList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pOutputList->verticalHeader()->setVisible(false);

	//选择模式
	m_pOutputList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pOutputList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pOutputList->setStyleSheet("selection-background-color:lightblue;");

	//滚动条
	m_pOutputList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	m_pOutputList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	FancyButton *addOutputButton = new FancyButton();
	addOutputButton->setIcon(QIcon("./Icon/tools/add"));
	addOutputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addOutputButton, SIGNAL(clicked()), this, SLOT(pSlotAddOutput()));

	FancyButton *editOutputButton = new FancyButton();
	editOutputButton->setIcon(QIcon("./Icon/tools/edit"));
	editOutputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editOutputButton, SIGNAL(clicked()), this, SLOT(pSlotEditOutput()));

	FancyButton *deleteOutputButton = new FancyButton();
	deleteOutputButton->setIcon(QIcon("./Icon/tools/delete"));
	deleteOutputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteOutputButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteOutput()));

	QWidget *outputButtons = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(addOutputButton);
	layout->addWidget(editOutputButton);
	layout->addWidget(deleteOutputButton);
	outputButtons->setLayout(layout);

	addTab(tr("OUTPUT"), m_pOutputList, outputButtons);

	//输入表
	m_pInputList = new QTableWidget();

	//表头
	m_pInputList->setColumnCount(3);
	m_pInputList->horizontalHeader()->setSectionsClickable(false);
	m_pInputList->setColumnWidth(0, 100);
	m_pInputList->setColumnWidth(1, 110);
	m_pInputList->setColumnWidth(2, 110);

	m_pInputList->horizontalHeader()->setFont(font);
	m_pInputList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header2;
	header2 << "Name" << "Type" << "Publisher";
	m_pInputList->setHorizontalHeaderLabels(header2);

	//列头
	m_pInputList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pInputList->verticalHeader()->setVisible(false);

	//选择模式
	m_pInputList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pInputList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pInputList->setStyleSheet("selection-background-color:lightblue;");

	//滚动条
	m_pInputList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	m_pInputList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	FancyButton *subscribeInputButton = new FancyButton();
	subscribeInputButton->setIcon(QIcon("./Icon/tools/subscribe"));
	subscribeInputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(subscribeInputButton, SIGNAL(clicked()), this, SLOT(pSlotSubscribeInput()));

	FancyButton *deleteInputButton = new FancyButton();
	deleteInputButton->setIcon(QIcon("./Icon/tools/delete"));
	deleteInputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteInputButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteInput()));

	QWidget *inputButtons = new QWidget();
	QHBoxLayout *layout2 = new QHBoxLayout();
	layout2->addWidget(subscribeInputButton);
	layout2->addWidget(deleteInputButton);
	inputButtons->setLayout(layout2);

	addTab(tr("INPUT"), m_pInputList, inputButtons);
}

void InterfaceInfoStack::pSlotAddOutput() {
	if (selectedModel != NULL) {
		dialog = new EditOutputDialog(selectedModel);
		connect(dialog, SIGNAL(nameCheck(QString)),
			this, SLOT(slotNameCheck(QString)));
		connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
		connect(dialog, SIGNAL(refreshOutput(bool, QString, outputInfo)),
			this, SLOT(slotRefreshOutput(bool, QString, outputInfo)));
		//暂无消息
		//connect(dialog, SIGNAL(signalSendMessage(QString)),
		//	this, SLOT(slotMessageFromDialog(QString)));
		dialog->exec();
	}
	else {
		QMessageBox::information(NULL, "warning", "no model selected",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void InterfaceInfoStack::pSlotEditOutput() {
	QString name;
	if (!m_pOutputList->currentItem()) {
		return;
	}
	if (m_pOutputList->currentItem()->isSelected()) {

		int row = m_pOutputList->currentItem()->row();
		QString name = m_pOutputList->item(row, 0)->text();

		outputInfo oInfo;
		oInfo.oName = name;
		oInfo.oDataType = interfaceMap.value(name).dataType;
		oInfo.oPublisher = interfaceMap.value(name).publisher;

		dialog = new EditOutputDialog(oInfo);
		connect(dialog, SIGNAL(nameCheck(QString)),
			this, SLOT(slotNameCheck(QString)));
		connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
		connect(dialog, SIGNAL(refreshOutput(bool, QString, outputInfo)),
			this, SLOT(slotRefreshOutput(bool, QString, outputInfo)));
		//暂无消息
		//connect(dialog, SIGNAL(signalSendMessage(QString)),
		//	this, SLOT(slotMessageFromDialog(QString)));
		dialog->exec();
	}
}
void InterfaceInfoStack::pSlotDeleteOutput() {
	if (!m_pOutputList->currentItem()) {
		return;
	}
	if (m_pOutputList->currentItem()->isSelected()) {
		int row = m_pOutputList->currentItem()->row();
		QString name = m_pOutputList->item(row, 0)->text();
		//在map中删除
		interfaceMap.remove(name);
		//在列表中删除
		m_pOutputList->removeRow(row);
	}
}

void InterfaceInfoStack::pSlotSubscribeInput() {
	if (selectedModel != NULL) {
		dialog_in = new EditInputDialog(selectedModel);
		//暂无消息
		//connect(dialog_in, SIGNAL(signalSendMessage(QString)),
		//	this, SLOT(slotMessageFromDialog(QString)));
		dialog_in->exec();
	}
	else {
		QMessageBox::information(NULL, "warning", "no model selected",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void InterfaceInfoStack::pSlotDeleteInput() {

}

void InterfaceInfoStack::slotNameCheck(QString newName) {
	//不管增加还是修改，name一定是新的
	if (interfaceMap.contains(newName)) {
		QMessageBox::information(ERROR, "error", "name already used",
			QMessageBox::Yes, QMessageBox::Yes);
	}
	else {
		//通过名称检测
		emit nameValid();
	}
}

void InterfaceInfoStack::slotRefreshOutput(bool isAdd, QString preName, outputInfo oInfo) {

	if (isAdd) {
		if (interfaceMap.contains(oInfo.oName)) {
			//默认不会出现的情况
			QMessageBox::information(ERROR, "error", "name valid but can not insert",
				QMessageBox::Yes, QMessageBox::Yes);
		}
		else {
			//添加map
			interfaceInfo iInfo;
			iInfo.dataType = oInfo.oDataType;
			iInfo.publisher = oInfo.oPublisher;
			interfaceMap.insert(oInfo.oName, iInfo);
			//添加列表
			int row = m_pOutputList->rowCount();
			m_pOutputList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(oInfo.oName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pOutputList->setItem(row, 0, item);

			item = new QTableWidgetItem(oInfo.oDataType);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pOutputList->setItem(row, 1, item);
		}
	}
	else {
		//edit
		interfaceInfo iInfo = interfaceMap.value(preName);
		iInfo.dataType = oInfo.oDataType;
		if (preName != oInfo.oName) {
			interfaceMap.remove(preName);
		}
		interfaceMap.insert(oInfo.oName, iInfo);

		//成功
		QTableWidgetItem *item;

		item = new QTableWidgetItem(oInfo.oName);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pOutputList->setItem(m_pOutputList->currentItem()->row(), 0, item);

		item = new QTableWidgetItem(oInfo.oDataType);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pOutputList->setItem(m_pOutputList->currentItem()->row(), 1, item);
	}
}

void InterfaceInfoStack::slotModelChange(QString modelName) {
	if (selectedModel == modelName) {
		return;
	}

	//有可能是NULL
	selectedModel = modelName;

	m_pOutputList->setRowCount(0);
	m_pOutputList->clearContents();

	m_pInputList->setRowCount(0);
	m_pInputList->clearContents();

	if (selectedModel != NULL) {
		QMap<QString, interfaceInfo>::iterator iter;
		for (iter = interfaceMap.begin(); iter != interfaceMap.end(); ++iter) {
			//输出表
			if (iter.value().publisher == modelName) {
				int row = m_pOutputList->rowCount();
				m_pOutputList->insertRow(row);

				QTableWidgetItem *item;

				item = new QTableWidgetItem(iter.key());
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pOutputList->setItem(row, 0, item);

				item = new QTableWidgetItem(iter.value().dataType);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pOutputList->setItem(row, 1, item);
			}
		}
	}
}

void InterfaceInfoStack::slotMessageFromDialog(QString message) {
	//
}