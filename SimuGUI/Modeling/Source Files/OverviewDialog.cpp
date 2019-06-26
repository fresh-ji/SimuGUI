
#include "OverviewDialog.h"

OverviewDialog::OverviewDialog(
	QMap<QString, dataTypeInfo> datas,
	QMap<QString, interfaceInfo> ios,
	QList<QString> models,
	QWidget *p) : FancyDialog(p) {

	setWindowTitle("Overview");
	setWindowIcon(QIcon(iconPath + "function/window"));
	setStyleSheet("background: #1890FF;");

	QWidget *centerWidget = new QWidget();
	centerWidget->setStyleSheet("background: #E7F7FE");
	QGridLayout *layout = new QGridLayout();

	//数据结构展示使用ColumnView
	QColumnView *dataColumn = new QColumnView();
	//不允许编辑
	dataColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//根节点
	QStandardItemModel *dataModel = new QStandardItemModel;
	//第一层遍历
	QMapIterator<QString, dataTypeInfo> it(datas);
	while (it.hasNext()) {
		it.next();
		QStandardItem *typeName = new QStandardItem("struct " + it.key());
		//第二层遍历
		QMap<QString, QString> dataContent = it.value().dataMap;
		QMapIterator<QString, QString> it2(dataContent);
		while (it2.hasNext()) {
			it2.next();
			QStandardItem *typeInfo = new QStandardItem(it2.value() + "   " + it2.key() + ";");
			typeName->appendRow(typeInfo);
		}
		dataModel->appendRow(typeName);
	}
	//设置根节点
	dataColumn->setModel(dataModel);

	//模型展示使用ColumnView
	QColumnView *modelColumn = new QColumnView();
	//不允许编辑
	modelColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//根节点
	QStandardItemModel *modelModel = new QStandardItemModel;
	//第一层遍历
	for (QString modelName : models) {
		QStandardItem *name = new QStandardItem(modelName);
		//第二层遍历
		QMapIterator<QString, interfaceInfo> it2(ios);
		while (it2.hasNext()) {
			it2.next();
			if (modelName == it2.value().publisher) {
				name->appendRow(new QStandardItem("OUT---> " + it2.key()));
			}
			if (it2.value().subscribers.contains(modelName)) {
				name->appendRow(new QStandardItem(" IN<--- " + it2.key()));
			}
		}
		modelModel->appendRow(name);
	}
	//设置根节点
	modelColumn->setModel(modelModel);

	layout->addWidget(dataColumn, 0, 0);
	layout->addWidget(modelColumn, 1, 0);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("Generate XML File", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 2, 0);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close后自动析构，防止之前的dialog发信号
	setAttribute(Qt::WA_DeleteOnClose);
}

void OverviewDialog::pSlotFinish() {
	QString uuid = QUuid::createUuid().toString();
	QString time = QDateTime::currentDateTime().toString("MM-dd-hh-mm-ss");
	QFile file(".\\Modeling\\result\\my@" + uuid + "@" + time + ".xml");
	if (file.open(QFile::ReadWrite | QIODevice::Truncate)) {

		QXmlStreamWriter stream(&file);
		stream.setAutoFormatting(true);
		stream.writeStartDocument();
		stream.writeStartElement("SimulationSystem");
		stream.writeAttribute("name", "my");



		stream.writeEndElement();//SimulationSystem
		stream.writeEndDocument();
		file.close();
		QMessageBox::information(NULL, "Title",
			"success-_-", QMessageBox::Yes, QMessageBox::Yes);
	}
}