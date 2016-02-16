# IQFeedClient

Example of how to connect to the DTN IQFeed service to receive real time streaming quotes.

## Features

    Uses Qt5 to allow cross platform
    Handles reconnection to DTN IQFeed service
    Streams three different symbols

CMake is used to create the Visual Studio project files. Because of this we can easily building the project under another platform like Linux. The project is currently setup to generate 64Bit code. There are only two dependencies. Qt5 and the DTN IQFeed application software available here: http://www.iqfeed.net/

To generate the project files;

    OpenInVisualStudio.bat
    
The batch file sets up the nessasary environment variables to allow running from the Visual Studio debugger.

## Running the application

The application needs to be passed a config file containing the account information used to connect to the DTN IQFeed service.
An example config file can be found in the Settings folder. There are two fields in the settings file;

    root - This is the root folder of your DTN software installation
    connectionInfo - This is a string containing your user id and password for the DTN IQFeed service
