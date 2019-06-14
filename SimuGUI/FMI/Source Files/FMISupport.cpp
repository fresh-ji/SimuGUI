
#ifdef WIN32
#include <direct.h>    
#define GetCurrentDir _getcwd
#else    
#include <unistd.h>    
#define GetCurrentDir getcwd 
#endif

#include "FMISupport.h"
#include "xmlVersionParser.h"

#include <sstream>
#include <string>

#define DIRECTORY_PATH_SIZE 1024
static char currentDirectory[DIRECTORY_PATH_SIZE];

#define UNZIP_CMD ".\\FMI\\7z\\7z x -aoa -o"
// -x   Extracts files from an archive with their full paths in the current dir, or in an output dir if specified
// -aoa Overwrite All existing files without prompt
// -o   Specifies a destination directory where files are to be extracted
#define SEVEN_ZIP_NO_ERROR 0
#define SEVEN_ZIP_WARNING 1
#define SEVEN_ZIP_ERROR 2
#define SEVEN_ZIP_COMMAND_LINE_ERROR 7
#define SEVEN_ZIP_OUT_OF_MEMORY 8
#define SEVEN_ZIP_STOPPED_BY_USER 255

//TODO:Ӧ�ü�֧��32��֧��64
#ifdef _WIN64
#define DLL_DIR   "binaries\\win64\\"
#else
#define DLL_DIR   "binaries\\win32\\"
#endif

ofstream logFile(".\\logs\\FMI.log");

using namespace std;

FMISupport::FMISupport(QObject *parent) : QObject(parent) {
}

