from Cryptodome.PublicKey import RSA
from Cryptodome.Cipher import AES, PKCS1_OAEP
from controller.redisHandler import redisObject
import datetime
import string
import base64
import random


class exchange:
    def __init__(self, name: str = None, node_id: str = None):
        """

        :param name:
        :param node_id:
        """
        self.sKey = None
        self.fpeKey = None
        self.fpeTweak = None

        self.prKey = None
        self.puKey = None
        self.nodePuKey = None

        self.redisDB_0 = redisObject(db=0)
        self.redisDB_1 = redisObject(db=1)

        self.symmetricObject = None
        self.nonce = None
        self.AsymmetricObject = None

        self.name = name
        self.node_id = node_id

    async def getPrKeyInDB(self):
        _keys_0 = await self.redisDB_0.getObject(object=self.name, types=False)
        _keys_1 = await self.redisDB_1.getObject(object=self.name, types=False)

        for _ in _keys_0:
            await self.redisDB_0.getTTL(object=_)

        self.sKey = (await self.redisDB_0.getObject(object=list(filter(lambda x: 'SKey' in x, _keys_0))[0], types=True))[0].encode('utf-8')
        self.fpeKey = (await self.redisDB_0.getObject(object=list(filter(lambda x: 'FPEKey' in x, _keys_0))[0], types=True))[0].encode('utf-8')
        self.fpeTweak = (await self.redisDB_0.getObject(object=list(filter(lambda x: 'FPETweak' in x, _keys_0))[0], types=True))[0].encode('utf-8')

        self.puKey = (await self.redisDB_1.getObject(object=list(filter(lambda x: 'private' in x, _keys_1))[0], types=True))[0].encode('utf-8')
        self.prKey = (await self.redisDB_1.getObject(object=list(filter(lambda x: 'public' in x, _keys_1))[0], types=True))[0].encode('utf-8')

        self.nodePuKey = (await self.redisDB_1.getObject(object=list(filter(lambda x: f'{self.node_id}' in x, _keys_1))[0], types=True))[0].encode('utf-8')
        self.nodePuKey = base64.b64decode(self.nodePuKey)

        self.symmetricObject = AES.new(self.sKey, AES.MODE_ECB)
        self.AsymmetricObject = PKCS1_OAEP.new(RSA.import_key(self.nodePuKey))

        # return self

    async def encryptHandler(self) -> tuple[str, str, str]:
        await self.getPrKeyInDB()

        print(f"[*] encryptHandler :: {self.AsymmetricObject.encrypt(self.sKey)}, {len(self.AsymmetricObject.encrypt(self.sKey))}")

        self.fpeKey = base64.b64encode(self.symmetricObject.encrypt(self.fpeKey)).decode('utf-8')
        self.fpeTweak = base64.b64encode(self.symmetricObject.encrypt(self.fpeTweak)).decode('utf-8')
        self.sKey = base64.b64encode(self.AsymmetricObject.encrypt(self.sKey)).decode('utf-8')
        print("[*] encryptHandler :: self.sKey, self.fpeKey, self.fpeTweak :: ", self.sKey, self.fpeKey, self.fpeTweak)
        return self.sKey, self.fpeKey, self.fpeTweak

    async def decryptHandler(self) -> tuple[str, str, str]:
        await self.getPrKeyInDB()

        async def symmetric(self):
            pass

        async def Asymmetric(self):
            pass
