
#include "PlotDialog.h"

PlotDialog::PlotDialog(QString name, QVector<double> timeLine,
	QVector<double> dataLine, QWidget *p) : FancyDialog(p) {

	setWindowTitle("Plot for " + name);
	setWindowIcon(QIcon("./Icon/function/window"));
	setStyleSheet("background: #1890FF;");

	QWidget *centerWidget = new QWidget();
	centerWidget->setStyleSheet("background: #E7F7FE");
	QGridLayout *layout = new QGridLayout();

	QCustomChart *graph = new QCustomChart();

	graph->setData(timeLine, dataLine);

	layout->addWidget(graph);
	centerWidget->setLayout(layout);
	setCentralWidget(centerWidget);

	//close���Զ���������ֹ֮ǰ��dialog���ź�
	setAttribute(Qt::WA_DeleteOnClose);
}