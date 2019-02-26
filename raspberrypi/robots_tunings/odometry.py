#!/usr/bin/env python3
#-*- coding: utf-8 -*-

#  L'objectif de ce code est de déterminer avec précision les valeurs des différents élements utiles à l'odométrie
#  en déplaçant le robot. 
#   - On tourne d'abord sur soit mếme pour déterminer l'axetrack
#   - Ensuite on fait des carrés pour déterminer le rayon de la roue droite
#   - Puis une ligne droite, d'une distance connue pour déterminer le rayon de la roue gauche
#
#   Il est important de replacer la robot sur la position qu'il aurait du atteindre sans le faire glisse 
#   avant de passer à l'étape suivante. C'est justement la mesure des valeurs des codeuses qui permet de faire
#   les calculs.
# 
#   Il est également nécessaire d'avoir une approximation des valeurs pour débuter le programme


from robots.setup_wheeledbase import *
import time
import math
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("robot_name")
args = parser.parse_args()

ccw=True
if(args.robot_name.lower() == "bornibus" or args.robot_name.lower() == "bb") :
    print("Chargement des constantes pour Bornibus : ")
    def write_cst() : 
        import tunings_bornibus
elif(args.robot_name.lower() == "128") :
    print("Chargement des constantes pour 128 : ")
    def write_cst() : 
        import tunings_128
#Si besoin d'autres robot ou changement de nom, simplement rajouter les lignes ci-dessous

#elif(args.robot_name.lower() == "nouveau_nom_du_robot_en_minuscule" or args.robot_name.lower() == "surnom1" or ... ) :
#   print("Chargement des constantes pour NOM_DU_ROBOT : ")
#    def write_cst() : 
#       import fichier_python_chargeant_les_constantes

write_cst()
'''
    Les constantes ci-dessous permettent de faire correctement tourner le robot pour que les mesures soient bonnes
    Lors de la première utilisation d'un robot il faut mesurer ces dimensions.
    
    
     axletrack  : l'écartement entre les deux roues codeuses
     radius_l   : rayon de la roue codeuse gauche
     radius_r   : rayon de la roue codeuse droite
     (left/right)codewheel_countperrev : nombre de tick/tour mesure par la roue codeuse (les compteurs sont en quadratures donc il faut multiplier la valeur présente sur la codeuse par 4 ( ou lire la valeur avec l'arduino/esp après et en faisant tourner la codeuse à la main))

    Toutes les mesures sont en mm.

    Attention  ! Dans la base roulante un des nombres de tick par tour doit être négatif pour 

'''
axletrack =  wheeledbase.get_parameter_value(ODOMETRY_AXLETRACK_ID, FLOAT)
radius_l = wheeledbase.get_parameter_value(LEFTCODEWHEEL_RADIUS_ID, FLOAT)
radius_r = wheeledbase.get_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, FLOAT)
leftcodewheel_countperrev = abs(wheeledbase.get_parameter_value(LEFTCODEWHEEL_COUNTSPERREV_ID, LONG))
rightcodewheel_countperrev = abs(wheeledbase.get_parameter_value(RIGHTCODEWHEEL_COUNTSPERREV_ID, LONG))

print('Valeurs utilisees pour la calibration : ')
print('-> axletrack = {}'.format(axletrack))
print('-> radius_l = {}'.format(radius_l))
print('-> radius_r = {}'.format(radius_r))

input('Veuillez aligner le robot:')

N = int(input('Combien de tours ?\n'))
if not ccw: N *= -1

theta0 = wheeledbase.get_position()[2]
lticks0, rticks0 = wheeledbase.get_codewheels_counter()

angvel = wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)
if not ccw: angvel = -angvel
wheeledbase.set_velocities(0, angvel)
while abs(wheeledbase.get_position()[2] - theta0) / (2 * math.pi) < abs(N):
	wheeledbase.set_velocities(0, angvel)
	progress = (wheeledbase.get_position()[2] - theta0) / (2 * math.pi) / N
	print('{:.0f}/{} ({:.0f}%)\ttemps restant: {:.0f}'.format(N * progress, N, 100 * progress, N * 2 * math.pi / angvel * (1 - progress)), end='\r')
	time.sleep(0.1)
wheeledbase.stop()

input('Veuillez réaligner le robot sur son orientation initiale:')

lticks1, rticks1 = wheeledbase.get_codewheels_counter()
print('lticks = {}\trticks = {}'.format(lticks1 - lticks0, rticks1 - rticks0))

alpha_l = (lticks1 - lticks0) / (leftcodewheel_countperrev * N)
alpha_r = (rticks1 - rticks0) / (rightcodewheel_countperrev * N)
print('alpha_l = {}\t alpha_r = {}'.format(alpha_l, alpha_r))

axletrack = alpha_r * radius_r + alpha_l * radius_l
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

radius_r = beta * (-radius_l)
axletrack = alpha_r * radius_r + alpha_l * radius_l
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, abs(radius_r), FLOAT)
print('-> axletrack = {}'.format(axletrack))
print('-> radius_r = {}'.format(radius_r))

input('Veuillez à nouveau aligner le robot:')

D = int(input('Quelle distance (en mm) ?\n'))

lticks0, rticks0 = wheeledbase.get_codewheels_counter()
wheeledbase.reset()
wheeledbase.goto(D, 0)
wheeledbase.stop()

input('Veuillez réaligner le robot sur son orientation initiale:')

lticks1, rticks1 = wheeledbase.get_codewheels_counter()
print('lticks = {}\trticks = {}'.format(lticks1 - lticks0, rticks1 - rticks0))

radius_l = -(D * (leftcodewheel_countperrev + rightcodewheel_countperrev) / 2) / (math.pi * (beta * (rticks1 - rticks0) + (lticks1 - lticks0)))
radius_r = beta *  (-radius_l)
axletrack = alpha_r * radius_r + alpha_l * radius_l
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID, axletrack, FLOAT)
wheeledbase.set_parameter_value(LEFTCODEWHEEL_RADIUS_ID, abs(radius_l), FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, abs(radius_r), FLOAT)
print('-> axletrack = {}'.format(axletrack))
print('-> radius_l = {}'.format(radius_l))
print('-> radius_r = {}'.format(radius_r))

save = input('Souhaitez-vous sauvegarder ces valeurs ?(o/N)\n')

if(save == "o" or save == "oui" or save == "yes"):
    print('Vous pouvez copier ces valeurs dans le fichier des constantes :\n')
    print('LEFTCODEWHEEL_RADIUS_VALUE	        = {}'.format(radius_l))
    print('RIGHTCODEWHEEL_RADIUS_VALUE         = {}'.format(radius_r))
    print('ODOMETRY_AXLETRACK_VALUE            = {}'.format(axletrack))
else :
    print("Rechargement des constantes par défaut : ")
    write_cst()
    print('fait')
