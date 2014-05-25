#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"
#include "Dndc_shell.h"
#include "CreateInputFile.h"
#include <direct.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifdef DEGUI

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char Pass[300];

int WINAPI OpenSesame(void)
{
	//create sub-directories
	CString DIR0;
	DIR0.Format("%s", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Inputs", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Inter", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Inter\\Management_1", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Inter\\RegionFiles", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Record", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Record\\Region", ROOTDIR);
	mkdir(DIR0);
	DIR0.Format("%sResult\\Record\\Site", ROOTDIR);
	mkdir(DIR0);
	
	DIR0.Format("%sResult\\Record\\MonteCarlo", ROOTDIR);
	mkdir(DIR0);

	DIR0.Format("%sResult\\Record\\Batch", ROOTDIR);
	mkdir(DIR0);

	///////////////////////////////////////////////////////////////////////
	//define path/name of batch file
	char BatchFileName[400], DndFileName[500][200], Site[160], junkc[160], FfFf[6][150]={"",""};
	char BatchName[300];
	int FileNumber, i, MonteCarlo = 0, sample=0, years=0, livestock=0, DroughtID=0;
	int option=0, Monitor=0;
	CString Fregion;
	FILE *region, *FileRun;
	
	sprintf(BatchFileName, "Batch_list.txt");
		
	Fregion.Format("%s", BatchFileName);
	region = fopen(Fregion, "r");
	if(region==NULL)
	{
		char err[120];
		sprintf(err, "Cannot find %s", Fregion);
		AfxMessageBox(err);
		exit(0); 
	}
	fscanf(region, "%d", &FileNumber);

	for(int i=1; i<=FileNumber; i++) fscanf(region, "%s", DndFileName[i]);
	fclose(region);

	for(int i=1; i<=FileNumber; i++)
	{
		DIR0.Format("%sResult\\Record\\Batch\\Case%d", ROOTDIR, i);
		mkdir(DIR0);

		sprintf(BatchName, "%s", DIR0); 

		sprintf(Pass, "%s", BatchName);

		//years = ReadInputDatafromDND(DndFileName[i], 0);
			
		CreateDndcInputFiles(DndFileName[i], "NULL");

		if(option>1000000) 
			option=0;
		else if(option>100)
		{
			MonteCarlo = 1;
			sample = option - 100;
		}

		if(Monitor>1000000) Monitor=0;			

#ifdef MANURE
	livestock = 1;
#else
	livestock = 0;
#endif

		int DroughtID=0;
		char FfFf[6][150]={"",""};

		if(option==10) DroughtID = 1;

		///////////////////////////
		char site_file[300], site[300];
		FILE *fp;

		sprintf(site_file, "%s\\INPUTS\\SITE", OUTPUT);
		fp = fopen(site_file, "r");
		if (fp==NULL) note(0, site_file);		 
		fscanf(fp, "%s", site);
		fscanf(fp, "%d", &years);
		fclose( fp );
		//////////////////////



#ifdef DEGUI
		Model_linux(0, "0", 0, 0, years, "None", "None","None", 0.0, 0, 0, 0, 0, 0, 1, 
			MonteCarlo, 0, 1, 1, "None", sample, 0, 0, "0", livestock, Monitor, DroughtID, FfFf, "0", 4, BatchName);
#else
		Model_link(link, 0, "0", 0, 0, years, "None", "None","None", 0.0, 0, 0, 0, 0, 0, 1, 
			MonteCarlo, 0, option, 1, "None", sample, 0, 0, "0", livestock, Monitor, DroughtID, FfFf, "0", 4);
#endif		
		
	}//end of batch file loop

/*#ifdef DEGUI
	AfxMessageBox("The simulation is accomplished");
#else
	AfxMessageBox("The simulation is accomplished");
#endif*/
	return 0;
}
#endif

