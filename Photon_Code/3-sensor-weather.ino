// -------------------------------------------------------------------------
// @3-sensor-weather.ino
//
//   Adapted from weatherstation.ino by Aaron Ardiri: 
//   http://ardiri.com/blog/building_a_weather_station_with_the_particle_io_photon
//   
// -------------------------------------------------------------------------
//  uses Particle photon and the control-everything weather sensor kit
//  http://controleverything.com/kit-4
//  Sensors: SI1132, SI7020, CPS120
// -------------------------------------------------------------------------
// configurations
// -------------------------------------------------------------------------

#define ENABLE_LOGS

#define ENABLE_TEMP 
#define ENABLE_BAR
#define ENABLE_LIGHT

unsigned long ts;
unsigned long s_ts;
#define COMMUNICATION_PERIOD 55500  // 60 second period between readings
#define SENSOR_PERIOD        5000   // 5 second interval between readings
#define HTTP_TIMEOUT         2500   // 2.5 seconds for timeout

// -------------------------------------------------------------------------

// configure the watchdog timer, is 15 seconds before check-in, reset
ApplicationWatchdog wd(15000, System.reset);

#define TEMP_ADDR  0x40
#define BAR_ADDR   0x28
#define LIGHT_ADDR 0x60


//declare the variables
  double humidity;
  double ctemp;
  double ftemp;

  double visible;
  double ir;
  double uv;

  double pressure;
  double pressureHg;


void setup() 
{
#ifdef ENABLE_LOGS
  Serial.begin(9600);

  // wait for the serial monitor to come alive
  delay(500);
  
  // print a small banner for information purposes
  Serial.println("3-sensor-weather-station");
  Serial.println("---------------");
  Serial.println();
  Serial.println("author:  MaxSym");
  Serial.println("version: " __DATE__);
  Serial.println();
#endif

  // initialise I2C communication as MASTER
  Wire.begin();  

  // 
  // https://github.com/ControlEverythingCommunity/SI7020-A20
  //

  // nothing required

  //
  // https://github.com/ControlEverythingCommunity/CPS120
  //

  // nothing required
  
  //
  // https://github.com/ControlEverythingCommunity/SI1132
  //

#ifdef ENABLE_LIGHT
  {
    int response;
  
    // enable UVindex measurements coefficients
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x13);  
    Wire.write(0x29);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x14);
    Wire.write(0x89);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x15);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x16);
    Wire.write(0x00);
    Wire.endTransmission();
    
    // enable uv, visible, ir
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);  
    Wire.write(0xF0);  // Enable uv, visible, IR
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x01 | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
    
    // INT pin driven low
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x03);
    Wire.write(0x01);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x04);
    Wire.write(0x01);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x07);
    Wire.write(0x17);
    Wire.endTransmission();
    
    // small IR photodiode, ALS_IR_ADCMUX in RAM
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x0E | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
    
    // set ADC clock divided / 1, ALS_IR_ADC_GAIN register in RAM
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x1E | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();

    // set 511 ADC clock, ALS_IR_ADC_COUNTER register in RAM  
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x70);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x1D | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
    
    // set ADC clock divided / 1, ALS_VIS_ADC_GAIN register in RAM
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x11 | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
  
    // high signal range, ALS_IR_ADC_MISC register in RAM
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x20);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x1F | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
    
    // set 511 ADC clock, ALS_VIS_ADC_COUNTER register in RAM  
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x70);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x10 | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();

    // high signal range, ALS_VIS_ADC_MISC register in RAM    
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x17);
    Wire.write(0x20);
    Wire.endTransmission();
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x18);
    Wire.write(0x12 | 0xA0);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(LIGHT_ADDR);
    Wire.write(0x2E);
    Wire.endTransmission();
    Wire.requestFrom(LIGHT_ADDR, 1);
    response = Wire.read();
  }
#endif

  delay(1000);

#ifdef ENABLE_LOGS
  // display some local network settings
  Serial.println(":: network configuration");
  Serial.println();
  
  Serial.print("  IP Address:  ");
  Serial.println(WiFi.localIP());
  Serial.print("  Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("  Gateway:   ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("  MAC address: ");
  {
    byte mac[6];
    WiFi.macAddress(mac);
    
    for (int i=0; i<6; i++)
    {
      if (mac[5-i] < 16) Serial.print("0");
      Serial.print(mac[5-i], HEX);
      if (i != 5) Serial.print(":");
    }
    Serial.println();
  }
  Serial.println();
#endif

  // we need a reference timestamp 
  ts = millis() - COMMUNICATION_PERIOD;
  s_ts = millis() - SENSOR_PERIOD;
  
#ifdef ENABLE_LOGS
    Particle.variable("humidity", humidity);
    Particle.variable("ctemp", ctemp);
    Particle.variable("ftemp", ftemp);
    Particle.variable("visible", visible);
    Particle.variable("ir", ir);
    Particle.variable("uv", uv);
    Particle.variable("pressure", pressure);
    Particle.variable("pressureHg", pressureHg);
#endif
}

