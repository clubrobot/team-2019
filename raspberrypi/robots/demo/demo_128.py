from robots.demo.demo_wheeledbase import *
from robots.R128.setup_128   import *

controller_keys.update({
    K_x: {KEYDOWN: lambda: armF.start_pump(),    KEYUP: lambda: armF.stop_pump()},
})

X_jb = 2
controller_joybuttons.update({
    X_jb: {JOYBUTTONDOWN: lambda: armF.start_pump(),    JOYBUTTONUP: lambda: armF.stop_pump()},
})


text = """
Demo 128
X => Pompe
"""
update_text(text)
controlEvent()
stop()
pygame.quit()