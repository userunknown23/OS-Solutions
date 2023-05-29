------------------------------------------------------------------------------------- 
EXP : 3     SYSTEM CALLS
-------------------------------------------------------------------------------------
//i) PROGRAM USING SYSTEM CALL fork()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    pid = fork();
    
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    } else if (pid == 0) {
        execlp("whoami", "whoami", NULL);
        exit(0);
    } else {
        printf("\nProcess id is %d\n", getpid());
        wait(NULL);
        exit(0);
    }
}

//ii) PROGRAM USING SYSTEM CALLS getpid() & getppid()

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main( ) {
    int pid;
    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(0);
    }
    if (pid == 0) {
        printf("\nChild process is under execution");
        printf("\nProcess id of the child process is %d", getpid());
        printf("\nProcess id of the parent process is %d", getppid());
    } else {
        printf("\nParent process is under execution");
        printf("\nProcess id of the parent process is %d", getpid());
        printf("\nProcess id of the child process in parent is %d", pid);
        printf("\nProcess id of the parent of parent is %d", getppid());
    }

    return 0;
}

//iii)PROGRAM USING SYSTEM CALLS opendir( ) readdir( ) closedir()

#include <stdio.h>
#include <sys/types.h> 
#include <sys/dir.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *rddir;
    printf("\nListing the directory content\n");
    dir = opendir(argv[1]);

    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }

    while ((rddir = readdir(dir)) != NULL) {
        printf("%s\t\n", rddir->d_name);
    }

    closedir(dir);
    return 0;
}

//iv) PROGRAM USING SYSTEM CALL exec( )

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n exec system call");
    printf("displaying the date");
    execlp("/bin/date", "date", NULL);
    return 0;
}

//v) PROGRAM USING SYSTEM CALLS open( ), read() & write()
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int fd[2];
    char buf1[25] = "just a test\n";
    char buf2[50];
    fd[0] = open("file1", O_RDWR);
    fd[1] = open("file2", O_RDWR);
    write(fd[0], buf1, strlen(buf1));
    printf("\nEnter the text now….\n");
    fgets(buf1, sizeof(buf1), stdin);
    write(fd[0], buf1, strlen(buf1));
    lseek(fd[0], 0, SEEK_SET);
    read(fd[0], buf2, sizeof(buf1));
    write(fd[1], buf2, sizeof(buf2));
    close(fd[0]);
    close(fd[1]);
    printf("\n");
    return 0;
}

------------------------------------------------------------------------------------
exp:4  CPU SCHEDULING
--------------------------------------------------------------------------------------

// A. FIRST COME FIRST SERVE SCHEDULING

#include <stdio.h>

struct process {
    int pid;
    int bt;
    int wt, tt;
} p[10];

int main() {
    int i, n, totwt, tottt, avg1, avg2;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        p[i].pid = i;
        printf("Enter the burst time for process %d: ", i);
        scanf("%d", &p[i].bt);
    }

    p[1].wt = 0;
    p[1].tt = p[1].bt + p[1].wt;

    i = 2;
    while (i <= n) {
        p[i].wt = p[i - 1].bt + p[i - 1].wt;
        p[i].tt = p[i].bt + p[i].wt;
        i++;
    }

    i = 1;
    totwt = tottt = 0;

    printf("\nProcessID \tBT\tWT\tTT\n");
    while (i <= n) {
        printf("\n\t%d \t%d \t%d \t%d", p[i].pid, p[i].bt, p[i].wt, p[i].tt);
        totwt = p[i].wt + totwt;
        tottt = p[i].tt + tottt;
        i++;
    }

    avg1 = totwt / n;
    avg2 = tottt / n;
    printf("\nAvg WT = %d \tAvg TT = %d\n", avg1, avg2);

    getchar(); // Pauses the program before exiting
    return 0;
}

//  B.SHORTEST JOB FIRST SCHEDULING

#include <stdio.h>

struct process {
    int pid;
    int bt;
    int wt;
    int tt;
} p[10], temp;

