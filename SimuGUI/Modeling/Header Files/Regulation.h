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

//基本数据类型
static QList<QString> basicTypes = QList<QString>() << "real"
<< "int" << "bool" << "string" << "enum";

//命名规则中的禁止组合
static QList<QString> forbidTypes = QList<QString>() << "new" << "struct" << "class"
<< "void" << "double" << "and" << "auto" << "bool" << "break" << "case" << "catch"
<< "char" << "class" << "const" << "const_cast" << "continue" << "default" << "delete"
<< "do" << "double" << "dynamic_cast" << "else" << "enum" << "explicit" << "export"
<< "extern" << "false" << "float" << "for" << "friend" << "goto" << "inline" << "int"
<< "namespace" << "long" << "if" << "operator" << "private" << "register" << "mutable"
<< "reinterpret_cast" << "throw" << "typeid" << "using" << "virtual" << "while"
<< "protected" << "public" << "return" << "signed" << "short" << "static"
<< "static_cast" << "sizeof" << "switch" << "true" << "template" << "this"
<< "typename" << "union" << "typedef" << "try" << "unsigned" << "volatile" << "wchar_t";

//统一的表格样式
static void tableStandardize(QTableWidget* table) {

	//行头
	QFont font;
	font.setBold(true);
	table->horizontalHeader()->setFont(font);
	table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#93D5FD;}");

	//列头
	table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	table->verticalHeader()->setVisible(false);

	//选择模式
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setStyleSheet("selection-background-color:gray;");

	//滚动条
	table->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	table->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
}

#endif // REGULATION_H