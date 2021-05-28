# Copyright (c) 2021 Cecilia Lee

import sys, getopt

def generate(n):
  filename = str(n) + "n.txt"
  f= open(filename,"w+")
  numEdges=0
  for i in range(n):
      for r in range(i):
          line = str(r) + "--" + str(i) +"\n"
          f.write(line)
          numEdges+=1
  f.close()
  print('number of nodes: ', n)
  print("number of edges: ", numEdges)


def generateCircularGraph(n):
  filename = str(n) + "n_c.txt"
  f= open(filename,"w+")
  numEdges=0
  for i in range(n-1):
    line = str(i) + "--" + str(i+1) +"\n"
    f.write(line)
    numEdges+=1
  line = str(n-1) + "--" + str(0) +"\n"
  f.write(line)
  numEdges+=1
  f.close()
  print('number of nodes: ', n)
  print("number of edges: ", numEdges)


if __name__ == "__main__":
  arguments = len(sys.argv)-1
  longOptions =['version', 'help','incomplete', 'circular','num']
  options = "vhicn:"
  loop = False
  circular = False
  numNodes = 10 #default number
  try: 
    opts, args = getopt.getopt(sys.argv[1:], options, longOptions)
  except getopt.GetoptError:
    print('wrong params')
  for opt,arg in opts:
    if opt in ("-c", "--circular"):
      circular = True
    if opt in ("-n", "--num"):
      numNodes = int(arg)
  
  if circular:
    generateCircularGraph(numNodes)
  else:
    generate(numNodes)
