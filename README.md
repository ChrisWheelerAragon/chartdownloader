# Chart Downloader

![Chart Downloader](screenshots/downloader.png?raw=true "Chart Downloader")

Chart Downloader is a very simple downloader for airport navigational charts. Currently only USA charts from FAA are supported. The following charts can be downloaded:

* Airport Diagram.
* Minimum: Takeoff & Alternate Minimums.
* Standard Instrument Departure (SID) charts.
* Standard Terminal Arrival (STARs) charts.
* Approach (ILS/GPS/RNAV..etc) charts.

The charts are downloaded as PDF and stored with a folder specified by the user.

# Aerobask Conversion

My primary motivation for developing the chart downloader is to be able to use charts in [Aerobask](http://aerobask.com/) planes in [XPlane 11](http://www.x-plane.com/) under Linux. Using poppler-qt5 library, all the charts can be converted to PNG files for use within the simulator. The download folder should be the *charts* folder under *Custom Data* directory in the XPlane 11 directory. 

# Requirements

No binary releases yet since it depends on Qt 5.8+. So you need to compile the application under your platform. You need the following:

* Git

Only required if you want to clone the repository. Otherwise, simply download the zip and uncompress it.

* Qt5

Qt 5.8 or higher is required. Please [download Qt 5.8+](https://www.qt.io/download-open-source/#section-2) from Qt and install it (e.g. under /opt)

* Poppler-qt5

Required to convert PDFs to PNGs.


Assuming you already downloaded and installed Qt 5.8+, you can proceed to install the other dependencies:

```
$ sudo apt-get -y install git libpoppler-qt5-dev qml-module-qt-labs-platform qml-module-qtquick-layouts qml-module-qtquick-controls qml-module-qtquick-controls2 qtdeclarative5-dev qml-module-qt-labs-folderlistmodel qml-module-qt-labs-calendar qml-module-qt-labs-settings
```

Now, clone this repo:

```
git clone git@github.com:ChrisWheelerAragon/chartdownloader.git

```

Next, build it with *qmake*.
```
cd chartdownloader
mkdir build
cd build
qmake ../chartdownloader.pro
make
```

Please note that if you have Qt 5.8 installed separately on your PC (e.g. under /opt/Qt5.8.0) then you need to use *qmake* from that path:
```
/opt/Qt5.8.0/5.8/gcc_64/bin/qmake ../chart-downloader.pro
make
```

Now you should chart **chart-download** executable in the build directory, to run the application, simply type **./chart-downloader**

# Usage

It is fairly straight forward to use the chart downloader. You need to enter the full ICAO airport name (US only for now), and you need to specify which directory to download to. If you intend to use Aerobask planes, then set this directory to your /path/to/xplane11/Custom Data/charts.

Specify which charts type to download and whether you want to convert the PDFs to PNGs, and click *Start*. After download is complete, you can go to the **View** tab and examine your image. If you want to edit an image (e.g. to rotate it), simply click the image and it will be launched using your systems image viewer where you can perform any edits necessary.

![Chart Viewer](screenshots/viewer.png?raw=true "Viewer")


That's it! Enjoy your cross-country fights!

## TODO

It would be nice to create executables where users can use simply this application without having to compile it. If you can help in this area, please lend a hand!

# Author

Jasem Mutlaq (mutlaqja@ikarustech.com)
