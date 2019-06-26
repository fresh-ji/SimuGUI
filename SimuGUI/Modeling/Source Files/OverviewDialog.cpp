
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

	//���ݽṹչʾʹ��ColumnView
	QColumnView *dataColumn = new QColumnView();
	//������༭
	dataColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//���ڵ�
	QStandardItemModel *dataModel = new QStandardItemModel;
	//��һ�����
	QMapIterator<QString, dataTypeInfo> it(datas);
	while (it.hasNext()) {
		it.next();
		QStandardItem *typeName = new QStandardItem("struct " + it.key());
		//�ڶ������
		QMap<QString, QString> dataContent = it.value().dataMap;
		QMapIterator<QString, QString> it2(dataContent);
		while (it2.hasNext()) {
			it2.next();
			QStandardItem *typeInfo = new QStandardItem(it2.value() + "   " + it2.key() + ";");
			typeName->appendRow(typeInfo);
		}
		dataModel->appendRow(typeName);
	}
	//���ø��ڵ�
	dataColumn->setModel(dataModel);

	//ģ��չʾʹ��ColumnView
	QColumnView *modelColumn = new QColumnView();
	//������༭
	modelColumn->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//���ڵ�
	QStandardItemModel *modelModel = new QStandardItemModel;
	//��һ�����
	for (QString modelName : models) {
		QStandardItem *name = new QStandardItem(modelName);
		//�ڶ������
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
	//���ø��ڵ�
	modelColumn->setModel(modelModel);

	layout->addWidget(dataColumn, 0, 0);
	layout->addWidget(modelColumn, 1, 0);

	QDialogButtonBox *result = new QDialogButtonBox();
	result->addButton("Generate XML File", QDialogButtonBox::AcceptRole);
	connect(result, SIGNAL(accepted()), this, SLOT(pSlotFinish()));
	layout->addWidget(result, 2, 0);

	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close���Զ���������ֹ֮ǰ��dialog���ź�
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