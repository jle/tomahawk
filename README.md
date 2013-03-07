Tomahawk
========

A bitmap manipulation library for Android.

Goals
-----

Tomahawk is designed for Android. Its goals are to be as fast and as small as possible; and to work on as many Android devices as possible. 

Requirements
------------

* The `core` library requires Android SDK 8 (Gingerbread) or higher.
* The `example` project is designed for Android SDK 14 or higher.
* Android NDK is required to build JNI files in `core`

Installation
------------

    $ android update lib-project -p core
    $ android update project -p example

Building
--------

To build the NDK files:

    $ cd core/jni && ndk-build

To build and install the example project:

    $ cd example && ant clean debug install

License
-------

Apache License, Version 2.0
