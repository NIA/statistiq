StatistiQ — experiment data statistics
======================================

![StatistiQ screenshot][statistiq-screen] ![StatistiQ report][statistiq-report-screen]

This is a simple Qt application which allows to enter numeric data of
an experiment or read them from file and then calculates and displays
various statistical information about these data.

Basic features
--------------

- Data input from keyboard or from file
- Editing data (modify, add, remove)
- Calculating E-value, standard deviation, dispersion and moments
- Building configurable histogram and data curve
- Showing report with ability to save it to html or print

Compiling
---------

In order to build, you need [Qt SDK] and [Qwt 6.0.1] installed.

Open `statistiq.pro` in Qt Creator and click *Run*.

Or, in command line, go to the folder where `statistiq.pro` is located and run

    qmake statistiq.pro
    make release

To build translation file for Russian language, also run

    lrelease statistiq.pro


  [Qt SDK]: http://qt-project.org/downloads
  [Qwt 6.0.1]: http://sourceforge.net/projects/qwt/files/qwt/6.0.1
  [statistiq-screen]: https://dl.dropbox.com/u/6068922/statistiq-screen.png
  [statistiq-report-screen]: https://dl.dropbox.com/u/6068922/statistiq-report-screen.png

