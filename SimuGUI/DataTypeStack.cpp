
#include "DataTypeStack.h"

DataTypeStack::DataTypeStack(QWidget *p) : MiniStack(p) {

	m_pDataTypeList = new QTableWidget();

	//表头
	m_pDataTypeList->setColumnCount(2);
	m_pDataTypeList->horizontalHeader()->setSectionsClickable(false);
	m_pDataTypeList->setColumnWidth(0, 200);
	m_pDataTypeList->setColumnWidth(1, 20);

	QFont font;
	font.setBold(true);
	m_pDataTypeList->horizontalHeader()->setFont(font);
	m_pDataTypeList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header;
	header << "Name" << "-";
	m_pDataTypeList->setHorizontalHeaderLabels(header);

	//列头
	m_pDataTypeList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pDataTypeList->verticalHeader()->setVisible(false);

	//选择模式
	m_pDataTypeList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pDataTypeList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pDataTypeList->setStyleSheet("selection-background-color:lightblue;");

	//滚动条
	m_pDataTypeList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	m_pDataTypeList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	FancyButton *addButton = new FancyButton();
	addButton->setIcon(QIcon("./Icon/tools/add"));
	addButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addButton, SIGNAL(clicked()), this, SLOT(pSlotAddDataType()));

	FancyButton *editButton = new FancyButton();
	editButton->setIcon(QIcon("./Icon/tools/edit"));
	editButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editButton, SIGNAL(clicked()), this, SLOT(pSlotEditDataType()));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/tools/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteDataType()));

	QWidget *buttons = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(addButton);
	layout->addWidget(editButton);
	layout->addWidget(deleteButton);
	buttons->setLayout(layout);

	addTab(tr("dataTypeList"), m_pDataTypeList, buttons);

	this->setFixedWidth(200);
}

void DataTypeStack::pSlotAddDataType() {
	dialog = new EditDataTypeDialog();
	connect(dialog, SIGNAL(nameCheck(bool, int, QString)),
		this, SLOT(slotNameCheck(bool, int, QString)));
	connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
	connect(dialog, SIGNAL(refreshDataType(bool, int, dataTypeInfo)),
		this, SLOT(slotRefreshDataType(bool, int, dataTypeInfo)));
	//暂无消息
	//connect(dialog, SIGNAL(signalSendMessage(QString)),
	//	this, SLOT(slotMessageFromDialog(QString)));
	dialog->exec();
}

void DataTypeStack::pSlotEditDataType() {

	QString name;
	if (!m_pDataTypeList->currentItem()) {
		return;
	}
	if (m_pDataTypeList->currentItem()->isSelected()) {
		int row = m_pDataTypeList->currentItem()->row();
		name = m_pDataTypeList->item(row, 0)->text();
	}

	int index = 0;
	for (dataTypeInfo d : dataTypeSet) {
		if (d.dName == name) {
			dialog = new EditDataTypeDialog(d, index);
			connect(dialog, SIGNAL(nameCheck(bool, int, QString)),
				this, SLOT(slotNameCheck(bool, int, QString)));
			connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
			connect(dialog, SIGNAL(refreshDataType(bool, int, dataTypeInfo)),
				this, SLOT(slotRefreshDataType(bool, int, dataTypeInfo)));
			//暂无消息
			//connect(dialog, SIGNAL(signalSendMessage(QString)),
			//	this, SLOT(slotMessageFromDialog(QString)));
			dialog->exec();
			return;
		}
		index++;
	}
}

void DataTypeStack::pSlotDeleteDataType() {
	if (!m_pDataTypeList->currentItem()) {
		return;
	}
	if (m_pDataTypeList->currentItem()->isSelected()) {
		int row = m_pDataTypeList->currentItem()->row();
		QString name = m_pDataTypeList->item(row, 0)->text();
		//在set中删除
		for (dataTypeInfo d : dataTypeSet) {
			if (d.dName == name) {
				dataTypeSet.erase(d);
				break;
			}
		}
		//在列表中删除
		m_pDataTypeList->removeRow(row);
	}
}

void DataTypeStack::slotNameCheck(bool isAdd, int index, QString name) {
	if (isAdd) {
		for (dataTypeInfo d : dataTypeSet) {
			if (d.dName == name) {
				QMessageBox::information(ERROR, "error", "name already used",
					QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
		}
		//通过名称检测
		emit nameValid();
	}
	else {
		int i = 0;
		for (dataTypeInfo d : dataTypeSet) {
			if (index == i) {
				i++;
				continue;
			}
			if (d.dName == name) {
				QMessageBox::information(ERROR, "error", "name already used",
					QMessageBox::Yes, QMessageBox::Yes);
				return;
			}
			i++;
		}
		//通过名称检测
		emit nameValid();
	}
}

void DataTypeStack::slotRefreshDataType(bool isAdd, int index, dataTypeInfo dInfo) {

	if (isAdd) {
		//如果主键存在则不会插入，且second = false
		auto ret = dataTypeSet.insert(dInfo);
		if (ret.second == 0) {
			//默认不会出现的情况
			QMessageBox::information(ERROR, "error", "name valid but can not insert",
				QMessageBox::Yes, QMessageBox::Yes);
		}
		else {
			//添加成功
			int row = m_pDataTypeList->rowCount();
			m_pDataTypeList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(dInfo.dName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pDataTypeList->setItem(row, 0, item);
		}
	}
	else {
		//edit
		int i = 0;
		for (dataTypeInfo d : dataTypeSet) {
			if (index == i) {
				dataTypeSet.erase(d);
				//如果主键存在则不会插入，且second = false
				auto ret = dataTypeSet.insert(dInfo);
				if (ret.second == 0) {
					//默认不会出现的情况
					QMessageBox::information(ERROR, "error", "name valid but can not insert in edit",
						QMessageBox::Yes, QMessageBox::Yes);
				}
				else {
					//成功
					QTableWidgetItem *item;
					item = new QTableWidgetItem(dInfo.dName);
					item->setFlags(item->flags() & (~Qt::ItemIsEditable));
					m_pDataTypeList->setItem(m_pDataTypeList->currentItem()->row(), 0, item);
				}
				return;
			}
			i++;
		}
	}
}

void DataTypeStack::slotMessageFromDialog(QString message) {
	//
}