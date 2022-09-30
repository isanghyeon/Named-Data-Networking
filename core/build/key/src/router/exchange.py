import time
import uuid
import datetime
from fastapi import APIRouter, HTTPException, Header, Request
from starlette.responses import JSONResponse
from starlette.status import HTTP_200_OK, HTTP_400_BAD_REQUEST
from controller.redisHandler import redisObject
from controller.exchangeHandler import exchange

app = APIRouter()


@app.get("/exchange")
async def appExchange(Authorization: list[str] | None = Header(default=None)):
    """

    :param Authorization: list[str] => [0] hashed name, [1] node id
    :return:
    """
    try:
        sKey, fpeKey, fpeTweak = await exchange(name=Authorization[0], node_id=Authorization[1]).encryptHandler()
        return JSONResponse(
            status_code=HTTP_200_OK,
            content={
                "status_code": HTTP_200_OK,
                "message": "success",
                "data": {
                    "name": Authorization[0],
                    "sKey": sKey,
                    "fpeKey": fpeKey,
                    "fpeTweak": fpeTweak
                }
            }
        )
    except Exception as e:
        print("[Exchange] error:: ", e, " - ", datetime.now().strftime("%Y-%m-%d %H:%M:%S"))


@app.get("/exchange/pkey")
async def appKeyExchange(Authorization: list[str] | None = Header(default=None)):
    """

    :param Authorization: list[str] => [0] hashed name, [1] node public key, [2] node id or empty
    :return:
    """
    node_id = None

    try:
        if len(Authorization) == 2 or Authorization[2] is None or not Authorization[2]:
            node_id = uuid.uuid4()
        else:
            node_id = Authorization[2]

        await redisObject(db=1).setObject(object={
            "name": f"{Authorization[0]}:node-key:{node_id}",
            "value": Authorization[1]
        })

    finally:
        apiPuKey = (await redisObject(db=1).getObject(object=f"{Authorization[0]}:public-key", types=True))[0]

        return JSONResponse(
            status_code=HTTP_200_OK,
            content={
                "status_code": HTTP_200_OK,
                "message": "success",
                "data": {
                    "name": Authorization[0],
                    "apiPubKey": apiPuKey,
                    "nodeId": str(node_id)
                }
            }
        )
