#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "decls.h"

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        u1[i] = i;
        u2[i] = (i+1)/N/2.0;
        v1[i] = (i+1)/N/4.0;
        v2[i] = (i+1)/N/6.0;
        y[i] = (i+1)/N/8.0;
        z[i] = (i+1)/N/9.0;
        x[i] = 0.0;
        w[i] = 0.0;
        for (j=0; j<N; j++) {
            A[i][j] = ((double) i*j)/N;
        }
    }
}

void print_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        fprintf(stdout, "%0.2lf ", w[i]);
        if (i%80 == 20) fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}


double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

main()
{
    double t_start, t_end;
    int i, j;

    init_array();

    IF_TIME(t_start = rtclock());

#pragma scop
    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            B[i][j] = A[i][j] + u1[i]*v1[j] + u2[i]*v2[j];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            x[i] = x[i] + beta* B[j][i]*y[j];


    for (i=0; i<N; i++)
        x[i] = x[i] + z[i];

    for (i=0; i<N; i++)
        for (j=0; j<N; j++)
            w[i] = w[i] + alpha* B[i][j]*x[j];
#pragma endscop

    IF_TIME(t_end = rtclock());
    IF_TIME(fprintf(stderr, "%0.6lfs\n", t_end - t_start));

    if (fopen(".test", "r")) {
        print_array();
    }

    return 0;
}