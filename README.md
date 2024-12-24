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

## Pre-requirement of the testbed
1. In order to run the code and testbed of virtual machine, the following enviroment should be installed in advance:
    1. Kurbenetes with version higher than 24.0.0, for this testbed, we use 31.0.1

        1). On Linux Plattform perform the Installation.

        Install instruction: https://kubernetes.io/docs/setup/production-environment/tools/kubeadm/install-kubeadm/ 

        2). After finish installing kubectl, kubelet, kubeadm, there is one plugin can be installed for auto completion of k8s: 

        ``sudo apt-get install -y bash-completion``

        ``echo "source <(kubectl completion bash)" >> ~/.bashrc``

        Reload and update the bashc file ``source ~/.bashrc``


    2. Cilium with 15.6 or above
    3. Install docker
    3. Install Container runtime, we use ``cri-dockerd``, but the new version of k8s (from 24.0.0) has removed cri-dockerd by default, so if you still want to use it, you need to download and configured it by yourself. Here is the key points which need to be modified:
        
        1). source code: https://github.com/Mirantis/cri-dockerd/tags, choose the new version, for me v3.0.4 is used.

        2). Unpack code to specified path: ``tar -zxvf cri-dockerd-0.3.14.amd64.tgz -C /tmp``

        3). Copy binary file to system enviroment path: ``sudo cp /tmp/cri-dockerd/cri-dockerd /usr/bin/``

        4). Launch cri-dockerd

        ``sudo systemctl daemon-reload``

        ``sudo systemctl daemon-reload``

        ``sudo systemctl start cri-docker``

        ``sudo systemctl status cri-docker``

## Compile instruction

We have already existed Makefile which uses compiler **p4c-ebpf**, **clang** and **llvm** to generate aim file.

1. from .p4 direct to .bc, .c and .o

    ``make -f <path to the kernel.mk> BPFOBJ=<file>.o P4FILE=<file>.p4``

    e.g. ``make -f ../p4c/backends/ebpf/runtime/kernel.mk BPFOBJ=out.o P4FILE=bytecounter.p4``

2. from .c to .o 

    ``make CFILE=<file>.c BPFOBJ=<file>.o``

    e.g. ``make CFILE=out.c BPFOBJ=export.o``

## Load eBPF code into kernel(Cilium hook point)

For the references you can see https://docs.cilium.io/en/stable/reference-guides/bpf/toolchain/#iproute2

Wahtever you want to load program into kernel or Cilium, they are all about loading to the kernel.

1. The above compile instruction is mentioned about generation of eBPF C code and then compile c code into .o file using clang.

2. Check the avaliable Cilium hook point that you can load to the kernel.

    - you can check the details of generated object file use command ``llvm-objdump -h <filename>.o``

    - check the avaliable hookpoint that you can insert eBPF program into:

    ``ip link show``

    - after geting the avaliable device, then use command to check already attached eBPF code of Cilium:

    ``sudo tc filter show dev <name> egress``

    ``sudo tc filter show dev <name> ingress``

    - attach eBPF code to cilium_vxlan

    ``tc filter add dev <name> egress bpf da obj <file.o> section <the name of your section in ebpf code>``

    - list all attached eBPF program of one certain Cilium hook point:
    
    ``sudo tc filter show dev ens33 <cilium hook point>``

    - show all loaded eBPF programs and their corresponding map's ips

    - verify bpf code has attached

    ``sudo bpftool net list``
    
## Test network traffic using wrk2
To test traffic sending from worker node to master node, we utilizes wrk2 tool.
For the installation and tutorial you can follow this official site:

https://github.com/giltene/wrk2

Send traffic from worker to master

``./wrk -t16 -c5000 -d50 -R100 -L http://10.0.1.142:80/``

where t stands for the number of threads, c number of simutanous connections, d is the time length during test, R is the amount of of connection for the test. 
L is the URL of master's address.









