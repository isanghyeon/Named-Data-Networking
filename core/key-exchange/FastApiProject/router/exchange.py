import time
from typing import Union, Optional

from fastapi import APIRouter, HTTPException, Header, Request
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST
from controller.redisHandler import redisObject
from controller.exchangeHandler import exchange

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
async def appKeyExchange(Authorization: list[str] = Header(default=None)):
    # return JSONResponse(
    #     status_code=HTTP_200_OK,
    #     content={
    #         "name": Authorization[0][:Authorization[0].find(',')],
    #         "pkey": Authorization[0][Authorization[0].find(',') + 2:]
    #     }
    # )

    pkey = await redisObject(db=1).getObject(object=f"{Authorization[0][:Authorization[0].find(',')]}:public-key", types=True)
    nodePKey = Authorization[0][Authorization[0].find(',') + 2:]

    return JSONResponse(
        status_code=HTTP_200_OK,
        content={
            "name": Authorization[0][:Authorization[0].find(',')],
            "pkey": pkey[0].decode('utf-8')
        }
    )


@app.get("/items/{item_id}")
async def read_root(item_id: str, request: Request):
    client_host = request.headers
    return {"client_host": client_host, "item_id": item_id}
