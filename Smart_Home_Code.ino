#define TRIGGER_PIN  12                            // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13                            // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200                           // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. [this is an arbitrary number]
#define ultraled_pin 11
#define infra 5
#define buzzer 6
int ldr=A0;
int ldr_led=7;

#include "NewPing.h"
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);// NewPing setup of pins and maximum distance.


unsigned int critical_distance_cms = 15;           // Cutoff distance at which the light will switch [this is an arbitrary number]
bool state = 0;

void setup() {
  Serial.begin(9600);                              // Open serial monitor at 115200 baud to see ping results.
  pinMode(ultraled_pin, OUTPUT);
  pinMode(infra,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(ldr_led,OUTPUT);
  digitalWrite(ultraled_pin, HIGH);                // Turn the light off
}

void loop() {
  delay(50);                                       // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int distance = readDistance();          // Current distance of any object facing the ultrasonic sensor

  Serial.print("Ultrasonic: ");
  Serial.print(distance);                          // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  int data= analogRead(ldr);
  Serial.println("");
  Serial.print("Sensor Value=");
  Serial.println(data);


  if (data<=50)
  {
    digitalWrite(ldr_led,HIGH);
  }
  else
  {
    digitalWrite(ldr_led,LOW);
  }
   
   int value= digitalRead(infra);
  Serial.println("");
  Serial.print("Sensor Value=");
  Serial.println(value);


  if (value==1)
  {
    digitalWrite(buzzer,HIGH);

  }
  else
  {
    digitalWrite(buzzer,LOW);
  }
  // Someone is near the door
  if (distance < critical_distance_cms)
  {
    while (distance < critical_distance_cms)
    {
      // Check if they moved away
      distance = readDistance();

      delay(5);                                     // Do nothing until the person moves away from the door
    }

    state = !state;                                 // Change the state of the relay

    if (state)
    {
      Serial.println("Door Open!");
      digitalWrite(ultraled_pin, LOW);             // Turn the light on
    }
    else
    {
      Serial.println("Door Closed!");
      digitalWrite(ultraled_pin, HIGH);            // Turn the light off
    }
  }
  
}

// Updates the value of the Ultrasonic reading
unsigned int readDistance()
{
  // Read 7 values from the ultrasonic and get the median value ( median filter )
  // Gets rid of noisy reading
  unsigned int distance = sonar.convert_cm(sonar.ping_median(7));

  // The value 0 indicates that the ultrasonic sensor is reading nothing in front of it
  // Set this distance to max distance so the light doesn't switch unnecessarily
  if (distance == 0)
  {
    distance = MAX_DISTANCE;
  }
  
  return distance;
  
}
