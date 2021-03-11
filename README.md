[![Build Status](https://travis-ci.com/lsscecilia/GraphVisualisation.svg?branch=main)](https://travis-ci.com/lsscecilia/GraphVisualisation)

# Graph  Drawing  by  Force-directed  Placement

## Input
- in .txt file 
- format: 
`1--2` <br/>
where 1 & 2 are nodes & there is an edge joining 1 & 2 
- example input and output can be found under input & output folder above

## Install 
`git clone https://github.com/lsscecilia/GraphVisualisation.git`

`cd GraphVisualisation`

`cmake -H. -Bbuild `

`cmake --build build -- -j3`

## How to run? 

### To generate final graph only 

`./build/bin/graph_visualisation {input folder}/{name}.txt  {output folder}/{name}.txt`

`python3 misc/PlotGraph.py {output folder}/{name}.txt {png output folder}/{name}.png`

### To generate process & final graph 

`./build/bin/graph_visualisation -n {interval} {input folder}/{name}.txt {output folder}/{name}`

`python3 misc/PlotGraph.py -n {interval} {output folder}/{name} {png output folder}/{name}`

### Usage 

```bash
-h help 
-v version 
-s static tree (only for barnes hut) 
-r random initial position (only for barnes hut) 
-i number of iterations, default = 100 
-w width, default = 10 
-l length, default = 10 
-n interval, default = 0 --> used only when you want to generate process 
-a algorithm type, default = 0 (barnes hut) | other option: 1 (brute force) 
-m mass, default = 1 (only for barnes hut)
```

## Generate text file of complete graph

`${input folder}: python3 ../misc/GenerateGraph.py -n {numNodes}`

### Usage
-n numNodes, default = 10 
-c circular graph

## Generate graph with grid

`${input folder}: python3 ../misc/PlotGridGraph.py {input.txt} {output.png}`

## References
Fruchterman, T. M. J. & Reingold, E. M. Graph drawing by force-directed placement. Softw. Pract. Exp. 21 , 1129 – 1164 (1991). DOI: 10.1002/spe.4380211102 <br/>
<br/>
Barnes, J. & Hut, P. A hierarchical O(N log N) force-calculation algorithm. Nature 324 , 446 – 449 (1986). DOI: 10.1038/324446a0

