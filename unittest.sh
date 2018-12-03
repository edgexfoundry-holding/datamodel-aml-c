###############################################################################
# Copyright 2018 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###############################################################################

#!/bin/bash

PROJECT_ROOT=$(pwd)

function build(){
    cd $PROJECT_ROOT
    ./build.sh --build_mode=debug
    if [ $? -ne 0 ]; then 
        echo -e "\033[31m"Build failed"\033[0m" 
        exit 1 
    fi
}

function run_test(){
    cd $PROJECT_ROOT/out/linux/x86_64/debug/unittests
    export LD_LIBRARY_PATH=${PROJECT_ROOT}/dependencies/datamodel-aml-cpp/out/linux/x86_64/debug
    ./caml_rep_test
    if [ $? -ne 0 ]; then 
        echo -e "\033[31m"Unittests failed"\033[0m" 
        exit 1 
    fi
}

function coverage() {
    cd $PROJECT_ROOT
    if [ ! -d "./coverage" ]; then
        mkdir coverage
    fi

    gcovr -r . \
            -e "build_common.*" \
            -e "coverage.*" \
            -e "dependencies.*" \
            -e "docs.*" \
            -e "extlibs.*" \
            -e "out.*" \
            -e "protobuf.*" \
            -e "samples.*" \
            -e "unittests.*" \
            --html --html-details -o ./coverage/report.html
}

echo -e "Building CAML DataModel library("${CAML_TARGET_ARCH}").."
build
echo -e "Done building CAML DataModel library("${CAML_TARGET_ARCH}")"

echo -e "Running CAML DataModel Unittests"
run_test
echo -e "Done Unittests"

if [ "$1" == --coverage ]; then
    coverage
fi

exit 0
