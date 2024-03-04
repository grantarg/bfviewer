# bfviewer

Program created to display information obtained by using decoded Betaflight log files in the .CSV format, then uses the [Jgraph](https://web.eecs.utk.edu/~jplank/plank/jgraph/jgraph.html) program to display it.

## Dependencies

For bfviewer to work, jgraph is required to be placed in the $PATH variable. Once this is the case, one can simply run the command `make`. Before running the program again, it is **highly recommended** to run the command `make clean` to fully erase any binaries left over.

## Usage

The format for running bfviewer outside of the makefile is as follows: `./bfviewer [FLAG] [Log file]`. The log file can be produced from a .bfl file using the [blackbox-tools](https://github.com/cleanflight/blackbox-tools)

The given makefile will compile the .cpp file and also produce 5 graphs using the sample log file found in the `logs` folder.

## Testing

This program has be certified to work on both my Macbook Pro using the M1 Pro chip and the UTK Hydra Lab machines.

---

Example Images

## Total mAh used (mahtotal.pdf)
![Electrical charge used in mAh](/images/charge.png)
## Battery Voltage (vbat.pdf)
![Graph of the battery voltage over time](/images/vbat.png)
## Current (amperage.pdf)
![Electrical charge used in mAh](/images/current.png)
## Motor speed (motors.pdf)
![Graph of motor speed](/images/motor.png)
## Signal (signal.pdf)
![Graph of RSSI](/images/signal.png)