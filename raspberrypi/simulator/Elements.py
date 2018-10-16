from simulator.Physics_tools import *
from simulator.Server_utils.Wheeledbase_virtual import *
from simulator.Server_utils.Buildingcollector_virtual import *
from simulator.Server_utils.Waterdispenser_virtual import *
import simulator.Module.Physics as Physics
import math


class Prob(printable):
    nb_digits = 4
    def __init__(self, parent, pos_actions):
        printable.__init__(self, parent.canvas, 0, 0, 0)
        self.probas = []
        self.actions = []
        self.Canvas = parent.canvas
        self.enable = [True]*len(pos_actions)
        for pos in pos_actions:
            x = pos[0]-100.0
            if(x < 0):
                x += 200
            y = pos[1]-100.0
            if(y < 0):
                y += 200
            x_simu = self.Canvas.width - y*self.Canvas.width/3000.0
            y_simu = self.Canvas.height - x*self.Canvas.height/2000.0
            
            self.actions += [self.Canvas.create_text(x_simu, y_simu, text="0")]
            self.probas += [0]


    def refresh(self):
        for i in range(len(self.actions)):
            if self.enable[i] == False:
                self.Canvas.itemconfig(self.actions[i], text="X")
            else:
                self.Canvas.itemconfig(self.actions[i], text=str(round(int(self.probas[i] * 100), 0)))

    def set_proba(self, id_cube, proba):
        self.probas[id_cube] = proba

    def disable(self, i):
        self.enable[i] = False


#Mettre les balls dans la soute
#Faire drop (pas animation)

class Ball(printable):
    radius = 20
    nb_element = 0
    physics_core = Physics.PhysicsMotor()
    physics_core.setTimestep(10)
    physics_core.start()

    def __init__(self,parent,color="green"):
        printable.__init__(self,parent.canvas,0,0,0)
        self.x = -1000.
        self.y = -1000.
        self.theta = 0.
        self.color = color
        self.parent = parent
        self.points = 3
        self.scored = None
        self.name = "Ball_{}".format(Ball.nb_element)
        self.parent.add(self)
        Ball.nb_element+=1

        Ball.physics_core.addObject(self.name)
        Ball.physics_core.setPosition(self.name,self.x,self.y)
        Ball.physics_core.disableObject(self.name)
        self.shape = self.parent.canvas.create_arc(self.x+1.1414*Ball.radius,self.y+1.1414*Ball.radius, self.x-1.1414*Ball.radius,self.y-1.1414*Ball.radius,fill=self.color, start=0,extent=359.999)

    def reset(self):
        Ball.physics_core.disableObject(self.name)
        Ball.physics_core.setPosition(self.name,-1000,-1000)
        self.scored = None

    def compute(self):
        self.x , self.y = Ball.physics_core.getPosition(self.name)
        #Verifions si il est présent à un bonne endroit !!
        if(self.x<0 and math.fabs(self.y-2820)<180):
            Ball.physics_core.setVelocity(self.name,0,0,True)

            self.scored = "Orange"

        if(self.x<0 and (math.fabs(self.y-180)<180)):
            Ball.physics_core.setVelocity(self.name,0,0,True)

            self.scored = "Green"

            


    def refresh(self):
        self.parent.canvas.coords(self.shape,*self.parent.canvas.convert_polygon(((self.x+1.1414*Ball.radius,self.y+1.1414*Ball.radius),( self.x-1.1414*Ball.radius,self.y-1.1414*Ball.radius))  )  )

    def __str__(self):
        return "{} ball".format(self.color)


