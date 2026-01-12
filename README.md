# Stack Adjacency Leak PoC

**An analysis of C memory layout and the blind spots of automated security tools.**

## 🔍 Overview
This project demonstrates a specific class of memory vulnerability known as a **Stack Adjacency Leak**. By exploiting the deterministic memory layout of Linux processes, this Proof of Concept (PoC) shows how a simple buffer over-read can silently exfiltrate sensitive **Environment Variables** (like session keys or tokens).

Crucially, this project highlights a limitation in **GCC AddressSanitizer (ASan)**: the tool fails to detect this leak because the read operations occur within valid, contiguous stack memory allocated by the operating system.

## 🛠️ The Mechanics
When a Linux process starts, the kernel lays out the stack as follows:

```text
[ argv[0] ... argv[argc] (NULL) ] [ envp[0] ... envp[n] ] [ Auxiliary Vector ]
```

Because `envp` (Environment Pointers) is placed immediately after `argv` (Argument Pointers), an application that iterates past `argc` does not hit unallocated memory. Instead, it seamlessly transitions into reading the environment block.

## 🚀 How to Run

### 1. Compilation
Use the included Makefile to compile both the vulnerable and secure versions. Note that we compile **with** AddressSanitizer enabled to prove it doesn't stop the leak.
```bash
make
```

### 2. The Vulnerability Demo
Run the vulnerable executable with a read count higher than the number of arguments provided.

```bash
# Request 20 reads, but only provide 1 argument ("test")
./vulnerable 20 "test"
```

**Expected Output:**
1. Prints the arguments you provided.
2. Prints `(null)` (the end of argv).
3. **LEAK:** Prints your Environment Variables (e.g., `SHELL`, `USER`, `SSH_AUTH_SOCK`).
4. **CRASH:** Eventually hits the Auxiliary Vector (non-string data) and segfaults.

### 3. The Mitigation
Run the secure version with the same parameters.
```bash
./secure 20 "test"
```
**Expected Output:** The program detects the boundary and stops before leaking data.

## 📊 Security Analysis
| Phase | Action | Result | ASan Detection? |
|-------|--------|--------|-----------------|
| 1 | Read `argv[i]` | Normal Output | N/A |
| 2 | Read `argv[argc+1]` | **Environment Leak** | ❌ **FAILED** (Silent) |
| 3 | Read `auxv` | Segfault/Crash | ✅ **DETECTED** (Too late) |

## 📝 Conclusion
Input validation (Logic Safety) is distinct from Memory Safety. Tools like ASan protect against invalid memory access, but they cannot protect against logic errors that access valid-but-unintended data structures. Explicit bounds checking is the only defense.

---
*Created for Academic Security Research.*
