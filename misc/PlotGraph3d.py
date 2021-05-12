import sys, getopt
import re
import math
import statistics

import networkx as nx
import matplotlib.pyplot as plt
import plotly.graph_objects as go
import numpy as np
import scipy.stats as stats
import plotly.express as px

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
    nodes = []
    for i in range(brk+1, len(lines)):
        if lines[i][0] == '^':
            brk = i + 1
            break

        v = re.split("\n|[|]|[)]|[(]|,", lines[i])

        nodes.append([float(v[2]),float(v[3]), float(v[4])])

        if link_consequetive_nodes:
            if int(v[0]) > max_:
                max_ = int(v[0])
            if int(v[0]) < min_:
                min_ = int(v[0])

        if distance_diff:
            node_pos_list[v[0]] =[float(v[2]), float(v[3]), float(v[4])]

        if noNodeColour:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]), Z=float(v[4]))
        elif colour:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]), Z=float(v[4]), Color=node_color[v[0]])
        else:
            g.add_node(v[0], X=float(v[2]), Y=float(v[3]), Z=float(v[4]))

    #node_positions = {node[0]: (node[1]['X'], node[1]['Y']) for node in g.nodes(data=True)}

    if link_consequetive_nodes:
        prev = False
        node_list = []
        for i in range(min_, max_, 1):
            if str(i) in g.nodes and str(i-1) in g.nodes:
                g.add_edge(str(i), str(i-1), Color='black')
                #print(str(i) + "--" + str(i-1))
        print("conseq nodes")
        print("num edges: ", len(g.edges))
                
    #size
    for r in range(len(lines[brk])):
        if lines[brk][r] == ',':
            comma = r
        if lines[brk][r] == '\n':
            end = r
    lines[brk]
    return [g, [int(lines[brk][:comma]), int(lines[brk][comma+1:end])], with_weight]


def plot(g, coor, outFile, with_weight, colour, nodeLabel, noNodeColour):
    #print('plotting...')
    #for e in g.nodes(data=True):
     #   print(e)
    # Define node positions data structure (dict) for plotting
    node_positions = {node[0]: (node[1]['X'], node[1]['Y'], node[1]['Z']) for node in g.nodes(data=True)}
    x_nodes = [node[1]['X'] for node in g.nodes(data=True)]# x-coordinates of nodes
    y_nodes = [node[1]['Y'] for node in g.nodes(data=True)]# y-coordinates
    z_nodes = [node[1]['Z'] for node in g.nodes(data=True)]# z-coordinates

    
    #we  need to create lists that contain the starting and ending coordinates of each edge.
    x_edges=[]
    y_edges=[]
    z_edges=[]
    #print(g.edges(data=True))

    #need to fill these with all of the coordiates
    for edge in g.edges(data=True):
        #format: [beginning,ending,None]
        x_coords = [node_positions[edge[0]][0],node_positions[edge[1]][0],None]
        x_edges += x_coords

        y_coords = [node_positions[edge[0]][1],node_positions[edge[1]][1],None]
        y_edges += y_coords

        z_coords = [node_positions[edge[0]][2],node_positions[edge[1]][2],None]
        z_edges += z_coords
    
    trace_edges = go.Scatter3d(x=x_edges,
                        y=y_edges,
                        z=z_edges,
                        mode='lines',
                        line=dict(color='blue', width=2),
                        hoverinfo='none')
    #create a trace for the nodes
    trace_nodes = go.Scatter3d(x=x_nodes,
                            y=y_nodes,
                            z=z_nodes,
                            mode='markers',
                            marker=dict(symbol='circle',
                                        size=2,
                                        color='grey', #color the nodes according to their community
                                        colorscale=['lightgreen','magenta'], #either green or mageneta
                                        line=dict(color='blue', width=0.5)),
                            hoverinfo='text')
    
    #we need to set the axis for the plot 
    axis = dict(showbackground=False,
                showline=False,
                zeroline=False,
                showgrid=False,
                showticklabels=False,
                title='')

    v = re.split(".html|/", outFile)
    title = v[len(v)-2]
    print(title)
    #also need to create the layout for our plot
    layout = go.Layout(title=title,
                    width=650,
                    height=625,
                    showlegend=False,
                    scene=dict(xaxis=dict(axis),
                            yaxis=dict(axis),
                            zaxis=dict(axis),
                            ),
                    margin=dict(t=100),
                    hovermode='closest')
    
    #Include the traces we want to plot and create a figure
    data = [trace_edges, trace_nodes]
    fig = go.Figure(data=data, layout=layout)
  
    fig.write_html(outFile)
    fig.show()


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