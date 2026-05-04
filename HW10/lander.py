import pgzrun
import serial
import pygame
import math
import random # Added for star placement

# --- SERIAL SETUP ---
ser = serial.Serial('/dev/tty.usbmodem101', 9600, timeout=0.1)

WIDTH = 1300
HEIGHT = 800

# Simple Constants
GRAVITY = 0.1
THRUST = 0.4
DRIFT_SENSITIVITY = 0.1 

target_angle = 0
current_angle = 0

# 2. Adjusted Planets
# Home planet is now smaller (radius 60)
home_p = {"pos": (150, HEIGHT - 150), "radius": 60, "color": (100, 100, 100)}
# Mars planet is moved lower (y=450 instead of 200)
mars_p = {"pos": (1000, 300), "radius": 80, "color": (200, 80, 40)}

# 3. Create Stars (Generates 100 random star positions)
stars = []
for i in range(100):
    stars.append((random.randint(0, WIDTH), random.randint(0, HEIGHT)))

ship = Actor('lander')
status = "Flying"
vx = 0
vy = 0

def reset_game():
    global status, vx, vy
    ship.pos = (home_p["pos"][0], home_p["pos"][1] - home_p["radius"] - ship.height//2)
    vx = 0
    vy = 0
    status = "Flying"

def update():
    global status, vx, vy, current_angle, target_angle

    if status != "Flying": return

    pico_thrust = False
    if ser and ser.in_waiting > 0:
        try:
            line = ser.readline().decode().strip()
            parts = line.split(',')
            if len(parts) == 2:
                if parts[0] == "0": 
                    pico_thrust = True
                
                target_angle = float(parts[1]) * 0.3
        except: pass

    current_angle += (target_angle - current_angle) * 0.1
    ship.angle = -current_angle 

    if keyboard.space or pico_thrust:
        rad = math.radians(ship.angle + 90)
        vx += math.cos(rad) * THRUST
        vy -= math.sin(rad) * THRUST

    vy += GRAVITY
    vx *= 0.99 
    
    ship.x += vx
    ship.y += vy

    for p in [home_p, mars_p]:
        dist = math.sqrt((ship.x - p["pos"][0])**2 + (ship.y - p["pos"][1])**2)
        if dist < p["radius"] + (ship.height / 2):
            speed = math.sqrt(vx**2 + vy**2)
            if p == mars_p:
                if speed < 10.0 and abs(ship.angle) < 30:
                    status = "SUCCESS!"
                else:
                    status = "CRASHED!"
            elif p == home_p and vy > 0:
                vx, vy = 0, 0
                ship.y = p["pos"][1] - p["radius"] - ship.height//2

def draw():
    screen.clear()
    screen.fill((5, 5, 20)) # Slightly darker space
    
    # 4. Draw Stars
    for star in stars:
        screen.draw.filled_circle(star, 1, (255, 255, 255))
    
    # Draw Planets
    screen.draw.filled_circle(home_p["pos"], home_p["radius"], home_p["color"])
    screen.draw.filled_circle(mars_p["pos"], mars_p["radius"], mars_p["color"])
    
    ship.draw()
    
    if status != "Flying":
        screen.draw.text(status, center=(WIDTH//2, HEIGHT//2), fontsize=80, color="white", owidth=1.5, ocolor="black")
        # Centered Replay Button
        button_rect = Rect(WIDTH//2-75, HEIGHT//2+60, 150, 50)
        screen.draw.filled_rect(button_rect, (50, 200, 50))
        screen.draw.text("REPLAY", center=button_rect.center, color="white", fontsize=40)


def on_mouse_down(pos):
    button_rect = Rect(WIDTH//2-75, HEIGHT//2+60, 150, 50)
    if status != "Flying" and button_rect.collidepoint(pos):
        reset_game()

reset_game()
pgzrun.go()