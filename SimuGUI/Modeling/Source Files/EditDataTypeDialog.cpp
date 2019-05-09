
/*
* @date : 2019/02/16
* @author : jihang
*/

#include "EditDataTypeDialog.h"

EditDataTypeDialog::EditDataTypeDialog(QList<QString> currentTypes,
	QWidget *p) : FancyDialog(p) {

	isAdd = true;
	preName = "";
	setWindowTitle("Add Data Type");
	createTemplate(currentTypes);
}

EditDataTypeDialog::EditDataTypeDialog(
	QList<QString> currentTypes,
	QString key,
	dataTypeInfo dInfo,
	QWidget *p) : FancyDialog(p) {

	isAdd = false;
	preName = key;
	setWindowTitle("Edit Data Type : " + key);
	currentTypes.removeOne(key);
	createTemplate(currentTypes);

	nameText->setText(key);
	this->dInfo = dInfo;
	for (QString s : dInfo.dataMap.keys()) {
		int row = dataDetailList->rowCount();
		dataDetailList->insertRow(row);

		QTableWidgetItem *item;

		item = new QTableWidgetItem(s);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		dataDetailList->setItem(row, 0, item);

		item = new QTableWidgetItem(dInfo.dataMap.value(s));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));
		dataDetailList->setItem(row, 1, item);
	}
}

void EditDataTypeDialog::createTemplate(QList<QString> currentTypes) {

	setWindowIcon(QIcon("./Icon/function/window"));
	setFixedSize(600, 400);
	setStyleSheet("background: #1890FF;");

	QFont font("Microsoft YaHei", 13, 50);

	QWidget *centerWidget = new QWidget();
	centerWidget->setStyleSheet("background: #E7F7FE");
	QGridLayout *layout = new QGridLayout();

	//���Ʋ���
	QLabel *nameLabel = new QLabel();
	nameLabel->setText("name:");
	nameLabel->setFont(font);
	layout->addWidget(nameLabel, 0, 0, 1, 1);
	nameText = new QLineEdit();
	nameText->setFixedHeight(50);
	nameText->setFont(*(new QFont("Microsoft YaHei", 16)));
	layout->addWidget(nameText, 0, 1, 1, 2);

	//������
	QLabel *addItemLabel = new QLabel();
	addItemLabel->setText("Item:");
	addItemLabel->setFont(font);
	addItemLabel->setFixedHeight(40);
	layout->addWidget(addItemLabel, 1, 0, 1, 1);
	itemContent = new QLineEdit();
	layout->addWidget(itemContent, 1, 1, 1, 1);

	QLabel *addTypeLabel = new QLabel();
	addTypeLabel->setText("Type:");
	addTypeLabel->setFont(font);
	addTypeLabel->setFixedHeight(40);
	layout->addWidget(addTypeLabel, 2, 0, 1, 1);
	typeContent = new QComboBox();
	typeContent->addItems(basicTypes);
	typeContent->addItems(currentTypes);
	layout->addWidget(typeContent, 2, 1, 1, 1);

	FancyButton *addButton = new FancyButton();
	addButton->setIcon(QIcon("./Icon/function/add"));
	addButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(addButton, SIGNAL(clicked()), this, SLOT(pSlotAdd()));
	addButton->setFixedWidth(40);
	layout->addWidget(addButton, 3, 0, 1, 1);

	FancyButton *deleteButton = new FancyButton();
	deleteButton->setIcon(QIcon("./Icon/function/delete"));
	deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(pSlotDelete()));
	deleteButton->setFixedWidth(40);
	layout->addWidget(deleteButton, 3, 1, 1, 1);

	//�б���
	dataDetailList = new QTableWidget();
	//��ͷ
	dataDetailList->setColumnCount(2);
	dataDetailList->horizontalHeader()->setSectionsClickable(false);
	dataDetailList->setColumnWidth(0, 130);
	dataDetailList->setColumnWidth(1, 125);

	QFont font2;
	font2.setBold(true);
	dataDetailList->horizontalHeader()->setFont(font2);
	dataDetailList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#93D5FD;}");
	QStringList header;
	header << "Item" << "Type";
	dataDetailList->setHorizontalHeaderLabels(header);

	//��ͷ
	dataDetailList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	dataDetailList->verticalHeader()->setVisible(false);

	//ѡ��ģʽ
	dataDetailList->setSelectionBehavior(QAbstractItemView::SelectRows);
	dataDetailList->setSelectionMode(QAbstractItemView::SingleSelection);
	dataDetailList->setStyleSheet("selection-background-color:gray;");

	//������
	dataDetailList->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	dataDetailList->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	layout->addWidget(dataDetailList, 1, 2, 8, 1);

	//���鲿��
	QLabel *noticeLabel = new QLabel();
	noticeLabel->setText("Notice:");
	noticeLabel->setFont(font);
	noticeLabel->setFixedHeight(50);
	layout->addWidget(noticeLabel, 9, 0, 1, 1);
	noticeContent = new QTextEdit();
	noticeContent->setFixedHeight(50);
	noticeContent->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	layout->addWidget(noticeContent, 9, 1, 1, 2);

	//ȷ������
	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("Finish", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 10, 1, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close���Զ���������ֹ֮ǰ��dialog���ź�
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditDataTypeDialog::pSlotAdd() {
	QString newName = itemContent->text();
	//�����淶
	if (newName != NULL && !(newName.contains(" ")) && !(forbidTypes.contains(newName))) {
		if (!(dInfo.dataMap.contains(newName))) {
			//����map
			dInfo.dataMap.insert(newName, typeContent->currentText());
			//������
			int row = dataDetailList->rowCount();
			dataDetailList->insertRow(row);

			QTableWidgetItem *item;

			item = new QTableWidgetItem(newName);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			dataDetailList->setItem(row, 0, item);

			item = new QTableWidgetItem(typeContent->currentText());
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			dataDetailList->setItem(row, 1, item);
			//���
			itemContent->clear();
		}
		else {
			QMessageBox::information(ERROR, "error", "item already exists",
				QMessageBox::Yes, QMessageBox::Yes);
		}
	}
	else {
		QMessageBox::information(ERROR, "error", "item violation",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void EditDataTypeDialog::pSlotDelete() {
	if (!dataDetailList->currentItem()) {
		return;
	}
	if (dataDetailList->currentItem()->isSelected()) {
		int row = dataDetailList->currentItem()->row();
		QString name = dataDetailList->item(row, 0)->text();
		//��map��ɾ��
		dInfo.dataMap.remove(name);
		//���б���ɾ��
		dataDetailList->removeRow(row);
	}
}

void EditDataTypeDialog::pSlotFinish() {
	QString newName = nameText->text();
	//�����淶
	if (newName != NULL && !(newName.contains(" "))) {
		//�Ƿ��޸�key
		if (newName == preName) {
			//û����
			close();
			emit refreshDataType(isAdd, preName, newName, dInfo);
		}
		else {
			emit nameCheck(newName);
		}
	}
	else {
		QMessageBox::information(ERROR, "error", "struct name contains wrong charactors",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void EditDataTypeDialog::slotNameValid() {
	//�����ҿ���
	close();
	emit refreshDataType(isAdd, preName, nameText->text(), dInfo);
}