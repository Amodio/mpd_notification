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
sudo aptitude install libnotify-dev libmpdclient-dev
```

If needed, you should install a notification daemon:
```
sudo aptitude install xfce4-notifyd
```

Then install the program in your $HOME (see Makefile)
```
make install distclean
```

Hope you'll like it; I enjoy feedbacks (bugs, suggestions, etc) :-)

[![Build Status](https://drone.io/github.com/Amodio/mpd_notification/status.png)](https://drone.io/github.com/Amodio/mpd_notification/latest)
