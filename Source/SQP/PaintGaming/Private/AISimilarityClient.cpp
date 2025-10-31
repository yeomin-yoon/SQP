#include "AISimilarityClient.h"

#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

static bool TextureToPNGBytes(UTexture2D* Texture, TArray<uint8>& OutData) { OutData.Empty(); if (!Texture || !Texture->GetPlatformData() || Texture->GetPlatformData()->Mips.Num() == 0) { UE_LOG(LogTemp, Error, TEXT("Invalid texture")); return false; } FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0]; int32 Width = Mip.SizeX; int32 Height = Mip.SizeY; if (Width <= 0 || Height <= 0) { UE_LOG(LogTemp, Error, TEXT("Invalid texture size")); return false; } void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY); if (!Data) { UE_LOG(LogTemp, Error, TEXT("Failed to lock texture bulk data")); return false; } const int32 NumPixels = Width * Height; TArray<FColor> Pixels; Pixels.SetNumUninitialized(NumPixels); FMemory::Memcpy(Pixels.GetData(), Data, NumPixels * sizeof(FColor)); Mip.BulkData.Unlock(); IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper"); TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG); if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(Pixels.GetData(), Pixels.Num() * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8)) { OutData = ImageWrapper->GetCompressed(100); return true; } UE_LOG(LogTemp, Error, TEXT("Failed to compress image")); return false; }

void UAISimilarityClient::CompareTextures(UTexture2D* Original, const TArray<UTexture2D*>& ComparisonTextures,
                                          const TArray<FString>& PlayerNames)
{
	if (!Original || ComparisonTextures.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid inputs"));
		return;
	}

	FString Boundary = TEXT("----AITextureBoundary");
	FString Url = TEXT("http://127.0.0.1:8000/compare_textures/");
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);

	TArray<uint8> Payload;

	auto AppendFile = [&](const FString& FieldName, const FString& FileName, const TArray<uint8>& FileData)
	{
		FString Header = FString::Printf(
			TEXT(
				"--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: image/png\r\n\r\n"),
			*Boundary, *FieldName, *FileName);
		FTCHARToUTF8 HeaderUtf8(*Header);
		Payload.Append((uint8*)HeaderUtf8.Get(), HeaderUtf8.Length());
		Payload.Append(FileData);
		const FString LineBreak = TEXT("\r\n");
		FTCHARToUTF8 BreakUtf8(*LineBreak);
		Payload.Append((uint8*)BreakUtf8.Get(), BreakUtf8.Length());
	};

	// 원본 추가
	TArray<uint8> OrigBytes;
	if (TextureToPNGBytes(Original, OrigBytes))
		AppendFile(TEXT("original"), TEXT("original.png"), OrigBytes);

	// 비교 텍스처들 추가
	for (int32 i = 0; i < ComparisonTextures.Num(); ++i)
	{
		TArray<uint8> Bytes;
		if (TextureToPNGBytes(ComparisonTextures[i], Bytes))
		{
			FString FileName = FString::Printf(TEXT("comp_%d.png"), i);
			AppendFile(TEXT("comparisons"), FileName, Bytes);
		}
	}

	// 끝 경계
	FString EndBoundary = FString::Printf(TEXT("--%s--\r\n"), *Boundary);
	FTCHARToUTF8 EndUtf8(*EndBoundary);
	Payload.Append((uint8*)EndUtf8.Get(), EndUtf8.Length());

	Request->SetContent(Payload);

	Request->OnProcessRequestComplete().BindLambda(
		[PlayerNames](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
		{
			if (bSuccess && Res.IsValid())
			{
				FString JsonStr = Res->GetContentAsString();
				TSharedPtr<FJsonObject> JsonObj;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);

				if (FJsonSerializer::Deserialize(Reader, JsonObj))
				{
					FString BestFile = JsonObj->GetStringField(TEXT("best_match"));
					float Score = JsonObj->GetNumberField(TEXT("score"));

					int32 Index = -1;
					FString IndexStr;
					if (BestFile.Split(TEXT("comp_"), nullptr, &IndexStr))
					{
						IndexStr.Split(TEXT(".png"), &IndexStr, nullptr);
						Index = FCString::Atoi(*IndexStr);
					}

					if (PlayerNames.IsValidIndex(Index))
					{
						UE_LOG(LogTemp, Warning, TEXT("가장 유사한 플레이어: %s (Score: %.3f)"), *PlayerNames[Index], Score);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("인덱스 매칭 실패: %s"), *BestFile);
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
			}
		});
	
	Request->ProcessRequest();
}
