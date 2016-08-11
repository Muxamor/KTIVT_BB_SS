#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "../include/main.h"
#include "../include/Config_pars.h"

static char *skip_spaces(char *str)
{
    char accept[] = " \t\n";
    int offset = strspn(str, accept);
    return str + offset;
}

void copy_token(char *dst, char *src, int maxlen)
{
    char reject[] = " \t";
    //size_t
	unsigned int offs = strcspn(src, reject);
    memcpy(dst, src, offs);
    dst[offs] = '\0';
}

int get_channel( char *str)
{
    long int channel;
    char *eptr;

    if( 0 == strcmp(str, "board") ){
    	return 0;
    }

    if( strstr(str, "ch") != str ){
        return -1;
    }

    str += 2;
    channel = strtol(str, &eptr, 10);
    if( str == eptr ){
        return -1;
    }
    return (int)channel;

}

int read_setting(FILE *fp, int *lineno, int *channel, char *option, char *value, int maxlen)
{
    char line[1024], channel_str[256], *ptr, *ptr1;
    int offset;

    while( 1 ){
        (*lineno)++;
        if( NULL == fgets(line, 1024, fp) ){
            (*lineno)--;
            return -1;
        }
        if( strlen(line) >= 1023 ){
            // write to log
            printf("ERROR (config): %d: line is too long\n", *lineno);
            return -1;
        }
        line[strlen(line)-1] = '\0';

        ptr = skip_spaces(line);
        if( '\0' == *ptr ){
            // empty line. skip
            continue;
        }
        if( strstr(ptr, "//") == ptr ){
            continue;
        }

        ptr1 = strchr(ptr,':');///Почему не использовать strstr? Вопрос теме
        if( NULL == ptr1 ){
            // write to log
            printf("ERROR (config): %d: no ':' symbol found\n", *lineno);
            return -1;
        }
        *ptr1 = '\0';
        copy_token(option, ptr, maxlen);

        *channel = get_channel(option);
        if( 0 > *channel ){
            // write to log
            printf("ERROR (config): %d: bad channel number\n", *lineno);
            return -1;
        }
        ptr = ptr1 + 1;

        ptr1 = strchr(ptr,'=');
        if( NULL == ptr1 ){
            // write to log
            printf("ERROR (config): %d: no '=' symbol found\n", *lineno);
            return -1;
        }
        *ptr1 = '\0';
        ptr = skip_spaces(ptr);
        copy_token(option, ptr, maxlen);

        ptr = ptr1 + 1;
        ptr = skip_spaces(ptr);
        copy_token(value, ptr, maxlen);
        break;
    }
    return 0;
}

typedef struct
{
    int id;
    char name[256];
    int convert;
    int base;
    int int_val;
} description_t;

static description_t options_chan[] = {
	{ 1, "Mode",    0, 0, 0 },
	{ 2, "State",   0, 0, 0 },
    { 3, "Input",   0, 0, 0 },
    { 4, "KU1",     1, 10, 0 },
    { 5, "Fcut",    1, 10, 0 },
    { 6, "KU2",     1, 10, 0 },
    { 7, "Fd",      1, 10, 0 },
	{ 8, "Fres",    1, 10, 0 },
	{ 9, "ID_ch",   1, 16, 0 },
	{ 10,"SID_ch",  1, 16, 0 },
	{ 11,"KEMS",    1, 16, 0 },
	{ 12,"X_Y_Z",   0, 0, 0 },
};
static int options_chan_sz = sizeof(options_chan) / sizeof(options_chan[0]);

static description_t options_brd[] = {
	{ 1, "dname",    	    0, 0, 0 },
	{ 2, "Port",   		    0, 0, 0 },
    { 3, "SerialNum",       1, 16, 0 },
    { 4, "SyncSrc",   	    0, 0, 0 },
    { 5, "DataReceiver",    0, 0, 0 },
	{ 6, "num_eqe_rec" ,    1, 10, 0},
	{ 7, "type_connection", 0, 0, 0 }
};
static int options_brd_sz = sizeof(options_brd) / sizeof(options_brd[0]);

static int
preprocess_option(description_t *opts, int opt_num, char *option, char *value)
{
    int i;
    for(i=0; i < opt_num; i++){
        if( strcmp(option, opts[i].name) != 0){
            continue;
        }

        if( opts[i].convert ){
            char *eptr;
            opts[i].int_val = strtol(value, &eptr, opts[i].base);
            if( eptr == value ){
                return -1;
            }
        }
        return i;
    }

    return -1;
}

