import datetime
import string
from typing import Union
from redis.asyncio import Redis, from_url


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
                if type(object) is dict:
                    if not all(object.values()):
                        for idx in object.keys():
                            object[idx] = '' if object[idx] is None else object[idx]
                            # raise Exception("Key generator is down, Please contact admin.")

                    if self.db == 0:
                        await pipe.set(name=object["item"], ex=None if object["ttl"] == "" else object["ttl"], value=object["value"]).execute()

                if type(object) is list:
                    for objVar in object:
                        if not all(objVar.values()):
                            raise Exception("Key generator is down, Please contact admin.")
                            # for idx in objVar.keys():
                            # objVar[idx] = '' if objVar[idx] is None else objVar[idx]

                        if self.db == 1:
                            await pipe.set(name=objVar["name"], value=objVar["value"]).execute()

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")

    async def setExpired(self, object: dict) -> None:
        print("[*] setExpired", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")
        await self.initialization()

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                await pipe.expire(name=object["name"], time=object["time"], xx=True).execute()

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")

    async def getObject(self, object: str, types: bool) -> Union[str, list]:
        print("[*] getObject", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")
        await self.initialization()

        result = Union[str, list]

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                if types is True:  # one key -> str
                    result = await pipe.get(name=object).execute()

                if types is False:  # various key -> dict
                    result = (await pipe.keys(pattern=f"{object}:*").execute())[0]

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")
            return result

    async def getTTL(self, object: str) -> int:
        print("[*] getTTL", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")
        await self.initialization()

        result = int

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                result = (await pipe.ttl(name=object).execute())[0]

            if 180 > result > 100:
                await self.setExpired(object={
                    "name": object,
                    "time": result + 300
                })

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")
            return result
