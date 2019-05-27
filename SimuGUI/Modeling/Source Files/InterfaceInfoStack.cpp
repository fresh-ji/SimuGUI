
/*
* @date : 2019/02/20
* @author : jihang
*/

#include "InterfaceInfoStack.h"

//QSet基于哈希表，set基于变种红黑树，还要加上==
uint qHash(const inputInfo i) {
	return i.iName.toUInt();
}

InterfaceInfoStack::InterfaceInfoStack(QWidget *p) : MiniStack(p) {

	//输出表
	m_pOutputList = new QTableWidget();

	//设置
	m_pOutputList->setColumnCount(3);
	m_pOutputList->setColumnWidth(0, 150);
	m_pOutputList->setColumnWidth(1, 150);
	m_pOutputList->setColumnWidth(2, 170);
	QStringList header;
	header << "Name" << "DataType" << "Notice";
	m_pOutputList->setHorizontalHeaderLabels(header);
	tableStandardize(m_pOutputList);

	FancyButton *addOutputButton = new FancyButton();
	addOutputButton->setIcon(QIcon("./Icon/function/add"));
	addOutputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addOutputButton, SIGNAL(clicked()), this, SLOT(pSlotAddOutput()));

	FancyButton *editOutputButton = new FancyButton();
	editOutputButton->setIcon(QIcon("./Icon/function/edit"));
	editOutputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editOutputButton, SIGNAL(clicked()), this, SLOT(pSlotEditOutput()));

	FancyButton *deleteOutputButton = new FancyButton();
	deleteOutputButton->setIcon(QIcon("./Icon/function/delete"));
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
	m_pInputList->setColumnWidth(0, 150);
	m_pInputList->setColumnWidth(1, 150);
	m_pInputList->setColumnWidth(2, 170);
	QStringList header2;
	header2 << "Name" << "DataType" << "Publisher";
	m_pInputList->setHorizontalHeaderLabels(header2);
	tableStandardize(m_pInputList);

	FancyButton *subscribeInputButton = new FancyButton();
	subscribeInputButton->setIcon(QIcon("./Icon/function/subscribe"));
	subscribeInputButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(subscribeInputButton, SIGNAL(clicked()), this, SLOT(pSlotSubscribeInput()));

	FancyButton *deleteInputButton = new FancyButton();
	deleteInputButton->setIcon(QIcon("./Icon/function/delete"));
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
		dialog = new EditOutputDialog(types, selectedModel);
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

		dialog = new EditOutputDialog(types, oInfo);
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
	//TODO:无删除功能，要加usage
	/*
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
	*/
}

void InterfaceInfoStack::pSlotSubscribeInput() {
	if (selectedModel != NULL) {
		QSet<inputInfo> subInfo;
		QMap<QString, interfaceInfo>::iterator iter;
		for (iter = interfaceMap.begin(); iter != interfaceMap.end(); ++iter) {
			inputInfo iInfo;
			iInfo.iName = iter.key();
			iInfo.iDataType = iter.value().dataType;
			iInfo.iPublisher = iter.value().publisher;
			if (iter.value().subscribers.contains(selectedModel)) {
				iInfo.isSubscribe = true;
			}
			else {
				iInfo.isSubscribe = false;
			}
			subInfo.insert(iInfo);
		}

		dialog_in = new EditInputDialog(selectedModel, subInfo);
		connect(dialog_in, SIGNAL(refreshInput(QSet<QString>, QSet<QString>)),
			this, SLOT(slotRefreshInput(QSet<QString>, QSet<QString>)));
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
	if (!m_pInputList->currentItem()) {
		return;
	}
	if (m_pInputList->currentItem()->isSelected()) {
		int row = m_pInputList->currentItem()->row();
		QString name = m_pInputList->item(row, 0)->text();
		//在map中删除
		interfaceInfo info = interfaceMap.value(name);
		info.subscribers.remove(selectedModel);
		interfaceMap.insert(name, info);
		//在列表中删除
		m_pInputList->removeRow(row);
	}
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

void InterfaceInfoStack::slotRefreshInput(QSet<QString> addSubSet, QSet<QString> cancelSubSet) {
	for (QString s : cancelSubSet) {
		interfaceInfo info = interfaceMap.value(s);
		info.subscribers.remove(selectedModel);
		interfaceMap.insert(s, info);
		for (int i = 0; i < m_pInputList->rowCount(); ++i) {
			if (m_pInputList->item(i, 0)->text() == s) {
				m_pInputList->removeRow(i);
				break;
			}
		}
	}
	for (QString s : addSubSet) {
		interfaceInfo info = interfaceMap.value(s);
		info.subscribers.insert(selectedModel);
		interfaceMap.insert(s, info);

		int row = m_pInputList->rowCount();
		m_pInputList->insertRow(row);

		QTableWidgetItem *item;

		item = new QTableWidgetItem(s);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pInputList->setItem(row, 0, item);

		item = new QTableWidgetItem(info.dataType);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pInputList->setItem(row, 1, item);

		item = new QTableWidgetItem(info.publisher);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pInputList->setItem(row, 2, item);
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
			//输入表
			if (iter.value().subscribers.contains(modelName)) {
				int row = m_pInputList->rowCount();
				m_pInputList->insertRow(row);

				QTableWidgetItem *item;

				item = new QTableWidgetItem(iter.key());
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInputList->setItem(row, 0, item);

				item = new QTableWidgetItem(iter.value().dataType);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInputList->setItem(row, 1, item);

				item = new QTableWidgetItem(iter.value().publisher);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				m_pInputList->setItem(row, 2, item);
			}
		}
	}
}

void InterfaceInfoStack::slotRefreshTypeSet(QString type1, QString type2) {
	//无论如何2都是新的
	types.append(type2);
	if (type1 != type2) {
		types.removeOne(type1);
		//改map
		for (QString s : interfaceMap.keys()) {
			if (type1 == interfaceMap.value(s).dataType) {
				interfaceInfo iInfo = interfaceMap.value(s);
				iInfo.dataType = type2;
				interfaceMap.insert(s, iInfo);
			}
		}
		//m_pOutputList
		for (int i = 0; i < m_pOutputList->rowCount(); ++i) {
			if (m_pOutputList->item(i, 1)->text() == type1) {
				m_pOutputList->item(i, 1)->setText(type2);
			}
		}
		//m_pInputList
		for (int i = 0; i < m_pInputList->rowCount(); ++i) {
			if (m_pInputList->item(i, 1)->text() == type1) {
				m_pInputList->item(i, 1)->setText(type2);
			}
		}
	}
}

void InterfaceInfoStack::slotMessageFromDialog(QString message) {
	//
}