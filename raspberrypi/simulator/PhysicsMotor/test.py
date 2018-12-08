import Physics
import time

class test():
    def __init__(self):
        self.core  =Physics.PhysicsMotor()
        self.core.setTimestep(10)




a = test()

a.core.addObject("bob")

print(a.core.getPosition("bob"))

#a.core.setVelocity("bob",100000,1,False)
a.core.setMaxAcceleration("bob",2,1)
a.core.setMinAcceleration("bob",4,1)
a.core.start()
a.core.setShape("bob", ((5,3.5), (10,245) )  )
a.core.setPosition("bob",1000,1000)
print(a.core.getShape("bob"))
time.sleep(3)
print(a.core.getShape("bob"))


'''
for k in range(100):
    time.sleep(0.05)
    print(",{} ".format(a.getPosition("bob")))
a.setVelocity("bob",-10000,0,False)
print(",{} ".format(a.getPosition("bob")))
for  k in range(100):
    time.sleep(0.05)
    print(",{} ".format(a.getPosition("bob")))
'''