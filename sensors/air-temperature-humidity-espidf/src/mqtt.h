#ifndef MQTT_H
#define MQTT_H

void mqtt_init(void);
void mqtt_publish(const char *topic, const char *data);

#endif // MQTT_H