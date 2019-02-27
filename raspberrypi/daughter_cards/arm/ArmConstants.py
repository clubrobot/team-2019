from daughter_cards.RobotArm import *

AX12_1_ID   = 1
AX12_2_ID   = 3
AX12_3_ID   = 2

AX12_1_OFFSET = 60.0
AX12_2_OFFSET = 150.0
AX12_3_OFFSET = 150.0

LINK1_LEN     = 7.0
LINK2_LEN     = 7.0
LINK3_LEN     = 3.8

WS_FRONT    = Workspace(0.0, 30.0, -10.0, 30.0, ELBOW_BACK)
WS_BACK     = Workspace(-30.0, 0.0, -10.0, 30.0, ELBOW_FRONT)

ORIGIN      = RobotSpacePoint(0.0, 0.0, 0.0)