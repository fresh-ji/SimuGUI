
/*
* @date : 2019/02/18
* @author : jihang
*/

#include "EditOutputDialog.h"

EditOutputDialog::EditOutputDialog(QList<QString> currentTypes, QString pub,
	QWidget *p) : FancyDialog(p) {

	isAdd = true;
	preName = "";
	setWindowTitle("Add Output for " + pub);
	publisher = pub;
	createTemplate(currentTypes);
}

EditOutputDialog::EditOutputDialog(
	QList<QString> currentTypes,
	outputInfo oInfo,
	QWidget *p) : FancyDialog(p) {

	isAdd = false;
	preName = oInfo.oName;
	setWindowTitle("Edit Output for " + oInfo.oPublisher);
	publisher = oInfo.oPublisher;
	createTemplate(currentTypes);

	nameText->setText(oInfo.oName);
	typeBox->setCurrentText(oInfo.oDataType);
}

void EditOutputDialog::createTemplate(QList<QString> currentTypes) {

	setWindowIcon(QIcon("./Icon/function/window"));
	setFixedSize(400, 250);
	setStyleSheet("background: #1890FF;");

	QFont font("Microsoft YaHei", 13, 50);

	QWidget *centerWidget = new QWidget();
	centerWidget->setStyleSheet("background: #E7F7FE");
	QGridLayout *layout = new QGridLayout();

	QLabel *nameLabel = new QLabel();
	nameLabel->setText("Name:");
	nameLabel->setFont(font);
	layout->addWidget(nameLabel, 0, 0, 1, 1);

	nameText = new QLineEdit();
	layout->addWidget(nameText, 0, 1, 1, 1);

	QLabel *dataType = new QLabel();
	dataType->setText("Data Type:");
	dataType->setFont(font);
	layout->addWidget(dataType, 1, 0, 1, 1);

	typeBox = new QComboBox();
	typeBox->addItems(basicTypes);
	typeBox->addItems(currentTypes);
	//增加自定义类型
	layout->addWidget(typeBox, 1, 1, 1, 1);

	QLabel *noticeLabel = new QLabel();
	noticeLabel->setText("Notice:");
	noticeLabel->setFont(font);
	layout->addWidget(noticeLabel, 2, 0, 1, 1);

	noticeContent = new QTextEdit();
	noticeContent->setFixedHeight(50);
	noticeContent->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	layout->addWidget(noticeContent, 2, 1, 1, 1);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("finish", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 3, 0, 1, 2);

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