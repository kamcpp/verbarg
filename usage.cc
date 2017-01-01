#include <iostream>

#include "verbarg.h"


int main(int argc, char **argv) {
  DEFINE_VERB (
    "node", 
    "Creates a node.",
    LIST (
      PARAM("interface", LIST("interface", "int"), "Interface to capture packets from.", OPTIONAL, HAS_VALUE),
      PARAM("pcapfile", LIST("pcapfile", "pcap"), "Pcap file to read from.", OPTIONAL, HAS_VALUE)
    )
  );

  Args args;
  int exit_status = ProcessArgs(argc, argv, args);
  if (exit_status) {
    return exit_status;
  }
  if (args.verb == "node") {
    if (args.HasKey("interface")) {
      // ParseFile(args);
    } else {
      // TODO(kamran)
    }
  } else {
    // TODO(kamran): Shouldn't reach here!
  }
  return 0;
}