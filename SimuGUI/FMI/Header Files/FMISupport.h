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
	//�ڱ���ܵĻ�����Ϣ
	string version;//�汾��
	string simuType;//ME��CS
	string modelId;//�ļ���
	string globalName;//��uuid��ȫ����
	string targetDir;//fmuĿ���ļ���
	string xmlPath;//xml·��
	string dllPath;//dll·��
	//xml��Ļ�����Ϣ
	map<string, string> basicInfo;
	//xml��ı�����Ϣ
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