static int channel_cfg(struct settings_ch *cfg, char *option, char *value, int lineno)
{
	int idx;

	idx = preprocess_option(options_chan, options_chan_sz, option, value);
	if( 0 > idx ){
		// write to log
		printf("ERROR (config): %d: bad option \"%s\" or it's value \"%s\"\n",
				lineno, option, value);
		return -1;
	}

	switch( options_chan[idx].id ){
	case 1:
		if( strcmp(value, "on") == 0){
			cfg->mode = 1;
		}else if( strcmp(value, "off") == 0){
			cfg->mode = 0;
		}
		break;

	case 2:
		if( strcmp(value, "start") == 0){
			cfg->state = 1;
		}else if( strcmp(value, "stop") == 0){
			cfg->state = 0;
		}else if ( strcmp(value, "earthquake_emul") == 0 ){
			cfg->state = 2;
		}
		break;

	case 3:
		if( strcmp(value, "1:1") == 0 ){
			cfg->config_ch.input = inp_1_1;
		} else if( strcmp(value,"1:10") == 0 ){
			cfg->config_ch.input = inp_1_10;
		} else if( strcmp(value,"0V") == 0 ){
			cfg->config_ch.input = inp_0V;
		} else if( strcmp(value, "calibrator" ) == 0){
			cfg->config_ch.input = inp_cal;
		} else if( strcmp(value,"z-state") == 0){
			cfg->config_ch.input = z_state;
		} else {
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",
					lineno, option, value);
			return -1;
		}
		break;

	case 4:
		cfg->config_ch.ku1 = options_chan[idx].int_val;
		break;

	case 5:
		cfg->config_ch.fcut = options_chan[idx].int_val;
		break;

	case 6:
		cfg->config_ch.ku2 = options_chan[idx].int_val;
		break;

	case 7:
		cfg->config_ch.fd = options_chan[idx].int_val;
		break;

	case 8:
		cfg->config_ch.fres = options_chan[idx].int_val;
		break;

	case 9:
		cfg->config_ch.ID_ch = options_chan[idx].int_val;
		break;

	case 10:
		if(options_chan[idx].int_val > 0xFF ){
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		cfg->config_ch.SID_ch = options_chan[idx].int_val;
		break;

	case 11:
		if(options_chan[idx].int_val > 0x270F ){
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		cfg->config_ch.KEMS = options_chan[idx].int_val;
		break;

	case 12:
		if( strcmp(value, "X") == 0 ){
			cfg->config_ch.X_Y_Z  = 0x01;
		} else if( strcmp(value,"Y") == 0 ){
			cfg->config_ch.X_Y_Z  = 0x02;
		} else if( strcmp(value,"Z") == 0 ){
			cfg->config_ch.X_Y_Z  = 0x03;
		} else if( strcmp(value,"NONE") == 0 ){
			cfg->config_ch.X_Y_Z  = 0x00;
		} else {
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		break;
	}

	return 0;
}

static int board_cfg(struct settings_brd *cfg, char *option, char *value, int lineno)
{
	int idx;
	char buf_path[64] = {0};

	idx = preprocess_option(options_brd, options_brd_sz, option, value);
	if( 0 > idx ){
		// write to log
		printf("ERROR (config): %d: bad option \"%s\" or it's value \"%s\"\n",
				lineno, option, value);
		return -1;
	}

	switch( options_brd[idx].id ){
	case 1:
		if( strlen(value) > sizeof(cfg->dname) ){
			printf("ERROR (config): %d: bad length of option \"%s\" value\n", lineno, option, value);
			return -1;
		}
		strcpy(cfg->dname, value);
		break;

	case 2:
		if( strlen(value) > sizeof(cfg->port) ){
			printf("ERROR (config): %d: bad length of option \"%s\" value\n",
					lineno, option, value);
			return -1;
		}
		strcpy(cfg->port, value);
		break;
	case 3:
		cfg->sn = options_brd[idx].int_val;
		break;

	case 4:
		if( strcmp(value, "GPS_auto") == 0 ){
			cfg->sync_src = 0x00;
		} else if( strcmp(value,"GPS_master") == 0 ){
			cfg->sync_src = 0x01;
		} else if( strcmp(value,"RS-485") == 0 ){
			cfg->sync_src = 0x02;
		} else if( strcmp(value,"IEEE1588") == 0 ){
			cfg->sync_src = 0x03;
		} else {
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		break;

	case 5:
		if( strcmp(value, "SD") == 0 ){
			cfg->sync_src = 0x00;
		} else if( strcmp(value,"SD_ring") == 0 ){
			cfg->sync_src = 0x01;
		} else if( strcmp(value,"Ethernet") == 0 ){
			cfg->sync_src = 0x02;
		} else if( strcmp(value,"RS-485") == 0 ){
			cfg->sync_src = 0x03;
		} else {
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		break;

	case 6:
		cfg->num_file_earthquake_emul = options_brd[idx].int_val;

		snprintf(buf_path,sizeof(buf_path), "/kti_bb_ss/earthquake_emul_%d.ktivtd1",cfg->num_file_earthquake_emul);

		cfg->fd_earthquake_emul = open(buf_path, O_RDONLY);
		if (cfg->fd_earthquake_emul < 0){
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		break;

	case 7:
		if( strcmp(value, "client") == 0 ){
			cfg->eth_type_connection = 0x00;
		} else if( strcmp(value,"server") == 0 ){
			cfg->eth_type_connection = 0x01;
		} else {
			printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",lineno, option, value);
			return -1;
		}
		break;

	}

	return 0;
}


int parse_config(FILE *fp, struct settings_brd *brd_cfg, struct settings_ch *ch_cfg, int chan_num){
	char option[1024], value[1024];
    int lineno;
    while( !feof(fp) ){
        int chan;
        int ret = read_setting(fp, &lineno, &chan, option, value, 1024);
        if( 0 > ret && !feof(fp) ){
            exit(0);
        } else if( feof(fp) ){
            break;
        }

        if( chan < 0 || chan > chan_num ){
            printf("ERROR (config): %d: bad channel number %d, max is %d\n",
                    lineno, chan, chan_num);
            return -1;
        }

        if( chan > 0 ){
        	int rc = channel_cfg(&ch_cfg[chan - 1], option, value, lineno);
        	if( rc ) {
        		return rc;
        	}
        } else {
        	int rc = board_cfg(brd_cfg, option, value, lineno);
        	if( rc ) {
        		return rc;
        	}
        }
    }
    return 0;
}
