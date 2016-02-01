#ifndef CONFIG_PARS_H
#define CONFIG_PARS_H

typedef enum { inp_1_1, inp_1_10, inp_0V, inp_cal, z_state } input_t;

struct config_t
{
    input_t input;
    unsigned short ku1;
    unsigned short fcut;
    unsigned short ku2;
    unsigned short fd;
    unsigned short fres;
};

struct settings_ch{
	unsigned char mode;
	unsigned char state;
	struct config_t config_ch;
};

int parse_config(FILE *fp, struct settings_ch *cfg, int chan_num);


#endif
