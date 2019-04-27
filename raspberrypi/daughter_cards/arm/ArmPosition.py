from collections import namedtuple

ArmPos = namedtuple('ArmPos', ['x', 'y', 'phi'])

#                                      | x(cm) | y(cm) | phi(deg)
# Home
HOME                            = ArmPos(1.5,    7.0,    0)
GLOBAL_POS_INTER                = ArmPos(0 ,    17.8,   90)

# Prepare Taking
PREPARE_TAKING_POS_ROAD         = ArmPos(11.5,   3.0,    10)
PREPARE_TAKING_POS_STATIC       = ArmPos(13,   4,    10)

# Take Puck
TAKE_PUCK_STATIC                = ArmPos(14.8,   3.5,    5)
TAKE_PUCK_INTER_AFTER_STATIC    = ArmPos(9.0,    10.0,   90.0)

# Tank Inter
TANK_POS_INTER                  = ArmPos(-5.0,   8.0,   190)

TANK_POS_INTER_150              = ArmPos(-5.0,   8.0,   200)

# Tank Put puck1
PUT_TANK_PUCK1                 = ArmPos(-12.5,   2,   200)
# Tank Put puck2
PUT_TANK_PUCK2                 = ArmPos(-10.5,   3,   200)
# Tank Put puck3
PUT_TANK_PUCK3                 = ArmPos(-8.5,  2.0,   200)

#put tank after()
PUT_TANK_AFTER                 = ArmPos(-5.5,  8.0,   120)

# Tank Take Puck1
BEFORE_TAKE_TANK_PUCK1          = ArmPos(-10.5,   2,   200)
TAKE_TANK_PUCK1                 = ArmPos(-14.5,   2,   200)
AFTER_TAKE_TANK_PUCK1           = ArmPos(-9.5,   7,   160)

# Tank Take Puck2
BEFORE_TAKE_TANK_PUCK2          = ArmPos(-9.5,   3,   210)
TAKE_TANK_PUCK2                 = ArmPos(-12,   2,   210)
AFTER_TAKE_TANK_PUCK2           = ArmPos(-8.5,   7,   160)

# Tank Take Puck3
BEFORE_TAKE_TANK_PUCK3          = ArmPos(-7.5,  4.0,   210)
TAKE_TANK_PUCK3                 = ArmPos(-9.5,  3.0,   210)
AFTER_TAKE_TANK_PUCK3           = ArmPos(-5.5,  7.0,   160)

# Put Balance
BALANCE                         = ArmPos(13.0,   10.0,   10)
RED_ZONE                        = ArmPos(10.0,   2.0,   300)

# Accelerator 
ACCELERATOR_BEFORE              = ArmPos(6.5,   6.0,   20)
ACCELERATOR                     = ArmPos(6.0,   4.5,   15)
ACCELERATOR_AFTER               = ArmPos(7.0,   10.0,   10)

BALANCE3_POS_INTER              = ArmPos(-5 ,    14,   90)


HomePath = [GLOBAL_POS_INTER, HOME]