class Dispenser():
    def __init__(self,parent,x,y,radius,color,capacity= 8,min_ang=-0.785398,max_ang=0.785398):
        self.x = x
        self.y = y
        self.min_ang = min_ang
        self.max_ang = max_ang
        self.parent  = parent
        self.parent.add_dispenser(self)
        self.radius = radius
        self.container = list()
        self.linked_ball = list()
        if(type(color)==list):
            color_list= list(color)*(capacity//len(color))
        else:
            color_list= [color]*capacity
        for col in color_list:
            self.linked_ball.append(Ball(self.parent,col))
        for ball in self.linked_ball:
            self.container.append(ball)

    def reset(self):
        self.container = list()
        for ball in self.linked_ball:
            ball.reset()
            self.container.append(ball)

    def getball(self,x,y,theta):
        if(math.hypot(y-self.y,x-self.x)<self.radius):
            
            if(len(self.container)>0):# and theta>self.min_ang and theta<self.max_ang):
                return self.container.pop()
        return None


class Cube(Object):
    cube_number = 0
    def __init__(self,parent,color="blue",x=1000,y = 1000,theta = 0):
        self.name = "Cube_{}".format(Cube.cube_number)
        Cube.cube_number+=1
        Object.__init__(self,self.name,parent,x,y,theta)
        self.polygone  =((-28,-28),(-28,0),(-28,28),(0,28),(28,28),(28,0),(28,-28),(0,-28))
        self.parent.core.setShape(self.name,self.polygone)
        self.parent.core.setWeight(self.name,100)
        self.initial_coordinates = (x,y)
        self.shape = self.canvas.create_polygon(self.polygone)
        self.canvas.itemconfig(self.shape,fill =color)
        self.scored = None
        self.points = 5
        

    def reset(self):
        self.parent.core.setPosition(self.name,*self.initial_coordinates)
        self.parent.core.enableObject(self.name)
        self.scored = None

    def forward(self):
         self.parent.core.setVelocity(self.name,100,0,False)

    def refresh(self):
        self.canvas.coords(self.shape,*(  self.canvas.convert_polygon(self.parent.core.getShape(self.name)) )  )


class Robot(Object):
    def __init__(self,name,parent,color,x=1000,y=1000,theta = 0,container_size = 10,container_size_ball=8):
        Object.__init__(self,name,parent,x,y,theta)
        self.polygone = ( (-57.5,-93.5),(-57.5,93.5),(-38.5,112.5),(38.5,112.5) ,(57.5,93.5),(57.5,-93.5),(38.5,-112.5),(-38.5,-112.5) )
        #self.polygone  =(  (-93.5,57.5) , (93.5,57.5),(112.5,38.5) ,(112.5,-38.5),(93.5,-57.5),(-93.5,-57.5),(-112.5,-38.5),(-112.5,38.5))
        #self.polygone = ( (-100,40),(100,40),(100,-40),(-100,-40)  )
        print("Je suis un nouveau robot {}".format(color))
        self.parent.core.setShape(self.name,self.polygone)
        self.shape = self.canvas.create_polygon(self.polygone)
        self.color = color
        self.parent.core.setWeight(self.name,2000)
        self.arduinos = [Wheeledbase(self),BuildingCollector(self),WaterDispenser(self)]
        self.parent.core.setMaxAcceleration(self.name,700,2)
        self.parent.core.setMinAcceleration(self.name,700,2)
        self.container = list()
        self.container_size  =container_size
        self.container_size_ball = container_size_ball
        self.container_ball = list()
        self.container_ball_bis = list()
        self.x = 0
        self.y = 0
        self.theta = 0
        self.polygone = tuple()

    def clear(self):
        print("suppression")
        #self.setPosition(-1000,-1000,0)
        self.canvas.delete(self.shape)
        time.sleep(0.1)
        self.canvas.remove_components(self)
        self.parent.remove(self)
    
    def pick_cube(self):
        #x , y = self.x , self.y
        #theta = self.theta
        #x+=65*math.cos(theta)
        #y+=65*math.sin(theta)
        for x,y in self.polygone:
            cube_taken = self.parent.core.getObject(x,y,50)
            if not cube_taken in ["None",self.name]:
                self.parent.core.setPosition(cube_taken,-100,-100)
                self.container.append(cube_taken)
                break
        
    def pick_ball(self):
        ball = self.parent.get_ball(self.x,self.y,self.theta)
        if(not ball is None and ball.color  == self.color):
            if  len(self.container_ball)<self.container_size_ball:
                self.container_ball.append(ball)
                #Mettre le robot en dessou
                self.parent.canvas.tag_lower(self.shape,ball.shape)
                return
        else:
            if not ball is None and (len(self.container_ball_bis)<self.container_size_ball):
                self.container_ball_bis.append(ball)
                #Mettre le robot en dessou
                self.parent.canvas.tag_lower(self.shape,ball.shape)
                return
        
    def fire_ball(self):
        if len(self.container_ball)>0:
            ball = self.container_ball.pop()
            Ball.physics_core.setPosition(ball.name,self.x,self.y)
            Ball.physics_core.setTheta(ball.name,self.theta)
            Ball.physics_core.setVelocity(ball.name,1000,0,True)
            Ball.physics_core.setMinAcceleration(ball.name,250,0)
            Ball.physics_core.setVelocity(ball.name,0,0,False)

    def drop_ball(self):
        if len(self.container_ball_bis)>0:
            ball = self.container_ball_bis.pop()
            Ball.physics_core.setPosition(ball.name,self.x +100*math.cos(self.theta),self.y+100*math.sin(self.theta))

    def get_ball(self):
        if len(self.container_ball)>0: return True
        return False

    def get_ball_dirty(self):
        if len(self.container_ball_bis)>0: return True
        return False

        

    def compute(self):
        self.x , self.y = self.parent.core.getPosition(self.name)
        self.theta = self.parent.core.getTheta(self.name)
        self.polygone = self.parent.core.getShape(self.name)
        if(hasattr(self, 'arduinos')):
            for arduino in self.arduinos:
                arduino.compute()
        self.pick_ball()
        if(hasattr(self,"container_ball")):
            i = -105
            j = 20
            for ball in self.container_ball:
                self.parent.canvas.tag_lower(self.shape,ball.shape)
                Ball.physics_core.setPosition(ball.name, self.x -i*math.sin(self.theta),self.y +i*math.cos(self.theta))
                i+=25
            for ball in self.container_ball_bis:
                self.parent.canvas.tag_lower(self.shape,ball.shape)
                Ball.physics_core.setPosition(ball.name, self.x -i*math.sin(self.theta),self.y +i*math.cos(self.theta))
                i+=25


    def refresh(self):
        self.canvas.coords(self.shape,*(  self.canvas.convert_polygon(self.parent.core.getShape(self.name)) )  )
