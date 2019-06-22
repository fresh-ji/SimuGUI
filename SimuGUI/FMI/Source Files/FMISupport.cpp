
#ifdef WIN32
#include <direct.h>    
#define GetCurrentDir _getcwd
#else    
#include <unistd.h>    
#define GetCurrentDir getcwd 
#endif

#include "FMISupport.h"
#include "xmlVersionParser.h"
#include "ErrorLog.h"

#define DIRECTORY_PATH_SIZE 1024
static char currentDirectory[DIRECTORY_PATH_SIZE];

FMUInfo FMISupport::loadFMU(const char* filePath, string uuid) {

	FMUInfo info;
	info.isSuccess = false;
	//TODO：所有错误回复都要清理解压缩文件及内存回收

	//判断fmu文件是否存在
	if (fopen(filePath, "r") == NULL) {
		info.message = "fmu file not exists";
		return info;
	}

	//获取当前目录
	currentDir = GetCurrentDir(currentDirectory, DIRECTORY_PATH_SIZE);

	//命名，添加一个uuid
	string str_filePath = filePath;
	string globalName = str_filePath.substr(str_filePath.find_last_of('/') + 1);
	globalName.append(uuid);
	string targetDir;

	targetDir.append(currentDir)
		.append(UNZIP_DIR).append(globalName).append("\\");

	//拼接cmd命令，原先用的是sstream，清空需要(ss.clear() + ss.str(""))
	string command;
	command.append(UNZIP_CMD).append("\"").append(targetDir)
		.append("\"").append(" \"").append(filePath).append("\" > NUL");

	//解压缩
	int code = system(command.data());
	switch (code) {
	case SEVEN_ZIP_NO_ERROR: break;
	case SEVEN_ZIP_WARNING: info.message = "unzip warning"; return info;
	case SEVEN_ZIP_ERROR: info.message = "unzip error"; return info;
	case SEVEN_ZIP_COMMAND_LINE_ERROR: info.message = "unzip command line error"; return info;
	case SEVEN_ZIP_OUT_OF_MEMORY: info.message = "unzip out of memory"; return info;
	case SEVEN_ZIP_STOPPED_BY_USER: info.message = "unzip stopped by user"; return info;
	default: info.message = "unzip unknown problem"; return info;
	}

	string resultDir;
	resultDir.append(currentDir).append(RESULT_DIR).append(globalName);

	string xmlPath = targetDir;
	xmlPath.append(MD_FILE_NAME);

	//校验及写入版本号，目前仅支持2.0
	//TODO:注掉了一句关闭xml的话
	char* xmlFmiVersion = extractVersion(xmlPath.data());
	if (xmlFmiVersion == NULL) {
		info.message = "The FMI version of the FMU could not be read";
		return info;
	}
	if (strcmp(xmlFmiVersion, fmi2Version) != 0) {
		info.message = "The FMI version is not 2.0";
		return info;
	}
	info.version = xmlFmiVersion;

	//模型基础
	FMU fmu;

	//TODO:注掉了一句关闭xml的话
	fmu.modelDescription = parse(const_cast<char*>(xmlPath.c_str()));

	//获取类型
	Component *component = getModelExchange(fmu.modelDescription);
	if (!component) {
		component = getCoSimulation(fmu.modelDescription);
		if (!component) {
			info.message = "model simulation type wrong";
			return info;
		}
		else {
			info.simuType = FMI_COSIMULATION;
			type = FMI_COSIMULATION;
		}
	}
	else {
		info.simuType = FMI_MODEL_EXCHANGE;
		type = FMI_MODEL_EXCHANGE;
	}

	//获取id
	info.modelId = getAttributeValue((Element*)component, att_modelIdentifier);

	//获取dll路径并写入全名和三个路径
	string dllPath = targetDir;
	dllPath.append(DLL_DIR).append(info.modelId).append(".dll");
	info.globalName = globalName;
	info.targetDir = targetDir;
	info.resultDir = resultDir;
	info.xmlPath = xmlPath;
	info.dllPath = dllPath;

	//加载dll功能
	if (!loadDll(dllPath.data(), &fmu)) {
		info.message = "64 process not support 32 model";
		return info;
	}

	Element* ele;
	const char* result;
	//头部基本信息
	ele = (Element*)(fmu.modelDescription);
	result = getAttributeValue(ele, att_modelName);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("modelName", result));
	}
	result = getAttributeValue(ele, att_guid);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("guid", result));
	}
	result = getAttributeValue(ele, att_description);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("description", result));
	}
	result = getAttributeValue(ele, att_author);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("author", result));
	}
	result = getAttributeValue(ele, att_copyright);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("copyright", result));
	}
	result = getAttributeValue(ele, att_license);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("license", result));
	}
	result = getAttributeValue(ele, att_generationTool);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("generationTool", result));
	}
	result = getAttributeValue(ele, att_generationDateAndTime);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("generationDateAndTime", result));
	}
	result = getAttributeValue(ele, att_variableNamingConvention);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("variableNamingConvention", result));
	}
	result = getAttributeValue(ele, att_numberOfEventIndicators);
	if (result != NULL) {
		info.basicInfo.insert(make_pair("numberOfEventIndicators", result));
	}
	//变量信息
	for (int i = 0; i < getScalarVariableSize(fmu.modelDescription); ++i) {
		ele = (Element*)getScalarVariable(fmu.modelDescription, i);
		FMIVariable* fv = new FMIVariable();
		result = getAttributeValue(ele, att_name);
		if (result != NULL) {
			fv->name = result;
		}
		result = getAttributeValue(ele, att_valueReference);
		if (result != NULL) {
			fv->valueReference = result;
		}
		result = getAttributeValue(ele, att_description);
		if (result != NULL) {
			fv->description = result;
		}
		result = getAttributeValue(ele, att_causality);
		if (result != NULL) {
			fv->causality = result;
		}
		result = getAttributeValue(ele, att_variability);
		if (result != NULL) {
			fv->variability = result;
		}
		result = getAttributeValue(ele, att_initial);
		if (result != NULL) {
			fv->initial = result;
		}
		result = getAttributeValue(ele, att_canHandleMultipleSetPerTimeInstant);
		if (result != NULL) {
			fv->canHandleMSPTI = result;
		}
		info.variableInfo.insert(fv);
	}

	//结束并返回
	info.fmu = fmu;
	info.isSuccess = true;
	info.message = "success";
	return info;
}

