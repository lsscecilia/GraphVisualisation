#include <iostream>
#include <fstream> 
#include <string> 
#include <getopt.h>
#include <unordered_map>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>

#include <config.h>
#include "algorithm.h"

using namespace std; 

void duplicateTxtFile(string file, string copyPath){
  ifstream inFile(file,       ios::binary);
  ofstream outFile(copyPath, ios::binary);
  outFile << inFile.rdbuf();
  outFile.close();
}

std::unordered_map<string, int> parseTxtFile(
  string path, 
  vector<shared_ptr<Vertex>>& vertices,
  vector<vector<double>>& edges, 
  string outputPath,
  bool with_coloring){
  string text, n1, n2;
  std::unordered_map<string, int> table; 
  std::ifstream infile(path);
  std::ofstream outfile;
  double indexN1, indexN2; 
  int iN1, iN2, end, wl, weight; 
  bool prev=false; 
  vector<bool> temp; 
  vector<Vertex> vtemp; 
  bool has_weight = false;
  outfile.open(outputPath);
  bool prev_color = false;
  int new_node=0;
  while (getline (infile, text)) {
    prev = false; 
    //std::cout << text << endl;
    
    if (with_coloring && !prev_color) {
      if (text[0]!='%'){
        outfile << text + "\n";
        continue;
      } else if (text[0]=='%') {
        outfile << text + "\n";
        prev_color = true;
        continue;
      }
    }
    for (int i=0; i< text.size(); i++){
      if (text[i]== '-' && !prev){
        prev = true; 
        iN1 = i;
      }
      else if (text[i]!='-' && prev){
        iN2 = i; 
        prev = false; 
      }
      else if (text[i]==','){
        has_weight = true;
        wl = i;
      }
      else if (text[i]=='\n'){
        end = i; 
      }
    }
    n1 = text.substr(0,iN1);
    if (!has_weight){
      int len = end - iN2; 
      n2 = text.substr(iN2, len);
    } else {
      int len = wl - iN2;
      n2 = text.substr(iN2, len);
      weight = std::stoi(text.substr(wl+1,end-wl));
      /*
      if (weight<50)
        continue;*/
    }

    outfile << text + "\n";
    // Output the text from the file
    auto it1 = table.find(n1); 
    if (it1 == table.end()){
      //add n1 to table & assign index
      indexN1 = table.size(); 
      table.insert(make_pair(n1, table.size())); 
      vtemp.push_back({{0, 0}, {0,0}}); 
      new_node++;
    }
    else{
      //get index
      indexN1 = it1->second; 
    }

    auto it2 = table.find(n2); 
    if (it2 == table.end()){
      //add n1 to table & assign index
      indexN2 = table.size(); 
      table.insert(make_pair(n2, table.size())); 
      vtemp.push_back({{0, 0}, {0,0}});
      new_node++;
    }
    else{
      //get index
      indexN2 = it2->second; 
    }
    if (new_node > 0) {
      int num_edges = edges.size();
      for (auto& col : edges) {
        for (int i = 0; i < new_node; i++) {
          col.push_back(0);
        }
      }
      for (int i = 0; i < new_node; i++) {
        vector<double> temp(table.size(), 0);
        edges.push_back(temp);
      }

    }
    //edges.push_back({}); 
    if (has_weight){
      edges[indexN1][indexN2] = weight; 
      edges[indexN2][indexN1] = weight;  
    } else {
      edges[indexN1][indexN2] = 1;
      edges[indexN2][indexN1] = 1;  
    }
    new_node=0;
  }
  outfile.close();

  for (int i=0;i<vtemp.size();i++){
		vertices.push_back(make_shared<Vertex>(vtemp[i])); 
	}

  std::cerr << "[GraphVisualisation] number of nodes.." << vertices.size() << endl;
  return table;
}

void ModerateEdges(vector<vector<double>>& edges, int numNode){
  int max=0;
  for (auto r: edges){
    for (auto c: r){
      if (c>max)
        max = c;
    }
  }
  //cerr << "max" << max << endl;
  for (int i=0; i<numNode;i++){
    for (int r=0; r<numNode;r++){
      //cerr << i << "," << r << endl;
      if (edges[i][r]!=0)
        edges[i][r] = (double) edges[i][r]/max;
    }
  }
}

void generateOutputFile(string inputPath,string path,vector<shared_ptr<Vertex>>& vertices, int width, int length, std::unordered_map<string, int> mapTable){
  std::ifstream infile(inputPath); 
  std::ofstream outfile(path, std::ios_base::app); 
  //outfile << infile.rdbuf();
  outfile << "-" << endl; 
  for (int i=0; i<vertices.size();i++){
      string vertex;
      for (auto map: mapTable){
        if (map.second==i){
          vertex = map.first;
          break;
        }
      }
      outfile << vertex << "|(" << vertices[i]->pos.x << "," << vertices[i]->pos.y << ")" << std::endl;
  }
  outfile << "^" << endl; 
  outfile << "50,50"<< endl; 
}

