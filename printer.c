#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "printer.h"

int printer_port;

int init_printer() {
    // open the serial port device
    printer_port = open(PRINTER_PORT_FILE, O_RDWR);
    if (printer_port < 0) {
        fprintf(stderr, "Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    // Read in existing tty settings
    struct termios tty;
    if (tcgetattr(printer_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    tty.c_cflag &= ~PARENB;     // no parity bit
    tty.c_cflag &= ~CSTOPB;     // one stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;    // disable RTS/CTS
    tty.c_cflag |= CLOCAL;      // disable modem type
    tty.c_cflag |= CREAD;       // turn on read

    tty.c_lflag &= ~ICANON;     // disable canonical mode
    tty.c_lflag &= ~ECHO;       // Disable echo
    tty.c_lflag &= ~ECHOE;      // Disable erasure
    tty.c_lflag &= ~ECHONL;     // Disable new-line echo
    tty.c_lflag &= ~ISIG;       // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                             // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);    // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST;      // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR;      // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10;       // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    cfsetspeed(&tty, B9600);    // set baud rate

    // Save tty settings
    if (tcsetattr(printer_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    return 0;
}

void close_printer() {
    close(printer_port);
}

int printer_print(const char * data, int length) {
    write(printer_port, data, length);
    return length;
}