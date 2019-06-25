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

//数据类型管理部分,以map管理,name是map的key
struct dataTypeInfo {
	QMap<QString, QString> dataMap;
};

//连接管理部分,以map管理,name是map的key
struct connectInfo {
	ItemElement::Anchor anchor;
	QPoint start;
	QPoint end;
	double busRatio;
};

//接口管理部分,以map管理,name是map的key
struct interfaceInfo {
	QString dataType;
	QString publisher;
	QSet<QString> subscribers;
};
//用作interface与dialog的通信返回
struct outputInfo {
	QString oName;
	QString oDataType;
	QString oPublisher;
};
//用作interface与dialog的通信返回
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