#!/usr/bin/python

import sys

###
# Hoved funktion
###
def main():
   intermediate_nodelist = [];
   
   # first do split of comma separated list
   for line in sys.stdin.readlines():
      dosplit = True;
      lastsplit = 0;
      for i,c in enumerate(line):
         if c == "[":
            dosplit = False;
         elif c == "]":
            dosplit = True;
         elif c == "," and dosplit:
            intermediate_nodelist.append(line[lastsplit:i].strip());
            lastsplit = i + 1;
         elif c == "\n":
            intermediate_nodelist.append(line[lastsplit:i].strip());
   
   # then do splits of bracketed ([]) lists
   for line in intermediate_nodelist:
      if line.find("[") != -1:
         lpos = line.find("[");
         rpos = line.find("]");
         prefix = line[0:lpos];
         postfix = line[rpos+1:];
         nodes = line[lpos + 1:rpos].split(",");
         for node in nodes:
            # Take care of ranges ( - )
            if node.find("-") != -1:
               subrange = node.split("-");
               for i in range(int(subrange[0]), int(subrange[1] ) + 1):
                  print prefix + str(i) + postfix;
            else:
               print prefix + node + postfix;
      else:
         print line


###
#
###
if __name__ == "__main__":
    main()
