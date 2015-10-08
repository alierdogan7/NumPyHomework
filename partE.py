#!/usr/bin/env python
import random
import time
 
class Account:
    def __init__(self, id, name, balance):
        self.id = id
        self.name = name
        self.balance = balance
 
    def __str__(self):
        return "[id:%d, name: %s, balance: %s]" % (self.id, self.name, self.balance)
 
    def getId(self):
        return self.id
 
    def getName(self):
        return self.name
 
    def getBalance(self):
        return self.balance
 
    def withdraw(self, amount):
        self.balance -= amount
        return self.balance
 
    def deposit(self, amount):
        self.balance += amount
        return self.balance
 
def generateRandomAccounts(n):
    accounts = []
    nameLen = 5
    balanceMax = 1000
    alpha = 'abcdefghijklmnopqrstuvwxyz'
    for id in xrange(1, n+1):
        name = ""
        for i in xrange(0, nameLen):
            name += random.choice(alpha)
        if id==1:
            print "First account name is '%s'" % name
        balance = random.choice(xrange(0, balanceMax))
        account = Account(id, name, balance)    
        accounts.append(account)
        account2 = Account(id+77, name, balance)
        accounts.append(account2)
    return accounts
 
def findAccountIndices(name, accounts):
    indices = []
    for (index, account) in enumerate(accounts):
        if account.getName() == name:
            indices.append(index)
    return indices

def createNameMap(accounts):
    map = {}
    for account in accounts:
        name = account.getName()
        id = account.getId()

        if map.get(name) is not type([]):
            map[name] = [ id ]
        else:
            map[name].append( id )

    return map
 
start = time.time()
accounts = generateRandomAccounts(1000000)
end = time.time()
print "Accounts created in %f seconds" % (end-start)
start = time.time()
nameMap = createNameMap(accounts) ##### Build a map to speed things up
end = time.time()
print "Name map created in %f seconds" % (end-start)
while True:
    name = raw_input("Account name: ")
    if name == "":
        print "Exiting..."
        break
    start = time.time()
    indices = nameMap.get(name, []) ##### Fast dictionary lookup
    end = time.time()
    if len(indices)>0:
        for index in indices:
            print "> Account found: %s" % accounts[index]
    else:
        print "> No accounts found."
    print "Search took %f seconds" % (end-start)