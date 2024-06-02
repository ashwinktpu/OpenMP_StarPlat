#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<atomic>
#include<cmath>
#include<algorithm>
#include<omp.h>
#include"graph.hpp"

using namespace std;

bool isFeasible(int u, int v,  graph &pattern, graph &target, vector<int> &mapping, vector<int> &reverseMapping) {
                           
                
    
    for (int edge = pattern.indexofNodes[u]; edge < pattern.indexofNodes[u+1]; edge ++) 
      {
        int nbr = pattern.edgeList[edge] ;
        if(mapping[nbr] != -1){
        
        bool found = false;
        
        for (int edge1 = target.indexofNodes[v]; edge1 < target.indexofNodes[v+1]; edge1 ++) 
        
        {
          int nbr1 = target.edgeList[edge1] ;
          if(mapping[nbr] == nbr1){
            found = true;
            break;
          }
        
        }
        if(!found){
          return false;
        }
        
        }
      
     }
     return true;
}

bool VF2Plus(graph &pattern, graph &target, vector<int> &mapping, vector<int> &reverseMapping, int depth = 0) {
    if (depth == pattern.num_nodes()) {
        return true;
    }

    int u = -1;
    for (int i = 0; i < pattern.num_nodes(); ++i) {
        if (mapping[i] == -1) {
            u = i;
            break;
        }
    }

    if (u == -1) return false;

    bool result = false;

    #pragma omp parallel for shared(result)
    for (int v = 0; v < target.num_nodes(); ++v) {
        if (result) 
          continue;

        if (reverseMapping[v] == -1 && isFeasible(u, v, pattern, target, mapping, reverseMapping)) {
            //#pragma omp critical
            {
                if (result) 
                  continue;

                mapping[u] = v;
                reverseMapping[v] = u;

                if (VF2Plus(pattern, target, mapping, reverseMapping, depth + 1)) {
                    result = true;
                }

                mapping[u] = -1;
                reverseMapping[v] = -1;
            }
        }
    }

    return result;
}






bool subgraphIsomorphism( graph &pattern, graph &target) {

    std::vector<int> mapping(pattern.num_nodes(), -1);
    std::vector<int> reverseMapping(target.num_nodes(), -1);
    

    return VF2Plus(pattern, target, mapping, reverseMapping);
}
 
 
 int main()
{

  graph target("/home/ashwina/Subgraph_isomorphism/openmp/input1.txt");
  target.parseGraph();
 // printf("%d", target.num_nodes());
 // printf("%d",target.num_edges());
 
  
  
  graph pattern("/home/ashwina/Subgraph_isomorphism/openmp/input2.txt");
  pattern.parseGraph();
  
  //printf("%d", pattern.num_nodes());
  //printf("%d",pattern.num_edges());
  
 
   double startTime = omp_get_wtime();
    if (subgraphIsomorphism(pattern, target)) {
        cout << "subgraph yes" << endl;
    } else {
        cout << "subgraph no" << endl;
    }
  double endTime = omp_get_wtime();
  printf("EXECUTION TIME %f \n", endTime - startTime);
  return 0;

}