int main() {
    int i, j, n, totwt, tottt;
    float avg1, avg2;

    printf("\nEnter the number of processes: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        p[i].pid = i;
        printf("Enter the burst time for process %d: ", i);
        scanf("%d", &p[i].bt);
    }

    for (i = 1; i < n; i++) {
        for (j = i + 1; j <= n; j++) {
            if (p[i].bt > p[j].bt) {
                temp.pid = p[i].pid;
                p[i].pid = p[j].pid;
                p[j].pid = temp.pid;

                temp.bt = p[i].bt;
                p[i].bt = p[j].bt;
                p[j].bt = temp.bt;
            }
        }
    }

    p[1].wt = 0;
    p[1].tt = p[1].bt + p[1].wt;

    i = 2;
    while (i <= n) {
        p[i].wt = p[i - 1].bt + p[i - 1].wt;
        p[i].tt = p[i].bt + p[i].wt;
        i++;
    }

    i = 1;
    totwt = tottt = 0;

    printf("\nProcess id \tbt \twt \ttt\n");
    while (i <= n) {
        printf("\n\t%d \t%d \t%d \t%d", p[i].pid, p[i].bt, p[i].wt, p[i].tt);
        totwt += p[i].wt;
        tottt += p[i].tt;
        i++;
    }

    avg1 = (float)totwt / n;
    avg2 = (float)tottt / n;
    printf("\nAVG1=%f \tAVG2=%f", avg1, avg2);

    printf("\nPress Enter to exit...");
    getchar(); // Pauses the program before exiting
    return 0;
}

// C . PRIORITY SCHEDULING

#include <stdio.h>

struct process {
    int pid;
    int bt;
    int wt;
    int tt;
    int prior;
} p[10], temp;

int main() {
    int i, j, n, totwt, tottt, arg1, arg2;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        p[i].pid = i;
        printf("Enter the burst time for process %d: ", i);
        scanf("%d", &p[i].bt);
        printf("Enter the priority for process %d: ", i);
        scanf("%d", &p[i].prior);
    }

    for (i = 1; i < n; i++) {
        for (j = i + 1; j <= n; j++) {
            if (p[i].prior > p[j].prior) {
                temp.pid = p[i].pid;
                p[i].pid = p[j].pid;
                p[j].pid = temp.pid;

                temp.bt = p[i].bt;
                p[i].bt = p[j].bt;
                p[j].bt = temp.bt;

                temp.prior = p[i].prior;
                p[i].prior = p[j].prior;
                p[j].prior = temp.prior;
            }
        }
    }

    p[1].wt = 0;
    p[1].tt = p[1].bt + p[1].wt;

    i = 2;
    while (i <= n) {
        p[i].wt = p[i - 1].bt + p[i - 1].wt;
        p[i].tt = p[i].bt + p[i].wt;
        i++;
    }

    i = 1;
    totwt = tottt = 0;

    printf("\nProcess id \tbt \twt \ttt\n");
    while (i <= n) {
        printf("%d \t\t%d \t%d \t%d\n", p[i].pid, p[i].bt, p[i].wt, p[i].tt);
        totwt += p[i].wt;
        tottt += p[i].tt;
        i++;
    }

    arg1 = totwt / n;
    arg2 = tottt / n;
    printf("\narg1=%d \targ2=%d\n", arg1, arg2);

    printf("Press Enter to exit...");
    getchar(); // Pauses the program before exiting
    return 0;
}

// D. ROUND ROBIN SCHEDULING

#include<stdio.h>

int main() {
    int pt[10][10], a[10][10], at[10], pname[10][10];
    int i, j, n, k = 0, q, sum = 0;
    float avg;

    printf("\nEnter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            pt[i][j] = 0;
            a[i][j] = 0;
        }
    }

    for (i = 0; i < n; i++) {
        printf("\nEnter the process time for process %d: ", i + 1);
        scanf("%d", &pt[i][0]);
    }

    printf("\nEnter the time slice: ");
    scanf("%d", &q);

    printf("\n");

    for (j = 0; j < 10; j++) {
        for (i = 0; i < n; i++) {
            a[2*j][i] = k;

            if ((pt[i][j] <= q) && (pt[i][j] != 0)) {
                pt[i][j+1] = 0;
                printf(" %d P%d %d\n", k, i + 1, k + pt[i][j]);
                k += pt[i][j];
                a[2*j+1][i] = k;
            } else if (pt[i][j] != 0) {
                pt[i][j+1] = pt[i][j] - q;
                printf(" %d P%d %d\n", k, i + 1, (k + q));
                k += q;
                a[2*j+1][i] = k;
            } else {
                a[2*j][i] = 0;
                a[2*j+1][i] = 0;
            }
        }
    }

    for (i = 0; i < n; i++)
        sum += a[0][i];

    for (i = 0; i < n; i++) {
        for (j = 1; j < 10; j++) {
            if ((a[j][i] != 0) && (a[j+1][i] != 0) && ((j+1) % 2 == 0))
                sum += (a[j+1][i] - a[j][i]);
        }
    }

    avg = (float)sum / n;
    printf("\nAverage waiting time = %f msec", avg);

    sum = avg = 0;

    for (j = 0; j < n; j++) {
        i = 1;
        while (a[i][j] != 0)
            i += 1;

        sum += a[i-1][j];
    }

    avg = (float)sum / n;
    printf("\nAverage turnaround time = %f msec\n\n", avg);

    return 0;
}

