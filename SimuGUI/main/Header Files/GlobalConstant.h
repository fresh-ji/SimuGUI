#pragma once

#ifndef GLOBALCONSTANT_H
#define GLOBALCONSTANT_H

#include "cwidgets.h"

#define ICON_PATH "./Icon/"

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
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

#endif // GLOBALCONSTANT_H