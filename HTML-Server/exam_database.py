import sqlite3
from sqlite3.dbapi2 import Cursor


class Student:
    pass


class ExamDataBase:
    def __init__(self, dbName="exam.db"):
        self.dbName = dbName
        self.conn = sqlite3.connect(self.dbName, timeout=2.0)

        self.cursor = self.conn.cursor()

    def initializeFile(self):
        self.conn = sqlite3.connect(self.dbName, timeout=2.0)
        self.cursor = self.conn.cursor()
        try:
            self.cursor.execute("CREATE TABLE user \
                                (ID             TEXT PRIMARY KEY    NOT NULL, \
                                 PASSWORD       TEXT                NOT NULL, \
                                 NICKNAME       TEXT                NOT NULL \
                                 )")
        except Exception as e:
            print(e)

        try:
            self.cursor.execute('CREATE TABLE friends \
                                (PAIRID         INT PRIMARY KEY     NOT NULL, \
                                 THISID         TEXT                NOT NULL, \
                                 THATID         TEXT                NOT NULL \
                                 )')
        except Exception as e:
            print(e)

        self.conn.commit()
        self.conn.close()

    def addUser(self, id: str, password: str, nickName: str, friends: list):
        self.conn = sqlite3.connect(self.dbName, timeout=2.0)
        self.cursor = self.conn.cursor()
        self.cursor.execute("INSERT INTO user (ID, PASSWORD, NICKNAME) \
                             VALUES ('%s', '%s', '%s')" % (id, password, nickName))

        curFriends = self.cursor.execute("SELECT PAIRID FROM friends")
        cntFriendsPair = 0
        for ff in curFriends:
            cntFriendsPair += 1

        for f in friends:
            cntFriendsPair += 1
            self.cursor.execute("INSERT INTO friends (PAIRID, THISID, THATID)  \
                                 VALUES ('%d', '%s', '%s')" % (cntFriendsPair, id, f))
            print("INSERT INTO friends (PAIRID, THISID, THATID)  \
                                 VALUES ('%d', '%s', '%s')" % (cntFriendsPair, id, f))
            '''
            cntFriendsPair += 1
            self.cursor.execute("INSERT INTO friends (PAIRID, THATID, THISID)  \
                                 VALUES ('%d', '%s', '%s')" % (cntFriendsPair, f, id))
            print("INSERT INTO friends (PAIRID, THISID, THATID)  \
                                 VALUES ('%d', '%s', '%s')" % (cntFriendsPair, f, id))
                                 '''

        self.conn.commit()
        self.conn.close()

    def userInfo(self) -> dict:
        self.conn = sqlite3.connect(self.dbName, timeout=2.0)
        self.cursor = self.conn.cursor()
        userInfoDict = {}

        userCursor = self.conn.execute(
            "SELECT ID, PASSWORD, NICKNAME FROM user")
        for user in userCursor:
            id = user[0]
            password = user[1]
            nickName = user[2]

            friendsCursor = self.conn.execute(
                "SELECT THATID FROM friends WHERE THISID = '%s'" % (id))
            friends = [i[0] for i in friendsCursor]

            userInfoDict[id] = ChatUser(id, password, nickName, friends)

        self.conn.close()

        return userInfoDict

    def close(self):
        self.conn.close()
