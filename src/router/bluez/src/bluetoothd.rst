==========
bluetoothd
==========

----------------
Bluetooth daemon
----------------

:Authors: - Marcel Holtmann
          - Philipp Matthias Hahn
          - Fredrik Noring
:Version: BlueZ
:Copyright: Free use of this software is granted under the terms of the GNU
            Lesser General Public Licenses (LGPL).
:Date: March, 2004
:Manual section: 8
:Manual group: System management commands

SYNOPSIS
========

**bluetoothd** [--version] | [--help]

**bluetoothd**  [--nodetach]  [--compat] [--experimental] [--debug=<*files*>]
[--plugin=<*plugins*>] [--noplugin=<*plugins*>]

DESCRIPTION
===========

This manual page documents briefly the **bluetoothd** daemon, which manages
all the Bluetooth devices. **bluetoothd** can also provide a number of services
via the D-Bus message bus system.

OPTIONS
=======

-v, --version       Print bluetoothd version and exit.

-h, --help          Print bluetoothd options and exit.

-n, --nodetach      Enable logging in foreground. Directs log output to the
                    controlling terminal in addition to syslog.

-f, --configfile    Specifies an explicit config file path instead of relying
                    on the default path(*/usr/etc/bluetooth/main.conf*)
                    for the config file.

-d, --debug=<file1>:<file2>:...
    Sets how much information bluetoothd sends to the log destination (usually
    syslog's "daemon" facility). If the file options are omitted, then
    debugging information from all the source files are printed. If file
    options are present, then only debug prints from that source file are
    printed. The option can be a pattern containing "*" and "?" characters.

    Example: --debug=src/adapter.c:src/agent.c

-p, --plugin=<plugin1>,<plugin2>,..
    Load these plugins only. The option can be a pattern containing  "*" and
    "?" characters.

-P, --noplugin=<plugin1>,<plugin2>,..
    Never load these plugins. The option can be a pattern containing "*" and
    "?"  characters.

-C, --compat        Provide deprecated command line interfaces.

-E, --experimental  Enable D-Bus experimental interfaces.
    These interfaces are not guaranteed to be compatible or present in future
    releases.

-T, --testing  Enable D-Bus testing interfaces.
    These interfaces are only meant for test validation of the internals of
    bluetoothd and shall not never be used by anything other than that.

-K, --kernel=<uuid1>,<uuid2>,...
    Enable Kernel experimental features. Kernel experimental features are
    considered unstable and may be removed from future kernel releases.

FILES
=====

*/usr/etc/bluetooth/main.conf*
    Location of the global configuration file.

RESOURCES
=========

http://www.bluez.org

REPORTING BUGS
==============

linux-bluetooth@vger.kernel.org
