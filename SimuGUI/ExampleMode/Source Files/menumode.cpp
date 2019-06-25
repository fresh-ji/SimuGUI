
#include "menumode.h"

MenuMode::MenuMode(QWidget *parent) : IMode(parent) {

	//���������������
	setObjectName(QLatin1String("MenuMode"));
	setDisplayName(tr("Menu"));
	setIcon(QIcon(iconPath + "mode/example"));

	//����menu�Ļ���Ĭ�ϼӵ�menu�����ܹ����ð�����ͣ����ʾ
	QMenu *menu = new QMenu();
	menu->addAction(QIcon(iconPath + "debug/debug"), tr("Action1"));
	menu->addAction(QIcon(iconPath + "debug/debug"), tr("Action2"));
	menu->addAction(QIcon(iconPath + "debug/debug"), tr("Action3"));
	menu->addAction(QIcon(iconPath + "debug/debug"), tr("Action4"));
	menu->setToolTip(tr("menu mode"));

	setMenu(menu);
}