#pragma once

/*
* @date : 2018/12/27
* @author : jihang
*/

#ifndef FMISIMULATOR_H
#define FMISIMULATOR_H

#include "imode.h"

#include "cwidgets.h"

class FMISimulator : public IMode {

	Q_OBJECT

public:
	explicit FMISimulator(QWidget *parent = 0);

signals:

public slots:
	void test();

private:
	void createWindow();

	QTextBrowser *t;
};
#endif // FMISIMULATOR_H