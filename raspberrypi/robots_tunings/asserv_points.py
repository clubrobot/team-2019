#!/usr/bin/env python3
#-*- coding: utf-8 -*-
import math 
pi = math.pi

normal_speed = [# t  | vel
			(    0,    0,  ),
			(    3,  400,  ),
			(    1,    0,  ),
			(    3, -400,  ),
			(    1,    0,  )]

fast_speed = [# t  | vel
			(    0,    0,  ),
			(    3,  600,  ),
			(    1.5,    0,  ),
			(    3, -600,  ),
			(    1,    0,  )]

slow_rotation = [# t  | vel
			(    0,    0,  ),
			(    3,  1.5,  ),
			(    1,    0,  ),
			(    3, - 1.5,  ),
			(    1,    0,  )]

fast_rotation = [# t  | vel
			(    0,    0,  ),
			(    3,  6,  ),
			(    1,    0,  ),
			(    3, - 6,  ),
			(    1,    0,  )]

square = [# x  | y | theta
		(    500,    0, 0 ),
		(    500,  0, pi/2),
		(    500,  500,  pi/2),
		(    500,  500,  pi),
		(    0,    500,  pi),
		(    0,    500,  -pi/2),
		(    0,    0,  -pi/2),
		(    0,    0,  0)]

round_trip_no_turn = [# x  | y | theta
		(    800,    0, 0 ),
		(    0, 0, 0)]

round_trip = [# x  | y | theta
		(    800,    0, 0 ),
		(    800, 0, pi),
		(    0, 0, pi),
		(    0, 0, 0)]



linvel_cmd = normal_speed
angvel_cmd = fast_rotation
objpoints_cmd = round_trip_no_turn