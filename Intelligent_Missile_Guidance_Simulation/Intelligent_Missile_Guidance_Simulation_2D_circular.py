import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.linalg import solve_continuous_are

class KalmanFilter:
    def __init__(self, F, H, Q, R, x0, P0):
        self.F = F
        self.H = H
        self.Q = Q
        self.R = R
        self.x = x0
        self.P = P0

    def predict(self):
        self.x = self.F @ self.x
        self.P = self.F @ self.P @ self.F.T + self.Q

    def update(self, z):
        y = z - self.H @ self.x
        S = self.H @ self.P @ self.H.T + self.R
        K = self.P @ self.H.T @ np.linalg.inv(S)
        self.x = self.x + K @ y
        self.P = (np.eye(self.P.shape[0]) - K @ self.H) @ self.P

def lqr(A, B, Q, R):
    P = solve_continuous_are(A, B, Q, R)
    K = np.linalg.inv(R) @ B.T @ P
    return K

dt = 0.1
total_time = 300
steps = int(total_time / dt)

missile_state = np.array([0.0, 0.0, 0.0, 0.0])
target_state = np.array([100.0, 100.0, -3.0, 0.0])

missile_states = [missile_state.copy()]
target_states = [target_state.copy()]
missile_estimates = []
target_estimates = []

F = np.array([[1, 0, dt, 0],
              [0, 1, 0, dt],
              [0, 0, 1, 0],
              [0, 0, 0, 1]])
H = np.eye(4)

Q = np.eye(4) * 0.1
R = np.eye(4) * 2.0

kf_missile = KalmanFilter(F, H, Q, R, missile_state, np.eye(4)*10)
kf_target = KalmanFilter(F, H, Q, R, target_state, np.eye(4)*10)

A = np.array([[0, 0, 1, 0],
              [0, 0, 0, 1],
              [0, 0, 0, 0],
              [0, 0, 0, 0]])
B = np.array([[0, 0],
              [0, 0],
              [1, 0],
              [0, 1]])

Q_lqr = np.diag([300, 300, 10, 10])
R_lqr = np.eye(2) * 0.01
K_lqr = lqr(A, B, Q_lqr, R_lqr)

max_accel = 3.0
max_speed = 15.0

for i in range(steps):
    t = i * dt

    target_state[0] = 100 + 20 * np.cos(0.15 * t)
    target_state[1] = 100 + 20 * np.sin(0.15 * t)
    target_state[2] = -3 * np.sin(0.15 * t)
    target_state[3] = 3 * np.cos(0.15 * t)
    target_states.append(target_state.copy())

    error = missile_state[:4] - target_state[:4]

    u = -K_lqr @ error

    accel = np.clip(u, -max_accel, max_accel)

    missile_state[2:] += accel * dt

    speed = np.linalg.norm(missile_state[2:])
    if speed > max_speed:
        missile_state[2:] = missile_state[2:] / speed * max_speed

    missile_state[:2] += missile_state[2:] * dt

    missile_state[0] = np.clip(missile_state[0], -50, 150)
    missile_state[1] = np.clip(missile_state[1], -50, 150)

    missile_states.append(missile_state.copy())

    missile_meas = missile_state + np.random.multivariate_normal(np.zeros(4), R)
    target_meas = target_state + np.random.multivariate_normal(np.zeros(4), R)

    kf_missile.predict()
    kf_missile.update(missile_meas)
    missile_estimates.append(kf_missile.x.copy())

    kf_target.predict()
    kf_target.update(target_meas)
    target_estimates.append(kf_target.x.copy())

missile_states = np.array(missile_states)
target_states = np.array(target_states)
missile_estimates = np.array(missile_estimates)
target_estimates = np.array(target_estimates)

fig, ax = plt.subplots()
ax.set_xlim(-20, 140)
ax.set_ylim(-20, 140)
missile_line, = ax.plot([], [], 'b-', lw=2, label="Missile (True)")
target_line, = ax.plot([], [], 'g--', lw=2, label="Target (True)")
missile_est_line, = ax.plot([], [], 'r:', lw=2, label="Missile (KF Estimate)")
target_est_line, = ax.plot([], [], 'm:', lw=2, label="Target (KF Estimate)")
missile_point, = ax.plot([], [], 'bo')
target_point, = ax.plot([], [], 'go')

def init():
    missile_line.set_data([], [])
    target_line.set_data([], [])
    missile_est_line.set_data([], [])
    target_est_line.set_data([], [])
    missile_point.set_data([], [])
    target_point.set_data([], [])
    return missile_line, target_line, missile_est_line, target_est_line, missile_point, target_point

def update(frame):
    missile_line.set_data(missile_states[:frame, 0], missile_states[:frame, 1])
    target_line.set_data(target_states[:frame, 0], target_states[:frame, 1])
    missile_est_line.set_data(missile_estimates[:frame, 0], missile_estimates[:frame, 1])
    target_est_line.set_data(target_estimates[:frame, 0], target_estimates[:frame, 1])
    missile_point.set_data([missile_states[frame, 0]], [missile_states[frame, 1]])
    target_point.set_data([target_states[frame, 0]], [target_states[frame, 1]])
    return missile_line, target_line, missile_est_line, target_est_line, missile_point, target_point

ani = FuncAnimation(fig, update, frames=len(missile_estimates),
                    init_func=init, blit=True, interval=50)

plt.title("Smooth Missile Guidance Simulation")
plt.xlabel("X Position")
plt.ylabel("Y Position")
plt.legend()
plt.grid(True)
plt.show()
