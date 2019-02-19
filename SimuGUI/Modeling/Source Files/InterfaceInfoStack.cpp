

#include "InterfaceInfoStack.h"

InterfaceInfoStack::InterfaceInfoStack(QWidget *p) : MiniStack(p) {

	//�����
	m_pOutputList = new QTableWidget();

	//��ͷ
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

	//��ͷ
	m_pOutputList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pOutputList->verticalHeader()->setVisible(false);

	//ѡ��ģʽ
	m_pOutputList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pOutputList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pOutputList->setStyleSheet("selection-background-color:lightblue;");

	//������
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

	//�����
	m_pInputList = new QTableWidget();

	//��ͷ
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

	//��ͷ
	m_pInputList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pInputList->verticalHeader()->setVisible(false);

	//ѡ��ģʽ
	m_pInputList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pInputList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pInputList->setStyleSheet("selection-background-color:lightblue;");

	//������
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
		interfaceInfo iInfo;
		iInfo.publisher = selectedModel;
		dialog = new EditOutputDialog(iInfo);
		connect(dialog, SIGNAL(nameCheck(bool, int, QString)),
			this, SLOT(slotNameCheck(bool, int, QString)));
		connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
		connect(dialog, SIGNAL(refreshOutput(bool, int, interfaceInfo)),
			this, SLOT(slotRefreshOutput(bool, int, interfaceInfo)));
		//������Ϣ
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
	
}
void InterfaceInfoStack::pSlotDeleteOutput() {}
void InterfaceInfoStack::pSlotSubscribeInput() {}
void InterfaceInfoStack::pSlotDeleteInput() {}


void InterfaceInfoStack::slotNameCheck(bool isAdd, int index, QString name) {
	if (isAdd) {
		for (interfaceInfo i : interfaceSet) {
			if (i.iName == name) {
				QMessageBox::information(ERROR, "error", "name already used",
					QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
		}
		//ͨ�����Ƽ��
		emit nameValid();
	}
}

void InterfaceInfoStack::slotRefreshOutput(bool isAdd, int index, interfaceInfo iInfo) {
	if (isAdd) {
		//������������򲻻���룬��second = false
		auto ret = interfaceSet.insert(iInfo);
		if (ret.second == 0) {
			//Ĭ�ϲ�����ֵ����
			QMessageBox::information(ERROR, "error", "name valid but can not insert",
				QMessageBox::Yes, QMessageBox::Yes);
		}
		else {
			//�ɹ�
			int row = m_pOutputList->rowCount();
			m_pOutputList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(iInfo.iName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pOutputList->setItem(row, 0, item);

			item = new QTableWidgetItem(iInfo.iDataType);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pOutputList->setItem(row, 1, item);
		}
	}
}

void InterfaceInfoStack::slotModelChange(QString modelName) {
	if (selectedModel == modelName) {
		return;
	}

	//�п�����NULL
	selectedModel = modelName;

	m_pOutputList->setRowCount(0);
	m_pOutputList->clearContents();

	m_pInputList->setRowCount(0);
	m_pInputList->clearContents();

	if (selectedModel != NULL) {
		for (interfaceInfo i : interfaceSet) {
			//�����
			if (i.publisher == modelName) {
				int row = m_pOutputList->rowCount();
				m_pOutputList->insertRow(row);

				QTableWidgetItem *item;

				item = new QTableWidgetItem(i.iName);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pOutputList->setItem(row, 0, item);

				item = new QTableWidgetItem(i.iDataType);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pOutputList->setItem(row, 1, item);
			}
			//�����
		}
	}
}

void InterfaceInfoStack::slotMessageFromDialog(QString message) {
	//
}