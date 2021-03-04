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


if __name__ == "__main__":
  arguments = len(sys.argv)-1
  longOptions =['version', 'help','incomplete', 'num']
  options = "vhin:"
  loop = False
  numNodes = 10 #default number
  try: 
    opts, args = getopt.getopt(sys.argv[1:], options, longOptions)
  except getopt.GetoptError:
    print('wrong params')
  for opt,arg in opts:
    if opt in ("-n", "--num"):
      numNodes = int(arg)
  generate(numNodes)
