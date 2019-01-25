#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from Server_utils.Components_virtual import SerialTalksComponent
from serialutils import *
from math import *
# Instructions

SET_OPENLOOP_VELOCITIES_OPCODE  = 0x04

GET_CODEWHEELS_COUNTERS_OPCODE  = 0x0D

SET_VELOCITIES_OPCODE           = 0x06

START_PUREPURSUIT_OPCODE        = 0x07
START_TURNONTHESPOT_OPCODE      = 0x09

POSITION_REACHED_OPCODE         = 0x08

SET_POSITION_OPCODE	            = 0x0A
GET_POSITION_OPCODE	            = 0x0B
GET_VELOCITIES_OPCODE           = 0x0C

SET_PARAMETER_VALUE_OPCODE      = 0x0E
GET_PARAMETER_VALUE_OPCODE      = 0x0F

RESET_PUREPURSUIT_OPCODE        = 0x10
ADD_PUREPURSUIT_WAYPOINT_OPCODE = 0x11

LEFTWHEEL_RADIUS_ID	            = 0x10
LEFTWHEEL_CONSTANT_ID           = 0x11
LEFTWHEEL_MAXPWM_ID             = 0x12
RIGHTWHEEL_RADIUS_ID            = 0x20
RIGHTWHEEL_CONSTANT_ID          = 0x21
RIGHTWHEEL_MAXPWM_ID            = 0x22
LEFTCODEWHEEL_RADIUS_ID	        = 0x40
LEFTCODEWHEEL_COUNTSPERREV_ID   = 0x41
RIGHTCODEWHEEL_RADIUS_ID        = 0x50
RIGHTCODEWHEEL_COUNTSPERREV_ID  = 0x51
ODOMETRY_AXLETRACK_ID           = 0x60
ODOMETRY_SLIPPAGE_ID            = 0x61
VELOCITYCONTROL_AXLETRACK_ID    = 0x80
VELOCITYCONTROL_MAXLINACC_ID    = 0x81
VELOCITYCONTROL_MAXLINDEC_ID    = 0x82
VELOCITYCONTROL_MAXANGACC_ID    = 0x83
VELOCITYCONTROL_MAXANGDEC_ID    = 0x84
VELOCITYCONTROL_SPINSHUTDOWN_ID = 0x85
LINVELPID_KP_ID                 = 0xA0
LINVELPID_KI_ID                 = 0xA1
LINVELPID_KD_ID                 = 0xA2
LINVELPID_MINOUTPUT_ID          = 0xA3
LINVELPID_MAXOUTPUT_ID          = 0xA4
ANGVELPID_KP_ID                 = 0xB0
ANGVELPID_KI_ID                 = 0xB1
ANGVELPID_KD_ID                 = 0xB2
ANGVELPID_MINOUTPUT_ID	        = 0xB3
ANGVELPID_MAXOUTPUT_ID	        = 0xB4
POSITIONCONTROL_LINVELKP_ID     = 0xD0
POSITIONCONTROL_ANGVELKP_ID     = 0xD1
POSITIONCONTROL_LINVELMAX_ID    = 0xD2
POSITIONCONTROL_ANGVELMAX_ID    = 0xD3
POSITIONCONTROL_LINPOSTHRESHOLD_ID  = 0xD4
POSITIONCONTROL_ANGPOSTHRESHOLD_ID  = 0xD5
PUREPURSUIT_LOOKAHEAD_ID        = 0xE0
PUREPURSUIT_LOOKAHEADBIS_ID     = 0xE2
FLOAT  = FloatType('f')
BYTEORDER = 'little'
LONG   = IntegerType(4, BYTEORDER, True)
INT    = IntegerType(2, BYTEORDER, True)
BYTE   = IntegerType(1, BYTEORDER, False)



#SPIN A FAIRE


