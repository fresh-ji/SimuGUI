
/*
* @date : 2018/01/05
* @author : jihang
*/

#include "Modeling.h"

Modeling::Modeling(QWidget *parent) : IMode(parent) {
	setObjectName(QLatin1String("Modeling"));
	setDisplayName(tr("Modeling"));
	setIcon(QIcon("./Icon/mode/model"));

	createWindow();
}

void Modeling::createWindow() {

	QWidget *widget = new QWidget();

	QLabel *title = new QLabel();
	title->setText("Modeling");
	QFont font("Microsoft YaHei", 20, 75);
	title->setFont(font);

	createToolStack();
	createModelStack();
	createModelList();
	createInterfaceList();

	//接收添加模型
	connect(m_pModelLabel, SIGNAL(signalAddModel(QString, QString)),
		this, SLOT(slotAddModel(QString, QString)));
	//发送删除模型
	connect(this, SIGNAL(signalDeleteModel(QString)),
		m_pModelLabel, SLOT(slotDeleteModel(QString)));
	//接收选择项改变
	connect(m_pModelLabel, SIGNAL(signalModelChange(QString)),
		this, SLOT(slotModelChange(QString)));
	//发送列表项改变
	connect(this, SIGNAL(signalModelChange(QString)),
		m_pModelLabel, SLOT(slotModelChange(QString)));
	//接收消息发送
	connect(m_pModelLabel, SIGNAL(signalSendMessage(QString)),
		this, SLOT(slotReceiveMessage(QString)));

	//私有获取模型列表改变
	connect(m_pModelList, SIGNAL(itemSelectionChanged()),
		this, SLOT(pSlotModelItemChanged()));

	QGridLayout *layout = new QGridLayout();
	layout->setMargin(10);
	layout->setSpacing(10);
	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 9);
	layout->addWidget(title, 0, 0, 1, 3, Qt::AlignCenter);
	layout->addWidget(m_pToolStack, 1, 0, 2, 1);
	layout->addWidget(m_pModelLabel, 1, 1, 2, 1);
	layout->addWidget(m_pModelList, 1, 2, 1, 1);
	layout->addWidget(m_pInterfaceList, 2, 2, 1, 1);

	widget->setLayout(layout);
	setWidget(widget);
}

void Modeling::createToolStack() {
	m_pToolStack = new ToolDragStack();
	m_pToolStack->setMinimumWidth(100);
	m_pToolStack->setTitle("tool");
}

void Modeling::createModelStack() {
	m_pModelLabel = new DropLabel();
	m_pModelLabel->setStyleSheet("border:2px solid black;");
	m_pModelLabel->setAcceptDrops(true);
}

void Modeling::createModelList() {
	m_pModelList = new QTableWidget();

	//表头
	m_pModelList->setColumnCount(3);
	m_pModelList->horizontalHeader()->setSectionsClickable(false);
	m_pModelList->setColumnWidth(0, 130);
	m_pModelList->setColumnWidth(1, 60);
	m_pModelList->setColumnWidth(2, 60);
	QFont font;
	font.setBold(true);
	m_pModelList->horizontalHeader()->setFont(font);
	m_pModelList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header;
	header << "Name" << "Type" << "Delete";
	m_pModelList->setHorizontalHeaderLabels(header);

	//列头
	m_pModelList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pModelList->verticalHeader()->setVisible(false);

	//选择模式
	m_pModelList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pModelList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pModelList->setStyleSheet("selection-background-color:lightblue;");

	//滚动条
	m_pModelList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	m_pModelList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
}

void Modeling::createInterfaceList() {
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
}

void Modeling::slotAddModel(QString modelName, QString modelType) {
	int row = m_pModelList->rowCount();
	m_pModelList->insertRow(row);
	m_pModelList->setItem(row, 0, new QTableWidgetItem(modelName));
	m_pModelList->setItem(row, 1, new QTableWidgetItem(modelType));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/tools/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteOne()));
	m_pModelList->setCellWidget(row, 2, deleteButton);

	m_pModelList->selectRow(row);
}

void Modeling::slotModelChange(QString modelName) {
	if (modelName == NULL) {
		m_pModelList->clearSelection();
		return;
	}
	else {
		QList<QTableWidgetItem*> items = m_pModelList->findItems(modelName, Qt::MatchExactly);
		for (QTableWidgetItem* item : items) {
			if (item->column() != 0) {
				continue;
			}
			//默认第0列不重复
			m_pModelList->selectRow(item->row());
			return;
		}
	}
	//TODO:error机制
}

void Modeling::slotReceiveMessage(QString info) {
	//TODO:info机制
	int row = m_pModelList->rowCount();
	m_pModelList->insertRow(row);
	m_pModelList->setItem(row, 0, new QTableWidgetItem(info));
}

void Modeling::pSlotDeleteOne() {
	FancyButton *senderObj = qobject_cast<FancyButton*>(sender());
	if (senderObj == 0) {
		return;
	}
	QModelIndex index = m_pModelList->indexAt(
		QPoint(senderObj->frameGeometry().x(), senderObj->frameGeometry().y()));
	int row = index.row();
	emit signalDeleteModel(m_pModelList->item(row, 0)->text());
	m_pModelList->removeRow(row);
}

void Modeling::pSlotModelItemChanged() {
	if (m_pModelList->currentItem()->isSelected()) {
		int row = m_pModelList->currentItem()->row();
		emit signalModelChange(m_pModelList->item(row, 0)->text());
	}
}