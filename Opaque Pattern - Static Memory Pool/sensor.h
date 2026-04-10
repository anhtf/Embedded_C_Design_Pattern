#ifndef SENSOR_H
#define SENSOR_H

struct sensor;

struct sensor *sensor_create(int pin);
void sensor_destroy(struct sensor *inst);
int sensor_read(struct sensor *inst);

#endif