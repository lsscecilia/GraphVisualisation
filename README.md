[![Build Status](https://travis-ci.com/lsscecilia/GraphVisualisation.svg?branch=main)](https://travis-ci.com/lsscecilia/GraphVisualisation)

# Graph  Drawing  by  Force-directed  Placement

## Input
- in .txt file 
- format: 
```bash
To have colour coding:
1 1
2 1
3 1
4 2
5 3
// node group
%
1--2,10
// where 1 & 2 are nodes & there is an edge joining 1 & 2 & weight = 10 (weight is optional)
1--2
// where 1 & 2 are nodes & there is an edge joining 1 & 2 (without weight)

- example input and output can be found under input & output folder above
```

## Install & Build
```bash
git clone https://github.com/lsscecilia/GraphVisualisation.git

cd GraphVisualisation

cmake -H. -Bbuild

cmake --build build -- -j3
```
## How to run? 

### To generate position of nodes:

```bash
./build/bin/graph_visualisation {input folder}/{name}.txt  {output folder}/{name}.txt

output will be in .txt file

usage:

-h help 
-v version 
-i number of iterations, default = 100 
-w width, default = 10 
-l length, default = 10 
-n interval, default = 0 --> used only when you want to generate process 
-a algorithm type, default = 0 (barnes hut) | other option: 1 (brute force) 
-c colour code

for barnes hut only
-s static tree 
-r random initial position 
-m mass, default = 1
-t theta, default = 0.5 --> threshold for estimation
```
### To plot Graph

#### Non-interactive Graph

```bash
python3 misc/PlotGraph.py {output folder}/{name}.txt {png output folder}/{name}.png`

usage: 

-h help 
-v version 
-n interval --> used when generate process (must match params of command that generate position of nodes)
-i iterations --> used when generate process
-c color, default = false --> used when there is color coding
-l no node label, default = false
-e no edges, default = false
```

#### Interactive Graph + Non-interactive Graph

```bash
python3 misc/PlotInteractiveGraph.py {input folder}/{name}.txt {output folder}/{name}

usage: 

-h help 
-v version 
-c color, default = false --> used when there is color coding
```

## Others 

### Generate text file of complete graph

```bash
${input folder}: python3 ../misc/GenerateGraph.py -n {numNodes}

Usage:

-h help 
-v version 
-n numNodes, default = 10 
-c circular graph
```

### Generate graph with grid

```bash
${input folder}: python3 ../misc/PlotGridGraph.py {input.txt} {output.png}

Usage:

-h help 
-v version
```

## References
Fruchterman, T. M. J. & Reingold, E. M. Graph drawing by force-directed placement. Softw. Pract. Exp. 21 , 1129 – 1164 (1991). DOI: 10.1002/spe.4380211102 <br/>
<br/>
Barnes, J. & Hut, P. A hierarchical O(N log N) force-calculation algorithm. Nature 324 , 446 – 449 (1986). DOI: 10.1038/324446a0

