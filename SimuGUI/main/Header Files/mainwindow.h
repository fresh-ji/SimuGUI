
/*
* ������
* @date : 2018/12/19
* @author : jihang
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextBrowser>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTreeWidget>
#include <QDockWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QSettings>
#include <QAction>

#include "fancywindow.h"
#include "fancytabbar.h"
#include "modemanager.h"
#include "fancybar.h"
#include "quickaccessbar.h"
#include "fancydialog.h"
#include "pagesorter.h"
#include "fancybanner.h"

#include "cwidgets.h"
#include "skindialog.h"
#include "theme.h"

#include "custommode.h"
#include "menumode.h"
#include "paintmode.h"
#include "HLARunControl.h"
#include "FMISimulator.h"
#include "Modeling.h"

class MainWindow : public FancyWindow {

	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	static MainWindow* instance();

private:
	//��һ����
	void createWindow();
	void createQuickAccessBar();
	void createMenuBar();
	void createAdditionalControls();
	void createModeBar();
	void createCentralWidget();
	void createDockWidget();
	void createToolBar();
	//�ڶ�����
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
	//�Լ�
	static MainWindow *m_instance;

	//����mode����Ҫ�������е�stack��ʼ��
	ModeManager *m_modeManager;

	//��������Ķ���stack
	FancyTabWidget *m_modeStack;

	//�洢������style����һ��style
	QList<QAction *> m_styleActions;
	QAction *m_styleAction;

	//��������mode
	CustomMode *m_pCustomMode;
	PaintMode *m_pPaintMode;
	MenuMode *m_pMenuMode;

	//HLARunControl *m_pHLARunControl;
	FMISimulator *m_pFMISimulator;
	Modeling *m_pModeling;
};

#endif // MAINWINDOW_H