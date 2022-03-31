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
} dt0;

int cost[4];


/* students to write the following two routines, and maybe some others */

void rtinit0() 
{
  printf("\ninit0:_____________________");
  dt0.costs[0][0] = 0;
  dt0.costs[0][1] = 1;
  dt0.costs[0][2] = 3;
  dt0.costs[0][3] = 7;
  struct rtpkt pkt;
  pkt.sourceid = 0;
  for (int i = 0; i < 4; i++) {
    pkt.mincost[i] = dt0.costs[0][i]; 
  }
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dt0.costs[i][j] = 999;      
    }
    pkt.destid = i;
    tolayer2(pkt);
  }

  printf("\ndt0:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt0.costs[i][j]);
    }
    printf("]\n");
  }
  printf("___________________________\n");
}


void rtupdate0(rcvdpkt)
  struct rtpkt *rcvdpkt;
{
  extern struct event *evlist;
  printf("\nupdate0:___time:%f___", evlist->evtime);
  cost[0] = 0;
  cost[1] = 1;
  cost[2] = 3;
  cost[3] = 7;
  for (int i = 0; i < 4; i++) {
    dt0.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
  }
  int changed = 0;
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if ((dt0.costs[i][j]+cost[i]) < dt0.costs[0][j]) {
	//printf("\n(costs[%d][%d]=%d) + (cost[%d]=%d) <? costs[0][%d]=%d\n",i, j,  dt0.costs[i][j], i, cost[i], j, dt0.costs[0][j]);
	dt0.costs[0][j] = dt0.costs[i][j]+cost[i];
	changed = 1;	
      }
    }
  }
  if (changed) {
    printf("\ndt changed\n");
    struct rtpkt pkt;
    pkt.sourceid = 0;
    for (int i = 0; i < 4; i++) {
      pkt.mincost[i] = dt0.costs[0][i];
    }
    for (int i = 1; i < 4; i++) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }
  
  printf("\nNode0 Recieved:\n");
  printf("Source:%d, costs[%d,%d,%d,%d]\n", rcvdpkt->sourceid, rcvdpkt->mincost[0], rcvdpkt->mincost[1], rcvdpkt->mincost[2], rcvdpkt->mincost[3]);

  printf("\ndt0:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt0.costs[i][j]);
    }
    printf("]\n");
  }
  //printdt0(&dt0);
  printf("___________________________\n");
}


printdt0(dtptr)
  struct distance_table *dtptr;
  
{
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n",dtptr->costs[1][1],
	 dtptr->costs[1][2],dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n",dtptr->costs[2][1],
	 dtptr->costs[2][2],dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][1],
	 dtptr->costs[3][2],dtptr->costs[3][3]);
}

linkhandler0(linkid, newcost)   
  int linkid, newcost;

/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}

