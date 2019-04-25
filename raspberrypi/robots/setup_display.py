from daughter_cards.display import *
from robots.display_manager import *
from robots.setup_serialtalks import *

led1 = LEDMatrix(manager, 1)
led2 = LEDMatrix(manager, 2)
ssd = SevenSegments(manager)

disp = DisplayPoints(ssd, led1, led2)
