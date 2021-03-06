
/*
* 主布局
* @date : 2018/12/19
* @author : jihang
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GlobalConstant.h"

#include "fancywindow.h"
#include "fancytabbar.h"
#include "modemanager.h"
#include "fancybar.h"
#include "quickaccessbar.h"
#include "fancydialog.h"
#include "pagesorter.h"
#include "fancybanner.h"

#include "skindialog.h"
#include "theme.h"

#include "custommode.h"
#include "menumode.h"
#include "paintmode.h"
#include "FMISimulator.h"
#include "Modeling.h"

class MainWindow : public FancyWindow {

	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	static MainWindow* instance();

private:
	//第一部分
	void createWindow();
	void createQuickAccessBar();
	void createMenuBar();
	void createAdditionalControls();
	void createModeBar();
	void createCentralWidget();
	void createDockWidget();
	void createToolBar();
	//第二部分
	void readSettings();
	void writeSettings();

public slots:
	void updateTheme();

private slots:
	void slotNew();
	void slotSkin();
	void slotChangeSkin();
	void slotSetStyle(QAction *action);

private:
	//自己
	static MainWindow *m_instance;

	//管理mode，需要管理其中的stack初始化
	ModeManager *m_modeManager;

	//中心区域的对象stack
	FancyTabWidget *m_modeStack;

	//存储的所有style和上一个style
	QList<QAction *> m_styleActions;
	QAction *m_styleAction;

	//三个例子mode
	CustomMode *m_pCustomMode;
	PaintMode *m_pPaintMode;
	MenuMode *m_pMenuMode;

	FMISimulator *m_pFMISimulator;
	Modeling *m_pModeling;

private:
	QString iconPath = ICON_PATH;
};

#endif // MAINWINDOW_H