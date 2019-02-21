#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_wheeledbase import *

# LEFTWHELL MOTOR CONSTANTS #
LEFTWHEEL_RADIUS_VALUE	            = 36.5
LEFTWHEEL_CONSTANT_VALUE            = 0.20353981852531433
LEFTWHEEL_MAXPWM_VALUE              = 1

# RIGHTWHEEL MOTOR CONSTANTS #
RIGHTWHEEL_RADIUS_VALUE             = 36.5 
RIGHTWHEEL_CONSTANT_VALUE           = 0.20353981852531433
RIGHTWHEEL_MAXPWM_VALUE             = 1

RIGHTCODEWHEEL_COUNTSPERREV_VALUE   = +8000
LEFTCODEWHEEL_COUNTSPERREV_VALUE    = -8000

LEFTCODEWHEEL_RADIUS_VALUE          = 25.851550890236105
RIGHTCODEWHEEL_RADIUS_VALUE         = 26.110835306067813

ODOMETRY_AXLETRACK_VALUE            = 124.81916465624636
ODOMETRY_SLIPPAGE_VALUE             = 0

VELOCITYCONTROL_AXLETRACK_VALUE     = 240.0
VELOCITYCONTROL_MAXLINACC_VALUE     = 1500
VELOCITYCONTROL_MAXLINDEC_VALUE     = 1500
VELOCITYCONTROL_MAXANGACC_VALUE     = 5
VELOCITYCONTROL_MAXANGDEC_VALUE     = 5
VELOCITYCONTROL_SPINSHUTDOWN_VALUE  = 1

LINVELPID_KP_VALUE                  = 1#1.79999999
LINVELPID_KI_VALUE                  = 0#035
LINVELPID_KD_VALUE                  = 0

LINVELPID_MINOUTPUT_VALUE           = -1100
LINVELPID_MAXOUTPUT_VALUE           = 1100 

ANGVELPID_KP_VALUE                  = 1#1.6
ANGVELPID_KI_VALUE                  = 0#40
ANGVELPID_KD_VALUE                  = 0

ANGVELPID_MINOUTPUT_VALUE	        = -18.0278263092041
ANGVELPID_MAXOUTPUT_VALUE	        = 18.0278263092041

POSITIONCONTROL_LINVELKP_VALUE      = 3
POSITIONCONTROL_ANGVELKP_VALUE      = 2.2

POSITIONCONTROL_LINVELMAX_VALUE     = 1000
POSITIONCONTROL_ANGVELMAX_VALUE     = 6

POSITIONCONTROL_LINPOSTHRESHOLD_VALUE  = 3
POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE  = 0.10000000149011612  

PUREPURSUIT_LOOKAHEAD_VALUE            = 200.0 
PUREPURSUIT_LOOKAHEADBIS_VALUE         = 150.0 



wheeledbase.set_parameter_value(LEFTWHEEL_RADIUS_ID	            , LEFTWHEEL_RADIUS_VALUE       , FLOAT)
wheeledbase.set_parameter_value(LEFTWHEEL_CONSTANT_ID           , LEFTWHEEL_CONSTANT_VALUE     , FLOAT)
wheeledbase.set_parameter_value(LEFTWHEEL_MAXPWM_ID             , LEFTWHEEL_MAXPWM_VALUE       , FLOAT)

wheeledbase.set_parameter_value(RIGHTWHEEL_RADIUS_ID            , RIGHTWHEEL_RADIUS_VALUE        , FLOAT)
wheeledbase.set_parameter_value(RIGHTWHEEL_CONSTANT_ID          , RIGHTWHEEL_CONSTANT_VALUE      , FLOAT)
wheeledbase.set_parameter_value(RIGHTWHEEL_MAXPWM_ID            , RIGHTWHEEL_MAXPWM_VALUE        , FLOAT)

wheeledbase.set_parameter_value(RIGHTCODEWHEEL_COUNTSPERREV_ID  , RIGHTCODEWHEEL_COUNTSPERREV_VALUE , LONG )
wheeledbase.set_parameter_value(LEFTCODEWHEEL_COUNTSPERREV_ID   , LEFTCODEWHEEL_COUNTSPERREV_VALUE  , LONG )

wheeledbase.set_parameter_value(LEFTCODEWHEEL_RADIUS_ID	        , LEFTCODEWHEEL_RADIUS_VALUE        , FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID        , RIGHTCODEWHEEL_RADIUS_VALUE       , FLOAT)


wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID           , ODOMETRY_AXLETRACK_VALUE  , FLOAT)
wheeledbase.set_parameter_value(ODOMETRY_SLIPPAGE_ID            , ODOMETRY_SLIPPAGE_VALUE   , FLOAT)

wheeledbase.set_parameter_value(VELOCITYCONTROL_AXLETRACK_ID    , VELOCITYCONTROL_AXLETRACK_VALUE     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXLINACC_ID    , VELOCITYCONTROL_MAXLINACC_VALUE     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXLINDEC_ID    , VELOCITYCONTROL_MAXLINDEC_VALUE     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXANGACC_ID    , VELOCITYCONTROL_MAXANGACC_VALUE     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXANGDEC_ID    , VELOCITYCONTROL_MAXANGDEC_VALUE     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_SPINSHUTDOWN_ID , VELOCITYCONTROL_SPINSHUTDOWN_VALUE  , BYTE )

wheeledbase.set_parameter_value(LINVELPID_KP_ID                 , LINVELPID_KP_VALUE                  , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_KI_ID                 , LINVELPID_KI_VALUE                  , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_KD_ID                 , LINVELPID_KD_VALUE                  , FLOAT)

wheeledbase.set_parameter_value(LINVELPID_MINOUTPUT_ID          , LINVELPID_MINOUTPUT_VALUE           , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_MAXOUTPUT_ID          , LINVELPID_MAXOUTPUT_VALUE           , FLOAT)

wheeledbase.set_parameter_value(ANGVELPID_KP_ID                 , ANGVELPID_KP_VALUE                  , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_KI_ID                 , ANGVELPID_KI_VALUE                  , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_KD_ID                 , ANGVELPID_KD_VALUE                  , FLOAT)

wheeledbase.set_parameter_value(ANGVELPID_MINOUTPUT_ID	        , ANGVELPID_MINOUTPUT_VALUE           , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_MAXOUTPUT_ID	        , ANGVELPID_MAXOUTPUT_VALUE           , FLOAT)

wheeledbase.set_parameter_value(POSITIONCONTROL_LINVELKP_ID     , POSITIONCONTROL_LINVELKP_VALUE      , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGVELKP_ID     , POSITIONCONTROL_ANGVELKP_VALUE      , FLOAT)

wheeledbase.set_parameter_value(POSITIONCONTROL_LINVELMAX_ID    , POSITIONCONTROL_LINVELMAX_VALUE     , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGVELMAX_ID    , POSITIONCONTROL_ANGVELMAX_VALUE     , FLOAT)

wheeledbase.set_parameter_value(POSITIONCONTROL_LINPOSTHRESHOLD_ID  , POSITIONCONTROL_LINPOSTHRESHOLD_VALUE  , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGPOSTHRESHOLD_ID  , POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE  , FLOAT)

wheeledbase.set_parameter_value(PUREPURSUIT_LOOKAHEAD_ID        , PUREPURSUIT_LOOKAHEAD_VALUE          , FLOAT)
wheeledbase.set_parameter_value(PUREPURSUIT_LOOKAHEADBIS_ID     , PUREPURSUIT_LOOKAHEADBIS_VALUE       , FLOAT)


print("LEFTWHEEL_RADIUS_VALUE                = " + str(wheeledbase.get_parameter_value(LEFTWHEEL_RADIUS_ID,     FLOAT)))
print("LEFTWHEEL_CONSTANT_VALUE              = " + str(wheeledbase.get_parameter_value(LEFTWHEEL_CONSTANT_ID,   FLOAT)))
print("LEFTWHEEL_MAXPWM_VALUE                = " + str(wheeledbase.get_parameter_value(LEFTWHEEL_MAXPWM_ID,     FLOAT)))

print("RIGHTWHEEL_RADIUS_VALUE               = " + str(wheeledbase.get_parameter_value(RIGHTWHEEL_RADIUS_ID,    FLOAT)))
print("RIGHTWHEEL_CONSTANT_VALUE             = " + str(wheeledbase.get_parameter_value(RIGHTWHEEL_CONSTANT_ID,  FLOAT)))
print("RIGHTWHEEL_MAXPWM_VALUE               = " + str(wheeledbase.get_parameter_value(RIGHTWHEEL_MAXPWM_ID,    FLOAT)))