class Wheeledbase(SerialTalksComponent):
    def __init__(self,parent):
        SerialTalksComponent.__init__(self,"wheeledbase")
        self.add_method(SET_VELOCITIES_OPCODE,         self.set_velocity )
        self.add_method(GET_VELOCITIES_OPCODE ,        self.get_velocity )
        self.add_method(SET_PARAMETER_VALUE_OPCODE,    self.set_parameter)
        self.add_method(GET_PARAMETER_VALUE_OPCODE,    self.get_parameter)
        self.add_method(SET_OPENLOOP_VELOCITIES_OPCODE,self.set_openloop )
        self.add_method(SET_POSITION_OPCODE           ,self.set_position )
        self.add_method(GET_POSITION_OPCODE           ,self.get_position )
        self.add_method(START_TURNONTHESPOT_OPCODE    ,self.start_turnonthespot)
        self.add_method(RESET_PUREPURSUIT_OPCODE      ,self.reset_purepursuit)
        self.add_method(ADD_PUREPURSUIT_WAYPOINT_OPCODE,self.add_purepursuit_point)
        self.add_method(START_PUREPURSUIT_OPCODE      ,self.start_purepursuit)
        self.add_method(POSITION_REACHED_OPCODE,self.isarrived)
        #self.velocity = velocity
        #self.position = coords
        #Purpursuit variable
        self.follow_path = False
        self.follow_spot = False
        self.final_angle = 0
        self.direction = 1
        self.numWaypoints = 0
        self.goalIndex = 0
        self.goalParam = 0
        self.goalReached = False
        self.angle = 0.0

        self.max = 0
        self.Waypoints = tuple()

        self.spin_accumulator = 0
        self.spin_error = False

        self.robot = parent
        self.parameter = dict()
        for i in range(227):
            self.parameter[i] = 1
    
        self.parameter[POSITIONCONTROL_LINVELMAX_ID] = 500
        self.parameter[POSITIONCONTROL_ANGVELMAX_ID]=4
        self.parameter[POSITIONCONTROL_ANGVELKP_ID]  = 1
        self.parameter[PUREPURSUIT_LOOKAHEAD_ID]  = 200
        self.parameter[PUREPURSUIT_LOOKAHEADBIS_ID] = 200
        self.parameter[VELOCITYCONTROL_MAXLINACC_ID ] = 500
        self.parameter[VELOCITYCONTROL_MAXLINDEC_ID]    = 1000
        self.parameter[VELOCITYCONTROL_MAXANGACC_ID]    = 5 
        self.parameter[VELOCITYCONTROL_MAXANGDEC_ID]    = 10

    ###### METHODE SIMULATEUR #####
    def compute(self):
        if self.robot.parent.core.isCollided(self.robot.name):
            self.spin_accumulator +=1
            if self.spin_accumulator>50 and (self.follow_path or self.follow_spot):
                self.follow_path = False
                self.follow_spot = False
                self.spin_accumulator = 0
                self.spin_error = True
        else:
           self.spin_accumulator = 0 


        if self.follow_spot:
            theta = self.robot.getTheta()
            delta = self.angle-theta
            delta = delta%(2*pi)
            if delta>pi : delta = delta-(2*pi)
            self.robot.setVelocity(0,(delta)*1)
            if((delta)**2<0.01): self.goalReached = True
            return

        if not self.follow_path:
            return



        (x,y) = self.robot.getPosition()
        
        theta = self.robot.getTheta()

        theta = fmod(theta,2*pi)

        if abs(theta)>pi:
            theta=theta - 2*pi

        if not self._checkLookAheadGoal(x, y):
            self._checkProjectionGoal(x, y)

        i = self.goalIndex
        t = self.goalParam
        goal = [0,0]
        if i < self.numWaypoints - 1:
            goal[0] = (1-t) * self.Waypoints[i][0] + t * self.Waypoints[i+1][0]
            goal[1] = (1-t) * self.Waypoints[i][1] + t * self.Waypoints[i+1][1]
        else:
        	goal[0] = self.Waypoints[i][0] + t * cos(self.final_angle)
        	goal[1] = self.Waypoints[i][1] + t * sin(self.final_angle)
        #print("droite en question : (" + str(self.Waypoints[i][0]) + " " + str(self.Waypoints[i][1]) + " ) , ( " +  str(self.Waypoints[i+1][0]) + " " + str(self.Waypoints[i+1][1]) + " ) ")

        chord = hypot(goal[0] - x,goal[1] - y)
        
        delta = atan2(goal[1] - y, goal[0] - x) - theta + pi / 2 * (1 - self.direction)  
        #print("delta " +str(delta))
        newLinVelMax =  self.parameter[POSITIONCONTROL_LINVELMAX_ID] 
        newAngVelMax =  self.parameter[POSITIONCONTROL_ANGVELMAX_ID]
        #print("new Lin VelMax" + str(newLinVelMax))
        if (newAngVelMax * chord) >= (newLinVelMax * abs(2 * sin(delta))):
            newAngVelMax = newLinVelMax * abs(2 * sin(delta)) / chord
        else:
            newLinVelMax = newAngVelMax * chord / abs(2 * sin(delta))


        #print("angVelMax : " + str(newAngVelMax))
        linPosSetpoint = (chord + self._getDistAfterGoal()) * self.direction
        #print("distaftergoal"+ str(self._getDistAfterGoal()) )
        #print(self.direction)
        
        linVelSetpoint = self.parameter[POSITIONCONTROL_LINVELKP_ID ] * linPosSetpoint
        if abs(linVelSetpoint)>newLinVelMax:
            linVelSetpoint = copysign(newLinVelMax,linVelSetpoint)
        if self.max<linVelSetpoint:
            self.max = linVelSetpoint
            print(self.max)


        angPosSetpoint = fmod(delta,2*pi)

        if abs(angPosSetpoint)>pi:
            angPosSetpoint=angPosSetpoint - 2*pi*copysign(1,angPosSetpoint)
        #print("angle delta : " + str(angPosSetpoint) )
        angVelSetpoint = self.parameter[POSITIONCONTROL_ANGVELKP_ID ]* angPosSetpoint
        if abs(angVelSetpoint)>newAngVelMax:
            angVelSetpoint = newAngVelMax*copysign(1,angVelSetpoint)

        if cos(delta) > 0:
            linVelSetpoint = linVelSetpoint*(1 + cos(2 * delta)) / 2
        else:
            #print("aie")
            linVelSetpoint = 0
        #print("lin Vel" + str(linVelSetpoint))
        #print("Ang speed : " + str(angVelSetpoint))        
        #print("Position (" +str(x)+" " + str(y)+")")
        #print("Vise : "+ str(goal))

        self.goalReached = abs(chord + self._getDistAfterGoal()) < self.parameter[POSITIONCONTROL_LINPOSTHRESHOLD_ID]
        #print(angVelSetpoint)
        self.robot.setVelocity(linVelSetpoint,angVelSetpoint)

	

    def _getDistAfterGoal(self):
        dist = 0
        for i in range(self.numWaypoints)[self.goalIndex:]:
            if i < ( self.numWaypoints - 1):
                edgedx = self.Waypoints[i+1][0] - self.Waypoints[i][0]
                edgedy = self.Waypoints[i+1][1] - self.Waypoints[i][1]
		
            else:
                edgedx = cos(self.final_angle)
                edgedy = sin(self.final_angle)

            edgeLength =hypot(edgedx,edgedy)

            if i == self.goalIndex:
                dist += (1 - self.goalParam) * edgeLength
            else:
                dist += edgeLength

        return dist


    def _checkProjectionGoal(self,x,y):
        hmin = 10000000.0
        for i in range(self.numWaypoints)[self.goalIndex:-1]:

            dx = x - self.Waypoints[i][0]
            dy = y - self.Waypoints[i][1]
            edgedx = self.Waypoints[i+1][0] - self.Waypoints[i][0]
            edgedy = self.Waypoints[i+1][1] - self.Waypoints[i][1]
            edgeLength = hypot(edgedx,edgedy)
            t = (edgedx * dx + edgedy * dy) / (edgeLength**2)
            if t > 1 and i+1 <self.numWaypoints-1:
                continue
		
            if t > 1:
                dx2 = x - self.Waypoints[i+1][0]
                dy2 = y - self.Waypoints[i+1][1]
                h = hypot(dx2,dy2)
                t = 1
            elif t <= 0:
                h = hypot(dx,dy)
                t = 0
            else:
                h = abs(edgedx * dy - edgedy * dx) / edgeLength

            if h < hmin:
		
                hmin = h
                if i > self.goalIndex or t > self.goalParam:
                    self.goalIndex = i
                    self.goalParam = t



    def _checkLookAheadGoal(self,x,y):
        for i in range(self.numWaypoints)[self.goalIndex:]:
            dx = x - self.Waypoints[i][0]
            dy = y - self.Waypoints[i][1]

            edgeLength = 1
            if(i != self.numWaypoints-1):
                edgedx = self.Waypoints[i+1][0] - self.Waypoints[i][0]
                edgedy = self.Waypoints[i+1][1] - self.Waypoints[i][1]
                edgeLength = hypot(edgedx, edgedy)
            else:
                edgedx = cos(self.final_angle)
                edgedy = sin(self.final_angle)	
            
            
            h = abs(edgedx * dy - edgedy * dx) / edgeLength
            if self.parameter[PUREPURSUIT_LOOKAHEAD_ID] < h:
                continue

            t = (edgedx * dx + edgedy * dy) / (edgeLength * edgeLength)
            t1 = t - sqrt(self.parameter[PUREPURSUIT_LOOKAHEAD_ID]**2 - h**2) / edgeLength
            t2 = t + sqrt(self.parameter[PUREPURSUIT_LOOKAHEAD_ID]**2 - h**2) / edgeLength

            if (t2 < 0):
                continue
            elif t2 > 1 and i < self.numWaypoints-1 :
                continue
            elif t1 > 1 and i == self.numWaypoints-1:
                continue
            elif t2 > (1 + self.parameter[PUREPURSUIT_LOOKAHEADBIS_ID] / edgeLength):
                t2 = 1 + self.parameter[PUREPURSUIT_LOOKAHEADBIS_ID] / edgeLength

            if i > self.goalIndex:


                self.goalIndex = i
                self.goalParam = t2      
            if t2 > self.goalParam:
                self.goalIndex = i
                self.goalParam = t2 

    
            return True

        return False

    def stop(self):
        
        
        self.reset_purepursuit()
        self.spin_error= True
        self.velocity.x = 0
        self.velocity.theta = 0

    ###### METHODE ARDUINO #######
    def reset_purepursuit(self):
        self.follow_path = False
        self.follow_spot = False
        self.spin_error= False
        self.Waypoints = tuple()
        self.final_angle = 0
        self.direction = 1
        self.numWaypoints = 0
        self.goalIndex = 0
        self.goalParam = 0
        self.goalReached = False



    def add_purepursuit_point(self,x,y):
        x , y = Deserializer(x+y).read(FLOAT,FLOAT)

        self.Waypoints += ( (x,y) ,)


    def start_purepursuit(self,direc,angle):
        self.spin_error= False
        self.follow_spot = False
        direc , angle = Deserializer(direc+angle).read(BYTE,FLOAT)
        if direc==0:
            self.direction=1
        else:
            self.direction=-1
        
        self.final_angle = angle
        self.numWaypoints = len(self.Waypoints)
        if self.numWaypoints>1:
            self.follow_path =True
        self.goalIndex = 0
        self.goalParam = 0
        self.goalReached = False
        
    def start_turnonthespot(self,angle):
        self.follow_spot = True
        self.spin_error= False
        self.follow_path =False
        self.angle = Deserializer(angle).read(FLOAT)
        self.robot.setVelocity(0,0)
        self.goalReached = False

    def isarrived(self,*args,**kwargs):
        return Deserializer(BYTE(self.goalReached)+BYTE(self.spin_error))   

    def set_openloop(self,right,left):
        self.follow_path = False
        self.spin_error= False
        right,left = Deserializer(right+left).read(FLOAT,FLOAT)
        lin_Vel = (right + left) /2
        ang_Vel = (right - left)/self.parameter[ODOMETRY_AXLETRACK_ID]
        self.robot.setVelocity(lin_Vel,ang_Vel)
        #self.velocity.x = lin_Vel
        #self.velocity.theta =ang_Vel


    def get_position(self):
        (x,y) = self.robot.getPosition()
        theta = self.robot.getTheta()
        return Deserializer(FLOAT(x) + FLOAT(y) + FLOAT(theta) )

    def set_position(self,x,y,theta):
        self.follow_spot = False
        x,y,theta = Deserializer(x +y+theta).read(FLOAT,FLOAT,FLOAT)
        self.robot.setPosition(x,y,theta)
        #self.position.x = x
        #self.position.y = y
        #self.position.theta = theta


    def get_velocity(self):
        (lin,ang) = self.robot.getVelocity()
        return Deserializer(FLOAT(lin)+ FLOAT(ang))


    def set_velocity(self,lin_Vel,ang_Vel):
        self.follow_path = False
        self.spin_error= False
        self.follow_spot = False
        out = Deserializer(lin_Vel +ang_Vel)
        self.robot.setVelocity(*out.read(FLOAT,FLOAT))

        


    def get_parameter(self,id):
        id = Deserializer(id).read(BYTE)
        if not id in (LEFTCODEWHEEL_COUNTSPERREV_ID,RIGHTCODEWHEEL_COUNTSPERREV_ID,VELOCITYCONTROL_SPINSHUTDOWN_ID):
            return Deserializer(FLOAT(self.parameter[id]) ) 
        elif id == VELOCITYCONTROL_SPINSHUTDOWN_ID:
            return Deserializer(BYTE(self.parameter[id]) )
        else:
            return Deserializer(LONG(self.parameter[id]) )

    def set_parameter(self,id,value):
        id = Deserializer(id).read(BYTE)
        out = Deserializer(value)
        if not id in (LEFTCODEWHEEL_COUNTSPERREV_ID,RIGHTCODEWHEEL_COUNTSPERREV_ID,VELOCITYCONTROL_SPINSHUTDOWN_ID):
            self.parameter[id] = out.read(FLOAT)
        elif id == VELOCITYCONTROL_SPINSHUTDOWN_ID:
            self.parameter[id] = out.read(BYTE)
        else:
            self.parameter[id] = out.read(LONG)