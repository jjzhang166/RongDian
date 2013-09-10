#include "PhpTidy.h"
#include "common.h"

#include "exception.h"
#include "settings.h"
#include "formatter.h"
#include "scanner.h"
#include "parser.h"

/* globals */
CSettings  *settings  = NULL;
PhpFormatter *formatter = NULL;
CScanner   *scanner   = NULL;
CParser    *parser    = NULL;

PhpTidyLib::PhpTidy::PhpTidy()
{
}

PhpTidyLib::PhpTidy::~PhpTidy()
{
}

bool PhpTidyLib::PhpTidy::Format( const char* pSourceIn, const char *pOptions, string &strOut, string &strErr)
{
	try
	{
		/* create settings class */
		settings = new CSettings();
		if (settings == NULL)
			throw new CException(E_MEMORY);

		/* create formatter class */
		formatter = new PhpFormatter();
		if (formatter == NULL)
			throw new CException(E_MEMORY);

		/* create scanner class */
		scanner = new CScanner();
		if (scanner == NULL)
			throw new CException(E_MEMORY);

		/* create parser class */
		parser = new CParser();
		if (parser == NULL)
			throw new CException(E_MEMORY);

		/* init settings */
		settings->InitFromOptions(pOptions);

		/* init formatter */
		formatter->Init();

		/* init scanner */
		scanner->Init(pSourceIn);

		/* init parser */
		//CParser->Init();

		/* start formatting */
		formatter->Run(strOut);
	} // end of 'try'
	/* catch exceptions */
	catch (CException *exception)
	{
		/* show message */
		if (!settings->quiet)
			strErr = exception->message;
		/* delete classes */
		if (settings != NULL)
			delete settings;
		if (formatter != NULL)
			delete formatter;
		if (scanner != NULL)
			delete scanner;
		if (parser != NULL)
			delete parser;

		return false;
	} // end of 'catch (CException *exception)'

	/* delete classes */
	if (settings != NULL)
		delete settings;
	if (formatter != NULL)
		delete formatter;
	if (scanner != NULL)
		delete scanner;
	if (parser != NULL)
		delete parser;

	return true;
}
