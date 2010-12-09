#!/usr/bin/python

import sys
import time
from subprocess import call

def make_merge_file(no_vars):
  f = open("merge%s.c" % (no_vars),"w")
  sys.stdout = f

  r = range(0,no_vars)

  print "#include <klee/klee.h>" + \
"""

int main() {

  int result = 0;
"""

  for i in r:
    print "  int var" + str(i) + ";"
    
  print ""

  for i in r:
    vari = "var" + str(i)
    print "  klee_make_symbolic(&%s, sizeof(%s), \"input %s\");" % (vari, vari, vari)
    
  for i in r:
    vari = "var" + str(i)
    print """
  if (var%d == 42)
    result = 1;
    """ % (i),
    
  print """
  return result;
}
  """

  sys.stdout = sys.__stdout__

def time_merge(n):
  make_merge_file(n)
  
  call(["llvm-gcc --emit-llvm -c -g merge%d.c" % n], shell="/bin/bash")

  t1 = time.time()
  call(["klee merge%d.o" % (n)], shell="/bin/bash", stderr = open("/dev/null", "w"))
  t2 = time.time()

  print 'no merge: n=%d took %0.3f ms' % (n,(t2-t1)*1000.0)
  
  t1 = time.time()
  call(["klee --use-exhaustive-merge merge%d.o" % (n)], shell="/bin/bash", stderr = open("/dev/null", "w"))
  t2 = time.time()

  print '   merge: n=%d took %0.3f ms' % (n,(t2-t1)*1000.0)

for i in range(0,20):
  time_merge(i)
