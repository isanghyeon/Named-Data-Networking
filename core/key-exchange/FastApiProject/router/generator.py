import time
from fastapi import APIRouter, HTTPException, Header
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST

from controller.redisHandler import redisObject
from controller.generateHandler import generator

app = APIRouter()


@app.get("/generator")
async def appGenerator(Authorization: str | None = Header(default=None)):
    try:
        await generator().publicKey(name=Authorization)
        await generator().fpeKey(name=Authorization)

    except Exception as e:
        raise HTTPException(status_code=HTTP_400_BAD_REQUEST, detail=str(e))
    finally:
        raise HTTPException(status_code=HTTP_200_OK, detail="OK")
