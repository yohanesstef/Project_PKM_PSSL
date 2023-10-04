void COMPASS_init(){
  // TwoWire COMPASS_I2C = TwoWire(0);
  // COMPASS_I2C.begin();
  Wire.begin();
  cmps.init();
  cmps.enableDefault();

  Serial.println("Compass Complete...");
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  cmps.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  cmps.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void COMPASS_routine(){
  cmps.read();
  
  float heading = cmps.heading();

  Serial.print("Heading:  ");
  Serial.println(heading);
  delay(200);
}
