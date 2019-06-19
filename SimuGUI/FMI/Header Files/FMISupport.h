#pragma once

#include <map>
#include <set>

#include "fmi2.h"

using namespace std;

constexpr auto FMI_MODEL_EXCHANGE = "Model Exchange";
constexpr auto FMI_COSIMULATION = "CoSimulation";

struct FMIVariable {
	string name;
	string valueReference;
	string description;
	string causality;
	string variability;
	string initial;
	string canHandleMSPTI;
};

struct FMUInfo {
	bool isSuccess;
	string message;
	FMU fmu;
	//在本框架的基本信息
	string version;//版本号
	string simuType;//ME或CS
	string modelId;//文件名
	string globalName;//带uuid的全局名
	string targetDir;//fmu目标文件夹
	string xmlPath;//xml路径
	string dllPath;//dll路径
	//xml里的基本信息
	map<string, string> basicInfo;
	//xml里的变量信息
	set<FMIVariable*> variableInfo;
};

class FMISupport {

public:
	FMISupport() {}
	virtual ~FMISupport() {}

public:
	FMUInfo loadFMU(const char*, string);
	//void unLoad();
	//bool simulateByCs(double, double, double, int, char **);
	//bool simulateByMe(double, double, double, int, char **);

private:
	bool loadDll(const char*, FMU*);
	void *getAdr(bool*, HMODULE, const char*);
	//void outputData(ofstream&, double, bool);

private:
	char* currentDir;
	string type;
};

//void fmuLogger(void*, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
//void replaceRefsInMessage(const char*, char*, int, FMU*);
//ScalarVariable* getSV(FMU*, char, fmi2ValueReference);
//const char* fmi2StatusToString(fmi2Status);