bool FMISupport::loadDll(const char* dllPath, FMU* fmu) {

	bool s = true;

	HMODULE h = LoadLibrary(dllPath);
	if (!h) {
		return false;
	}

	fmu->dllHandle = h;
	fmu->getTypesPlatform = (fmi2GetTypesPlatformTYPE *)getAdr(&s, h, "fmi2GetTypesPlatform");
	fmu->getVersion = (fmi2GetVersionTYPE *)getAdr(&s, h, "fmi2GetVersion");
	fmu->setDebugLogging = (fmi2SetDebugLoggingTYPE *)getAdr(&s, h, "fmi2SetDebugLogging");
	fmu->instantiate = (fmi2InstantiateTYPE *)getAdr(&s, h, "fmi2Instantiate");
	fmu->freeInstance = (fmi2FreeInstanceTYPE *)getAdr(&s, h, "fmi2FreeInstance");
	fmu->setupExperiment = (fmi2SetupExperimentTYPE *)getAdr(&s, h, "fmi2SetupExperiment");
	fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *)getAdr(&s, h, "fmi2EnterInitializationMode");
	fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *)getAdr(&s, h, "fmi2ExitInitializationMode");
	fmu->terminate = (fmi2TerminateTYPE *)getAdr(&s, h, "fmi2Terminate");
	fmu->reset = (fmi2ResetTYPE *)getAdr(&s, h, "fmi2Reset");
	fmu->getReal = (fmi2GetRealTYPE *)getAdr(&s, h, "fmi2GetReal");
	fmu->getInteger = (fmi2GetIntegerTYPE *)getAdr(&s, h, "fmi2GetInteger");
	fmu->getBoolean = (fmi2GetBooleanTYPE *)getAdr(&s, h, "fmi2GetBoolean");
	fmu->getString = (fmi2GetStringTYPE *)getAdr(&s, h, "fmi2GetString");
	fmu->setReal = (fmi2SetRealTYPE *)getAdr(&s, h, "fmi2SetReal");
	fmu->setInteger = (fmi2SetIntegerTYPE *)getAdr(&s, h, "fmi2SetInteger");
	fmu->setBoolean = (fmi2SetBooleanTYPE *)getAdr(&s, h, "fmi2SetBoolean");
	fmu->setString = (fmi2SetStringTYPE *)getAdr(&s, h, "fmi2SetString");
	fmu->getFMUstate = (fmi2GetFMUstateTYPE *)getAdr(&s, h, "fmi2GetFMUstate");
	fmu->setFMUstate = (fmi2SetFMUstateTYPE *)getAdr(&s, h, "fmi2SetFMUstate");
	fmu->freeFMUstate = (fmi2FreeFMUstateTYPE *)getAdr(&s, h, "fmi2FreeFMUstate");
	fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *)getAdr(&s, h, "fmi2SerializedFMUstateSize");
	fmu->serializeFMUstate = (fmi2SerializeFMUstateTYPE *)getAdr(&s, h, "fmi2SerializeFMUstate");
	fmu->deSerializeFMUstate = (fmi2DeSerializeFMUstateTYPE *)getAdr(&s, h, "fmi2DeSerializeFMUstate");
	fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *)getAdr(&s, h, "fmi2GetDirectionalDerivative");

	if (type == FMI_MODEL_EXCHANGE) {
		fmu->enterEventMode = (fmi2EnterEventModeTYPE *)getAdr(&s, h, "fmi2EnterEventMode");
		fmu->newDiscreteStates = (fmi2NewDiscreteStatesTYPE *)getAdr(&s, h, "fmi2NewDiscreteStates");
		fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *)getAdr(&s, h, "fmi2EnterContinuousTimeMode");
		fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *)getAdr(&s, h, "fmi2CompletedIntegratorStep");
		fmu->setTime = (fmi2SetTimeTYPE *)getAdr(&s, h, "fmi2SetTime");
		fmu->setContinuousStates = (fmi2SetContinuousStatesTYPE *)getAdr(&s, h, "fmi2SetContinuousStates");
		fmu->getDerivatives = (fmi2GetDerivativesTYPE *)getAdr(&s, h, "fmi2GetDerivatives");
		fmu->getEventIndicators = (fmi2GetEventIndicatorsTYPE *)getAdr(&s, h, "fmi2GetEventIndicators");
		fmu->getContinuousStates = (fmi2GetContinuousStatesTYPE *)getAdr(&s, h, "fmi2GetContinuousStates");
		fmu->getNominalsOfContinuousStates = (fmi2GetNominalsOfContinuousStatesTYPE *)getAdr(&s, h, "fmi2GetNominalsOfContinuousStates");
	}
	else if (type == FMI_COSIMULATION) {
		fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *)getAdr(&s, h, "fmi2SetRealInputDerivatives");
		fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *)getAdr(&s, h, "fmi2GetRealOutputDerivatives");
		fmu->doStep = (fmi2DoStepTYPE *)getAdr(&s, h, "fmi2DoStep");
		fmu->cancelStep = (fmi2CancelStepTYPE *)getAdr(&s, h, "fmi2CancelStep");
		fmu->getStatus = (fmi2GetStatusTYPE *)getAdr(&s, h, "fmi2GetStatus");
		fmu->getRealStatus = (fmi2GetRealStatusTYPE *)getAdr(&s, h, "fmi2GetRealStatus");
		fmu->getIntegerStatus = (fmi2GetIntegerStatusTYPE *)getAdr(&s, h, "fmi2GetIntegerStatus");
		fmu->getBooleanStatus = (fmi2GetBooleanStatusTYPE *)getAdr(&s, h, "fmi2GetBooleanStatus");
		fmu->getStringStatus = (fmi2GetStringStatusTYPE *)getAdr(&s, h, "fmi2GetStringStatus");
	}

	if (fmu->getVersion == NULL && fmu->instantiate == NULL) {
		//printf("warning: Functions from FMI 2.0 could not be found in %s\n", dllPath);
		//printf("warning: Simulator will look for FMI 2.0 RC1 functions names...\n");
		s = true;
		fmu->getTypesPlatform = (fmi2GetTypesPlatformTYPE *)getAdr(&s, h, "fmiGetTypesPlatform");
		fmu->getVersion = (fmi2GetVersionTYPE *)getAdr(&s, h, "fmiGetVersion");
		fmu->setDebugLogging = (fmi2SetDebugLoggingTYPE *)getAdr(&s, h, "fmiSetDebugLogging");
		fmu->instantiate = (fmi2InstantiateTYPE *)getAdr(&s, h, "fmiInstantiate");
		fmu->freeInstance = (fmi2FreeInstanceTYPE *)getAdr(&s, h, "fmiFreeInstance");
		fmu->setupExperiment = (fmi2SetupExperimentTYPE *)getAdr(&s, h, "fmiSetupExperiment");
		fmu->enterInitializationMode = (fmi2EnterInitializationModeTYPE *)getAdr(&s, h, "fmiEnterInitializationMode");
		fmu->exitInitializationMode = (fmi2ExitInitializationModeTYPE *)getAdr(&s, h, "fmiExitInitializationMode");
		fmu->terminate = (fmi2TerminateTYPE *)getAdr(&s, h, "fmiTerminate");
		fmu->reset = (fmi2ResetTYPE *)getAdr(&s, h, "fmiReset");
		fmu->getReal = (fmi2GetRealTYPE *)getAdr(&s, h, "fmiGetReal");
		fmu->getInteger = (fmi2GetIntegerTYPE *)getAdr(&s, h, "fmiGetInteger");
		fmu->getBoolean = (fmi2GetBooleanTYPE *)getAdr(&s, h, "fmiGetBoolean");
		fmu->getString = (fmi2GetStringTYPE *)getAdr(&s, h, "fmiGetString");
		fmu->setReal = (fmi2SetRealTYPE *)getAdr(&s, h, "fmiSetReal");
		fmu->setInteger = (fmi2SetIntegerTYPE *)getAdr(&s, h, "fmiSetInteger");
		fmu->setBoolean = (fmi2SetBooleanTYPE *)getAdr(&s, h, "fmiSetBoolean");
		fmu->setString = (fmi2SetStringTYPE *)getAdr(&s, h, "fmiSetString");
		fmu->getFMUstate = (fmi2GetFMUstateTYPE *)getAdr(&s, h, "fmiGetFMUstate");
		fmu->setFMUstate = (fmi2SetFMUstateTYPE *)getAdr(&s, h, "fmiSetFMUstate");
		fmu->freeFMUstate = (fmi2FreeFMUstateTYPE *)getAdr(&s, h, "fmiFreeFMUstate");
		fmu->serializedFMUstateSize = (fmi2SerializedFMUstateSizeTYPE *)getAdr(&s, h, "fmiSerializedFMUstateSize");
		fmu->serializeFMUstate = (fmi2SerializeFMUstateTYPE *)getAdr(&s, h, "fmiSerializeFMUstate");
		fmu->deSerializeFMUstate = (fmi2DeSerializeFMUstateTYPE *)getAdr(&s, h, "fmiDeSerializeFMUstate");
		fmu->getDirectionalDerivative = (fmi2GetDirectionalDerivativeTYPE *)getAdr(&s, h, "fmiGetDirectionalDerivative");
		if (type == FMI_MODEL_EXCHANGE) {
			fmu->enterEventMode = (fmi2EnterEventModeTYPE *)getAdr(&s, h, "fmiEnterEventMode");
			fmu->newDiscreteStates = (fmi2NewDiscreteStatesTYPE *)getAdr(&s, h, "fmiNewDiscreteStates");
			fmu->enterContinuousTimeMode = (fmi2EnterContinuousTimeModeTYPE *)getAdr(&s, h, "fmiEnterContinuousTimeMode");
			fmu->completedIntegratorStep = (fmi2CompletedIntegratorStepTYPE *)getAdr(&s, h, "fmiCompletedIntegratorStep");
			fmu->setTime = (fmi2SetTimeTYPE *)getAdr(&s, h, "fmiSetTime");
			fmu->setContinuousStates = (fmi2SetContinuousStatesTYPE *)getAdr(&s, h, "fmiSetContinuousStates");
			fmu->getDerivatives = (fmi2GetDerivativesTYPE *)getAdr(&s, h, "fmiGetDerivatives");
			fmu->getEventIndicators = (fmi2GetEventIndicatorsTYPE *)getAdr(&s, h, "fmiGetEventIndicators");
			fmu->getContinuousStates = (fmi2GetContinuousStatesTYPE *)getAdr(&s, h, "fmiGetContinuousStates");
			fmu->getNominalsOfContinuousStates = (fmi2GetNominalsOfContinuousStatesTYPE *)getAdr(&s, h, "fmiGetNominalsOfContinuousStates");
		}
		else if (type == FMI_COSIMULATION) {
			fmu->setRealInputDerivatives = (fmi2SetRealInputDerivativesTYPE *)getAdr(&s, h, "fmiSetRealInputDerivatives");
			fmu->getRealOutputDerivatives = (fmi2GetRealOutputDerivativesTYPE *)getAdr(&s, h, "fmiGetRealOutputDerivatives");
			fmu->doStep = (fmi2DoStepTYPE *)getAdr(&s, h, "fmiDoStep");
			fmu->cancelStep = (fmi2CancelStepTYPE *)getAdr(&s, h, "fmiCancelStep");
			fmu->getStatus = (fmi2GetStatusTYPE *)getAdr(&s, h, "fmiGetStatus");
			fmu->getRealStatus = (fmi2GetRealStatusTYPE *)getAdr(&s, h, "fmiGetRealStatus");
			fmu->getIntegerStatus = (fmi2GetIntegerStatusTYPE *)getAdr(&s, h, "fmiGetIntegerStatus");
			fmu->getBooleanStatus = (fmi2GetBooleanStatusTYPE *)getAdr(&s, h, "fmiGetBooleanStatus");
			fmu->getStringStatus = (fmi2GetStringStatusTYPE *)getAdr(&s, h, "fmiGetStringStatus");
		}
	}
	return s;
}

