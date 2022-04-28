#!/bin/bash

declare -r NONE="\033[m"
declare -r LIGHT_RED="\033[1;31m"
declare -r LIGHT_GREEN="\033[1;32m"
declare -r YELLOW="\033[1;33m"
declare -r LIGHT_BLUE="\033[1;34m"
declare -r PURPLE="\033[1;35m"
declare -r WHITE="\033[1;37m"
declare -r RED="\033[2;31m"

SRC_DIR=./src
OUT_DIR=./out
OBJ_DIR=${OUT_DIR}/obj
BIN_DIR=${OUT_DIR}/bin

LIB=lpthread
INC=./include

#Create Output Directory
function create_out_dir()
{
    mkdir -p ${OBJ_DIR}
    mkdir -p ${BIN_DIR}
}

function white_msg()
{
    echo -e "$WHITE $@ $NONE"
}

function red_msg()
{
    echo -e "$LIGHT_RED $@ $NONE"
}

function red_error_msg()
{
    echo -e "$RED $@ $NONE"
}

function green_msg()
{
    echo -e "$LIGHT_GREEN $@ $NONE"
}

function yellow_msg()
{
    echo -e "$YELLOW $@ $NONE"
}

function blue_msg()
{
    echo -e "$LIGHT_BLUE $@ $NONE"
}

function purple_msg()
{
    echo -e "$PURPLE $@ $NONE"
}

function do_make()
{
    gcc ./src/*.c -
}

#SOURCE = $(wildcard $(SRC_DIR)/*.c)
#OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCE)))
#DEST = $(BIN_DIR)/mqtt_demo

#	$(CC) $(SOURCE) -o $(DEST) -I$(INC) -$(LIB)

red_msg "1"
red_error_msg "2"
yellow_msg "3"
green_msg "4"
blue_msg "5"
if [ -e ${OUT_DIR} ];then
    create_out_dir
fi

