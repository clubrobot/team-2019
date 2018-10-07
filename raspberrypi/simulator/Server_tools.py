from simulator.Server_utils.Components_virtual import *
from threading import Thread, Lock, Event, current_thread
from simulator.Physics_tools import *
from simulator.Elements import *
import time

GET_OPCODE =0xFA
SET_PROB_OPCODE =0xFB

class ServerManager(Thread):
    def __init__(self,simulator, probs):
        Thread.__init__(self)
        self.robots  = list()
        self.simulator = simulator
        self.lock = Lock()
        self.server = Server()
        self.stop = Event()
        self.nb_connect = 0
        self.remove_rob = Event()
        self.enabling_function = None
        self.probs = probs

        self.server.bind(GET_OPCODE,self.getInf)
        self.server.bind(SET_PROB_OPCODE, self.set_prob_server)

    def clean(self):
        self.stop.set()

    def bind_reset_function(self,function):
        try:
            self.enabling_function = function
        except TypeError:
            pass

    def set_prob_server(self,client, action_id, prob):
        if prob == "X":
            self.probs.disable(action_id)
        else:
            self.probs.set_proba(action_id, float(prob))

    def getInf(self,client,Type,ID):
        print("ID : " + str(ID))
        print("Type : "+ str(Type))
        #ROBOT POSITION
        if(Type==0):
            try:
                return self.robots[ID].getPosition()
            except IndexError:
                return (-1,-1)
        #CROSS STATE
        if(Type==1):
            # We need to swap due to twist order in main
            #  2  -> 0
            #  0  ->  3
            #  1   -> 2
            #  3    ->1
            #  4    -> 5
            #  5    -> 4

            liste = [2,3,1,0,5,4]
            ID = liste[ID]
            try:
                return self.simulator.getCrossState(ID)
            except IndexError:
                return False
        #Dispenser STATE
        if(Type==2):

            try:
                return self.simulator.getDispensetState(ID)
            except IndexError:
                return False
        #Robot capacity ball
        if(Type==3):
            try:
                if(len(self.robots[ID].container_ball)>0):
                    return 1
                else:
                    return 0       
            except IndexError:
                return 0
        #Robot capacity ball dirty     
        if(Type==4):
            try:
                if(len(self.robots[ID].container_ball_bis)>0):
                    return 1
                else:
                    return 0       
            except IndexError:
                return 0
        #Cube
        if(Type==5):
            try:
                if(len(self.robots[ID].container)>0):
                    return 1
                else:
                    return 0       
            except IndexError:
                return 0

        if(Type==6):
            try:
                return self.robots[ID].getVelocity()
            except IndexError:
                return (0,0)
        
        if(Type==7):
            try:
                return self.robots[ID].getTheta()
            except IndexError:
                return (0,0)
        if(Type==8):
            try:
                self.robots[ID].setPosition(4000,0,0)
                return
            except IndexError:
                return
        

    def kick_all(self):

        #self.lock.acquire()
        self.server.disconnect()
        self.server.cleanup()
        for robot in self.robots:
            robot.clear()
            del robot
        self.robots = list()
        self.rob_temp.clear()
        
        self.remove_rob.set()
        self.enabling_function()

        #self.lock.release()

    def reset(self):
        pos = [(300,300),(300,2700)]
        self.enabling_function()
        for robot in self.robots:
            print(pos)
            robot.setPosition(*pos[0],0)
            robot.container = list()
            robot.container_bis = list()
            if(len(pos)>1) : del pos[0]
        self.rob_temp.setPosition(-1000,-1000,0)



    def run(self):
        color = "green"
        while not self.stop.is_set():
            #Create  a robot
            self.rob_temp = Robot("rob_{}".format(self.nb_connect),self.simulator,color,x=-1000,y =-1000)
            #Wait to clear the lock
            time.sleep(0.1)
            #Add his arduino to Components server
            for arduino in self.rob_temp.arduinos:
                self.server.addcomponent(arduino)
            #wait for a client
            while not self.stop.is_set():
                try:
   

                    #self.lock.acquire()
                    self.server.connect(timeout=0.5)
                    self.rob_temp.setPosition(1000,1000,0)
                    color = "green" if color=="orange" else "orange"

                except ForeverAloneError:
                    if self.remove_rob.is_set():
                        self.rob_temp = Robot("rob_{}".format(self.nb_connect),self.simulator,color,x=-1000,y =-1000)
                        for arduino in self.rob_temp.arduinos:
                            self.server.addcomponent(arduino)  
                        self.remove_rob.clear()
                    continue
                except AuthentificationError:
                    continue
                else:
                    self.nb_connect+=1
                    self.robots.append(self.rob_temp)
                    self.rob_temp =Robot("rob_{}".format(self.nb_connect),self.simulator,color,x=-1000,y =-1000)
                    time.sleep(1)
                    for arduino in self.rob_temp.arduinos:
                        self.server.addcomponent(arduino)

                    continue
                
    



