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
} dt1;

int cost[4];

/* students to write the following two routines, and maybe some others */


rtinit1() 
{
  printf("\ninit1:_____________________");
  dt1.costs[1][0] = 1;
  dt1.costs[1][1] = 0;
  dt1.costs[1][2] = 1;
  dt1.costs[1][3] = 999;
  struct rtpkt pkt;
  pkt.sourceid = 1;
  for (int i = 0; i < 4; i++) {
    pkt.mincost[i] = dt1.costs[1][i]; 
  }
  for (int i = 0; i < 4; i++) {
    if (i == 1){i++;}
    for (int j = 0; j < 4; j++) {
      dt1.costs[i][j] = 999;      
    }
    if (i != 3) {
      pkt.destid = i;
      tolayer2(pkt);
    }
  }

  printf("\ndt1:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt1.costs[i][j]);
    }
    printf("]\n");
  }
  printf("___________________________\n");
}


rtupdate1(rcvdpkt)
  struct rtpkt *rcvdpkt;
  
{
  extern struct event *evlist;
  printf("\nupdate1:___time:%f___", evlist->evtime);
  cost[0] = 1;
  cost[1] = 0;
  cost[2] = 1;
  cost[3] = 999;
  for (int i = 0; i < 4; i++) {
    dt1.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
  }
  int changed = 0;
  for (int i = 0; i < 4; i++) {
    if (i == 1){i++;}
    for (int j = 0; j < 4; j++) {
      if ((dt1.costs[i][j]+cost[i]) < dt1.costs[1][j]) {
	dt1.costs[1][j] = dt1.costs[i][j]+cost[i];
	changed = 1;
      }
    }
  }
  if (changed) {
    printf("\ndt changed\n");
    struct rtpkt pkt;
    pkt.sourceid = 1;
    for (int i = 0; i < 4; i++) {
      pkt.mincost[i] = dt1.costs[1][i];
    }
    for (int i = 0; i < 3; i++) {
      if (i == 1){i++;}
      pkt.destid = i;
      tolayer2(pkt);
    }
  }
  
  printf("\nNode1 Recieved:\n");
  printf("Source:%d, costs[%d,%d,%d,%d]\n", rcvdpkt->sourceid, rcvdpkt->mincost[0], rcvdpkt->mincost[1], rcvdpkt->mincost[2], rcvdpkt->mincost[3]);

  printf("\ndt1:\n");
  for (int i = 0; i < 4; i++) {
    printf("[");
    for (int j = 0; j < 4; j++) {
      printf("%d,", dt1.costs[i][j]);
    }
    printf("]\n");
  }
  //printdt1(&dt1);
  printf("___________________________\n");
}


printdt1(dtptr)
  struct distance_table *dtptr;
  
{
  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}



linkhandler1(linkid, newcost)   
int linkid, newcost;   
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
	
{
}