void loop() 
{
  unsigned int data[4];


  // pet the watchdog timer
  wd.checkin();
  
  // we want to publish this information to our web service
  if (((millis() - s_ts) >= SENSOR_PERIOD) ||
       (millis() < s_ts)) // overflow - we cannot rely on the value anymore
  {
    // 
    // https://github.com/ControlEverythingCommunity/SI7020-A20
    //
  
#ifdef ENABLE_TEMP
    {
      double temp;
      
      // obtain humidity  
      Wire.beginTransmission(TEMP_ADDR);
      Wire.write(0xF5); // humidity, no hold master
      Wire.endTransmission();
      delay(500);
      Wire.requestFrom(TEMP_ADDR, 2);
      if (Wire.available() == 2)
      {
        data[0] = Wire.read();
        data[1] = Wire.read();
  
        humidity  = ((data[0] * 256.0) + data[3]);
        humidity = ((125 * humidity) / 65536.0) - 6;
      }
      else
        humidity = 0;
    
      // obtain temperature
      Wire.beginTransmission(TEMP_ADDR);
      Wire.write(0xF3); // temperature, no hold master
      Wire.endTransmission();
      delay(500);
      Wire.requestFrom(TEMP_ADDR, 2);
      if (Wire.available() == 2)
      {
        data[0] = Wire.read();
        data[1] = Wire.read();
  
        temp  = ((data[0] * 256.0) + data[1]);
        ctemp = ((175.72 * temp) / 65536.0) - 46.85;
        ftemp = ((ctemp * 9) / 5) + 32;
      }
      else
      {
        temp = 0;
        ctemp = 0;
      }
      
#ifdef ENABLE_LOGS
      // output data to serial monitor
      Serial.print("Relative humidity : ");
      Serial.print(humidity);
      Serial.println(" % RH");
      Serial.print("Temperature in Celsius : ");
      Serial.print(ctemp);
      Serial.println(" C");
      Serial.print("Temperature in Farenheit : ");
      Serial.print(ftemp);
      Serial.println(" F");
#endif
    }
#endif

    //
    // https://github.com/ControlEverythingCommunity/CPS120
    //
  
#ifdef ENABLE_BAR
    {
      // obtain barometric pressure
      Wire.beginTransmission(BAR_ADDR);
      Wire.write(0x80); // barometric pressure, no hold master
      Wire.endTransmission();
      delay(500);
      Wire.requestFrom(BAR_ADDR, 4);
      if (Wire.available() == 4)
      {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
  
        pressure = (((data[0] & 0x3F) * 256 + data[1]) * (90 / 16384.00)) + 30;
        //convert pressure to Hg, used in most commercial weather stations
        //kPa to hg = 1 to 0.2952998751
        pressureHg = (pressure * 0.2952998751);
      }
      else
        pressure = 0;
  
#ifdef ENABLE_LOGS
      // output data to serial monitor
      Serial.print("Pressure : ");
      Serial.print(pressure);
      Serial.println(" kPa");
      Serial.print("Pressure(hg): ");
      Serial.print(pressureHg);
      Serial.println(" Hg(in)");
#endif
    }
#endif

    //
    // https://github.com/ControlEverythingCommunity/SI1132
    //
  
#ifdef ENABLE_LIGHT
    {
      // obtain visible and ir values
      Wire.beginTransmission(LIGHT_ADDR);
      Wire.write(0x18);
      Wire.write(0x0E);
      Wire.endTransmission();
      delay(500);
      
      // obtain visible, ir values
      Wire.beginTransmission(LIGHT_ADDR);
      Wire.write(0x22);
      Wire.endTransmission();
      Wire.requestFrom(LIGHT_ADDR, 4);
      if (Wire.available() == 4)
      {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        
        visible = (data[1] * 256.0 + data[0]);
        ir = (data[3] * 256.0 + data[2]);
      }
      else
      {
        visible = 0;
        ir = 0;
      }
  
      // obtain uv value
      Wire.beginTransmission(LIGHT_ADDR);
      Wire.write(0x2C);
      Wire.endTransmission();
      Wire.requestFrom(LIGHT_ADDR, 2);
      if (Wire.available() == 2)
      {
        data[0] = Wire.read();
        data[1] = Wire.read();
        
        uv = (data[1] * 256.0 + data[0]);
      }
      else
      {
        uv = 0;
      }
  
#ifdef ENABLE_LOGS
      // output data to serial monitor
      Serial.print("Visible Light of Source: ");
      Serial.print(visible);
      Serial.println(" lux");
      Serial.print("IR Light of Source: ");
      Serial.print(ir);
      Serial.println(" lux");
      Serial.print("UV Light of Source: ");
      Serial.print(uv);
      Serial.println(" lux");
#endif
    }
#endif
  }
  
  // we want to publish this information to our web service
  if (((millis() - ts) >= COMMUNICATION_PERIOD) ||
       (millis() < ts)) // overflow - we cannot rely on the value anymore
  {
      //publish this data.
      Particle.publish("Humidity: ", String(humidity));
      Particle.publish("Temp - celsius: ", String(ctemp));
      Particle.publish("Temp - fahrenheit: ", String(ftemp));
      Particle.publish("visible light: ", String(visible));
      Particle.publish("IR: ", String(ir));
      Particle.publish("UV: ", String(uv));
      Particle.publish("Pressure : ", String(pressureHg));
      
      //flag the done time so we can reset
      ts = millis();
      #ifdef ENABLE_LOGS
      Serial.println(ts); //timing, get 60000 millis between readings
      #endif
  }
}

// -------------------------------------------------------------------------