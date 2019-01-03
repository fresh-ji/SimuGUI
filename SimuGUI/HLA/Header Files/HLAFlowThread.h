#pragma once

/*
* �߳���
* @date : 2018/10/28
* @author : jihang
*/

#include <QThread>

#include "HLAPortAdapter.h"

class HLAFlowThread : public QThread {

	Q_OBJECT

public:
	explicit HLAFlowThread(QObject *parent = 0);

protected:
	void run();

signals:
	//���͸�������ź�
	void FlowSignal(QString);
	void StateSignal(int);

private:
	//״̬������״̬���ƽ�
	int state;
	//����
	HLAPortAdapter *ha;

public:
	//��ȡ��ǰ״̬
	int getState();
	//��ȡ����
	HLAPortAdapter* getInstance();
};