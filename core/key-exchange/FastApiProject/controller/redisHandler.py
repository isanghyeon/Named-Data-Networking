import logging
from typing import Union
import datetime
from redis.asyncio import Redis, from_url
import string


class redisObject:
    def __init__(self, db: int = 0, ttl: int = -1):
        self.handler = None
        self.db = db
        self.ttl = ttl

    async def initialization(self) -> from_url:
        self.handler = await from_url(f"redis://localhost:6379/{self.db}")

    async def setObject(self, object: Union[dict, list]) -> None:
        print("[*] setObject", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")

        await self.initialization()

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                print(f"[+] type :: {type(object)}")

                if type(object) is dict:
                    if not all(object.values()):
                        for idx in object.keys():
                            object[idx] = '' if object[idx] is None else object[idx]
                            # raise Exception("Key generator is down, Please contact admin.")

                    if self.db == 0:
                        await pipe.set(object["item"], object["value"], object["ttl"]).execute()


                if type(object) is list:
                    for objVar in object:
                        if not all(objVar.values()):
                            raise Exception("Key generator is down, Please contact admin.")
                            # for idx in objVar.keys():
                            # objVar[idx] = '' if objVar[idx] is None else objVar[idx]

                        if self.db == 1:
                            await pipe.set(objVar["name"], objVar["value"]).execute()

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")

    async def getAllObject(self, object: str):
        await self.initialization()
        async with self.handler.pipeline(transaction=True) as pipe:
            result = await pipe.hgetall(name=object).execute()

        assert result

        return result

    async def getObject(self, object: dict):
        await self.initialization()
        async with self.handler.pipeline(transaction=True) as pipe:
            result = await pipe.hmget(name=object["name"], keys=object["keys"]).execute()
            EOL = await pipe.ttl(name=object["name"]).execute()

        assert result
        assert EOL

        return result, EOL
