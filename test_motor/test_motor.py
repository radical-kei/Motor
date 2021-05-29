import pigpio
import time

IN1_GPIO = 18
IN2_GPIO = 19

pi = pigpio.pi()

# Forward 20% 
# IN2 - HI80% : LO20%
pi.hardware_PWM(IN2_GPIO, 400000, 800000)
# IN1 - HI100% : LO0%
pi.hardware_PWM(IN1_GPIO, 400000, 1000000)
time.sleep(5)

# Forward 50%
# IN2 - HI50% : LO50%
pi.hardware_PWM(IN2_GPIO, 400000, 500000)
# IN1 - HI100% : LO0%
pi.hardware_PWM(IN1_GPIO, 400000, 1000000)
time.sleep(5)

# Stop
pi.hardware_PWM(IN2_GPIO, 0, 0)
pi.hardware_PWM(IN1_GPIO, 0, 0)
time.sleep(2)

# Reverse 20%
# IN1 - HI80% : LO20%
pi.hardware_PWM(IN1_GPIO, 400000, 800000)
# IN2 - HI100% : LO0%
pi.hardware_PWM(IN2_GPIO, 400000, 1000000)
time.sleep(5)

# Reverse 50%
# IN1 - HI50% : LO50%
pi.hardware_PWM(IN1_GPIO, 400000, 500000)
# IN2 - HI100% : LO0%
pi.hardware_PWM(IN2_GPIO, 400000, 1000000)
time.sleep(5)

# Stop
pi.hardware_PWM(IN1_GPIO, 0, 0)
pi.hardware_PWM(IN2_GPIO, 0, 0)

pi.stop()

