#include "TCPEchoServer.h"
#include <pthread.h>

#define MAX_CONN 4  // 最大接続数

/* Structure of arguments to pass to client thread */
struct ThreadArgs {
    int clntSock; /* Socket descriptor for client */
};

// int nconn; // 接続数
int ndone; // 終了したスレッド数
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER; // 相互排除変数 
pthread_cond_t ndone_cond = PTHREAD_COND_INITIALIZER; // 条件変数

void *ThreadMain(void *arg); /* Main program of a thread */

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    unsigned short echoServPort;     /* Server port */
    pthread_t threadID;              /* Thread ID from pthread_create() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    int maxconn = MAX_CONN;

    if (argc != 2) {
        fprintf(stderr,"Usage:  %s <SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* local port */
    servSock = CreateTCPServerSocket(echoServPort);

    // nconn = 0; // 接続数を0としておく
    for (;;) 
    {   
        while (ndone < maxconn) {
            clntSock = AcceptTCPConnection(servSock);

            /* Create separate memory for client argument */
            if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
                DieWithError("malloc() failed");
            threadArgs -> clntSock = clntSock;

            /* Create client thread */
            if (pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
                DieWithError("pthread_create() failed");
            printf("with thread %ld\n", (long int) threadID);

            // nconn++; 
            // printf("nconn: %d\n", nconn);
        }

        // if (pthread_mutex_lock(&ndone_mutex) != 0) // ndoneについてロック
        //     DieWithError("pthread_mutex_lock() failed.");

        // while (ndone == 0)
        //     if (pthread_cond_wait(&ndone_cond, &ndone_mutex))
        //         DieWithError("pthread_cond_wait() failed.");

        // if (pthread_mutex_unlock(&ndone_mutex) != 0) // ndoneについてアンロック
        //     DieWithError("pthread_mutex_unlock() failed.");
    }
    // exit(0);
}

void *ThreadMain(void *threadArgs)
{
    int clntSock; /* Socket descriptor for client connection */
    int val;

    // /* Guarantees that thread resources are deallocated upon return */
    pthread_detach(pthread_self());

    /* Extract socket file descriptor from argument */
    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    free(threadArgs); /* Deallocate memory for argument */

    if (pthread_mutex_lock(&ndone_mutex) != 0)
        DieWithError("pthread_mutex_lock() failed.");
    val = ndone;
    ndone = val + 1;
    printf("ndone: %d\n", ndone);
    HandleTCPClient(clntSock, val+1);
    // if (pthread_cond_signal(&ndone_cond) != 0)
    //     DieWithError("pthread_cond_signal() failed.");
    if (pthread_mutex_unlock(&ndone_mutex) != 0)
        DieWithError("pthread_mutex_unlock() failed.");

     sleep(10); // 10分後

    if (pthread_mutex_lock(&ndone_mutex) != 0)
        DieWithError("pthread_mutex_lock() failed.");
    ndone--;
    // if (pthread_cond_signal(&ndone_cond) != 0)
    //     DieWithError("pthread_cond_signal() failed.");
    if (pthread_mutex_unlock(&ndone_mutex) != 0)
        DieWithError("pthread_mutex_unlock() failed.");

    return (NULL);
}
