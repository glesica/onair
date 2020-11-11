#!/bin/sh

set -e 

arduino-cli lib install "Adafruit GFX Library" HT1632
cat '#define HUMAN "Human"' > human.h

