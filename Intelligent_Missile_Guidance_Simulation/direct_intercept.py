import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

dt = 0.1
T = 60
steps = int(T / dt)

def target_motion(t):
    x = 50 * np.sin(0.1 * t) + 0.5 * t
    y = 30 * np.cos(0.07 * t) + 0.3 * t
    return np.array([x, y])

sensor_noise_std = 1.5

missile_pos = np.array([0.0, 0.0])
missile_speed = 6.5

missile_positions = []
target_positions = []
measured_positions = []

fig, ax = plt.subplots(figsize=(10, 6))
ax.set_xlim(-20, 80)
ax.set_ylim(-50, 50)
ax.set_title("Missile vs Target (Direct Intercept)")
ax.set_xlabel("X (m)")
ax.set_ylabel("Y (m)")
ax.grid()

true_path_line, = ax.plot([], [], 'b-', label="Target Path")
measured_path_line, = ax.plot([], [], 'r.', alpha=0.5, label="Sensor Data")
missile_path_line, = ax.plot([], [], 'g-', label="Missile Path")
missile_point, = ax.plot([], [], 'go', label="Missile")
target_point, = ax.plot([], [], 'bo', label="Target")

ax.legend()

def init():
    true_path_line.set_data([], [])
    measured_path_line.set_data([], [])
    missile_path_line.set_data([], [])
    missile_point.set_data([], [])
    target_point.set_data([], [])
    return true_path_line, measured_path_line, missile_path_line, missile_point, target_point

def update(frame):
    global missile_pos

    t = frame * dt
    target_pos = target_motion(t)
    measurement = target_pos + np.random.normal(0, sensor_noise_std, size=2)

    direction = measurement - missile_pos
    distance = np.linalg.norm(direction)
    if distance != 0:
        direction = direction / distance
        missile_pos += missile_speed * direction * dt

    missile_positions.append(missile_pos.copy())
    target_positions.append(target_pos.copy())
    measured_positions.append(measurement.copy())

    if np.linalg.norm(missile_pos - target_pos) < 2:
        print(f"Target hit at time {t:.2f} seconds!")
        anim.event_source.stop()

    true_path_line.set_data(np.array(target_positions)[:,0], np.array(target_positions)[:,1])
    measured_path_line.set_data(np.array(measured_positions)[:,0], np.array(measured_positions)[:,1])
    missile_path_line.set_data(np.array(missile_positions)[:,0], np.array(missile_positions)[:,1])
    missile_point.set_data([missile_pos[0]], [missile_pos[1]])
    target_point.set_data([target_pos[0]], [target_pos[1]])

    return true_path_line, measured_path_line, missile_path_line, missile_point, target_point

anim = FuncAnimation(fig, update, frames=steps, init_func=init, blit=True, interval=dt*1000, repeat=False)
plt.show()
