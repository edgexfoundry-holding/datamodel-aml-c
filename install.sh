#!/bin/bash

USR_LIB_DIR=/usr/local/lib
USR_INC_DIR=/usr/local/include
KEYWORD_AML_C=datamodel-aml-c

# COPY shared library to /usr/local/lib
sudo cp out/linux/armhf/release/libcaml.so $USR_LIB_DIR

# COPY header file to /usr/local/include/datamodel-aml-c
sudo rm -rf $USR_INC_DIR/$KEYWORD_AML_C
sudo mkdir $USR_INC_DIR/$KEYWORD_AML_C
sudo cp -r ./include/* $USR_INC_DIR/$KEYWORD_AML_C
