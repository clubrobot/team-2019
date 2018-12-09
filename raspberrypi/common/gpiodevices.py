from RPi import GPIO
import time


# 10 GPIO.BOARD

class Device:
    list_pin = [0] * 59


class Switch(Device):
    def __init__(self, input_pin, function=None, *args, **kwargs):
        if Device.list_pin[input_pin] == 0:
            self.function = function
            self.state = False
            Device.list_pin[input_pin] = 1
            self.args = args
            self.kwargs = kwargs
            self.input_pin = input_pin
            if (GPIO.getmode() != 10):
                GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.input_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            GPIO.add_event_detect(self.input_pin, GPIO.BOTH, callback=self.launch_function, bouncetime=500)
            if (GPIO.input(self.input_pin) == 0):
                self.state = True
            else:
                self.state = False
        else:
            raise RuntimeError('pin already in use')

    def launch_function(self, *args):
        time.sleep(0.001)

        if (GPIO.input(self.input_pin) == 0):
            self.state = True
        else:
            self.state = False

        if self.function != None:
            self.function(*self.args,**self.kwargs)

    def set_function(self, function, *args, **kwargs):
        self.function = function
        self.kwargs = kwargs
        self.args = args

    def close(self):
        Device.list_pin[self.input_pin] = 0
        GPIO.remove_event_detect(self.input_pin)
        GPIO.cleanup(self.input_pin)

class LightButton(Device):

    def __init__(self, input_pin, light_pin, function=None, *args, **kwargs):
        if Device.list_pin[input_pin] == 0 and Device.list_pin[light_pin] == 0:
            self.function = function
            self.state = False
            Device.list_pin[input_pin] = 1
            Device.list_pin[light_pin] = 1
            self.auto_switch = False
            self.args = args
            self.kwargs = kwargs
            self.input_pin = input_pin
            self.light_pin = light_pin
            if (GPIO.getmode() != 10):
                GPIO.setmode(GPIO.BOARD)
            GPIO.setup(self.input_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            GPIO.setup(self.light_pin, GPIO.OUT)
            GPIO.add_event_detect(self.input_pin, GPIO.FALLING, callback=self.launch_function, bouncetime=500)
        else:
            raise RuntimeError('pin already in use')

    def launch_function(self, *args):
        if self.function != None:
            self.function(*self.args,**self.kwargs)

        if self.auto_switch == True:
            self.switch()

    def set_auto_switch(self, value):
        self.auto_switch = value

    def on(self):
        self.state = True
        GPIO.output(self.light_pin, GPIO.HIGH)

    def off(self):
        self.state = False
        GPIO.output(self.light_pin, GPIO.LOW)

    def switch(self):
        if self.state == False:
            self.on()
        else:
            self.off()

    def set_function(self, function, *args, **kwargs):
        self.function = function
        self.args = args
        self.kwargs = kwargs

    def close(self):
        Device.list_pin[self.input_pin] = 0
        Device.list_pin[self.light_pin] = 0
        GPIO.remove_event_detect(self.input_pin)
        GPIO.cleanup(self.light_pin)
        GPIO.cleanup(self.input_pin)
