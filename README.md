# novdso

## About vdso

vdso is a shared library provided by the kernel that exports a subset of
frequently used system calls (e.g., gettimeofday(2)) as user space functions.
By using this technique, the need for costly context switches can be avoided.
On the other hand, vdso can complicate debugging since these vsyscalls are
invisible to ptrace.

The AT_SYSINFO_EHDR field in the auxiliary vector of a process points to the
vdso memory area mapped by the kernel.

```
man 7 vdso
```

## About novdso

novdso is a small wrapper programm that forces the wrapped process to not use
the vdso lib by overwriting the AT_SYSINFO_EHDR tag in its auxiliary vector.

Compatible with amd64 machines.

```
usage: novdso FILE [ARGV]
example: novdso /bin/ls ls -l -i -s -a
```
