// TestRcmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Hello World!" << endl;
	system(".\\R-3.0.1\\bin\\Rscript.exe .\\swiner\\growth_model_run.R");
	system(".\\R-3.0.1\\bin\\Rscript.exe .\\swiner\\sow_model_run.R");
	char exit;
	cout << "Exit? [y/n]" << endl;
	while(cin >> exit){
		if (exit == 'y'){
			return 0;
		}
	}
	return 0;
}

