// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <fstream>
#include <vector>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <iostream>
using namespace std;

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ClipAnimSequenceTools.generated.h"

/**
 * 
 */
UCLASS()
class NIERREBIRTH_API UClipAnimSequenceTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable, Category = "AnimAutoClip")
		static void ClipAnimAsset(UAnimSequence* anim, int startFrame, int endFrame);

	UFUNCTION(BlueprintCallable, Category = "HandleTxt")
		static FString ReadTxt(FString path);

	UFUNCTION(BlueprintCallable, Category = "HandleTxt")
		static bool WriteTxtArray(TArray<FString> results, FString path);

	UFUNCTION(BlueprintCallable, DisplayName = "ReadFileArray", Category = "File")
		static TArray<FString> ReadFileArray(FString path);

	UFUNCTION(BlueprintCallable, DisplayName = "Get_O_Turn_To_0", Category = "File")
		static FString Get_O_Turn_To_0(FString str);

	UFUNCTION(BlueprintCallable, DisplayName = "ReadMotFile", Category = "File")
		static void ReadMotFile(FString path);

	UFUNCTION(BlueprintCallable, DisplayName = "AnimationDataTrim", Category = "File")
		static bool AnimationDataTrim(UAnimSequence* InSequence, float TrimStart, float TrimEnd, bool bInclusiveEnd);

	UFUNCTION(BlueprintCallable, DisplayName = "ConsoleLog", Category = "File")
		static void ConsoleLog(FString text);

	static void getFiles(string path, vector<string>& files);
	static size_t GetFileSize(const std::string& file_name);
};
