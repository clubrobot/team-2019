from collections import namedtuple

ArmPos = namedtuple('ArmPos', ['x', 'y', 'phi'])

#                                      | x(cm) | y(cm) | phi(deg)
# Home
HOME                            = ArmPos(1.5,    7.0,    0)
GLOBAL_POS_INTER                = ArmPos(0 ,    19,  90)
GLOBAL_POS_INTER_AFTER_TAKE     = ArmPos(0 ,   19.5,   90)
GLOBAL_POS_INTER_AFTER_BALANCE  = ArmPos(0 ,   19.5,   90)
# Prepare Taking
PREPARE_TAKING_POS_ROAD         = ArmPos(13.7,   3.1,    10)
PREPARE_TAKING_POS_STATIC       = ArmPos(14.7,   3.5,    00)

# Take Puck
TAKE_PUCK_STATIC                = ArmPos(17,   3.0,    5)
TAKE_PUCK_INTER_AFTER_STATIC    = ArmPos(10.2,    12.0,   90.0)

# Tank Inter
TANK_POS_INTER                  = ArmPos(-8.0,   10.0,   190)

TANK_POS_INTER_150              = ArmPos(-8.0,   10.0,   200)

# Tank Put puck1
PUT_TANK_PUCK1                 = ArmPos(-15.5,   2.2,   200)
# Tank Put puck2
PUT_TANK_PUCK2                 = ArmPos(-13.5,   3.2,   200)
# Tank Put puck3
PUT_TANK_PUCK3                 = ArmPos(-10.5,  2.2,   200)

#put tank after()
PUT_TANK_AFTER                 = ArmPos(-5.5,  10.0,   120)

# Tank Take Puck1
BEFORE_TAKE_TANK_PUCK1          = ArmPos(-12.5,   2,   200)
TAKE_TANK_PUCK1                 = ArmPos(-16,   2,   200)
AFTER_TAKE_TANK_PUCK1           = ArmPos(-11.5,   7,   160)

# Tank Take Puck2
BEFORE_TAKE_TANK_PUCK2          = ArmPos(-11.5,   2.5,   210)
TAKE_TANK_PUCK2                 = ArmPos(-14,   2,   210)
AFTER_TAKE_TANK_PUCK2           = ArmPos(-10.5,   7,   160)

# Tank Take Puck3
BEFORE_TAKE_TANK_PUCK3          = ArmPos(-9.5,  3.5,   210)
TAKE_TANK_PUCK3                 = ArmPos(-11,  3.2,   210)
AFTER_TAKE_TANK_PUCK3           = ArmPos(-7.5,  7.0,   160)

# Put Balance
BALANCE                         = ArmPos(17.0,   10.0,   30)

RED_ZONE1                       = ArmPos(11.0,   -5.0,   270)
RED_ZONE2                       = ArmPos(11.0,   -3.0,   270)
RED_ZONE3                       = ArmPos(11.0,   -1.0,   270)

# Accelerator 
ACCELERATOR_BEFORE_ARM1          = ArmPos(12,   10.0,  45)
ACCELERATOR_ARM1                 = ArmPos(12.2,    7,  25)
ACCELERATOR_AFTER_ARM1           = ArmPos(12,   10.0,  45)

ACCELERATOR_BEFORE_ARM2          = ArmPos(12,   13.0,  45)
ACCELERATOR_ARM2                 = ArmPos(12.2,    10.0,  25)
ACCELERATOR_AFTER_ARM2           = ArmPos(12,   13.0,  45)

BALANCE3_POS_INTER               = ArmPos(-5 ,    15,   90)


HomePath = [GLOBAL_POS_INTER, HOME]