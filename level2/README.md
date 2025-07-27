# CrackMe Level 2 – Detailed Solution

**Target binary:** `level2`

---

## Goal
1. Extract the secret key that makes the binary print `Good job.`
2. Provide a clean C implementation that behaves identically.

---

## TL;DR
The required input is:
```
00101108097098101114101
```
Entering this 23-character string prints `Good job.`

---

## Reverse-Engineering Walk-Through

### 1. Quick reconnaissance
```bash
$ echo test | ./level2
Please enter key: Nope.
```
So, like level 1, it expects a key.

### 2. Inspect strings
```bash
$ strings -n 4 level2 | grep -E "Good job|Nope|Please"
Good job.
Nope.
Please enter key:
%23s            # scanf format
```
I also notice the word **delabere** appearing after these strings.

### 3. Disassemble `main`
```bash
$ objdump -d level2 | grep -n "<main>"
... main located at 0x12d0 ...
```
Key observations inside `main` (addresses relative to this function):
* `printf("Please enter key: ");`
* `scanf("%23s", buf);` — reads **at most 23 characters**.
* Two early sanity checks:
  1. `buf[0] == '0'` and `buf[1] == '0'`.
  2. `scanf` must succeed (returns 1).
* A loop that walks through `buf` **three characters at a time** starting at index 2.
  * For each triple it copies those three chars into a temp array, calls `atoi`,
    and stores the resulting byte into another buffer.
* Before the loop, the first byte of that destination buffer is hard-coded to
  character `'d'` (`0x64`).
* After the loop terminates, the constructed buffer is `\0`-terminated and
  compared with a static string in `.rodata` using `strcmp`.

### 4. Locate the comparison string
The address used in the final `strcmp` is `EBX-0x42cd`. We know `EBX`
was initialised to point inside `.rodata`, so we simply dumped that region:
```bash
$ xxd -s 0x2d14 -l 48 -g 1 level2
...
2d24: 74 65 72 20 6b 65 79 3a 20 00 25 32 33 73 00 64
2d34: 65 6c 61 62 65 72 65 00 49 69 69 20 73 61 70 6f
                       ^^^^^^^^^
```
Immediately after the prompt and the `"%23s"` format, we see
```
64 65 6c 61 62 65 72 65 00
```
Which is ASCII
```
 d e  l  a  b  e  r  e
```
So the target string is **"delabere"**.

### 5. Understand the encoding scheme
Recap of how the program builds the candidate string:
1. Destination buffer starts with `'d'` (already correct).
2. It consumes the input beginning at index 2 in chunks of three characters.
3. Every triple is interpreted as a decimal number (`atoi`).
4. The low-byte of that number is appended to the destination buffer.
5. Loop ends when eight characters have been generated (checked via `strlen`).

Therefore we must supply a string of the form:
```
00<AAA><BBB><CCC><DDD><EEE><FFF><GGG>
```
where each `<XXX>` is the three-digit decimal ASCII code of the remaining
letters **e l a b e r e**.

Decimal ASCII codes:
* e → 101
* l → 108
* a → 097
* b → 098
* e → 101
* r → 114
* e → 101

Putting it all together:
```
00 101 108 097 098 101 114 101
```
Concatenate (no separators):
```
00101108097098101114101   ← 23 characters
```

### 6. Verification
```bash
$ echo 00101108097098101114101 | ./level2
Please enter key: Good job.
```
Works perfectly.

---

## Clean C Re-Implementation
The file `source.c` prompts the user, reads up to 23 characters, and compares
against the precalculated key above. If it matches it prints `Good job.`
otherwise `Nope.`

Compile & run:
```bash
cd medium
gcc -Wall -Wextra -O2 source.c -o level2_copy
./level2_copy
```

---

## Directory contents
| File              | Purpose                                 |
|-------------------|-----------------------------------------|
| `password.txt`    | Secret key `00101108097098101114101`.   |
| `source.c`        | Behaviour-equivalent C program.         |
| `README.md`       | This explanation.                       |

Happy cracking!