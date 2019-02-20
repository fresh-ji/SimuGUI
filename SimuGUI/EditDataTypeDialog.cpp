
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

	//close���Զ���������ֹ֮ǰ��dialog���ź�
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditDataTypeDialog::pSlotFinish() {
	QString newName = nameText->text();
	//�����淶
	if (newName != NULL && !(newName.contains(" "))) {
		//�Ƿ��޸�key
		if (newName == preName) {
			//û����
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
	//�����ҿ���
	dataTypeInfo dInfo;
	close();
	emit refreshDataType(isAdd, preName, nameText->text(), dInfo);
}