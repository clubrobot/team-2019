#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os, sys
from math import *
from tkinter import *
from tkinter.ttk import *
from tkinter.messagebox import *
from simulator.Elements import *
from simulator.Server_utils.Components_virtual import *
from common.tcptalks import *
from simulator.Server_tools import *
from common.geogebra import Geogebra
import random

BLUE = '#3498DB'
RED  = 'red'
WHITE = '#FFF'


def key(event):
    global rob
    global c
    global window
    global simu
    
    
    if(event.char =='z'):
        rob.setVelocity(1500,0)
        #c.forward()
    elif(event.char=='s'):
        rob.setVelocity(-500,0)
    elif(event.char=='q'):
        rob.setVelocity(0,1)
    elif(event.char=='d'):
        rob.setVelocity(0,-1)
    elif(event.char ==' '):
        rob.setVelocity(0,0)
    elif(event.char =='i'):
        Thread(target=rob.arduinos[1].grab_left).start()
    elif(event.char == 'o'):
        Thread(target=rob.arduinos[1].grab_center).start()
    elif(event.char == 'p'):
        Thread(target=rob.arduinos[1].grab_right).start()
    elif(event.char == 'l'):
        Thread(target=rob.arduinos[1].drop_tower).start()
    elif(event.char == 'f'):
        rob.fire_ball()




    

#Paramètre de la fenêtre
window = Tk()
window.title("Simulateur d'IA - Club Robot - INSA Rennes")
window.tk.call('wm', 'iconphoto', window._w, PhotoImage(file = os.path.join(os.path.dirname(__file__), 'logo.png')))
window.configure(width=1080,height=1720)
window.resizable(width=False,height=False)



style = Style()
style.theme_use("clam")

Canvas = Canvas_area(window,width=800 ,height=533)
Canvas.show()


simu = Simulator(window,Canvas)
servers = ServerManager(simu, None)
simu.core.setMap(( (0,0),(0,3000),(2000,3000),(2000,2106), (1740,2106) ,(1740,894) ,(2000,894),(2000,0) ))
simu.launch()



Canvas.add_background("background.png")
menuBar = Menu(window)
window['menu'] = menuBar

FichierMenu = Menu(menuBar)
menuBar.add_cascade(label='Fichier')#, menu=FichierMenu)
FichierMenu.add_command(label='Nouveau')
FichierMenu.add_command(label='Ouvrir...')

ServerMenu = Menu(menuBar)
menuBar.add_cascade(label='Serveur')#, menu=ServerMenu)


OutilMenu = Menu(menuBar)
menuBar.add_cascade(label='Outils', menu=OutilMenu)
OutilMenu.add_command(label='Résolution',command=Canvas.update_resolution)
OutilMenu.add_command(label='Préférence connexion')
OutilMenu.add_command(label='Préférence IA')
OutilMenu.add_command(label='Manette')

def coucou():
    global simu
    time.sleep(100)
    score  = simu.getPoints()
    if(score[0]>score[1]):
        message = "Orange Gagne avec {} contre {}".format(*score)
    if(score[0]<score[1]):
        message = "Vert Gagne avec {} contre {}".format(score[1],score[0])
    if(score[0]==score[1]):
        message = "match nul avec {} partout".format(score[0])
    showinfo("RESULTAT", message)




def points():
    Thread(target=coucou).start()





ServerMenu.add_command(label='Lancer')#,command = Server.start)
ServerMenu.add_command(label='Arrêter')#,command = Server.stop)
ServerMenu.add_command(label='Paramètre')#,command = Server.setup)


launch_button = Button(window, text="Start",takefocus=False,command=points)
launch_button.pack(side=LEFT ,padx = 10, pady = 1)

reset = Button(window, text="reset",takefocus=False,command=servers.kick_all)
reset.pack(side=RIGHT ,padx = 10, pady = 1)

speed_scale = tkinter.Scale(window, orient='horizontal', from_=0, to=2,resolution=0.01, tickinterval=1, length=100,label='Speed')
speed_scale.pack(side=BOTTOM, padx = 2, pady = 2)
speed_scale.set(1)



#t=  ( (-57.5,-93.5),(-38.5,-112.5),(38.5,-112.5),(57.5,-93.5),(57.5,93.5),(38.5,112.5),(-38.5,112.5),(-57.5,93.5)) 



#rob = Robot("bornibus",simu)

#charge le geogebra
from common.geogebra import Geogebra
map_source = GeoGebra("map_ressources.ggb")

Cube_list = list()
pos_list = list()
for x,y in map_source.getall("Cube_{J"):
    c = Cube(simu,color="yellow",x=x,y=y)
    Cube_list.append(c)
    pos_list += [c.initial_coordinates]

for x,y in map_source.getall("Cube_{N"):
    c = Cube(simu,color="black",x=x,y=y)
    Cube_list.append(c)
    
for x,y in map_source.getall("Cube_{B"):
    c = Cube(simu,color="blue",x=x,y=y)
    Cube_list.append(c)
for x,y in map_source.getall("Cube_{O"):
    c = Cube(simu,color="orange",x=x,y=y)
    Cube_list.append(c)
for x,y in map_source.getall("Cube_{V"):
    c = Cube(simu,color="green",x=x,y=y)
    Cube_list.append(c)





d1 = Dispenser(simu,840.0,0.0,125,"green")
pos_list += [(840, 0)]
d2 = Dispenser(simu,2000.0,610.0,125,["orange","green"],min_ang=-2.356194000,max_ang=3.92699)
pos_list += [(2000, 610)]
d3 = Dispenser(simu,2000.0,2390.0,125,["green","orange"],min_ang=-2.356194000,max_ang=3.92699)
pos_list += [(2000, 2390)]
d4 = Dispenser(simu,840.0,3000.0,125,"orange")
pos_list += [(840, 3000)]

probs = Prob(simu, pos_list)
servers.probs = probs


def dispenser_reset():
    global d1
    global d2
    global d3
    d1.reset()
    d2.reset()
    d3.reset()
    d4.reset()


def cube_reset():
    global Cube_list
    for cube in Cube_list:
        cube.reset()


def reset_all():
    cube_reset()
    dispenser_reset()

"""

rob = Robot("dem",simu)
rob.setPosition(1000,1000,0)
"""
servers.bind_reset_function(reset_all)


servers.start()
window.mainloop()
servers.clean()
servers.join()

#remove background-redim
os.remove("background_redim.gif")
