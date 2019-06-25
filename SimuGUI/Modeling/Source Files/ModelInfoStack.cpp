
#include "ModelInfoStack.h"

ModelInfoStack::ModelInfoStack(QWidget *p) : MiniStack(p) {

	m_pModelList = new QTableWidget();

	//����
	m_pModelList->setColumnCount(3);
	m_pModelList->setColumnWidth(0, 150);
	m_pModelList->setColumnWidth(1, 150);
	m_pModelList->setColumnWidth(2, 170);
	QStringList header;
	header << "Name" << "Tool" << "Notice";
	m_pModelList->setHorizontalHeaderLabels(header);
	tableStandardize(m_pModelList);

	FancyButton *editButton = new FancyButton();
	editButton->setIcon(QIcon(iconPath + "function/edit"));
	editButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editButton, SIGNAL(clicked()), this, SLOT(pSlotEditModel()));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon(iconPath + "function/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDeleteModel()));

	QWidget *buttons = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(editButton);
	layout->addWidget(deleteButton);
	buttons->setLayout(layout);

	addTab(tr("modelList"), m_pModelList, buttons);

	//˽�л�ȡģ���б�ı�
	connect(m_pModelList, SIGNAL(itemSelectionChanged()),
		this, SLOT(pSlotModelItemChanged()));
}

void ModelInfoStack::slotAddModel(QString modelName, QString modelType) {
	int row = m_pModelList->rowCount();
	m_pModelList->insertRow(row);

	QTableWidgetItem *item;
	item = new QTableWidgetItem(modelName);
	//���ÿ�ѡ�񵫲��ɱ༭
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	m_pModelList->setItem(row, 0, item);

	item = new QTableWidgetItem(modelType);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));
	m_pModelList->setItem(row, 1, item);

	m_pModelList->selectRow(row);
}

void ModelInfoStack::slotModelChange(QString modelName) {
	if (modelName == NULL) {
		m_pModelList->clearSelection();
		emit signalModelChange(NULL);
		return;
	}
	else {
		for (int i = 0; i < m_pModelList->rowCount(); ++i) {
			if (m_pModelList->item(i, 0)->text() == modelName) {
				m_pModelList->selectRow(i);
				return;
			}
		}
	}
}

void ModelInfoStack::pSlotModelItemChanged() {
	if (!m_pModelList->currentItem()) {
		return;
	}
	if (m_pModelList->currentItem()->isSelected()) {
		int row = m_pModelList->currentItem()->row();
		emit signalModelChange(m_pModelList->item(row, 0)->text());
	}
}

void ModelInfoStack::pSlotEditModel() {
	//TODO:�༭���ܲ�����
}

void ModelInfoStack::pSlotDeleteModel() {
	//TODO:ɾ�����ܲ����ã���Ҫ��usage
	/*
	if (!m_pModelList->currentItem()) {
		return;
	}
	if (m_pModelList->currentItem()->isSelected()) {
		int row = m_pModelList->currentItem()->row();
		emit signalDeleteModel(m_pModelList->item(row, 0)->text());
		m_pModelList->removeRow(row);
	}
	*/
}