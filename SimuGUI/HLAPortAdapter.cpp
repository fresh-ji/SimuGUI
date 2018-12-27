
/*
* RunControlAdapter��Դ�ļ�
* @date : 2018/10/22
* @author : jihang
*/

#include "HLAPortAdapter.h"

#include "ControlCode.h"

HLAPortAdapter::HLAPortAdapter(QObject *parent) : QObject(parent) {

	//��Ա����
	m_fedFile = "testfom.fed";
	m_federationName = "ExampleFederation";
	m_federateName = "runControl";
	isPause = false;
	isEnd = false;

	//�������
	objectPause.className = "ObjectRoot.PAUSE";
	objectPause.attributeList.push_back("pause");

	objectContinue.className = "ObjectRoot.CONTINUE";
	objectContinue.attributeList.push_back("continue");

	objectEnd.className = "ObjectRoot.End";
	objectEnd.attributeList.push_back("end");

	//����
	objectA.className = "ObjectRoot.A";
	objectA.attributeList.push_back("aa");
	objectA.attributeList.push_back("ab");
	objectA.attributeList.push_back("ac");

	objectB.className = "ObjectRoot.B";
	objectB.attributeList.push_back("ba");
	objectB.attributeList.push_back("bb");
	objectB.attributeList.push_back("bc");
}

//�߼�������
void HLAPortAdapter::initial() {
	emit postUIMsg(initOneHandle(objectPause)->getMessage().c_str());
	emit postUIMsg(initOneHandle(objectContinue)->getMessage().c_str());
	emit postUIMsg(initOneHandle(objectEnd)->getMessage().c_str());

	//����
	emit postUIMsg(initOneHandle(objectA)->getMessage().c_str());
	emit postUIMsg(initOneHandle(objectB)->getMessage().c_str());
}

void HLAPortAdapter::pubAndSub() {

	emit postUIMsg(publishOne(objectPause)->getMessage().c_str());
	emit postUIMsg(publishOne(objectContinue)->getMessage().c_str());
	emit postUIMsg(publishOne(objectEnd)->getMessage().c_str());

	//����
	emit postUIMsg(subscribeOne(objectA)->getMessage().c_str());
	emit postUIMsg(subscribeOne(objectB)->getMessage().c_str());
}

void HLAPortAdapter::rigister() {
	InterfaceResponse<long> *ir;

	ir = registerObject(objectPause.className);
	emit postUIMsg(ir->getMessage().c_str());
	handlePause = ir->getData();

	ir = registerObject(objectContinue.className);
	emit postUIMsg(ir->getMessage().c_str());
	handleContinue = ir->getData();

	ir = registerObject(objectEnd.className);
	emit postUIMsg(ir->getMessage().c_str());
	handleEnd = ir->getData();
}

void HLAPortAdapter::simulate() {
	while (true) {
		if (isPause) {
			send(ControlCode::getPauseType());
			while (isPause) {
				//��ͣ״̬�µ����
				if (isEnd) {
					break;
				}
			}
			send(ControlCode::getContinueType());
		}
		if (isEnd) {
			del();
			//TODO�����ܳ�����bug����ʾPAUSE����������ûdel
			//ha->resignFederation();
			//����Ҳ���ٲ���
			//postUIMsg(ControlCode::getPrintCode(), ha->destroyFederation()->getMessage().c_str());
			send(ControlCode::getEndType());
			break;
		}
		advanceTime(1.0);
		emit postUITime(getCurrentTime()->getData());
	}
}

void HLAPortAdapter::del() {
	emit postUIMsg(deleteOne(handlePause)->getMessage().c_str());
	emit postUIMsg(deleteOne(handleContinue)->getMessage().c_str());
	emit postUIMsg(deleteOne(handleEnd)->getMessage().c_str());
}

void HLAPortAdapter::send(QString type) {
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create(1);
	char controlInfo[64];
	if (type == ControlCode::getPauseType()) {
		//���ܻ����¸������յ��������й����������ס��ʵ�����ղ�����
		//���Կ��ǵ��Ƕ����Լ��������Ļ��϶�ȫ���������¸������յ�
		//Ŀǰ������������������
		sprintf(controlInfo, "you should pause");
		attributes->add(objectPause.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		emit postUIMsg(sendObject(handlePause, attributes)->getMessage().c_str());
	}
	else if (type == ControlCode::getContinueType()) {
		sprintf(controlInfo, "you should continue");
		attributes->add(objectContinue.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		emit postUIMsg(sendObject(handleContinue, attributes)->getMessage().c_str());
	}
	else if (type == ControlCode::getEndType()) {
		sprintf(controlInfo, "you should end");
		attributes->add(objectEnd.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		emit postUIMsg(sendObject(handleEnd, attributes)->getMessage().c_str());
	}

}
//����ص�
void HLAPortAdapter::processData(RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes, const char* theTag)
	throw(RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
		RTI::FederateInternalError) {
	//������Ƶ��ʱ���Բ����д�ӡ
	QString s1 = "#callback# data received from : ";
	QString s2 = QString::number(theObject);
	emit postUIMsg(s1 + s2);

	//���ɶ�������ȡ�����������Լ���Զ��
	int classHandle = getObjectClass(theObject)->getData();
	if (classHandle == objectPause.classHandle) {
		emit postUIMsg("i should pause");
	}
	if (classHandle == objectContinue.classHandle) {
		emit postUIMsg("i should continue");
	}
	if (classHandle == objectEnd.classHandle) {
		emit postUIMsg("i should end");
	}
}

void HLAPortAdapter::syncSuccess(const char *label) {
	QString s1 = "#callback# Successfully registered sync point : ";
	QString s2 = label;
	emit postUIMsg(s1 + s2);
}

void HLAPortAdapter::HLAPortAdapter::syncFailure(const char *label) {
	QString s1 = "#callback# Failed to register sync point : ";
	QString s2 = label;
	emit postUIMsg(s1 + s2);
}

void HLAPortAdapter::announceSync(const char *label, const char *tag) {
	QString s1 = "#callback# Synchronization point announced : ";
	QString s2 = label;
	emit postUIMsg(s1 + s2);
}

void HLAPortAdapter::syncComplete(const char *label) {
	QString s1 = "#callback# Federation Synchronized : ";
	QString s2 = label;
	emit postUIMsg(s1 + s2);
}

void HLAPortAdapter::discover(RTI::ObjectHandle theObject,
	RTI::ObjectClassHandle theObjectClass,
	const char * theObjectName) {
	char str1[25], str2[25];
	_ultoa_s(theObject, str1, 10);
	_ultoa_s(theObjectClass, str2, 10);
	QString s1 = "#callback# Discoverd Object, name = ";
	QString s2 = theObjectName;
	QString s3 = " handle = ";
	QString s4 = str1;
	QString s5 = " classhandle = ";
	QString s6 = str2;
	emit postUIMsg(s1 + s2 + s3 + s4 + s5 + s6);
}

void HLAPortAdapter::remove(RTI::ObjectHandle theObject, const char *theTag) {
	char str1[25];
	_ultoa_s(theObject, str1, 10);
	QString s1 = "#callback# Object Removed, handle = ";
	QString s2 = str1;
	emit postUIMsg(s1 + s2);
}