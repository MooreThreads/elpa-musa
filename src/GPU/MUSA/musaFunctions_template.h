#if 0
//
//    Copyright 2014 - 2023, A. Marek
//
//    This file is part of ELPA.
//
//    The ELPA library was originally created by the ELPA consortium,
//    consisting of the following organizations:
//
//    - Max Planck Computing and Data Facility (MPCDF), formerly known as
//      Rechenzentrum Garching der Max-Planck-Gesellschaft (RZG),
//    - Bergische Universität Wuppertal, Lehrstuhl für angewandte
//      Informatik,
//    - Technische Universität München, Lehrstuhl für Informatik mit
//      Schwerpunkt Wissenschaftliches Rechnen ,
//    - Fritz-Haber-Institut, Berlin, Abt. Theorie,
//    - Max-Plack-Institut für Mathematik in den Naturwissenschaften,
//      Leipzig, Abt. Komplexe Strukutren in Biologie und Kognition,
//      and
//    - IBM Deutschland GmbH
//
//    This particular source code file contains additions, changes and
//    enhancements authored by Intel Corporation which is not part of
//    the ELPA consortium.
//
//    More information can be found here:
//    http://elpa.mpcdf.mpg.de/
//
//    ELPA is free software: you can redistribute it and/or modify
//    it under the terms of the version 3 of the license of the
//    GNU Lesser General Public License as published by the Free
//    Software Foundation.
//
//    ELPA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with ELPA.  If not, see <http://www.gnu.org/licenses/>
//
//    ELPA reflects a substantial effort on the part of the original
//    ELPA consortium, and we ask you to respect the spirit of the
//    license that we chose: i.e., please contribute any changes you
//    may have back to the original ELPA library distribution, and keep
//    any derivatives of ELPA under the same license that we chose for
//    the original distribution, the GNU Lesser General Public License.
//
//
// --------------------------------------------------------------------------------------------------
//
// This file was written by A. Marek, MPCDF
#endif

