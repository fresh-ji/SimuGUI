#pragma once

/*
* 非常牛逼的FMI仿真引擎，写死tm我了
* @date : 2019/06/21
* @author : jihang
*/

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
	string resultDir;//fmu结果文件夹
	string xmlPath;//xml路径
	string dllPath;//dll路径
	//xml里的基本信息
	map<string, string> basicInfo;
	//xml里的变量信息
	set<FMIVariable*> variableInfo;
};

struct simuInfo {
	bool isSucess;
	string message;
	string resultPath;
};

class FMISupport {

public:
	FMISupport() {}
	virtual ~FMISupport() {}

public:
	FMUInfo loadFMU(const char*, string);
	simuInfo simulateByCs(FMU, string, double, double, double, int, char **);
	simuInfo simulateByMe(FMU, string, double, double, double, int, char **);
	void unLoad();

private:
	bool loadDll(const char*, FMU*);
	void *getAdr(bool*, HMODULE, const char*);
	void outputData(ofstream&, double, bool, FMU, fmi2Component);	

private:
	char* currentDir;
	string type;
	FMU currentFMU;
};

void fmuLogger(void*, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
void replaceRefsInMessage(const char*, char*, int, FMU*);
ScalarVariable* getSV(char, fmi2ValueReference, FMU*);
const char* fmi2StatusToString(fmi2Status);