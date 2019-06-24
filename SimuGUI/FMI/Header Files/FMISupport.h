#pragma once

/*
* FMI�������棬64λ���̰�
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
	//�ڱ���ܵĻ�����Ϣ
	string version;//�汾�ţ�Ŀǰ��֧��2.0
	string platform;//WIN_64��WIN_32
	string simuType;//FMI_MODEL_EXCHANGE��FMI_COSIMULATION
	string modelId;//�ļ���
	string globalName;//��uuid��ȫ����
	string targetDir;//fmuĿ���ļ���
	string resultDir;//fmu����ļ���
	string xmlPath;//xml·��
	string dllPath;//dll·��
	string resultFile;//���һ�εĽ���ļ�
	//xml��Ļ�����Ϣ
	map<string, string> basicInfo;
	//xml��ı�����Ϣ
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