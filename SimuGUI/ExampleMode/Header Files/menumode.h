
/*
* Mode Bar���ѡ�
* @date : 2018/12/12
* @author : jihang
*/

#ifndef MENUMODE_H
#define MENUMODE_H

#include "GlobalConstant.h"

#include "imode.h"

class MenuMode : public IMode {

	Q_OBJECT

public:
	explicit MenuMode(QWidget *parent = 0);

signals:

public slots:

private:
	QString iconPath = ICON_PATH;
};
#endif // MENUMODE_H