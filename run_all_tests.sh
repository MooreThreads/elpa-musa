#!/bin/bash
# ELPA MUSA GPU full test suite

ELPA_DIR="/data/4.16elpa/elpa"
cd "$ELPA_DIR"

FC="mpif90"
FFLAGS="-O2 -cpp -I. -Iprivate_modules -Itest_modules -I/usr/local/include/elpa-2026.02.001/modules -I/usr/local/include/elpa-2026.02.001"
LIBS=".libs/libelpatest.a .libs/libelpa.a -lopenblas -lscalapack-openmpi -L/usr/local/musa/lib -lmusart -lmublas -lmusolver -lstdc++ -lgfortran"
export LD_LIBRARY_PATH=/usr/local/musa/lib:.libs:$LD_LIBRARY_PATH
MPI_RUN="mpirun --allow-run-as-root --oversubscribe -np 1"
GPU_OFF="-DTEST_NVIDIA_GPU=0 -DTEST_AMD_GPU=0 -DTEST_INTEL_GPU=0 -DTEST_OPENMP_OFFLOAD_GPU=0 -DTEST_INTEL_GPU_OPENMP=0 -DTEST_INTEL_GPU_SYCL=0"
GPU_ON="-DTEST_NVIDIA_GPU=1 -DTEST_GPU_SET_ID=0 -DTEST_GPU_DEVICE_POINTER_API=0"

PASS=0
FAIL=0
TOTAL=0
RESULTS=""
FAIL_DETAILS=""