#ifdef __cplusplus
extern "C" {    
#endif

int musaDeviceGetAttributeFromC(int *value, int attribute) {
  musaDeviceAttr attr;
  switch(attribute) {
    case 0:
      attr = musaDevAttrMaxThreadsPerBlock;
      break;
    case 1:
      attr = musaDevAttrMaxBlockDimX;
      break;
    case 2:
      attr = musaDevAttrMaxBlockDimY;
      break;
    case 3:
      attr = musaDevAttrMaxBlockDimZ;
      break;
    case 4:
      attr = musaDevAttrMaxGridDimX;
      break;
    case 5:
      attr = musaDevAttrMaxGridDimY;
      break;
    case 6:
      attr = musaDevAttrMaxGridDimZ;
      break;
    case 7:
      attr = musaDevAttrWarpSize;
      break;
    case 8:
      attr = musaDevAttrMultiProcessorCount;
      break;
  }
  
  musaError_t status = musaDeviceGetAttribute(value, attr, 0);
  if (status == musaSuccess) {
    return 1;
  }
  else{
    errormessage("Error in musaDeviceGetAttribute: %s\n", "unknown error");
    return 0;
  }
}

int musaDeviceGetCapabilityFromC(int *value) {
  *value = 0;

  int device;
  musaError_t status = musaGetDevice(&device);
  if (status != musaSuccess) {
    errormessage("Error in musaGetDevice: %s\n", "unknown error");
    return 0;
  }

  musaDeviceProp deviceProp;
  status = musaGetDeviceProperties(&deviceProp, device);
  if (status != musaSuccess) {
    errormessage("Error in musaDeviceGetProperties: %s\n", "unknown error");
    return 0;
  }

  *value = deviceProp.major * 10 + deviceProp.minor;
  return 1;
}

int mublasGetVersionFromC(mublasHandle_t musaHandle, int *version) {
  mublasStatus status = mublasGetVersion(musaHandle, version);
  if (status == MUBLAS_STATUS_SUCCESS) {
    if (*version >= 111103)
      {
      // https://docs.nvidia.com/cuda/archive//11.8.0/cuda-toolkit-release-notes/index.html
      // https://docs.nvidia.com/cuda/archive//12.3.1/cuda-toolkit-release-notes/index.html
      // Heuristics caching for the repeated CUBLAS operations was introduced in CUDA 11.8.0 and improved in CUDA 12.3.1
      // Between these versions, the caching significantly decreases the performance of cublas Gemm, Gemv
      // More recent CUDA versions still suffer from the slowdown for the small matrices, so we switch it off completely
#if defined(MUBLAS_VERSION) && MUBLAS_VERSION >= 111103
      /* mublasLt: cache control not available *///cublasLtHeuristicsCacheSetCapacity(0);
#endif
      }

    return 1;
  }
  else if (status == MUBLAS_STATUS_INVALID_VALUE) {
    errormessage("Error in mublasGetVersion: %s\n", "the provided storage for library version number is not initialized (NULL)");
    return 0;
  }
  else{
    errormessage("Error in mublasGetVersion: %s\n", "unknown error");
    return 0;
  }
}
    
int musaGetLastErrorFromC() {
  musaError_t status = musaGetLastError();
  
  if (status == musaSuccess) {
    return 1;
  }
  else{
    printf("Error in executing  musaGetLastErrorFrom: %s\n", musaGetErrorString(status));
    errormessage("Error in musaGetLastError: %s\n", "unknown error");
    return 0;
  }

}   

int musaStreamCreateFromC(musaStream_t *musaStream) {
  //*stream = (intptr_t) malloc(sizeof(musaStream_t));

  if (sizeof(intptr_t) != sizeof(musaStream_t)) {
    printf("Stream sizes do not match \n");
  }

  musaError_t status = musaStreamCreate(musaStream);

  if (status == musaSuccess) {
//       printf("all OK\n");
    return 1;
  }
  else{
    errormessage("Error in musaStreamCreate: %s\n", "unknown error");
    return 0;
  }

}

int musaStreamDestroyFromC(musaStream_t musaStream){
  musaError_t status = musaStreamDestroy(musaStream);
  if (status == musaSuccess) {
//       printf("all OK\n");
  //free((void*) *stream);
    return 1;
  }
  else{
    errormessage("Error in musaStreamDestroy: %s\n", "unknown error");
    return 0;
  }
}

int musaStreamSynchronizeExplicitFromC(musaStream_t musaStream) {
  musaError_t status = musaStreamSynchronize(musaStream);
  if (status == musaSuccess) {
    return 1;
  }
  else{
    errormessage("Error in musaStreamSynchronizeExplicit: %s\n", "unknown error");
    return 0;
  }
}

int musaStreamSynchronizeImplicitFromC() {
  musaError_t status = musaStreamSynchronize(musaStreamPerThread);
  if (status == musaSuccess) {
    return 1;
  }
  else{
    errormessage("Error in musaStreamSynchronizeImplicit: %s\n", "unknown error");
    return 0;
  }
}

int mublasSetStreamFromC(mublasHandle_t musaHandle, musaStream_t musaStream) {
  //mublasStatus status = mublasSetStream(*((mublasHandle_t*)handle), *((musaStream_t*)stream));
  mublasStatus status = mublasSetStream(musaHandle, musaStream);
  if (status == MUBLAS_STATUS_SUCCESS) {
    return 1;
  }
  else if (status == MUBLAS_STATUS_NOT_IMPLEMENTED) {
    errormessage("Error in mublasSetStream: %s\n", "the CUDA Runtime initialization failed");
    return 0;
  }
  else{
    errormessage("Error in mublasSetStream: %s\n", "unknown error");
    return 0;
  }
}

int musaMemcpy2dAsyncFromC(intptr_t *dest, size_t dpitch, intptr_t *src, size_t spitch, size_t width, size_t height, int dir, musaStream_t musaStream) {

  musaError_t muerr = musaMemcpy2DAsync( dest, dpitch, src, spitch, width, height, (musaMemcpyKind)dir, musaStream );
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemcpy2dAsync: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int mublasCreateFromC(mublasHandle_t *mublas_handle) {
  //*mublas_handle = (intptr_t) malloc(sizeof(mublasHandle_t));
  if (sizeof(intptr_t) != sizeof(mublasHandle_t)) {
    //errormessage("Error in mublasCreate: sizes not the same");
printf("ERROR on sizes\n");
    return 0;
  }
  mublasStatus status = mublasCreate(mublas_handle);
  if (status == MUBLAS_STATUS_SUCCESS) {
//       printf("all OK\n");
    return 1;
  }
  else if (status == MUBLAS_STATUS_NOT_IMPLEMENTED) {
    errormessage("Error in mublasCreate: %s\n", "the CUDA Runtime initialization failed");
    return 0;
  }
  else if (status == MUBLAS_STATUS_INTERNAL_ERROR) {
    errormessage("Error in mublasCreate: %s\n", "the resources could not be allocated");
    return 0;
  }
  else{
    errormessage("Error in mublasCreate: %s\n", "unknown error");
    return 0;
  }
}

int mublasDestroyFromC(mublasHandle_t mublas_handle) {
  mublasStatus status = mublasDestroy(mublas_handle);
  if (status == MUBLAS_STATUS_SUCCESS) {
//	 free((void*) *mublas_handle);
//       printf("all OK\n");
    return 1;
  }
  else if (status == MUBLAS_STATUS_NOT_IMPLEMENTED) {
    errormessage("Error in mublasDestroy: %s\n", "the library has not been initialized");
    return 0;
  }
  else{
    errormessage("Error in mublasDestroy: %s\n", "unknown error");
    return 0;
  }
}

int musaSetDeviceFromC(int n) {

  musaError_t muerr = musaSetDevice(n);
  if (muerr != musaSuccess) {
    errormessage("Error in musaSetDevice: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaGetDeviceCountFromC(int *count) {

  musaError_t muerr = musaGetDeviceCount(count);
  if (muerr != musaSuccess) {
    errormessage("Error in musaGetDeviceCount: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaDeviceSynchronizeFromC() {

  musaError_t muerr = musaDeviceSynchronize();
  if (muerr != musaSuccess) {
    errormessage("Error in musaDeviceSynchronize: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMallocFromC(intptr_t *a, size_t width_height) {

  musaError_t muerr = musaMalloc((void **) a, width_height);
#ifdef DEBUG_MUSA
  printf("CUDA Malloc,  pointer address: %p, size: %d \n", *a, width_height);
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaFreeFromC(intptr_t *a) {
#ifdef DEBUG_MUSA
  printf("CUDA Free, pointer address: %p \n", a);
#endif
  musaError_t muerr = musaFree(a);

  if (muerr != musaSuccess) {
    errormessage("Error in musaFree: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMallocHostFromC(intptr_t *a, size_t width_height) {

  musaError_t muerr = musaMallocHost((void **) a, width_height);
#ifdef DEBUG_MUSA
  printf("MallocHost pointer address: %p \n", *a);
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in musaMallocHost: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaFreeHostFromC(intptr_t *a) {
#ifdef DEBUG_MUSA
  printf("FreeHost pointer address: %p \n", a);
#endif
  musaError_t muerr = musaFreeHost(a);

  if (muerr != musaSuccess) {
    errormessage("Error in musaFreeHost: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemsetFromC(intptr_t *a, int value, size_t count) {

  musaError_t muerr = musaMemset( a, value, count);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemset: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemsetAsyncFromC(intptr_t *a, int value, size_t count, musaStream_t musaStream) {

  musaError_t muerr = musaMemsetAsync( a, value, count, musaStream);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemsetAsync: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemcpyFromC(intptr_t *dest, intptr_t *src, size_t count, int dir) {

  musaError_t muerr = musaMemcpy( dest, src, count, (musaMemcpyKind)dir);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemcpy: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemcpyAsyncFromC(intptr_t *dest, intptr_t *src, size_t count, int dir, musaStream_t musaStream) {

  musaError_t muerr = musaMemcpyAsync( dest, src, count, (musaMemcpyKind)dir, musaStream);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemcpyAsync: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemcpy2dFromC(intptr_t *dest, size_t dpitch, intptr_t *src, size_t spitch, size_t width, size_t height, int dir) {

  musaError_t muerr = musaMemcpy2D( dest, dpitch, src, spitch, width, height, (musaMemcpyKind)dir);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMemcpy2d: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaHostRegisterFromC(intptr_t *a, intptr_t value, int flag) {

  musaError_t muerr = musaHostRegister( a, value, flag);
  if (muerr != musaSuccess) {
    errormessage("Error in musaHostRegister: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaHostUnregisterFromC(intptr_t *a) {

  musaError_t muerr = musaHostUnregister( a);
  if (muerr != musaSuccess) {
    errormessage("Error in musaHostUnregister: %s\n",musaGetErrorString(muerr));
    return 0;
  }
  return 1;
}

int musaMemcpyDeviceToDeviceFromC(void) {
    int val = musaMemcpyDeviceToDevice;
    return val;
}
int musaMemcpyHostToDeviceFromC(void) {
    int val = musaMemcpyHostToDevice;
    return val;
}
int musaMemcpyDeviceToHostFromC(void) {
    int val = musaMemcpyDeviceToHost;
    return val;
}
int musaHostRegisterDefaultFromC(void) {
    int val = musaHostRegisterDefault;
    return val;
}
int musaHostRegisterPortableFromC(void) {
    int val = musaHostRegisterPortable;
    return val;
}
int musaHostRegisterMappedFromC(void) {
    int val = musaHostRegisterMapped;
    return val;
}

mublasOperation operation_new_api(char trans) {
  if (trans == 'N' || trans == 'n') {
    return MUBLAS_OP_N;
  }
  else if (trans == 'T' || trans == 't') {
    return MUBLAS_OP_T;
  }
  else if (trans == 'C' || trans == 'c') {
    return MUBLAS_OP_C;
  }
  else {
    errormessage("Error when transfering %c to mublasOperation\n",trans);
    // or abort?
    return MUBLAS_OP_N;
  }
}


mublasFillMode fill_mode_new_api(char uplo) {
  if (uplo == 'L' || uplo == 'l') {
    return MUBLAS_FILL_MODE_LOWER;
  }
  else if(uplo == 'U' || uplo == 'u') {
    return MUBLAS_FILL_MODE_UPPER;
  }
  else {
    errormessage("Error when transfering %c to mublasFillMode\n", uplo);
    // or abort?
    return MUBLAS_FILL_MODE_LOWER;
  }
}

mublasSideMode side_mode_new_api(char side) {
  if (side == 'L' || side == 'l') {
    return MUBLAS_SIDE_LEFT;
  }
  else if (side == 'R' || side == 'r') {
    return MUBLAS_SIDE_RIGHT;
  }
  else{
    errormessage("Error when transfering %c to mublasSideMode\n", side);
    // or abort?
    return MUBLAS_SIDE_LEFT;
  }
}

mublasDiagType diag_type_new_api(char diag) {
  if (diag == 'N' || diag == 'n') {
    return MUBLAS_DIAG_NON_UNIT;
  }
  else if (diag == 'U' || diag == 'u') {
    return MUBLAS_DIAG_UNIT;
  }
  else {
    errormessage("Error when transfering %c to mublasDiagType\n", diag);
    // or abort?
    return MUBLAS_DIAG_NON_UNIT;
  }
}

//_________________________________________________________________________________________________

void mublasDgemv_elpa_wrapper (mublasHandle_t musaHandle, char trans, int m, int n, double alpha,
                              const double *A, int lda,  const double *x, int incx,
                              double beta, double *y, int incy) {

  //mublasStatus status = mublasDgemv(*((mublasHandle_t*)handle), operation_new_api(trans),
  mublasStatus status = mublasDgemv(musaHandle, operation_new_api(trans),
                                      m, n, &alpha, A, lda, x, incx, &beta, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDgemv\n");
  }
}

void mublasSgemv_elpa_wrapper (mublasHandle_t musaHandle, char trans, int m, int n, float alpha,
                              const float *A, int lda,  const float *x, int incx,
                              float beta, float *y, int incy) {

  //mublasStatus status = mublasSgemv(*((mublasHandle_t*)handle), operation_new_api(trans),
  mublasStatus status = mublasSgemv(musaHandle, operation_new_api(trans),
              m, n, &alpha, A, lda, x, incx, &beta, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSgemv\n");
  }
}

void mublasZgemv_elpa_wrapper (mublasHandle_t musaHandle, char trans, int m, int n, double _Complex alpha,
                              const double _Complex *A, int lda,  const double _Complex *x, int incx,
                              double _Complex beta, double _Complex *y, int incy) {

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));
  muDoubleComplex beta_casted = *((muDoubleComplex*)(&beta));

  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  const muDoubleComplex* x_casted = (const muDoubleComplex*) x;
  muDoubleComplex* y_casted = (muDoubleComplex*) y;

  //mublasStatus status = mublasZgemv(*((mublasHandle_t*)handle), operation_new_api(trans),
  mublasStatus status = mublasZgemv(musaHandle, operation_new_api(trans),
              m, n, &alpha_casted, A_casted, lda, x_casted, incx, &beta_casted, y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZgemv\n");
  }
}

void mublasCgemv_elpa_wrapper (mublasHandle_t musaHandle, char trans, int m, int n, float _Complex alpha,
                              const float _Complex *A, int lda,  const float _Complex *x, int incx,
                              float _Complex beta, float _Complex *y, int incy) {

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));
  muFloatComplex beta_casted = *((muFloatComplex*)(&beta));

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  const muFloatComplex* x_casted = (const muFloatComplex*) x;
  muFloatComplex* y_casted = (muFloatComplex*) y;

  //mublasStatus status = mublasCgemv(*((mublasHandle_t*)handle), operation_new_api(trans),
  mublasStatus status = mublasCgemv(musaHandle, operation_new_api(trans),
              m, n, &alpha_casted, A_casted, lda, x_casted, incx, &beta_casted, y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCgemv\n");
  }
}

//_________________________________________________________________________________________________

void mublasDgemm_elpa_wrapper (mublasHandle_t musaHandle, char transa, char transb, int m, int n, int k,
                              double alpha, const double *A, int lda,
                              const double *B, int ldb, double beta,
                              double *C, int ldc) {

  //mublasStatus status = mublasDgemm(*((mublasHandle_t*)handle), operation_new_api(transa), operation_new_api(transb),
  mublasStatus status = mublasDgemm(musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha, A, lda, B, ldb, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDgemm\n");
  }
}


void mublasSgemm_elpa_wrapper (mublasHandle_t musaHandle, char transa, char transb, int m, int n, int k,
                              float alpha, const float *A, int lda,
                              const float *B, int ldb, float beta,
                              float *C, int ldc) {

  //mublasStatus status = mublasSgemm(((mublasHandle_t*)handle), operation_new_api(transa), operation_new_api(transb),
  mublasStatus status = mublasSgemm(musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha, A, lda, B, ldb, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSgemm\n");
  }
}

void mublasZgemm_elpa_wrapper (mublasHandle_t musaHandle, char transa, char transb, int m, int n, int k,
                              double _Complex alpha, const double _Complex *A, int lda,
                              const double _Complex *B, int ldb, double _Complex beta,
                              double _Complex *C, int ldc) {

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));
  muDoubleComplex beta_casted = *((muDoubleComplex*)(&beta));

  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  const muDoubleComplex* B_casted = (const muDoubleComplex*) B;
  muDoubleComplex* C_casted = (muDoubleComplex*) C;

  //mublasStatus status = mublasZgemm(*((mublasHandle_t*)handle), operation_new_api(transa), operation_new_api(transb),
  mublasStatus status = mublasZgemm(musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha_casted, A_casted, lda, B_casted, ldb, &beta_casted, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZgemm\n");
  }
}

void mublasCgemm_elpa_wrapper (mublasHandle_t musaHandle, char transa, char transb, int m, int n, int k,
                              float _Complex alpha, const float _Complex *A, int lda,
                              const float _Complex *B, int ldb, float _Complex beta,
                              float _Complex *C, int ldc) {

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));
  muFloatComplex beta_casted = *((muFloatComplex*)(&beta));

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  const muFloatComplex* B_casted = (const muFloatComplex*) B;
  muFloatComplex* C_casted = (muFloatComplex*) C;

  //mublasStatus status =  mublasCgemm(*((mublasHandle_t*)handle), operation_new_api(transa), operation_new_api(transb),
  mublasStatus status =  mublasCgemm(musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha_casted, A_casted, lda, B_casted, ldb, &beta_casted, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCgemm\n");
  }
}

//_________________________________________________________________________________________________

void mublasDgemm_elpa_wrapper_intptr_handle (intptr_t* musaHandle, char transa, char transb, int m, int n, int k,
                              double alpha, const double *A, int lda,
                              const double *B, int ldb, double beta,
                              double *C, int ldc) {

  mublasStatus status = mublasDgemm((mublasHandle_t) *musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha, A, lda, B, ldb, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDgemm\n");
  }
}


void mublasSgemm_elpa_wrapper_intptr_handle (intptr_t* musaHandle, char transa, char transb, int m, int n, int k,
                              float alpha, const float *A, int lda,
                              const float *B, int ldb, float beta,
                              float *C, int ldc) {

  mublasStatus status = mublasSgemm((mublasHandle_t) *musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha, A, lda, B, ldb, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSgemm\n");
  }
}

void mublasZgemm_elpa_wrapper_intptr_handle (intptr_t* musaHandle, char transa, char transb, int m, int n, int k,
                              double _Complex alpha, const double _Complex *A, int lda,
                              const double _Complex *B, int ldb, double _Complex beta,
                              double _Complex *C, int ldc) {

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));
  muDoubleComplex beta_casted = *((muDoubleComplex*)(&beta));

  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  const muDoubleComplex* B_casted = (const muDoubleComplex*) B;
  muDoubleComplex* C_casted = (muDoubleComplex*) C;

  mublasStatus status = mublasZgemm((mublasHandle_t) *musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha_casted, A_casted, lda, B_casted, ldb, &beta_casted, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZgemm\n");
  }
}

void mublasCgemm_elpa_wrapper_intptr_handle (intptr_t* musaHandle, char transa, char transb, int m, int n, int k,
                              float _Complex alpha, const float _Complex *A, int lda,
                              const float _Complex *B, int ldb, float _Complex beta,
                              float _Complex *C, int ldc) {

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));
  muFloatComplex beta_casted = *((muFloatComplex*)(&beta));

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  const muFloatComplex* B_casted = (const muFloatComplex*) B;
  muFloatComplex* C_casted = (muFloatComplex*) C;

  mublasStatus status =  mublasCgemm((mublasHandle_t) *musaHandle, operation_new_api(transa), operation_new_api(transb),
              m, n, k, &alpha_casted, A_casted, lda, B_casted, ldb, &beta_casted, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCgemm\n");
  }
}

// todo: new CUBLAS API diverged from standard BLAS api for these functions
// todo: it provides out-of-place (and apparently more efficient) implementation
// todo: by passing B twice (in place of C as well), we should fall back to in-place algorithm

//_________________________________________________________________________________________________

void mublasDcopy_elpa_wrapper (mublasHandle_t musaHandle, int n, double *x, int incx, double *y, int incy){

  //mublasStatus status = mublasDcopy(*((mublasHandle_t*)handle), n, x, incx, y, incy);
  mublasStatus status = mublasDcopy(musaHandle, n, x, incx, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDcopy\n");
  }
}

void mublasScopy_elpa_wrapper (mublasHandle_t musaHandle, int n, float *x, int incx, float *y, int incy){

  //mublasStatus status = mublasScopy(*((mublasHandle_t*)handle), n, x, incx, y, incy);
  mublasStatus status = mublasScopy(musaHandle, n, x, incx, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasScopy\n");
  }
}

void mublasZcopy_elpa_wrapper (mublasHandle_t musaHandle, int n, double _Complex *x, int incx, double _Complex *y, int incy){
  const muDoubleComplex* X_casted = (const muDoubleComplex*) x;
        muDoubleComplex* Y_casted = (      muDoubleComplex*) y;

  //mublasStatus status = mublasZcopy(*((mublasHandle_t*)handle), n, X_casted, incx, Y_casted, incy);
  mublasStatus status = mublasZcopy(musaHandle, n, X_casted, incx, Y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZcopy\n");
  }
}

void mublasCcopy_elpa_wrapper (mublasHandle_t musaHandle, int n, float _Complex *x, int incx, float _Complex *y, int incy){
  const muFloatComplex* X_casted = (const muFloatComplex*) x;
        muFloatComplex* Y_casted = (      muFloatComplex*) y;

  //mublasStatus status = mublasCcopy(handle, n, X_casted, incx, Y_casted, incy);
  mublasStatus status = mublasCcopy(musaHandle, n, X_casted, incx, Y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCcopy\n");
  }
}

//_________________________________________________________________________________________________

void mublasDtrsm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, double alpha, const double *A,
                              int lda, double *B, int ldb){

  mublasStatus status = mublasDtrsm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
                                      diag_type_new_api(diag), m, n, &alpha, A, lda, B, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDtrsm\n");
  }
}

void mublasStrsm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, float alpha, const float *A,
                              int lda, float *B, int ldb){

  mublasStatus status = mublasStrsm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
                                      diag_type_new_api(diag), m, n, &alpha, A, lda, B, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasStrsm\n");
  }
}

void mublasZtrsm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, double _Complex alpha, const double _Complex *A,
                              int lda, double _Complex *B, int ldb){

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));

  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  muDoubleComplex* B_casted = (muDoubleComplex*) B;

  mublasStatus status = mublasZtrsm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha_casted, A_casted, lda, B_casted, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZtrsm\n");
  }
}

void mublasCtrsm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, float _Complex alpha, const float _Complex *A,
                              int lda, float _Complex *B, int ldb){

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  muFloatComplex* B_casted = (muFloatComplex*) B;

  mublasStatus status = mublasCtrsm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha_casted, A_casted, lda, B_casted, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCtrsm\n");
  }
}

