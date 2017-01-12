/* 
 *   Verbarg: Verbal argument parser for C++
 *   Copyright (C) 2017  Kamran Amini <kam.cpp@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "verbarg.h"


void StartNoded(Args &args);
void StartFssd(Args &args);
void StartMond(Args &args);
void StartStatd(Args &args);
void StartMon(Args &args);
void StartStat(Args &args);
void StartTop(Args &args);

int main(int argc, char **argv) {
  DEFINE_VERB (
    "noded", 
    "Creates a node service.",
    LIST (
      PARAM (
        "interface", 
        LIST("interface", "int"), 
        "The network interface to capture from.", 
        OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "pcapfile", 
        LIST("pcapfile", "pcap", "file"), 
        "Pcap file to read from.", 
        OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "settings", 
        LIST("settings", "config"), 
        "Use the specified settings file. If not specified a settings.json file in the current directory is used.", 
        OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "origins", 
        LIST("origins"), 
        "Origins of the contents to spit out.", 
        OPTIONAL, 
        HAS_VALUE
      )
    )
  )
  DEFINE_VERB (
    "fssd", 
    "Creates a file system service.",
    LIST (
      PARAM (
        "unixsock", 
        LIST("unixsock", "sock", "sockpath", "unixsockpath"), 
        "The path to unix socket which will be bound.", 
        NOT_OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "workdir", 
        LIST("workdir", "wdir", "wpath"), 
        "Working directory for all file system operations.", 
        NOT_OPTIONAL,
        HAS_VALUE
      )
    )
  )
  DEFINE_VERB (
    "statd", 
    "Creates a service for receiving stats.",
    LIST (
      PARAM (
        "unixsock", 
        LIST("unixsock", "sock", "sockpath", "unixsockpath"), 
        "The path to unix socket which will be bound.", 
        OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "bindip", 
        LIST("bindip", "bindint"), 
        "IP which should be bound if TCP networking is enabled.", 
        OPTIONAL,
        HAS_VALUE
      ),
      PARAM (
        "bindport", 
        LIST("bindport"), 
        "Port which should be bound if TCP networking is enabled.", 
        OPTIONAL,
        HAS_VALUE
      )
    )
  )
  DEFINE_VERB (
    "mond", 
    "Creates a service for monitoring services.",
    LIST (
      PARAM (
        "unixsock", 
        LIST("unixsock", "sock", "sockpath", "unixsockpath"), 
        "The path to unix socket which will be bound.", 
        OPTIONAL, 
        HAS_VALUE
      ),
      PARAM (
        "bindip", 
        LIST("bindip", "bindint"), 
        "IP which should be bound if TCP networking is enabled.", 
        OPTIONAL,
        HAS_VALUE
      ),
      PARAM (
        "bindport", 
        LIST("bindport"), 
        "Port which should be bound if TCP networking is enabled.", 
        OPTIONAL,
        HAS_VALUE
      )
    )
  )  
  DEFINE_VERB (
    "stat", 
    "???",
    LIST (
    )
  )
  DEFINE_VERB (
    "mon", 
    "???",
    LIST (
    )
  )
  DEFINE_VERB (
    "top", 
    "???",
    LIST (
    )
  )
  Args args;
  int exit_status = ProcessArgs(argc, argv, args);
  if (exit_status) {
    return exit_status;
  }
  if (args.verb == "noded") {
    StartNoded(args);
  } else if (args.verb == "fssd") {
    StartFssd(args);
  }  else if (args.verb == "statd") {
    StartStatd(args);
  } else if (args.verb == "mond") {
    StartMond(args);
  } else if (args.verb == "stat") {
    StartStat(args);
  } else if (args.verb == "mon") {
    StartMon(args);
  } else if (args.verb == "top") {
    StartTop(args);
  } else {
    std::cout << "ERROR: Unknown verb '" << args.verb << "'" << std::endl;
    exit(1);
  }
  return 0;
}