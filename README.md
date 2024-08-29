# Diplomarbeit
- echo "# Diplomarbeit" >> README.md
- git init
- git add README.md <and other file>
- git commit -m "first commit"
- git branch -M main
- git remote add origin git@github.com:daqingwangchao/Diplomarbeit.git
- git push -u origin main

## create new branch using command line(you have already created at least one default main branch)
1. Make sure your local repository is up to date with the remote repository

    `git fetch origin`

    `git pull origin main`

2. Create a new branch locally:

    ``git checkout -b <new-branch-name>``

    ``git push -u origin <new-branch-name>``

## Compile instruction
We have already existed Makefile which uses compiler **p4c-ebpf**, **clang** and **llvm** to generate aim file.

1. from .p4 direct to .bc, .c and .o

    ``make -f <path to the kernel.mk> BPFOBJ=<file>.o P4FILE=<file>.p4``

    e.g. ``make -f ../p4c/backends/ebpf/runtime/kernel.mk BPFOBJ=out.o P4FILE=bytecounter.p4``

2. from .c to .o 

    ``make CFILE=<file>.c BPFOBJ=<file>.o``

    e.g. ``make CFILE=out.c BPFOBJ=export.o``



