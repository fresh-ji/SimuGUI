#pragma once

/*
* FMI仿真引擎，64位进程版
* @date : 2019/06/21
* @author : jihang
*/

#include <map>
#include <set>
#include <string>

#include "Constant.h"

using namespace std;

struct FMIVariable {
	string causality;
	string name;
	string valueReference;
	string description;
	string variability;
	string initial;
	string canHandleMSPTI;
};

struct FMUInfo {
	bool isSuccess;
	string message;
	FMU fmu;
	//在本框架的基本信息
	string version;//版本号，目前仅支持2.0
	string platform;//WIN_64或WIN_32
	string simuType;//FMI_MODEL_EXCHANGE或FMI_COSIMULATION
	string modelId;//文件名
	string globalName;//带uuid的全局名
	string targetDir;//fmu目标文件夹
	string resultDir;//fmu结果文件夹
	string xmlPath;//xml路径
	string dllPath;//dll路径
	string resultFile;//最近一次的结果文件
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
};

void fmuLogger(void*, fmi2String, fmi2Status, fmi2String, fmi2String, ...);
void replaceRefsInMessage(const char*, char*, int, FMU*);
ScalarVariable* getSV(char, fmi2ValueReference, FMU*);
const char* fmi2StatusToString(fmi2Status);