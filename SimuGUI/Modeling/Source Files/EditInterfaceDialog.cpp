
#include "EditInterfaceDialog.h"

EditInterfaceDialog::EditInterfaceDialog(
	interfaceInfo iInfo,
	bool isAdd,
	QWidget *p) : FancyDialog(p) {

	this->iInfo = iInfo;
	this->isAdd = isAdd;

	setWindowTitle("Interface for " + iInfo.publisher);
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
	if (iInfo.iName != NULL) {
		nameText->setText(iInfo.iName);
	}

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

	//缺一个设置当前

	layout->addWidget(typeBox, 1, 1, 1, 1);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 2, 0, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);
}

void EditInterfaceDialog::pSlotFinish() {
	if (nameText->text() != NULL && !(nameText->text().contains(" "))) {
		iInfo.iName = nameText->text();
		iInfo.iDataType = typeBox->currentText();
		close();
		emit refreshInterface(iInfo, isAdd);
	}
	else {
		emit signalSendMessage("contains wrong charactors");
	}
}