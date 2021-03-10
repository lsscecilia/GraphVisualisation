import itertools
import copy
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import sys, getopt

def parseTxtFile(name):
  sum=0
  g = nx.Graph()
  with open(name) as f:
      lines = f.readlines()
  for i in range(len(lines)):
    if (lines[i][0]=='-'):
      brk = int(i)
      break
  for i in range(brk+1,len(lines)):
    if (lines[i][0]=='^'):
      brk = i + 1
      break
    for r in range(len(lines[i])):
      if (lines[i][r]=='|'):
        s = r
      if (lines[i][r]==','):
        comma = r
      if (lines[i][r]==')'):
        end = r
          
    node = lines[i][:s]
    x =lines[i][s+2:comma]
    y =lines[i][comma+1:r-1]
      
    g.add_node(node,X=float(x), Y=float(y))
  for r in range(len(lines[brk])):
    if (lines[brk][r]==','):
      comma = r
    if (lines[brk][r]=='\n'):
      end = r
    lines[brk]
  return [g,[int(lines[brk][:comma]), int(lines[brk][comma+1:end])]]


def plot(g, coor, outFile):  
  fig = plt.figure(figsize=(10/2,10/2))
  ax = fig.add_subplot(1, 1, 1)

  # Major ticks every 20, minor ticks every 5
  major_ticks = np.arange(0, coor[0], 1)
  minor_ticks = np.arange(0, coor[0], 1)

  ax.set_xticks(major_ticks)
  ax.set_xticks(minor_ticks, minor=True)
  ax.set_yticks(major_ticks)
  ax.set_yticks(minor_ticks, minor=True)

  # And a corresponding grid
  ax.grid(which='both')

  # Or if you want different settings for the grids:
  #ax.grid(which='minor', alpha=0.2)
  #ax.grid(which='major', alpha=0.5)

  points = [(node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)]
  x = list(map(lambda x: x[0], points))
  y = list(map(lambda x: x[1], points))
  plt.scatter(x, y)
  plt.grid(True)
  plt.title('Graph', size=15)
  plt.savefig(outFile)

if __name__ == "__main__":
  arguments = len(sys.argv)-1
  longOptions =['version', 'help']
  options = "vh"
  loop = False
  try: 
    opts, args = getopt.getopt(sys.argv[1:], options, longOptions)
  except getopt.GetoptError:
    print('wrong params')

  inFile = sys.argv[1]
  outFile = sys.argv[2]
  results = parseTxtFile(inFile)
  plot(results[0],results[1], outFile)
