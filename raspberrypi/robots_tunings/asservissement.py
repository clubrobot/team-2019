#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_wheeledbase import *
from asserv_points import * 
import time
import math
import argparse
import matplotlib.pyplot as plt
parser = argparse.ArgumentParser()
parser.add_argument("robot_name")
args = parser.parse_args()

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





def change_PID_parameters(PID):
	print("Les coefficients actuels sont : Kp = " + str(PID[0]) + " | Ki = " +  str(PID[1]) + " | Kd = " +  str(PID[2])+ " |")
	newPID = list(PID)
	end = False
	while(not end):
		coeff_type = input('-> Modifier un coefficient ? (P - I - D ou ok) : ')
		if(coeff_type.lower() == "kp" or coeff_type.lower() == "p" ):
			newPID[0] = float(input('-> Kp = ? '))
		elif(coeff_type.lower() == "ki" or coeff_type.lower() == "i" ):
			newPID[1] = float(input('-> Ki = ? '))
		elif(coeff_type.lower() == "kd" or coeff_type.lower() == "d" ):
			newPID[2] = float(input('-> Kd = ? '))
		else:
			end = True
		if(not end):
			print("\n Kp = " +  str(newPID[0]) + " | Ki = " +  str(newPID[1]) + " | Kd = " +  str(newPID[2])+ " |\n")
	return tuple(newPID) 

asser_type = input("Type d'asservissement : \n 1 - Position      2 - Vitesse \n-> ")

#
#   ----- ASSERVISEMENT EN POSITION  ------
#
if(asser_type.lower() == "p" or asser_type.lower() == "position" or asser_type.lower() == "1" or asser_type.lower() == "pos"):
	first_it = True
	run_again = True
	print(" ----- ASSERVISEMENT EN POSITION  ----- ")
	PID = ( wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELKP_ID ,FLOAT),
			wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELKP_ID ,FLOAT),
			0)

	while(run_again):    
		PID = change_PID_parameters(PID)
		wheeledbase.set_parameter_value(POSITIONCONTROL_LINVELKP_ID, PID[0] ,FLOAT)
		wheeledbase.set_parameter_value(POSITIONCONTROL_ANGVELKP_ID, PID[1] ,FLOAT)
		
		input("Appuyez sur entrée pour lancer le test :")
		wheeledbase.reset() 
	# Data arrays
		time_array       = [] # time
		linvel_sp_array  = [] # linear speed setpoint
		linvel_in_array  = [] # linear speed input (real speed)
		linvel_out_array = [] # linear speed output (given to the motors)
		angvel_sp_array  = [] # angular speed setpoint
		angvel_in_array  = [] # angular speed input
		angvel_out_array = [] # angular speed output

		wheeledbase.getout(timeout=1)
		wheeledbase.geterr(timeout=1)

		print('start benchmark')

		try:
			for s in objpoints_cmd:
				x_sp, y_sp, theta_sp = s[:]
				wheeledbase.goto(x_sp, y_sp, theta_sp)

			#   Cut off motors
		finally:
			wheeledbase.stop()

		print('benchmark over')
		
		# Get Arduino output
		out = wheeledbase.getout(timeout=1)
		err = wheeledbase.geterr(timeout=1)

		# Print Arduino output
		print(out)
		print(err)

		for line in out.splitlines():

			if not line.startswith('#'):
				values = ['nan' if x == 'ovf' else x for x in line.split('\t')]

				t = float(values[0])
				time_array.append(t * 1e-3)
				
				linvel_sp, linvel_in, linvel_out = map(float, values[1:4])
				linvel_sp_array .append(linvel_sp)
				linvel_in_array .append(linvel_in)
				linvel_out_array.append(linvel_out)
				
				angvel_sp, angvel_in, angvel_out = map(float, values[4:7])
				angvel_sp_array .append(angvel_sp)
				angvel_in_array .append(angvel_in)
				angvel_out_array.append(angvel_out)
		
		plt.figure(1)
		plt.subplot(211)
		plt.plot(time_array, linvel_sp_array, time_array, linvel_in_array, time_array,linvel_out_array)
		plt.title("LinKp= " + str(PID[0]) + " | AngKp = " +  str(PID[1]), fontsize=12)
		plt.ylabel("Linear velocity")  

		plt.subplot(212)
		plt.plot(time_array, angvel_sp_array, time_array, angvel_in_array, time_array, angvel_out_array)
		plt.ylabel("Angular velocity")                     

		#affichage du test précédent pour comparer
		if(not first_it):
			plt.figure(2)
			plt.subplot(211)
			plt.title("Prec : LinKp = " + str(PID_old[0]) + " | AngKp = " +  str(PID_old[1]), fontsize=12)
			plt.plot(time_array_old, linvel_sp_array_old, time_array_old, linvel_in_array_old, time_array_old, linvel_out_array_old)
			plt.ylabel("Linear velocity")  

			plt.subplot(212)
			plt.plot(time_array_old, angvel_sp_array_old, time_array_old, angvel_in_array_old, time_array_old, angvel_out_array_old)
			plt.ylabel("Angular velocity")

		axletrack = wheeledbase.wheels_axletrack.get()
		leftwheel_sp  = [linvel_out_array[i] - angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]
		rightwheel_sp = [linvel_out_array[i] + angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]

		plt.figure(3)
		plt.plot(time_array, leftwheel_sp, time_array, rightwheel_sp)
		plt.title('Wheels setpoints')
		plt.show()

		#sauvegarde pour comparaison
		time_array_old =	time_array
		linvel_sp_array_old = linvel_sp_array
		linvel_in_array_old = linvel_in_array
		linvel_out_array_old = linvel_out_array
		angvel_sp_array_old = angvel_sp_array
		angvel_in_array_old = angvel_in_array
		angvel_out_array_old = angvel_out_array
		PID_old = PID		
		restart = input("Faire un autre test : (O/n) ")
		if(restart.lower() == "n" or restart.lower() == "non" or restart.lower() == "no"):
			run_again = False
			to_save_value = "POSITIONCONTROL_LINVELKP_VALUE      = " + str(PID[0]) +"\nPOSITIONCONTROL_ANGVELKP_VALUE      = " + str(PID[1])	
		first_it = False
	#end while


