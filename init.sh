#!/bin/bash

SCRIPT_PATH=$(realpath "$0")
SCRIPT_DIR=$(dirname "${SCRIPT_PATH}")

mkdir -p module
cd "$SCRIPT_DIR/module" || return

if [ ! -d "$SCRIPT_DIR/module/arduino-esp32" ]; then
    git clone --recursive -b 3.1.0-RC3 https://github.com/espressif/arduino-esp32.git
fi
if [ ! -d "$SCRIPT_DIR/module/esp-idf" ]; then
    git clone --recursive -b v5.3.1 https://github.com/espressif/esp-idf.git esp-idf
fi

cd "$SCRIPT_DIR/module/esp-idf" || return
chmod +x "$SCRIPT_DIR/module/esp-idf/install.sh"
./install.sh
echo -e "Run the following afterwards:"
echo -e "source $SCRIPT_DIR/module/esp-idf/export.sh"
