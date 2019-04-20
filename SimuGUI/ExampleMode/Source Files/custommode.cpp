
/*
* Mode Bar���ѡ�
* @date : 2018/12/12
* @author : jihang
*/

#include "custommode.h"

CustomMode::CustomMode(QWidget *parent) : IMode(parent) {

	setObjectName(QLatin1String("CustomMode"));
	setDisplayName(tr("Custom"));
	setIcon(QIcon("./Icon/mode/example"));

	//�����Ҫ�ж��������贴������
	createWindow();
}

void CustomMode::createWindow() {

	//��һ�ֲ��card
	QGridLayout *pCardLayout = new QGridLayout();
	//������϶
	pCardLayout->setSpacing(10);
	//���ϡ��ҡ��¼�϶
	pCardLayout->setContentsMargins(10, 10, 10, 5);
	//�������ſ�Ƭ
	createCards();
	//�涨ÿ�����6��������6��û�£�����6��������һ��
	for (int j = 0; j < m_infoCards.count(); j++) {
		pCardLayout->addWidget(m_infoCards.at(j), j / 6, j % 6);
	}

	//�ڶ��ֲ��stack
	MiniStack *stack1 = new MiniStack();
	stack1->setTitle(tr("Device Chooser"));
	stack1->setTitleMargins(5, 1, 5, 1);

	//stack���plot����������
	MiniStack *stack2 = new MiniStack();
	stack2->setTitle(tr("real-time curve - chart"));
	stack2->setTitleMargins(5, 1, 5, 1);
	stack2->addTab(tr("Tab1"), new QCustomChart());
	stack2->addTab(tr("Tab2"), new QWidget);
	stack2->addTab(tr("Tab3"), new QWidget);
	stack2->addTab(tr("Tab4"), new QWidget);

	//stack�Ĳ���
	QHBoxLayout *pStackLayout = new QHBoxLayout();
	pStackLayout->setContentsMargins(10, 5, 10, 5);
	pStackLayout->setSpacing(10);
	pStackLayout->addWidget(stack1);
	pStackLayout->addWidget(stack2);

	//�������ֲ���Ĳ��֣��ŵ�һ��QWidget��
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addLayout(pCardLayout);
	layout->addLayout(pStackLayout);
	layout->addStretch();
	QWidget *w = new QWidget();
	w->setLayout(layout);

	//�ִ���һ����������ͬ��table2
	TinyTabWidget *tabWidget2 = new TinyTabWidget();
	tabWidget2->addTab(new QWidget(), tr("tab77"));
	tabWidget2->addTab(new QWidget(), tr("tab88"));

	tabWidget2->setTextColor(QColor(10, 10, 10));
	tabWidget2->setSelectedTextColor(QColor(255, 255, 255));
	tabWidget2->setHoverColor(QColor(180, 180, 180));
	tabWidget2->setBackgroundColor(QColor(100, 0, 200, 120));
	tabWidget2->setTabPosition(TinyTabWidget::South);
	//tabWidget1->setTabPosition(TinyTabWidget::West);
	//tabWidget3->setTabPosition(TinyTabWidget::South);
	//tabWidget->setTabPosition(TinyTabWidget::East);

	//��w��table2�ŵ�����
	TinyTabWidget *tabWidget = new TinyTabWidget();
	tabWidget->addTab(w, tr("tab1"));
	tabWidget->addTab(tabWidget2, tr("tab2"));

	tabWidget->setTextColor(QColor(10, 10, 10));
	tabWidget->setSelectedTextColor(QColor(255, 255, 255));
	tabWidget->setHoverColor(QColor(180, 180, 180));
	tabWidget->setBackgroundColor(QColor(0, 0, 0, 20));
	tabWidget->setTabPosition(TinyTabWidget::North);

	setWidget(tabWidget);
}

//������Ƭ
void CustomMode::createCards() {
	MiniCard *card = new MiniCard();
	card->setTitle(tr("Device"));
	card->setInfo(tr("Device info"));
	card->setValue("0");
	card->setDesc(tr("Online num"));
	card->setNote(tr("Total: 0"));
	m_infoCards.append(card);

	card = new MiniCard();
	card->setTitle(tr("Sensor"));
	card->setInfo(tr("Sensor info"));
	card->setValue("0");
	card->setDesc(tr("Total"));
	card->setNote("20%");
	card->setInfoBackgroundColor(QColor(35, 198, 200));
	card->setNoteColor(QColor(35, 198, 200, 255));
	m_infoCards.append(card);

	card = new MiniCard();
	card->setTitle(tr("Warning record"));
	card->setInfo(tr("Warning info"));
	card->setValue("0");
	card->setDesc(tr("Warning num"));
	card->setNote(QDateTime::currentDateTime().toString("MM/dd hh:mm"));
	card->setInfoBackgroundColor(QColor(248, 172, 89));
	card->setNoteColor(QColor(35, 198, 200));
	m_infoCards.append(card);
}