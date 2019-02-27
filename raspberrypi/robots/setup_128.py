from robots.setup_wheeledbase import *
from daughter_cards.RobotArm import *
from daughter_cards.arm_constants import *
wheeledbase.set_velocities(0, 0)

arm = RobotArm(manager)

arm.set_motors_id(AX12_1_ID, AX12_2_ID, AX12_3_ID)
arm.set_motors_offsets(AX12_1_OFFSET, AX12_2_OFFSET, AX12_3_OFFSET)
arm.set_workspaces(WS_FRONT, WS_BACK)
arm.set_links_len(LINK1_LEN, LINK2_LEN, LINK3_LEN)
arm.set_origin(ORIGIN)

arm.begin()

arm.stop_pump()
arm.go_home()