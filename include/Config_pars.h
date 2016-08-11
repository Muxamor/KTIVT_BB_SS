#ifndef CONFIG_PARS_H
#define CONFIG_PARS_H

#include <unistd.h>
#include <stdint.h>

typedef enum { inp_1_1, inp_1_10, inp_0V, inp_cal, z_state } input_t;

struct config_t
{
    input_t input;
    unsigned short ku1;
    unsigned short fcut;
    unsigned short ku2;
    unsigned short fd;
    unsigned short fres;
    unsigned short ID_ch;
    unsigned short SID_ch;
    unsigned short KEMS;
    unsigned short X_Y_Z;
};

struct settings_ch{
	unsigned char mode;
	unsigned char state;
	struct config_t config_ch;
};

struct settings_brd{
	char dname[256];
	char port[7];
	uint16_t sn;
	uint8_t sync_src;
	uint8_t data_recv;
	uint8_t num_file_earthquake_emul;
	uint8_t eth_type_connection;
	int fd_earthquake_emul;

};


int parse_config(FILE *fp, struct settings_brd *brd_cfg, struct settings_ch *ch_cfg, int chan_num);

#endif
