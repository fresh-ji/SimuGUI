
#include "EditInputDialog.h"

EditInputDialog::EditInputDialog(QString modelName, QSet<inputInfo> subInfo,
	QWidget *p) : FancyDialog(p) {

	setWindowTitle("Subscribe for " + modelName);
	setWindowIcon(QIcon(iconPath + "function/window"));
	setStyleSheet("background: #1890FF;");

	QFont font("Microsoft YaHei", 13, 50);

	QWidget *centerWidget = new QWidget();
	centerWidget->setStyleSheet("background: #E7F7FE");
	QGridLayout *layout = new QGridLayout();

	treeWidget = new QTreeWidget();

	for (inputInfo i : subInfo) {

		QTreeWidgetItem *info = new QTreeWidgetItem();
		info->setText(0, i.iName);
		info->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		if (i.isSubscribe) {
			info->setCheckState(0, Qt::Checked);
			preSet.insert(i.iName);
		}
		else {
			info->setCheckState(0, Qt::Unchecked);
		}

		bool isHave = false;
		for (int j = 0; j < treeWidget->topLevelItemCount(); ++j) {
			if (treeWidget->topLevelItem(j)->text(0) == i.iPublisher) {
				treeWidget->topLevelItem(j)->addChild(info);
				isHave = true;
				break;
			}
		}
		if (!isHave) {
			QTreeWidgetItem *model = new QTreeWidgetItem();
			model->setText(0, i.iPublisher);
			if (i.iPublisher == modelName) {
				model->setTextColor(0, QColor(Qt::red));
			}
			model->addChild(info);
			treeWidget->addTopLevelItem(model);
		}
	}

	layout->addWidget(treeWidget);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("OK", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close后自动析构，防止之前的dialog发信号
	setAttribute(Qt::WA_DeleteOnClose);
}

void EditInputDialog::pSlotFinish() {
	QSet<QString> addSubSet, cancelSubSet;
	QTreeWidgetItemIterator it(treeWidget);
	while (*it) {
		QString info = (*it)->text(0);
		if ((*it)->checkState(0) == Qt::Checked) {
			if (!(preSet.contains(info))) {
				addSubSet.insert(info);
			}
		}
		else if ((*it)->checkState(0) == Qt::Unchecked) {
			if (preSet.contains(info)) {
				cancelSubSet.insert(info);
			}
		}
		++it;
	}
	close();
	refreshInput(addSubSet, cancelSubSet);
}