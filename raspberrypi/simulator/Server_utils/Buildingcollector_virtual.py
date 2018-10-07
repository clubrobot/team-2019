#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from Server_utils.Components_virtual import SerialTalksComponent
from serialutils import *
from math import *
import time

GRAB_LEFT_SIDE_OPCODE   = 0x10
GRAB_RIGHT_SIDE_OPCODE   = 0x20
GRAB_CENTER_SIDE_OPCODE = 0x30

DROP_TOWER_OPCODE       = 0x40

class BuildingCollector(SerialTalksComponent):
    def __init__(self,parent):
        SerialTalksComponent.__init__(self,"buildingcollector")
        self.add_method(GRAB_LEFT_SIDE_OPCODE,   self.grab_left )
        self.add_method(GRAB_RIGHT_SIDE_OPCODE , self.grab_right )
        self.add_method(GRAB_CENTER_SIDE_OPCODE, self.grab_center)
        self.add_method(DROP_TOWER_OPCODE,       self.drop_tower)
        self.robot = parent
        self.parent = parent.parent

    
    def grab_left(self):
        x , y = self.parent.core.getPosition(self.robot.name)
        theta = self.parent.core.getTheta(self.robot.name)
        x+=65*cos(theta)-50*sin(theta)
        y+=65*sin(theta)+50*cos(theta)
        cube_taken = self.parent.core.getObject(x,y,50)
        if not cube_taken in ["None",self.robot.name]:
            self.robot.container.append(cube_taken)
        else:
            return 
        self.parent.core.disableObject(cube_taken)
        for i in range(65):
            x , y = self.parent.core.getPosition(self.robot.name)
            theta = self.parent.core.getTheta(self.robot.name)
            x+=(65-i)*cos(theta)-55*sin(theta)
            y+=(65-i)*sin(theta)+55*cos(theta)
            self.parent.core.setPosition(cube_taken,x,y)
            time.sleep(0.02)
        self.parent.core.setPosition(cube_taken,-1000,-1000)
    
    def grab_right(self):
        x , y = self.parent.core.getPosition(self.robot.name)
        theta = self.parent.core.getTheta(self.robot.name)
        x+=65*cos(theta)+50*sin(theta)
        y+=65*sin(theta)-50*cos(theta)
        cube_taken = self.parent.core.getObject(x,y,50)
        if not cube_taken in ["None",self.robot.name]:
            self.robot.container.append(cube_taken)
        else:
            return 
        self.parent.core.disableObject(cube_taken)
        for i in range(65):
            x , y = self.parent.core.getPosition(self.robot.name)
            theta = self.parent.core.getTheta(self.robot.name)
            x+=(65-i)*cos(theta)+55*sin(theta)
            y+=(65-i)*sin(theta)-55*cos(theta)
            self.parent.core.setPosition(cube_taken,x,y)
            time.sleep(0.02)
        self.parent.core.setPosition(cube_taken,-1000,-1000)

    def grab_center(self):
        x , y = self.parent.core.getPosition(self.robot.name)
        theta = self.parent.core.getTheta(self.robot.name)
        x+=65*cos(theta)
        y+=65*sin(theta)
        cube_taken = self.parent.core.getObject(x,y,50)
        if not cube_taken in ["None",self.robot.name]:
            self.robot.container.append(cube_taken)
        else:
            return 
        self.parent.core.disableObject(cube_taken)
        for i in range(65):
            x , y = self.parent.core.getPosition(self.robot.name)
            theta = self.parent.core.getTheta(self.robot.name)
            x+=(65-i)*cos(theta)
            y+=(65-i)*sin(theta)
            self.parent.core.setPosition(cube_taken,x,y)
            time.sleep(0.02)
        self.parent.core.setPosition(cube_taken,-1000,-1000)

    def drop_tower(self):
        x , y = self.parent.core.getPosition(self.robot.name)
        theta = self.parent.core.getTheta(self.robot.name)
        print(x,y)
        x-=65*cos(theta)
        y-=65*sin(theta)
        for cube in self.robot.container:
            if(x<300 and y>400 and y<1000) : self.parent.get(cube).scored = "Green"
            if(x<300 and y>2000 and y<2500) : self.parent.get(cube).scored = "Orange"
            self.parent.core.setPosition(cube,x,y)
        self.robot.container = list()

    def compute(self):
        pass
