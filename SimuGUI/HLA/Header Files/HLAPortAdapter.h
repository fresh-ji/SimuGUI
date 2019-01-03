#pragma once

/*
* RunControlAdapter��
* @date : 2018/10/13
* @author : jihang
*/

#include <QObject>

#include "HLAPortInterface.h"

class HLAPortAdapter : public QObject, public CHLAPortInterface {

	Q_OBJECT

public:
	//1.�����ඨ��
	//������ƣ�Ŀǰ�����飬ȫ������
	ObjectClassSet objectPause;
	RTI::ObjectHandle handlePause;

	ObjectClassSet objectContinue;
	RTI::ObjectHandle handleContinue;

	ObjectClassSet objectEnd;
	RTI::ObjectHandle handleEnd;

	//���ģ��Ƿ��ܻ��ȫ�����У�
	ObjectClassSet objectA;
	ObjectClassSet objectB;

public:
	//2.���캯��
	explicit HLAPortAdapter(QObject *parent = NULL);
	virtual ~HLAPortAdapter() {};

public:
	void initial();

	void pubAndSub();

	void rigister();

	void simulate();

	void send(QString type);

	void del();

	//=======================����Ϊ�ص�==========================
	void processData(RTI::ObjectHandle, const RTI::AttributeHandleValuePairSet&, const char*);

	void syncSuccess(const char*);

	void syncFailure(const char*);

	void announceSync(const char*, const char*);

	void syncComplete(const char*);

	void discover(RTI::ObjectHandle, RTI::ObjectClassHandle, const char*);

	void remove(RTI::ObjectHandle, const char*);

signals:
	//����UI���ź�
	void postUIMsg(QString);
	//����UI��ʱ��
	void postUITime(double);
};