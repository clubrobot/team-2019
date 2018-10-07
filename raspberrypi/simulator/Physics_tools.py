#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from simulator.Canvas_tools import *
import sys
import simulator.Module.Physics as Physics
from threading import Thread, RLock, Event, current_thread

class Object(printable):
    def __init__(self,name,parent,x,y,theta):
        self.name = name
        self.parent=parent
        self.parent.add(self)
        printable.__init__(self,parent.canvas,x,y,theta)
        self.parent.core.addObject(self.name)
        self.parent.core.setPosition(self.name,x,y)
        self.parent.core.setTheta(self.name,theta)
        self.scored = None
        self.points = 3
    
    def compute(self):
        pass
        #Mettre les méthodes qu'il faut refresh de tant à autre

    def setVelocity(self,lin,ang,forced = False):
        self.parent.core.setVelocity(self.name,lin,ang,forced)

    def getVelocity(self):
        return self.parent.core.getVelocity(self.name)
        
    def setPosition(self,x,y,theta):
        self.parent.core.setPosition(self.name,x,y)
        self.parent.core.setTheta(self.name,theta)

    def getPosition(self):
        return self.parent.core.getPosition(self.name)

    def getTheta(self):
        return self.parent.core.getTheta(self.name)
    
    



    


class Simulator(Thread):

    def __init__(self,window,canvas,timestep=10):
        Thread.__init__(self)
        self.canvas = canvas
        self.daemon = True
        self.window = window
        self.core = Physics.PhysicsMotor()
        self.core.setTimestep(timestep)
        self.elements = dict()
        self.dispensers = list()
        self.running = False

    def launch(self):
        self.core.setTimestep(10)
        self.core.start()
        self.running = True
        self.start()

    def get(self,name):
        return self.elements[name]

    def add(self,element):
        if(isinstance(element, printable)):
            self.elements[element.name]=element

        else:
            raise TypeError("Simulator requiere a Object not a {}".format(type(element)))

    def remove(self,element):
        if(isinstance(element, printable)):
            self.core.removeObject(element.name)
            del self.elements[element.name] 

        else:
            raise TypeError("Simulator requiere a Object not a {}".format(type(element)))

    #Dispenser method
    def add_dispenser(self,disp):
        if(True):
            self.dispensers.append(disp)
        else:
           raise TypeError("Simulator requiere a Dispenser not a {}".format(type(disp))) 

    def get_ball(self,x,y,theta):
        ball = None

        for disp in self.dispensers:
            ball = disp.getball(x,y,theta)
            if not ball is None : break
        return ball


    #Permet au element d'executer leurs fonction interne
    def compute(self):
        for k in self.elements:
            self.elements[k].compute()

    def stop(self):
        self.running = False
        self.core.stop()
        Ball.physics_core.stop()

    def run(self):
        self.compute()
        self.canvas.refresh()
        if self.running:
            self.window.after(10,self.run)

        
        
    def getPoints(self):
        orange = 0
        green = 0
        for element in self.elements:
            if hasattr(self.elements[element], 'scored'):
                if self.elements[element].scored =="Orange":
                    orange+=self.elements[element].points
                if self.elements[element].scored == "Green":
                    green+=self.elements[element].points

        return(orange,green)

    def getCrossState(self,id):
        for element in self.elements:
            if(element == "Cube_{}".format(id)):
                print(element)
                print(self.elements[element].getPosition())
                print(self.elements[element].initial_coordinates)
                return self.elements[element].getPosition()==self.elements[element].initial_coordinates
        return False

    def getDispensetState(self,id):
        return len(self.dispensers[id].container)!=0

    