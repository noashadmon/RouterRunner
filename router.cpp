// Author: Sean Davis

#include "router.h"
#include "RouterRunner.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ostream>
using namespace std;
Router::Router(CityInfo *infos, int numCities) 
{
  Num = numCities;
  CInfo = new CityInfo[Num];
  memcpy(CInfo, infos, Num * sizeof(CityInfo));
  
  for(int i = 0; i < Num; i++)
    {
      Net[i] = CInfo[i].production - CInfo[i].usage;
    }//initilizing nets

   
} // Router()


int Router::setTransfers(Transfer **transfers)
{
  int front, back, Tfers = 0, City;
  int Tamount = 0, Famount = 0;
  //int d = 0;  
  for(int i = 0; i < Num; i++)
  {
    //cout << "c " << i << " Onet " << Net[i]<<endl; 
    for(int j = 0; j < CInfo[i].adjCount; j++)
     {
       transfers[i][j].destCity = CInfo[i].adjList[j];
       transfers[i][j].amount = 0;
     }
  }//initializing transfer matrix

  int *adjListPtr;
  Transfer *transfersPtr;
  int *ptrNetI, *ptrNetCity;
  int maxJ;
  PrevV *prevCityPtr;
  
 for(int i = 0; i < Num; i++)
  {
    
adjListPtr = &CInfo[i].adjList[0];
transfersPtr = &transfers[i][0];
ptrNetI = &Net[i];
maxJ = CInfo[i].adjCount;

      for(int j = 0; j < maxJ; j++)
      {
        if(Net[*adjListPtr] < 0)
         {
          //transfers[i][j].destCity = *adjListPtr;
          Famount = Net[*adjListPtr];
          if(-Famount <= *ptrNetI )
            {
             (*transfersPtr).destCity = *adjListPtr;
             (*transfersPtr).amount -= Famount;
             *ptrNetI  += Famount;
             Net[*adjListPtr] = 0;
             Tfers -= Famount;
            }
          else if(-Famount > *ptrNetI )
            {
             (*transfersPtr).destCity = *adjListPtr;
             (*transfersPtr).amount = *ptrNetI ;
             Net[*adjListPtr] += *ptrNetI;
             //Net[i] = 0;
             Tfers += *ptrNetI ;
             *ptrNetI  = 0;
            }
         }//if adjacent city's net power is negetive trasfer power

       ++adjListPtr;
       ++transfersPtr;

}//for loop to access cities

  }


  for(int i = 0; i < Num; i++)
  {
    if(Net[i] > 0)
     {
       PV[i].city = PV[i].pos = -1;
       memset(visited, false, Num * sizeof(bool));
       memset(depth, 0, Num * sizeof(int));
       visited[i] = true;
       front = back = 0;
       depth[i] = 0;
       Queue[back++] = i;
       int d = 1;
       City = i;
       
       while(Net[i] != 0)
       {
       maxJ = CInfo[City].adjCount;
       adjListPtr = &CInfo[City].adjList[0];
             for(int j = 0; j < maxJ; j++)
          {
            if(!visited[*adjListPtr])
             {
               visited[*adjListPtr] = true;
               PV[*adjListPtr].city = City;
               PV[*adjListPtr].pos = j;
               depth[*adjListPtr] = d;
               Queue[back++] = *adjListPtr;
             }//not visited
             ++adjListPtr;
          }//for unvisited adj vertex

         City = Queue[front++];
         //cout << City;
         ptrNetCity = &Net[City];
         prevCityPtr = &PV[City];
         if(*ptrNetCity < 0)
         {
           int l = City;
           int h = (*prevCityPtr).pos;
           int k = (*prevCityPtr).city;
           Tamount = *ptrNetCity;
            
       
 
         // cout << "l-city " << l << " h=pv-pos " << h << " k-pv-city " << k<<endl;
            while(depth[l] > 0)
           {
           transfersPtr = &transfers[k][h];
             //cout << " h " <<h<< " k " <<k<< " l " <<l<<endl;
             if(-Tamount <= Net[i])
              {
                (*transfersPtr).destCity = l; 
                (*transfersPtr).amount -= Tamount;
                Net[k] += Tamount;
                Net[l] -= Tamount;
                Tfers -= Tamount;      
              }
              else
              {
                //cout << "help " << " h " << h << " k " << " l "<<endl;
                (*transfersPtr).destCity = l; 
                (*transfersPtr).amount += Net[i];
                Net[l] += Net[i];
                Tfers += Net[i];
                Net[k] -= Net[i];
                //Net[i] = 0; 
              }
              
              l = k;
              h = PV[k].pos;
              k = PV[k].city;
              ++transfersPtr;
           }
         }// if needy found
         
          d++;

       }//while net of source is pos
     }//postive Net city found
  }//BFS at every positive Net


  return (Tfers);  // should be set to total of all transfers.
}  // setTransfers
