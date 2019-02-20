
/*
* @date : 2018/02/16
* @author : jihang
*/

#include "EditDataTypeDialog.h"

EditDataTypeDialog::EditDataTypeDialog(QWidget *p) : FancyDialog(p) {

	isAdd = true;
	preName = "";
	setWindowTitle("Add Data Type");
	createTemplate();

}

EditDataTypeDialog::EditDataTypeDialog(QString key, dataTypeInfo dInfo, QWidget *p) : FancyDialog(p) {

	isAdd = false;
	preName = key;
	setWindowTitle("Edit Data Type : " + key);
	createTemplate();

	nameText->setText(key);

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
	QString newName = nameText->text();
	//命名规范
	if (newName != NULL && !(newName.contains(" "))) {
		//是否修改key
		if (newName == preName) {
			//没改名
			dataTypeInfo dInfo;
			close();
			emit refreshDataType(isAdd, preName, newName, dInfo);
		}
		else {
			emit nameCheck(newName);
		}
	}
	else {
		QMessageBox::information(ERROR, "error", "contains wrong charactors",
			QMessageBox::Yes, QMessageBox::Yes);
	}
}

void EditDataTypeDialog::slotNameValid() {
	//改名且可用
	dataTypeInfo dInfo;
	close();
	emit refreshDataType(isAdd, preName, nameText->text(), dInfo);
}