------------------------------------------------------------------------------------- 
EXP: 5    INTER PROCESS COMMUNICATION USING SHARED MEMORY
--------------------------------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int child, shmid, i;
    char *shmptr;

    child = fork();

    if (child == -1) {
        printf("Failed to create child process\n");
        return 1;
    } else if (child == 0) {
        shmid = shmget(2041, 32, IPC_CREAT | 0666);

        if (shmid == -1) {
            printf("Shared memory creation failed\n");
            return 1;
        }

        shmptr = (char*) shmat(shmid, 0, 0);

        if (shmptr == (char*)-1) {
            printf("Shared memory attachment failed\n");
            return 1;
        }

        printf("\nParent writing\n");

        for (i = 0; i < 10; i++) {
            shmptr[i] = 'a' + i;
            putchar(shmptr[i]);
        }
    } else {
        wait(NULL);
        shmid = shmget(2041, 32, 0666);

        if (shmid == -1) {
            printf("Shared memory retrieval failed\n");
            return 1;
        }

        shmptr = (char*) shmat(shmid, 0, 0);

        if (shmptr == (char*)-1) {
            printf("Shared memory attachment failed\n");
            return 1;
        }

        printf("\n\n%s", shmptr);

        printf("\nChild is reading\n");

        for (i = 0; i < 10; i++)
            putchar(shmptr[i]);

        shmdt(shmptr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}

------------------------------------------------------------------------------------
  EXP: 6  PRODUCER-CONSUMER PROBLEM USING SEMOPHERES
--------------------------------------------------------------------------------------

#include <iostream>

int main() {
    int buffer[10], bufsize, in, out, produce, consume, choice = 0;
    in = 0;
    out = 0;
    bufsize = 10;

    while (choice != 3) {
        std::cout << "\n1. Produce \t2. Consume \t3. Exit";
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                if ((in + 1) % bufsize == out)
                    std::cout << "\nBuffer is Full";
                else {
                    std::cout << "\nEnter the value: ";
                    std::cin >> produce;
                    buffer[in] = produce;
                    in = (in + 1) % bufsize;
                }
                break;
            case 2:
                if (in == out)
                    std::cout << "\nBuffer is Empty";
                else {
                    consume = buffer[out];
                    std::cout << "\nThe consumed value is " << consume;
                    out = (out + 1) % bufsize;
                }
                break;
        }
    }

    return 0;
}
--------------------------------------------------------------------------------
  EXP : 7    SIMULATE BANKERS ALGORITHM FOR DEADLOCK AVOIDENCE
--------------------------------------------------------------------------------------

#include <stdio.h>

int main() {
    int k=0, output[10], d=0, t=0, ins[5], i, avail[5], allocated[10][5], need[10][5], MAX[10][5], pno, P[10], j, rz, count=0;

    printf("\n Enter the number of resources: ");
    scanf("%d", &rz);

    printf("\n Enter the max instances of each resource:\n");
    for (i=0; i<rz; i++) {
        avail[i] = 0;
        printf("%c= ", (i+97));
        scanf("%d", &ins[i]);
    }

    printf("\n Enter the number of processes: ");
    scanf("%d", &pno);

    printf("\n Enter the allocation matrix:\n ");
    for (i=0; i<rz; i++)
        printf(" %c", (i+97));
    printf("\n");

    for (i=0; i<pno; i++) {
        P[i] = i;
        printf("P[%d] ", P[i]);
        for (j=0; j<rz; j++) {
            scanf("%d", &allocated[i][j]);
            avail[j] += allocated[i][j];
        }
    }

    printf("\nEnter the MAX matrix:\n ");
    for (i=0; i<rz; i++) {
        printf(" %c", (i+97));
        avail[i] = ins[i] - avail[i];
    }
    printf("\n");

A:
    d = -1;
    for (i=0; i<pno; i++) {
        count = 0;
        t = P[i];
        for (j=0; j<rz; j++) {
            need[t][j] = MAX[t][j] - allocated[t][j];
            if (need[t][j] <= avail[j])
                count++;
        }
        if (count == rz) {
            output[k++] = P[i];
            for (j=0; j<rz; j++)
                avail[j] += allocated[t][j];
        } else {
            P[++d] = P[i];
        }
    }

    if (d != -1) {
        pno = d+1;
        goto A;
    }

    printf("\t <");
    for (i=0; i<k; i++)
        printf(" P[%d] ", output[i]);
    printf(">\n");

    return 0;
}

