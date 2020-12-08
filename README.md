# novdso

## About vDSO

vDSO is a shared library provided by the kernel that exports a subset of
frequently used system calls (e.g., gettimeofday(2)) as user space functions.
By using this technique, the need for costly context switches can be avoided.
On the other hand, vDSO can complicate debugging since these vsyscalls are
invisible to ptrace.

The AT_SYSINFO_EHDR field in the auxiliary vector of a process points to the
vDSO memory area mapped by the kernel.

```
man 7 vdso
```

## About novdso

novdso is a small wrapper programm that forces the wrapped process to not use
the vDSO lib by overwriting the AT_SYSINFO_EHDR tag in its auxiliary vector.
Afterwards, the wrapped process can be seized by another program (e.g., `lldb`
or `strace`) for further debugging.

Compatible with amd64 machines.

## Build

```
make
```

## Usage

```
usage: novdso FILE name-of-executable [ARGV]
example: novdso /bin/ls /bin/ls -l -i -s -a
```

### Example

Assume one wanted to evaluate the syscalls issued by `ls` with vDSO disabled,
one would run novdso as follows:

```
$ novdso /bin/ls /bin/ls -l -i -s -a
```

Once the following output occurs, the process is SIGSTOPed and novdso waits
for the process state to change:

```
--- Process paused and detached. PID: 25215 ---
```

The process can now be seized by strace:

```
$ strace -p 25215
strace: Process 25215 attached
--- stopped by SIGSTOP ---
```

Finally, order to continue the paused process, a SIGCONT has to be raised:

```
kill -SIGCONT 25215
```

The process is now unpaused and can be traced as usual.