void ProjectVersion(){
    std::cerr << "v" << PROJECT_VER << endl; 
}

void Help(){
    std::cerr << "some help commands" << endl; 
}

static struct option long_options[] = {
  {"version", no_argument,       0, 'v'},
  {"help",  no_argument,       0, 'h'},
  {"iterations",  required_argument, 0, 'i'},
  {"width",  required_argument, 0, 'w'},
  {"length",    required_argument, 0, 'l'},
  {"interval", required_argument, 0, 'n'} ,
  {"algorithm", required_argument, 0, 'a'},
  {"static", no_argument, 0, 's'}, 
  {"random", no_argument, 0, 'r'},
  {"mass", required_argument, 0, 'm'}, 
  {"color", no_argument, 0, 'c'},
  {"theta", required_argument, 0, 't'},
  {0, 0, 0, 0}
}; 

int main(int argc, char * argv[]){
  int option_index = 0;
  int c=0; 
  int iterations=100, width = 10, length = 10; 
  int interval=0; 
  int algoType=0;  //default is barnes hut
  bool dynamic = true, random = false, color = false;
  double mass=10, theta = 0.5; 

	while ((c = getopt_long (argc, argv, "vhsri:w:l:n:a:m:ct:",
				   long_options, &option_index)) != -1){
	
		switch (c) {
			case 'v':
			  ProjectVersion(); 
			  break;

			case 'h':
			  Help(); 
			  break;

			case 'i':
				iterations = atoi(optarg);	
			  break;

			case 'w':
				width = atoi(optarg);
			  break;

			case 'l':
				length = atoi(optarg);
				break;
      
      case 'n':
        interval = atoi(optarg); 
        break;

      case 'a':
        algoType = atoi(optarg); 
        break; 

      case 's':
        dynamic = false; 
        break; 
      case 'r': 
        random = true; 
        break; 
      case 'm':
        mass = atoi(optarg); 
        break; 
      case 'c':
        color = true;
        break;

      case 't':
        theta = stod(optarg);
        break;
			case '?':
			  /* getopt_long already printed an error message. */
			  break;

			default:
			  //abort();
			  break;	
		}
	}
  if (optind < argc){
    std::ifstream infile(argv[optind]);
    std::ofstream outfile(argv[optind+1]);
    vector<shared_ptr<Vertex>> vertices; 
    vector<vector<double>> edges;
    std::cerr << "[GraphVisualisation] Reading vertices" << std::endl;
    std::unordered_map<string, int> map_table;
    string output;
    if (interval==0){
      map_table = parseTxtFile(argv[optind], vertices, edges, argv[optind+1],color); 
      initVerticesPosition(vertices, width, length, random); 
    } else {
      output = argv[optind+1]; 
      output +=  ".txt";
      map_table = parseTxtFile(argv[optind], vertices, edges,output,color); 
      initVerticesPosition(vertices, width, length, random); 
    }

    //ModerateEdges(edges, vertices.size());

    /*
    for (auto r:edges){
      for (auto c:r){
        cerr << c << " ";
      }
      cerr<<endl;
    }*/
    
    std::cerr << "[GraphVisualisation] calculating, iterations: " <<iterations << std::endl;
    
    string initpath; 
    if (interval==0){
      initpath = argv[optind+1]; 
      initpath +="original"; 
      //for debug only
      //generateOutputFile(argv[optind], initpath, vertices, width, length); 
      directedForceAlgorithm(vertices, edges, width,length,iterations, algoType, theta, mass, dynamic);
      std::cerr << "[GraphVisualisation] Generating output" << endl; 
      generateOutputFile(argv[optind], argv[optind+1], vertices, width, length, map_table); 
    }

    else{

      std::cerr << "[GraphVisualisation] Generating output" << endl; 
      string outputPath, temp = argv[optind+1];
      int iterPerInterval = iterations/interval; 
      outputPath = temp + "_0.txt";
      duplicateTxtFile(output,outputPath);
      generateOutputFile(argv[optind], outputPath, vertices, width, length, map_table);
      for (int i=interval; i<=iterations; i+=interval){
        std::cerr << "[GraphVisualisation] after " << i << " iterations..." << endl; 
        outputPath = temp + "_" + to_string(i) +".txt";
        duplicateTxtFile(output,outputPath);
        directedForceAlgorithm(vertices, edges, width,length,iterPerInterval, algoType, theta, mass, dynamic);
        generateOutputFile(argv[optind], outputPath, vertices, width, length, map_table);
      }
      generateOutputFile(argv[optind], output, vertices, width, length, map_table);
    }
    std::cerr << "[GraphVisualisation] txt file generated" << std::endl;
  }
  return 0;
}

