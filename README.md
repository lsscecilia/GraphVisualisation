[![Build Status](https://travis-ci.com/lsscecilia/GraphVisualisation.svg?branch=main)](https://travis-ci.com/lsscecilia/GraphVisualisation)

# Graph  Drawing  by  Force-directed  Placement

## Input
- in .txt file 
- format: 
`1--2` <br/>
where 1 & 2 are nodes & there is an edge joining 1 & 2 

## How to run? 

### To generate final graph only 

`./build/bin/graph_visualisation {input folder}/{name}.txt  {output folder}/{name}.txt`

`python3 misc/PlotGraph.py {output folder}/{name}.txt {png output folder}/{name}.png`

### To generate process & final graph 

`./build/bin/graph_visualisation -n {interval} {input folder}/{name}.txt {output folder}/{name}`

`python3 misc/PlotGraph.py -n {interval} {output folder}/{name} {png output folder}/{name}`

### Usage 

-h help <br/>
-v version <br/>
-i number of iterations, default = 100 <br/>
-w width, default = 10 <br/>
-l length, default = 10 <br/>
-n interval, default = 0 --> used only when you want to generate process 

## References
reference: http://citeseer.ist.psu.edu/viewdoc/download;jsessionid=19A8857540E8C9C26397650BBACD5311?doi=10.1.1.13.8444&rep=rep1&type=pdf
