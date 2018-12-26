
/*
* �߳���
* @date : 2018/10/28
* @author : jihang
*/

#include "HLAFlowThread.h"

#include "StateMachineCode.h"

//���첢��ʼ��״̬
HLAFlowThread::HLAFlowThread(QObject *parent) : QThread(parent) {
	state = StateMachineCode::getUnstartCode();
	ha = new HLAPortAdapter();
}

void HLAFlowThread::run() {
	InterfaceResponse<long> *ir0;
	InterfaceResponse<bool> *ir1;
	if (state == StateMachineCode::getUnstartCode()) {//---------����׼��----------
		//������Լ�
		ir0 = ha->joinFederation();
		if (!ir0->isSuccessful()) {
			emit FlowSignal("HERE IS AN ERROR");
		}
		emit FlowSignal(ir0->getMessage().c_str());
		emit FlowSignal("Federate handle is " + QString::number(ir0->getData()));
		state++;
		emit StateSignal(state);

		//������������Լ�
		ha->initial();
		state++;
		emit StateSignal(state);

		//������ص�
		ha->registerSynchronization(READY_TO_RUN);
		emit FlowSignal("#Flow Thread Complete# Please start other federates...");
		state++;
		emit StateSignal(state);
	}
	else if (state == StateMachineCode::getRegisPointCode()) {//---------����׼��----------
		//������ص�
		ha->synchronizationAchieved(READY_TO_RUN);
		state++;
		emit StateSignal(state);

		//������Լ�
		ir1 = ha->enableTimePolicy(0.01);
		emit FlowSignal(ir1->getMessage().c_str());
		state++;
		emit StateSignal(state);

		//������������Լ�
		ha->pubAndSub();
		state++;
		emit StateSignal(state);

		//������������Լ�
		ha->rigister();
		state++;
		emit StateSignal(state);
		emit FlowSignal("#Flow Thread Complete# Please start simulation...");
	}
	else if (state == StateMachineCode::getRegisteredCode()) {//---------��������----------
		state++;
		emit StateSignal(state);
		ha->simulate();
		state++;
		emit StateSignal(state);
		emit FlowSignal("#Flow Thread Complete# Simulation Ends.");
	}
}

int HLAFlowThread::getState() {
	return state;
}

HLAPortAdapter* HLAFlowThread::getInstance() {
	return ha;
}