# GPS RMC Message parser

to run this project on your microcontroller you will need to have `mbed-cli` installed
instructions to do that can be found here: https://github.com/ARMmbed/mbed-cli#installing-mbed-cli

## Running the project
to run the project `cd` into the repo, then run `mbed new .`
then `mbed deploy`

once all the libraries have finished downloading, select your target board using the command `mbed target YOUR_TARGET_BOARD`
I was using the NUCLEO_F070RB while developing, so you might have to change the serial pins

you can use the `build.sh` and `flash.sh` scripts to build and flash the code on the board

## Fake GPS
to test the library, you will need a second microcontroller. I again used another NUCLEO_F070RB board connected to the first one over UART.
the `fake_gps.cpp` sends an RMC message over UART mimicking the data coming from the GPS receiver