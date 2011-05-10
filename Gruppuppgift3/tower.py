class Magic:
	type = "none"
	damage = 5
	def SetType(self, aType):
		self.type = aType
	def GetType(self):
		return self.type
#end Magic

class Tower(Magic):
	tID = -1
	range = 4
	damage = 10
	enemyInSight = False
	eID = -1
	upgradeNr = 0
	cooldown = 6
	cooldownEnd = 0
	magic = Magic

	def __init__(self):
		global id
		self.tID = id
		id = id+1

	def EnemyInRange(self, id):
		self.enemyInSight = True
		self.eID = id

	def EnemyLost(self):
		self.enemyInSight = False
		outID = self.eID
		self.eID = -1
		return outID
	
	def Upgrade(self):
		if self.upgradeNr < 3:
			self.range = self.range + 2
			self.damage = self.damage + 5
			self.cooldown = self.cooldown - 1
			self.upgradeNr = self.upgradeNr + 1

	def Attack(self, time):
		res = 0
		if self.cooldownEnd < time and self.enemyInSight:
			res = self.damage + self.magic.damage
			self.cooldownEnd = time + self.cooldown
		return res

	def SetMagic(self, mType):
		self.magic.SetType(self, mType)

	def GetMagicType(self):
		return self.magic.GetType(self)
#end Tower

id = 0
nrOfTowers = 0

towers = []

def AddTower():
	global nrOfTowers
	global towers
	towers.append(Tower())
	print("antal torn: ", nrOfTowers)
	nrOfTowers += 1
	return towers[nrOfTowers-1].tID

def RemoveTower(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		print("i is: ", i, " towersID: ", towers[i].tID)
		if towers[i].tID == id:
			towers.pop(i)
			break
	nrOfTowers -= 1
	for i in range(nrOfTowers):
		print("i is: ", i, " towersID: ", towers[i].tID)

def UpdateTower(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			if towers[i].enemyInSight == False:
				return 1
			else:
				return 2
	return 0

def Attack(id, time):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].eID, towers[i].Attack(time), towers[i].GetMagicType()
	return -1, -1, "none"
		
def UpgradeTower(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			towers[i].Upgrade()
	
def GetTowerID(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].tID
	return -1

def SetTowerMagic(id, newType):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			towers[i].SetMagic(newType)	
	
def GetTowerRadius(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].range
	return 0

def GetEnemyID(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].eID
	return -1
	
def EnemyFound(id, eID):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			towers[i].EnemyInRange(eID)

def EnemyLost(id):
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].EnemyLost()
	return -1
	
def GerTowerRange():
	global towers
	global nrOfTowers
	for i in range(nrOfTowers):
		if towers[i].tID == id:
			return towers[i].range
	return -1		