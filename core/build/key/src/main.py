import os
import sys
from datetime import datetime

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.exceptions import RequestValidationError
from starlette.exceptions import HTTPException as StarletteHTTPException
from starlette.responses import JSONResponse
from loguru import logger

from utils import logging as logType

sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"]
)

logger.add("log/access_log", format="{time:YYYY-MM-DDTHH:mm:ssZ} | {level} | {message}", rotation="100 MB")

from router import exchange, generator, lifecycle

app.include_router(
    exchange.app,
    tags=["exchange"],
    # dependencies=[Depends(getHeaderAuthorization)],
    responses={200: {"description": "key exchange api"}}
)
app.include_router(
    generator.app,
    tags=["generator"],
    # dependencies=[Depends(getHeaderAuthorization)],
    responses={200: {"description": "key generator api"}}
)
app.include_router(
    lifecycle.app,
    tags=["lifecycle"],
    # dependencies=[Depends(getHeaderAuthorization)],
    responses={200: {"description": "key lifecycle api"}}
)


@app.exception_handler(StarletteHTTPException)
async def httpExceptionHandler(request, exc):
    logger.debug(f"[{request.client.host}] [{exc.status_code}] - {request.method}: {request.url} - {exc.detail}")

    return JSONResponse(
        status_code=exc.status_code,
        content={
            "status_code": exc.status_code,
            "message": exc.detail,
            "data": {}
        }
    )


@app.exception_handler(RequestValidationError)
async def validationExceptionHandler(request, exc):
    logger.debug(f"[{exc.status_code}] - [{request.client.host}] {request.method}: {request.url} - {exc.detail}")

    print(repr(exc))
    print(repr(request))
    return JSONResponse(
        status_code=exc.status_code,
        content={
            "status_code": exc.status_code,
            "message": exc.detail,
            "data": {}
        }
    )


@app.get("/")
async def root():
    # logger.debug(f"[*] [{exc.status_code}] - [{request.client.host}] {exc.detail}")
    return {"message": "Hello World"}

# if __name__ == '__main__':
#     application = createApplication()
#     uvicorn.run(app="main:application", host="0.0.0.0", port=8000, reload=True)
