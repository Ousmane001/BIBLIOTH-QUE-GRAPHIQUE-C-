#!/bin/bash

cd cmake

make $1

cd ..
./cmake/$1

