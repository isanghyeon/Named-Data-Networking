import random
import string
import time
import datetime
import json
from Cryptodome.PublicKey import RSA
from Cryptodome import Random
from controller.redisHandler import redisObject


# keyExport Obj is same key
class keyExport:
    SKey = "".join(random.choice(string.hexdigits.upper()) for _ in range(16))
    FPEKey = {
        "key": "".join(random.choice(string.hexdigits.upper()) for _ in range(32)),
        "tweak": "".join(random.choice(string.hexdigits.upper()) for _ in range(16))
    }
    puKeyPair = RSA.generate(3096, Random.new().read)


class endOfLifetime:
    EOL = 1800


class generator:
    # RSA public
    @staticmethod
    async def publicKey(name: str = None, **kwargs):
        if name is None:
            from fastapi.exceptions import HTTPException
            raise HTTPException(status_code=400, detail="400 Bad Request")

        publicKey, privateKey = keyExport.puKeyPair.public_key().export_key(), keyExport.puKeyPair.export_key()

        try:
            await redisObject(db=1).setObject(object=[
                {
                    "name": f"{name}:private-key",
                    "value": privateKey.decode('utf-8')
                },
                {
                    "name": f"{name}:public-key",
                    "value": publicKey.decode('utf-8')
                }
            ])

        except Exception as e:
            raise Exception("[Generate handler] - PK is down")

    @staticmethod
    async def fpeKey(name: str = None, **kwargs):
        if name is None:
            from fastapi.exceptions import HTTPException
            raise HTTPException(status_code=400, detail="400 Bad Request")

        data = {
            "generate-time": None,
            "exchange-time": None,
            "FPEKey": keyExport.FPEKey["key"],
            "FPETweak": keyExport.FPEKey["tweak"],
            "SKey": keyExport.SKey
        }

        try:
            redisObj = redisObject(db=0)

            for _ in data.keys():
                await redisObj.setObject(object={
                    "item": f"{name}:{_}",
                    "value": data[_],
                    "ttl": endOfLifetime.EOL if "-time" not in _ else None
                })

        except Exception as e:
            raise Exception("[Generate handler] - FPE key is down")

    # Not Using
    # Format-Preserving Encryption
    # async def keyGenerate(self, **kwargs) -> str:
    #     return self.keyString.join(random.choice(self.stringCase) for _ in range(self.keyLength))
    #
    # async def tweakGenerate(self, **kwargs) -> str:
    #     return self.tweakString.join(random.choice(self.stringCase) for _ in range(self.tweakLength))
    #
    # async def lifecycleGenerate(self, **kwargs) -> float:
    #     return self.EOL
