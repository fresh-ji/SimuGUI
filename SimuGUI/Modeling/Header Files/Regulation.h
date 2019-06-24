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

//������������
static QList<QString> basicTypes = QList<QString>() << "real"
<< "int" << "bool" << "string" << "enum";

//���������еĽ�ֹ���
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

//ͳһ�ı����ʽ
static void tableStandardize(QTableWidget* table) {

	//��ͷ
	QFont font;
	font.setBold(true);
	table->horizontalHeader()->setFont(font);
	table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#93D5FD;}");

	//��ͷ
	table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	table->verticalHeader()->setVisible(false);

	//ѡ��ģʽ
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setStyleSheet("selection-background-color:gray;");

	//������
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