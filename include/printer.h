#ifndef __PRINTER_H__
#define __PRINTER_H__

#define PRINTER_PORT_FILE "/dev/ttyS7"

extern int printer_port;

int init_printer();

int printer_print(const char * data, int length);


#endif