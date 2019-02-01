
#include "ModelInfoStack.h"

ModelInfoStack::ModelInfoStack(QWidget *p) : MiniStack(p) {

	m_pModelList = new QTableWidget();
	//��ͷ
	m_pModelList->setColumnCount(2);
	m_pModelList->horizontalHeader()->setSectionsClickable(false);
	m_pModelList->setColumnWidth(0, 150);
	m_pModelList->setColumnWidth(1, 100);
	QFont font;
	font.setBold(true);
	m_pModelList->horizontalHeader()->setFont(font);
	m_pModelList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	QStringList header;
	header << "Name" << "Type";
	m_pModelList->setHorizontalHeaderLabels(header);

	//��ͷ
	m_pModelList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_pModelList->verticalHeader()->setVisible(false);

	//ѡ��ģʽ
	m_pModelList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pModelList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pModelList->setStyleSheet("selection-background-color:lightblue;");

	//������
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

	FancyButton *editButton = new FancyButton();
	editButton->setIcon(QIcon("./Icon/tools/edit"));
	editButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(editButton, SIGNAL(clicked()), this, SLOT(pSlotEditModel()));

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/tools/delete"));
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
		QList<QTableWidgetItem*> items = m_pModelList->findItems(modelName, Qt::MatchExactly);
		for (QTableWidgetItem* item : items) {
			if (item->column() != 0) {
				continue;
			}
			//Ĭ�ϵ�0�в��ظ�
			m_pModelList->selectRow(item->row());
			return;
		}
	}
	//TODO:error����
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

}

void ModelInfoStack::pSlotDeleteModel() {
	if (!m_pModelList->currentItem()) {
		return;
	}
	if (m_pModelList->currentItem()->isSelected()) {
		int row = m_pModelList->currentItem()->row();
		emit signalDeleteModel(m_pModelList->item(row, 0)->text());
		m_pModelList->removeRow(row);
	}
}