//_________________________________________________________________________________________________

void mublasDtrmm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, double alpha, const double *A,
                              int lda, double *B, int ldb){

  //mublasStatus status = mublasDtrmm(*((mublasHandle_t*)handle), side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
  mublasStatus status = mublasDtrmm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha, A, lda, B, ldb, B, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDtrmm\n");
  }
}

void mublasStrmm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, float alpha, const float *A,
                              int lda, float *B, int ldb){

  //mublasStatus status = mublasStrmm(*((mublasHandle_t*)handle), side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
  mublasStatus status = mublasStrmm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha, A, lda, B, ldb, B, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasStrmm\n");
  }
}

void mublasZtrmm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, double _Complex alpha, const double _Complex *A,
                              int lda, double _Complex *B, int ldb){

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));

  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  muDoubleComplex* B_casted = (muDoubleComplex*) B;

  //mublasStatus status = mublasZtrmm(*((mublasHandle_t*)handle), side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
  mublasStatus status = mublasZtrmm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha_casted, A_casted, lda, B_casted, ldb, B_casted, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZtrmm\n");
  }
}

void mublasCtrmm_elpa_wrapper (mublasHandle_t musaHandle, char side, char uplo, char transa, char diag,
                              int m, int n, float _Complex alpha, const float _Complex *A,
                              int lda, float _Complex *B, int ldb){

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  muFloatComplex* B_casted = (muFloatComplex*) B;

  //mublasStatus status = mublasCtrmm(*((mublasHandle_t*)handle), side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
  mublasStatus status = mublasCtrmm(musaHandle, side_mode_new_api(side), fill_mode_new_api(uplo), operation_new_api(transa),
              diag_type_new_api(diag), m, n, &alpha_casted, A_casted, lda, B_casted, ldb, B_casted, ldb);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCtrmm\n");
  }
}

