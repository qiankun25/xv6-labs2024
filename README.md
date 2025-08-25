# xv6-labs-2024 实验汇总

- 2351232 魏义乾

本项目基于 [xv6-labs-2024](https://pdos.csail.mit.edu/6.S081/2024/labs.html) 实验，包含 9 个实验模块，涵盖 Unix 工具、系统调用、页表、陷阱机制、写时复制、网络、锁、文件系统以及内存映射等多个操作系统核心主题。每个实验均对应独立分支，便于切换和管理。

---

## 目录

- [xv6-labs-2024 实验汇总](#xv6-labs-2024-实验汇总)
  - [目录](#目录)
  - [仓库简介](#仓库简介)
  - [实验列表](#实验列表)
    - [环境搭建与 Tools](#环境搭建与-tools)
    - [Lab1: Xv6 and Unix utilities](#lab1-xv6-and-unix-utilities)
    - [Lab2: System Calls](#lab2-system-calls)
    - [Lab3: Page Tables](#lab3-page-tables)
    - [Lab4: Traps](#lab4-traps)
    - [Lab5: Copy-on-Write Fork for xv6](#lab5-copy-on-write-fork-for-xv6)
    - [Lab6: Networking](#lab6-networking)
    - [Lab7: Locks](#lab7-locks)
    - [Lab8: File System](#lab8-file-system)
    - [Lab9: mmap](#lab9-mmap)
  - [实验准备与常规步骤](#实验准备与常规步骤)
  - [参考资料](#参考资料)

---

## 仓库简介

本仓库用于记录和提交 xv6-labs-2024 各实验的实现与分析。你可以在对应分支下查看和运行每个实验的具体代码与测试用例。

---

## 实验列表

每个实验详细内容见对应 `lab*.pdf` 或官方实验文档。

### 环境搭建与 Tools

- **内容**：建议在 Ubuntu（建议 20.04+）或 WSL、VMware 虚拟机内搭建开发环境。推荐使用 VS Code 进行代码编写，qemu 用于运行和调试 xv6。详细搭建说明见 `tools` 文档。
- **分支**：`main` 或 `tools`
- **测试方法**：
  ```bash
  make qemu
  ```

---

### Lab1: Xv6 and Unix utilities

- **内容**：熟悉 xv6 基本系统调用及命令，实现 `sleep`、`pingpong`、`primes`、`find`、`xargs` 等工具。
- **分支**：`util`
- **测试方法**：
  ```bash
  make grade
  ```

---

### Lab2: System Calls

- **内容**：调试与实现系统调用，包括 gdb 调试、系统调用跟踪、模拟攻击等。
- **分支**：`syscall`
- **测试方法**：
  ```bash
  make grade
  ```

---

### Lab3: Page Tables

- **内容**：深入理解进程页表结构，实现用户进程页表检查、系统调用加速等。
- **分支**：`pgtbl`
- **测试方法**：
  ```bash
  make grade
  ```

---

### Lab4: Traps

- **内容**：RISC-V 汇编、异常（trap）处理、backtrace、alarm 等机制实现。
- **分支**：`traps`
- **测试方法**：
  ```bash
  make grade
  ```

---

### Lab5: Copy-on-Write Fork for xv6

- **内容**：实现写时复制（Copy-on-Write, COW）机制优化 `fork` 性能。
- **分支**：`cow`
- **测试方法**：
  ```bash
  make grade
  ```
  其他测试命令：
  ```bash
  cowtest
  usertests -q
  ```

---

### Lab6: Networking

- **内容**：实现网卡驱动（NIC）、UDP 接收等基础网络功能。
- **分支**：`net`
- **测试方法**：
  ```bash
  make grade
  ```

---

### Lab7: Locks

- **内容**：优化内存分配器、缓冲区缓存等关键路径的锁机制。
- **分支**：`lock`
- **测试方法**：
  ```bash
  make grade
  ```
  其他测试命令：
  ```bash
  make qemu
  bcachetest
  usertests -q
  ```

---

### Lab8: File System

- **内容**：支持大文件与符号链接，完善 xv6 文件系统能力。
- **分支**：`fs`
- **测试方法**：
  ```bash
  make grade
  ```
  其他测试命令：
  ```bash
  bigfile
  usertests -q
  ```

---

### Lab9: mmap

- **内容**：实现 `mmap`/`munmap` 系统调用，支持内存映射文件操作。
- **分支**：`mmp`
- **测试方法**：
  ```bash
  make grade
  ```
  其他测试命令：
  ```bash
  mmaptest
  usertests -q
  ```

---

## 实验准备与常规步骤

每次切换实验时，建议严格按照如下步骤操作：

1. **切换到对应分支**
   ```bash
   git fetch
   git checkout <branch_name>
   make clean
   ```
   其中 `<branch_name>` 替换为当前实验的分支名。

2. **编译与测试**
   ```bash
   make qemu
   # 或根据实验说明执行 make grade / 相关测试脚本
   ```

---

## 参考资料

- [xv6-labs-2024 官方实验](https://pdos.csail.mit.edu/6.S081/2024/labs.html)
- [xv6 官方代码和文档](https://pdos.csail.mit.edu/6.S081/2021/xv6.html)
- [RISC-V 指令集手册](https://riscv.org/technical/specifications/)
- [MIT 6.S081: Operating System Engineering](https://pdos.csail.mit.edu/6.S081/2024/)

---

> 如有疑问或建议，请提 issue 或联系仓库维护者。