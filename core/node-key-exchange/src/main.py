import os
import sys

from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.exceptions import RequestValidationError
from starlette.exceptions import HTTPException as StarletteHTTPException
from starlette.responses import JSONResponse

sys.path.append(os.path.dirname(os.path.abspath(os.path.dirname(__file__))))

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"]
)

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
    return {"message": "Hello World"}

# if __name__ == '__main__':
#     application = createApplication()
#     uvicorn.run(app="main:application", host="127.0.0.1", port=8000, reload=True)
