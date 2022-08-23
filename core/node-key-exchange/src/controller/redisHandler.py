import datetime
import string
from redis.asyncio import Redis, from_url


class redisObject:
    def __init__(self, db: int = 0, ttl: int = -1):
        """

        :param db:
        :param ttl:
        """
        self.handler = None
        self.db = db
        self.ttl = ttl

    async def initialization(self) -> from_url:
        """

        :return:
        """
        self.handler = await from_url(f"redis://localhost:6379/{self.db}", decode_responses=True)

    async def setObject(self, object: dict | list) -> None:
        """

        :param object:
        :return:
        """
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
                        await pipe.set(name=object["item"], ex=None if object["ttl"] == "" or not object["ttl"] else object["ttl"], value=object["value"]).execute()
                    if self.db == 1:
                        await pipe.set(name=object["name"], value=object["value"]).execute()

                if type(object) is list:
                    for objVar in object:
                        if not all(objVar.values()):
                            raise Exception("Key generator is down, Please contact admin.")
                            # for idx in objVar.keys():
                            # objVar[idx] = '' if objVar[idx] is None else objVar[idx]

                        if self.db == 1:
                            await pipe.set(name=objVar["name"], value=objVar["value"]).execute()

        except Exception as e:
            print("[-] error:: ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")

    async def setExpired(self, object: dict) -> None:
        """

        :param object:
        :return:
        """
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

    async def getObject(self, object: str, types: bool) -> str | list | bytes:
        """

        :param object:
        :param types:
        :return:
        """
        print("[*] getObject", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")
        await self.initialization()

        result = str | list | bytes

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                if types is True:  # one key -> str
                    result = await pipe.get(name=object).execute()

                if types is False:  # various key -> list
                    result = (await pipe.keys(pattern=f"{object}:*").execute())[0]

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")
            return result

    async def getTTL(self, object: str) -> int:
        """

        :param object:
        :return:
        """
        print("[*] getTTL", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        print("[+] start")
        await self.initialization()

        result = int

        try:
            print("[+] transaction init")
            async with self.handler.pipeline(transaction=True) as pipe:
                result = (await pipe.ttl(name=object).execute())[0]

            if 180 > result > 60:
                await self.setExpired(object={
                    "name": object,
                    "time": result + 300
                })

        except Exception as e:
            print("[-] ", e, " ", datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        finally:
            print("[*] done")
            return result
