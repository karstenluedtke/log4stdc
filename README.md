# log4stdc
Logging library for standard "C" programs

The library log4stdc uses objects like loggers and appenders similar to log4j or log4cxx,
but since standard "C" does not have any classes or methods, these objects are created
and accessed using "C" functions and macros.

Configuration log4j.property or log4j.xml files can be read (though only a subset of the 
configuration options is actually implemented).

Appenders: ConsoleAppender, RollingFileAppender, some TCP streams (xml, json, java object).

Operating systems: UNIX (incl. Linux), Windows, MacOSx, Android, iOS, and possibly others.
I have even testwise compiled a 16-bit version using MS-C 6.0.
