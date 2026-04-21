#!/bin/bash
# CUDA to MUSA porting script for ELPA
# Converts .cu -> .mu, .cuh -> .muh, applies API name replacements

convert_file() {
    local src="$1"
    local dst="$2"

    sed \
        -e 's|<cuda_runtime\.h>|<musa_runtime.h>|g' \
        -e 's|"cuda_runtime\.h"|"musa_runtime.h"|g' \
        -e 's|<cuComplex\.h>|<muComplex.h>|g' \
        -e 's|<cublas_v2\.h>|<mublas.h>|g' \
        -e 's|<cublas_api\.h>|<mublas.h>|g' \
        -e 's|<cublasLt\.h>|<mublasLt.h>|g' \
        -e 's|<cusolverDn\.h>|<musolverDn.h>|g' \
        -e 's|"nccl\.h"|"mccl.h"|g' \
        -e 's|<nvToolsExt\.h>|/* nvToolsExt not available on MUSA */|g' \
        -e 's|cudaDeviceAttr|musaDeviceAttr|g' \
        -e 's|cudaDevAttrMaxThreadsPerBlock|musaDevAttrMaxThreadsPerBlock|g' \
        -e 's|cudaDevAttrMaxBlockDimX|musaDevAttrMaxBlockDimX|g' \
        -e 's|cudaDevAttrMaxBlockDimY|musaDevAttrMaxBlockDimY|g' \
        -e 's|cudaDevAttrMaxBlockDimZ|musaDevAttrMaxBlockDimZ|g' \
        -e 's|cudaDevAttrMaxGridDimX|musaDevAttrMaxGridDimX|g' \
        -e 's|cudaDevAttrMaxGridDimY|musaDevAttrMaxGridDimY|g' \
        -e 's|cudaDevAttrMaxGridDimZ|musaDevAttrMaxGridDimZ|g' \
        -e 's|cudaDevAttrWarpSize|musaDevAttrWarpSize|g' \
        -e 's|cudaDevAttrMultiProcessorCount|musaDevAttrMultiprocessorCount|g' \
        -e 's|cudaDeviceGetAttribute|musaDeviceGetAttribute|g' \
        -e 's|cudaGetDevice|musaGetDevice|g' \
        -e 's|cudaDeviceProp|musaDeviceProp|g' \
        -e 's|cudaGetDeviceProperties|musaGetDeviceProperties|g' \
        -e 's|cudaError_t|musaError_t|g' \
        -e 's|cudaSuccess|musaSuccess|g' \
        -e 's|cudaGetLastError|musaGetLastError|g' \
        -e 's|cudaGetErrorString|musaGetErrorString|g' \
        -e 's|cudaStream_t|musaStream_t|g' \
        -e 's|cudaStreamCreate|musaStreamCreate|g' \
        -e 's|cudaStreamDestroy|musaStreamDestroy|g' \
        -e 's|cudaStreamSynchronize|musaStreamSynchronize|g' \
        -e 's|cudaStreamPerThread|musaStreamPerThread|g' \
        -e 's|cudaDeviceSynchronize|musaDeviceSynchronize|g' \
        -e 's|cudaMemcpy2DAsync|musaMemcpy2DAsync|g' \
        -e 's|cudaMemcpyKind|musaMemcpyKind|g' \
        -e 's|cudaMemcpyHostToDevice|musaMemcpyHostToDevice|g' \
        -e 's|cudaMemcpyDeviceToHost|musaMemcpyDeviceToHost|g' \
        -e 's|cudaMemcpyDeviceToDevice|musaMemcpyDeviceToDevice|g' \
        -e 's|cudaMemcpyHostToHost|musaMemcpyHostToHost|g' \
        -e 's|cudaMemcpy|musaMemcpy|g' \
        -e 's|cudaMalloc|musaMalloc|g' \
        -e 's|cudaFree|musaFree|g' \
        -e 's|cudaMallocHost|musaMallocHost|g' \
        -e 's|cudaFreeHost|musaFreeHost|g' \
        -e 's|cudaMemset|musaMemset|g' \
        -e 's|cudaSetDevice|musaSetDevice|g' \
        -e 's|cudaGetDeviceCount|musaGetDeviceCount|g' \
        -e 's|cudaPointerAttributes|musaPointerAttributes|g' \
        -e 's|cudaPointerGetAttributes|musaPointerGetAttributes|g' \
        -e 's|cudaMemoryTypeHost|musaMemoryTypeHost|g' \
        -e 's|cudaMemoryTypeDevice|musaMemoryTypeDevice|g' \
        -e 's|cudaEventCreate|musaEventCreate|g' \
        -e 's|cudaEventDestroy|musaEventDestroy|g' \
        -e 's|cudaEventRecord|musaEventRecord|g' \
        -e 's|cudaEventSynchronize|musaEventSynchronize|g' \
        -e 's|cudaEventElapsedTime|musaEventElapsedTime|g' \
        -e 's|cudaEvent_t|musaEvent_t|g' \
        -e 's|cublasHandle_t|mublasHandle_t|g' \
        -e 's|cublasStatus_t|mublasStatus|g' \
        -e 's|CUBLAS_STATUS_SUCCESS|MUBLAS_STATUS_SUCCESS|g' \
        -e 's|CUBLAS_STATUS_NOT_INITIALIZED|MUBLAS_STATUS_NOT_INITIALIZED|g' \
        -e 's|CUBLAS_STATUS_ALLOC_FAILED|MUBLAS_STATUS_ALLOC_FAILED|g' \
        -e 's|CUBLAS_STATUS_INVALID_VALUE|MUBLAS_STATUS_INVALID_VALUE|g' \
        -e 's|CUBLAS_STATUS_MAPPING_ERROR|MUBLAS_STATUS_MAPPING_ERROR|g' \
        -e 's|CUBLAS_STATUS_EXECUTION_FAILED|MUBLAS_STATUS_EXECUTION_FAILED|g' \
        -e 's|CUBLAS_STATUS_INTERNAL_ERROR|MUBLAS_STATUS_INTERNAL_ERROR|g' \
        -e 's|CUBLAS_OP_N|MUBLAS_OP_N|g' \
        -e 's|CUBLAS_OP_T|MUBLAS_OP_T|g' \
        -e 's|CUBLAS_OP_C|MUBLAS_OP_C|g' \
        -e 's|CUBLAS_FILL_MODE_UPPER|MUBLAS_FILL_MODE_UPPER|g' \
        -e 's|CUBLAS_FILL_MODE_LOWER|MUBLAS_FILL_MODE_LOWER|g' \
        -e 's|CUBLAS_SIDE_LEFT|MUBLAS_SIDE_LEFT|g' \
        -e 's|CUBLAS_SIDE_RIGHT|MUBLAS_SIDE_RIGHT|g' \
        -e 's|CUBLAS_DIAG_NON_UNIT|MUBLAS_DIAG_NON_UNIT|g' \
        -e 's|CUBLAS_DIAG_UNIT|MUBLAS_DIAG_UNIT|g' \
        -e 's|CUBLAS_VERSION|MUBLAS_VERSION|g' \
        -e 's|cublasCreate|mublasCreate|g' \
        -e 's|cublasDestroy|mublasDestroy|g' \
        -e 's|cublasSetStream|mublasSetStream|g' \
        -e 's|cublasGetStream|mublasGetStream|g' \
        -e 's|cublasGetVersion|mublasGetVersion|g' \
        -e 's|cublasLtHeuristicsCacheSetCapacity|/* mublasLt: cache control not available */\/\/cublasLtHeuristicsCacheSetCapacity|g' \
        -e 's|cublasDgemm|mublasDgemm|g' \
        -e 's|cublasSgemm|mublasSgemm|g' \
        -e 's|cublasZgemm|mublasZgemm|g' \
        -e 's|cublasCgemm|mublasCgemm|g' \
        -e 's|cublasDgemv|mublasDgemv|g' \
        -e 's|cublasSgemv|mublasSgemv|g' \
        -e 's|cublasZgemv|mublasZgemv|g' \
        -e 's|cublasCgemv|mublasCgemv|g' \
        -e 's|cublasDtrmm|mublasDtrmm|g' \
        -e 's|cublasStrmm|mublasStrmm|g' \
        -e 's|cublasZtrmm|mublasZtrmm|g' \
        -e 's|cublasCtrmm|mublasCtrmm|g' \
        -e 's|cublasDtrsm|mublasDtrsm|g' \
        -e 's|cublasStrsm|mublasStrsm|g' \
        -e 's|cublasZtrsm|mublasZtrsm|g' \
        -e 's|cublasCtrsm|mublasCtrsm|g' \
        -e 's|cublasDcopy|mublasDcopy|g' \
        -e 's|cublasScopy|mublasScopy|g' \
        -e 's|cublasZcopy|mublasZcopy|g' \
        -e 's|cublasCcopy|mublasCcopy|g' \
        -e 's|cublasDtrmv|mublasDtrmv|g' \
        -e 's|cublasStrmv|mublasStrmv|g' \
        -e 's|cublasZtrmv|mublasZtrmv|g' \
        -e 's|cublasCtrmv|mublasCtrmv|g' \
        -e 's|cublasDaxpy|mublasDaxpy|g' \
        -e 's|cublasSaxpy|mublasSaxpy|g' \
        -e 's|cublasZaxpy|mublasZaxpy|g' \
        -e 's|cublasCaxpy|mublasCaxpy|g' \
        -e 's|cublasDscal|mublasDscal|g' \
        -e 's|cublasSscal|mublasSscal|g' \
        -e 's|cublasZscal|mublasZscal|g' \
        -e 's|cublasCscal|mublasCscal|g' \
        -e 's|cublasDdot|mublasDdot|g' \
        -e 's|cublasSdot|mublasSdot|g' \
        -e 's|cublasZdotc|mublasZdotc|g' \
        -e 's|cublasCdotc|mublasCdotc|g' \
        -e 's|cublasDnrm2|mublasDnrm2|g' \
        -e 's|cublasSnrm2|mublasSnrm2|g' \
        -e 's|cublasDznrm2|mublasDznrm2|g' \
        -e 's|cublasScnrm2|mublasScnrm2|g' \
        -e 's|cublasIdamax|mublasIdamax|g' \
        -e 's|cublasIsamax|mublasIsamax|g' \
        -e 's|cublasIzamax|mublasIzamax|g' \
        -e 's|cublasIcamax|mublasIcamax|g' \
        -e 's|cublasDsyrk|mublasDsyrk|g' \
        -e 's|cublasSsyrk|mublasSsyrk|g' \
        -e 's|cublasZherk|mublasZherk|g' \
        -e 's|cublasCherk|mublasCherk|g' \
        -e 's|cusolverDnHandle_t|musolverDnHandle_t|g' \
        -e 's|cusolverStatus_t|musolverStatus_t|g' \
        -e 's|CUSOLVER_STATUS_SUCCESS|MUSOLVER_STATUS_SUCCESS|g' \
        -e 's|cusolverDnCreate|musolverDnCreate|g' \
        -e 's|cusolverDnDestroy|musolverDnDestroy|g' \
        -e 's|cusolverDnSetStream|musolverDnSetStream|g' \
        -e 's|cusolverDnDpotrf_bufferSize|musolverDnDpotrf_bufferSize|g' \
        -e 's|cusolverDnSpotrf_bufferSize|musolverDnSpotrf_bufferSize|g' \
        -e 's|cusolverDnZpotrf_bufferSize|musolverDnZpotrf_bufferSize|g' \
        -e 's|cusolverDnCpotrf_bufferSize|musolverDnCpotrf_bufferSize|g' \
        -e 's|cusolverDnDpotrf|musolverDnDpotrf|g' \
        -e 's|cusolverDnSpotrf|musolverDnSpotrf|g' \
        -e 's|cusolverDnZpotrf|musolverDnZpotrf|g' \
        -e 's|cusolverDnCpotrf|musolverDnCpotrf|g' \
        -e 's|CUBLAS_FILL_MODE|MUBLAS_FILL_MODE|g' \
        -e 's|cuDoubleComplex|muDoubleComplex|g' \
        -e 's|cuFloatComplex|muFloatComplex|g' \
        -e 's|make_cuDoubleComplex|make_muDoubleComplex|g' \
        -e 's|make_cuFloatComplex|make_muFloatComplex|g' \
        -e 's|ncclComm_t|mcclComm_t|g' \
        -e 's|ncclUniqueId|mcclUniqueId|g' \
        -e 's|ncclResult_t|mcclResult_t|g' \
        -e 's|ncclSuccess|mcclSuccess|g' \
        -e 's|ncclGetUniqueId|mcclGetUniqueId|g' \
        -e 's|ncclCommInitRank|mcclCommInitRank|g' \
        -e 's|ncclCommDestroy|mcclCommDestroy|g' \
        -e 's|ncclAllReduce|mcclAllReduce|g' \
        -e 's|ncclBroadcast|mcclBroadcast|g' \
        -e 's|ncclReduce|mcclReduce|g' \
        -e 's|ncclDouble|mcclDouble|g' \
        -e 's|ncclFloat|mcclFloat|g' \
        -e 's|ncclSum|mcclSum|g' \
        -e 's|ncclGroupStart|mcclGroupStart|g' \
        -e 's|ncclGroupEnd|mcclGroupEnd|g' \
        -e 's|nvtxRangePushA|/* nvtx not available */\/\/nvtxRangePushA|g' \
        -e 's|nvtxRangePop|/* nvtx not available */\/\/nvtxRangePop|g' \
        -e 's|DEBUG_CUDA|DEBUG_MUSA|g' \
        -e 's|WITH_NVIDIA_GPU_VERSION|WITH_MUSA_GPU_VERSION|g' \
        -e 's|WITH_NVIDIA_CUSOLVER|WITH_MUSA_MUSOLVER|g' \
        -e 's|WITH_NVTX|WITH_MUSA_NVTX|g' \
        "$src" > "$dst"
}

