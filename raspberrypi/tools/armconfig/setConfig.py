from robots.setup_wheeledbase import *
from daughter_cards.arm.RobotArm import *

AX12_1_ID   = 1
AX12_2_ID   = 3
AX12_3_ID   = 2

AX12_1_OFFSET = 60.0
AX12_2_OFFSET = 150.0
AX12_3_OFFSET = 150.0

LINK1_LEN     = 7.0
LINK2_LEN     = 7.0
LINK3_LEN     = 6
ELBOW         = ELBOW_BACK

WS_FRONT    = Workspace(1.0, 30.0, -10.0, 30.0, ELBOW_BACK)
WS_BACK     = Workspace(-30.0, 1.0, -10.0, 30.0, ELBOW_FRONT)

ORIGIN      = SpacePoint(0.0, 0.0, 0.0)
JOINTS      = JointPoint(0.0, 0.0, 0.0)

TIMESTEP    = 0.025

# Configure arm
robArm = RobotArm(manager)

robArm.set_parameter_value(MOTOR1_ID_ID, AX12_1_ID, INT)
robArm.set_parameter_value(MOTOR2_ID_ID, AX12_2_ID, INT)
robArm.set_parameter_value(MOTOR3_ID_ID, AX12_3_ID, INT)
time.sleep(0.1)
robArm.set_parameter_value(MOTOR1_OFFSET_ID, AX12_1_OFFSET, FLOAT)
robArm.set_parameter_value(MOTOR2_OFFSET_ID, AX12_2_OFFSET, FLOAT)
robArm.set_parameter_value(MOTOR3_OFFSET_ID, AX12_3_OFFSET, FLOAT)
time.sleep(0.1)
robArm.set_parameter_value(LINK_1_ID, LINK1_LEN, FLOAT)
robArm.set_parameter_value(LINK_2_ID, LINK2_LEN, FLOAT)
robArm.set_parameter_value(LINK_3_ID, LINK3_LEN, FLOAT)
robArm.set_parameter_value(ELBOW_ID, ELBOW     , FLOAT)
time.sleep(0.1)
robArm.set_workspace(WORKSPACE_FRONT_ID, WS_FRONT)
time.sleep(0.1)
robArm.set_workspace(WORKSPACE_BACK_ID , WS_BACK)
time.sleep(0.1)
robArm.set_origin(ORIGIN)
time.sleep(0.1)
robArm.set_joint(JOINTS)

robArm.set_parameter_value(TIMESTEP_ID, TIMESTEP, FLOAT)