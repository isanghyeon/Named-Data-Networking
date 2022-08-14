import time
from typing import Union, List

from fastapi import APIRouter, HTTPException, Header
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST

from controller.redisHandler import redisObject

app = APIRouter()


@app.get("/lifecycle")
async def appLifecycle(Authorization: Union[str, None] = Header(default=None)):
    result, EOL = await redisObject().getObject(object={
        "name": Authorization,
        "keys": ["generate-time", "exchange-time"]
    })

    data = {
        "name": Authorization,
        "generate-time": result[0][0].decode("utf-8"),
        "exchange-time": result[0][1].decode("utf-8"),
        "key": None,
        "tweak": None,
        "lifecycle": EOL[0]
    }

    print(data)


    return JSONResponse(
        status_code=HTTP_200_OK,
        content={
            "status_code": HTTP_200_OK,
            "message": "success",
            "data": data
        }
    )
