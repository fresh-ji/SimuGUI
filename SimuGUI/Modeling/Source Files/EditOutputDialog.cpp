
/*
* @date : 2018/02/18
* @author : jihang
*/

#include "EditOutputDialog.h"

EditOutputDialog::EditOutputDialog(interfaceInfo iInfo, QWidget *p) : FancyDialog(p) {
	isAdd = true;
	setIndex = -1;
	setWindowTitle("Add Output for " + iInfo.publisher);
	publisher = iInfo.publisher;
	createTemplate();
}

EditOutputDialog::EditOutputDialog(interfaceInfo iInfo, int index, QWidget *p) : FancyDialog(p) {
	isAdd = false;
	setIndex = index;
	setWindowTitle("Edit Output for " + iInfo.publisher);
	publisher = iInfo.publisher;
	createTemplate();

	nameText->setText(iInfo.iName);

	//设置类型
}

void EditOutputDialog::createTemplate() {

	setWindowIcon(QIcon("./Icon/tools/interface"));
	setFixedSize(400, 150);

	QFont font("Microsoft YaHei", 13, 50);

	QWidget *centerWidget = new QWidget();
	QGridLayout *layout = new QGridLayout();

	QLabel *nameLabel = new QLabel();
	nameLabel->setText("Name:");
	nameLabel->setFont(font);
	layout->addWidget(nameLabel, 0, 0, 1, 1);

	nameText = new QLineEdit();
	layout->addWidget(nameText, 0, 1, 1, 1);

	QLabel *dataType = new QLabel();
	dataType->setText("Type:");
	dataType->setFont(font);
	layout->addWidget(dataType, 1, 0, 1, 1);

	typeBox = new QComboBox();
	typeBox->addItem("real");
	typeBox->addItem("int");
	typeBox->addItem("bool");
	typeBox->addItem("string");
	typeBox->addItem("enum");
	//增加自定义类型
	layout->addWidget(typeBox, 1, 1, 1, 1);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 2, 0, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close后自动析构，防止之前的dialog发信号
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditOutputDialog::pSlotFinish() {
	if (nameText->text() != NULL && !(nameText->text().contains(" "))) {
		emit nameCheck(isAdd, setIndex, nameText->text());
	}
	else {
		QMessageBox::information(ERROR, "error", "contains wrong charactors",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void EditOutputDialog::slotNameValid() {
	interfaceInfo iInfo;
	iInfo.iName = nameText->text();
	iInfo.iDataType = typeBox->currentText();
	iInfo.publisher = publisher;
	close();
	emit refreshOutput(isAdd, setIndex, iInfo);
}