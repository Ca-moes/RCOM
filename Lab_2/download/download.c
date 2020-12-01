#include "download.h"

int main(int argc, char** argv){ 

  args arguments, arguments2;
  char noUser[] = "ftp://netlab1.fe.up.pt/pub.txt";
  char withUser[] = "ftp://abcd111111111111:12345@netlab1.fe.up.pt/pub.txt";
  parseArgs(noUser, &arguments);

  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\n", arguments.host, arguments.path, arguments.user, arguments.password);
  puts("-----------");
  parseArgs(withUser, &arguments2);
  printf("\nhost: %s\npath: %s\nuser: %s\npassword: %s\n", arguments2.host, arguments2.path, arguments2.user, arguments2.password);
  return 0;
}