//_________________________________________________________________________________________________

void mublasDtrmv_elpa_wrapper(mublasHandle_t mublasHandle, char uplo, char trans, char diag,
                              int n, const double *A, int lda, double *x, int incx){

  mublasStatus status = mublasDtrmv(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans), diag_type_new_api(diag),
                                      n, A, lda, x, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
    printf("error when calling mublasDtrmv\n");
  }
}

void mublasStrmv_elpa_wrapper(mublasHandle_t mublasHandle, char uplo, char trans, char diag,
                              int n, const float *A, int lda, float *x, int incx){

  mublasStatus status = mublasStrmv(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans), diag_type_new_api(diag),
                                      n, A, lda, x, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
    printf("error when calling mublasStrmv\n");
  }
}

void mublasZtrmv_elpa_wrapper(mublasHandle_t mublasHandle, char uplo,  char trans, char diag,
                              int n, const double _Complex *A, int lda, double _Complex *x, int incx){

  muDoubleComplex* A_casted = (muDoubleComplex*) A;
  muDoubleComplex* x_casted = (muDoubleComplex*) x;

  mublasStatus status = mublasZtrmv(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans), diag_type_new_api(diag),
                                      n, A_casted, lda, x_casted, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
    printf("error when calling mublasZtrmv\n");
  }
}