void* FMISupport::getAdr(bool *success, HMODULE dllHandle, const char *functionName) {
	void* fp = GetProcAddress(dllHandle, functionName);
	if (!fp) {
		*success = false;
	}
	return fp;
}

simuInfo FMISupport::simulateByCs(
	FMU fmu,
	string resultDir,
	double tStart,
	double tEnd,
	double h,
	int nCategories,
	char **categories) {

	simuInfo info;
	info.isSucess = false;

	ModelDescription* md = fmu.modelDescription;
	//获取信息
	const char* guid = getAttributeValue((Element*)md, att_guid);
	const char *instanceName = getAttributeValue((Element*)getCoSimulation(md), att_modelIdentifier);

	//回调
	fmi2CallbackFunctions callbacks = { fmuLogger, calloc, free, NULL, &fmu };

	//是否有可视化组件
	fmi2Boolean visible = fmi2False;

	//最后一项为loggingOn
	fmi2Component c = fmu.instantiate(instanceName, fmi2CoSimulation,
		guid, resultDir.c_str(), &callbacks, visible, true);

	if (!c) {
		info.message = "could not instantiate model";
		return info;
	}

	if (nCategories > 0) {
		fmi2Status fmi2Flag = fmu.setDebugLogging(c, fmi2True, nCategories, categories);
		if (fmi2Flag > fmi2Warning) {
			info.message = "failed FMI set debug logging";
			return info;
		}
	}

	Element *defaultExp = getDefaultExperiment(md);
	fmi2Boolean toleranceDefined = fmi2False;
	fmi2Real tolerance = 0;

	ValueStatus vs;
	if (defaultExp) {
		tolerance = getAttributeDouble(defaultExp, att_tolerance, &vs);
		if (vs == valueDefined) {
			toleranceDefined = fmi2True;
		}
	}

	fmi2Status fmi2Flag = fmu.setupExperiment(c,
		toleranceDefined, tolerance, tStart, fmi2True, tEnd);

	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI setup experiment";
		return info;
	}

	fmi2Flag = fmu.enterInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI enter initialization mode";
		return info;
	}

	fmi2Flag = fmu.exitInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI exit initialization mode";
		return info;
	}

	//结果输出
	string resultFile = resultDir + "-CS-" + LOG::getCurrentTime() + ".csv";
	ofstream dataFile(resultFile, ios::ate);

	outputData(dataFile, tStart, true, fmu, c);
	outputData(dataFile, tStart, false, fmu, c);

	int nSteps = 0;
	double hh = h;
	double time = tStart;
	while (time < tEnd) {
		//最后不足一步按实际来
		if (h > tEnd - time) {
			hh = tEnd - time;
		}
		fmi2Flag = fmu.doStep(c, time, hh, fmi2True);
		if (fmi2Flag == fmi2Discard) {
			fmi2Boolean b;
			if (fmi2OK != fmu.getBooleanStatus(c, fmi2Terminated, &b)) {
				info.message =
					"could not complete simulation of the model. getBooleanStatus return other than fmi2OK";
				return info;
			}
			if (b == fmi2True) {
				break;
			}
			info.message = "could not complete simulation of the model 1";
			return info;
		}
		if (fmi2Flag != fmi2OK) {
			info.message = "could not complete simulation of the model 2";
			return info;
		}
		time += hh;
		outputData(dataFile, time, false, fmu, c);
		//计步
		nSteps++;
	}

	info.message += "Simulation from ";
	info.message += to_string(tStart);
	info.message += " to ";
	info.message += to_string(time);

	info.message += "   steps = ";
	info.message += to_string(nSteps);

	info.message += "   fixed step size = ";
	info.message += to_string(h);

	info.isSucess = true;
	info.resultPath = resultFile;
	return info;
}

