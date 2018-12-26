/*
** Client Process
*/

#define INCL_DOSMEMMGR
#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <dos.h>
#include <os2def.h>
#include <bse.h>			/* MS OS/2 function declarations     */

char ServerMsg[]  = "Hello World!";
char ServerName[] = "\\QUEUES\\SERVER.QUE";

void main (int argc, char *argv[], char *envp[])
{
   USHORT   rc;                 /* a return code                */
   HQUEUE   QueueServer;        /* handle to the server's queue */
   USHORT   ServerQPID;         /* server's process ID          */
   SEL	   ShrSegSel;
   PSEL     SelPtr;
   printf( "Starting Client Process\n" );

   /* Open a shared segment to put string in */
   if(rc = DosAllocShrSeg(80,"\\SHAREMEM\\MYSEG",&ShrSegSel))
    { printf("error code is %d\n",rc);
      exit(1);
    }
   /* Build pointer to shared segment */

   FP_SEG(SelPtr) = ShrSegSel;
   FP_OFF(SelPtr) = 0x0000;

   /* Copy the string to the segment */

   strcpy(SelPtr,ServerMsg);

   /* wait for queue to open */

   while(rc = DosOpenQueue( &ServerQPID, &QueueServer, ServerName ))
      ;
   printf( "rc from DosOpenQueue: %d\n", rc );

   /* Write to the queue */

   rc = DosWriteQueue( QueueServer, 0, (strlen(ServerMsg)+1 ), (PBYTE)SelPtr, 0 );
   printf( "rc from DosWriteQueue: %d\n", rc );

   printf( "Quitting client process\n" );
   DosFreeSeg(ShrSegSel);
   DosCloseQueue( QueueServer );
}