void mublasCtrmv_elpa_wrapper(mublasHandle_t mublasHandle, char uplo,  char trans, char diag,
                              int n, const float _Complex *A, int lda, float _Complex *x, int incx){

  muFloatComplex* A_casted = (muFloatComplex*) A;
  muFloatComplex* x_casted = (muFloatComplex*) x;

  mublasStatus status = mublasCtrmv(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans), diag_type_new_api(diag),
                                      n, A_casted, lda, x_casted, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
    printf("error when calling mublasCtrmv\n");
  }
}

//_________________________________________________________________________________________________

void mublasDsyrk_elpa_wrapper(mublasHandle_t mublasHandle, char uplo, char trans, 
                              int n, int k, 
                              double alpha, const double *A, int lda,
                              double beta, double *C, int ldc){
  
  mublasStatus status = mublasDsyrk(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans),
                                        n, k, &alpha, A, lda, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDsyrk\n");
  }
}

void mublasSsyrk_elpa_wrapper(mublasHandle_t mublasHandle, char uplo, char trans, 
                              int n, int k, 
                              float alpha, const float *A, int lda,
                              float beta, float *C, int ldc){

  mublasStatus status = mublasSsyrk(mublasHandle, fill_mode_new_api(uplo), operation_new_api(trans),
                                      n, k, &alpha, A, lda, &beta, C, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSsyrk\n");
  }
}

