# ec -- An emacsclient launcher for Cygwin

Why another emacsclient launcher for Cygwin? There may be a perfect
emacsclient wrapper out there (I haven't found it, and this one most
certainly isn't), but most of the launchers I've encountered suffered
from some combination of these three issues:

1. spaces in the path
2. only one argument at a time
3. were either wait or no-wait (-n) only

ec works around these issues by naively passing anything at the
beginning of the argument list starting with a dash on to emacsclient,
everything that comes after the first non-dash-starting argument on as
a path shortened (spaces removed) by Windows and posixified by Cygwin.

It's been tested to work with Windows SendTo (shell:sendto), network
drives, and various applications which can call external text editing
applications and seems to work pretty well for my needs.

ec is covered by an MIT license so you can pretty much do what you
will with it for what it's worth.