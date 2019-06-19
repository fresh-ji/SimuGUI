#pragma once

#include <QObject>
#include <QFileInfo>
#include <QUuid>
#include <QMap>
#include <QSet>

#include "fmi2.h"

constexpr auto FMI_MODEL_EXCHANGE = "Model Exchange";
constexpr auto FMI_COSIMULATION = "CoSimulation";

struct FMIVariable {
	QString name;
	QString valueReference;
	QString description;
	QString causality;
	QString variability;
	QString initial;
	QString canHandleMSPTI;
};

struct FMUInfo {
	bool isSuccess;
	QString message;
	FMU fmu;
	//在本框架的基本信息
	QString version;//版本号
	QString simuType;//ME或CS
	QString modelId;//文件名
	QString globalName;//带uuid的全局名
	QString targetDir;//fmu目标文件夹
	QString xmlPath;//xml路径
	QString dllPath;//dll路径
	//xml里的基本信息
	QMap<QString, QString> basicInfo;
	//xml里的变量信息
	QSet<FMIVariable*> variableInfo;
};

class FMISupport : public QObject {

	Q_OBJECT

public:
	explicit FMISupport(QObject *parent = NULL);
	virtual ~FMISupport() {};

public:
	FMUInfo loadFMU(QString);
	//void unLoad();
	//bool simulateByCs(double, double, double, int, char **);
	//bool simulateByMe(double, double, double, int, char **);

private:
	bool loadDll(const char*, FMU*);
	void *getAdr(bool*, HMODULE, const char*);
	//void outputData(ofstream&, double, bool);

private:
	char* currentDir;
	QString type;

signals:
	void postUIMsg(QString);
};

//void fmuLogger(void*, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
//void replaceRefsInMessage(const char*, char*, int, FMU*);
//ScalarVariable* getSV(FMU*, char, fmi2ValueReference);
//const char* fmi2StatusToString(fmi2Status);