---------------------------------------------------------------------------------------
EXP: 8     ALGORITHM FOR DEADLOCK DETECTION
---------------------------------------------------------------------------------------

#include <stdio.h>

int max[100][100];
int alloc[100][100];
int need[100][100];
int avail[100];
int n, r;

void input();
void show();
void cal();

int main()
{
    printf("********** Deadlock Detection Algo ************\n");
    input();
    show();
    cal();

    return 0;
}

void input()
{
    int i, j;
    printf("Enter the number of Processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource instances: ");
    scanf("%d", &r);

    printf("Enter the Max Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the Allocation Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    printf("Enter the available Resources:\n");
    for (j = 0; j < r; j++) {
        scanf("%d", &avail[j]);
    }
}

void show()
{
    int i, j;
    printf("Process\tAllocation\tMax\tAvailable\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t", i + 1);
        for (j = 0; j < r; j++) {
            printf("%d ", alloc[i][j]);
        }
        printf("\t");
        for (j = 0; j < r; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\t");
        if (i == 0) {
            for (j = 0; j < r; j++) {
                printf("%d ", avail[j]);
            }
        }
        printf("\n");
    }
}

void cal()
{
    int finish[100], temp, flag = 1, k, c1 = 0;
    int dead[100];
    int safe[100];
    int i, j;

    for (i = 0; i < n; i++) {
        finish[i] = 0;
    }

    // Find the need matrix
    for (i = 0; i < n; i++) {
        for (j = 0; j < r; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    while (flag) {
        flag = 0;
        for (i = 0; i < n; i++) {
            int c = 0;
            for (j = 0; j < r; j++) {
                if ((finish[i] == 0) && (need[i][j] <= avail[j])) {
                    c++;
                    if (c == r) {
                        for (k = 0; k < r; k++) {
                            avail[k] += alloc[i][j];
                        }
                        finish[i] = 1;
                        flag = 1;
                        if (finish[i] == 1) {
                            i = n;
                        }
                    }
                }
            }
        }
    }

    j = 0;
    flag = 0;
    for (i = 0; i < n; i++) {
        if (finish[i] == 0) {
            dead[j] = i;
            j++;
            flag = 1;
        }
    }

    if (flag == 1) {
        printf("\n\nSystem is in Deadlock and the Deadlock processes are:\n");
        for (i = 0; i < n; i++) {
            printf("P%d\t", dead[i]);
        }
        printf("\n");
    } else {
        printf("\nNo Deadlock Occurs.\n");
    }
}
---------------------------------------------------------------------------------------
EXP :9     THREADING & SYNCHRONIZATION APPLICATIONS 
---------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];

void* doSomeThing(void* arg)
{
    unsigned long i = 0;
    pthread_t id = pthread_self();

    if (pthread_equal(id, tid[0]))
    {
        printf("\nFirst thread processing\n");
    }
    else
    {
        printf("\nSecond thread processing\n");
    }

    for (i = 0; i < 0xFFFFFFFF; i++);
    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    while (i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\nCan't create thread: [%s]", strerror(err));
        else
            printf("\nThread created successfully\n");

        i++;
    }

    sleep(5);
    return 0;
}
----------------------------------------------------------------------
EXP:10            PAGING TECHNIQUE OF MEMORY MANAGEMENT
----------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* doSomeThing(void* arg)
{
    pthread_mutex_lock(&lock);
    unsigned long i = 0;
    counter += 1;
    printf("\nJob %d started\n", counter);
    for (i = 0; i < 0xFFFFFFFF; i++);
    printf("\nJob %d finished\n", counter);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\nMutex init failed\n");
        return 1;
    }

    while (i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\nCan't create thread: [%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}

----------------------------------------------------------------------
EXP:11           MEMORY ALLOCATION TECHNIQUES
----------------------------------------------------------------------

--->  a) Worst-fit

#include <stdio.h>
#define max 25

int main()
{
    int frag[max], b[max], f[max], i, j, nb, nf, temp;
    static int bf[max], ff[max];

    printf("\n\tMemory Management Scheme - First Fit");
    printf("\nEnter the number of blocks: ");
    scanf("%d", &nb);
    printf("Enter the number of files: ");
    scanf("%d", &nf);

    printf("\nEnter the size of the blocks:\n");
    for (i = 1; i <= nb; i++)
    {
        printf("Block %d: ", i);
        scanf("%d", &b[i]);
    }

    printf("Enter the size of the files:\n");
    for (i = 1; i <= nf; i++)
    {
        printf("File %d: ", i);
        scanf("%d", &f[i]);
    }

    for (i = 1; i <= nf; i++)
    {
        for (j = 1; j <= nb; j++)
        {
            if (bf[j] != 1)
            {
                temp = b[j] - f[i];
                if (temp >= 0)
                {
                    ff[i] = j;
                    break;
                }
            }
        }
        frag[i] = temp;
        bf[ff[i]] = 1;
    }

    printf("\nFile_no:\tFile_size:\tBlock_no:\tBlock_size:\tFragment");
    for (i = 1; i <= nf; i++)
        printf("\n%d\t\t%d\t\t%d\t\t%d\t\t%d", i, f[i], ff[i], b[ff[i]], frag[i]);

    return 0;
}
-->  b) Best-fit

#include <stdio.h>
#define max 25

int main()
{
    int frag[max], b[max], f[max], i, j, nb, nf, temp, lowest = 10000;
    static int bf[max], ff[max];

    printf("\nEnter the number of blocks: ");
    scanf("%d", &nb);
    printf("Enter the number of files: ");
    scanf("%d", &nf);

    printf("\nEnter the size of the blocks:\n");
    for (i = 1; i <= nb; i++)
    {
        printf("Block %d: ", i);
        scanf("%d", &b[i]);
    }

    printf("Enter the size of the files:\n");
    for (i = 1; i <= nf; i++)
    {
        printf("File %d: ", i);
        scanf("%d", &f[i]);
    }

    for (i = 1; i <= nf; i++)
    {
        for (j = 1; j <= nb; j++)
        {
            if (bf[j] != 1)
            {
                temp = b[j] - f[i];
                if (temp >= 0)
                {
                    if (lowest > temp)
                    {
                        ff[i] = j;
                        lowest = temp;
                    }
                }
            }
        }
        frag[i] = lowest;
        bf[ff[i]] = 1;
        lowest = 10000;
    }

    printf("\nFile No\tFile Size\tBlock No\tBlock Size\tFragment");
    for (i = 1; i <= nf && ff[i] != 0; i++)
        printf("\n%d\t\t%d\t\t%d\t\t%d\t\t%d", i, f[i], ff[i], b[ff[i]], frag[i]);

    return 0;
}

-->  c) First-fit

