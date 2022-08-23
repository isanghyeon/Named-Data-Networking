import json
import time
from fastapi import APIRouter, HTTPException, Header
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST

from controller.redisHandler import redisObject

app = APIRouter()


@app.get("/lifecycle")
async def appLifecycle(Authorization: str | None = Header(default=None)):
    """

    :param Authorization:
    :return:
    """
    EoL = {}

    try:
        redisObj = redisObject(db=0)
        key = await redisObj.getObject(object=Authorization, types=False)

        for _ in key:
            EoL[_[_.find(":") + 1:]] = await redisObj.getTTL(object=_)

            if EoL[_[_.find(":") + 1:]] == -1:
                del EoL[_[_.find(":") + 1:]]

        return JSONResponse(
            status_code=HTTP_200_OK,
            content={
                "status_code": HTTP_200_OK,
                "message": "success",
                "data": EoL
            }
        )

    except Exception as e:
        raise HTTPException(status_code=HTTP_400_BAD_REQUEST, detail=str(e))
