#pragma once

#include "fmi2.h"

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

#define FMI_MODEL_EXCHANGE "Model Exchange"
#define FMI_COSIMULATION "CoSimulation"

#define WIN_64 "WIN 64"
#define WIN_32 "WIN 32"

#define UNZIP_DIR "\\FMI\\extracted\\"

#define RESULT_DIR "\\FMI\\result\\"

#define MD_FILE_NAME "modelDescription.xml"

#define DLL_DIR "binaries\\win64\\"

//在xmlVersionParser.c里也有，但没写进去
#define LOG_FILE ".\\FMI\\log.log"