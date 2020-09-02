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

Compatible with amd64 machines.

## Build

```
make
```

## Run

```
usage: novdso FILE name-of-executable [ARGV]
example: novdso /bin/ls /bin/ls -l -i -s -a
```
