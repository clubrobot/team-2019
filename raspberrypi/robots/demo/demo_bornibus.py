from robots.demo.demo_wheeledbase import *
from robots.Bornibus.setup_bornibus import *

controller_keys.update({
    K_x: {KEYDOWN: lambda: gripper.close(),    KEYUP: lambda: gripper.open()},
    K_a: {KEYDOWN: lambda: arm.deploy(),       KEYUP: lambda: arm.up()},
    K_b: {KEYDOWN: lambda: pushers.down(),     KEYUP: lambda: pushers.up()},
})

X_jb = 2
A_jb = 0
B_jb = 1
controller_joybuttons.update({
    X_jb: {JOYBUTTONDOWN: lambda: gripper.close(),    JOYBUTTONUP: lambda: gripper.open()},
    A_jb: {JOYBUTTONDOWN: lambda: arm.deploy(),       JOYBUTTONUP: lambda: arm.up()},
    B_jb: {JOYBUTTONDOWN: lambda: pushers.down(),     JOYBUTTONUP: lambda: pushers.up()},
})


text = """
Demo Bornibus
X => Pince
A => Bras
B => Pousseurs
"""
update_text(text)
controlEvent()
stop()
pygame.quit()