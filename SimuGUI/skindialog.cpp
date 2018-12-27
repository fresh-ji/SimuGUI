
/*
* Ƥ��Դ�ļ�
* @date : 2018/12/18
* @author : jihang
*/

#include "skindialog.h"

#define SKIN_NUM 15
#define SKIN_RES_NAME(n)   QString("./Icon/skin/%1").arg(n)
#define THEME_PATH QString("./Icon/theme")

SkinDialog::SkinDialog(QWidget *parent) : FancyDialog(parent) {

	m_pLastSkinItem = 0;
	m_skinItemList.clear();
	m_pThemeComboBox = 0;
	m_themeMap.clear();

	createWindow();

	setFixedSize(800, 600);
	setWindowTitle(tr("Change Skin and Theme"));
	setWindowIcon(QIcon("./Icon/main/logo"));
}

//��setting��洢�ŵ�ǰ��skin��themeTODO
void SkinDialog::createWindow() {

	QWidget *pCentralWidget = new QWidget();

	//��skin��
	TinyTabWidget * m_pTabWidget = new TinyTabWidget();
	m_pTabWidget->setHoverColor(Theme::g_tinyTabHoverColor);
	m_pTabWidget->setPressColor(Theme::g_tinyTabPressColor);
	m_pTabWidget->setTextColor(Theme::g_tinyTabTextColor);
	m_pTabWidget->setSelectedTextColor(Theme::g_tinyTabSelectedTextColor);
	m_pTabWidget->setBackgroundColor(Theme::g_tinyTabBackgroundColor);
	m_pTabWidget->setNormalColor(Theme::g_tinyTabNormalColor);
	m_pTabWidget->setSlideColor(Theme::g_tinyTabSlideColor);

	//widget�ϼ�areaʹ��tabҳ
	CentralArea *area = new CentralArea();
	m_pTabWidget->addTab(area, tr("All page"));

	//area�ϱ�����һ��widget
	QWidget *allPageWidget = new QWidget();
	QGridLayout *pGridLayout = new QGridLayout();
	pGridLayout->setSpacing(15);
	pGridLayout->setContentsMargins(20, 10, 10, 10);
	allPageWidget->setLayout(pGridLayout);
	area->setWidget(allPageWidget);

	//ȫ����ʱװ��
	QStringList skinNameList;
	for (int i = 0; i < SKIN_NUM; i++) {
		skinNameList << SKIN_RES_NAME(i);
	}
	//��ȡ��ǰƤ��
	QString currentSkin = getSkin();
	//ȫ��չʾ����дlast�����б�
	for (int i = 0; i < SKIN_NUM; i++) {
		SkinItem *item = new SkinItem();
		item->setMinimumHeight(150);
		item->setPixmapName(skinNameList.at(i));
		if (currentSkin == skinNameList.at(i)) {
			m_pLastSkinItem = item;
		}
		m_skinItemList.append(item);
		//���Ӻͼ���
		connect(item, SIGNAL(clicked()), this, SLOT(slotItemClicked()));
		pGridLayout->addWidget(item, i / 3, i % 3);
	}
	//�����ǰû��skin��Ĭ�ϵ�һ����ѡ��last
	if (!m_pLastSkinItem) {
		m_pLastSkinItem = m_skinItemList.at(0);
	}
	m_pLastSkinItem->select(true);

	//ȫ������comboBox����ʼtheme
	m_pThemeComboBox = new QComboBox();
	QDir dir(THEME_PATH);
	QStringList typeList;
	typeList << "*.cptheme" << "*.ini";
	QFileInfoList themeFiles = dir.entryInfoList(typeList, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	foreach(QFileInfo info, themeFiles) {
		QString fileName = info.absoluteFilePath();
		QString themeName = Theme::getThemeName(fileName);
		m_pThemeComboBox->addItem(themeName, fileName);
	}
	//��ȡ��ǰ����
	QString currentThemeName = Theme::getThemeName(Theme::getTheme());
	//��comboBox��λΪ��ǰ��
	int index = m_pThemeComboBox->findText(currentThemeName);
	if (index != -1) {
		m_pThemeComboBox->setCurrentIndex(index);
	}
	//����
	connect(m_pThemeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(slotThemeChanged(int)));

	//��comboBox�ŵ�HBoxLayout��
	m_pThemeComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	QHBoxLayout *themeLayout = new QHBoxLayout();
	themeLayout->setContentsMargins(10, 5, 10, 5);
	themeLayout->setSpacing(10);
	themeLayout->addWidget(new QLabel(tr("Theme:")));
	themeLayout->addWidget(m_pThemeComboBox);
	themeLayout->addStretch();

	//�ϲ�skin��theme������
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_pTabWidget);
	layout->addLayout(themeLayout);

	//��dialog�����
	pCentralWidget->setLayout(layout);
	setCentralWidget(pCentralWidget);
}

//set and get
void SkinDialog::setSkin(const QString &skinName) {
	QSettings settings("Canpool", "qtcanpool");
	settings.setValue("skin", skinName);
	update();
}

QString SkinDialog::getSkin() {
	QSettings settings("Canpool", "qtcanpool");
	QString skinName = settings.value("skin").toString();
	return skinName;
}

//���Ƥ��ʱ�����ķ�Ӧ
void SkinDialog::slotItemClicked() {
	SkinItem *item = qobject_cast<SkinItem *>(sender());

	//С�̹��ı䣬���ı�last
	if (m_pLastSkinItem == item) {
		return;
	}
	if (m_pLastSkinItem) {
		m_pLastSkinItem->select(false);
	}
	m_pLastSkinItem = item;
	m_pLastSkinItem->select(true);

	//��set�����о��Ѿ�update����
	QString skinName = item->pixmapName();
	setSkin(skinName);

	emit changeSkin();
}

//ѡ��themeʱ�����ķ�Ӧ
void SkinDialog::slotThemeChanged(int index) {
	//��set�����о��Ѿ�load����
	QString fileName = m_pThemeComboBox->itemData(index).toString();
	Theme::setTheme(fileName);

	emit changeTheme();
}