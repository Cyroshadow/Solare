//#include <MKRGSM.h>
#include <SoftwareSerial.h>
#include <DHT11.h>


//Assign pin numbers to variables for convenience of calling it in functions
const byte echo_Pin = 13;
const byte trig_Pin = 12;
DHT11 dht11(11);
const byte ult_Sen_Pin = 10;
const byte siren_Severity_1 = 9;
const byte siren_Severity_2 = 8;
const byte siren_Severity_3 = 7;
const byte red = A2;
const byte blue = A0;
const byte green = A1;
SoftwareSerial gsmMod(3, 2);

//Assign variables constants for use ibn logic and whatnot
const int flood_Level_1 = 30; //Distance between water and ultsen to determine flood level
const int flood_Level_2 = 20; //Distance between water and ultsen to determine flood level
const int flood_Level_3 = 10; //Distance between water and ultsen to determine flood level
const float sound_Speed = 0.0343; //Initialize for use in get_Distance() function
const float normal_Humidity = 50; //This variable is what we will compare the relative humidity to, to determine if it is raining
const float normal_Temperature = 27; //This variable is what we will compare the relative temp to, to determine if it is raining
String test_Number = "+639948033248"; //Phone number ni Ash; For debugging purposes
bool rain = false; //This variable is the placeholder for the condition when the device will assume its raining and start running the flood level detection processes
//Basically, "if raining then check if flooding"
int flood_Severity = 0;
float pulse_Delay; //Initialize for use in get_Distance() function
float distance; //Initialize for use in get_Distance() function
float humidity; //Initialize for use in get_Humidity() function
float temperature;
char flood_Message_Light[] = "The flood is 4in deep. Please stay alert for evacuation";
char flood_Message_Moderate[] = "The flood is 8in deep. Please stay alert for evacuation";
char flood_Message_Severe[] = "The flood is 8in deep. Please evacuate";


void setup () {

  pinMode(ult_Sen_Pin, OUTPUT);
  pinMode(echo_Pin, INPUT);
  pinMode(trig_Pin, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
  Serial.print("Hello");
  gsmMod.begin(9600);
  rgb(255, 0, 0);

  gsmMod.println("AT+CMGF=1"); // Configuring TEXT mode
  gsmMod.println("AT+CMGS=\"+639948033248\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
}

void loop () {

  
  get_Humidity(); //Get humidity 

  if (rain) {
    digitalWrite(ult_Sen_Pin, HIGH);
    get_Distance(50); //Get water level by computing for the distance between ultrasonic sensor and the water level
    if (distance <= flood_Level_3) {
    flood_Severity = 3;
  } else if (distance <= flood_Level_2) {
    flood_Severity = 2;
  } else if (distance <= flood_Level_1) {
    flood_Severity = 1;
  } else {
    flood_Severity = 0;
  }
  } else {
    digitalWrite(ult_Sen_Pin, LOW);
    flood_Severity = 0;
  }
  


  switch (flood_Severity) {

    //Uncomment send_SMS functions when doing final testing
    case 0:
      Serial.println("No flood hooray");
      digitalWrite(siren_Severity_1, LOW);
      digitalWrite(siren_Severity_2, LOW);
      digitalWrite(siren_Severity_3, LOW);
      rgb(255, 255, 255);
    break;

    case 1:
      //send_SMS(test_Number, flood_Message_Light);
      digitalWrite(siren_Severity_1, HIGH); 
      Serial.println("Level 1 Flood");
      rgb(0, 255, 0);
    break;

    case 2:
      //send_SMS(test_Number, flood_Message_Moderate);
      digitalWrite(siren_Severity_2, HIGH); 
      Serial.println("Level 2 Flood");
      rgb(255, 180, 0);
    break;

    case 3:
      //send_SMS(test_Number, flood_Message_Severe);
      digitalWrite(siren_Severity_3, HIGH); 
      Serial.println("Level 3 Flood");
      rgb(255, 0, 0);
    break;

    default:
      digitalWrite(siren_Severity_1, LOW);
      digitalWrite(siren_Severity_2, LOW);
      digitalWrite(siren_Severity_3, LOW);
    break;
  }

}

void get_Distance(int pulse_Length) {

  digitalWrite(trig_Pin, LOW); //Initialize trigger pin
  delay(50);
  digitalWrite(trig_Pin, HIGH); //Begin sending pulse
  delay(pulse_Length); //Increase length of pulse for accurate readings
  digitalWrite(trig_Pin, LOW); //Stop sending pulse

  pulse_Delay = (pulseIn(echo_Pin, HIGH))/2; //Get the time it took for the pulse to get from the ultrasonic sensor to object
  distance = (pulse_Delay)*sound_Speed; //Compute for distance using formula speed*time
  Serial.println(distance);
}

void get_Humidity() {
  
  humidity = dht11.readHumidity();
  temperature = dht11.readTemperature();

  if (humidity >= normal_Humidity && temperature <= normal_Temperature) {  //When debugging change to or instead of and
    rain = true;
  } else {
    rain = false;
  }
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature: "); 
  Serial.println(temperature);
  rain = true;
}

void send_SMS(String recipient_Num, String message) {
  gsmMod.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  gsmMod.println("AT+CMGS=\"+639948033248\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  gsmMod.print(message); //text content
  updateSerial();
  gsmMod.write(26);
}

void rgb(int red_Val, int green_Val, int blue_Val ) {
  analogWrite(red, red_Val);
  analogWrite(blue, blue_Val);
  analogWrite(green, green_Val);
}

void updateSerial()
{
  //delay(500);
  while (Serial.available()) 
  {
    gsmMod.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(gsmMod.available()) 
  {
    Serial.write(gsmMod.read());//Forward what Software Serial received to Serial Port
  }
}

/*
  _   _       _         _                         
 | \ | |     | |       | |                        
 |  \| | _ | |_ _  | |_ _                   
 | . ` |/ _ \| __/ _ \ | __/ _ \                  
 | |\  | () | ||  __/ | || () |                 
 |_| \_|\___/ \__\___|  \__\___/                  
   __       _                        _            
  / _|     | |                      | |           
 | |_ _   | | _   _ _ _ __    _| | ____   __
 |  _| | | | __| | | | '__/ _ \  / _` |/ _ \ \ / /
 | | | |_| | |_| |_| | | |  __/ | (_| |  __/\ V / 
 |_|  \__,_|\__|\__,_|_|  \___|  \__,_|\___| \_/  


some essential functions:

GSM gsmAccess - base class for all GSM functions.
GSM_SMS sms - base class for all GSM functions for SMS.
gsmAccess.begin(pin) - connects to the GSM network with the pin number as a parameter, e.g. 0123.
sms.beginSMS(number); - creates an SMS for a specific number.
sms.print(message); - prints the content of the SMS.
sms.endSMS() - sends the SMS.                                                  
*/