simuInfo FMISupport::simulateByMe(
	FMU fmu,
	string resultDir,
	double tStart,
	double tEnd,
	double h,
	int nCategories,
	char **categories) {

	simuInfo info;
	info.isSucess = false;

	bool loggingOn = true;

	ModelDescription* md = fmu.modelDescription;
	//获取信息
	const char* guid = getAttributeValue((Element*)md, att_guid);
	const char* instanceName = getAttributeValue(
		(Element*)getModelExchange(md), att_modelIdentifier);

	//回调
	fmi2CallbackFunctions callbacks = { fmuLogger, calloc, free, NULL, &fmu };

	//无可视化组件
	fmi2Boolean visible = fmi2False;

	//最后一项为loggingOn
	fmi2Component c = fmu.instantiate(instanceName, fmi2ModelExchange,
		guid, resultDir.c_str(), &callbacks, visible, loggingOn);
	if (!c) {
		info.message = "could not instantiate model";
		return info;
	}

	if (nCategories > 0) {
		fmi2Status fmi2Flag = fmu.setDebugLogging(c, fmi2True, nCategories, categories);
		if (fmi2Flag > fmi2Warning) {
			info.message = "failed FMI set debug logging";
			return info;
		}
	}

	//状态数
	int nx = getDerivativesSize(getModelStructure(md));
	//indicators数
	ValueStatus vs;
	int nz = getAttributeInt((Element*)md, att_numberOfEventIndicators, &vs);
	//各个状态，是double类型的，对应于real
	double *x = NULL;
	x = (double*)calloc(nx, sizeof(double));
	//各个状态微分，是double类型的，对应于real
	double *xdot = NULL;
	xdot = (double*)calloc(nx, sizeof(double));
	//各个indicators
	double *z = NULL;
	//前一个indicator
	double *prez = NULL;
	if (nz > 0) {
		z = (double*)calloc(nz, sizeof(double));
		prez = (double*)calloc(nz, sizeof(double));
	}
	if ((!x || !xdot) || (nz > 0 && (!z || !prez))) {
		info.message = "out of memory";
		return info;
	}

	fmi2Boolean toleranceDefined = fmi2False;
	fmi2Real tolerance = 0;
	//准备
	fmi2Status fmi2Flag = fmu.setupExperiment(c, toleranceDefined, tolerance, tStart, fmi2True, tEnd);
	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI setup experiment";
		return info;
	}

	//模型初始化
	fmi2Flag = fmu.enterInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI enter initialization mode";
		return info;
	}
	fmi2Flag = fmu.exitInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		info.message = "failed FMI exit initialization mode";
		return info;
	}

	//事件迭代
	fmi2EventInfo eventInfo;
	eventInfo.newDiscreteStatesNeeded = fmi2True;
	eventInfo.terminateSimulation = fmi2False;
	while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
		fmi2Flag = fmu.newDiscreteStates(c, &eventInfo);
		if (fmi2Flag > fmi2Warning) {
			info.message = "could not set a new discrete state";
			return info;
		}
	}

	string resultFile = resultDir + "-ME-" + LOG::getCurrentTime() + ".csv";
	ofstream dataFile(resultFile, ios::ate);

	double time = tStart;
	double dt, tPre;
	fmi2Boolean timeEvent, stateEvent, stepEvent, terminateSimulation;
	int nSteps = 0;
	int nTimeEvents = 0;
	int nStateEvents = 0;
	int nStepEvents = 0;
	if (eventInfo.terminateSimulation) {
		info.message = "model requested termination at t = " + to_string(time);
	}
	else {
		fmu.enterContinuousTimeMode(c);

		outputData(dataFile, tStart, true, fmu, c);
		outputData(dataFile, tStart, false, fmu, c);

		while (time < tEnd) {
			//1.获取当前状态
			fmi2Flag = fmu.getContinuousStates(c, x, nx);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not retrieve states";
				return info;
			}
			//2.获取状态导数
			fmi2Flag = fmu.getDerivatives(c, xdot, nx);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not retrieve derivatives";
				return info;
			}
			//3.时间事件
			tPre = time;
			time = min(time + h, tEnd);
			timeEvent = eventInfo.nextEventTimeDefined && (eventInfo.nextEventTime < time);
			if (timeEvent) {
				time = eventInfo.nextEventTime;
			}
			dt = time - tPre;
			fmi2Flag = fmu.setTime(c, time);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not set time";
				return info;
			}
			//4.前进一步
			for (int i = 0; i < nx; i++) {
				//欧拉折线
				x[i] += dt * xdot[i];
			}
			fmi2Flag = fmu.setContinuousStates(c, x, nx);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not set states";
				return info;
			}

			//"Step " << nSteps << " to t = " << time

			//5.检查状态事件
			for (int i = 0; i < nz; i++) {
				prez[i] = z[i];
			}
			fmi2Flag = fmu.getEventIndicators(c, z, nz);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not retrieve event indicators";
				return info;
			}
			stateEvent = FALSE;
			for (int i = 0; i < nz; i++) {
				stateEvent = stateEvent || (prez[i] * z[i] < 0);
			}
			//6.检查步进事件
			fmi2Flag = fmu.completedIntegratorStep(c, fmi2True, &stepEvent, &terminateSimulation);
			if (fmi2Flag > fmi2Warning) {
				info.message = "could not complete integrator step";
				return info;
			}
			if (terminateSimulation) {
				break;
			}
			//7.处理事件
			if (timeEvent || stateEvent || stepEvent) {
				fmu.enterEventMode(c);
				if (timeEvent) {
					nTimeEvents++;
					//"time event at t=" << time
				}
				if (stateEvent) {
					nStateEvents++;
					for (int i = 0; i < nz; i++) {
						//"state event " << ((prez[i] > 0 && z[i] < 0) ? "-\\-" : "-/-") << " z[" << i << "] at t=" << time
					}
				}
				if (stepEvent) {
					nStepEvents++;
					//"step event at t = " << time
				}
				eventInfo.newDiscreteStatesNeeded = fmi2True;
				eventInfo.terminateSimulation = fmi2False;
				while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
					fmi2Flag = fmu.newDiscreteStates(c, &eventInfo);
					if (fmi2Flag > fmi2Warning) {
						info.message = "could not set a new discrete state";
						return info;
					}
					if (eventInfo.valuesOfContinuousStatesChanged) {
						//"continuous state values changed at t=" + QString::number(time)
					}
					if (eventInfo.nominalsOfContinuousStatesChanged) {
						//"nominals of continuous state changed  at t=" + QString::number(time)
					}
				}
				if (eventInfo.terminateSimulation) {
					break;
				}
				fmu.enterContinuousTimeMode(c);
			}
			outputData(dataFile, time, false, fmu, c);
			nSteps++;
		}
	}

	info.message += "Simulation from ";
	info.message += to_string(tStart);
	info.message += " to ";
	info.message += to_string(time);

	info.message += "   steps = ";
	info.message += to_string(nSteps);

	info.message += "   fixed step size = ";
	info.message += to_string(h);

	info.message += "   time events = ";
	info.message += to_string(nTimeEvents);

	info.message += "   state events = ";
	info.message += to_string(nStateEvents);

	info.message += "   step events = ";
	info.message += to_string(nStepEvents);

	info.isSucess = true;
	info.resultPath = resultFile;
	return info;
}

