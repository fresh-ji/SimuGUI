
/*
* @date : 2018/02/18
* @author : jihang
*/

#include "EditOutputDialog.h"

EditOutputDialog::EditOutputDialog(QString pub, QWidget *p) : FancyDialog(p) {
	isAdd = true;
	preName = "";
	setWindowTitle("Add Output for " + pub);
	publisher = pub;
	createTemplate();
}

EditOutputDialog::EditOutputDialog(outputInfo oInfo, QWidget *p) : FancyDialog(p) {
	isAdd = false;
	preName = oInfo.oName;
	setWindowTitle("Edit Output for " + oInfo.oPublisher);
	publisher = oInfo.oPublisher;
	createTemplate();

	nameText->setText(oInfo.oName);
	typeBox->setCurrentText(oInfo.oDataType);
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
	QString newName = nameText->text();
	//命名规范
	if (newName != NULL && !(newName.contains(" "))) {
		//是否修改key
		if (newName == preName) {
			//没改名
			outputInfo oInfo;
			oInfo.oName = newName;
			oInfo.oDataType = typeBox->currentText();
			oInfo.oPublisher = publisher;
			close();
			emit refreshOutput(isAdd, preName, oInfo);
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

void EditOutputDialog::slotNameValid() {
	//改名且可用
	outputInfo oInfo;
	oInfo.oName = nameText->text();
	oInfo.oDataType = typeBox->currentText();
	oInfo.oPublisher = publisher;
	close();
	emit refreshOutput(isAdd, preName, oInfo);
}