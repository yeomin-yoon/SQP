#include "AISimilarityClient.h"

#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


static bool SafeTextureToPNG(UTexture2D* Texture, TArray<uint8>& OutData)
{
    OutData.Empty();

    // 유효성 검사
    if (!IsValid(Texture))
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Invalid Texture"));
        return false;
    }

    FTexturePlatformData* PlatformData = Texture->GetPlatformData();
    if (!PlatformData || PlatformData->Mips.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Missing Platform Data or Mips"));
        return false;
    }

    FTexture2DMipMap& Mip = PlatformData->Mips[0];
    const int32 Width = Mip.SizeX;
    const int32 Height = Mip.SizeY;

    if (Width <= 0 || Height <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Invalid Texture Size: %d x %d"), Width, Height);
        return false;
    }

    // BulkData 접근
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
    if (!Data)
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Failed to lock bulk data"));
        return false;
    }

    const int32 NumPixels = Width * Height;
    TArray<FColor> Pixels;
    Pixels.SetNumUninitialized(NumPixels);

    FMemory::Memcpy(Pixels.GetData(), Data, NumPixels * sizeof(FColor));
    Mip.BulkData.Unlock();

    // ImageWrapper 모듈 로드
    IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>("ImageWrapper");
    if (!ImageWrapperModule)
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Failed to load ImageWrapper module"));
        return false;
    }

    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule->CreateImageWrapper(EImageFormat::PNG);
    if (!ImageWrapper.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Failed to create ImageWrapper"));
        return false;
    }

    // RAW 데이터 설정
    if (!ImageWrapper->SetRaw(Pixels.GetData(), Pixels.Num() * sizeof(FColor),
                              Width, Height, ERGBFormat::BGRA, 8))
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Failed to set raw data for ImageWrapper"));
        return false;
    }

    // 압축 및 변환 (TArray64 → TArray)
    const TArray64<uint8>& CompressedData64 = ImageWrapper->GetCompressed(100);
    if (CompressedData64.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[SafeTextureToPNG] Compression failed"));
        return false;
    }

    OutData.Reset(CompressedData64.Num());
    OutData.Append(CompressedData64.GetData(), CompressedData64.Num());

    UE_LOG(LogTemp, Log, TEXT("[SafeTextureToPNG] PNG Conversion Success: %d bytes"), OutData.Num());
    return true;
}

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
	if (SafeTextureToPNG(Original, OrigBytes))
		AppendFile(TEXT("original"), TEXT("original.png"), OrigBytes);

	// 비교 텍스처들 추가
	for (int32 i = 0; i < ComparisonTextures.Num(); ++i)
	{
		TArray<uint8> Bytes;
		if (SafeTextureToPNG(ComparisonTextures[i], Bytes))
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
