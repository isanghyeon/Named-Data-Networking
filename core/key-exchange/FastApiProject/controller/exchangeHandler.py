from Cryptodome.PublicKey import RSA
from Cryptodome.Cipher import AES, PKCS1_OAEP
from controller.redisHandler import redisObject


class exchange:
    def __init__(self, name: str = None):
        self.sKey = None
        self.fpeKey = None
        self.fpeTweak = None
        self.prKey = None
        self.puKey = None

        self.redisDB_0 = redisObject(db=0)
        self.redisDB_1 = redisObject(db=1)
        self.name = name

    async def getKeyInDB(self):
        _keys_0 = await self.redisDB_0.getObject(object=self.name, types=False)
        _keys_1 = await self.redisDB_1.getObject(object=self.name, types=False)

        self.sKey = (await self.redisDB_0.getObject(object=_keys_0[2], types=True))[0]
        self.fpeKey = (await self.redisDB_0.getObject(object=_keys_0[4], types=True))[0].decode('utf-8')
        self.fpeTweak = (await self.redisDB_0.getObject(object=_keys_0[0], types=True))[0].decode('utf-8')

        self.puKey = (await self.redisDB_1.getObject(object=_keys_1[0], types=True))[0]
        self.prKey = (await self.redisDB_1.getObject(object=_keys_1[1], types=True))[0]

    async def symmetricEncrypt(self):
        self.sKey

    async def symmetricDecrypt(self):
        pass

    async def AsymmetricEncrypt(self):
        pass

    async def AsymmetricDecrypt(self):
        pass
