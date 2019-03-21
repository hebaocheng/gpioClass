
#ifndef __GPIOCLASS_API_H_
#define __GPIOCLASS_API_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define GPIODIRIN  0
#define GPIODIROUT 1

#define GPIOLEVELHIGH 1
#define GPIOLEVELOW   0

class gpioClass {
private:
	gpioClass();
	virtual ~gpioClass();

	bool isGpioExist(int pin);

	int gpioclass_export(int pin);
	int gpioclass_unexport(int pin);
private:
	static gpioClass *instance_;

public:
	static gpioClass *GetInstance();
	static void ReleaseInstance();

	int gpioclass_direction(int pin, int dir);
	int gpioclass_write(int pin, int value);
	int gpioclass_read(int pin);
	int gpioclass_edge(int pin, int edge);
};


#endif // __GPIOCLASS_API_H_
