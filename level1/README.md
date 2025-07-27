# CrackMe Level 1 – Detailed Solution

**Target binary:** `level1`

---

## Objective
1. Discover the secret password required by the binary to print the _"Good job."_ message.
2. Re-implement the binary’s behaviour in clean C code.

---

## Tooling Used
* `strings` – quick scan for readable strings.
* `objdump -d` – disassemble the executable.
* `objdump -s -j .rodata` / `xxd` – dump raw bytes of the `.rodata` section.
* `grep` – search inside the binary for offsets of specific strings.
* Occasional runs of the program to validate hypotheses.

All commands were executed on Linux using the standard tool-chain that ships with Ubuntu 22.04.

---

## Step-by-Step Reverse-Engineering Walk-through

### 1. First glance: see what the program prints
```bash
$ ./level1
Please enter key: <waits for stdin>
```
After supplying an arbitrary word:
```bash
$ echo test | ./level1
Please enter key: Nope.
```
So the program prints _“Good job.”_ only if the supplied key matches the embedded password.

### 2. Scan for useful strings
```bash
$ strings -n 4 level1 | head -n 40
…
Please enter key:
Good job.
Nope.
```
No obvious password string appears, which suggests it might be hidden in data that isn’t a standard ASCII C-string or that it is pieced together in code.

### 3. Disassemble `main`
```bash
$ objdump -d level1 | less   # and locate <main>
```
Inside `main` I quickly notice the sequence:
```asm
11e0: 8b 83 08 e0 ff ff    mov -0x1ff8(%ebx),%eax
11e6: 89 45 86             mov %eax,-0x7a(%ebp)
11e9: 8b 83 0c e0 ff ff    mov -0x1ff4(%ebx),%eax
11ef: 89 45 8a             mov %eax,-0x76(%ebp)
11f2: 8b 83 10 e0 ff ff    mov -0x1ff0(%ebx),%eax
11f8: 89 45 8e             mov %eax,-0x72(%ebp)
11fb: 66 8b 83 14 e0 ff ff mov -0x1fec(%ebx),%ax
1202: 66 89 45 92          mov %ax,-0x6e(%ebp)
```
Interpretation: five successive reads copy 14 bytes **from the binary’s read-only data** into the local stack buffer beginning at `-0x7a(%ebp)`. Immediately afterwards that buffer is compared to the user input via `strcmp`:
```asm
1232: lea -0x7a(%ebp),%edx   ; pointer to secret string
1235: …
123c: call strcmp@plt
```
Hence the password sits in the 14 bytes starting at address `%ebx-0x1ff8` (first `mov`).

### 4. Map the offsets back to file addresses
When `main` starts it adjusts `%ebx` with:
```asm
11d0: add $0x2e31,%ebx   ; reloc-style PIC base
```
So the runtime address `%ebx-0x1ff8` translates to file offset:
```
(0x2e31) - 0x1ff8 = 0x2008
```
That is inside `.rodata`.

### 5. Dump that region of `.rodata`
```bash
$ xxd -g 1 -c 16 -s 0x2000 -l 64 level1
…
00002008: 5f 5f 73 74 61 63 6b 5f 63 68 65 63 6b 00
```
Converting the ASCII bytes up to the NUL terminator yields:
```
__stack_check
```
That is exactly 13 printable characters + NUL = 14 bytes – matching the earlier copy length.

### 6. Verify the candidate password
```bash
$ echo '__stack_check' | ./level1
Please enter key: Good job.
```
Success! The password is `__stack_check`.

---

## Clean Re-implementation (`source.c`)
`easy/source.c` reproduces the logic:
1. Prompt the user.
2. Read an input string.
3. Compare against the constant `"__stack_check"`.
4. Print _“Good job.”_ or _“Nope.”_ accordingly.

The corresponding executable behaves exactly like the original crackme.

---

## Files in the `level1/` directory
| File | Purpose |
|------|---------|
| `password.txt` | Contains the secret password (`__stack_check`). |
| `source.c` | C source replicating original binary behaviour. |
| `README.md` | This detailed explanation. |

---

## Compilation instructions
```bash
cd level1
cc -Wall -Wextra -O2 source.c -o level1_copy
./level1_copy
```

Entering `__stack_check` should display:
```
Please enter key: Good job.
```

---

Happy cracking! :) 