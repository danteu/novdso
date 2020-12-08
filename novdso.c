#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>

/* from <asm/auxvec.h> */
#define AT_NULL 0
#define AT_IGNORE 1
#define AT_SYSINFO_EHDR 33

/*
 * removeVDSO() alters the auxiliary table of a newly created process in order
 * to disable VDSO.
 */
void removeVDSO(int pid) {
  size_t pos;
  int zeroCount;
  long val;

  pos = (size_t)ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * RSP, NULL);

  /* skip to auxiliary vector */
  zeroCount = 0;
  while (zeroCount < 2) {
    val = ptrace(PTRACE_PEEKDATA, pid, pos += 8, NULL);
    if (val == 0)
      zeroCount++;
  }

  /* search the auxiliary vector for AT_SYSINFO_EHDR... */
  val = ptrace(PTRACE_PEEKDATA, pid, pos += 8, NULL);
  while(1) {
    if (val == AT_NULL)
      break;
    if (val == AT_SYSINFO_EHDR) {
      /* ... and overwrite it */
      ptrace(PTRACE_POKEDATA, pid, pos, AT_IGNORE);
      break;
    }
    val = ptrace(PTRACE_PEEKDATA, pid, pos += 16, NULL);
  }
}

/*
 * traceProcess() waits for execve(2) to return, calls removeVDSO() and
 * detaches from the child afterwards.
 */
int traceProcess(int pid) {
  int status, exitStatus;

  waitpid(pid, &status, 0);
  ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC);
  ptrace(PTRACE_SYSCALL, pid, NULL, NULL);

  while(1) {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
      break;

    if (status >> 8 == (SIGTRAP | (PTRACE_EVENT_EXEC << 8))) {
      removeVDSO(pid);
      kill(pid, SIGSTOP);
      ptrace(PTRACE_DETACH, pid, NULL, NULL);
      printf("--- Process paused and detached. PID: %i ---\n", pid);
      /* wait for child to exit */
      while (waitpid(pid, &status, 0) > 0);
      break;
    }

    ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
  }
  exitStatus = WEXITSTATUS(status);
  return exitStatus;
}

int main(int argc, char *argv[]) {
  char *myfile, **myargv;
  int exitStatus;
  pid_t child;

  if (argc < 3) {
    printf("usage: novdso FILE ARGV...\n");
    printf("example: novdso /bin/ls ls -l -i -s -a\n");
    return 1;
  }

  myfile = argv[1];
  myargv = &argv[2];

  child = fork();
  if (child == 0) {
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    kill(getpid(), SIGSTOP);
    execvp(myfile, myargv);
  } else {
    exitStatus = traceProcess(child);
  }

  return exitStatus;
}