void mublasZherk_elpa_wrapper(mublasHandle_t musaHandle, char uplo, char trans, 
                              int n, int k, 
                              double _Complex alpha, const double _Complex *A, int lda,
                              double _Complex beta, double _Complex *C, int ldc){

  double alpha_real = __real__(alpha);
  double beta_real  = __real__(beta);
  
  const muDoubleComplex* A_casted = (const muDoubleComplex*) A;
  muDoubleComplex* C_casted = (muDoubleComplex*) C;

  mublasStatus status = mublasZherk(musaHandle, fill_mode_new_api(uplo), operation_new_api(trans),
                                      n, k, &alpha_real, A_casted, lda, &beta_real, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZherk\n");
  }
}

void mublasCherk_elpa_wrapper(mublasHandle_t musaHandle, char uplo, char trans, 
                              int n, int k, 
                              float _Complex alpha, const float _Complex *A, int lda,
                              float _Complex beta, float _Complex *C, int ldc){

  float alpha_real = __real__(alpha);
  float beta_real  = __real__(beta);

  const muFloatComplex* A_casted = (const muFloatComplex*) A;
  muFloatComplex* C_casted = (muFloatComplex*) C;

  mublasStatus status = mublasCherk(musaHandle, fill_mode_new_api(uplo), operation_new_api(trans),
                                      n, k, &alpha_real, A_casted, lda, &beta_real, C_casted, ldc);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCherk\n");
  }
}

