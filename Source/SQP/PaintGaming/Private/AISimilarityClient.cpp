#include "AISimilarityClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"


bool UAISimilarityClient::ConvertTextureToPNG(UTexture2D* Texture, TArray<uint8>& OutData)
{
    if (!Texture) return false;

    // 텍스쳐 데이터 읽기 위해 압축 풀기
    Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    Texture->SRGB = true;
    Texture->UpdateResource();

    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
    int32 Width = Mip.SizeX;
    int32 Height = Mip.SizeY;

    // RGBA8 형식으로 PNG 변환
    TArray<uint8> RawData;
    RawData.AddUninitialized(Width * Height * 4);
    FMemory::Memcpy(RawData.GetData(), Data, RawData.Num());
    Mip.BulkData.Unlock();

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper->SetRaw(RawData.GetData(), RawData.Num(), Width, Height, ERGBFormat::RGBA, 8))
    {
        OutData = ImageWrapper->GetCompressed();
        return true;
    }
    return false;
}

void UAISimilarityClient::SendCompareRequest(
    UTexture2D* Original, UTexture2D* A, UTexture2D* B, FAIResultDelegate ResultCallback)
{
    if (!Original || !A || !B)
    {
        ResultCallback.ExecuteIfBound(TEXT("Error: Invalid texture"));
        return;
    }

    // HTTP 요청 준비
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ServerURL);
    Request->SetVerb(TEXT("POST"));

    FString Boundary = TEXT("---------------------------UEBoundary");
    FString ContentType = TEXT("multipart/form-data; boundary=") + Boundary;
    Request->SetHeader(TEXT("Content-Type"), ContentType);

    // 바디 생성
    TArray<uint8> Body;
    auto AddFileToBody = [&](const FString& Name, const FString& Filename, const TArray<uint8>& FileData)
    {
        FString Header = FString::Printf(
            TEXT("--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n")
            TEXT("Content-Type: application/octet-stream\r\n\r\n"),
            *Boundary, *Name, *Filename);

        Body.Append((uint8*)TCHAR_TO_UTF8(*Header), Header.Len());
        Body.Append(FileData);
        Body.Append((const uint8*)"\r\n", 2);
    };

    // 텍스처 PNG 변환
    TArray<uint8> OrigData, AData, BData;
    ConvertTextureToPNG(Original, OrigData);
    ConvertTextureToPNG(A, AData);
    ConvertTextureToPNG(B, BData);

    AddFileToBody("original", "original.png", OrigData);
    AddFileToBody("A", "A.png", AData);
    AddFileToBody("B", "B.png", BData);

    // 끝 경계
    FString EndBoundary = FString::Printf(TEXT("--%s--\r\n"), *Boundary);
    Body.Append((uint8*)TCHAR_TO_UTF8(*EndBoundary), EndBoundary.Len());

    Request->SetContent(Body);

    // 응답 처리
    Request->OnProcessRequestComplete().BindLambda(
        [ResultCallback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
        {
            if (!bSuccess || !Res.IsValid())
            {
                ResultCallback.ExecuteIfBound(TEXT("Error: Request failed"));
                return;
            }

            FString ResponseString = Res->GetContentAsString();
            UE_LOG(LogTemp, Warning, TEXT("AI Response: %s"), *ResponseString);

            // winner만 추출 (단순 방식)
            FString Winner;
            if (ResponseString.Contains("\"winner\":\"A\""))
                Winner = "A";
            else if (ResponseString.Contains("\"winner\":\"B\""))
                Winner = "B";
            else
                Winner = "Error";

            ResultCallback.ExecuteIfBound(Winner);
        });

    Request->ProcessRequest();
}