print("RIGHTCODEWHEEL_COUNTSPERREV_VALUE     = " + str(wheeledbase.get_parameter_value(RIGHTCODEWHEEL_COUNTSPERREV_ID, LONG )))
print("LEFTCODEWHEEL_COUNTSPERREV_VALUE      = " + str(wheeledbase.get_parameter_value(LEFTCODEWHEEL_COUNTSPERREV_ID,  LONG )))

print("LEFTCODEWHEEL_RADIUS_VALUE            = " + str(wheeledbase.get_parameter_value(LEFTCODEWHEEL_RADIUS_ID,  FLOAT)))
print("RIGHTCODEWHEEL_RADIUS_VALUE           = " + str(wheeledbase.get_parameter_value(RIGHTCODEWHEEL_RADIUS_ID, FLOAT)))

print("ODOMETRY_AXLETRACK_VALUE              = " + str(wheeledbase.get_parameter_value(ODOMETRY_AXLETRACK_ID, FLOAT)))
print("ODOMETRY_SLIPPAGE_VALUE               = " + str(wheeledbase.get_parameter_value(ODOMETRY_SLIPPAGE_ID,  FLOAT)))

print("VELOCITYCONTROL_AXLETRACK_VALUE       = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_AXLETRACK_ID,    FLOAT)))
print("VELOCITYCONTROL_MAXLINACC_VALUE       = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_MAXLINACC_ID,    FLOAT)))
print("VELOCITYCONTROL_MAXLINDEC_VALUE       = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_MAXLINDEC_ID,    FLOAT)))
print("VELOCITYCONTROL_MAXANGACC_VALUE       = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_MAXANGACC_ID,    FLOAT)))
print("VELOCITYCONTROL_MAXANGDEC_VALUE       = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_MAXANGDEC_ID,    FLOAT)))
print("VELOCITYCONTROL_SPINSHUTDOWN_VALUE    = " + str(wheeledbase.get_parameter_value(VELOCITYCONTROL_SPINSHUTDOWN_ID, BYTE )))

print("LINVELPID_KP_VALUE                    = " + str(wheeledbase.get_parameter_value(LINVELPID_KP_ID, FLOAT)))
print("LINVELPID_KI_VALUE                    = " + str(wheeledbase.get_parameter_value(LINVELPID_KI_ID, FLOAT)))
print("LINVELPID_KD_VALUE                    = " + str(wheeledbase.get_parameter_value(LINVELPID_KD_ID, FLOAT)))

print("LINVELPID_MINOUTPUT_VALUE             = " + str(wheeledbase.get_parameter_value(LINVELPID_MINOUTPUT_ID, FLOAT)))
print("LINVELPID_MAXOUTPUT_VALUE             = " + str(wheeledbase.get_parameter_value(LINVELPID_MAXOUTPUT_ID, FLOAT)))

print("ANGVELPID_KP_VALUE                    = " + str(wheeledbase.get_parameter_value(ANGVELPID_KP_ID, FLOAT)))
print("ANGVELPID_KI_VALUE                    = " + str(wheeledbase.get_parameter_value(ANGVELPID_KI_ID, FLOAT)))
print("ANGVELPID_KD_VALUE                    = " + str(wheeledbase.get_parameter_value(ANGVELPID_KD_ID, FLOAT)))

print("ANGVELPID_MINOUTPUT_VALUE             = " + str(wheeledbase.get_parameter_value(ANGVELPID_MINOUTPUT_ID, FLOAT)))
print("ANGVELPID_MAXOUTPUT_VALUE	         = " + str(wheeledbase.get_parameter_value(ANGVELPID_MAXOUTPUT_ID, FLOAT)))

print("POSITIONCONTROL_LINVELKP_VALUE        = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELKP_ID, FLOAT)))
print("POSITIONCONTROL_ANGVELKP_VALUE        = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELKP_ID, FLOAT)))

print("POSITIONCONTROL_LINVELMAX_VALUE       = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELMAX_ID, FLOAT)))
print("POSITIONCONTROL_ANGVELMAX_VALUE       = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)))

print("POSITIONCONTROL_LINPOSTHRESHOLD_VALUE = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINPOSTHRESHOLD_ID, FLOAT)))
print("POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGPOSTHRESHOLD_ID, FLOAT)))

print("PUREPURSUIT_LOOKAHEAD_VALUE           = " + str(wheeledbase.get_parameter_value(PUREPURSUIT_LOOKAHEAD_ID,    FLOAT)))
print("PUREPURSUIT_LOOKAHEADBIS_VALUE        = " + str(wheeledbase.get_parameter_value(PUREPURSUIT_LOOKAHEADBIS_ID, FLOAT)))