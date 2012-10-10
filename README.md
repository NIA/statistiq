StatistiQ — experiment data statistics
======================================

This is a simple Qt application which allows to enter numeric data of
an experiment or read them from file and then calculates and displays
various statistical information about these data.

Basic functions
---------------

- Data input from keyboard or from file *(in progress)*
- Showing E-value, standard deviation, etc  *(planned)*
- Building histogram *(planned)*

Compiling
---------

In order to build, you need Qt SDK installed.

Open `statistiq.pro` in Qt Creator and click *Run*.

Or, in command line, go to the folder where `statistiq.pro` is located and run

    qmake statistiq.pro
    make release

