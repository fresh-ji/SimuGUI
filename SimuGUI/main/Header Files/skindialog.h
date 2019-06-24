
/*
* Ƥ��ͷ�ļ�
* @date : 2018/12/18
* @author : jihang
*/

#ifndef SKINDIALOG_H
#define SKINDIALOG_H

#include <QMap>

#include "GlobalConstant.h"

#include "tinytabwidget.h"
#include "skinitem.h"
#include "centralarea.h"
#include "fancydialog.h"

#include "cwidgets.h"
#include "theme.h"

class SkinDialog : public FancyDialog {

	Q_OBJECT

public:
	explicit SkinDialog(QWidget *parent = 0);

private:
	void createWindow();

	void setSkin(const QString &);
	QString getSkin();

signals:
	//Ƥ�������ı�
	void changeSkin();
	//���ⷢ���ı�
	void changeTheme();

public slots:

private slots:
	//���Ƥ��ʱ�����ķ�Ӧ
	void slotItemClicked();
	//ѡ��themeʱ�����ķ�Ӧ
	void slotThemeChanged(int index);

private:
	//ֻ��m_pLastSkinItem�б�Ҫ�������������������������createWindow�ľֲ�����
	//Ƥ������������Ժ������洢��ǰΪlast��������list
	SkinItem *m_pLastSkinItem;
	QList<SkinItem*> m_skinItemList;

	//����
	QComboBox *m_pThemeComboBox;
	QMap<QString, QString> m_themeMap;
};

#endif // SKINDIALOG_H