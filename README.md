Open Vehicle Tracker
====================
An open source solution to both the hardware and software aspects of tracking a vehicle in real-time.
[http://mainehackerclub.github.io/open_vehicle_tracker/](http://mainehackerclub.github.io/open_vehicle_tracker/)

About
-----
On June 1st & 2nd we organized, and participated in, a hack-a-thon, one of many across the nation.  The purpose of these code sprints were to create open source civic applications which would improve communities on both a macro and micro scale.  

Our team's project focused on an open source solution for tracking vehicles through GPS devices.  Originally our idea was specifically catered to capturing data from snow plow trucks, but myriad uses have been realised since conception.  Our project has two aspects: hardware and software, both created with open source materials.  

Hardware
--------
Our hardware is comprised of an Arduino microprocessor with a ethernet shield and a GPS shield.  Once programmed, the board provides real time updates of GPS coordinates to a server.  The advantage of using open source hardware in this case is minimum cost and maximum extensibility.  The device transmits data to the server via a cellular network, currently through an actual cell phone.  However with an alternate shield that provides both GPS and cellular functionality the rig becomes standalone.  The estimated cost of this multi tiered device is only $100.


Software
--------
The software application is a virtual private server that hosts a site running on nginx and, as discussed previously, receives these data from the Arduino via a cellular network.  Data is stored in a postgres database and then displayed in (near) real time on a web map using the Google Maps API v3.  Since we realised the potential to expand the service to a variety of vehicles, we are able to dynamically view different classes of vehicles (snow plow trucks, recycle trucks, street-sweepers, and even city busses have been imagined within the system).  Additional information can also be accessed from the hardware unit.  For example: data from a snow plow might include whether or not the plow is down and the amount of sand and salt it's dispersing.  Speed can also be calculated and monitored.

Installation
------------
TBD.


License
-------

The MIT License (MIT)

Copyright (c) 2013 Maine Hacker Club

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
