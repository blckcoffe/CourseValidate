#include "stdafx.h"
#include "fileProcessor.h"


fileProcessor::fileProcessor()
{
}


fileProcessor::~fileProcessor()
{
}


void fileProcessor::setFilePath(string sPath){
	sFilePath = sPath;
	sFileName = "cangqilaideyuanxing";
	size_t sPos;
	sPos = sPath.find_last_of("\\");
	string sFolderName;
	if (sPos != string::npos )
	{
		sFolderName = sPath.substr(sPos + 1);
		setFileFolderName(sFolderName);
		
	}
}

void fileProcessor::setNoteTitle(string sTitle){
//	if (sTitle == sFileFolderName)
//	{
		sNoteTitle = sTitle;
//	}
	
}

bool fileProcessor::fileValidation(string sPath){
	setFilePath(sPath);
	if (true == processJsonFile()){
		return validate();
	}
	return false;
}

void fileProcessor::setFileFolderName(string sName){
	sFileFolderName = sName;
}

bool fileProcessor::processJsonFile(){
	// read json content into string.
	string      stringFromStream;
	ifstream    in;
	in.open(sFilePath + "\\" + sFileName + ".json", ifstream::in);
	if (!in.is_open())
		return false;
	string line;
	while (getline(in, line)) {
		stringFromStream.append(line + "\n");
		cout << line << endl;
	}
	in.close();

	locale chs("chs");
	wifstream ifs(sFilePath + "\\" + sFileName + ".json");
	ifs.imbue(chs);
	wcout.imbue(chs);
	wstring wline;
	while (getline(ifs, wline)){
		wcout << wline << endl;
	}

	// ---------------------------- read json --------------------
	// parse json from string.
	using rapidjson::Document;
	Document doc;
	doc.Parse<0>(stringFromStream.c_str());
	if (doc.HasParseError()) {
		rapidjson::ParseErrorCode code = doc.GetParseError();
		return false;
	}

	sNoteTitle = doc["notetitle"].GetString();
	string sVideoName;
	// use values in parse result.
	using rapidjson::Value;
	Value & vLinkedVideo = doc["linkedvideo"];
	if (vLinkedVideo.IsArray()) {
		for (size_t i = 0; i < vLinkedVideo.Size(); ++i) {
			Value & vL = vLinkedVideo[i];
			assert(vL.IsString());
			//			psln(vL.GetString());
			sVideoName = vL.GetString();
			if (string::npos != sVideoName.find(sNoteTitle))
			{
				vLinkedVideos.push_back(vL.GetString());
			}
		}
	}

	Value & vNoteContent = doc["notecontent"];
	string sPlayCoursewareName;

	if (vNoteContent.IsArray()) {
		for (size_t i = 0; i < vNoteContent.Size(); ++i) {
			Value & vN = vNoteContent[i];
			assert(vN.IsObject());
			if (vN.HasMember("sectioncontent") && vN["sectioncontent"].IsArray()) {
				Value &vSectionContent = vN["sectioncontent"];
				assert(vSectionContent.IsArray());
				for (size_t i = 0; i < vSectionContent.Size(); ++i){
					Value &vSC = vSectionContent[i];
					assert(vSC.IsObject());
					if (vSC.HasMember("section") && vSC["section"].IsArray())
					{
						Value &vS = vSC["section"];
						string sSectionValue;
						for (size_t i = 0; i < vS.Size(); ++i){
							assert(vS[i].IsString());
							sSectionValue = vS[i].GetString();
							std::wstring wstr;
							wstr.assign(sSectionValue.begin(), sSectionValue.end());
							cout << (sSectionValue) << endl;
							wcout.imbue(locale("chs"));
							wcout << wstr << endl;
							if (getCourseNameInSection(sSectionValue, sPlayCoursewareName, sNoteTitle)){
								vCoursewareNames.push_back(sPlayCoursewareName);
							}
						}
					}
				}
				break; //"sectioncontent 节点处理完毕，其它节点不需要处理"
			}
		}
	}

	return true;
}

bool fileProcessor::getCourseNameInSection(string s, string &subString, string stitle){
	size_t iTitleLength;
	size_t iPos;
	iPos = s.find(stitle);
	if (string::npos != iPos)
	{
		iTitleLength = stitle.length();
		subString = s.substr(iPos, iTitleLength + 2);
		return true;
	}

	return false;
}

bool  fileProcessor::validate(){
	string sVideoName;
	bool fFirstElement = true;
	size_t i = 0;
	size_t iPos;

	time_t rawtime;
	time(&rawtime);
	char localtime[64];
	ctime_s(localtime, 64, &rawtime);
	ofstream logInfo( "checklog.txt");
	logInfo << "检查错误开始：" << localtime << endl;
	for (auto iter = vLinkedVideos.cbegin(); iter != vLinkedVideos.cend(); iter++){
		sVideoName = *iter;
		if (fFirstElement)
		{
			if  (sVideoName == sNoteTitle + ".obb")
			{
				fFirstElement = false;
				continue;
			}
			else{
				logInfo << sNoteTitle + ".obb" << "没有找到" << endl;
				logInfo.close();
				return false;
			}
		}
		
		iPos = sVideoName.find(".");
		if ( string::npos != iPos )
		{
			sVideoName = sVideoName.substr(0, iPos);
		}

		if ( sVideoName != vCoursewareNames[i++] )
		{
			logInfo << sVideoName << "没有找到" << endl;
			logInfo.close();
			return false;
		}

	}

	logInfo << "没发现什么问题" << endl;
	logInfo.close();
	return true;
}