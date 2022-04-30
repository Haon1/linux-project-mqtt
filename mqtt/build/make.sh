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
DEST=${BIN_DIR}/mqtt_demo

LIB=pthread
INC="-I./include/net -I./include/os -I./include/utils"

file=""
temp=""

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

function generate_obj()
{
    file=`find  ${SRC_DIR} -name *.c`
    for a in $file
    do
        temp=${a%.*}
        gcc $a -c -o ${OBJ_DIR}/${temp##*/}.o ${INC}/
        
    done
}

function link()
{
    gcc ${OBJ_DIR}/*.o -o ${DEST} -l${LIB} -lc -lgcc
}

function do_make()
{
    generate_obj
    link
}


#Create Output Directory
if [ ! -e ${OUT_DIR} ]; then
    green_msg "### Create Output Directory ###"
    create_out_dir
fi

#begin to compile
purple_msg "### Start Compile ###"
do_make
green_msg "### End ###"

