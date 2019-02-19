
/*
* @date : 2018/02/16
* @author : jihang
*/

#include "EditDataTypeDialog.h"

EditDataTypeDialog::EditDataTypeDialog(QWidget *p) : FancyDialog(p) {

	isAdd = true;
	setIndex = -1;
	setWindowTitle("Add Data Type");
	createTemplate();

}

EditDataTypeDialog::EditDataTypeDialog(dataTypeInfo dInfo, int index, QWidget *p) : FancyDialog(p) {

	isAdd = false;
	setIndex = index;
	setWindowTitle("Edit Data Type : " + dInfo.dName);
	createTemplate();

	nameText->setText(dInfo.dName);

}

void EditDataTypeDialog::createTemplate() {

	setWindowIcon(QIcon("./Icon/tools/dataType"));
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

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 1, 0, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close后自动析构，防止之前的dialog发信号
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditDataTypeDialog::pSlotFinish() {
	if (nameText->text() != NULL && !(nameText->text().contains(" "))) {
		emit nameCheck(isAdd, setIndex, nameText->text());
	}
	else {
		QMessageBox::information(ERROR, "error", "contains wrong charactors",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void EditDataTypeDialog::slotNameValid() {
	dataTypeInfo dInfo;
	dInfo.dName = nameText->text();
	close();
	emit refreshDataType(isAdd, setIndex, dInfo);
}