echo "=== Porting ELPA CUDA source to MUSA ==="

# src/GPU/CUDA -> src/GPU/MUSA
for f in src/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g; s/cuUtils/muUtils/g; s/cuUtils/muUtils/g')
    convert_file "$f" "src/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/GPU/MUSA/${newname}.mu"
done

for f in src/GPU/CUDA/*.h; do
    base=$(basename "$f")
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g; s/cusolver/musolver/g; s/cuUtils/muUtils/g; s/nccl/mccl/g')
    convert_file "$f" "src/GPU/MUSA/${newname}"
    echo "  $f -> src/GPU/MUSA/${newname}"
done

# ncclFunctions.cpp -> mcclFunctions.cpp
if [ -f src/GPU/CUDA/ncclFunctions.cpp ]; then
    convert_file "src/GPU/CUDA/ncclFunctions.cpp" "src/GPU/MUSA/mcclFunctions.cpp"
    echo "  ncclFunctions.cpp -> mcclFunctions.cpp"
fi

# elpa1/GPU/CUDA -> elpa1/GPU/MUSA
for f in src/elpa1/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "src/elpa1/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/elpa1/GPU/MUSA/${newname}.mu"
done

# elpa2/GPU/CUDA -> elpa2/GPU/MUSA (skip sm80 PTX kernel)
for f in src/elpa2/GPU/CUDA/ev_tridi_band_nvidia_gpu_real.cu src/elpa2/GPU/CUDA/ev_tridi_band_nvidia_gpu_complex.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/nvidia/musa/g')
    convert_file "$f" "src/elpa2/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/elpa2/GPU/MUSA/${newname}.mu"
done

# cholesky
for f in src/cholesky/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "src/cholesky/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/cholesky/GPU/MUSA/${newname}.mu"
done

# invert_trm
for f in src/invert_trm/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "src/invert_trm/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/invert_trm/GPU/MUSA/${newname}.mu"
done

# multiply_a_b
for f in src/multiply_a_b/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "src/multiply_a_b/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/multiply_a_b/GPU/MUSA/${newname}.mu"
done

# solve_tridi
for f in src/solve_tridi/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "src/solve_tridi/GPU/MUSA/${newname}.mu"
    echo "  $f -> src/solve_tridi/GPU/MUSA/${newname}.mu"
done

# test
for f in test/shared/GPU/CUDA/*.cu; do
    base=$(basename "$f" .cu)
    newname=$(echo "$base" | sed 's/cuda/musa/g; s/Cuda/Musa/g')
    convert_file "$f" "test/shared/GPU/MUSA/${newname}.mu"
    echo "  $f -> test/shared/GPU/MUSA/${newname}.mu"
done

echo "=== Porting complete ==="
echo "Files created:"
find src/GPU/MUSA src/elpa1/GPU/MUSA src/elpa2/GPU/MUSA src/cholesky/GPU/MUSA src/invert_trm/GPU/MUSA src/multiply_a_b/GPU/MUSA src/solve_tridi/GPU/MUSA test/shared/GPU/MUSA -type f | wc -l