#include<stdio.h>
// #include<conio.h>
#define max 25

void main()
{
    int frag[max], b[max], f[max], i, j, nb, nf, temp, highest = 0;
    static int bf[max], ff[max];
    // clrscr();
    printf("\n\tMemory Management Scheme - Worst Fit");
    printf("\nEnter the number of blocks:");
    scanf("%d", &nb);
    printf("Enter the number of files:");
    scanf("%d", &nf);
    printf("\nEnter the size of the blocks:-\n");
    for (i = 1; i <= nb; i++)
    {
        printf("Block %d:", i);
        scanf("%d", &b[i]);
    }
    printf("Enter the size of the files :-\n");
    for (i = 1; i <= nf; i++)
    {
        printf("File %d:", i);
        scanf("%d", &f[i]);
    }
    for (i = 1; i <= nf; i++)
    {
        highest = 0;
        for (j = 1; j <= nb; j++)
        {
            if (bf[j] != 1) //if bf[j] is not allocated
            {
                temp = b[j] - f[i];
                if (temp >= 0)
                {
                    if (highest < temp)
                    {
                        ff[i] = j;
                        highest = temp;
                    }
                }
            }
        }
        frag[i] = highest;
        bf[ff[i]] = 1;
    }
    printf("\nFile_no:\tFile_size:\tBlock_no:\tBlock_size:\tFragement");
    for (i = 1; i <= nf; i++)
        printf("\n%d\t\t%d\t\t%d\t\t%d\t\t%d", i, f[i], ff[i], b[ff[i]], frag[i]);
    // getch();
}

