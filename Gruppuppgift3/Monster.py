import random

class Monster:
	id = -1
	lvl = -1
	hp = 0
	isAlive = False
	iceRes = 0
	fireRes = 0
	earthRes = 0
	def __init__(self, id, hp, lvl, iA, iR, fR, eR):
		self.id = id
		self.lvl = lvl
		self.hp = hp
		self.isAlive = iA		
		self.iceRes = iR
		self.fireRes = fR
		self.earthRes = eR
	def getisAlive(self):
		return self.isAlive
	def setisAlive(self, iA):
		self.isAlive = iA


class mHandler():
	nrOfMonsters = -1
	level = -1
	monsters = []
	def initHandler(self, nr, lvl):
		self.nrOfMonsters = nr
		self.level = lvl
	def initMonsters(self):
		for i in range(0, self.nrOfMonsters):
			self.monsters.append(Monster(i, 100, self.level, True, random.random(), random.random(), random.random()))
	def calcDmg(self, Mid, dmg, dmgType):
		for i in range(0, self.nrOfMonsters):
			if self.monsters[i].id == Mid:
				if dmgType == "Fire":
					self.monsters[i].hp -= dmg * self.monsters[i].fireRes
					return str(self.monsters[i].hp)
				if dmgType == "Ice":
					self.monsters[i].hp -= dmg * self.monsters[i].iceRes
					return "dmg"
				if dmgType == "Earth":
					self.monsters[i].hp -= dmg * self.monsters[i].earthRes
					return "dmg"
				if dmgType ==  "None":
					self.monsters[i].hp -= dmg
					return "dmg"

	def updateM(self):
		for i in range(0, self.nrOfMonsters):
			if self.monsters[i].isAlive == True:
				if self.monsters[i].hp <= 0:
					self.monsters[i].isAlive = False
	def getIsAlive(self, pos):
		temp = self.monsters[pos].isAlive
		return temp
		
	
			
f = mHandler()

def setHandler(nr, lvl):
	global f
	f.initHandler(nr,lvl)
	f.initMonsters()

def setUpdate():
	global f
	f.updateM()

def setDmg(id, dmg, dmgType):
	global f
	s = f.calcDmg(id, dmg, str(dmgType))
	return s

def getAlive(pos):
	global f
	b = f.getIsAlive(pos)
	return b

