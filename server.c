#include "mytest.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include<signal.h>
#include<fcntl.h>

#define DATA_SIZE (1 << 20) * 4

int server_oriented_io() {

#ifdef TIMES
	struct timeval astime, aetime;
	long atime_result, atime_uresult;
	struct timeval io_stime, io_etime;
	struct timeval comm_stime, comm_etime;
	long io_time_result = 0;
	long comm_time_result = 0;
#endif

#ifdef TIMES
	gettimeofday(&astime, NULL);
#endif

	int i, j;
	char chunk[4096];
	void* shmaddr1, * shmaddr2, * shmaddr3, * shmaddr4;

	int f1, f2, f3, f4;
	int ipd1, ipd2, ipd3, ipd4;
	int shmid1, shmid2, shmid3, shmid4;

	shmid1 = shmget(0251, 4096 * 256, IPC_CREAT | 0644);
	shmid2 = shmget(0252, 4096 * 256, IPC_CREAT | 0644);
	shmid3 = shmget(0253, 4096 * 256, IPC_CREAT | 0644);
	shmid4 = shmget(0254, 4096 * 256, IPC_CREAT | 0644);

	switch (fork())
	{
	case -1:
		perror("fork");
		exit(1);
		break;
	case 0:
		switch (fork())
		{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			switch (fork())
			{
			case -1:
				perror("fork");
				exit(1);
				break;
			case 0:			// Client 4 : p4.dat를 shmaddr4로 write
				f4 = open("p4.dat", O_RDONLY);
				shmaddr4 = shmat(shmid4, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					read(f4, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					memcpy(shmaddr4 + i * 4096, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Client 4 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr4);
				close(f4);
				exit(0);
				break;
			default:		// Client 3 : p3.dat를 shmaddr3로 write
				f3 = open("p3.dat", O_RDONLY);
				shmaddr3 = shmat(shmid3, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					read(f3, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					memcpy(shmaddr3 + i * 4096, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Client 3 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr3);
				close(f3);
				exit(0);
				break;
			}
			break;
		default:
			switch (fork())
			{
			case -1:
				perror("fork");
				exit(1);
				break;
			case 0:			// Client 2 : p2.dat를 shmaddr2로 write
				f2 = open("p2.dat", O_RDONLY);
				shmaddr2 = shmat(shmid2, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					read(f2, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					memcpy(shmaddr2 + i * 4096, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Client 2 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr2);
				close(f2);
				exit(0);
				break;
			default:		// Client 1 : p1.dat를 shmaddr1로 write
				f1 = open("p1.dat", O_RDONLY);
				shmaddr1 = shmat(shmid1, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					read(f1, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					memcpy(shmaddr1 + i * 4096, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Client 1 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr1);
				close(f1);
				exit(0);
				break;
			}
			break;
		}
		break;
	default:

		/*Child Process Waiting..*/
		sleep(1);

		switch (fork())
		{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			switch (fork())
			{
			case -1:
				perror("fork");
				exit(1);
				break;
			case 0:			// Server 4
				/**/
				ipd4 = open("io8.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				shmaddr1 = shmat(shmid1, NULL, 0);
				shmaddr2 = shmat(shmid2, NULL, 0);
				shmaddr3 = shmat(shmid3, NULL, 0);
				shmaddr4 = shmat(shmid4, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					for (j = 0; j < 256; j++)
					{
						memcpy(chunk + 16 * j, shmaddr1 + 4 * j + 4096 * i + (3 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 4, shmaddr2 + 4 * j + 4096 * i + (3 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 8, shmaddr3 + 4 * j + 4096 * i + (3 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 12, shmaddr4 + 4 * j + 4096 * i + (3 * 4 * 256), 4);
					}
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd4, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 4 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr1);
				shmdt(shmaddr2);
				shmdt(shmaddr3);
				shmdt(shmaddr4);
				close(ipd4);
				exit(0);
				break;
			default:		// Server 3
				/**/
				ipd3 = open("io7.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				shmaddr1 = shmat(shmid1, NULL, 0);
				shmaddr2 = shmat(shmid2, NULL, 0);
				shmaddr3 = shmat(shmid3, NULL, 0);
				shmaddr4 = shmat(shmid4, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					for (j = 0; j < 256; j++)
					{
						memcpy(chunk + 16 * j, shmaddr1 + 4 * j + 4096 * i + (2 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 4, shmaddr2 + 4 * j + 4096 * i + (2 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 8, shmaddr3 + 4 * j + 4096 * i + (2 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 12, shmaddr4 + 4 * j + 4096 * i + (2 * 4 * 256), 4);
					}
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd3, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 3 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr1);
				shmdt(shmaddr2);
				shmdt(shmaddr3);
				shmdt(shmaddr4);
				close(ipd3);
				exit(0);
				break;
			}
			break;
		default:
			switch (fork())
			{
			case -1:
				perror("fork");
				exit(1);
				break;
			case 0:			// Server 2
				/**/
				ipd2 = open("io6.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				shmaddr1 = shmat(shmid1, NULL, 0);
				shmaddr2 = shmat(shmid2, NULL, 0);
				shmaddr3 = shmat(shmid3, NULL, 0);
				shmaddr4 = shmat(shmid4, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					for (j = 0; j < 256; j++)
					{
						memcpy(chunk + 16 * j, shmaddr1 + 4 * j + 4096 * i + (1 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 4, shmaddr2 + 4 * j + 4096 * i + (1 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 8, shmaddr3 + 4 * j + 4096 * i + (1 * 4 * 256), 4);
						memcpy(chunk + 16 * j + 12, shmaddr4 + 4 * j + 4096 * i + (1 * 4 * 256), 4);
					}
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd2, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 2 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr1);
				shmdt(shmaddr2);
				shmdt(shmaddr3);
				shmdt(shmaddr4);
				close(ipd2);
				exit(0);
				break;
			default:		// Server 1
				/**/
				ipd1 = open("io5.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				shmaddr1 = shmat(shmid1, NULL, 0);
				shmaddr2 = shmat(shmid2, NULL, 0);
				shmaddr3 = shmat(shmid3, NULL, 0);
				shmaddr4 = shmat(shmid4, NULL, 0);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					for (j = 0; j < 256; j++)
					{
						memcpy(chunk + 16 * j, shmaddr1 + 4 * j + 4096 * i, 4);
						memcpy(chunk + 16 * j + 4, shmaddr2 + 4 * j + 4096 * i, 4);
						memcpy(chunk + 16 * j + 8, shmaddr3 + 4 * j + 4096 * i, 4);
						memcpy(chunk + 16 * j + 12, shmaddr4 + 4 * j + 4096 * i, 4);
					}
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd1, chunk, sizeof(chunk));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 1 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				shmdt(shmaddr1);
				shmdt(shmaddr2);
				shmdt(shmaddr3);
				shmdt(shmaddr4);
				close(ipd1);

				break;
			}
			break;
		}
		break;
	}

#ifdef TIMES
	gettimeofday(&aetime, NULL);
	atime_result = aetime.tv_sec - astime.tv_sec;
	atime_uresult = aetime.tv_usec - astime.tv_usec;
#endif

	/*Wait Other Process*/
	sleep(5);

	/*shmctl() Section*/

	shmctl(shmid1, IPC_RMID, NULL);
	shmctl(shmid2, IPC_RMID, NULL);
	shmctl(shmid3, IPC_RMID, NULL);
	shmctl(shmid4, IPC_RMID, NULL);

#ifdef TIMES
	printf("Server_oriented_io Total TIMES : %ld usec\n", atime_result * 1000000 + atime_uresult);
#endif

	return 0;
}
