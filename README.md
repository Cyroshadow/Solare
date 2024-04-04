This device is a solar powered lamppost intended to function as an early flood detection warning system. The device works by using a humidity sensor to check if it is raining, 
once it has been deemed as raining, the arduino powers an ultrasonic sensor that constantly checks on the water level. If the distance between the sensor and the ground (i.e. the water level) is less 
than x amount, it sets the flood severity level to 1, if the water level is high enough, the flood severity level is 2, so on and so forth. This device is also intended send a corresponding text message about 
the flood level to everyone in the vicinity during the flood as well as turn on RGB leds with colors corresponding to the current flood severity.
