import sys, getopt
import re
import math
import statistics

import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats

def random_color():
    color = list(np.random.uniform(range(1), size=3))
    return tuple(color)

def parseTxtFile(name, with_coloring, no_edges, link_consequetive_nodes, distance_diff, noNodeColour):
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
            if not link_consequetive_nodes and not no_edges:
                g.add_edge(v[0], v[1], Color='black', weight=v[2])
        else:
            if not link_consequetive_nodes and not no_edges:
                g.add_edge(v[0], v[1], Color='black')
    
    node_pos_list = {}
    if link_consequetive_nodes:
        min_ = 1000000000
        max_ = 0
    # nodes & position
    for i in range(brk+1, len(lines)):
        if lines[i][0] == '^':
            brk = i + 1
            break

        v = re.split("\n|[|]|[)]|[(]|,", lines[i])

        if link_consequetive_nodes:
            if int(v[0]) > max_:
                max_ = int(v[0])
            if int(v[0]) < min_:
                min_ = int(v[0])

        if distance_diff:
            node_pos_list[v[0]] =[float(v[2]), float(v[3])]

        if noNodeColour:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]))
        elif colour:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]), Color=node_color[v[0]])
        else:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]))

    #node_positions = {node[0]: (node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)}

    if link_consequetive_nodes:
        prev = False
        node_list = []
        for i in range(min_, max_, 1):
            if str(i) in g.nodes and str(i-1) in g.nodes:
                g.add_edge(str(i), str(i-1), Color='black')
                #print(str(i) + "--" + str(i-1))
                

        
    if distance_diff:
        outFile = name.split(".txt")
        #print(outFile)
        print(outFile[0])
        g = differentiate_different_distance(g, outFile[0]+"_edges_more_than_x3_median.txt", min_, max_, outFile[0])
    #size
    for r in range(len(lines[brk])):
        if lines[brk][r] == ',':
            comma = r
        if lines[brk][r] == '\n':
            end = r
    lines[brk]
    return [g, [int(lines[brk][:comma]), int(lines[brk][comma+1:end])], with_weight]

def differentiate_different_distance(g, outFile, min_, max_, histOutFile):
    abs_values = {}
    abs_list = []
    #print(g.nodes(data=True))
    node_positions = {node[0]: (node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)}
    #print("die...")
    #print(g.edges)
    for e in g.edges:
        n1 = e[0]
        n2 = e[1]
        pos1 = node_positions[n1]
        pos2 = node_positions[n2]
        abs_value = abs(pos1, pos2)
        abs_values[e] = abs_value
        abs_list.append(abs_value)
        #print("edge")

    median = statistics.median(abs_list)
    #print("median: ", median)
    #print(abs_values)
    f = open(outFile,"w+")
    f.truncate(0)
    for e in g.edges(data=True):
        #print("edge tuple?: ", (e[0],e[1]))
        if abs_values.get((e[0],e[1])) > median*3:
            e[2]['Color'] = 'red'
        elif abs_values.get((e[0],e[1])) > median*2:
            e[2]['Color'] = 'orange'
        else:
            e[2]['Color'] = 'blue'
        
        if abs_values.get((e[0],e[1])) >= (median*3):
            f.write(e[0]+ "--" + e[1] + "," + str(abs_values.get((e[0],e[1]))) + "\n")
    f.close()

    plotHistogram(abs_list, max_, min_, histOutFile)

    print("statistics| median: ", median, ", min: ", min(abs_list), ", max: ", max(abs_list), ", average: ", statistics.mean(abs_list))
    return g
            
def plotHistogram(abs_list, max_, min_, outFile):
    q25, q75 = np.percentile(abs_list,[.25,.75])
    bin_width = 2*(q75 - q25)*len(abs_list)**(-1/3)
    if bin_width == 0:
        bins = 10
    else: 
        bins = round((max(abs_list)- min(abs_list))/bin_width)
    plt.figure(figsize=(8,8))
    plt.hist(abs_list,bins,range=[min(abs_list),max(abs_list)] )
    plt.ylabel('Edge')
    plt.xlabel('Edge length')
    plt.title("Histogram")
    plt.savefig(outFile+"_histogram.png")
    

    #density = stats.gaussian_kde(abs_list)
    #n, x, _ = plt.hist(abs_list, bins=np.linspace(min(abs_list), max(abs_list), 50), 
                   # histtype=u'step', density=True)  
    #plt.plot(x, density(x))
    #plt.show()
    #plt.savefig(outFile+"_histogram.png")


