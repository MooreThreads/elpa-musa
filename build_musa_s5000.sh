#!/bin/bash
# Build ELPA with MUSA GPU support for MTT S5000 (mp_31)
#
# Prerequisites:
#   - MUSA SDK installed at /usr/local/musa (or set MUSA_HOME)
#   - MPI (OpenMPI): mpifort, mpicc, mpicxx
#   - ScaLAPACK + OpenBLAS
#   - autoconf, automake, libtool, python3
#
# Usage:
#   bash build_musa_s5000.sh [install_prefix]

set -e

MUSA_HOME="${MUSA_HOME:-/usr/local/musa}"
INSTALL_PREFIX="${1:-/tmp/elpa_musa_install}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

echo "=== ELPA MUSA Build for S5000 (mp_31) ==="
echo "MUSA_HOME: $MUSA_HOME"
echo "Install prefix: $INSTALL_PREFIX"

# Generate configure if needed
if [ ! -f "$SCRIPT_DIR/configure" ]; then
    echo ">> Running autogen.sh..."
    cd "$SCRIPT_DIR"
    bash autogen.sh
fi

# Build in a subdirectory
rm -rf "$SCRIPT_DIR/build_s5000"
mkdir -p "$SCRIPT_DIR/build_s5000"
cd "$SCRIPT_DIR/build_s5000"

echo ">> Configuring..."
"$SCRIPT_DIR/configure" \
    --enable-musa-gpu \
    --with-musa-path="$MUSA_HOME" \
    --disable-avx512 \
    --prefix="$INSTALL_PREFIX" \
    MCCFLAGS="-O2 --offload-arch=mp_31 -I${MUSA_HOME}/include" \
    FC=mpifort \
    CC=mpicc \
    CXX=mpicxx \
    CFLAGS="-O2 -msse3 -mavx -mfma -mavx2" \
    SCALAPACK_LDFLAGS="-lscalapack-openmpi" \
    SCALAPACK_FCFLAGS="-I/usr/include"

echo ">> Building (single-threaded to respect Fortran module dependencies)..."
make -j1

echo ">> Installing to $INSTALL_PREFIX..."
make install

echo ""
echo "=== Build successful ==="
echo "Library: $INSTALL_PREFIX/lib/libelpa.so"
echo ""
echo "Quick GPU verification:"
echo "  export LD_LIBRARY_PATH=${MUSA_HOME}/lib:${INSTALL_PREFIX}/lib:\$LD_LIBRARY_PATH"
echo "  # Check GPU count (should return 8 for 8x S5000):"
echo "  echo '#include <stdio.h>"
echo "  extern int nvidia_gpu_count();"
echo "  int main() { printf(\"GPUs: %d\\n\", nvidia_gpu_count()); return 0; }' > /tmp/t.c"
echo "  mpicc /tmp/t.c -L${INSTALL_PREFIX}/lib -lelpa -L${MUSA_HOME}/lib -lmusart -lmublas -lstdc++ -o /tmp/t && /tmp/t"