compile_and_run() {
    local name="$1"
    local src="$2"
    local defs="$3"
    local binary="/tmp/elpa_test_$$_${name}"

    TOTAL=$((TOTAL + 1))

    # Compile (add TEST_CASE automatically)
    local test_case_def="-DTEST_CASE='\"$name\"'"
    cerr=$(eval $FC $FFLAGS $defs $test_case_def \"$src\" $LIBS -o \"$binary\" 2>&1)
    if [ $? -ne 0 ]; then
        RESULTS="${RESULTS}COMPILE_FAIL  ${name}\n"
        FAIL_DETAILS="${FAIL_DETAILS}\n=== COMPILE_FAIL: ${name} ===\n${cerr}\n"
        FAIL=$((FAIL + 1))
        return
    fi

    # Run with 30s timeout
    output=$(timeout 30 $MPI_RUN "$binary" 2>&1) || true
    rc=$?

    rm -f "$binary"

    if [ $rc -eq 0 ]; then
        RESULTS="${RESULTS}PASS          ${name}\n"
        PASS=$((PASS + 1))
    else
        RESULTS="${RESULTS}FAIL(rc=$rc)   ${name}\n"
        FAIL_DETAILS="${FAIL_DETAILS}\n=== FAIL: ${name} (rc=$rc) ===\n${output}\n"
        FAIL=$((FAIL + 1))
    fi
}

echo "=========================================="
echo " ELPA MUSA GPU Full Test Suite"
echo " GPU: $(LD_LIBRARY_PATH=/usr/local/musa/lib /usr/local/musa/bin/musaInfo 2>/dev/null | grep Name | head -1)"
echo "=========================================="

# ---- GPU eigenvectors tests ----
echo ""
echo ">> GPU eigenvectors tests"
for domain in real complex; do
    for prec in double single; do
        for solver in 1stage 2stage; do
            for matrix in analytic toeplitz frank; do
                DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_${solver^^}"
                DEFS="$DEFS $GPU_ON -DTEST_EIGENVECTORS -DTEST_MATRIX_${matrix^^}"
                if [ "$solver" = "2stage" ]; then
                    DEFS="$DEFS -DTEST_ALL_KERNELS"
                fi
                NAME="eigvec_${domain}_${prec}_${solver}_${matrix}_gpu"
                printf "  %-55s " "$NAME"
                compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
                # Print last result
                echo -e "$RESULTS" | tail -1
            done
        done
    done
done

# ---- GPU eigenvalues tests ----
echo ""
echo ">> GPU eigenvalues tests"
for domain in real complex; do
    for prec in double single; do
        for solver in 1stage 2stage; do
            DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_${solver^^}"
            DEFS="$DEFS $GPU_ON -DTEST_EIGENVALUES -DTEST_MATRIX_ANALYTIC"
            if [ "$solver" = "2stage" ]; then
                DEFS="$DEFS -DTEST_ALL_KERNELS"
            fi
            NAME="eigval_${domain}_${prec}_${solver}_gpu"
            printf "  %-55s " "$NAME"
            compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
            echo -e "$RESULTS" | tail -1
        done
    done
done

# ---- GPU Cholesky tests ----
echo ""
echo ">> GPU Cholesky tests"
for domain in real complex; do
    for prec in double single; do
        DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_1STAGE"
        DEFS="$DEFS $GPU_ON -DTEST_CHOLESKY -DTEST_MATRIX_ANALYTIC"
        NAME="cholesky_${domain}_${prec}_gpu"
        printf "  %-55s " "$NAME"
        compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
        echo -e "$RESULTS" | tail -1
    done
done

# ---- GPU Hermitian multiply tests ----
echo ""
echo ">> GPU Hermitian multiply tests"
for domain in real complex; do
    for prec in double single; do
        for variant in full upper lower; do
            DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_1STAGE"
            DEFS="$DEFS $GPU_ON -DTEST_HERMITIAN_MULTIPLY_${variant^^} -DTEST_MATRIX_ANALYTIC"
            NAME="hermitian_multiply_${variant}_${domain}_${prec}_gpu"
            printf "  %-55s " "$NAME"
            compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
            echo -e "$RESULTS" | tail -1
        done
    done
done

# ---- GPU solve tridiagonal tests ----
echo ""
echo ">> GPU solve tridiagonal tests"
for domain in real complex; do
    for prec in double single; do
        DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_1STAGE"
        DEFS="$DEFS $GPU_ON -DTEST_SOLVE_TRIDIAGONAL -DTEST_MATRIX_ANALYTIC"
        NAME="solve_tridiagonal_${domain}_${prec}_gpu"
        printf "  %-55s " "$NAME"
        compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
        echo -e "$RESULTS" | tail -1
    done
done

# ---- GPU invert triangular tests ----
echo ""
echo ">> GPU invert triangular tests"
for domain in real complex; do
    for prec in double single; do
        DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_1STAGE $GPU_ON"
        NAME="invert_trm_${domain}_${prec}_gpu"
        printf "  %-55s " "$NAME"
        compile_and_run "$NAME" "test/Fortran/test_invert_triangular.F90" "$DEFS"
        echo -e "$RESULTS" | tail -1
    done
done

# ---- GPU generalized eigenproblem tests ----
echo ""
echo ">> GPU generalized eigenproblem tests"
for domain in real complex; do
    for prec in double single; do
        DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_1STAGE"
        DEFS="$DEFS $GPU_ON -DTEST_GENERALIZED_EIGENPROBLEM -DTEST_MATRIX_ANALYTIC"
        NAME="generalized_${domain}_${prec}_1stage_gpu"
        printf "  %-55s " "$NAME"
        compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
        echo -e "$RESULTS" | tail -1
    done
done

# ---- CPU baseline tests ----
echo ""
echo ">> CPU baseline tests"
for domain in real complex; do
    for prec in double single; do
        for solver in 1stage 2stage; do
            DEFS="-DTEST_${domain^^} -DTEST_${prec^^} -DTEST_SOLVER_${solver^^}"
            DEFS="$DEFS $GPU_OFF -DTEST_EIGENVECTORS -DTEST_MATRIX_ANALYTIC"
            if [ "$solver" = "2stage" ]; then
                DEFS="$DEFS -DTEST_ALL_KERNELS"
            fi
            NAME="eigvec_${domain}_${prec}_${solver}_analytic_cpu"
            printf "  %-55s " "$NAME"
            compile_and_run "$NAME" "test/Fortran/test.F90" "$DEFS"
            echo -e "$RESULTS" | tail -1
        done
    done
done

echo ""
echo "=========================================="
echo " Test Results Summary"
echo "=========================================="
echo -e "$RESULTS" | grep -v '^$' | sort
echo ""
echo "=========================================="
printf " TOTAL: %d   PASS: %d   FAIL: %d\n" $TOTAL $PASS $FAIL
echo "=========================================="

if [ $FAIL -gt 0 ]; then
    echo ""
    echo "=========================================="
    echo " Failure Details"
    echo "=========================================="
    echo -e "$FAIL_DETAILS"
fi
