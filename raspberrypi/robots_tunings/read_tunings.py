from robots.setup_wheeledbase import *

print("------------------------------ Stored constant on the robot ----------------------------------")


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
print("ANGVELPID_MAXOUTPUT_VALUE             = " + str(wheeledbase.get_parameter_value(ANGVELPID_MAXOUTPUT_ID, FLOAT)))

print("POSITIONCONTROL_LINVELKP_VALUE        = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELKP_ID, FLOAT)))
print("POSITIONCONTROL_ANGVELKP_VALUE        = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELKP_ID, FLOAT)))

print("POSITIONCONTROL_LINVELMAX_VALUE       = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINVELMAX_ID, FLOAT)))
print("POSITIONCONTROL_ANGVELMAX_VALUE       = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGVELMAX_ID, FLOAT)))

print("POSITIONCONTROL_LINPOSTHRESHOLD_VALUE = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_LINPOSTHRESHOLD_ID, FLOAT)))
print("POSITIONCONTROL_ANGPOSTHRESHOLD_VALUE = " + str(wheeledbase.get_parameter_value(POSITIONCONTROL_ANGPOSTHRESHOLD_ID, FLOAT)))

print("PUREPURSUIT_LOOKAHEAD_VALUE           = " + str(wheeledbase.get_parameter_value(PUREPURSUIT_LOOKAHEAD_ID,    FLOAT)))
print("PUREPURSUIT_LOOKAHEADBIS_VALUE        = " + str(wheeledbase.get_parameter_value(PUREPURSUIT_LOOKAHEADBIS_ID, FLOAT)))


print("------------------------------------------------------------------------------------------")