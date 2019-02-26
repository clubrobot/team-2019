#!/usr/bin/env python3
#-*- coding: utf-8 -*-

#from robot_setup import *
from robots.setup_wheeledbase import *
import time
import math

ccw=True

axletrack = wheeledbase.get_parameter_value(ODOMETRY_AXLETRACK_ID, FLOAT)
radius_l = wheeledbase.get_parameter_value(LEFTCODEWHEEL_RADIUS_ID, FLOAT)
radius_r = wheeledbase.get_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, FLOAT)


print('-> axletrack = {}'.format(axletrack))
print('-> radius_l = {}'.format(radius_l))
print('-> radius_r = {}'.format(radius_r))

#Bornibus
#axletrack = 209.6046142578125
#radius_l = -22.769546508789062
#radius_r = +22.769325256347656
leftcodewheel_countperrev  = 10000
rightcodewheel_countperrev = 10000

#Murray
#axletrack = 123#123.94139971715916
#radius_l = -25.65499416863814
#radius_r = 25.616884952933084

#127
#axletrack = 355.8540086249092
#radius_l = 21.844856068091886
#radius_r = 21.89002869767
#leftcodewheel_countperrev = 8000
#rightcodewheel_countperrev = 8000


print('-> axletrack = {}'.format(axletrack))
print('-> radius_l = {}'.format(radius_l))
print('-> radius_r = {}'.format(radius_r))

wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
wheeledbase.set_parameter_value(LEFTCODEWHEEL_RADIUS_ID, radius_l, FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, radius_r, FLOAT)

input('Veuillez aligner le robot:')

N = int(input('Combien de tours ?\n'))
if not ccw: N *= -1

theta0 = wheeledbase.get_position()[2]
lticks0, rticks0 = wheeledbase.get_codewheels_counter()

angvel = 2#wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)
if not ccw: angvel = -angvel
wheeledbase.set_velocities(0, angvel)
while abs(wheeledbase.get_position()[2] - theta0) / (2 * math.pi) < abs(N):
	wheeledbase.set_velocities(0, angvel)
	progress = (wheeledbase.get_position()[2] - theta0) / (2 * math.pi) / N
	print('{:.0f}/{} ({:.0f}%)\ttemps restant: {:2.0f}'.format(N * progress, N, 100 * progress, N * 2 * math.pi / angvel * (1 - progress)), end='\r')
	time.sleep(0.1)
wheeledbase.stop()

input('Veuillez réaligner le robot sur son orientation initiale:')

lticks1, rticks1 = wheeledbase.get_codewheels_counter()
print('lticks = {}\trticks = {}'.format(lticks1 - lticks0, rticks1 - rticks0))

alpha_l = (lticks1 - lticks0) / (leftcodewheel_countperrev * N)
alpha_r = (rticks1 - rticks0) / (rightcodewheel_countperrev * N)
print('alpha_l = {}\t alpha_r = {}'.format(alpha_l, alpha_r))

axletrack = alpha_r * radius_r - alpha_l * radius_l
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
print('-> axletrack = {}'.format(axletrack))

input('Veuillez à nouveau aligner le robot:')

M = int(input('Combien de carrés ?\n'))
if not ccw: M *= -1

lticks0, rticks0 = wheeledbase.get_codewheels_counter()
wheeledbase.reset()
for k in range(abs(M)):
	if ccw: wheeledbase.goto(500,   0); wheeledbase.turnonthespot( math.pi/2); wheeledbase.wait()
	else:   wheeledbase.goto(500,   0); wheeledbase.turnonthespot(-math.pi/2); wheeledbase.wait()
	print('{:.0f}/{} ({:.0f}%)'.format(k, M, 100 * (k + 0.25) / M), end='\r')
	if ccw: wheeledbase.goto(500, 500); wheeledbase.turnonthespot(   math.pi); wheeledbase.wait()
	else:   wheeledbase.goto(500,-500); wheeledbase.turnonthespot(   math.pi); wheeledbase.wait()
	print('{:.0f}/{} ({:.0f}%)'.format(k, M, 100 * (k + 0.50) / M), end='\r')
	if ccw: wheeledbase.goto(  0, 500); wheeledbase.turnonthespot(-math.pi/2); wheeledbase.wait()
	else:   wheeledbase.goto(  0,-500); wheeledbase.turnonthespot( math.pi/2); wheeledbase.wait()
	print('{:.0f}/{} ({:.0f}%)'.format(k, M, 100 * (k + 0.75) / M), end='\r')
	if ccw: wheeledbase.goto(  0,   0); wheeledbase.turnonthespot(         0); wheeledbase.wait()
	else:   wheeledbase.goto(  0,   0); wheeledbase.turnonthespot(         0); wheeledbase.wait()
	print('{:.0f}/{} ({:.0f}%)'.format(k + 1, M, 100 * (k + 1.00) / M), end='\r')
wheeledbase.stop()

input('Veuillez réaligner le robot sur son orientation initiale:')

lticks1, rticks1 = wheeledbase.get_codewheels_counter()
print('lticks = {}\trticks = {}'.format(lticks1 - lticks0, rticks1 - rticks0))

beta = ((lticks1 - lticks0) - (alpha_l * leftcodewheel_countperrev * M)) / ((rticks1 - rticks0) - (alpha_r * leftcodewheel_countperrev * M))
print('beta = {}'.format(beta))

radius_r = beta * radius_l
axletrack = alpha_r * radius_r - alpha_l * radius_l
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, abs(radius_r), FLOAT)
print('-> axletrack = {}'.format(axletrack))
print('-> radius_r = {}'.format(radius_r))

input('Veuillez à nouveau aligner le robot:')

lticks0, rticks0 = wheeledbase.get_codewheels_counter()

D = int(input('Quelle distance (en mm) ?\n'))

wheeledbase.reset()
wheeledbase.goto(D, 0)
wheeledbase.stop()

input('Veuillez réaligner le robot sur son orientation initiale:')

lticks1, rticks1 = wheeledbase.get_codewheels_counter()
print('lticks = {}\trticks = {}'.format(lticks1 - lticks0, rticks1 - rticks0))

radius_l = (D * (leftcodewheel_countperrev + rightcodewheel_countperrev) / 2) / (math.pi * (beta * (rticks1 - rticks0) + (lticks1 - lticks0)))
radius_r = beta * radius_l
axletrack = alpha_r * radius_r - alpha_l * radius_l
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
wheeledbase.set_parameter_value(LEFTCODEWHEEL_RADIUS_ID, abs(radius_l), FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, abs(radius_r), FLOAT)
print('-> axletrack = {}'.format(axletrack))
print('-> radius_l = {}'.format(radius_l))
print('-> radius_r = {}'.format(radius_r))


