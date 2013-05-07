MPD-Notification: Display a notification each time MPD's state changes
================

Dependencies
------------

    * libmpdclient
    * libnotify


Installation
------------

Install dependencies (on Debian)
```
aptitude install libnotify-dev libmpdclient-dev
```

Then install the program in your $HOME (see Makefile)
```
make install distclean
```

Hope you'll like it; I enjoy feedbacks (bugs, suggestions, etc) :-)
