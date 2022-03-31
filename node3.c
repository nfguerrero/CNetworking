#include <stdio.h>

extern struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct rtpkt *rtpktptr; /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
 };

extern int TRACE;
extern int YES;
extern int NO;

struct distance_table 
{
  int costs[4][4];
} dt3;

int cost[4];

/* students to write the following two routines, and maybe some others */

void rtinit3() 
{
  printf("\ninit3:_____________________");
  dt3.costs[3][0] = 7;
  dt3.costs[3][1] = 999;
  dt3.costs[3][2] = 2;
  dt3.costs[3][3] = 0;
  struct rtpkt pkt;
  pkt.sourceid = 3;
  for (int i = 0; i < 4; i++) {
    pkt.mincost[i] = dt3.costs[3][i]; 
  }
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      dt3.costs[i][j] = 999;      
    }
    if (i != 1) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }

  printf("\ndt3:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt3.costs[i][j]);
    }
    printf("]\n");
  }
  printf("___________________________\n");
}


void rtupdate3(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  extern struct event *evlist;
  printf("\nupdate3:___time:%f___", evlist->evtime);
  cost[0] = 7;
  cost[1] = 999;
  cost[2] = 2;
  cost[3] = 0;
  for (int i = 0; i < 4; i++) {
    dt3.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
  }
  int changed = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      if ((dt3.costs[i][j]+cost[i]) < dt3.costs[3][j]) {
	dt3.costs[3][j] = dt3.costs[i][j]+cost[i];
	changed = 1;
      }
    }
  }
  if (changed) {
    printf("\ndt changed\n");
    struct rtpkt pkt;
    pkt.sourceid = 3;
    for (int i = 0; i < 4; i++) {
      pkt.mincost[i] = dt3.costs[3][i];
    }
    for (int i = 0; i < 3; i++) {
      if (i != 1) {
	pkt.destid = i;
	tolayer2(pkt);
      }      
    }
  }
  
  printf("\nNode3 Recieved:\n");
  printf("Source:%d, costs[%d,%d,%d,%d]\n", rcvdpkt->sourceid, rcvdpkt->mincost[0], rcvdpkt->mincost[1], rcvdpkt->mincost[2], rcvdpkt->mincost[3]);

  printf("\ndt3:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt3.costs[i][j]);
    }
    printf("]\n");
  }
  //printdt3(&dt3);
  printf("___________________________\n");
}


printdt3(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via     \n");
  printf("   D3 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 1|  %3d   %3d\n",dtptr->costs[1][0], dtptr->costs[1][2]);
  printf("     2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);

}







