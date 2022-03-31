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
} dt2;

int cost[4];

/* students to write the following two routines, and maybe some others */

void rtinit2() 
{
  printf("\ninit2:_____________________");
  dt2.costs[2][0] = 3;
  dt2.costs[2][1] = 1;
  dt2.costs[2][2] = 0;
  dt2.costs[2][3] = 2;
  struct rtpkt pkt;
  pkt.sourceid = 2;
  for (int i = 0; i < 4; i++) {
    pkt.mincost[i] = dt2.costs[2][i]; 
  }
  for (int i = 0; i < 4; i++) {
    if (i == 2){i++;}
    for (int j = 0; j < 4; j++) {      
      dt2.costs[i][j] = 999;      
    }
    pkt.destid = i;
    tolayer2(pkt);
  }

  printf("\ndt2:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt2.costs[i][j]);
    }
    printf("]\n");
  }
  printf("___________________________\n");
}


void rtupdate2(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  extern struct event *evlist;
  printf("\nupdate2:___time:%f___", evlist->evtime);
  cost[0] = 3;
  cost[1] = 1;
  cost[2] = 0;
  cost[3] = 2;
  for (int i = 0; i < 4; i++) {
    dt2.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
  }
  int changed = 0;
  for (int i = 0; i < 4; i++) {
    if (i == 2){i++;}
    for (int j = 0; j < 4; j++) {
      if ((dt2.costs[i][j]+cost[i]) < dt2.costs[2][j]) {
	dt2.costs[2][j] = dt2.costs[i][j]+cost[i];
	changed = 1;
      }
    }
  }
  if (changed) {
    printf("\ndt changed\n");
    struct rtpkt pkt;
    pkt.sourceid = 2;
    for (int i = 0; i < 4; i++) {
      pkt.mincost[i] = dt2.costs[2][i];
    }
    for (int i = 0; i < 4; i++) {
      if (i == 2){i++;}
      pkt.destid = i;
      tolayer2(pkt);
    }
  }
  
  printf("\nNode2 Recieved:\n");
  printf("Source:%d, costs[%d,%d,%d,%d]\n", rcvdpkt->sourceid, rcvdpkt->mincost[0], rcvdpkt->mincost[1], rcvdpkt->mincost[2], rcvdpkt->mincost[3]);

  printf("\ndt2:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt2.costs[i][j]);
    }
    printf("]\n");
  }
  //printdt2(&dt2);
  printf("___________________________\n");
}


printdt2(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}