----------------------------------------------------------------------
EXP:12          SIMULATE PAGE REPLACEMENT ALGORITHMS
----------------------------------------------------------------------
---->  FIFO
#include<stdio.h>
// #include<conio.h>
int i, j, nof, nor, flag = 0, ref[50], frm[50], pf = 0, victim = -1;

int main()
{
    // clrscr();
    printf("\n \t\t\t FIFO PAGE REPLACEMENT ALGORITHM");
    printf("\n Enter the number of frames: ");
    scanf("%d", &nof);
    printf("Enter the number of pages: ");
    scanf("%d", &nor);
    printf("\n Enter the Page Numbers: ");
    for (i = 0; i < nor; i++)
        scanf("%d", &ref[i]);

    printf("\nThe given Pages are:");
    for (i = 0; i < nor; i++)
        printf("%4d", ref[i]);

    for (i = 1; i <= nof; i++)
        frm[i] = -1;

    printf("\n");

    for (i = 0; i < nor; i++)
    {
        flag = 0;
        printf("\n\t page no %d->\t", ref[i]);

        for (j = 0; j < nof; j++)
        {
            if (frm[j] == ref[i])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            pf++;
            victim++;
            victim = victim % nof;
            frm[victim] = ref[i];
            for (j = 0; j < nof; j++)
                printf("%4d", frm[j]);
        }
    }

    printf("\n\n\t\t Number of page faults: %d", pf);
    // getch();
    return 0;
}

---->  LRU
#include<stdio.h>
// #include<conio.h>
int i, j, nof, nor, flag = 0, ref[50], frm[50], pf = 0, victim = -1;
int recent[10], lrucal[50], count = 0;

int lruvictim();

int main()
{
    // clrscr();
    printf("\n\t\t\t LRU PAGE REPLACEMENT ALGORITHM");
    printf("\n Enter the number of frames: ");
    scanf("%d", &nof);
    printf("Enter the number of reference strings: ");
    scanf("%d", &nor);
    printf("\n Enter the reference strings: ");
    for (i = 0; i < nor; i++)
        scanf("%d", &ref[i]);

    printf("\n\n\t\t LRU PAGE REPLACEMENT ALGORITHM ");
    printf("\n\t The given reference strings:");
    printf("\n..................................................");
    for (i = 0; i < nor; i++)
        printf("%4d", ref[i]);

    for (i = 1; i <= nof; i++)
    {
        frm[i] = -1;
        lrucal[i] = 0;
    }

    for (i = 0; i < 10; i++)
        recent[i] = 0;

    printf("\n");

    for (i = 0; i < nor; i++)
    {
        flag = 0;
        printf("\n\t Reference No %d ->\t", ref[i]);

        for (j = 0; j < nof; j++)
        {
            if (frm[j] == ref[i])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            count++;
            if (count <= nof)
                victim++;
            else
                victim = lruvictim();

            pf++;
            frm[victim] = ref[i];
            for (j = 0; j < nof; j++)
                printf("%4d", frm[j]);
        }

        recent[ref[i]] = i;
    }

    printf("\n\n\t Number of page faults: %d", pf);
    // getch();
    return 0;
}

int lruvictim()
{
    int i, j, temp1, temp2;
    for (i = 0; i < nof; i++)
    {
        temp1 = frm[i];
        lrucal[i] = recent[temp1];
    }

    temp2 = lrucal[0];

    for (j = 1; j < nof; j++)
    {
        if (temp2 > lrucal[j])
            temp2 = lrucal[j];
    }

    for (i = 0; i < nof; i++)
    {
        if (ref[temp2] == frm[i])
            return i;
    }

    return 0;
}

---->  OPTIMAL
#include<stdio.h>
// #include<conio.h>
int n, page[20], f, fr[20], i;

void display()
{
    for (i = 0; i < f; i++)
    {
        printf("%d", fr[i]);
    }
    printf("\n");
}

void request()
{
    printf("Enter the number of pages: ");
    scanf("%d", &n);
    printf("Enter the number of frames: ");
    scanf("%d", &f);
    printf("Enter the page numbers: ");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &page[i]);
    }
    for (i = 0; i < f; i++)
    {
        fr[i] = -1;
    }
}