#
#   ----- ASSERVISEMENT EN VITESSE  ------
#
elif(asser_type.lower() == "v" or asser_type.lower() == "vitesse" or asser_type.lower() == "2" or asser_type.lower() == "vel" or asser_type.lower() == "velocities"):
	velocity_type = input("Quel type de vitesse ?\n 1 - Linéaire      2 - angulaire \n->")

	#   ----- VITESSE LINEAIRE ------
	if(velocity_type.lower() == "l" or velocity_type.lower() == "1" or velocity_type.lower() == "lineaire" or velocity_type.lower() == "lineare" or velocity_type.lower() == "lin"):
		first_it = True
		run_again = True
		print(" ----- ASSERVISEMENT EN VITESSE LINEAIRE  ----- ")
		PID = ( wheeledbase.get_parameter_value(LINVELPID_KP_ID, FLOAT),
				wheeledbase.get_parameter_value(LINVELPID_KI_ID, FLOAT),
				wheeledbase.get_parameter_value(LINVELPID_KD_ID, FLOAT))
		while(run_again):
			PID = change_PID_parameters(PID)
			wheeledbase.set_parameter_value(LINVELPID_KP_ID, PID[0] ,FLOAT)
			wheeledbase.set_parameter_value(LINVELPID_KI_ID, PID[1] ,FLOAT)
			wheeledbase.set_parameter_value(LINVELPID_KD_ID, PID[2] ,FLOAT)

			input("Appuyez sur entrée pour lancer le test :")
			# Data arrays
			time_array       = [] # time
			linvel_sp_array  = [] # linear speed setpoint
			linvel_in_array  = [] # linear speed input (real speed)
			linvel_out_array = [] # linear speed output (given to the motors)
			angvel_sp_array  = [] # angular speed setpoint
			angvel_in_array  = [] # angular speed input
			angvel_out_array = [] # angular speed output

			wheeledbase.getout(timeout=1)
			wheeledbase.geterr(timeout=1)

			print('start benchmark')

			try:
				for s in linvel_cmd:
					t, linvel_sp = s[:]
					wheeledbase.set_velocities(linvel_sp, 0)
					time.sleep(t)

				#   Cut off motors
			finally:
				wheeledbase.stop()

			print('benchmark over')
			
			# Get Arduino output
			out = wheeledbase.getout(timeout=1)
			err = wheeledbase.geterr(timeout=1)

			# Print Arduino output
			print(out)
			print(err)

			for line in out.splitlines():

				if not line.startswith('#'):
					values = ['nan' if x == 'ovf' else x for x in line.split('\t')]

					t = float(values[0])
					time_array.append(t * 1e-3)
					
					linvel_sp, linvel_in, linvel_out = map(float, values[1:4])
					linvel_sp_array .append(linvel_sp)
					linvel_in_array .append(linvel_in)
					linvel_out_array.append(linvel_out)
					
					angvel_sp, angvel_in, angvel_out = map(float, values[4:7])
					angvel_sp_array .append(angvel_sp)
					angvel_in_array .append(angvel_in)
					angvel_out_array.append(angvel_out)
			
			plt.figure(1)
			plt.subplot(211)
			plt.plot(time_array, linvel_sp_array, time_array, linvel_in_array, time_array,linvel_out_array)
			plt.title("Kp = " + str(PID[0]) + " | Ki = " +  str(PID[1]) + " | Kd = " +  str(PID[2]), fontsize=12)
			plt.ylabel("Linear velocity")  

			plt.subplot(212)
			plt.plot(time_array, angvel_sp_array, time_array, angvel_in_array, time_array, angvel_out_array)
			plt.ylabel("Angular velocity")                     

			#affichage du test précédent pour comparer
			if(not first_it):
				plt.figure(2)
				plt.subplot(211)
				plt.title("Précédent : Kp = " + str(PID_old[0]) + " | Ki = " +  str(PID_old[1]) + " | Kd = " +  str(PID_old[2]), fontsize=12)
				plt.plot(time_array_old, linvel_sp_array_old, time_array_old, linvel_in_array_old, time_array_old, linvel_out_array_old)
				plt.ylabel("Linear velocity")  

				plt.subplot(212)
				plt.plot(time_array_old, angvel_sp_array_old, time_array_old, angvel_in_array_old, time_array_old, angvel_out_array_old)
				plt.ylabel("Angular velocity")

			axletrack = wheeledbase.wheels_axletrack.get()
			leftwheel_sp  = [linvel_out_array[i] - angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]
			rightwheel_sp = [linvel_out_array[i] + angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]

			plt.figure(3)
			plt.plot(time_array, leftwheel_sp, time_array, rightwheel_sp)
			plt.title('Wheels setpoints')
			plt.show()

			#sauvegarde pour comparaison
			time_array_old =	time_array
			linvel_sp_array_old = linvel_sp_array
			linvel_in_array_old = linvel_in_array
			linvel_out_array_old = linvel_out_array
			angvel_sp_array_old = angvel_sp_array
			angvel_in_array_old = angvel_in_array
			angvel_out_array_old = angvel_out_array
			PID_old = PID

			restart = input("Faire un autre test : (O/n) ")
			if(restart.lower() == "n" or restart.lower() == "non" or restart.lower() == "no" ):
				run_again = False
				to_save_value = "LINVELPID_KP_VALUE      = " + str(PID[0]) +"\nLINVELPID_KI_VALUE      = " + str(PID[1]) +"\nLINVELPID_KD_VALUE      = " + str(PID[2])
			first_it = False
		#end while
		
	#   ----- VITESSE ANGULAIRE ------
	elif (velocity_type.lower() == "a" or velocity_type.lower() == "2" or velocity_type.lower() == "angulaire" or velocity_type.lower() == "angular" or velocity_type.lower() == "ang"):
		first_it = True
		run_again = True
		print(" ----- ASSERVISEMENT EN VITESSE ANGULAIRE  ----- ")
		PID = ( wheeledbase.get_parameter_value(ANGVELPID_KP_ID ,FLOAT),
				wheeledbase.get_parameter_value(ANGVELPID_KI_ID ,FLOAT),
				wheeledbase.get_parameter_value(ANGVELPID_KD_ID ,FLOAT))
		while(run_again):    
			PID = change_PID_parameters(PID)
			wheeledbase.set_parameter_value(ANGVELPID_KP_ID, PID[0] ,FLOAT)
			wheeledbase.set_parameter_value(ANGVELPID_KI_ID, PID[1] ,FLOAT)
			wheeledbase.set_parameter_value(ANGVELPID_KD_ID, PID[2] ,FLOAT) 
			
			input("Appuyez sur entrée pour lancer le test :") 
		# Data arrays
			time_array       = [] # time
			linvel_sp_array  = [] # linear speed setpoint
			linvel_in_array  = [] # linear speed input (real speed)
			linvel_out_array = [] # linear speed output (given to the motors)
			angvel_sp_array  = [] # angular speed setpoint
			angvel_in_array  = [] # angular speed input
			angvel_out_array = [] # angular speed output

			wheeledbase.getout(timeout=1)
			wheeledbase.geterr(timeout=1)

			print('start benchmark')

			try:
				for s in angvel_cmd:
					t, anvel_sp = s[:]
					wheeledbase.set_velocities(0, anvel_sp)
					time.sleep(t)

				#   Cut off motors
			finally:
				wheeledbase.stop()

			print('benchmark over')
			
			# Get Arduino output
			out = wheeledbase.getout(timeout=1)
			err = wheeledbase.geterr(timeout=1)

			# Print Arduino output
			print(out)
			print(err)

			for line in out.splitlines():

				if not line.startswith('#'):
					values = ['nan' if x == 'ovf' else x for x in line.split('\t')]

					t = float(values[0])
					time_array.append(t * 1e-3)
					
					linvel_sp, linvel_in, linvel_out = map(float, values[1:4])
					linvel_sp_array .append(linvel_sp)
					linvel_in_array .append(linvel_in)
					linvel_out_array.append(linvel_out)
					
					angvel_sp, angvel_in, angvel_out = map(float, values[4:7])
					angvel_sp_array .append(angvel_sp)
					angvel_in_array .append(angvel_in)
					angvel_out_array.append(angvel_out)
			
			plt.figure(1)
			plt.subplot(211)
			plt.plot(time_array, linvel_sp_array, time_array, linvel_in_array, time_array,linvel_out_array)
			plt.title("Kp = " + str(PID[0]) + " | Ki = " +  str(PID[1]) + " | Kd = " +  str(PID[2]), fontsize=12)
			plt.ylabel("Linear velocity")  

			plt.subplot(212)
			plt.plot(time_array, angvel_sp_array, time_array, angvel_in_array, time_array, angvel_out_array)
			plt.ylabel("Angular velocity")                     

			#affichage du test précédent pour comparer
			if(not first_it):
				plt.figure(2)
				plt.subplot(211)
				plt.title("Précédent : Kp = " + str(PID_old[0]) + " | Ki = " +  str(PID_old[1]) + " | Kd = " +  str(PID_old[2]), fontsize=12)
				plt.plot(time_array_old, linvel_sp_array_old, time_array_old, linvel_in_array_old, time_array_old, linvel_out_array_old)
				plt.ylabel("Linear velocity")  

				plt.subplot(212)
				plt.plot(time_array_old, angvel_sp_array_old, time_array_old, angvel_in_array_old, time_array_old, angvel_out_array_old)
				plt.ylabel("Angular velocity")

			axletrack = wheeledbase.wheels_axletrack.get()
			leftwheel_sp  = [linvel_out_array[i] - angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]
			rightwheel_sp = [linvel_out_array[i] + angvel_out_array[i] * axletrack / 2 for i in range(len(linvel_out_array))]

			plt.figure(3)
			plt.plot(time_array, leftwheel_sp, time_array, rightwheel_sp)
			plt.title('Wheels setpoints')
			plt.show()

			#sauvegarde pour comparaison
			time_array_old =	time_array
			linvel_sp_array_old = linvel_sp_array
			linvel_in_array_old = linvel_in_array
			linvel_out_array_old = linvel_out_array
			angvel_sp_array_old = angvel_sp_array
			angvel_in_array_old = angvel_in_array
			angvel_out_array_old = angvel_out_array
			PID_old = PID
				
			
			
			restart = input("Faire un autre test : (O/n) ")
			if(restart.lower() == "n" or restart.lower() == "non" or restart.lower() == "no"):
				run_again = False
				to_save_value = "ANGVELPID_KP_VALUE      = " + str(PID[0]) +"\nANGVELPID_KI_VALUE      = " + str(PID[1]) +"\nANGVELPID_KD_VALUE      = " + str(PID[2])
			first_it = False
		#end while




#
#   ----- SAUVEGARDE DES COEFFIECIENTS  ------
#
save = input('Souhaitez-vous sauvegarder ces valeurs ? (o/N)\n')
if(save == "o" or save == "oui" or save == "yes"):
	print('Vous pouvez copier ces valeurs dans le fichier des constantes :\n')
	print(to_save_value)
else :
	print("Rechargement des constantes par défaut : ")
	write_cst()
	print('fait')