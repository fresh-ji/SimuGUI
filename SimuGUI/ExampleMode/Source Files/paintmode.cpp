
#include "paintmode.h"

PaintMode::PaintMode(QWidget *parent) : IMode(parent) {

	setObjectName(QLatin1String("PaintMode"));
	setDisplayName(tr("Painter"));
	setIcon(QIcon(iconPath + "mode/example"));

	createWindow();
}

void PaintMode::createWindow() {

	m_pMainTabWidget = new TinyTabWidget();

	createProgressWidget();

	//��һ��������CentralArea���Ǹ���scroll�ģ�ֱ��add�����
	CentralArea *progressArea = new CentralArea();
	progressArea->setWidget(m_pProgressWidget);

	//Switchδ����
	m_pMainTabWidget->addTab(progressArea, tr("Progress"));
	m_pMainTabWidget->addTab(new QWidget(), tr("Switch"));

	m_pMainTabWidget->setTextColor(QColor(10, 10, 10));
	m_pMainTabWidget->setSelectedTextColor(QColor(255, 255, 255));
	m_pMainTabWidget->setHoverColor(QColor(180, 180, 180));
	m_pMainTabWidget->setBackgroundColor(QColor(0, 100, 200, 120));
	m_pMainTabWidget->setTabPosition(TinyTabWidget::North);

	setWidget(m_pMainTabWidget);
}

void PaintMode::createProgressWidget() {

	m_pProgressWidget = new QWidget();

	createProgressBar();
	createDial();

	//��box�ű��̣���box�Ű�������������scroll
	QHBoxLayout *layout1 = new QHBoxLayout();
	layout1->setMargin(0);
	layout1->setSpacing(10);
	layout1->addWidget(m_pProgressBarStack);
	layout1->addWidget(m_pDialStack);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(1, 1, 1, 1);
	layout->setSpacing(10);
	layout->addLayout(layout1);

	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);
	layout->addWidget(new QTextBrowser);

	m_pProgressWidget->setLayout(layout);
}

