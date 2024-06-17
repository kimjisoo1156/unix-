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

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short* array;
};

int initsem(key_t semkey) {
	union semun semunarg;
	int status = 0, semid;

	semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0600);
	if (semid == -1) {
		if (errno == EEXIST)
			semid = semget(semkey, 1, 0);
	}
	else {
		semunarg.val = 1;
		status = semctl(semid, 0, SETVAL, semunarg);
	}

	if (semid == -1 || status == -1) {
		perror("initsem");
		return (-1);
	}

	return semid;
}

int semlock(int semid) {
	struct sembuf buf;

	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	if (semop(semid, &buf, 1) == -1) {
		perror("semlock failed");
		exit(1);
	}
	return 0;
}

int semunlock(int semid) {
	struct sembuf buf;

	buf.sem_num = 0;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;
	if (semop(semid, &buf, 1) == -1) {
		perror("semunlock failed");
		exit(1);
	}
	return 0;
}

int client_oriented_io() {

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
	char buf[4096];
	int tmp1, tmp2, tmp3, tmp4;

	int f1, f2, f3, f4;
	int cpd1, cpd2, cpd3, cpd4;
	int ifw1, ifw2, ifw3, ifw4;
	int ifr1, ifr2, ifr3, ifr4;
	int ipd1, ipd2, ipd3, ipd4;

	int semid1 = initsem(552);
	int semid2 = initsem(553);
	int semid3 = initsem(554);
	int semid4 = initsem(555);

	f1 = open("p1.dat", O_RDONLY);
	f2 = open("p2.dat", O_RDONLY);
	f3 = open("p3.dat", O_RDONLY);
	f4 = open("p4.dat", O_RDONLY);

	cpd1 = open("CHUNK1", O_RDWR);
	cpd2 = open("CHUNK2", O_RDWR);
	cpd3 = open("CHUNK3", O_RDWR);
	cpd4 = open("CHUNK4", O_RDWR);

	mkfifo("./IO1_FIFO", 0666);
	mkfifo("./IO2_FIFO", 0666);
	mkfifo("./IO3_FIFO", 0666);
	mkfifo("./IO4_FIFO", 0666);

	switch (fork())
	{
	case -1:
		perror("fork");
		exit(1);
		break;
	case 0:         // Client
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
			case 0:     /*4번 (p4.dat 처리)*/
				ifw4 = open("./IO4_FIFO", O_WRONLY);

				for (i = 0; i < 256; i++)
				{
					semid1 = initsem(552);
					semlock(semid1);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f1, (3 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f1, &tmp4, sizeof(int));
						lseek(cpd4, 4 * sizeof(int) * j, SEEK_SET);
						write(cpd4, &tmp4, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid1);

					/*-----------------------------------------------------*/

					semid2 = initsem(553);
					semlock(semid2);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f2, (3 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f2, &tmp4, sizeof(int));
						lseek(cpd4, 4 * sizeof(int) * j + 4, SEEK_SET);
						write(cpd4, &tmp4, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid2);

					/*-----------------------------------------------------*/

					semid3 = initsem(554);
					semlock(semid3);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f3, (3 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f3, &tmp4, sizeof(int));
						lseek(cpd4, 4 * sizeof(int) * j + 8, SEEK_SET);
						write(cpd4, &tmp4, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid3);

					/*-----------------------------------------------------*/

					semid4 = initsem(555);
					semlock(semid4);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f4, (3 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f4, &tmp4, sizeof(int));
						lseek(cpd4, 4 * sizeof(int) * j + 12, SEEK_SET);
						write(cpd4, &tmp4, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid4);

					/*-----------------------------------------------------*/

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif

					/*IO4_FIFO로 쓰기 수행*/
					lseek(cpd4, 0, SEEK_SET);
					read(cpd4, buf, sizeof(buf));
					write(ifw4, buf, sizeof(buf));

#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}
				close(ifw4);

#ifdef TIMES
				printf("Client 4 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif
				exit(0);
				break;
			default:    /*3번 (p3.dat 처리)*/
				ifw3 = open("./IO3_FIFO", O_WRONLY);

				for (i = 0; i < 256; i++)
				{
					semid1 = initsem(552);
					semlock(semid1);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f1, (2 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f1, &tmp3, sizeof(int));
						lseek(cpd3, 4 * sizeof(int) * j, SEEK_SET);
						write(cpd3, &tmp3, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid1);

					/*-----------------------------------------------------*/

					semid2 = initsem(553);
					semlock(semid2);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f2, (2 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f2, &tmp3, sizeof(int));
						lseek(cpd3, 4 * sizeof(int) * j + 4, SEEK_SET);
						write(cpd3, &tmp3, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid2);

					/*-----------------------------------------------------*/

					semid3 = initsem(554);
					semlock(semid3);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f3, (2 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f3, &tmp3, sizeof(int));
						lseek(cpd3, 4 * sizeof(int) * j + 8, SEEK_SET);
						write(cpd3, &tmp3, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid3);

					/*-----------------------------------------------------*/

					semid4 = initsem(555);
					semlock(semid4);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f4, (2 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f4, &tmp3, sizeof(int));
						lseek(cpd3, 4 * sizeof(int) * j + 12, SEEK_SET);
						write(cpd3, &tmp3, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid4);

					/*-----------------------------------------------------*/

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif

					/*IO3_FIFO로 쓰기 수행*/
					lseek(cpd3, 0, SEEK_SET);
					read(cpd3, buf, sizeof(buf));
					write(ifw3, buf, sizeof(buf));

#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}
				close(ifw3);

#ifdef TIMES
				printf("Client 3 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif
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
			case 0:     /*2번 (p2.dat 처리)*/
				ifw2 = open("./IO2_FIFO", O_WRONLY);

				for (i = 0; i < 256; i++)
				{
					semid1 = initsem(552);
					semlock(semid1);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f1, (1 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f1, &tmp2, sizeof(int));
						lseek(cpd2, 4 * sizeof(int) * j, SEEK_SET);
						write(cpd2, &tmp2, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid1);

					/*-----------------------------------------------------*/

					semid2 = initsem(553);
					semlock(semid2);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f2, (1 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f2, &tmp2, sizeof(int));
						lseek(cpd2, 4 * sizeof(int) * j + 4, SEEK_SET);
						write(cpd2, &tmp2, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid2);

					/*-----------------------------------------------------*/

					semid3 = initsem(554);
					semlock(semid3);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f3, (1 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f3, &tmp2, sizeof(int));
						lseek(cpd2, 4 * sizeof(int) * j + 8, SEEK_SET);
						write(cpd2, &tmp2, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid3);

					/*-----------------------------------------------------*/

					semid4 = initsem(555);
					semlock(semid4);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f4, (1 * sizeof(int) * 256) + (sizeof(int) * j) + (4096 * i), SEEK_SET);
						read(f4, &tmp2, sizeof(int));
						lseek(cpd2, 4 * sizeof(int) * j + 12, SEEK_SET);
						write(cpd2, &tmp2, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid4);

					/*-----------------------------------------------------*/

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif

					/*IO2_FIFO로 쓰기 수행*/
					lseek(cpd2, 0, SEEK_SET);
					read(cpd2, buf, sizeof(buf));
					write(ifw2, buf, sizeof(buf));

#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}
				close(ifw2);

#ifdef TIMES
				printf("Client 2 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif
				exit(0);
				break;
			default:    /*1번 (p1.dat 처리)*/
				ifw1 = open("./IO1_FIFO", O_WRONLY);

				for (i = 0; i < 256; i++)
				{
					semid1 = initsem(552);
					semlock(semid1);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f1, sizeof(int) * j + (4096 * i), SEEK_SET);
						read(f1, &tmp1, sizeof(int));
						lseek(cpd1, 4 * sizeof(int) * j, SEEK_SET);
						write(cpd1, &tmp1, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid1);

					/*-----------------------------------------------------*/

					semid2 = initsem(553);
					semlock(semid2);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f2, sizeof(int) * j + (4096 * i), SEEK_SET);
						read(f2, &tmp1, sizeof(int));
						lseek(cpd1, 4 * sizeof(int) * j + 4, SEEK_SET);
						write(cpd1, &tmp1, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid2);

					/*-----------------------------------------------------*/

					semid3 = initsem(554);
					semlock(semid3);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f3, sizeof(int) * j + (4096 * i), SEEK_SET);
						read(f3, &tmp1, sizeof(int));
						lseek(cpd1, 4 * sizeof(int) * j + 8, SEEK_SET);
						write(cpd1, &tmp1, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid3);

					/*-----------------------------------------------------*/

					semid4 = initsem(555);
					semlock(semid4);

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif

					for (j = 0; j < 256; j++)
					{
						lseek(f4, sizeof(int) * j + (4096 * i), SEEK_SET);
						read(f4, &tmp1, sizeof(int));
						lseek(cpd1, 4 * sizeof(int) * j + 12, SEEK_SET);
						write(cpd1, &tmp1, sizeof(int));
					}

#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif

					semunlock(semid4);

					/*-----------------------------------------------------*/

#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif

					/*IO1_FIFO로 쓰기 수행*/
					lseek(cpd1, 0, SEEK_SET);
					read(cpd1, buf, sizeof(buf));
					write(ifw1, buf, sizeof(buf));

#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif
				}
				close(ifw1);

#ifdef TIMES
				printf("Client 1 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif
				exit(0);
				break;
			}
			break;
		}
		break;
	default:        // Server

		/*Waiting Child Process..*/
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
			case 0:     /*4번 (CHUNK4 처리)*/
				ifr4 = open("./IO4_FIFO", O_RDONLY);
				ipd4 = open("io4.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				lseek(ipd4, 0, SEEK_SET);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					read(ifr4, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd4, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 4 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				close(ipd4);
				close(ifr4);
				exit(0);
				break;
			default:    /*3번 (CHUNK3 처리)*/
				ifr3 = open("./IO3_FIFO", O_RDONLY);
				ipd3 = open("io3.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				lseek(ipd3, 0, SEEK_SET);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					read(ifr3, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd3, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 3 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				close(ipd3);
				close(ifr3);
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
			case 0:     /*2번 (CHUNK2 처리)*/
				ifr2 = open("./IO2_FIFO", O_RDONLY);
				ipd2 = open("io2.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				lseek(ipd2, 0, SEEK_SET);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					read(ifr2, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd2, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 2 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif

				close(ipd2);
				close(ifr2);
				exit(0);
				break;
			default:    /*1번 (CHUNK1 처리)*/
				ifr1 = open("./IO1_FIFO", O_RDONLY);
				ipd1 = open("io1.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
				lseek(ipd1, 0, SEEK_SET);

				for (i = 0; i < 256; i++)
				{
#ifdef TIMES
					gettimeofday(&comm_stime, NULL);
#endif
					read(ifr1, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&comm_etime, NULL);
					comm_time_result += (comm_etime.tv_usec - comm_stime.tv_usec) + 1000000 * (comm_etime.tv_sec - comm_stime.tv_sec);
#endif

#ifdef TIMES
					gettimeofday(&io_stime, NULL);
#endif
					write(ipd1, buf, sizeof(buf));
#ifdef TIMES
					gettimeofday(&io_etime, NULL);
					io_time_result += (io_etime.tv_usec - io_stime.tv_usec) + 1000000 * (io_etime.tv_sec - io_stime.tv_sec);
#endif
				}

#ifdef TIMES
				printf("Server 1 -> I/O Time : %ld usec, Comm Time : %ld usec\n", io_time_result, comm_time_result);
#endif
				close(ipd1);
				close(ifr1);
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

	/*semctl() Section*/

	semctl(semid1, 0, IPC_RMID, NULL);
	semctl(semid2, 0, IPC_RMID, NULL);
	semctl(semid3, 0, IPC_RMID, NULL);
	semctl(semid4, 0, IPC_RMID, NULL);

	/*close() Section*/

	close(cpd1);
	close(cpd2);
	close(cpd3);
	close(cpd4);

	close(f1);
	close(f2);
	close(f3);
	close(f4);

#ifdef TIMES
	printf("Client_oriented_io Total TIMES : %ld usec\n", atime_result * 1000000 + atime_uresult);
#endif

	return 0;
}
