
/*
* Mode Bar里的选项卡
* @date : 2018/12/12
* @author : jihang
*/

#ifndef CUSTOMMODE_H
#define CUSTOMMODE_H

/*
* Mode Bar里的选项卡
* @date : 2018/12/12
* @author : jihang
*/

#include "GlobalConstant.h"

#include "imode.h"

#include "ministack.h"
#include "minicard.h"
#include "tinytabwidget.h"

#include "qcustomchart.h"

class CustomMode : public IMode {

	Q_OBJECT

public:
	explicit CustomMode(QWidget *parent = 0);

private:
	void createWindow();
	void createCards();

signals:

public slots:

private:
	TinyTabWidget *m_pTabWidget;
	QList<MiniCard *> m_infoCards;

private:
	QString iconPath = ICON_PATH;
};
#endif // CUSTOMMODE_H