void FMISupport::outputData(
	ofstream& file, double time, bool isHeader, FMU fmu, fmi2Component c) {
	if (isHeader) {
		file << "time";
	}
	else {
		file << time;
	}
	for (int k = 0; k < getScalarVariableSize(fmu.modelDescription); ++k) {
		ScalarVariable *sv = getScalarVariable(fmu.modelDescription, k);
		if (isHeader) {
			const char *s = getAttributeValue((Element*)sv, att_name);
			file << ",";
			while (*s) {
				//忽略空格和逗号
				if (*s != ' ') {
					file << (*s == ',' ? '.' : *s);
				}
				s++;
			}
		}
		else {
			fmi2ValueReference vr = getValueReference(sv);
			switch (getElementType(getTypeSpec(sv))) {
			case elm_Real:
				fmi2Real r;
				fmu.getReal(c, &vr, 1, &r);
				file << "," << r;
				break;
			case elm_Integer:
			case elm_Enumeration:
				fmi2Integer i;
				fmu.getInteger(c, &vr, 1, &i);
				file << "," << i;
				break;
			case elm_Boolean:
				fmi2Boolean b;
				fmu.getBoolean(c, &vr, 1, &b);
				file << "," << b;
				break;
			case elm_String:
				fmi2String s;
				fmu.getString(c, &vr, 1, &s);
				file << "," << s;
				break;
			default:
				file << ",NoValueForType=" << getElementType(getTypeSpec(sv));
			}
		}
	}
	file << endl;
}

