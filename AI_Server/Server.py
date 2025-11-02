from fastapi import FastAPI, UploadFile, File
from typing import List
from PIL import Image
import io
import numpy as np
import uvicorn

app = FastAPI()

def calc_similarity(img1, img2):
    img1 = img1.resize((128, 128)).convert("RGB")
    img2 = img2.resize((128, 128)).convert("RGB")
    arr1 = np.array(img1).astype(np.float32)
    arr2 = np.array(img2).astype(np.float32)
    diff = np.mean((arr1 - arr2) ** 2)
    return 1.0 / (1.0 + diff / 1000.0)

@app.post("/compare_textures/")
async def compare_textures(
    original: UploadFile = File(...),
    comparisons: List[UploadFile] = File(...)
):
    orig_img = Image.open(io.BytesIO(await original.read()))
    best_score = -1.0
    best_name = None

    for comp in comparisons:
        comp_img = Image.open(io.BytesIO(await comp.read()))
        score = calc_similarity(orig_img, comp_img)
        if score > best_score:
            best_score = score
            best_name = comp.filename

    return {"best_match": best_name, "score": float(best_score)}

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)