FMUInfo FMISupport::loadFMU(QString filePath) {

	FMUInfo info;
	info.isSuccess = false;
	//TODO�����д���ظ���Ҫ�����ѹ���ļ����ڴ����

	//�ж�fmu�ļ��Ƿ����
	QFileInfo fileInfo(filePath);
	if (!fileInfo.isFile()) {
		info.message = "fmu file not exists";
		return info;
	}

	//��ȡ��ǰĿ¼
	currentDir = GetCurrentDir(currentDirectory, DIRECTORY_PATH_SIZE);

	//���������һ��uuid
	QString globalName = filePath.section('/', -1);
	globalName.append(QUuid::createUuid().toString());
	QString targetDir;
	targetDir.append(currentDir)
		.append("\\FMI\\extracted\\").append(globalName).append("\\");

	//ƴ��cmd���ԭ���õ���sstream�������Ҫ(ss.clear() + ss.str(""))
	QString command;
	command.append(UNZIP_CMD).append("\"").append(targetDir)
		.append("\"").append(" \"").append(filePath).append("\" > NUL");

	//��ѹ��
	int code = system(command.toLocal8Bit().constData());
	switch (code) {
	case SEVEN_ZIP_NO_ERROR: break;
	case SEVEN_ZIP_WARNING: info.message = "unzip warning"; return info;
	case SEVEN_ZIP_ERROR: info.message = "unzip error"; return info;
	case SEVEN_ZIP_COMMAND_LINE_ERROR: info.message = "unzip command line error"; return info;
	case SEVEN_ZIP_OUT_OF_MEMORY: info.message = "unzip out of memory"; return info;
	case SEVEN_ZIP_STOPPED_BY_USER: info.message = "unzip stopped by user"; return info;
	default: info.message = "unzip unknown problem"; return info;
	}

	QString descriptionPath = targetDir;
	descriptionPath.append("modelDescription.xml");

	//У�鼰д��汾�ţ�Ŀǰ��֧��2.0
	//TODO:ע����һ��ر�xml�Ļ�
	char* xmlFmiVersion = extractVersion(descriptionPath.toLocal8Bit().constData());
	if (xmlFmiVersion == NULL) {
		info.message = "The FMI version of the FMU could not be read";
		return info;
	}
	if (strcmp(xmlFmiVersion, fmi2Version) != 0) {
		info.message = "The FMI version is not 2.0";
		return info;
	}
	info.version = xmlFmiVersion;

	//ģ�ͻ���
	FMU fmu;
	//TODO:ע����һ��ر�xml�Ļ�
	fmu.modelDescription = parse(descriptionPath.toLocal8Bit().data());

	//����������
	Element* e = (Element*)(fmu.modelDescription);
	int n;
	const char **attributes = getAttributesAsArray(e, &n);
	if (!attributes) {
		info.message = "ModelDescription printing aborted.";
		return info;
	}
	QString des1;
	for (int i = 0; i < n; i += 2) {
		QString s1 = attributes[i];
		QString s2 = " = ";
		QString s3 = attributes[i + 1];
		des1.append(s1 + s2 + s3).append("\n");
	}

	//��ȡ����
	QString simuType;
	Component *component = getModelExchange(fmu.modelDescription);
	if (!component) {
		component = getCoSimulation(fmu.modelDescription);
		if (!component) {
			info.message = "model simulation type wrong";
			return info;
		}
		else {
			simuType = FMI_COSIMULATION;
		}
	}
	else {
		simuType = FMI_MODEL_EXCHANGE;
	}

	//��������
	attributes = getAttributesAsArray((Element*)component, &n);
	if (!attributes) {
		info.message = "ModelDescription printing aborted 2.";
		return info;
	}
	QString des2;
	for (int i = 0; i < n; i += 2) {
		QString s1 = attributes[i];
		QString s2 = " = ";
		QString s3 = attributes[i + 1];
		des2.append(s1 + s2 + s3).append("\n");
	}

	//��ȡid
	const char *modelId;
	if (simuType == FMI_MODEL_EXCHANGE) {
		modelId = getAttributeValue((Element*)getModelExchange(fmu.modelDescription), att_modelIdentifier);
	}
	else if (simuType == FMI_COSIMULATION) {
		modelId = getAttributeValue((Element*)getCoSimulation(fmu.modelDescription), att_modelIdentifier);
	}

	//��ȡdll·��
	QString dllPath = targetDir;
	dllPath.append(DLL_DIR).append(modelId).append(".dll");

	//����dll����
	if (!loadDll(dllPath.toLocal8Bit().constData(), &fmu)) {
		info.message = "load dll error";
		return info;
	}

	info.fmu = fmu;
	info.simuType = simuType;
	info.globalName = globalName;
	info.targetDir = targetDir;
	info.descriptionPath = descriptionPath;
	info.dllPath = dllPath;
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


void FMISupport::unLoad() {
	fmu0.terminate(c);
	fmu0.freeInstance(c);
	FreeLibrary(fmu0.dllHandle);
	freeModelDescription(fmu0.modelDescription);
	ostringstream ss;
	string str;
	//ss << "rmdir /S /Q " << currentDir << OUT_PATH;
	str = ss.str();
	system(str.c_str());
}

bool FMISupport::simulateByCs(
	double tStart,
	double tEnd,
	double h,
	int nCategories,
	char **categories) {
	ModelDescription* md = fmu0.modelDescription;
	//��ȡ��Ϣ
	const char* guid = getAttributeValue((Element*)md, att_guid);
	const char *instanceName = getAttributeValue((Element*)getCoSimulation(md), att_modelIdentifier);

	ostringstream ss;
	string str;

	//ss << currentDir << OUT_PATH << "resources\\";
	str = ss.str();

	//�ص�
	fmi2CallbackFunctions callbacks = { fmuLogger, calloc, free, NULL, &fmu0 };

	//�Ƿ��п��ӻ����
	fmi2Boolean visible = fmi2False;

	//���һ��ΪloggingOn
	c = fmu0.instantiate(instanceName, fmi2CoSimulation, guid, str.c_str(), &callbacks, visible, true);

	if (!c) {
		emit postUIMsg("could not instantiate model");
		return false;
	}

	if (nCategories > 0) {
		fmi2Status fmi2Flag = fmu0.setDebugLogging(c, fmi2True, nCategories, categories);
		if (fmi2Flag > fmi2Warning) {
			emit postUIMsg("failed FMI set debug logging");
			return false;
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

	fmi2Status fmi2Flag = fmu0.setupExperiment(c, toleranceDefined, tolerance, tStart, fmi2True, tEnd);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI setup experiment");
		return false;
	}

	fmi2Flag = fmu0.enterInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI enter initialization mode");
		return false;
	}

	fmi2Flag = fmu0.exitInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI exit initialization mode");
		return false;
	}

	QString ss1 = ".\\logs\\fmics";
	QString ss2 = instanceName;
	QString ss3 = ".csv";
	QString s = ss1 + ss2 + ss3;

	ofstream dataFile(s.toStdString().data());

	outputData(dataFile, tStart, true);
	outputData(dataFile, tStart, false);

	int nSteps = 0;
	double hh = h;
	double time = tStart;
	while (time < tEnd) {
		//�����һ����ʵ����
		if (h > tEnd - time) {
			hh = tEnd - time;
		}
		fmi2Flag = fmu0.doStep(c, time, hh, fmi2True);
		if (fmi2Flag == fmi2Discard) {
			fmi2Boolean b;
			if (fmi2OK != fmu0.getBooleanStatus(c, fmi2Terminated, &b)) {
				emit postUIMsg(
					"could not complete simulation of the model. getBooleanStatus return other than fmi2OK");
				return false;
			}
			if (b == fmi2True) {
				emit postUIMsg("the model requested to end the simulation");
				return false;
			}
			emit postUIMsg("could not complete simulation of the model 1");
			return false;
		}
		if (fmi2Flag != fmi2OK) {
			emit postUIMsg("could not complete simulation of the model 2");
			return false;
		}
		time += hh;
		outputData(dataFile, time, false);
		//�Ʋ�
		nSteps++;
	}

	QString s1 = "Simulation from ";
	QString s2 = QString::number(tStart);
	QString s3 = " to ";
	QString s4 = QString::number(tEnd);;
	emit postUIMsg(s1 + s2 + s3 + s4);

	QString s5 = "steps ............ ";
	QString s6 = QString::number(nSteps);
	emit postUIMsg(s5 + s6);

	QString s7 = "fixed step size .. ";
	QString s8 = QString::number(h);
	emit postUIMsg(s7 + s8);

	return true;
}

