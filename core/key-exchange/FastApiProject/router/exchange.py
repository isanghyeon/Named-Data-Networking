import time
from typing import Union, List

from fastapi import APIRouter, HTTPException, Header
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST

app = APIRouter()


@app.get("/exchange")
async def appExchange(Authorization: Union[str, None] = Header(default=None)):
    return JSONResponse(
        status_code=HTTP_200_OK,
        content={
            "status_code": HTTP_200_OK,
            "message": "success",
            "data": {}
        }
    )


@app.get("/exchange/pkey")
async def appKeyExchange(Authorization: Union[list, None] = Header(default=None)):
    print(type(Authorization), Authorization)
    return JSONResponse(
        status_code=HTTP_200_OK,
        content=Authorization
    )
