/*
 * Copyright QTronic GmbH. All rights reserved.
 */

/* ---------------------------------------------------------------------------*
 * xmlVersionParser.c
 *
 * Author: Adrian Tirea
 * ---------------------------------------------------------------------------*/

#include <string.h>
#define checkStrdup(str) strdup(str)
#define BOOL int
#define TRUE 1
#define FALSE 0

#include <libxml/xmlreader.h>
#include "xmlVersionParser.h"

#define LOG_FILE ".\\FMI\\log.log"

// Element and attribute that identify the fmiVersion.
#define ELM_fmiModelDescription_NAME "fmiModelDescription"
#define ATT_fmiVersion_NAME "fmiVersion"

static BOOL readNextInXml(xmlTextReaderPtr xmlReader) {
    int ret;
    do {
        ret = xmlTextReaderRead(xmlReader);
    } while (ret == 1 && xmlTextReaderNodeType(xmlReader) == XML_READER_TYPE_COMMENT);

    if (ret != 1) {
        return FALSE;
    }
    return TRUE;
}

// The receiver must free the return.
static char *extractFmiVersionAttribute(xmlTextReaderPtr xmlReader) {
    char *value = (char *)xmlTextReaderGetAttribute(xmlReader, ATT_fmiVersion_NAME);
    char *result = value ? (char *)checkStrdup(value) : NULL;
    xmlFree(value);
    return result;
}

// The receiver must free the return.
static char *streamFile(const char *xmlPath) {

	FILE *fp = fopen(LOG_FILE, "a+");

    xmlTextReaderPtr xmlReader;
    char *fmiVersion = NULL;
    xmlReader = xmlReaderForFile(xmlPath, NULL, 0);
    if (xmlReader != NULL) {
        if (readNextInXml(xmlReader)) {
            // I expect that first element is fmiModelDescription.
            if (0 != strcmp((char *)xmlTextReaderConstLocalName(xmlReader), ELM_fmiModelDescription_NAME)) {
				char msg[512];
				sprintf(msg, "[ERROR] Expected '%s' element. Found instead: '%s'",
					ELM_fmiModelDescription_NAME, xmlTextReaderConstLocalName(xmlReader));
				//fprintf(fp, msg);
				fprintf(fp, "s2");
            } else {
                fmiVersion = extractFmiVersionAttribute(xmlReader);
            }
        } else {
			char msg[512];
			sprintf(msg, "[ERROR] Syntax error parsing xml file '%s'", xmlPath);
			//fprintf(fp, msg);
			fprintf(fp, "s1");
        }
		//edit by jh
        //xmlFreeTextReader(xmlReader);
    } else {
		char msg[512];
		sprintf(msg, "[ERROR] Cannot open file '%s'", xmlPath);
		//fprintf(fp, msg);
		fprintf(fp, "s3");
    }

	fclose(fp);

    return fmiVersion;
}

//����
char *extractVersion(const char *xmlDescriptionPath) {
    char *fmiVersion;
    xmlInitParser();
    fmiVersion = streamFile(xmlDescriptionPath);
    xmlCleanupParser();
    return fmiVersion;
}
