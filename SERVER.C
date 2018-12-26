/*
** Server Process
*/

#define INCL_DOSMEMMGR
#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <dos.h>
#include <os2def.h>
#include <bse.h>			/* MS OS/2 function declarations     */

#define FIFO            0
#define FIRSTELEMENT    0                 /* First element in queue */
#define DUMMYPARAM      (long) 0          /* Dummy for DosQueueRead */

char ServerName[] = "\\QUEUES\\SERVER.QUE";

void main (int argc, char *argv[], char *envp[])
{
   USHORT   rc;                 /* a return code                */
   HQUEUE   QueueServer;        /* handle to the server's queue */
   USHORT   DataLength;         /* length of element received   */
   ULONG    DataAddress;	/* long to store address of queue info*/
   UCHAR    ElemPriority;       /* priority of element in queue */
   SEL	    SegSel;
   int i;
   struct   {
      PID   pidProcess;         /* Process ID                   */
      USHORT usEventCode;       /* I don't know                 */
   } PID_RequestID;
   printf( "server: Starting server process\n" );
   rc = DosCreateQueue( &QueueServer, FIFO, ServerName );
   printf( "rc from DosCreateQueue: %d\n", rc );

   /* Wait for shared segment to be created */
   while(DosGetShrSeg("\\SHAREMEM\\MYSEG",&SegSel))
   ;

   /* Read server data */
   rc = DosReadQueue( QueueServer, (PULONG) &PID_RequestID, &DataLength,
		      (PULONG) &DataAddress, FIRSTELEMENT, DCWW_WAIT,
		      &ElemPriority, (HSEM) DUMMYPARAM );
   printf( "rc from DosReadQueue: %d\n", rc );
   printf( "DosReadQueue data length: %d\n", DataLength );
   printf( "DosReadQueue data: %s\n",  (char far *)DataAddress);

   printf( "Quitting server process\n" );
   DosFreeSeg(SegSel);
   DosCloseQueue( QueueServer );
}
