
/*
* @date : 2019/02/14
* @author : jihang
*/

#include "DataTypeStack.h"

DataTypeStack::DataTypeStack(QWidget *p) : MiniStack(p) {

	m_pDataTypeList = new QTableWidget();

	//����
	m_pDataTypeList->setColumnCount(2);
	m_pDataTypeList->setColumnWidth(0, 95);
	m_pDataTypeList->setColumnWidth(1, 95);
	QStringList header;
	header << "Name" << "Notice";
	m_pDataTypeList->setHorizontalHeaderLabels(header);
	tableStandardize(m_pDataTypeList);

	FancyButton *addButton = new FancyButton();
	addButton->setIcon(QIcon("./Icon/function/add"));
	addButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addButton, SIGNAL(clicked()), this, SLOT(pSlotAddDataType()));

	FancyButton *editButton = new FancyButton();
	editButton->setIcon(QIcon("./Icon/function/edit"));
	editButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editButton, SIGNAL(clicked()), this, SLOT(pSlotEditDataType()));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/function/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteDataType()));

	QWidget *buttons = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(addButton);
	layout->addWidget(editButton);
	layout->addWidget(deleteButton);
	buttons->setLayout(layout);

	addTab(tr("dataTypeList"), m_pDataTypeList, buttons);

	setFixedWidth(200);
}

void DataTypeStack::pSlotAddDataType() {
	dialog = new EditDataTypeDialog(dataTypeMap.keys());
	connect(dialog, SIGNAL(nameCheck(QString)),
		this, SLOT(slotNameCheck(QString)));
	connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
	connect(dialog, SIGNAL(refreshDataType(bool, QString, QString, dataTypeInfo)),
		this, SLOT(slotRefreshDataType(bool, QString, QString, dataTypeInfo)));
	//������Ϣ
	//connect(dialog, SIGNAL(signalSendMessage(QString)),
	//	this, SLOT(slotMessageFromDialog(QString)));
	dialog->exec();
}

void DataTypeStack::pSlotEditDataType() {

	if (!m_pDataTypeList->currentItem()) {
		return;
	}
	if (m_pDataTypeList->currentItem()->isSelected()) {
		int row = m_pDataTypeList->currentItem()->row();
		QString name = m_pDataTypeList->item(row, 0)->text();

		dialog = new EditDataTypeDialog(dataTypeMap.keys(), name, dataTypeMap.value(name));
		connect(dialog, SIGNAL(nameCheck(QString)),
			this, SLOT(slotNameCheck(QString)));
		connect(this, SIGNAL(nameValid()), dialog, SLOT(slotNameValid()));
		connect(dialog, SIGNAL(refreshDataType(bool, QString, QString, dataTypeInfo)),
			this, SLOT(slotRefreshDataType(bool, QString, QString, dataTypeInfo)));
		//������Ϣ
		//connect(dialog, SIGNAL(signalSendMessage(QString)),
		//	this, SLOT(slotMessageFromDialog(QString)));
		dialog->exec();
	}
}

void DataTypeStack::pSlotDeleteDataType() {
	//TODO:��ɾ�����ܣ�Ҫ��usage
	/*
	if (!m_pDataTypeList->currentItem()) {
		return;
	}
	if (m_pDataTypeList->currentItem()->isSelected()) {
		int row = m_pDataTypeList->currentItem()->row();
		QString name = m_pDataTypeList->item(row, 0)->text();
		//��map��ɾ��
		dataTypeMap.remove(name);
		//���б���ɾ��
		m_pDataTypeList->removeRow(row);
	}
	*/
}

void DataTypeStack::slotNameCheck(QString newName) {
	//�������ӻ����޸ģ�nameһ�����µ�
	if (dataTypeMap.contains(newName)) {
		QMessageBox::information(ERROR, "error", "name already used",
			QMessageBox::Yes, QMessageBox::Yes);
	}
	else {
		//ͨ�����Ƽ��
		emit nameValid();
	}
}

void DataTypeStack::slotRefreshDataType(bool isAdd, QString preName, QString newName, dataTypeInfo dInfo) {

	if (isAdd) {
		if (dataTypeMap.contains(newName)) {
			//Ĭ�ϲ�����ֵ����
			QMessageBox::information(ERROR, "error", "name valid but can not insert",
				QMessageBox::Yes, QMessageBox::Yes);
		}
		else {
			//���map
			dataTypeMap.insert(newName, dInfo);
			//����б�
			int row = m_pDataTypeList->rowCount();
			m_pDataTypeList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(newName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			m_pDataTypeList->setItem(row, 0, item);

			//����interface���
			emit refreshTypeSet(newName, newName);
		}
	}
	else {
		//edit
		if (preName != newName) {
			dataTypeMap.remove(preName);
			//����interface�޸� 
			emit refreshTypeSet(preName, newName);
		}
		dataTypeMap.insert(newName, dInfo);

		//�ɹ�
		QTableWidgetItem *item;

		item = new QTableWidgetItem(newName);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		m_pDataTypeList->setItem(m_pDataTypeList->currentItem()->row(), 0, item);
	}
}

void DataTypeStack::slotMessageFromDialog(QString message) {
	//
}