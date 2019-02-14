from robots.demo_wheeledbase import *
from robots.setup_128   import *

controller_keys.update({
    K_x: {KEYDOWN: lambda: arm.start_pump(),    KEYUP: lambda: arm.stop_pump()},
})

X_jb = 2
controller_joybuttons.update({
    X_jb: {JOYBUTTONDOWN: lambda: arm.start_pump(),    JOYBUTTONUP: lambda: arm.stop_pump()},
})


text = """
Demo 128
X => Pompe
"""
update_text(text)
controlEvent()
stop()
pygame.quit()