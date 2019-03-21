#include "gpioclass.h"

#define MSG(args...) printf(args)

gpioClass:: gpioClass()
{
}

gpioClass::~gpioClass()
{
	ReleaseInstance();
}

gpioClass *gpioClass::instance_ = NULL;
gpioClass *gpioClass::GetInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new gpioClass;
    }
    return instance_;
}

void gpioClass::ReleaseInstance()
{
    if (instance_ == NULL)
    {
        delete instance_;
        instance_ = NULL;
    }
}

bool gpioClass::isGpioExist(int pin)
{
	char path[64];
	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d", pin);
	if (!access(path, F_OK))
		return true;
	else
		return false;
}

int gpioClass::gpioclass_export(int pin)
{
	char buffer[64];
	int len;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
		MSG("Failed to open export for writing!\n");
		return(-1);
	}

	len = snprintf(buffer, sizeof(buffer), "%d", pin);
	if (write(fd, buffer, len) < 0) {
		MSG("Failed to export gpio!");
		return -1;
	}

	close(fd);

	return 0;
}

int gpioClass::gpioclass_unexport(int pin)
{
	char buffer[64];
	int len;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (fd < 0) {
		MSG("Failed to open unexport for writing!\n");
		return -1;
	}

	len = snprintf(buffer, sizeof(buffer), "%d", pin);
	if (write(fd, buffer, len) < 0) {
		MSG("Failed to unexport gpio!");
		return -1;
	}

	close(fd);

	return 0;
}

//dir: 0-->IN, 1-->OUT
int gpioClass::gpioclass_direction(int pin, int dir)
{
	if (!isGpioExist(pin)) gpioclass_export(pin);

	static const char dir_str[] = "in\0out";
	char path[64];
	int fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		MSG("Failed to open gpio direction for writing!\n");
		return -1;
	}

	if (write(fd, &dir_str[dir == 0 ? 0 : 3], dir == 0 ? 2 : 3) < 0) {
		MSG("Failed to set direction!\n");
		return -1;
	}

	close(fd);

	return 0;
}

//value: 0-->LOW, 1-->HIGH
int gpioClass::gpioclass_write(int pin, int value)
{
	if (!isGpioExist(pin)) gpioclass_export(pin);

	static const char values_str[] = "01";
	char path[64];
	int fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		MSG("Failed to open gpio value for writing!\n");
		return -1;
	}

	if (write(fd, &values_str[value == GPIOLEVELOW ? GPIOLEVELOW : GPIOLEVELHIGH], 1) < 0) {
		MSG("Failed to write value!\n");
		return -1;
	}

	close(fd);

	return 0;
}

int gpioClass::gpioclass_read(int pin)
{
	if (!isGpioExist(pin)) gpioclass_export(pin);

	char path[64];
	char value_str[3];
	int fd;

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) {
		MSG("Failed to open gpio value for reading!\n");
		return -1;
	}

	if (read(fd, value_str, 3) < 0) {
		MSG("Failed to read value!\n");
		return -1;
	}

	close(fd);

	return (atoi(value_str));
}

/**
 * none    表示引脚为输入，其它是中断引脚
 * rising  表示引脚为中断输入，上升沿触发
 * falling 表示引脚为中断输入，下降沿触发
 * both    表示引脚为中断输入，边沿触发
 * 0-->none, 1-->rising, 2-->falling, 3-->both
 */
int gpioClass::gpioclass_edge(int pin, int edge)
{
	if (!isGpioExist(pin)) gpioclass_export(pin);

	const char dir_str[] = "none\0rising\0falling\0both";
	int ptr;
	char path[64];
	int fd;
	switch(edge)
	{
		case 0:	ptr = 0; break;
		case 1:	ptr = 5; break;
		case 2:	ptr = 12; break;
		case 3:	ptr = 20; break;
		default: ptr = 0; break;
	}

	snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) {
		MSG("Failed to open gpio edge for writing!\n");
		return -1;
	}

	if (write(fd, &dir_str[ptr], strlen(&dir_str[ptr])) < 0) {
		MSG("Failed to set edge!\n");
		return -1;
	}

	close(fd);

	return 0;
}


#if 1
int main(int argc, char**argv)
{
	if (argc < 2) {
		fprintf(stdout, "example:\n");
		fprintf(stdout, "write: %s gpio level\n", argv[0]);
		fprintf(stdout, "read: %s gpio\n", argv[0]);
		exit(1);
	}

	gpioClass *m_gpioClass = gpioClass::GetInstance();

	if (argc == 2) {
		int gpio = atoi(argv[1]);
		printf("read gpio value: ");
		m_gpioClass->gpioclass_direction(gpio, GPIODIRIN);
		fprintf(stdout, "%d\n", m_gpioClass->gpioclass_read(gpio));
	}
	else if (argc == 3) {
		int gpio = atoi(argv[1]);
		int level = atoi(argv[2]);
		fprintf(stdout, "set gpio value: ");
		m_gpioClass->gpioclass_direction(gpio, GPIODIROUT);
		m_gpioClass->gpioclass_write(gpio, level);
		fprintf(stdout, (0 == argv[2])?"LOW Level\n":"HIGH Level\n");
	}

	return 0;
}
#endif
