#pragma once

#define _GNU_SOURCE 1

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define packed __attribute__ ((packed))
// #define big_endian __attribute__ ((scalar_storage_order ("big-endian")))
// #define lil_endian __attribute__ ((scalar_storage_order ("little-endian")))


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;



