

#include "EditInputDialog.h"

EditInputDialog::EditInputDialog(QString modelName, QWidget *p) : FancyDialog(p) {

	setWindowTitle("Subscribe for " + modelName);
	setWindowIcon(QIcon("./Icon/tools/interface"));

	QFont font("Microsoft YaHei", 13, 50);

	QWidget *centerWidget = new QWidget();
	QGridLayout *layout = new QGridLayout();

	//QLabel *nameLabel = new QLabel();
	//nameLabel->setText("Name:");
	//nameLabel->setFont(font);
	//layout->addWidget(nameLabel, 0, 0, 1, 1);

	//nameText = new QLineEdit();
	//layout->addWidget(nameText, 0, 1, 1, 1);

	//QLabel *dataType = new QLabel();
	//dataType->setText("Type:");
	//dataType->setFont(font);
	//layout->addWidget(dataType, 1, 0, 1, 1);

	//typeBox = new QComboBox();
	//typeBox->addItem("real");
	//typeBox->addItem("int");
	//typeBox->addItem("bool");
	//typeBox->addItem("string");
	//typeBox->addItem("enum");
	//�����Զ�������
	//layout->addWidget(typeBox, 1, 1, 1, 1);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result);
	//layout->addWidget(result, 2, 0, 1, 2);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close���Զ���������ֹ֮ǰ��dialog���ź�
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditInputDialog::pSlotFinish() {
}