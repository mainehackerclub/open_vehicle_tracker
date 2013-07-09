Open Vehicle Tracker
====================
An open source solution to both the hardware and software aspects of tracking a vehicle in real-time.
[http://mainehackerclub.github.io/open_vehicle_tracker/](http://mainehackerclub.github.io/open_vehicle_tracker/)

About
-----
This project was started at [Maine Civic Hack Day 2013](http://mainecivichackday.com), which was part of the [National Day of Civic Hacking](http://hackforchange.org)

Hardware
--------

Using open source hardware for minimum cost and maximum extensibility, The board provides real time updates of GPS coordinates to a server.  

####Prototype v0.0.1

  * Arduino UNO
  * Ethernet shield
  * GPS shield.  
  * Using GPRS internet connection, via Ethernet
  * Cost: ~$100.

Software
--------

####Web Server

  * Nginx
  * Postgres database 
  * Google Maps API v3.
  * PHP
  * jQuery

#### HTTP API

   * Documentation & Code samples at [docs.openvehicletracker.apiary.io](http://docs.openvehicletracker.apiary.io)

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
