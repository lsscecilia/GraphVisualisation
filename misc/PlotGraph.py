import itertools
import copy
import networkx as nx
import matplotlib.pyplot as plt
import sys

def parseTxtFile(name):
    g = nx.Graph()
    with open(name) as f:
        lines = f.readlines()
    
    for i in range(len(lines)):
        if (lines[i][0]=='-'):
            brk = int(i)
            break
        
        prev = False 
        for r in range(len(lines[i])):
            if (lines[i][r]=='-' and not prev):
                fn = r
                prev = True
            if (lines[i][r]!='-' and prev):
                ln = r
            if (lines[i][r]=='\n'):
                end = r
            
        n1 = lines[i][:fn]
        n2 = lines[i][ln-1:end]
        g.add_edge(n1,n2, Color='red')
        
    
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

if __name__ == "__main__":
    inFile = sys.argv[1]
    outFile = sys.argv[2]
    results = parseTxtFile(inFile)
    g = results[0]
    coor = results[1]
    # Define node positions data structure (dict) for plotting
    node_positions = {node[0]: (node[1]['X'], -node[1]['Y']) for node in g.nodes(data=True)}
    # Define data structure (list) of edge colors for plotting
    edge_colors = [e[2]['Color'] for e in g.edges(data=True)]
    cm = 1/2.54
    plt.figure(figsize=(coor[0]*cm, coor[1]*cm))
    nx.draw(g, pos=node_positions, edge_color=edge_colors, node_size=10, node_color='black')
    plt.title('Graph', size=15)
    plt.savefig(outFile)
