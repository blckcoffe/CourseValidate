// JsonErrorCheck.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include "rapidjson/document.h"
#include "fileProcessor.h"
#include <direct.h>
#include <wchar.h>  

#ifdef _WIN32  
#include <Windows.h>  
#endif  



#define psln(x) std::cout << #x " = " << (x) << std::endl



using namespace rapidjson;
using namespace std;

int main()
{
	fileProcessor lo_fileProcessor;
	string sPath;
	lo_fileProcessor.fileValidation("C:\\Users\\i075995\\Documents\\2018\\20170508\\藏起来的圆形");
    return 0;
}