def abs(pos1, pos2):
    return math.sqrt((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)




def plot(g, coor, outFile, with_weight, colour, nodeLabel, noNodeColour):
    #print('plotting...')
    #for e in g.nodes(data=True):
     #   print(e)
    # Define node positions data structure (dict) for plotting
    node_positions = {node[0]: (node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)}
    #print(node_positions)
    # Define data structure (list) of edge colors for plotting
    edge_colors = [e[2]['Color'] for e in g.edges(data=True)]
    #for e in g.edges:
        #print(e)
    cm = 1/2.54
    plt.figure(figsize=(coor[0]*cm, coor[1]*cm))
    if noNodeColour: 
        if nodeLabel:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=2,
                node_color='grey',
                with_labels=True,
                font_size=5,
                width=0.2)
        else:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=2,
                node_color='grey',
                with_labels=False,
                font_size=5, 
                width=0.2)
    elif colour:
        #node colors
        node_colors = [node[1]['Color'] for node in g.nodes(data=True)]
        if nodeLabel:
            nx.draw(
                g,
                pos=node_positions,
                node_color=node_colors,
                edge_color=edge_colors,
                node_size=2,
                with_labels=True,
                font_size=5,
                width=0.1)
        else:
            nx.draw(
                g,
                pos=node_positions,
                node_color=node_colors,
                edge_color=edge_colors,
                node_size=2,
                with_labels=False,
                font_size=5, 
                width=0.2)
    else:
        if nodeLabel:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=2,
                node_color='grey',
                with_labels=True,
                font_size=5, 
                width=0.2)
        else:
            nx.draw(
                g,
                pos=node_positions,
                edge_color=edge_colors,
                node_size=2,
                node_color='grey',
                with_labels=False,
                font_size=5, 
                width=0.2)

    if with_weight:
        labels = {e: g.edges[e]['weight'] for e in g.edges}
        nx.draw_networkx_edge_labels(g, pos=node_positions, edge_labels=labels)
    plt.title('Graph', size=15)
    print(outFile)
    plt.savefig(outFile, dpi=1000)


if __name__ == "__main__":
    arguments = len(sys.argv)-1
    longOptions = ['version', 'help', 'iterations', 'interval', 'colour', 'resolution', 'nolabel', 'noedges', 'linkConseqNodes', 'differentiateEdges', "noNodeColour"]
    options = "vhi:n:cr:lekdu"
    loop = False
    colour = False
    resolution_overwrite = False
    iterations = 100
    label = True
    no_edges = False
    link_consequetive_nodes = False
    distance_diff = False
    noNodeColour = False
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
        elif opt in ('-k', '--linkConseqNodes'):
            link_consequetive_nodes = True
        elif opt in ('-d', '--differentiateEdges'):
            distance_diff = True
        elif opt in ('-u', '--noNodeColour'):
            noNodeColour = True



    if loop:
        inPath = sys.argv[arguments-1]
        outPath = sys.argv[arguments]
        for i in range(0, iterations, interval):
            results = parseTxtFile(inPath+"_"+str(i)+".txt", colour, no_edges, link_consequetive_nodes, distance_diff, noNodeColour)
            if resolution_overwrite:
                plot(
                    results[0],
                    [resolution, resolution],
                    outPath+"_"+str(i)+".png",
                    results[2],
                    colour,
                    label,
                    noNodeColour)
            else:
                plot(
                    results[0],
                    results[1],
                    outPath+"_"+str(i)+".png",
                    results[2],
                    colour,
                    label,
                    noNodeColour)

    else:
        inFile = sys.argv[arguments-1]
        outFile = sys.argv[arguments]
        results = parseTxtFile(inFile, colour, no_edges, link_consequetive_nodes, distance_diff, noNodeColour)
        if resolution_overwrite:
            plot(results[0], [resolution, resolution], outFile, results[2], colour, label, noNodeColour)
        else:
            plot(results[0], results[1], outFile, results[2], colour, label, noNodeColour)