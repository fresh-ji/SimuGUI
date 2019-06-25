#pragma once

/*
* @date : 2019/03/04
* @author : jihang
*/

#ifndef REGULATION_H
#define REGULATION_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancydialog.h"
#include "fancybutton.h"
#include "ItemElement.h"

//�������͹�����,��map����,name��map��key
struct dataTypeInfo {
	QMap<QString, QString> dataMap;
};

//���ӹ�����,��map����,name��map��key
struct connectInfo {
	ItemElement::Anchor anchor;
	QPoint start;
	QPoint end;
	double busRatio;
};

//�ӿڹ�����,��map����,name��map��key
struct interfaceInfo {
	QString dataType;
	QString publisher;
	QSet<QString> subscribers;
};
//����interface��dialog��ͨ�ŷ���
struct outputInfo {
	QString oName;
	QString oDataType;
	QString oPublisher;
};
//����interface��dialog��ͨ�ŷ���
struct inputInfo {
	QString iName;
	QString iDataType;
	QString iPublisher;
	bool isSubscribe;
	bool operator < (const inputInfo& i) const {
		return this->iName < i.iName;
	}
	bool operator == (const inputInfo& i) const {
		return this->iName == i.iName;
	}
};

#endif // REGULATION_H