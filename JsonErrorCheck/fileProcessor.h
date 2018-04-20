#pragma once
#include <direct.h>
#include "common.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <time.h>
#include <locale>

using std::string;
using std::ifstream;

class fileProcessor
{
private:
	string sFilePath;
	string sFileName;
	string sFileFolderName;
	string sNoteTitle;
	vector<string> vLinkedVideos;
	vector<string> vCoursewareNames;
	void setFileName(string sName);
	void setFileFolderName(string sName);
	bool processJsonFile();
	bool getCourseNameInSection(string s, string &subString, string stitle);
	bool validate();
public:
	void setFilePath(string sPath);
	void setNoteTitle(string sTitle);
	bool fileValidation(string sPath);
	fileProcessor();
	~fileProcessor();
};

