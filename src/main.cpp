#include <iostream>
#include <fstream> 
#include <string> 
#include <getopt.h>
#include <unordered_map>
#include <string.h>

#include <config.h>
#include "algorithm.h"

using namespace std; 

void parseTxtFile(string path, vector<Vertex>& vertices,vector<vector<bool>>& edges){
  string text, n1, n2;
  std::unordered_map<string, int> table; 
  std::ifstream infile(path);
  double indexN1, indexN2; 
  int iN1, iN2, end; 
  bool prev=false; 
  vector<bool> temp; 

  while (getline (infile, text)) {
    std::cout << text << endl;
    for (int i=0; i< text.size(); i++){
      if (text[i]== '-' && !prev){
        prev = true; 
        iN1 = i;
      }
      else if (text[i]!='-' && prev){
        iN2 = i; 
      }
      else if (text[i]=='\n'){
        end = i; 
      }
    }
    std::cout << iN1 << endl; 
    std::cout << iN2 << endl;
    std::cout << text.size() << endl; 
    cerr << text.substr(0,1) << endl; 
    cerr << text[0] << endl; 
    cerr << iN1 << endl; 
    n1 = text.substr(0,iN1);
    //n1 = text[0]; 
    
    
    n2 = text.substr(iN2, text.size()); 

    cerr << "node1: " << n1 << endl; 
    cerr << "node2: " << n2 << endl; 

    // Output the text from the file
    auto it1 = table.find(n1); 
    if (it1 == table.end()){
      //add n1 to table & assign index
      indexN1 = table.size(); 
      table.insert(make_pair(n1, table.size())); 
      vertices.push_back({{0, 0}, {0,0}}); 
    }
    else{
      //get index
      indexN1 = it1->second; 
    }

    cerr <<"table size" << table.size() << endl;

    auto it2 = table.find(n2); 
    if (it2 == table.end()){
      //add n1 to table & assign index
      indexN2 = table.size(); 
      table.insert(make_pair(n2, table.size())); 
      vertices.push_back({{0, 0}, {0,0}});
    }
    else{
      //get index
      indexN2 = it2->second; 
    }

    cerr <<" error" << endl;
    cerr <<"table size" << table.size() << endl;

    
    cerr <<" error 1" << endl;
    edges.push_back({}); 
    edges.push_back({}); 
    edges[indexN1].push_back(true); 
    edges[indexN2].push_back(true);  
    cerr <<" error 2" << endl;
    //edges[indexN2][indexN1] = true;  
    cerr <<" error 3" << endl;
    //cout << text;
  }
}

void ProjectVersion(){
    cerr << "v" << PROJECT_VER << endl; 
}

void Help(){
    cerr << "some help commands" << endl; 
}

static struct option long_options[] = {
  /* These options don’t set a flag.
	 We distinguish them by their indices. */
  {"version", no_argument,       0, 'v'},
  {"help",  no_argument,       0, 'h'},
  {"iterations",  required_argument, 0, 'i'},
  {"width",  required_argument, 0, 'w'},
  {"length",    required_argument, 0, 'l'},
  {0, 0, 0, 0}
}; 

int main(int argc, char * argv[])
{
    int option_index = 0;
	int c=0; 
    int iterations=100, width = 10, length = 10; 

	while ((c = getopt_long (argc, argv, "vhi:w:l:",
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
        vector<Vertex> vertices; 
        vector<vector<bool>> edges; 
        std::cerr << "[GraphVisualisation] Reading vertices" << std::endl;
        parseTxtFile(argv[optind], vertices, edges); 
        initVerticesPosition(vertices, width, length); 
        std::cout  << "num vertices: " << vertices.size() << endl; 
        for (int i=0; i<vertices.size();i++){
          cerr << "pos: " << vertices[i].pos.x << "," << vertices[i].pos.y << endl; 
        }

        int numEdge=0; 
        for (int i=0; i<vertices.size(); i++){
          for (int r=0; r<i; r++){
            if (edges[i][r])
              numEdge++; 
          }
        }
        std::cout << "num edges: " << numEdge << endl; 

        std::cerr << "[GraphVisualisation] calculating" << std::endl;
        directedForceAlgorithm(vertices, edges, 7,10,100); 
              
        std::cerr << "[GraphVisualisation] Generating output" << endl; 
        outfile << infile.rdbuf();
        //outfile.open(argv[optind+1], std::ios_base::app);
        outfile << "-" << endl; 
        for (int i=0; i<vertices.size();i++){
            outfile << i << "|(" << vertices[i].pos.x << "," << vertices[i].pos.y << ")" << std::endl;
        }
        outfile << "^" << endl; 
        outfile << width<< "," << length << endl; 
        std::cerr << "[GraphVisualisation] txt file generated" << std::endl;
    }
    return 0;
}

