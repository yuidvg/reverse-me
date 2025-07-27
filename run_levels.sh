#!/bin/bash

echo "Reverse Engineering Challenge Runner"
echo "===================================="
echo ""

case "$1" in
    "1")
        echo "Running Level 1 (32-bit)..."
        qemu-i386-static level1/level1
        ;;
    "2")
        echo "Running Level 2 (32-bit)..."
        qemu-i386-static level2/level2
        ;;
    "3")
        echo "Running Level 3 (64-bit)..."
        qemu-x86_64-static ./level3
        ;;
    *)
        echo "Usage: $0 [1|2|3]"
        echo ""
        echo "Available levels:"
        echo "  1 - Level 1 (32-bit binary)"
        echo "  2 - Level 2 (32-bit binary)"
        echo "  3 - Level 3 (64-bit binary)"
        echo ""
        echo "All binaries now run with QEMU emulation and GLIBC 2.36 support."
        ;;
esac