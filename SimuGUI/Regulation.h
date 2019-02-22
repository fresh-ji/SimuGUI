#pragma once

#ifndef REGULATION_H
#define REGULATION_H

#include "cwidgets.h"
#include "fancydialog.h"
#include "fancybutton.h"

struct dataTypeInfo {
	QMap<QString, QString> dataMap;
};

static QList<QString> basicTypes = QList<QString>() << "real" << "int" << "bool" << "string" << "enum";

static QList<QString> forbidTypes = QList<QString>() << "new" << "struct" << "class" << "void" << "double";

#endif // REGULATION_H