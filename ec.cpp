/*

ec -- an emacsclient launcher for Cygwin

Copyright (c) 2016 George Demmy <gdemmy@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <windows.h>
#include <sys/cygwin.h>
#include <sstream>
#include <iostream>
#include <cstring>

enum { MXWINPATH = 32768 };

static const char version[] = "ec version: 1.0 build " __DATE__ " " __TIME__;

static void usage()
{
  std::cout << "usage: ec [options] [files]\n\n"
	    << "Invoke Cygwin emacsclient on Windows, "
	    << "accounting for spaces and whatnot.\n\n"
	    << "ec options:\n"
	    << "-h print this message\n"
	    << "-v print version\n"
	    << "Type ec -H or emacsclient -H for emacsclient options\n";
}

static int patherr(int ret)
{
  switch (ret) {
  case EINVAL:
    std::cerr << "ec: invalid path name\n";
    break;
  case EFAULT:
    std::cerr << "ec: bad reference\n";
    break;
  case ENAMETOOLONG:
    std::cerr << "ec: path name too long\n";
    break;
  case ENOSPC:
    std::cerr << "ec: size is less than space required\n";
    break;
  default:
    break;
  }
  return ret;
}

static int winpathp(char* p)
{
  int i = 0;
  while(*p) {
    if (*p == ':' || *p == '\\') ++i;
    ++p;
  }
  return i;
}

static int proc_args(int argc, char* argv[], std::ostringstream& os)
{
  if (1 == argc) {
    // pop a new frame b/c emacs shell can't handle emacsclient
    system("/bin/emacsclient -c");
    exit(0);
  }

  int i = 1;
  for (; i < argc; ++i) {
    if ('-' == argv[i][0]) {
      if ('v' == argv[i][1]) {
	std::cout << version << '\n';
	exit(0);
      } else if ('h' == argv[i][1]) {
	usage();
	exit(0);
      }
      os << ' ' << argv[i];
    } else {
      // fixme: some args take args (socket)
      return i;
    }
  }
  return i;
}

int main(int argc, char* argv[])
{
  std::ostringstream os;
  os << "/bin/emacsclient";

  // skip args

  int i = proc_args(argc, argv, os);

  for (; i < argc; ++i) {
    char sp[MXWINPATH];
    if (winpathp(argv[i])) {
      if (!GetShortPathName(argv[i], sp, MXWINPATH)) {
	std::cerr << "ec: cannot make short path from: " << argv[i] << '\n';
	continue;
      }
    } else {
      strcpy(sp, argv[i]);
    }
    ssize_t sz = cygwin_conv_path(CCP_WIN_A_TO_POSIX, sp, NULL, 0);
    if (0 > sz) {
      std::cerr << "ec: problem with input: " << argv[i] << '\n';
      return patherr(sz);
    }
    char buf[MXWINPATH];
    (void)cygwin_conv_path(CCP_WIN_A_TO_POSIX, sp, buf, MXWINPATH);
    os << ' ' << buf;
  }
  return system(os.str().c_str());
}
