
/*
* Mode Bar���ѡ�
* @date : 2018/12/12
* @author : jihang
*/

#include "menumode.h"

MenuMode::MenuMode(QWidget *parent) : IMode(parent) {

	//���������������
	setObjectName(QLatin1String("MenuMode"));
	setDisplayName(tr("Menu"));
	setIcon(QIcon("./Icon/mode/menu"));

	//����menu�Ļ���Ĭ�ϼӵ�menu�����ܹ����ð�����ͣ����ʾ
	QMenu *menu = new QMenu();
	menu->addAction(QIcon("./Icon/debug/debug"), tr("Action1"));
	menu->addAction(QIcon("./Icon/debug/debug"), tr("Action2"));
	menu->addAction(QIcon("./Icon/debug/debug"), tr("Action3"));
	menu->addAction(QIcon("./Icon/debug/debug"), tr("Action4"));
	menu->setToolTip(tr("menu mode"));

	setMenu(menu);
}