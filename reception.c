/*
 *  Copyright (C) 2008 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version 0.1
 *  Author: Octavio Benedi Sanchez
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <termios.h> /* Terminal control library (POSIX) */

#define MAX 120
int sd = 3;
    char *serialPort = "";
    char *serialPort0 = "/dev/ttyS0";
    char *serialPort1 = "/dev/ttyS1";
    char *USBserialPort0 = "/dev/ttyUSB0";
    char *USBserialPort1 = "/dev/ttyUSB1";
    char *USBserialPort2 = "/dev/ttyUSB2";
    char *USBserialPort3 = "/dev/ttyUSB3";
    char *USBserialPort4 = "/dev/ttyUSB4";
    char *USBserialPort5 = "/dev/ttyUSB5";
    char aux[MAX] = "";
	char mac[20] = "";
	int battery,luminosity,pressure,temperature,watermark,x,y,z;
    char c;
    struct termios opciones;
    char *s0 = "S0";
    char *s1 = "S1";
    char *u0 = "USB0";
    char *u1 = "USB1";
    char *u2 = "USB2";
    char *u3 = "USB3";
    char *u4 = "USB4";
    char *u5 = "USB5";

    int speed = B19200;
	
void getFrame() {
   char c;
   int j = 0;

   //Get frame char by char untill '\n'
   while (1)
   {
      read(sd,&c,1);
      if((c=='\0') || (c=='\r'))
      {
         //Strange chars will be modified
         aux[j]='_';
      }
      else
      {
         aux[j]=c;
         j++;
      }

      if((c=='\n') || (j==(MAX-1)))
      {
         aux[j]='\0';
         break;
      }
   }
}

void parseFrame() {
char order[120] = "";

//printf("%s\n",aux);
sscanf(aux,"%*s mac:%16s bat:%d x:%d y:%d z:%d temp:%d pres:%d moist:%d lum:%d %*s", mac, &battery, &x,&y,&z,&temperature , &pressure , &watermark , &luminosity);
printf("mac:%s\n",mac);
printf("battery:%d\n",battery);
printf("acc_x:%d\n",x);
printf("acc_y:%d\n",y);
printf("acc_z:%d\n",z);
printf("temperature:%d\n",temperature);
printf("pressure:%d\n",pressure);
printf("watermark:%d\n",watermark);
printf("luminosity:%d\n",luminosity);
sprintf(order,"python script.py -b %d -l %d -m %s -p %d -t %d -w %d -x %d -y %d -z %d",battery,luminosity,mac,pressure,temperature,watermark,x,y,z);
system(order);
}

int main(int argc, char *argv[]) {
    

    typedef struct {
        char *name;
        int flag;
    } speed_spec;

    speed_spec speeds[] ={
        {"1200", B1200},
        {"2400", B2400},
        {"4800", B4800},
        {"9600", B9600},
        {"19200", B19200},
        {"38400", B38400},
        {"57600", B57600},
        {"115200", B115200},
        {NULL, 0}
    };

    if ((argc != 2) && (argc != 3)) {
        fprintf(stderr, "Usage: %s port\nOr: %s port speed\nDefault speed value is 19200\nAllowed ports: S0 S1 USB0 USB1 USB2 USB3 USB4 USB5\nAllowed speed values: 1200 2400 4800 9600 19200 38400 57600 115200\n", argv[0], argv[0]);
        fprintf(stderr, "Author: Octavio Benedi Sanchez\n");
        exit(0);
    }
    if (argc == 3) {
        speed_spec *s;
        for (s = speeds; s->name; s++) {
            if (strcmp(s->name, argv[2]) == 0) {
                speed = s->flag;
                //fprintf(stderr, "setting speed %s\n", s->name);
                break;
            }
        }
    }

    if (!strcmp(argv[1], s0)) {
        //fprintf(stderr,"ttyS0 chosen\n...");
        serialPort = serialPort0;
    }
    if (!strcmp(argv[1], s1)) {
        //fprintf(stderr,"ttyS1 chosen\n...");
        serialPort = serialPort1;
    }
    if (!strcmp(argv[1], u0)) {
        //fprintf(stderr,"ttyUSB0 chosen\n...");
        serialPort = USBserialPort0;
    }
    if (!strcmp(argv[1], u1)) {
        //fprintf(stderr,"ttyUSB1 chosen\n...");
        serialPort = USBserialPort1;
    }
    if (!strcmp(argv[1], u2)) {
        //fprintf(stderr,"ttyUSB2 chosen\n...");
        serialPort = USBserialPort2;
    }
    if (!strcmp(argv[1], u3)) {
        //fprintf(stderr,"ttyUSB3 chosen\n...");
        serialPort = USBserialPort3;
    }
    if (!strcmp(argv[1], u4)) {
        //fprintf(stderr,"ttyUSB4 chosen\n...");
        serialPort = USBserialPort4;
    }
    if (!strcmp(argv[1], u5)) {
        //fprintf(stderr,"ttyUSB5 chosen\n...");
        serialPort = USBserialPort5;
    }
    if (!strcmp(serialPort, "")) {
        fprintf(stderr, "Choose a valid port (S0, S1, USB0, USB1, USB2, USB3, USB4, USB5)\n");
        exit(0);
    }

    if ((sd = open(serialPort, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1) {
        fprintf(stderr, "Unable to open the serial port %s - \n", serialPort);
        exit(-1);
    } else {
        if (!sd) {
            /*Sometimes the first time you call open it does not return the
             * right value (3) of the free file descriptor to use, for this
             * reason you can set manually the sd value to 3 or call again
             * the open function (normally returning 4 to sd), advised!*/
            sd = open(serialPort, O_RDWR | O_NOCTTY | O_NONBLOCK);
        }
        //fprintf(stderr,"Serial Port open at: %i\n", sd);
        fcntl(sd, F_SETFL, 0);
    }
    tcgetattr(sd, &opciones);
    cfsetispeed(&opciones, speed);
    cfsetospeed(&opciones, speed);
    opciones.c_cflag |= (CLOCAL | CREAD);
    /*No parity*/
    opciones.c_cflag &= ~PARENB;
    opciones.c_cflag &= ~CSTOPB;
    opciones.c_cflag &= ~CSIZE;
    opciones.c_cflag |= CS8;
    /*raw input:
     * making the applycation ready to receive*/
    opciones.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /*Ignore parity errors*/
    opciones.c_iflag |= ~(INPCK | ISTRIP | PARMRK);
    opciones.c_iflag |= IGNPAR;
    opciones.c_iflag &= ~(IXON | IXOFF | IXANY | IGNCR | IGNBRK);
    opciones.c_iflag |= BRKINT;
    /*raw output
     * making the applycation ready to transmit*/
    opciones.c_oflag &= ~OPOST;
    /*aply*/
    tcsetattr(sd, TCSANOW, &opciones);
    int j = 0;
    while (1) {
		getFrame();
		parseFrame();
    }
    close(sd);
}
