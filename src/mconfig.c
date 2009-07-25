/*
 * MALHEUR - Automatic Malware Analysis on Steroids
 * Copyright (c) 2009 Konrad Rieck (rieck@cs.tu-berlin.de)
 * Berlin Institute of Technology (TU Berlin).
 * --
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.  This program is distributed without any
 * warranty. See the GNU General Public License for more details. 
 * --
 */

#include "common.h"
#include "util.h"
#include "mconfig.h"

/* Default configuration */
static config_default_t defaults[] = {
    {"input",       "format",          0,  NAN, "raw"},
    {"input",       "mist_level",      2,  NAN, NULL},
    {"input",       "mist_rlen",       0,  NAN, NULL},
    {"input",       "mist_tlen",       0,  NAN, NULL},
    
    {"features",    "ngram_len",       3,  NAN, NULL},
    {"features",    "ngram_delim",     0,  NAN, "%20%0a%0d"},
    {"features",    "vect_embed",      0,  NAN, "bin"},
    {"features",    "vect_norm",       0,  NAN, "l2"},
    
    {NULL, NULL, 0, 0, NULL}
};

static void config_setting_print(config_setting_t *cs, int depth)
{
    assert(cs && depth >= 0);

    int i;
    for (i = 0; i < depth; i++)
        printf("  ");
    
    char *n = config_setting_name(cs);
    
    switch(config_setting_type(cs)) {
    case CONFIG_TYPE_GROUP:
        if (depth > 0)
            printf("%s = {\n", n);

        for (i = 0; i < config_setting_length(cs); i++)
            config_setting_print(config_setting_get_elem(cs, i), depth + 1); 
        
        if (depth > 0) {
            for (i = 0; i < depth; i++)
                printf("  ");
            printf("};\n");
        }
        break;
    case CONFIG_TYPE_STRING:
        printf("%s\t= \"%s\";\n", n, config_setting_get_string(cs));
        break;
    case CONFIG_TYPE_FLOAT:
        printf("%s\t= %8.5f;\n", n, config_setting_get_float(cs));
        break;
    case CONFIG_TYPE_INT:
        printf("%s\t= %ld;\n", n, config_setting_get_int(cs));
        break;
    default:
        error("Unsupported type for configuration setting '%s'", n);
    }
}

/**
 * Print the Malheur configuration is valid. 
 * @param cfg configuration
 */
void config_print(config_t *cfg)
{
    printf("Malheur configuration\n");
    config_setting_print(config_root_setting(cfg), 0);
}

/**
 * Checks if the configuration is valid. The function checks if all 
 * required parameters are set and adds default values if necessary.
 * @param cfg configuration
 */
void config_check(config_t *cfg)
{
    int i;
    long j;
    const char *s;
    double f;
    config_setting_t *cs, *vs;
    
    for (i = 0; defaults[i].name; i++) {
        /* Lookup setting group */
        cs = config_lookup(cfg, defaults[i].group);
        if (!cs) 
            cs = config_setting_add(config_root_setting(cfg),
                                    defaults[i].group, CONFIG_TYPE_GROUP);
    
        /* Lookup variable */
        if (defaults[i].str) {
            /* Check for string */
            if (config_setting_lookup_string(cs, defaults[i].name, &s))
                continue;

            /* Add default value */
            vs = config_setting_add(cs, defaults[i].name, CONFIG_TYPE_STRING);
            config_setting_set_string (vs, defaults[i].str);
        } else if (!isnan(defaults[i].fnum)) {
            /* Check for float */
            if (config_setting_lookup_float(cs, defaults[i].name, &f))
                continue;

            /* Add default value */
            vs = config_setting_add(cs, defaults[i].name, CONFIG_TYPE_FLOAT);
            config_setting_set_float(vs, defaults[i].fnum);
        } else {
            /* Check for integer */
            if (config_setting_lookup_int(cs, defaults[i].name, &j))
                continue;

            /* Add default value */
            vs = config_setting_add(cs, defaults[i].name, CONFIG_TYPE_INT);
            config_setting_set_int(vs, defaults[i].inum);
        }
    }
}