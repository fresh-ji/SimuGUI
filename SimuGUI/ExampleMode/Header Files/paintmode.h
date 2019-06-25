
/*
* Mode Bar���ѡ�
* @date : 2018/12/12
* @author : jihang
*/

#ifndef PAINTMODE_H
#define PAINTMODE_H

#include "GlobalConstant.h"

#include "imode.h"

#include "tinytabwidget.h"
#include "centralarea.h"
#include "ministack.h"

#include "stateprogressbar.h"
#include "qroundprogressbar.h"
#include "colorprogressbar.h"
#include "zoomcirclewait.h"
#include "waitprogressbar.h"

#include "dialplot.h"
#include "dashboard.h"

class PaintMode : public IMode {

	Q_OBJECT

public:
	explicit PaintMode(QWidget *parent = 0);

private:
	void createWindow();

	void createProgressWidget();
	void createProgressBar();
	void createDial();

signals:

public slots:
	//������ֵ��slot
	void slotStateSlider(int);

private slots:

private:
	TinyTabWidget *m_pMainTabWidget;

	// progress widget
	QWidget *m_pProgressWidget;
	MiniStack *m_pProgressBarStack;
	MiniStack *m_pDialStack;

private:
	QString iconPath = ICON_PATH;
};
#endif // PAINTMODE_H