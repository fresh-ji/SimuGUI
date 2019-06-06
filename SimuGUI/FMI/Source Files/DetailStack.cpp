
#include "DetailStack.h"

DetailStack::DetailStack(QWidget *p) : MiniStack(p) {

	setTitleHeight(40);

	QWidget *modelInfoStack = new QWidget();
	addTab(tr("FMU"), modelInfoStack);

	QWidget *simuInfoStack = new QWidget();
	addTab(tr("Simulation"), modelInfoStack);
}

void DetailStack::slotModelDetail() {

}