void replace()
{
    int j, flag = 0, pf = 0;
    int max, lp[10], index, m;

    for (j = 0; j < f; j++)
    {
        fr[j] = page[j];
        flag = 1;
        pf++;
        display();
    }

    for (j = f; j < n; j++)
    {
        flag = 0;
        for (i = 0; i < f; i++)
        {
            if (fr[i] == page[j])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            for (i = 0; i < f; i++)
                lp[i] = 0;

            for (i = 0; i < f; i++)
            {
                for (m = j + 1; m < n; m++)
                {
                    if (fr[i] == page[m])
                    {
                        lp[i] = m - j;
                        break;
                    }
                }
            }

            max = lp[0];
            index = 0;
            for (i = 0; i < f; i++)
            {
                if (lp[i] == 0)
                {
                    index = i;
                    break;
                }
                else
                {
                    if (max < lp[i])
                    {
                        max = lp[i];
                        index = i;
                    }
                }
            }

            fr[index] = page[j];
            pf++;
            display();
        }
    }
    printf("Page faults: %d\n", pf);
}

int main()
{
    // clrscr();
    request();
    replace();
    // getch();
    return 0;
}


----------------------------------------------------------------------
EXP:13    FILE ORGANIZATION TECHNIQUE
----------------------------------------------------------------------

----------------------------------------------------------------------
EXP:14  A) FILE ALLOCAION TECHNIQUE - 
----------------------------------------------------------------------

---->  SEQUENTIAL 

#include<stdio.h>

int main() {
    int n, i, j, b[20], sb[20], t[20], x, c[20][20];
    
    printf("Enter the number of files: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        printf("Enter the number of blocks occupied by file %d: ", i+1);
        scanf("%d", &b[i]);
        
        printf("Enter the starting block of file %d: ", i+1);
        scanf("%d", &sb[i]);
        
        t[i] = sb[i];
        for (j = 0; j < b[i]; j++) {
            c[i][j] = sb[i]++;
        }
    }
    
    printf("Filename\tStart block\tLength\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\n", i+1, t[i], b[i]);
    }
    
    printf("Enter file name: ");
    scanf("%d", &x);
    printf("File name is: %d\n", x);
    printf("Length is: %d\n", b[x-1]);
    printf("Blocks occupied: ");
    for (i = 0; i < b[x-1]; i++) {
        printf("%4d", c[x-1][i]);
    }
    
    return 0;
}

---> INDEXED FILE ALLOCAION TECHNIQUE
#include<stdio.h>

int main() {
    int n, m[20], i, j, sb[20], s[20], b[20][20], x;

    printf("Enter no. of files:");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter starting block and size of file %d:", i + 1);
        scanf("%d%d", &sb[i], &s[i]);
        printf("Enter blocks occupied by file %d:", i + 1);
        scanf("%d", &m[i]);
        printf("Enter blocks of file %d:", i + 1);
        for (j = 0; j < m[i]; j++)
            scanf("%d", &b[i][j]);
    }

    printf("\nFile\t index\tlength\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, sb[i], m[i]);
    }

    printf("\nEnter file name:");
    scanf("%d", &x);
    printf("File name is: %d\n", x);
    i = x - 1;
    printf("Index is: %d\n", sb[i]);
    printf("Blocks occupied are:");
    for (j = 0; j < m[i]; j++)
        printf("%3d", b[i][j]);

    return 0;
}


---> LINKED FILE ALLOCATION
#include<stdio.h>

struct file {
    char fname[10];
    int start, size, block[10];
};

int main() {
    struct file f[10];
    int i, j, n;

    printf("Enter no. of files:");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter file name:");
        scanf("%s", f[i].fname);
        printf("Enter starting block:");
        scanf("%d", &f[i].start);
        f[i].block[0] = f[i].start;
        printf("Enter no. of blocks:");
        scanf("%d", &f[i].size);
        printf("Enter block numbers:");
        for (j = 1; j <= f[i].size; j++) {
            scanf("%d", &f[i].block[j]);
        }
    }

    printf("File\tstart\tsize\tblock\n");
    for (i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t", f[i].fname, f[i].start, f[i].size);
        for (j = 1; j <= f[i].size - 1; j++) {
            printf("%d--->", f[i].block[j]);
        }
        printf("%d", f[i].block[j]);
        printf("\n");
    }

    return 0;
}


