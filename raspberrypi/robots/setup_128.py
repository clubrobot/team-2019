from robots.setup_wheeledbase import *
from daughter_cards.arm.RobotArm import *
from daughter_cards.arm.ArmController import *
from daughter_cards.arm.arm_constants import *

# Configure wheeledbase
wheeledbase.set_velocities(0, 0)

# Configure arm
robArm = RobotArm(manager)

robArm.set_motors_id(AX12_1_ID, AX12_2_ID, AX12_3_ID)
robArm.set_motors_offsets(AX12_1_OFFSET, AX12_2_OFFSET, AX12_3_OFFSET)
robArm.set_workspaces(WS_FRONT, WS_BACK)
robArm.set_links_len(LINK1_LEN, LINK2_LEN, LINK3_LEN)
robArm.set_origin(ORIGIN)
robArm.begin()

# Configure arm Mover
armController = ArmController(robArm)
armController.setup_tank_size(3)

# Start arm
armController.arm.stop_pump()
armController.go_home()