//_________________________________________________________________________________________________

// result can be on host or device depending on pointer mode
void mublasDdot_elpa_wrapper (mublasHandle_t musaHandle, int length, const double *X, int incx, const double *Y, int incy, double *result) {
  mublasStatus status = mublasDdot(musaHandle, length, X, incx, Y, incy, result);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDdot\n");
  }
}

void mublasSdot_elpa_wrapper (mublasHandle_t musaHandle, int length, const float *X, int incx, const float *Y, int incy, float *result) {

  //mublasSetPointerMode(musaHandle, MUBLAS_POINTER_MODE_DEVICE);
  mublasStatus status = mublasSdot(musaHandle, length, X, incx, Y, incy, result);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSdot\n");
  }
}

void mublasZdot_elpa_wrapper (char conju, mublasHandle_t musaHandle, int length, const double _Complex *X, int incx, const double _Complex *Y, int incy, double _Complex *result) {

  //mublasSetPointerMode(musaHandle, MUBLAS_POINTER_MODE_DEVICE);
  const muDoubleComplex* X_casted = (const muDoubleComplex*) X;
  const muDoubleComplex* Y_casted = (const muDoubleComplex*) Y;
        muDoubleComplex* result_casted = (muDoubleComplex*) result;
  mublasStatus status;
  if (conju == 'C' || conju == 'c') {
    status = mublasZdotc(musaHandle, length, X_casted, incx, Y_casted, incy, result_casted);
  }
  if (conju == 'U' || conju == 'u') {
    printf("not using conjugate in dot\n");
    status = mublasZdotu(musaHandle, length, X_casted, incx, Y_casted, incy, result_casted);
  }

  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZdot\n");
  }
  //muDoubleComplex* result = (muDoubleComplex*) result_casted;
}

