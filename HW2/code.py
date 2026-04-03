import board
import pwmio
import time

led = pwmio.PWMOut(board.GP16, variable_frequency = True)
led.frequency = 500
led.duty_cycle = 0

while True:
    for i in range(0, 65535, 100):
        led.duty_cycle = i
        time.sleep(0.01)
    for i in range(65535, 0, -100):
        led.duty_cycle = i
        time.sleep(0.01)