//TODO:目前不支持
void FMISupport::unLoad() {
	//fmu0.terminate(c);
	//fmu0.freeInstance(c);
	//FreeLibrary(fmu0.dllHandle);
	//freeModelDescription(fmu0.modelDescription);
	//ostringstream ss;
	//string str;
	////ss << "rmdir /S /Q " << currentDir << OUT_PATH;
	//str = ss.str();
	//system(str.c_str());
}

/**************************************************************/
constexpr auto MAX_MSG_SIZE = 1000;
void fmuLogger(
	void *componentEnvironment,
	fmi2String instanceName,
	fmi2Status status,
	fmi2String category,
	fmi2String message,
	...) {
	char msg[MAX_MSG_SIZE];
	char* copy;
	va_list argp;

	// replace C format strings
	va_start(argp, message);
	vsprintf(msg, message, argp);
	va_end(argp);

	// replace e.g. ## and #r12#
	copy = strdup(msg);
	replaceRefsInMessage(copy, msg, MAX_MSG_SIZE, (FMU*)componentEnvironment);
	free(copy);

	// print the final message
	if (!instanceName) {
		instanceName = "?";
	}
	if (!category) {
		category = "?";
	}

	char infoMsg[512];
	sprintf(infoMsg, "[INFO] %s  %s  (%s):%s",
		fmi2StatusToString(status), instanceName, category, msg);
	LOG::logToSystem(infoMsg);
}
// replace e.g. #r1365# by variable name and ## by # in message
// copies the result to buffer
void replaceRefsInMessage(const char* msg, char* buffer, int nBuffer, FMU* fmu) {
	int i = 0; // position in msg
	int k = 0; // position in buffer
	int n;
	char c = msg[i];
	while (c != '\0' && k < nBuffer) {
		if (c != '#') {
			buffer[k++] = c;
			i++;
			c = msg[i];
		}
		else if (strlen(msg + i + 1) >= 3
			&& (strncmp(msg + i + 1, "IND", 3) == 0 || strncmp(msg + i + 1, "INF", 3) == 0)) {
			// 1.#IND, 1.#INF
			buffer[k++] = c;
			i++;
			c = msg[i];
		}
		else {
			char* end = (char*)strchr(msg + i + 1, '#');
			if (!end) {

				char errMsg[512];
				sprintf(errMsg, "[ERROR] unmatched '#' in '%s'", msg);
				LOG::logToSystem(msg);

				buffer[k++] = '#';
				break;
			}
			n = end - (msg + i);
			if (n == 1) {
				// ## detected, output #
				buffer[k++] = '#';
				i += 2;
				c = msg[i];
			}
			else {
				char type = msg[i + 1]; // one of ribs
				fmi2ValueReference vr;
				int nvr = sscanf(msg + i + 2, "%u", &vr);
				if (nvr == 1) {
					// vr of type detected, e.g. #r12#
					ScalarVariable* sv = getSV(type, vr, fmu);
					const char* name = sv ? getAttributeValue((Element *)sv, att_name) : "?";
					sprintf(buffer + k, "%s", name);
					k += strlen(name);
					i += (n + 1);
					c = msg[i];
				}
				else {
					// could not parse the number

					char errMsg[512];
					sprintf(errMsg, "[ERROR] illegal value reference at position %d in '%s'",
						i + 2, msg);
					LOG::logToSystem(msg);

					buffer[k++] = '#';
					break;
				}
			}
		}
	} // while
	buffer[k] = '\0';
}
// search a fmu for the given variable, matching the type specified.
// return NULL if not found
ScalarVariable* getSV(char type, fmi2ValueReference vr, FMU* fmu) {
	int i;
	int n = getScalarVariableSize(fmu->modelDescription);
	Elm tp;

	switch (type) {
	case 'r': tp = elm_Real;    break;
	case 'i': tp = elm_Integer; break;
	case 'b': tp = elm_Boolean; break;
	case 's': tp = elm_String;  break;
	default: tp = elm_BAD_DEFINED;
	}
	for (i = 0; i < n; i++) {
		ScalarVariable* sv = getScalarVariable(fmu->modelDescription, i);
		if (vr == getValueReference(sv) && tp == getElementType(getTypeSpec(sv))) {
			return sv;
		}
	}
	return NULL;
}
const char* fmi2StatusToString(fmi2Status status) {
	switch (status) {
	case fmi2OK:      return "ok";
	case fmi2Warning: return "warning";
	case fmi2Discard: return "discard";
	case fmi2Error:   return "error";
	case fmi2Fatal:   return "fatal";
	case fmi2Pending: return "fmi2PendingIFCS";
	default:         return "?";
	}
}

/**************************************************************/