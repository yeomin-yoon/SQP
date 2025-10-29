from fastapi import FastAPI, UploadFile
from fastapi.middleware.cors import CORSMiddleware
from starlette.responses import JSONResponse
import torch
import torchvision.transforms as transforms
from PIL import Image
import io
import uvicorn

app = FastAPI()

# CORS 허용 (언리얼 HTTP 통신 위해)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# CLIP 모델 로드
import open_clip
model, _, preprocess = open_clip.create_model_and_transforms('ViT-B-32', pretrained='openai')
model.eval()

def image_to_feature(img_bytes):
    image = Image.open(io.BytesIO(img_bytes)).convert('RGB')
    image = preprocess(image).unsqueeze(0)
    with torch.no_grad():
        feature = model.encode_image(image)
        feature /= feature.norm(dim=-1, keepdim=True)
    return feature

@app.post("/compare")
async def compare_images(original: UploadFile, A: UploadFile, B: UploadFile):
    # 이미지 특징 추출
    feat_orig = image_to_feature(await original.read())
    feat_A = image_to_feature(await A.read())
    feat_B = image_to_feature(await B.read())

    scoreA = torch.cosine_similarity(feat_orig, feat_A).item()
    scoreB = torch.cosine_similarity(feat_orig, feat_B).item()

    winner = "A" if scoreA > scoreB else "B"
    return JSONResponse({
        "winner": winner,
        "scoreA": scoreA,
        "scoreB": scoreB
    })

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)

# ✅ 헬스 체크용 기본 엔드포인트 추가
@app.get("/")
def root():
    return {"message": "AI Server is running!"}