from collections import namedtuple

ArmPos = namedtuple('ArmPos', ['x', 'y', 'phi'])

#                                      | x(cm) | y(cm) | phi(deg)
HOME                            = ArmPos(1.5,    9.0,    10)

TAKE_PUCK_INTER_BEFORE          = ArmPos(14.5,   1.5,    10)
TAKE_PUCK_INTER_AFTER_1         = ArmPos(15.5,   1.5,    45.0)
TAKE_PUCK_INTER_AFTER_2         = ArmPos(9.0,    10.0,   90.0)
TAKE_PUCK_INTER_SOL             = ArmPos(8.0,    8.0,    0.0)
TAKE_PUCK                       = ArmPos(15.7,   1.5,    10)

TANK_POS_INTER_PUT              = ArmPos(0.0,   14.0,   160)
TANK_POS_INTER                  = ArmPos(-6.0,   8.0,   190)
TANK_POS_INTER_TAKE             = ArmPos(-3.0,   10.0,   130)

TANK_POS_INTER_TAKE_PUCK3       = ArmPos(-4.5,   5.0,    220)
TANK_POS_INTER_TAKE_PUCK2       = ArmPos(-6.0,   4.0,    220)
TANK_POS_INTER_TAKE_PUCK1       = ArmPos(-8.5,   3.0,    220)

TANK_POS_INTER_TAKE_PUCK3_BIS   = ArmPos(-5.0,   6.0,    140)
TANK_POS_INTER_TAKE_PUCK2_BIS   = ArmPos(-8.0,   6.0,    140)
TANK_POS_INTER_TAKE_PUCK1_BIS   = ArmPos(-9.0,   6.0,    140)

TANK_POS_3                      = ArmPos(-7.0,   3.0,    205)
TANK_POS_2                      = ArmPos(-9.0,   3.0,    205)
TANK_POS_1                      = ArmPos(-12.0,  3.0,    205)
   
BALANCE                         = ArmPos(13.0,   10.0,   0)

SOL_POS                         = ArmPos(12.5,   -6.0,   270)
SOL_POS_INTER                   = ArmPos(12.5,   -2.0,   270)

GLOBAL_POS_INTER                = ArmPos(3.8,    13.0,   0)