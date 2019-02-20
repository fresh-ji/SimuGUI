
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
	//�����Զ�������
	layout->addWidget(typeBox, 1, 1, 1, 1);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 2, 0, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close���Զ���������ֹ֮ǰ��dialog���ź�
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditOutputDialog::pSlotFinish() {
	QString newName = nameText->text();
	//�����淶
	if (newName != NULL && !(newName.contains(" "))) {
		//�Ƿ��޸�key
		if (newName == preName) {
			//û����
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
	//�����ҿ���
	outputInfo oInfo;
	oInfo.oName = nameText->text();
	oInfo.oDataType = typeBox->currentText();
	oInfo.oPublisher = publisher;
	close();
	emit refreshOutput(isAdd, preName, oInfo);
}