bool FMISupport::simulateByMe(
	double tStart,
	double tEnd,
	double h,
	int nCategories,
	char **categories) {

	bool loggingOn = true;

	ModelDescription* md = fmu0.modelDescription;
	//��ȡ��Ϣ
	const char* guid = getAttributeValue((Element*)md, att_guid);
	const char* instanceName = getAttributeValue((Element*)getModelExchange(md), att_modelIdentifier);

	ostringstream ss;
	string str;

	//ss << currentDir << OUT_PATH << "resources\\";
	str = ss.str();

	//�ص� 
	fmi2CallbackFunctions callbacks = { fmuLogger, calloc, free, NULL, &fmu0 };

	//�޿��ӻ����
	fmi2Boolean visible = fmi2False;

	//���һ��ΪloggingOn
	fmi2Component c = fmu0.instantiate(instanceName, fmi2ModelExchange, guid, str.c_str(), &callbacks, visible, loggingOn);
	if (!c) {
		emit postUIMsg("could not instantiate model");
		return false;
	}

	if (nCategories > 0) {
		fmi2Status fmi2Flag = fmu0.setDebugLogging(c, fmi2True, nCategories, categories);
		if (fmi2Flag > fmi2Warning) {
			emit postUIMsg("failed FMI set debug logging");
			return 0;
		}
	}

	//״̬��
	int nx = getDerivativesSize(getModelStructure(md));
	//indicators��
	ValueStatus vs;
	int nz = getAttributeInt((Element*)md, att_numberOfEventIndicators, &vs);
	//����״̬����double���͵ģ���Ӧ��real
	double *x = NULL;
	x = (double*)calloc(nx, sizeof(double));
	//����״̬΢�֣���double���͵ģ���Ӧ��real
	double *xdot = NULL;
	xdot = (double*)calloc(nx, sizeof(double));
	//����indicators
	double *z = NULL;
	//ǰһ��indicator
	double *prez = NULL;
	if (nz > 0) {
		z = (double*)calloc(nz, sizeof(double));
		prez = (double*)calloc(nz, sizeof(double));
	}
	if ((!x || !xdot) || (nz > 0 && (!z || !prez))) {
		emit postUIMsg("out of memory");
		return false;
	}

	fmi2Boolean toleranceDefined = fmi2False;
	fmi2Real tolerance = 0;
	//׼��
	fmi2Status fmi2Flag = fmu0.setupExperiment(c, toleranceDefined, tolerance, tStart, fmi2True, tEnd);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI setup experiment");
		return false;
	}

	//ģ�ͳ�ʼ��
	fmi2Flag = fmu0.enterInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI enter initialization mode");
		return false;
	}
	fmi2Flag = fmu0.exitInitializationMode(c);
	if (fmi2Flag > fmi2Warning) {
		emit postUIMsg("failed FMI exit initialization mode");
		return false;
	}

	//�¼�����
	fmi2EventInfo eventInfo;
	eventInfo.newDiscreteStatesNeeded = fmi2True;
	eventInfo.terminateSimulation = fmi2False;
	while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
		fmi2Flag = fmu0.newDiscreteStates(c, &eventInfo);
		if (fmi2Flag > fmi2Warning) {
			emit postUIMsg("could not set a new discrete state");
			return 0;
		}
	}

	QString ss1 = ".\\logs\\fmime\\";
	QString ss2 = instanceName;
	QString ss3 = ".csv";
	QString s = ss1 + ss2 + ss3;

	ofstream dataFile(s.toStdString().data());

	double time = tStart;
	double dt, tPre;
	fmi2Boolean timeEvent, stateEvent, stepEvent, terminateSimulation;
	int nSteps = 0;
	int nTimeEvents = 0;
	int nStateEvents = 0;
	int nStepEvents = 0;
	if (eventInfo.terminateSimulation) {
		emit postUIMsg("model requested termination at t = " + QString::number(time));
	}
	else {
		fmu0.enterContinuousTimeMode(c);
		outputData(dataFile, tStart, true);
		outputData(dataFile, tStart, false);

		while (time < tEnd) {
			//1.��ȡ��ǰ״̬
			fmi2Flag = fmu0.getContinuousStates(c, x, nx);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not retrieve states");
				return false;
			}
			//2.��ȡ״̬����
			fmi2Flag = fmu0.getDerivatives(c, xdot, nx);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not retrieve derivatives");
				return false;
			}
			//3.ʱ���¼�
			tPre = time;
			time = min(time + h, tEnd);
			timeEvent = eventInfo.nextEventTimeDefined && (eventInfo.nextEventTime < time);
			if (timeEvent) {
				time = eventInfo.nextEventTime;
			}
			dt = time - tPre;
			fmi2Flag = fmu0.setTime(c, time);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not set time");
				return false;
			}
			//4.ǰ��һ��
			for (int i = 0; i < nx; i++) {
				//ŷ������
				x[i] += dt * xdot[i];
			}
			fmi2Flag = fmu0.setContinuousStates(c, x, nx);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not set states");
				return false;
			}
			if (loggingOn) {
				logFile << "Step " << nSteps << " to t = " << time << endl;
			}
			//5.���״̬�¼�
			for (int i = 0; i < nz; i++) {
				prez[i] = z[i];
			}
			fmi2Flag = fmu0.getEventIndicators(c, z, nz);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not retrieve event indicators");
				return false;
			}
			stateEvent = FALSE;
			for (int i = 0; i < nz; i++) {
				stateEvent = stateEvent || (prez[i] * z[i] < 0);
			}
			//6.��鲽���¼�
			fmi2Flag = fmu0.completedIntegratorStep(c, fmi2True, &stepEvent, &terminateSimulation);
			if (fmi2Flag > fmi2Warning) {
				emit postUIMsg("could not complete integrator step");
				return false;
			}
			if (terminateSimulation) {
				emit postUIMsg("model requested termination at t=" + QString::number(time));
				break;
			}
			//7.�����¼�
			if (timeEvent || stateEvent || stepEvent) {
				fmu0.enterEventMode(c);
				if (timeEvent) {
					nTimeEvents++;
					if (loggingOn) {
						logFile << "time event at t=" << time << endl;
					}
				}
				if (stateEvent) {
					nStateEvents++;
					if (loggingOn) {
						for (int i = 0; i < nz; i++) {
							logFile << "state event " << ((prez[i] > 0 && z[i] < 0) ? "-\\-" : "-/-")
								<< " z[" << i << "] at t=" << time << endl;
						}
					}
				}
				if (stepEvent) {
					nStepEvents++;
					if (loggingOn) {
						logFile << "step event at t = " << time << endl;
					}
				}
				eventInfo.newDiscreteStatesNeeded = fmi2True;
				eventInfo.terminateSimulation = fmi2False;
				while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation) {
					fmi2Flag = fmu0.newDiscreteStates(c, &eventInfo);
					if (fmi2Flag > fmi2Warning) {
						emit postUIMsg("could not set a new discrete state");
						return false;
					}
					if (eventInfo.valuesOfContinuousStatesChanged && loggingOn) {
						emit postUIMsg("continuous state values changed at t=" + QString::number(time));
					}
					if (eventInfo.nominalsOfContinuousStatesChanged && loggingOn) {
						emit postUIMsg("nominals of continuous state changed  at t=" + QString::number(time));
					}
				}
				if (eventInfo.terminateSimulation) {
					emit postUIMsg("model requested termination at t=" + QString::number(time));
					break;
				}
				fmu0.enterContinuousTimeMode(c);
			}
			outputData(dataFile, time, false);
			nSteps++;
		}
	}

	QString s1 = "Simulation from ";
	QString s2 = QString::number(tStart);
	QString s3 = " to ";
	QString s4 = QString::number(tEnd);;
	emit postUIMsg(s1 + s2 + s3 + s4);

	QString s5 = "steps ............ ";
	QString s6 = QString::number(nSteps);
	emit postUIMsg(s5 + s6);

	QString s7 = "fixed step size .. ";
	QString s8 = QString::number(h);
	emit postUIMsg(s7 + s8);

	QString s9 = "time events ...... ";
	QString s10 = QString::number(nTimeEvents);
	emit postUIMsg(s9 + s10);

	QString s11 = "state events ..... ";
	QString s12 = QString::number(nStateEvents);
	emit postUIMsg(s11 + s12);

	QString s13 = "step events ...... ";
	QString s14 = QString::number(nStepEvents);
	emit postUIMsg(s13 + s14);

	return true;
}

