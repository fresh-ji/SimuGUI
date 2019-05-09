#pragma once

#ifndef REGULATION_H
#define REGULATION_H

#include "cwidgets.h"
#include "ministack.h"
#include "fancydialog.h"
#include "fancybutton.h"

//数据类型管理部分
struct dataTypeInfo {
	QMap<QString, QString> dataMap;
};

//接口管理部分
struct interfaceInfo {
	QString dataType;
	QString publisher;
	QSet<QString> subscribers;
};
struct outputInfo {
	QString oName;
	QString oDataType;
	QString oPublisher;
};
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

//连接管理部分
struct connectInfo {
	//
};

static QList<QString> basicTypes = QList<QString>() << "real" << "int" << "bool" << "string" << "enum";

static QList<QString> forbidTypes = QList<QString>() << "new" << "struct" << "class" << "void" << "double"
<< "and" << "auto" << "bool" << "break" << "case" << "catch" << "char" << "class" << "const" << "const_cast"
<< "continue" << "default" << "delete" << "do" << "double" << "dynamic_cast" << "else" << "enum" << "explicit"
<< "export" << "extern" << "false" << "float" << "for" << "friend" << "goto" << "inline" << "int" << "namespace"
<< "long" << "if" << "operator" << "private" << "register" << "mutable" << "reinterpret_cast" << "throw"
<< "typeid" << "using" << "virtual" << "while" << "protected" << "public" << "return" << "signed" << "short"
<< "static" << "static_cast" << "sizeof" << "switch" << "true" << "template" << "this" << "typename" << "union"
<< "typedef" << "try" << "unsigned" << "volatile" << "wchar_t";

#endif // REGULATION_H