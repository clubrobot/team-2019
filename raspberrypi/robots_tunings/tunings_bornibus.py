#!/usr/bin/env python3
#-*- coding: utf-8 -*-

from robots.setup_wheeledbase import *

LEFTWHEEL_RADIUS_VALUE	            = 22.799999237060547
LEFTWHEEL_CONSTANT_VALUE            = 0.16661794483661652
LEFTWHEEL_MAXPWM_VALUE              = 1
RIGHTWHEEL_RADIUS_VALUE             = 22.799999237060547
RIGHTWHEEL_CONSTANT_VALUE           = -0.16661794483661652
RIGHTWHEEL_MAXPWM_VALUE             = 1

RIGHTCODEWHEEL_COUNTSPERREV_VALUE   = 10000
LEFTCODEWHEEL_COUNTSPERREV_VALUE    = -10000

# ---- ODOMETRY CONSTANTS ----
LEFTCODEWHEEL_RADIUS_VALUE	        = 22.748967510427093
RIGHTCODEWHEEL_RADIUS_VALUE         = 22.737077347716163
ODOMETRY_AXLETRACK_VALUE            = 209.3511053264129


ODOMETRY_SLIPPAGE_VALUE             = 0
VELOCITYCONTROL_AXLETRACK_VALUE     = 125.0
VELOCITYCONTROL_MAXLINACC_VALUE     = 500.0
VELOCITYCONTROL_MAXLINDEC_VALUE     = 700.0
VELOCITYCONTROL_MAXANGACC_VALUE     = 5
VELOCITYCONTROL_MAXANGDEC_VALUE     = 10
VELOCITYCONTROL_SPINSHUTDOWN_VALUE  = 1

# ---- LINEAR VELOCITIES PID  ----
LINVELPID_KP_VALUE      = 1.6
LINVELPID_KI_VALUE      = 15.0
LINVELPID_KD_VALUE      = 0.0

LINVELPID_MINOUTPUT_VALUE           = -897.501220703125
LINVELPID_MAXOUTPUT_VALUE           = 897.501220703125

# ---- ANGULAR VELOCITIES PID ----
ANGVELPID_KP_VALUE      = 2.2
ANGVELPID_KI_VALUE      = 15.0
ANGVELPID_KD_VALUE      = 0.0

ANGVELPID_MINOUTPUT_VALUE	        = -14.36001968383789
ANGVELPID_MAXOUTPUT_VALUE	        = 14.36001968383789

# ---- POSITION PID ----
POSITIONCONTROL_LINVELKP_VALUE      = 1.4
POSITIONCONTROL_ANGVELKP_VALUE      = 2.4

POSITIONCONTROL_LINVELMAX_VALUE     = 400
POSITIONCONTROL_ANGVELMAX_VALUE     = 6
POSITIONCONTROL_LINPOSTHRESHOLD_VALUE  = 3
POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE  = 0.10000000149011612  
PUREPURSUIT_LOOKAHEAD_VALUE            = 150.0 
PUREPURSUIT_LOOKAHEADBIS_VALUE         = 150.0

wheeledbase.set_parameter_value(LEFTWHEEL_RADIUS_ID	            , LEFTWHEEL_RADIUS_VALUE       , FLOAT)
wheeledbase.set_parameter_value(LEFTWHEEL_CONSTANT_ID           , LEFTWHEEL_CONSTANT_VALUE        , FLOAT)
wheeledbase.set_parameter_value(LEFTWHEEL_MAXPWM_ID             , LEFTWHEEL_MAXPWM_VALUE          , FLOAT)
wheeledbase.set_parameter_value(RIGHTWHEEL_RADIUS_ID            , RIGHTWHEEL_RADIUS_VALUE        , FLOAT)
wheeledbase.set_parameter_value(RIGHTWHEEL_CONSTANT_ID          , RIGHTWHEEL_CONSTANT_VALUE      , FLOAT)
wheeledbase.set_parameter_value(RIGHTWHEEL_MAXPWM_ID            , RIGHTWHEEL_MAXPWM_VALUE                           , FLOAT)
wheeledbase.set_parameter_value(LEFTCODEWHEEL_RADIUS_ID	        , LEFTCODEWHEEL_RADIUS_VALUE        , FLOAT)
wheeledbase.set_parameter_value(LEFTCODEWHEEL_COUNTSPERREV_ID   , LEFTCODEWHEEL_COUNTSPERREV_VALUE                     , LONG )
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_RADIUS_ID        , RIGHTCODEWHEEL_RADIUS_VALUE         , FLOAT)
wheeledbase.set_parameter_value(RIGHTCODEWHEEL_COUNTSPERREV_ID  , RIGHTCODEWHEEL_COUNTSPERREV_VALUE                      , LONG )
wheeledbase.set_parameter_value(ODOMETRY_AXLETRACK_ID           , ODOMETRY_AXLETRACK_VALUE          , FLOAT)
wheeledbase.set_parameter_value(ODOMETRY_SLIPPAGE_ID            , ODOMETRY_SLIPPAGE_VALUE                         , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_AXLETRACK_ID    , VELOCITYCONTROL_AXLETRACK_VALUE                     , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXLINACC_ID    , VELOCITYCONTROL_MAXLINACC_VALUE                      , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXLINDEC_ID    , VELOCITYCONTROL_MAXLINDEC_VALUE                      , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXANGACC_ID    , VELOCITYCONTROL_MAXANGACC_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_MAXANGDEC_ID    , VELOCITYCONTROL_MAXANGDEC_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(VELOCITYCONTROL_SPINSHUTDOWN_ID , VELOCITYCONTROL_SPINSHUTDOWN_VALUE                          , BYTE )
wheeledbase.set_parameter_value(LINVELPID_KP_ID                 , LINVELPID_KP_VALUE                       , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_KI_ID                 , LINVELPID_KI_VALUE                        , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_KD_ID                 , LINVELPID_KD_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_MINOUTPUT_ID          , LINVELPID_MINOUTPUT_VALUE           , FLOAT)
wheeledbase.set_parameter_value(LINVELPID_MAXOUTPUT_ID          , LINVELPID_MAXOUTPUT_VALUE           , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_KP_ID                 , ANGVELPID_KP_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_KI_ID                 , ANGVELPID_KI_VALUE                         , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_KD_ID                 , ANGVELPID_KD_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_MINOUTPUT_ID	        , ANGVELPID_MINOUTPUT_VALUE         , FLOAT)
wheeledbase.set_parameter_value(ANGVELPID_MAXOUTPUT_ID	        , ANGVELPID_MAXOUTPUT_VALUE          , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_LINVELKP_ID     , POSITIONCONTROL_LINVELKP_VALUE                         , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGVELKP_ID     , POSITIONCONTROL_ANGVELKP_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_LINVELMAX_ID    , POSITIONCONTROL_LINVELMAX_VALUE                        , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGVELMAX_ID    , POSITIONCONTROL_ANGVELMAX_VALUE                          , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_LINPOSTHRESHOLD_ID  , POSITIONCONTROL_LINPOSTHRESHOLD_VALUE                      , FLOAT)
wheeledbase.set_parameter_value(POSITIONCONTROL_ANGPOSTHRESHOLD_ID  , POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE    , FLOAT)
wheeledbase.set_parameter_value(PUREPURSUIT_LOOKAHEAD_ID        , PUREPURSUIT_LOOKAHEAD_VALUE                     , FLOAT)
wheeledbase.set_parameter_value(PUREPURSUIT_LOOKAHEADBIS_ID     , PUREPURSUIT_LOOKAHEADBIS_VALUE                      , FLOAT)
wheeledbase.save_parameters()