void FMISupport::outputData(ofstream& file, double time, bool isHeader) {

	if (isHeader) {
		file << "time";
	}
	else {
		file << time;
	}
	for (int k = 0; k < getScalarVariableSize(fmu0.modelDescription); ++k) {
		ScalarVariable *sv = getScalarVariable(fmu0.modelDescription, k);
		if (isHeader) {
			const char *s = getAttributeValue((Element*)sv, att_name);
			file << ",";
			while (*s) {
				//���Կո�Ͷ���
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
				fmu0.getReal(c, &vr, 1, &r);
				file << "," << r;
				break;
			case elm_Integer:
			case elm_Enumeration:
				fmi2Integer i;
				fmu0.getInteger(c, &vr, 1, &i);
				file << "," << i;
				break;
			case elm_Boolean:
				fmi2Boolean b;
				fmu0.getBoolean(c, &vr, 1, &b);
				file << "," << b;
				break;
			case elm_String:
				fmi2String s;
				fmu0.getString(c, &vr, 1, &s);
				file << "," << s;
				break;
			default:
				file << ",NoValueForType=" << getElementType(getTypeSpec(sv));
			}
		}
	}
	file << endl;
}



/**************************************************************/
#define MAX_MSG_SIZE 1000
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
	if (!instanceName) instanceName = "?";
	if (!category) category = "?";

	logFile << fmi2StatusToString(status)
		<< "  "
		<< instanceName
		<< "("
		<< category
		<< "):"
		<< msg
		<< endl;
}
// replace e.g. #r1365# by variable name and ## by # in message
// copies the result to buffer
void replaceRefsInMessage(const char* msg, char* buffer, int nBuffer, FMU* fmu0) {
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
				logFile << "<error>unmatched '#' in '"
					<< msg
					<< "'"
					<< endl;
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
					ScalarVariable* sv = getSV(fmu0, type, vr);
					const char* name = sv ? getAttributeValue((Element *)sv, att_name) : "?";
					sprintf(buffer + k, "%s", name);
					k += strlen(name);
					i += (n + 1);
					c = msg[i];

				}
				else {
					// could not parse the number
					logFile << "<error>illegal value reference at position "
						<< i + 2
						<< " in '"
						<< msg
						<< "'"
						<< endl;
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
ScalarVariable* getSV(FMU* fmu0, char type, fmi2ValueReference vr) {
	int i;
	int n = getScalarVariableSize(fmu0->modelDescription);
	Elm tp;

	switch (type) {
	case 'r': tp = elm_Real;    break;
	case 'i': tp = elm_Integer; break;
	case 'b': tp = elm_Boolean; break;
	case 's': tp = elm_String;  break;
	default: tp = elm_BAD_DEFINED;
	}
	for (i = 0; i < n; i++) {
		ScalarVariable* sv = getScalarVariable(fmu0->modelDescription, i);
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