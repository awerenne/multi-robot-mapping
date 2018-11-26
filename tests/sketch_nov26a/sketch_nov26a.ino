#include <Sensors.h>
#include <constants.h>
#include <Actuators.h>
#include <Messenger.h>

#include <QTRSensors.h>
#include <SoftwareSerial.h>

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   20     // emitter is controlled by digital pin 2
#define SHARP_PIN 21
#define ID_SLAVE 0


QTRSensorsRC qtrrc((unsigned char[]) {19, 18, 17, 16, 15, 14, 13, 12},
    NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
Sensors* sensors = new Sensors(&qtrrc, NUM_SENSORS, SHARP_PIN);

Actuators* actuators = new Actuators((unsigned char[]) {2, 3, 4, 6, 7, 5});

int id_slave = 0;
SoftwareSerial port(0,1);
Messenger* messenger = new Messenger(id_slave, &port);


void setup()
{
    ;
}

bool stop_signal = false;
unsigned int cycle_counter = 0;
int current_error = 0, previous_error = 0;
bool road_left = false, road_right = false, road_front = false;
float distance_to_object = -1;
int distance_travelled = 0;

void loop()
{
    if (!stop_signal) {
        sensors->QTRRead();
        road_left = sensors->IsRoadLeft();
        road_right = sensors->IsRoadRight();

        if (road_left || road_right) {  // Intersection
            actuators->OneInch();
            road_front = sensors->IsRoadMiddle();
            byte type_intersection = TypeIntersection(road_left, road_front,
                road_right);
            String msg = String(type_intersection) + "," + String(
                actuators->GetDistance());
            messenger->Send(msg);
            msg = messenger->Receive();
            actuators->ResetDistance();
        }

        else {  // No intersection - follow line
            current_error = sensors->GetError();
            actuators->FollowLine(current_error, previous_error);
            previous_error = current_error;
            cycle_counter++;
            if (cycle_counter == 10) {  // 10 Hz
                distance_to_object = sensors->SharpRead();
                if (distance_to_object >= 0) {  // Object detected
                    String msg = "rendez-vous";
                    messenger->Send(msg);
                    msg = messenger->Receive();
                    actuators->ResetDistance();
                }
                cycle_counter = 0;
            }
        }
    }
    delay(10);  // 100 Hz
}

byte TypeIntersection(bool road_left, bool road_front, bool road_right) {
    if (road_left && road_front && road_right)
        return 0;
    else if (road_left && road_front && !road_right)
        return 1;
    else if (!road_left && road_front && road_right)
        return 2;
    else if (road_left && !road_front && road_right)
        return 3;
    else if (road_left && !road_front && !road_right)
        return 4;
    else if (road_left && !road_front && !road_right)
        return 5;
    else if (!road_left && !road_front && !road_right)
        return 6;
    return -1;  // Return undetermined intersection
}
