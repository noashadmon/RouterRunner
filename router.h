#ifndef ROUTER_H
#define	ROUTER_H

#include "RouterRunner.h"

typedef struct
{
  int city;
  int pos;
} PrevV;


class Router {
public:
  int Num;
  CityInfo *CInfo;
  PrevV PV[25000];
  bool visited[25000];
  int Net[25000];
  int Queue[25000];
  int depth[25000];
  Router(CityInfo *infos, int numCities);
  int setTransfers(Transfer **transfers);
}; // class Router 

#endif	// ROUTER_H

