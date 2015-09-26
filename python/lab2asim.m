function data = lab2asim()
  % Use Kt for calculating torque from measured current.
  global Kt
  sys = system(); % Get the model for the single link arm.
  % Go from 0-60 degrees (goal is zero velocity).
  x0 = [0; 0];
  goal = [60 * pi / 180; 0];
  % Get the data itself for the PID loop.
  data = goto(sys, x0, goal);
  % Go 60-0 now.
  x0 = goal;
  goal = [0; 0];
  data = [data; goto(sys, x0, goal)];
  % Pull out the data we want to graph.
  pos = data(:, 1);
  volts = data(:, 3) / 10; % Scale by 10 to fit nicely on graph.
  setpoint = data(:, 4);
  data = [pos, volts, setpoint];
  plot(data);
  legend('Theta', 'Voltage/10', 'Setpoint');
  xlabel('Time (Hundredths of a Second)');
  ylabel('Position (Radians), Voltage (tens of Volts)');
  title('Tuned Simulated Step Response, 0 - 60 degrees');
  xlim([0 300]);
  ylim([-1.5 1.5]);
  uiwait(); % Wait for the graph to be closed.
  % Run the model using provided current data.
  current = csvread('part9.csv', 880, 3, [880 3 950 3]);
  kt = Kt / 1000;
  torque = -kt * current;
  data = runsimt(sys, [0; 0], torque);
  pos = data(:, 1)
  plot(pos);
  legend('Theta');
  xlabel('Time (Hundredths of a Second)')
  ylabel('Position (Radians)')
  title('Predicted position of arm from motor current data')
end

function sysd = system()
  % x = [theta; thetadot];
  % alpha * J = Torque = I * Kt
  % V_motor = I * R + omega / Kv
  % Kt = Stall Torque / Stall Current
  % R = Voltage / Stall Current
  % Kv = Free Speed / (Voltage - Free Current * R)
  % Calculate motor parameters.
  stall_torque = 0.6;
  stall_cur = 2.4;
  % To rad/sec.
  free_speed = 82 * 2 * pi / 60;
  free_current = 0.15;
  rated_volts = 6;
  global Kt
  Kt = stall_torque / stall_cur;
  R = rated_volts / stall_cur;
  Kv = free_speed / (rated_volts - free_current * R);
  global J
  % In theory, J is supposed to be the 0.0012 number.
  % In tuning the model, multiplying the J value is to account for inaccuracies.
  J = 0.00126271845 * 80; % kg * m^2
  % alpha = I * Kt / J
  % I = (V - omega / Kv) / R
  % alpha = (V - omega / Kv) * Kt / (J * R)
  common = Kt / J * R
  dampening = 10; % Add constants dampening proportional to velocity (for tuning).
  A_cont = [0, 1; 0, -common / Kv - dampening]
  B_cont = [0; common]
  C = [1, 0]
  D = [0]
  sys = ss(A_cont, B_cont, C, D)
  sysd = c2d(sys, 0.01)
end

% Calculate the actual acceleration due to gravity.
function accel = grav_accel(x)
  m = 0.05; % kg
  cg = 0.06; % meters
  g = 9.8; % m/s/s
  J = 0.010;
  torque = -m * g * cg * cos(x(1));
  accel = torque / J;
end

% Perform same gravity compensation as actual code.
function ug = getgravcomp(x)
  holdzero = 1.34;
  theta = abs(x(1));
  ug = holdzero * (pi/2 - theta) * 2 / pi;
end

% Do PID calculation.
function u = getpid(x, setpoint, reseti)
  kp = 780 * 180 / pi / 1000;
  ki = 5.1 * 180 / pi / 1000;
  kd = 0.06 * 180 / pi / 1000;
  persistent itotal;
  if or(isempty(itotal), reseti)
    itotal = 0;
  end
  err = setpoint(1) - x(1);
  % In theory, this derr should be exactly right, as it uses
  % the velocity from the state.
  derr = setpoint(2) - x(2);
  itotal = itotal + err * 0.01
  u = kp * err + ki * itotal + kd * derr + getgravcomp(x);
  u = min(6, max(-6, u)); % Clip to +/- Vcc.
end

% Perform one iteration of simulation, using a provided
% control input.
function x1 = simiter(sys, xm1, u)
  [a, b] = ssdata(sys);
  gaccel = grav_accel(xm1);
  dt = 0.01;
  gdvel = gaccel * dt;
  gdp = 0.5 * gdvel * dt;
  gdx = [gdp; gdvel];
  x1 = a * xm1 + b * u + gdx;
end

% Perform a simulation step using an output torque.
% For use with torque data.
function x1 = simitertorque(sys, xm1, t)
  global J
  moi = J;
  gaccel = grav_accel(xm1);
  dt = 0.01;
  gdvel = gaccel * dt;
  gdp = 0.5 * gdvel * dt;
  gdx = [gdp; gdvel];
  accel = t / J;
  dvel = accel * dt;
  dp = 0.5 * dvel * dt;
  dx = [dp; dvel];
  x1 = xm1 + dx + gdx;
end

% Run single iteration of PID loop.
function [x1, u] = runiter(sys, xm1, setpoint, reset)
  u = getpid(xm1, setpoint, reset);
  % Return cells due to matrix dimension mis-matches.
  x1 = {simiter(sys, xm1, u)};
  u = {u};
end

% Run full simulation with a set of control-input data.
function x = runsimu(sys, x0, udata)
  x = []
  for u = udata
    xm1 = x(end)
    x1 = simiter(sys, transpose(xm1), u)
    x = [x; transpose(x1)]
  end
end

% Run full simulation with a set of torque data.
function x = runsimt(sys, x0, tdata)
  x = [transpose(x0)]
  tdata = transpose(tdata);
  for t=tdata
    xm1 = x(end);
    x1 = simitertorque(sys, transpose(xm1), t);
    x = [x; transpose(x1)];
  end
end

% Run full PID simulation with provided initial position/setpoint.
function data = goto(sys, x0, setpoint)
  % Run initial iteration with reset=true.
  [x1, u] = runiter(sys, x0, setpoint, true);
  x1 = cell2mat(x1);
  u = cell2mat(u);
  % Note: have to transpose all x-vectors so that data's dimensions are nice.
  data = [[transpose(x0), [0], transpose(setpoint(1))]; [transpose(x1), u, transpose(setpoint(1))]];
  for i = 1:200
    xm1 = data(end, 1:2);
    [x, u] = runiter(sys, transpose(xm1), setpoint, false);
    % Extract matrices from cells.
    x = cell2mat(x);
    u = cell2mat(u);
    data = [[data]; [transpose(x), u, transpose(setpoint(1))]];
  end
end
