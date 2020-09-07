ARGoS3-AutoMoDe README
=====================

```
________________________________________________________________________________________________
|.............................................MATE.............................................|
|______________________________________________________________________________________________|
|..............................................................................................|
|.................................................*............................................|
|................................................*.............................................|
|...............................................*..............................................|
|............................................*****.............................................|
|..........................................*********...........................................|
|........................................*************.........................................|
|........................................*************.........................................|
|.........................................***********..........................................|
|...........................................*******............................................|
|______________________________________________________________________________________________|
```

# Table of Contents

- [Installation](#Installation)
- [Run AutoMoDe](#Run-AutoMoDe)
- [Package content](#Package-content)
- [References](#References)

## Installation

All useful information about the ARGoS3-AutoMoDe package, including installation and utilization instructions,
are regrouped in the following technical report [[techrep]](#References). Please cite this report if you use the ARGoS3-AutoMoDe package.

First, compile the dependences: `argos3`, `argos3-epuck`, `demiurge-epuck-dao`, `experiments-loop-functions`.

#### ARGoS simulator

ARGoS is a simulator designed to simulate large-scale robot swarms. For more information, please visit [here](https://github.com/ilpincy/argos3).

On Linux, install these dependences:

```
$sudo apt-get install cmake libfreeimage-dev libfreeimageplus-dev \
  qt5-default freeglut3-dev libxi-dev libxmu-dev liblua5.3-dev \
  lua5.3 doxygen graphviz graphviz-dev asciidoc
```

Then download, compile and install `argos3` version 48:

```
$ cd ~
$ git clone https://github.com/ilpincy/argos3.git
$ cd argos3
$ git checkout 3.0.0-beta48
$ mkdir build
$ cd build
$ cmake ../src
$ make
$ make doc
$ sudo make install
```

#### ARGoS epuck plugin

ARGoS epuck plugin enables the use of the E-puck robot in ARGoS. For more information, please visit [here](https://github.com/demiurge-project/argos3-epuck).

Download, compile and install `argos3-epuck` version 48:

```
$ cd ~
$ git clone https://github.com/demiurge-project/argos3-epuck.git
$ cd argos3-epuck
$ git checkout v48
$ mkdir build
$ cd build
$ cmake ../src
$ make
$ sudo make install
```

#### epuck DAO

This package contains the reference models of the e-puck robot. For more information, please visit [here](https://github.com/demiurge-project/demiurge-epuck-dao).

Download, compile and install `demiurge-epuck-dao`, branch `mate`:

```
$ cd ~
$ git clone https://github.com/demiurge-project/demiurge-epuck-dao.git
$ cd demiurge-epuck-dao
$ git checkout mate
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

#### Experiments loop functions

The package `experiments-loop-functions` contains the source code of the loop fonctions. For more information, please visit [here](https://github.com/demiurge-project/experiments-loop-functions).

Download, compile and install `experiments-loop-functions`, branch `mate`:

```
$ cd ~
$ git clone https://github.com/demiurge-project/experiments-loop-functions.git
$ cd experiments-loop-functions
$ git checkout mate
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

#### AutoMoDe

Download and compile `ARGoS3-AutoMoDe`, branch `mate`:

```
$ cd ~
$ git clone https://github.com/demiurge-project/ARGoS3-AutoMoDe.git
$ cd ARGoS3-AutoMoDe
$ git checkout mate
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run AutoMoDe

For example, to run a mission `mate_apc_visu.xml` with the simple finite state machine `--nstates 1 --s0 4 --att0 5`, open a terminal and copy this command:

```
$ cd ~/AUTOMODE/AutoMoDe-Mate/bin
$ ./automode_main -c ../experiments/mate/mate_apc_visu.xml --seed 0 --readable-fsm --fsm-config --nstates 1 --s0 4 --att0 5
```

## Package content

* bin :: This empty folder will contain the executable automode_main after compilation.
* experiments :: The folder where one should place experiment configuration files (.argos or .xml)

    * mate :: The experiment configuration files of the experiments described in [[mate]](#References)

* optimization :: The folder where one should place files regarding the optimization algorithm used.

    * execution-folder

    * experiments-folder

    * depend_config.txt

    * grammar.txt

    * scenario.txt

    * target-runner

* src :: The source files of ARGoS3-AutoMoDe package.

    * cmake :: The .cmake files for ARGoS3.

    * core :: The core classes of ARGoS3-AutoMoDe.

    * modules :: The six behavior and six condition modules described in [[chocolate]](#References) and the new behavior described in [[mate]](#References).

    * AutoMoDeMain.cpp :: The main procedure to launch ARGoS3-AutoMoDe with irace.


## References

- [techrep] A. Ligot, K. Hasselmann, B. Delhaisse, L. Garattoni, G. Francesca, M. Birattari.
  'AutoMoDe and NEAT implementations for the e-puck robot in ARGoS3'. Technical report TR/IRIDIA/2017-002.
  IRIDIA, Université libre de Bruxelles, Belgium.
- [chocolate] G. Francesca and M. Brambilla and A. Brutschy and L. Garattoni and R. Miletitch and G. Podevijn and A. Reina and T. Soleymani and M. Salvaro and C. Pinciroli and F. Mascia and V. Trianni and M. Birattari.
  'AutoMoDe-Chocolate: automatic design of control software for robot swarms'. Swarm Intelligence.
