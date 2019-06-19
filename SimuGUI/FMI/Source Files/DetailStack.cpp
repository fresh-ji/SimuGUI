
#include "DetailStack.h"

DetailStack::DetailStack(QWidget *p) : MiniStack(p) {

	setTitleHeight(40);

	infoWidget = new QWidget();
	infoLayout = new QGridLayout();
	infoWidget->setLayout(infoLayout);
	addTab(tr("Info"), infoWidget);

	QWidget *variableInfoStack = new QWidget();
	addTab(tr("Variables"), variableInfoStack);

	QWidget *simuInfoStack = new QWidget();
	addTab(tr("Simulation"), simuInfoStack);
}

void DetailStack::slotModelDetail(FMUInfo* info) {

	/*
	//删除之前布局
	QLayoutItem *child;
	while ((child = infoLayout->takeAt(0)) != 0)
	{
		infoLayout->removeWidget(child->widget());
		child->widget()->setParent(0);
		delete child;
	}

	if (info != NULL) {
		QLabel *typeLabel = new QLabel();
		typeLabel->setText("FMI type");
		infoLayout->addWidget(typeLabel, 0, 0);
		QLabel *type = new QLabel();
		type->setText(info->simuType);
		infoLayout->addWidget(type, 0, 1);

		QMap<QString, QString>::iterator it;
		int count;
		for (count = 1, it = info->basicInfo.begin(); it != info->basicInfo.end(); ++it, ++count) {
			QLabel *title = new QLabel();
			title->setText(it.key());
			infoLayout->addWidget(title, count, 0);
			QLabel *content = new QLabel();
			content->setText(it.value());
			infoLayout->addWidget(content, count, 1);
		}
	}
	*/
}