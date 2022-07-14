#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

int verbose = 0;
int outflag = 0;
int argflag = 0; // 1 dump, 2 reset, 3 load calib, 4 write calib
char opr_mode[9] = {0};
char pwr_mode[8] = {0};
char datatype[256];
char senaddr[256] = "0x28";
char i2c_bus[256] = I2CBUS;
char htmfile[256];
char calfile[256];

