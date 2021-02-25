#include <iostream>
#include <fstream> 
#include <string> 
#include <getopt.h>

#include <config.h>
#include "algorithm.h"

using namespace std; 

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
        std::cerr << "[GraphVisualisation] calculating" << std::endl;
        vector<Vertex> vertices; 
        vertices.push_back({{4,1}, {0,0}});
        vertices.push_back({{4.4,0.1}, {0,0}});
        vertices.push_back({{3.2,4.2}, {0,0}});
        vertices.push_back({{3.3,1}, {0,0}});
        vertices.push_back({{1.5,2.4}, {0,0}});

        vector<vector<bool>> edges; 
        edges.push_back({false,true,true,true, true}); 
        edges.push_back({true,false,true,true, true}); 
        edges.push_back({true,true,false,true, true}); 
        edges.push_back({true,true,true,false, true}); 
        edges.push_back({true,true,true,true, false}); 

        directedForceAlgorithm(vertices, edges, 7,10,100); 

        std::ofstream outfile;
        outfile.open(argv[optind], std::ios_base::app);
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