#pragma once

#include <QObject>
#include <QFileInfo>
#include <QUuid>

#include <fstream>

#include "fmi2.h"

constexpr auto FMI_MODEL_EXCHANGE = "Model Exchange";
constexpr auto FMI_COSIMULATION = "CoSimulation";

using namespace std;

struct FMUInfo {
	bool isSuccess;
	QString message;
	FMU fmu;
	QString version;
	QString simuType;
	QString globalName;
	QString targetDir;
	QString descriptionPath;
	QString dllPath;
};

class FMISupport : public QObject {

	Q_OBJECT

public:
	explicit FMISupport(QObject *parent = NULL);
	virtual ~FMISupport() {};

public:
	FMUInfo loadFMU(QString);
	void unLoad();
	bool simulateByCs(double, double, double, int, char **);
	bool simulateByMe(double, double, double, int, char **);

private:
	bool loadDll(const char*, FMU*);
	void *getAdr(bool*, HMODULE, const char*);
	void outputData(ofstream&, double, bool);

private:
	char* currentDir;
	FMU fmu0;
	QString type;
	fmi2Component c;

signals:
	void postUIMsg(QString);
};

void fmuLogger(void*, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
void replaceRefsInMessage(const char*, char*, int, FMU*);
ScalarVariable* getSV(FMU*, char, fmi2ValueReference);
const char* fmi2StatusToString(fmi2Status);