void PaintMode::createProgressBar() {

	m_pProgressBarStack = new MiniStack();
	//����
	m_pProgressBarStack->setTitle("ProgressBar");
	//��
	QSlider *slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 5);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(1);

	//��һ��������״̬Ǩ�Ƶ�bar��stateLayout���ڼ���������
	QVBoxLayout *stateLayout = new QVBoxLayout();
	stateLayout->setMargin(0);
	stateLayout->setSpacing(2);
	StateProgressBar *stateProgressBar = new StateProgressBar();
	stateProgressBar->setType(StateProgressBar::ACK); // NACK, ACK
	QStringList states;
	states << tr("C") << tr("C++") << tr("Linux") << tr("Qt") << tr("Java");
	stateProgressBar->setStates(states);
	stateProgressBar->setTip("now");
	stateProgressBar->setValue(4);
	//    stateProgressBar->setFixedHeight(140);
	stateProgressBar->setMinimumHeight(140);
	stateLayout->addWidget(stateProgressBar);
	connect(slider, SIGNAL(valueChanged(int)), stateProgressBar, SLOT(setValue(int)));
	//��ʼ��һ�����ش�stateLayout��Widget�����ֻ��һ��ֱ��stateProgressBarҲ��
	QWidget *stateProgressWidget = new QWidget();
	stateProgressWidget->setLayout(stateLayout);
	//���
	m_pProgressBarStack->addTab(tr("State"), stateProgressWidget, slider);

	//�ڶ�����Բ�α���ȣ�Ҫ���»�slider
	slider = new QSlider(Qt::Horizontal);
	slider->setMaximum(999);
	slider->setValue(150);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(25);
	//��һ��
	QGridLayout *grid1 = new QGridLayout();
	QLabel *label = new QLabel(tr("Donut Style"));
	label->setAlignment(Qt::AlignCenter);
	grid1->addWidget(label, 0, 0, 1, 1);
	label = new QLabel(tr("Pie Style"));
	label->setAlignment(Qt::AlignCenter);
	grid1->addWidget(label, 0, 1, 1, 1);
	label = new QLabel(tr("Line Style"));
	label->setAlignment(Qt::AlignCenter);
	grid1->addWidget(label, 0, 2, 1, 1);
	//�ڶ���
	// 1. donut (default style)
	QRoundProgressBar *roundProgressBar = new QRoundProgressBar();
	roundProgressBar->setFormat("%v");
	roundProgressBar->setDecimals(0);
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 1, 0, 1, 1);
	// 2. pie
	roundProgressBar = new QRoundProgressBar();
	roundProgressBar->setNullPosition(QRoundProgressBar::PositionRight);
	roundProgressBar->setBarStyle(QRoundProgressBar::StylePie);
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 1, 1, 1, 1);
	// 3. line
	roundProgressBar = new QRoundProgressBar();
	roundProgressBar->setFormat("%m");
	roundProgressBar->setBarStyle(QRoundProgressBar::StyleLine);
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 1, 2, 1, 1);
	//������
	// 4. donut (customized via QPalette + gradient)
	roundProgressBar = new QRoundProgressBar();
	QPalette p1;
	p1.setBrush(QPalette::AlternateBase, Qt::black);
	p1.setColor(QPalette::Text, Qt::yellow);
	roundProgressBar->setPalette(p1);
	roundProgressBar->setNullPosition(QRoundProgressBar::PositionLeft);
	roundProgressBar->setDecimals(0);
	// make a gradient from green over yellow to red
	QGradientStops gradientPoints;
	gradientPoints << QGradientStop(0, Qt::green) << QGradientStop(0.5, Qt::yellow) << QGradientStop(1, Qt::red);
	roundProgressBar->setDataColors(gradientPoints);
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 2, 0, 1, 1);
	// 5.pie (customized via QPalette + gradient)
	roundProgressBar = new QRoundProgressBar();
	QPalette p2(p1);
	p2.setBrush(QPalette::Base, Qt::lightGray);
	p2.setColor(QPalette::Text, Qt::magenta);
	p2.setColor(QPalette::Shadow, Qt::green);
	roundProgressBar->setPalette(p2);
	roundProgressBar->setNullPosition(QRoundProgressBar::PositionRight);
	roundProgressBar->setBarStyle(QRoundProgressBar::StylePie);
	roundProgressBar->setDataColors(gradientPoints);   // here gradient colors is set
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 2, 1, 1, 1);
	// 6. line style with custom outline
	roundProgressBar = new QRoundProgressBar();
	roundProgressBar->setDecimals(2);
	roundProgressBar->setBarStyle(QRoundProgressBar::StyleLine);
	roundProgressBar->setOutlinePenWidth(18);
	roundProgressBar->setDataPenWidth(10);
	roundProgressBar->setRange(slider->minimum(), slider->maximum());
	roundProgressBar->setValue(slider->value());
	connect(slider, SIGNAL(valueChanged(int)), roundProgressBar, SLOT(setValue(int)));
	grid1->addWidget(roundProgressBar, 2, 2, 1, 1);
	//���ǰ�layout�ŵ�һ��HBox����VBox��ٸ�widget�������layout
	QHBoxLayout *roundLayout = new QHBoxLayout();
	roundLayout->addLayout(grid1);
	QWidget *roundProgressWidget = new QWidget();
	roundProgressWidget->setLayout(roundLayout);
	//���
	m_pProgressBarStack->addTab(tr("Round"), roundProgressWidget, slider);

	//�������������ͽ�������Ҫ���»�slider
	slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 100);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(5);
	//������
	ColorProgressBar *colorProgressBar1 = new ColorProgressBar();
	colorProgressBar1->setDecimal(0);
	colorProgressBar1->setBarBackGroundColor(QColor(Qt::white));
	colorProgressBar1->setBarColor(QColor(100, 184, 255));
	colorProgressBar1->setShowBarSplitLine(false);
	colorProgressBar1->setValue(40);
	colorProgressBar1->setFixedHeight(20);
	connect(slider, SIGNAL(valueChanged(int)), colorProgressBar1, SLOT(setValue(int)));
	ColorProgressBar *colorProgressBar3 = new ColorProgressBar();
	colorProgressBar3->setDecimal(1);
	colorProgressBar3->setBarBackGroundColor(QColor(250, 250, 250));
	colorProgressBar3->setBarColor(QColor(24, 189, 155));
	colorProgressBar3->setShowBarSplitLine(true);
	colorProgressBar3->setBarSplitLineDelta(5);
	colorProgressBar3->setBarSplitLineColor(QColor(242, 242, 242));
	colorProgressBar3->setValue(30);
	colorProgressBar3->setFixedHeight(20);
	connect(slider, SIGNAL(valueChanged(int)), colorProgressBar3, SLOT(setValue(int)));
	//���
	QVBoxLayout *colorLayout = new QVBoxLayout();
	colorLayout->setContentsMargins(5, 5, 5, 5);
	colorLayout->setSpacing(5);
	colorLayout->addWidget(colorProgressBar1);
	colorLayout->addWidget(colorProgressBar3);
	QWidget *colorProgressWidget = new QWidget();
	colorProgressWidget->setLayout(colorLayout);
	//���
	m_pProgressBarStack->addTab(tr("Color"), colorProgressWidget, slider);

	//���ĸ����ȴ�
	WaitProgressBar *taichiWait1 = new WaitProgressBar(WaitProgressBar::Taichi);
	taichiWait1->setFrontColor(QColor(97, 117, 118));
	taichiWait1->setBackColor(QColor(255, 107, 107));

	WaitProgressBar *circleWait2 = new WaitProgressBar(WaitProgressBar::Circle);
	circleWait2->setFrontColor(QColor(46, 194, 163));
	//Ĭ��˳ʱ��
	circleWait2->setClockwiseRotate(false);

	WaitProgressBar *pieWait1 = new WaitProgressBar(WaitProgressBar::Pie);
	pieWait1->setFrontColor(QColor(255, 107, 107));
	pieWait1->setBackColor(QColor(97, 117, 118));

	WaitProgressBar *lineWait1 = new WaitProgressBar(WaitProgressBar::Line);
	lineWait1->setFrontColor(QColor(253, 132, 132));

	WaitProgressBar *donutWait1 = new WaitProgressBar(WaitProgressBar::Donut);
	donutWait1->setFrontColor(QColor(255, 107, 107));
	donutWait1->setBackColor(QColor(97, 117, 118));

	ZoomCircleWait *zoomCircleWait1 = new ZoomCircleWait();
	zoomCircleWait1->setColor(QColor(255, 107, 107));
	zoomCircleWait1->setZoomDelta(5);

	ZoomCircleWait *zoomCircleWait2 = new ZoomCircleWait();
	zoomCircleWait2->setColor(Qt::green);
	zoomCircleWait2->setMinRadious(25);
	zoomCircleWait2->setZoomDelta(5);
	zoomCircleWait2->setZoomOut(true);
	ZoomCircleWait *zoomCircleWait3 = new ZoomCircleWait();
	zoomCircleWait3->setColor(Qt::red);
	zoomCircleWait3->setMinRadious(25);
	zoomCircleWait3->setRadious(50);
	zoomCircleWait3->setZoomDelta(5);
	zoomCircleWait3->setZoomOut(true);
	//������С�����һ����ģ���дһ��Layout������widget��װ
	QHBoxLayout *zoomLayout = new QHBoxLayout();
	zoomLayout->addWidget(zoomCircleWait2);
	zoomLayout->addWidget(zoomCircleWait3);
	QWidget *zoomWidget = new QWidget();
	zoomWidget->setLayout(zoomLayout);

	//���岼��
	QGridLayout *waitLayout = new QGridLayout();
	waitLayout->addWidget(taichiWait1, 0, 0, 1, 1);
	waitLayout->addWidget(circleWait2, 0, 1, 1, 1);
	waitLayout->addWidget(pieWait1, 0, 2, 1, 1);
	waitLayout->addWidget(lineWait1, 0, 3, 1, 1);
	waitLayout->addWidget(donutWait1, 1, 1, 1, 1);
	waitLayout->addWidget(zoomCircleWait1, 1, 2, 1, 1);
	waitLayout->addWidget(zoomWidget, 1, 3, 1, 1);
	QWidget *waitProgressWidget = new QWidget();
	waitProgressWidget->setLayout(waitLayout);
	//���
	m_pProgressBarStack->addTab(tr("Wait"), waitProgressWidget);
}

