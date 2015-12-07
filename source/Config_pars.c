#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../include/Config_pars.h"

static char *
skip_spaces(char *str)
{
    char accept[] = " \t\n";
    int offset = strspn(str, accept);
    return str + offset;
}

void copy_token(char *dst, char *src, int maxlen)
{
    char reject[] = " \t"; //Сделано на всякий случай если после ch1 поставить пробел или табуляцию?
    //size_t
	unsigned int offs = strcspn(src, reject);
    memcpy(dst, src, offs);
    dst[offs] = '\0';
}

int get_channel( char *str)
{
    long int channel;
    char *eptr;
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

static description_t options[] = {
	  { 1, "Mode",    0, 0, 0 },
	  { 2, "State",   0, 0, 0 },
    { 3, "Input",   0, 0, 0 },
    { 4, "KU1",     1, 10, 0 },
    { 5, "Fcut",    1, 10, 0 },
    { 6, "KU2",     1, 10, 0 },
    { 7, "Fd",      1, 10, 0 }
};
static int options_size = sizeof(options) / sizeof(options[0]);

static int
preprocess_option(char *option, char *value)
{
    int i;
    for(i=0; i < options_size; i++){
        if( strcmp(option, options[i].name) != 0){
            continue;
        }

        if( options[i].convert ){
            char *eptr;
            options[i].int_val = strtol(value, &eptr, options[i].base);
            if( eptr == value ){
                return -1;
            }
        }
        return i;
    }

    return -1;
}

int parse_config(FILE *fp, struct settings_ch *cfg, int chan_num)
{
    int lineno;
    while( !feof(fp) ){
        char option[1024], value[1024];//зачем объевлять в while? Вопрос к Теме
        int chan, idx;//зачем объевлять в while? Вопрос к Теме
        int ret = read_setting(fp, &lineno, &chan, option, value, 1024);
        if( 0 > ret && !feof(fp) ){
            exit(0);
        } else if( feof(fp) ){
            break;
        }

        if( chan > chan_num ){
            printf("ERROR (config): %d: bad channel number %d, max is %d\n",
                    lineno, chan, chan_num);
            return -1;
        }

        idx = preprocess_option(option, value);
        if( 0 > idx ){
            // write to log
            printf("ERROR (config): %d: bad option \"%s\" or it's value \"%s\"\n",
                    lineno, option, value);
            return -1;
        }

        switch( options[idx].id ){
        case 1:
        	if( strcmp(value, "on") == 0){
        		cfg[chan-1].mode = 1;
        	}else if( strcmp(value, "off") == 0){
        		cfg[chan-1].mode = 0;
        	}
        	break;
        case 2:
        	if( strcmp(value, "start") == 0){
        		cfg[chan-1].state = 1;
        	}else if( strcmp(value, "stop") == 0){
        		cfg[chan-1].state = 0;
        	}
        	break;


        case 3:
            if( strcmp(value, "1:1") == 0 ){
                cfg[chan-1].config_ch.input = inp_1_1;
            } else if( strcmp(value,"1:10") == 0 ){
            	cfg[chan-1].config_ch.input = inp_1_10;
            } else if( strcmp(value,"0V") == 0 ){
            	cfg[chan-1].config_ch.input = inp_0V;
            } else if( strcmp(value, "calibrator" ) == 0){
            	cfg[chan-1].config_ch.input = inp_cal;
            } else if( strcmp(value,"z-state") == 0){
            	cfg[chan-1].config_ch.input = z_state;
            } else {
                printf("ERROR (config): %d: bad option \"%s\" value \"%s\"\n",
                    lineno, option, value);
                return -1;
            }
            break;
        case 4:
        	cfg[chan-1].config_ch.ku1 = options[idx].int_val;
            break;
        case 5:
        	cfg[chan-1].config_ch.fcut = options[idx].int_val;
            break;
        case 6:
        	cfg[chan-1].config_ch.ku2 = options[idx].int_val;
            break;
        case 7:
        	cfg[chan-1].config_ch.fd = options[idx].int_val;
            break;
        }
    }
    return 0;
}
