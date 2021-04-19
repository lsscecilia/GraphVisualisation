import sys, getopt
import re

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

def random_color():
    color = list(np.random.uniform(range(1), size=3))
    return tuple(color)

def parseTxtFile(name, with_coloring, no_edges):
    with_weight = False
    sum = 0
    g = nx.Graph()
    with open(name) as f:
        lines = f.readlines()
    color_brk = 0
    chr_dict = {}
    chr_color = {}
    node_color = {}
    if with_coloring:
        for i in range(len(lines)):
            if lines[i][0] == '%':
                color_brk = int(i) + 1
                break
            v = re.split(" |\n", lines[i])
            chr_dict[v[0]] = v[1]

    # assign random colour to dictionary
    for key, value in chr_dict.items():
        if value in chr_color:
            node_color[key] = chr_color[value]
            continue

        chr_color[value] = random_color()
        node_color[key] = chr_color[value]

    # edges
    for i in range(color_brk, len(lines)):
        if lines[i][0] == '-':
            brk = int(i)
            break
        wl = -1
        prev = False
        v = re.split("--|\n|,", lines[i])
        if len(v) == 4:
            #if (v[2]>=50)
            if not no_edges:
                g.add_edge(v[0], v[1], Color='black', weight=v[2])
        else:
            if not no_edges:
                g.add_edge(v[0], v[1], Color='black')

    # nodes & position
    for i in range(brk+1, len(lines)):
        if lines[i][0] == '^':
            brk = i + 1
            break

        v = re.split("\n|[|]|[)]|[(]|,", lines[i])
        if colour:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]), Color=node_color[v[0]])
        else:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]))

    #size
    for r in range(len(lines[brk])):
        if lines[brk][r] == ',':
            comma = r
        if lines[brk][r] == '\n':
            end = r
    lines[brk]
    return [g, [int(lines[brk][:comma]), int(lines[brk][comma+1:end])], with_weight]

def plot(g, coor, outFile, with_weight, colour, nodeLabel):
    # Define node positions data structure (dict) for plotting
    node_positions = {node[0]: (node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)}
    print(node_positions)
    # Define data structure (list) of edge colors for plotting
    edge_colors = [e[2]['Color'] for e in g.edges(data=True)]

    cm = 1/2.54
    plt.figure(figsize=(coor[0]*cm, coor[1]*cm))
    if colour:
        #node colors
        node_colors = [node[1]['Color'] for node in g.nodes(data=True)]
        if nodeLabel:
            nx.draw(
                g,
                pos=node_positions,
                node_color=node_colors,
                edge_color=edge_colors,
                node_size=50,
                with_labels=True,
                font_size=5)
        else:
            nx.draw(
                g,
                pos=node_positions,
                node_color=node_colors,
                edge_color=edge_colors,
                node_size=50,
                with_labels=False,
                font_size=5)
    else:
        if nodeLabel:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=20,
                node_color='grey',
                with_labels=True,
                font_size=5)
        else:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=20,
                node_color='grey',
                with_label=False,
                font_size=5)

    if with_weight:
        labels = {e: g.edges[e]['weight'] for e in g.edges}
        nx.draw_networkx_edge_labels(g, pos=node_positions, edge_labels=labels)
    plt.title('Graph', size=15)
    plt.savefig(outFile, dpi=1000)


if __name__ == "__main__":
    arguments = len(sys.argv)-1
    longOptions = ['version', 'help', 'iterations', 'interval', 'colour', 'resolution', 'nolabel', 'noedges']
    options = "vhi:n:cr:le"
    loop = False
    colour = False
    resolution_overwrite = False
    iterations = 100
    label = True
    no_edges = False
    try:
        opts, args = getopt.getopt(sys.argv[1:], options, longOptions)
    except getopt.GetoptError:
        print('wrong params')
    for opt, arg in opts:
        if opt in ("-i", "--iterations"):
            iterations = int(arg)
        elif opt in ("-n", "--interval"):
            interval = int(arg)
            loop = True
        elif opt in ("-c", "--colour"):
            colour = True
        elif opt in ("-r", '--resolution'):
            resolution = int(arg)
            resolution_overwrite = True
        elif opt in ("-l", '--nolabel'):
            label = False
        elif opt in ("-e", '--noedges'):
            no_edges = True


    if loop:
        inPath = sys.argv[arguments-1]
        outPath = sys.argv[arguments]
        for i in range(0, iterations, interval):
            results = parseTxtFile(inPath+"_"+str(i)+".txt", colour, no_edges)
            if resolution_overwrite:
                plot(
                    results[0],
                    [resolution, resolution],
                    outPath+"_"+str(i)+".png",
                    results[2],
                    colour,
                    label)
            else:
                plot(
                    results[0],
                    results[1],
                    outPath+"_"+str(i)+".png",
                    results[2],
                    colour,
                    label)

    else:
        inFile = sys.argv[arguments-1]
        outFile = sys.argv[arguments]
        results = parseTxtFile(inFile, colour, no_edges)
        if resolution_overwrite:
            plot(results[0], [resolution, resolution], outFile, results[2], colour, label)
        else:
            plot(results[0], results[1], outFile, results[2], colour, label)