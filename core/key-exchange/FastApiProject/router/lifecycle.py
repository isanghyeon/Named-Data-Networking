import json
import time
from typing import Union, List

from fastapi import APIRouter, HTTPException, Header
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST

from controller.redisHandler import redisObject

app = APIRouter()


@app.get("/lifecycle")
async def appLifecycle(Authorization: Union[str, None] = Header(default=None)):
    EoL = {}

    try:
        redisObj = redisObject(db=0)

        key = await redisObj.getObject(object=Authorization, types=False)

        for _ in key:
            EoL[_.decode('utf-8')] = await redisObj.getTTL(_.decode('utf-8'))
            if EoL[_.decode('utf-8')] == -1:
                del EoL[_.decode('utf-8')]

    except Exception as e:
        raise HTTPException(status_code=HTTP_400_BAD_REQUEST, detail=str(e))
    finally:
        return JSONResponse(
            status_code=HTTP_200_OK,
            content={
                "status_code": HTTP_200_OK,
                "message": "OK",
                "data": EoL
            }
        )
