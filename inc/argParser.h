#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

#include "arg.h"

struct args *ap_init(int argc, char **argv);
void ap_deinit();

#endif /* _ARG_PARSER_H_ */