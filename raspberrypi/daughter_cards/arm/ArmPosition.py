from collections import namedtuple

ArmPos = namedtuple('ArmPos', ['x', 'y', 'phi'])

#                                      | x(cm) | y(cm) | phi(deg)
# Home
HOME                            = ArmPos(1.5,    7.0,    0)
GLOBAL_POS_INTER                = ArmPos(3.8,    13.0,   0)

# Prepare Taking
PREPARE_TAKING_POS_ROAD         = ArmPos(11.5,   3.0,    10)
PREPARE_TAKING_POS_STATIC       = ArmPos(13,   4,    10)

# Take Puck
TAKE_PUCK_STATIC                = ArmPos(14.8,   5,    10)
TAKE_PUCK_INTER_AFTER_STATIC    = ArmPos(9.0,    10.0,   90.0)

# Tank Inter
TANK_POS_INTER                  = ArmPos(-5.0,   8.0,   190)


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
AFTER_TAKE_TANK_PUCK1           = ArmPos(-10.5,   6,   160)

# Tank Take Puck2
BEFORE_TAKE_TANK_PUCK2          = ArmPos(-9.5,   3,   210)
TAKE_TANK_PUCK2                 = ArmPos(-12,   2,   210)
AFTER_TAKE_TANK_PUCK2           = ArmPos(-9.5,   6,   160)

# Tank Take Puck3
BEFORE_TAKE_TANK_PUCK3          = ArmPos(-7.5,  4.0,   210)
TAKE_TANK_PUCK3                 = ArmPos(-9.5,  3.0,   210)
AFTER_TAKE_TANK_PUCK3           = ArmPos(-6.5,  6.0,   160)

# Put Balance
BALANCE                         = ArmPos(13.0,   10.0,   10)
RED_ZONE                        = ArmPos(10.0,   2.0,   300)


HomePath = [GLOBAL_POS_INTER, HOME]