----------------------------------------------------------------------
EXP:15     DISK SCHEDULING ALGORITHMS
----------------------------------------------------------------------

---> a) FCFS

#include <stdio.h>

int main() {
    int t[20], n, i, j, tohm[20], tot = 0;
    float avhm;

    printf("Enter the number of tracks: ");
    scanf("%d", &n);

    printf("Enter the tracks to be traversed: ");
    for (i = 0; i < n; i++)
        scanf("%d", &t[i]);

    for (i = 0; i < n - 1; i++) {
        tohm[i] = t[i + 1] - t[i];
        if (tohm[i] < 0)
            tohm[i] = -tohm[i];
    }

    for (i = 0; i < n - 1; i++)
        tot += tohm[i];

    avhm = (float)tot / (n - 1);

    printf("\nTracks traversed\tDifference between tracks\n");
    for (i = 0; i < n - 1; i++)
        printf("%d\t\t%d\n", t[i], tohm[i]);

    printf("\nAverage header movements: %f", avhm);

    return 0;
}

---> b) SCAN 

#include <stdio.h>

int main() {
    int t[20], d[20], h, i, j, n, temp, k, atr[20], tot, p, sum = 0;

    printf("Enter the number of tracks to be traversed: ");
    scanf("%d", &n);

    printf("Enter the position of the head: ");
    scanf("%d", &h);

    t[0] = 0;
    t[1] = h;

    printf("Enter the tracks: ");
    for (i = 2; i < n + 2; i++)
        scanf("%d", &t[i]);

    for (i = 0; i < n + 2; i++) {
        for (j = 0; j < (n + 2) - i - 1; j++) {
            if (t[j] > t[j + 1]) {
                temp = t[j];
                t[j] = t[j + 1];
                t[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < n + 2; i++) {
        if (t[i] == h) {
            j = i;
            k = i;
            break;
        }
    }

    p = 0;
    while (t[j] != 0) {
        atr[p] = t[j];
        j--;
        p++;
    }

    atr[p] = t[j];

    for (p = k + 1; p < n + 2; p++, k++)
        atr[p] = t[k + 1];

    for (j = 0; j < n + 1; j++) {
        if (atr[j] > atr[j + 1])
            d[j] = atr[j] - atr[j + 1];
        else
            d[j] = atr[j + 1] - atr[j];
        sum += d[j];
    }

    printf("\nAverage header movements: %f", (float)sum / n);

    return 0;
}

---> c) C-SCAN
#include<stdio.h>

int main() {
    int t[20], d[20], h, i, j, n, temp, k, atr[20], tot, p, sum = 0;

    printf("Enter the number of tracks to be traversed: ");
    scanf("%d", &n);
    printf("Enter the position of the head: ");
    scanf("%d", &h);

    t[0] = 0;
    t[1] = h;

    printf("Enter the total number of tracks: ");
    scanf("%d", &tot);
    t[2] = tot - 1;

    printf("Enter the tracks: ");
    for(i = 3; i <= n + 2; i++)
        scanf("%d", &t[i]);

    for(i = 0; i <= n + 2; i++) {
        for(j = 0; j <= (n + 2) - i - 1; j++) {
            if(t[j] > t[j + 1]) {
                temp = t[j];
                t[j] = t[j + 1];
                t[j + 1] = temp;
            }
        }
    }

    for(i = 0; i <= n + 2; i++) {
        if(t[i] == h) {
            j = i;
            break;
        }
    }

    p = 0;
    temp = t[j];
    t[j] = t[j + 1];
    t[j + 1] = temp;

    while(t[j] != tot - 1) {
        atr[p] = t[j];
        j++;
        p++;
    }

    atr[p] = t[j];
    p++;

    i = 0;
    while(p != (n + 3) && t[i] != t[h]) {
        atr[p] = t[i];
        i++;
        p++;
    }

    for(j = 0; j < n + 2; j++) {
        if(atr[j] > atr[j + 1]) {
            d[j] = atr[j] - atr[j + 1];
        } else {
            d[j] = atr[j + 1] - atr[j];
        }
        sum += d[j];
    }

    printf("Tracks traversed\tDifference Between tracks\n");
    for(j = 0; j < n + 2; j++) {
        printf("%d\t\t\t%d\n", atr[j], d[j]);
    }

    printf("Total header movements: %d\n", sum);
    printf("Average header movements: %.2f\n", (float)sum / n);

    return 0;
}
