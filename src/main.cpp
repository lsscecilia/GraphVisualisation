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

void parseTxtFile(string path, vector<shared_ptr<Vertex>>& vertices,vector<vector<bool>>& edges){
  string text, n1, n2;
  std::unordered_map<string, int> table; 
  std::ifstream infile(path);
  double indexN1, indexN2; 
  int iN1, iN2, end; 
  bool prev=false; 
  vector<bool> temp; 
  vector<Vertex> vtemp; 
  while (getline (infile, text)) {
    prev = false; 
    //std::cout << text << endl;
    for (int i=0; i< text.size(); i++){
      if (text[i]== '-' && !prev){
        prev = true; 
        iN1 = i;
      }
      else if (text[i]!='-' && prev){
        iN2 = i; 
        prev = false; 
      }
      else if (text[i]=='\n'){
        end = i; 
      }
    }
    n1 = text.substr(0,iN1);
    n2 = text.substr(iN2, text.size()); 

    // Output the text from the file
    auto it1 = table.find(n1); 
    if (it1 == table.end()){
      //add n1 to table & assign index
      indexN1 = table.size(); 
      table.insert(make_pair(n1, table.size())); 
      vtemp.push_back({{0, 0}, {0,0}}); 
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
    }
    else{
      //get index
      indexN2 = it2->second; 
    }

    edges.push_back({}); 
    edges.push_back({}); 
    edges[indexN1].push_back(true); 
    edges[indexN2].push_back(true);  
  }
   

  for (int i=0;i<vtemp.size();i++){
		vertices.push_back(make_shared<Vertex>(vtemp[i])); 
	}

  std::cerr << "[GraphVisualisation] number of nodes.." << vertices.size() << endl;
}

void generateOutputFile(string inputPath,string path,vector<shared_ptr<Vertex>>& vertices, int width, int length){
  std::ifstream infile(inputPath); 
  std::ofstream outfile(path); 
  outfile << infile.rdbuf();
  outfile << "-" << endl; 
  for (int i=0; i<vertices.size();i++){
      outfile << i << "|(" << vertices[i]->pos.x << "," << vertices[i]->pos.y << ")" << std::endl;
  }
  outfile << "^" << endl; 
  outfile << "100,100"<< endl; 
}

void ProjectVersion(){
    std::cerr << "v" << PROJECT_VER << endl; 
}

void Help(){
    std::cerr << "some help commands" << endl; 
}

static struct option long_options[] = {
  /* These options don’t set a flag.
	 We distinguish them by their indices. */
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
  {"theta", required_argument, 0, 'e'}, 
  {0, 0, 0, 0}
}; 

int main(int argc, char * argv[]){
  int option_index = 0;
  int c=0; 
  int iterations=100, width = 10, length = 10; 
  int interval=0; 
  int algoType=0;  //default is barnes hut
  bool dynamic = true, random = false;
  double mass=10; 
  double theta=0.5; 

	while ((c = getopt_long (argc, argv, "vhsri:w:l:n:a:m:e:",
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
      
      case 'e':
        theta = atoi(optarg); 
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
      vector<vector<bool>> edges; 
      std::cerr << "[GraphVisualisation] Reading vertices" << std::endl;
      parseTxtFile(argv[optind], vertices, edges); 
      initVerticesPosition(vertices, width, length, random); 
      
      std::cerr << "[GraphVisualisation] calculating, iterations: " <<iterations << std::endl;
      
      string initpath; 
      if (interval==0){
        initpath = argv[optind+1]; 
        initpath +="original"; 
        //for debug only
        //generateOutputFile(argv[optind], initpath, vertices, width, length); 
        directedForceAlgorithm(vertices, edges, width,length,iterations, algoType, mass, dynamic);
        std::cerr << "[GraphVisualisation] Generating output" << endl; 
        generateOutputFile(argv[optind], argv[optind+1], vertices, width, length); 
      }

      else{
        std::cerr << "[GraphVisualisation] Generating output" << endl; 
        string outputPath, temp = argv[optind+1];
        int iterPerInterval = iterations/interval; 
        outputPath = temp + "_initial_random.txt";
        generateOutputFile(argv[optind], outputPath, vertices, width, length);
        for (int i=0; i<iterations; i+=interval){
          std::cerr << "[GraphVisualisation] after " << i << " iterations..." << endl; 
          outputPath = temp + "_" + to_string(i) +".txt";
          directedForceAlgorithm(vertices, edges, width,length,iterPerInterval, algoType, mass, dynamic);
          generateOutputFile(argv[optind], outputPath, vertices, width, length);
        }
      }
      std::cerr << "[GraphVisualisation] txt file generated" << std::endl;
    }
    return 0;
}