void PaintMode::createDial() {

	m_pDialStack = new MiniStack();
	//����
	m_pDialStack->setTitle(tr("Dial"));
	//��
	QSlider *slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 100);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(5);
	//�Ǳ�1
	DialPlot *dialPlot1 = new DialPlot();
	dialPlot1->setDesc(tr("dial"));
	dialPlot1->setUnit(" unit");
	dialPlot1->setRange(0, 100);
	dialPlot1->setUpper(70);
	dialPlot1->setLower(30);
	dialPlot1->setValue(10);
	dialPlot1->setTextColor(QColor(0, 0, 0));
	dialPlot1->setBgColor(QColor(255, 255, 255));
	dialPlot1->setFirstColor(QColor(240, 0, 0));
	dialPlot1->setSecondColor(QColor(0, 205, 0));
	dialPlot1->setThirdColor(QColor(255, 255, 10));
	dialPlot1->setCenterColor(QColor(51, 51, 51));
	connect(slider, SIGNAL(valueChanged(int)), dialPlot1, SLOT(setValue(int)));
	//�Ǳ�2
	Dashboard *dashBoard = new Dashboard();
	connect(slider, SIGNAL(valueChanged(int)), dashBoard, SLOT(setValue(int)));

	QHBoxLayout *dialLayout = new QHBoxLayout();
	dialLayout->addWidget(dialPlot1);
	dialLayout->addWidget(dashBoard);
	QWidget *dialWidget = new QWidget();
	dialWidget->setLayout(dialLayout);
	//���
	m_pDialStack->addTab("Dial", dialWidget, slider);
}

void PaintMode::slotStateSlider(int value) {
	QList<StateProgressBar*> stateProgressBars = m_pProgressWidget->findChildren<StateProgressBar*>();
	foreach(StateProgressBar *state, stateProgressBars) {
		state->setValue(value);
	}
}