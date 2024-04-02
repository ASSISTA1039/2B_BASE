// Fill out your copyright notice in the Description page of Project Settings.


#include "ClipAnimSequenceTools.h"
#include "Animation/AnimSequenceHelpers.h"

class MotFile {
public:
	char      id; // "mot\0"
	uint32_t    hash;
	uint16_t    flag;
	int16_t     frameCount;
	uint32_t    recordOffset;
	uint32_t    recordNumber;
	uint32_t    unknown; // usually 0 or 0x003c0000, maybe two uint16
	//string    animName = ""; // found at most 12 bytes with terminating 0
};

bool UClipAnimSequenceTools::AnimationDataTrim(UAnimSequence* InSequence, float TrimStart, float TrimEnd, bool bInclusiveEnd) {
	return UE::Anim::AnimationData::Trim(InSequence, TrimStart, TrimEnd, bInclusiveEnd);
}

void UClipAnimSequenceTools::ConsoleLog(FString text) {
	UE_LOG(LogTemp, Log, TEXT("%s"), *text);
}

void UClipAnimSequenceTools::ClipAnimAsset(UAnimSequence* anim, int startFrame, int endFrame) {

	if (UE::Anim::AnimationData::Trim(anim,  (float)endFrame / anim->ImportFileFramerate, (float)(anim->GetNumberOfSampledKeys() - 1) / anim->ImportFileFramerate, true))
	{
		UE_LOG(LogTemp, Log, TEXT("%f %f %f"), (float)endFrame / anim->ImportFileFramerate, (float)(anim->GetNumberOfSampledKeys() - 1) / anim->ImportFileFramerate, anim->ImportFileFramerate);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), (float)endFrame / anim->ImportFileFramerate, (float)(anim->GetNumberOfSampledKeys() - 1) / anim->ImportFileFramerate, anim->ImportFileFramerate);
	}

	if (UE::Anim::AnimationData::Trim(anim, 0.0,  (float)startFrame / anim->ImportFileFramerate))
	{
		UE_LOG(LogTemp, Log, TEXT("%d %f"), 0, (float)startFrame / anim->ImportFileFramerate);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%d %f"), 0, (float)startFrame / anim->ImportFileFramerate);
	}

	//TArray<struct FRawAnimSequenceTrack> data = anim->RawAnimationData;
	//anim->CropRawAnimData(t, false);
	//t = anim->GetPlayLength() / (float)(anim->GetNumberOfSampledKeys() - 1) * (float)startFrame;
	//anim->CropRawAnimData(t, true);

	//UE_LOG(LogTemp, Warning, TEXT("%d %d %f %f %f"), startFrame, endFrame, anim->SequenceLength, anim->GetNumberOfFrames(), t);
}

FString UClipAnimSequenceTools::ReadTxt(FString path)
{
	FString resultString;

	FFileHelper::LoadFileToString(resultString, *(path));

	return resultString;
}

TArray<FString> UClipAnimSequenceTools::ReadFileArray(FString path)
{
	TArray<FString> results;
	FFileHelper::LoadFileToStringArray(results, *path);
	return results;
}


bool UClipAnimSequenceTools::WriteTxtArray(TArray<FString> results, FString path)
{
	bool result = true;
	std::string _path = TCHAR_TO_UTF8(*path);
	std::ofstream outfile(_path);
	for (auto i : results) {
		std::string s = TCHAR_TO_UTF8(*i);
		outfile << s << std::endl;
	}
	outfile.close();
	return result;
}

void UClipAnimSequenceTools::ReadMotFile(FString path)
{
	std::string _path = TCHAR_TO_UTF8(*path);
	std::ofstream outfile(_path + "\\A_result.txt");
	int _firstFrame = 1;
	int _lastFrame = _firstFrame - 1;
	std::vector<string> _files = {};

	getFiles(_path, _files);
	//for (auto _target : results) {
	//	std::string target = TCHAR_TO_UTF8(*_target);
	//	if (target == "restpose")
	//	{
	//		_lastFrame += 1;
	//		outfile << _firstFrame << "|" << _lastFrame << "|" << "restpose" << std::endl;
	//		_firstFrame += 1;
	//		continue;
	//	}
	for (auto _filePath : _files)
	{
		//1.��ȡ����·�����ļ���
		string::size_type iPos = _filePath.find_last_of('\\') + 1;
		std::string filename = _filePath.substr(iPos, _filePath.length() - iPos);
		//2.��ȡ������׺���ļ���
		std::string _fileNameWithoutExtension = filename.substr(0, filename.rfind("."));

		//3.��ȡ��׺��
		std::string _extension = filename.substr(filename.find_last_of('.'));

		if (_extension == ".mot")
		{
			MotFile s;
			std::ifstream inFile(_filePath, ios::in | ios::binary); //�����ƶ���ʽ��
			if (!inFile) {
				std::cout << "error" << std::endl;
				inFile.close();
				continue;
			}
			inFile.read((char*)&s, sizeof(s));
			//int readedBytes = inFile.gcount(); //���ղŶ��˶����ֽ�
			std::cout << s.frameCount << std::endl;
			int _frameCount = s.frameCount;
			inFile.close();
			//��ȡ����Ƭ����֡��

			_lastFrame += _frameCount;
			outfile << _firstFrame << "|" << _lastFrame << "|" << _fileNameWithoutExtension << std::endl;
			_firstFrame += _frameCount;
		}
		//}
	}

	outfile.close();
	//File.WriteAllLines(_path + $"/{_directoryName}.txt", _list);
}

FString UClipAnimSequenceTools::Get_O_Turn_To_0(FString str)
{
	std::string MyStdString = TCHAR_TO_UTF8(*str);

	//2.��ȡ������׺���ļ���
	std::string _fileNameWithoutExtension = MyStdString.substr(0, MyStdString.rfind("."));
	for (int i = 0; i < _fileNameWithoutExtension.size(); i++) {
		if (_fileNameWithoutExtension[i] == 'o' || _fileNameWithoutExtension[i] == 'O') {
			_fileNameWithoutExtension[i] = '0';
		}
	}
	return FString((_fileNameWithoutExtension + ".mot").c_str());;
}

void UClipAnimSequenceTools::getFiles(string path, vector<string>& files) {
	//�ļ����  
	intptr_t hFile = 0;
	//�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
	struct _finddata_t fileinfo;
	string p;  //�ַ��������·��
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
	{
		do
		{
			//�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				//�ļ���������"."&&�ļ���������".."
				//.��ʾ��ǰĿ¼
				//..��ʾ��ǰĿ¼�ĸ�Ŀ¼
				//�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//�������,�����б�  
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose������������
		_findclose(hFile);
	}

}