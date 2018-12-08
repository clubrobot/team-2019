#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import os, sys
from math import *
from random import *
import tkinter
from PIL import Image
from threading import Thread, RLock, Event, current_thread
import time


BLUE = '#3498DB'
RED  = 'red'
WHITE = '#FFF'
BLACK = '#000'


def get_resolution(width):


    width.set(0)
    window1 = tkinter.Tk()
    window1.title("Resolution")
    window1.geometry("400x400")
    MODES = [
            ("400x267" , 400 ),
            ("600x400" , 600 ),
            ("800x533" , 800 ),
            ("1000x667", 1000),
            ("1200x800", 1200)
        ]

    for text, mode in MODES:
        tkinter.Radiobutton(window1, text=text,variable=width, value=mode).pack(anchor=tkinter.W)
    
    button = tkinter.Button(window1, text="Validée", command= window1.destroy)
    button.pack()

    window1.mainloop()

    return(width.get())


            
class Canvas_area(tkinter.Canvas,Thread):
    def __init__(self,parent,color= '#333',width=999,height=664):
        tkinter.Canvas.__init__(self,parent, width= width,height=height,bg=color)
        self.components = []
        self.height= height
        self.width = width
        self.picture_link = ''
        self.pack()


    def show(self):
        self.pack(fill = tkinter.NONE, expand = tkinter.YES)

    def add_components(self,component):
        if(isinstance(component, printable)):
            self.components.append(component)
        else:
            raise TypeError("Canvas requiere a printable object")

    def remove_components(self,component):
        if(isinstance(component, printable)):
            self.components.remove(component)
        else:
            raise TypeError("Canvas requiere a printable object")

    def update_resolution(self):
        width = tkinter.IntVar()
        width.set(600)
        self.widt = get_resolution(width)
        print(width.get())
        self.config(width = self.width,height= self.width*(2/3))
        self.add_background(self.picture_link)

    def add_background(self,picture):
        self.picture_link = picture
        img = Image.open(picture, 'r')
        img = img.resize((self.width,self.height))
        img.save('background_redim.gif')
        img.close()
        self.photo = tkinter.PhotoImage(file="background_redim.gif")
        self.create_image((self.width//2,self.height//2) , image=self.photo) 


    def refresh(self):
        for component in self.components:
            try :
                component.refresh()
            except:
                pass

    def convert_polygon(self,tupl):
        result = []
        for (x,y) in tupl:
            result.append(self.width-y*self.width/3000)
            result.append(  self.height-x*self.height/2000 )  
        return tuple(result)


class printable():
    def __init__(self,parent,x,y,theta):
        self.canvas = parent
        self.canvas.add_components(self)


    def refresh(self):
        raise NotImplementedError
        self.canvas.set_parameter(self.shape,x,y)