void mublasCdot_elpa_wrapper (char conju, mublasHandle_t musaHandle, int length, const float _Complex *X, int incx, const float _Complex *Y, int incy, float _Complex *result) {

  //mublasSetPointerMode(musaHandle, MUBLAS_POINTER_MODE_DEVICE);
  const muFloatComplex* X_casted = (const muFloatComplex*) X;
  const muFloatComplex* Y_casted = (const muFloatComplex*) Y;
        muFloatComplex* result_casted = (muFloatComplex*) result;

  mublasStatus status;
  if (conju == 'C' || conju == 'c') {
    status = mublasCdotc(musaHandle, length, X_casted, incx, Y_casted, incy, result_casted);
  }
  if (conju == 'U' || conju == 'u') {
    status = mublasCdotu(musaHandle, length, X_casted, incx, Y_casted, incy, result_casted);
  }
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCdot\n");
  }
  printf("Leaving setPointer\n");
}

void mublasSetPointerModeFromC(mublasHandle_t musaHandle, mublasPointerMode mode) {
  mublasSetPointerMode(musaHandle, mode);
}

void mublasGetPointerModeFromC(mublasHandle_t musaHandle, mublasPointerMode *mode) {
  //mublasPointerMode mode_tmp;
  //mublasGetPointerMode(musaHandle, &mode_tmp);
  mublasGetPointerMode(musaHandle, mode);
  //printf("in getpointer mode %d \n",mode_tmp);
  //printf("in getpointer mode %d \n",*mode);
  //*mode = mode_tmp;
}

int mublasPointerModeDeviceFromC(void) {
    int val = MUBLAS_POINTER_MODE_DEVICE;
    return val;
}

int mublasPointerModeHostFromC(void) {
    int val = MUBLAS_POINTER_MODE_HOST;
    return val;
}

void mublasDscal_elpa_wrapper (mublasHandle_t musaHandle, int n, double alpha, double *x, int incx){

  mublasStatus status = mublasDscal(musaHandle, n, &alpha, x, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDscal\n");
  }
}

void mublasSscal_elpa_wrapper (mublasHandle_t musaHandle, int n, float alpha, float *x, int incx){

  mublasStatus status = mublasSscal(musaHandle, n, &alpha, x, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSscal\n");
  }
}

void mublasZscal_elpa_wrapper (mublasHandle_t musaHandle, int n, double _Complex alpha, double _Complex *x, int incx){
  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));
  muDoubleComplex* X_casted     = (muDoubleComplex*) x;

  mublasStatus status = mublasZscal(musaHandle, n, &alpha_casted, X_casted, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZscal\n");
  }
}

void mublasCscal_elpa_wrapper (mublasHandle_t musaHandle, int n, float _Complex alpha, float _Complex *x, int incx){
  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));
  muFloatComplex* X_casted     = (muFloatComplex*) x;

  mublasStatus status = mublasCscal(musaHandle, n, &alpha_casted, X_casted, incx);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCscal\n");
  }
}

void mublasDaxpy_elpa_wrapper (mublasHandle_t musaHandle, int n, double alpha, double *x, int incx, double *y, int incy){

  mublasStatus status = mublasDaxpy(musaHandle, n, &alpha, x, incx, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasDaxpy\n");
  }
}

void mublasSaxpy_elpa_wrapper (mublasHandle_t musaHandle, int n, float alpha, float *x, int incx, float *y, int incy){

  mublasStatus status = mublasSaxpy(musaHandle, n, &alpha, x, incx, y, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasSaxpy\n");
  }
}

void mublasZaxpy_elpa_wrapper (mublasHandle_t musaHandle, int n, double _Complex alpha, double _Complex *x, int incx, double _Complex *y, int incy){

  muDoubleComplex alpha_casted = *((muDoubleComplex*)(&alpha));
  muDoubleComplex* X_casted     = (muDoubleComplex*) x;
  muDoubleComplex* Y_casted     = (muDoubleComplex*) y;

  mublasStatus status = mublasZaxpy(musaHandle, n, &alpha_casted, X_casted, incx, Y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasZaxpy\n");
  }
}

void mublasCaxpy_elpa_wrapper (mublasHandle_t musaHandle, int n, float _Complex alpha, float _Complex *x, int incx, float _Complex *y, int incy){

  muFloatComplex alpha_casted = *((muFloatComplex*)(&alpha));
  muFloatComplex* X_casted     = (muFloatComplex*) x;
  muFloatComplex* Y_casted     = (muFloatComplex*) y;

  mublasStatus status = mublasCaxpy(musaHandle, n, &alpha_casted, X_casted, incx, Y_casted, incy);
  if (status != MUBLAS_STATUS_SUCCESS) {
      printf("error when calling mublasCaxpy\n");
  }
}

#ifdef __cplusplus
}    
#endif
