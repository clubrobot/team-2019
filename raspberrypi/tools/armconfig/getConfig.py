from robots.setup_wheeledbase import *
from daughter_cards.arm.RobotArm import *

# Configure arm
robArm = RobotArm(manager)

print('MOTOR1 ID : '+str(robArm.get_parameter_value(MOTOR1_ID_ID, INT)))
print('MOTOR2 ID : '+str(robArm.get_parameter_value(MOTOR2_ID_ID, INT)))
print('MOTOR3 ID : '+str(robArm.get_parameter_value(MOTOR3_ID_ID, INT)))

print('MOTOR1 OFFSET : '+str(robArm.get_parameter_value(MOTOR1_OFFSET_ID, FLOAT)))
print('MOTOR2 OFFSET : '+str(robArm.get_parameter_value(MOTOR2_OFFSET_ID, FLOAT)))
print('MOTOR3 OFFSET : '+str(robArm.get_parameter_value(MOTOR3_OFFSET_ID, FLOAT)))

print('LINK1 LEN : '+str(robArm.get_parameter_value(LINK_1_ID, FLOAT)))
print('LINK2 LEN : '+str(robArm.get_parameter_value(LINK_2_ID, FLOAT)))
print('LINK3 LEN : '+str(robArm.get_parameter_value(LINK_3_ID, FLOAT)))
print('ELBOW OR  : '+str(robArm.get_parameter_value(ELBOW_ID,  FLOAT)))

print('WORKSPACE FRONT : '+str(robArm.get_workspace(WORKSPACE_FRONT_ID)))
print('WORKSPACE BACK  : '+str(robArm.get_workspace(WORKSPACE_BACK_ID)))

print('ORIGIN : '+str(robArm.get_origin()))
print('JOINTS : '+str(robArm.get_joint()))

print('TIMESTEP : '+str(robArm.get_parameter_value(TIMESTEP_ID, FLOAT)))