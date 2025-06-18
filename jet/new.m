function generate_motor_data()
  motors = {'motor1', 'motor2', 'motor3'};
  save_path = 'C:/Users/TUNAHAN/Desktop/jet/';

  for i = 1:length(motors)
    motor_name = motors{i};
    t = (1:100)';

    P0 = 101325;
    T0 = 288.15;
    gamma = 1.4;
    R = 287;

    switch i
      case 1
        pressure = P0 * (1 + 0.2*sin(0.1*t));
        temperature = T0 * (1 + 0.1*sin(0.1*t));
      case 2
        pressure = P0 * (1 + 0.2*sin(0.1*t)) + randn(size(t))*2000;
        temperature = T0 * (1 + 0.1*sin(0.1*t)) + randn(size(t))*5;
      case 3
        pressure = P0 * (1 - 0.003*t);
        temperature = T0 * (1 - 0.002*t);
    end

    velocity = sqrt(gamma * R .* temperature);
    data = [t, pressure, temperature, velocity];

    filename = strcat(save_path, motor_name, '_verisi.csv');
    csvwrite(filename, data);
    printf("File written: %s\n", filename);
  end
end

generate_motor_data();




