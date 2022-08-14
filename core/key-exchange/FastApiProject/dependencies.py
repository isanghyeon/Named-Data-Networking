from fastapi import Header, HTTPException


async def getHeaderAuthorization(Authorization: str = Header()):
    if Authorization is None or not Authorization:
        raise HTTPException(status_code=400, detail="Bad Request")


