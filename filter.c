#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "unistd.h"
#include "ringbuf.h"

ringbuffer_t *rb;
static uint8_t databuf[(sizeof(ringbuffer_t) - 1 + 4096)];
volatile int done = 0;

void *writer (void *), *reader (void *);

int main(void) {
    int status;
    pthread_t tid_writer, tid_reader;

    rb = ringbuffer_alloc(sizeof(databuf), databuf);

    pthread_create(&tid_reader, NULL, reader, NULL);
    pthread_create(&tid_writer, NULL, writer, NULL);
 
    pthread_join (tid_writer, NULL);
    pthread_join (tid_reader, NULL);

    return 0;
}

void* reader(void* arg) {
    char bufferIn[512];
    size_t nRead;
    size_t wa;
    int eof = 0;

    while (!eof) {
        nRead = fread(bufferIn, 1 , 512, stdin);
        if (nRead != sizeof(bufferIn)) {
            eof = feof(stdin);
//            fprintf(stderr, "Done: %d\n", nRead);
            if (!eof) {
//                fputs("Reading error\n", stderr);
                exit(1);
            }
        }
        while ((wa = ringbuffer_write_avail(rb)) < nRead) {
            sleep(0);
        }
        ringbuffer_write(rb, bufferIn, nRead);
//        fputs("written\n", stderr);
        done = eof;
    }

    return 0;
}

void* writer(void* arg) {
    size_t ra;
    size_t len;
    char bufferOut[1024];
    while (1) {
        ra = ringbuffer_read_avail(rb);
        if (ra > 0) {
//            fputs("Reading\n", stderr);
            if (ra > sizeof(bufferOut)) {
                len = sizeof(bufferOut);
            } else {
                len = ra;
            }
            ringbuffer_read(rb, bufferOut, len);
            fwrite(bufferOut, 1, len, stdout);
        } else {
//            fputs("No reading\n", stderr);
            if (done) {
                break;
            }
        }
        sleep(0);
    }
    return 0;
}
