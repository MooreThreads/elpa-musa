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
// This file was written by P. Karpov, MPCDF
#endif


extern "C" {

cudaDataType getCudaDataType(char dataType) {
  if      (dataType=='D') return CUDA_R_64F;
  else if (dataType=='S') return CUDA_R_32F;
  else if (dataType=='Z') return CUDA_C_64F;
  else if (dataType=='C') return CUDA_C_32F;
  else {
    errormessage("Error in getCudaDataType: unknown data type, %s\n", "aborting");
    return CUDA_R_64F;
  }
}

void elpa_cusolverPrintError(musolverStatus_t status){
  switch (status){
    case MUSOLVER_STATUS_SUCCESS:
        printf("cusolverStatus=MUSOLVER_STATUS_SUCCESS\n");
        break;
    case CUSOLVER_STATUS_NOT_INITIALIZED:
        printf("cusolverStatus=CUSOLVER_STATUS_NOT_INITIALIZED\n");
        break;
    case CUSOLVER_STATUS_ALLOC_FAILED:
        printf("cusolverStatus=CUSOLVER_STATUS_ALLOC_FAILED\n");
        break;
    case CUSOLVER_STATUS_INVALID_VALUE:
        printf("cusolverStatus=CUSOLVER_STATUS_INVALID_VALUE\n");
        break;
    case CUSOLVER_STATUS_ARCH_MISMATCH:
        printf("cusolverStatus=CUSOLVER_STATUS_ARCH_MISMATCH\n");
        break;
    case CUSOLVER_STATUS_MAPPING_ERROR:
        printf("cusolverStatus=CUSOLVER_STATUS_MAPPING_ERROR\n");
        break;
    case CUSOLVER_STATUS_EXECUTION_FAILED:
        printf("cusolverStatus=CUSOLVER_STATUS_EXECUTION_FAILED\n");
        break;
    case CUSOLVER_STATUS_INTERNAL_ERROR:
        printf("cusolverStatus=CUSOLVER_STATUS_INTERNAL_ERROR\n");
        break;
    case CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
        printf("cusolverStatus=CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED\n");
        break;
    case CUSOLVER_STATUS_NOT_SUPPORTED:
        printf("cusolverStatus=CUSOLVER_STATUS_NOT_SUPPORTED\n");
        break;
    default:
        printf("Unknown cusolverStatus status: %d\n", status);
  }
}

int cusolverGetVersionFromC() {

  int major=0, minor=0, patch=0;
  cusolverGetProperty(MAJOR_VERSION, &major);
  cusolverGetProperty(MINOR_VERSION, &minor);
  cusolverGetProperty(PATCH_LEVEL,   &patch);

  return major*10000 + minor*100 + patch;
}

int cusolverSetStreamFromC(musolverDnHandle_t cusolver_handle, musaStream_t stream) {
  //musolverStatus_t status = musolverDnSetStream(*((musolverDnHandle_t*)cusolver_handle), *((musaStream_t*)stream));
  musolverStatus_t status = musolverDnSetStream(cusolver_handle, stream);
  if (status == MUSOLVER_STATUS_SUCCESS) {
    return 1;
  }
  else if (status == CUSOLVER_STATUS_NOT_INITIALIZED) {
    errormessage("Error in musolverDnSetStream: %s\n", "the CUDA Runtime initialization failed");
    return 0;
  }
  else{
    errormessage("Error in musolverDnSetStream: %s\n", "unknown error");
    return 0;
  }
}


int cusolverCreateFromC(musolverDnHandle_t *cusolver_handle) {
  //*cusolver_handle = (intptr_t) malloc(sizeof(musolverDnHandle_t));
  //musolverStatus_t status = musolverDnCreate((musolverDnHandle_t*) *cusolver_handle);
  if (sizeof(intptr_t) != sizeof(musolverDnHandle_t)) {
    printf("cusolver sizes wrong\n");
  }
  musolverStatus_t status = musolverDnCreate(cusolver_handle);
  if (status == MUSOLVER_STATUS_SUCCESS) {
//       printf("all OK\n");
    return 1;
  }
  else if (status == CUSOLVER_STATUS_NOT_INITIALIZED) {
    errormessage("Error in cusolverCreate: %s\n", "the CUDA Runtime initialization failed");
    return 0;
  }
  else if (status == CUSOLVER_STATUS_ALLOC_FAILED) {
    errormessage("Error in cusolverCreate: %s\n", "the resources could not be allocated");
    return 0;
  }
  else{
    errormessage("Error in cusolverCreate: %s\n", "unknown error");
    return 0;
  }
}


int cusolverDestroyFromC(musolverDnHandle_t cusolver_handle) {
  //musolverStatus_t status = musolverDnDestroy(*((musolverDnHandle_t*) *cusolver_handle));
  musolverStatus_t status = musolverDnDestroy(cusolver_handle);
  if (status == MUSOLVER_STATUS_SUCCESS) {
//       printf("all OK\n");
    //free((void*) *cusolver_handle);
    return 1;
  }
  else if (status == CUSOLVER_STATUS_NOT_INITIALIZED) {
    errormessage("Error in cusolverDestroy: %s\n", "the library has not been initialized");
    return 0;
  }
  else{
    errormessage("Error in cusolverDestroy: %s\n", "unknown error");
    return 0;
  }
}

//_________________________________________________________________________________________________
// cusolver?trtri

void cusolverDtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, double *A, int64_t lda, int *info) {
  musolverStatus_t status;

  int info_gpu = 0;

  int *devInfo = NULL; 
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dtrtri devInfo: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverDtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)devInfo, sizeof(int));
#endif

  double *d_work = NULL, *h_work=NULL;
  size_t d_lwork = 0;
  size_t h_lwork = 0;
  //status = cusolverDnXtrtri_bufferSize(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_64F, A, lda, &d_lwork, &h_lwork);
  status = cusolverDnXtrtri_bufferSize(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_64F, A, lda, &d_lwork, &h_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnDtrtri_buffer_size %s \n","aborting");
  }

  if (h_lwork != 0) {
    errormessage("Error in cusolver_Dtrtri host work array needed of size=: %d\n",h_lwork);
  }

#if CUSOLVER_VERSION < 11601
  // temporary workaround for cusolverDnXtrtri_bufferSize bug
  // https://docs.nvidia.com/cuda/archive/12.4.0/cuda-toolkit-release-notes/index.html#cusolver-release-12-4
  d_lwork *= 8;

  // the problem is fixed in CUDA 12.4.1 (cuSOLVER 11.6.1.9)
#endif

  //muerr = musaMalloc((void**) &d_work, sizeof(double) * d_lwork);
  muerr = musaMalloc((void**) &d_work, d_lwork); // d_lwork already in bytes
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dtrtri d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverDtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, d_lwork);
#endif

  //status = cusolverDnXtrtri(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_64F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);
  status = cusolverDnXtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_64F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  //muerr = musaDeviceSynchronize();
  //if (muerr != musaSuccess) {
  //  errormessage("Error in cusolver_Dtrtri: musaDeviceSynchronize: %s\n",musaGetErrorString(muerr));
  //}

  muerr = musaMemcpy(&info_gpu, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dtrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }

  *info = info_gpu;
  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dtrtri cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dtrtri cuda_free(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverStrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, float *A, int64_t lda, int *info) {
  musolverStatus_t status;

  int info_gpu = 0;

  int *devInfo = NULL; 
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverStrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)devInfo, sizeof(int));
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Strtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  float *d_work = NULL, *h_work=NULL;
  size_t d_lwork = 0;
  size_t h_lwork = 0;

  //status = cusolverDnXtrtri_bufferSize(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_32F, A, lda, &d_lwork, &h_lwork);
  status = cusolverDnXtrtri_bufferSize(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_32F, A, lda, &d_lwork, &h_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnStrtri_buffer_size %s \n","aborting");
  }

  if (h_lwork != 0) {
    errormessage("Error in cusolver_Strtri host work array needed of size=: %d\n",h_lwork);
  }

#if CUSOLVER_VERSION < 11601
  d_lwork *= 4;
#endif

  //muerr = musaMalloc((void**) &d_work, sizeof(float) * d_lwork);
  muerr = musaMalloc((void**) &d_work, d_lwork); // d_lwork already in bytes
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverStrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, d_lwork);
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Strtri d_work: %s\n",musaGetErrorString(muerr));
  }

  //status = cusolverDnXtrtri(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_32F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);
  status = cusolverDnXtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_R_32F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  //muerr = musaDeviceSynchronize();
  //if (muerr != musaSuccess) {
  //  errormessage("Error in cusolver_Strtri: musaDeviceSynchronize: %s\n",musaGetErrorString(muerr));
  //}

  muerr = musaMemcpy(&info_gpu, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Strtri info_gpu: %s\n",musaGetErrorString(muerr));
  }

  *info = info_gpu;
  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Strtri cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Strtri cuda_free(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverZtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, double _Complex *A, int64_t lda, int *info) {
  musolverStatus_t status;

  int info_gpu = 0;

  int *devInfo = NULL; 
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverZtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)devInfo, sizeof(int));
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ztrtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  //muDoubleComplex A_casted = *((muDoubleComplex*)(A));
  double _Complex *d_work = NULL, *h_work=NULL;
  size_t d_lwork = 0;
  size_t h_lwork = 0;

  //status = cusolverDnXtrtri_bufferSize(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_64F, A, lda, &d_lwork, &h_lwork);
  status = cusolverDnXtrtri_bufferSize(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_64F, A, lda, &d_lwork, &h_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnZtrtri_buffer_size %s \n","aborting");
  }

  if (h_lwork != 0) {
    errormessage("Error in cusolver_Ztrtri host work array needed of size=: %d\n",h_lwork);
  }

#if CUSOLVER_VERSION < 11601
  d_lwork *= 16;
#endif

  //muerr = musaMalloc((void**) &d_work, sizeof(double _Complex) * d_lwork);
  muerr = musaMalloc((void**) &d_work, d_lwork); // d_lwork in bytes
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverZtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, d_lwork);
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ztrtri d_work: %s\n",musaGetErrorString(muerr));
  }

  //status = cusolverDnXtrtri(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_64F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);
  status = cusolverDnXtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_64F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  //muerr = musaDeviceSynchronize();
  //if (muerr != musaSuccess) {
  //  errormessage("Error in cusolver_Ztrtri: musaDeviceSynchronize: %s\n",musaGetErrorString(muerr));
  //}

  muerr = musaMemcpy(&info_gpu, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ztrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }

  *info = info_gpu;
  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ztrtri cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ztrtri cuda_free(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverCtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, float _Complex *A, int64_t lda, int *info) {
  musolverStatus_t status;

  int info_gpu = 0;

  int *devInfo = NULL; 
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverCtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)devInfo, sizeof(int));
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ctrtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  //muFloatComplex A_casted = *((muFloatComplex*)(A));
  float _Complex *d_work = NULL, *h_work=NULL;
  size_t d_lwork = 0;
  size_t h_lwork = 0;

  //status = cusolverDnXtrtri_bufferSize(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_32F, A, lda, &d_lwork, &h_lwork);
  status = cusolverDnXtrtri_bufferSize(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_32F, A, lda, &d_lwork, &h_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnCtrtri_buffer_size %s \n","aborting");
  }

  if (h_lwork != 0) {
    errormessage("Error in cusolver_Ctrtri host work array needed of size=: %d\n",h_lwork);
  }

#if CUSOLVER_VERSION < 11601
  d_lwork *= 8;
#endif

  //muerr = musaMalloc((void**) &d_work, sizeof(float _Complex) * d_lwork);
  muerr = musaMalloc((void**) &d_work, d_lwork); // d_lwork already in bytes
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverCtrtri_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, d_lwork);
#endif
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ctrtri d_work: %s\n",musaGetErrorString(muerr));
  }

  //status = cusolverDnXtrtri(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_32F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);
  status = cusolverDnXtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), n, CUDA_C_32F, A, lda, d_work, d_lwork, h_work, h_lwork, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  //muerr = musaDeviceSynchronize();
  //if (muerr != musaSuccess) {
  //  errormessage("Error in cusolver_Ctrtri: musaDeviceSynchronize: %s\n",musaGetErrorString(muerr));
  //}

  muerr = musaMemcpy(&info_gpu, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ctrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }

  *info = info_gpu;
  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ctrtri cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Ctrtri cuda_free(devInfo): %s\n",musaGetErrorString(muerr));
  }
}

//_________________________________________________________________________________________________
// cusolver?potrf - deprecated

void cusolverDpotrf_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, int n, double *A, int lda, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  double *d_work = NULL;
  int d_lwork = 0;

  //status = musolverDnDpotrf_bufferSize(*((musolverDnHandle_t*)handle), fill_mode_new_api(uplo),  n, A, lda, &d_lwork);
  status = musolverDnDpotrf_bufferSize(musaHandle, fill_mode_new_api(uplo),  n, A, lda, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnDpotrf_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(double) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dpotrf d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverDpotrf_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work,  sizeof(double)*d_lwork);
#endif

  status = musolverDnDpotrf(musaHandle, fill_mode_new_api(uplo), n, A, lda, d_work, d_lwork, info_dev);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);


  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Dpotrf cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}

void cusolverSpotrf_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, int n, float *A, int lda, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  float *d_work = NULL;
  int d_lwork = 0;

  status = musolverDnSpotrf_bufferSize(musaHandle, fill_mode_new_api(uplo),  n, A, lda, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnSpotrf_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(float) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Spotrf d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverSpotrf_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, sizeof(float)*d_lwork);
#endif

  status = musolverDnSpotrf(musaHandle, fill_mode_new_api(uplo), n, A, lda, d_work, d_lwork, info_dev);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Spotrf cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}

void cusolverZpotrf_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, int n, double _Complex *A, int lda, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  muDoubleComplex *d_work = NULL;
  int d_lwork = 0;
  muDoubleComplex* A_casted = (muDoubleComplex*) A;

  status = musolverDnZpotrf_bufferSize(musaHandle, fill_mode_new_api(uplo),  n, A_casted, lda, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnZpotrf_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(muDoubleComplex) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Zpotrf d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverZpotrf_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, sizeof(muDoubleComplex)*d_lwork);
#endif

  status = musolverDnZpotrf(musaHandle, fill_mode_new_api(uplo), n, A_casted, lda, d_work, d_lwork, info_dev);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Zpotrf cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}

void cusolverCpotrf_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, int n, float _Complex *A, int lda, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  muFloatComplex *d_work = NULL;
  int d_lwork = 0;
  muFloatComplex* A_casted = (muFloatComplex*) A;

  status = musolverDnCpotrf_bufferSize(musaHandle, fill_mode_new_api(uplo),  n, A_casted, lda, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnCpotrf_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(muFloatComplex) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Cpotrf d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverCpotrf_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, sizeof(muFloatComplex)*d_lwork);
#endif

  status = musolverDnCpotrf(musaHandle, fill_mode_new_api(uplo), n, A_casted, lda, d_work, d_lwork, info_dev);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_Cpotrf cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}


//_________________________________________________________________________________________________
// cusolverXpotrf

// Introduced with CUDA 11.1 (CUDA_VERSION >= 11010)

void cusolverXpotrf_bufferSize_elpa_wrapper(musolverDnHandle_t cusolverHandle, char uplo, int n, char dataType, intptr_t A, int lda, 
                                          size_t *workspaceInBytesOnDevice, size_t *workspaceInBytesOnHost){

  musolverStatus_t status;
  cudaDataType cuda_data_type =  getCudaDataType(dataType);

  status = cusolverDnXpotrf_bufferSize(cusolverHandle, NULL, fill_mode_new_api(uplo), (int64_t) n, cuda_data_type, (void *) A, (int64_t) lda, 
                                        cuda_data_type, workspaceInBytesOnDevice, workspaceInBytesOnHost);

  if (status != MUSOLVER_STATUS_SUCCESS){
    elpa_cusolverPrintError(status);
    errormessage("Error in cusolverDnXpotrf_bufferSize %s \n", "aborting");
  }
}


void cusolverXpotrf_elpa_wrapper(musolverDnHandle_t cusolverHandle, char uplo, int n, char dataType, intptr_t A, int lda, 
                                intptr_t buffer_dev , size_t *workspaceInBytesOnDevice, 
                                intptr_t buffer_host, size_t *workspaceInBytesOnHost, int *info_dev){

  musolverStatus_t status;
  cudaDataType cuda_data_type =  getCudaDataType(dataType);

  status = cusolverDnXpotrf(cusolverHandle, NULL, fill_mode_new_api(uplo), (int64_t) n, cuda_data_type, (void *) A, (int64_t) lda, cuda_data_type,
                            (void *) buffer_dev , *workspaceInBytesOnDevice,
                            (void *) buffer_host, *workspaceInBytesOnHost, info_dev);
  
  if (status != MUSOLVER_STATUS_SUCCESS){
    elpa_cusolverPrintError(status);
    errormessage("Error in cusolverDnXpotrf %s \n", "aborting");
  }
}


//_________________________________________________________________________________________________
// cusolverXsyevd

void cusolverDsyevd_elpa_wrapper (musolverDnHandle_t musaHandle, int n, double *A, int lda, double *eigenvalues, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  double *d_work = NULL;
  int d_lwork = 0;

  cusolverEigMode_t jobz = CUSOLVER_EIG_MODE_VECTOR; // compute eigenvalues and eigenvectors.
  cublasFillMode_t uplo = MUBLAS_FILL_MODE_LOWER;

  status = cusolverDnDsyevd_bufferSize(musaHandle, jobz,  uplo, n, A, lda, eigenvalues, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnSsyevd_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(double) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverDsyevd_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, sizeof(double)*d_lwork);
#endif

  NVTX_RANGE_PUSH("cusolverDnDsyevd");
  status = cusolverDnDsyevd(musaHandle, jobz, uplo, n, A, lda, eigenvalues, d_work, d_lwork, info_dev);
  NVTX_RANGE_POP("cusolverDnDsyevd");

  if (status != MUSOLVER_STATUS_SUCCESS) elpa_cusolverPrintError(status);

  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolverDnDsyevd cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}

void cusolverSsyevd_elpa_wrapper (musolverDnHandle_t musaHandle, int n, float *A, int lda, float *eigenvalues, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  float *d_work = NULL;
  int d_lwork = 0;

  cusolverEigMode_t jobz = CUSOLVER_EIG_MODE_VECTOR; // compute eigenvalues and eigenvectors.
  cublasFillMode_t uplo = MUBLAS_FILL_MODE_LOWER;

  status = cusolverDnSsyevd_bufferSize(musaHandle, jobz,  uplo, n, A, lda, eigenvalues, &d_lwork);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in cusolverDnSsyevd_buffer_size %s \n","aborting");
  }

  muerr = musaMalloc((void**) &d_work, sizeof(float) * d_lwork);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc d_work: %s\n",musaGetErrorString(muerr));
  }
#ifdef DEBUG_MUSA
  printf("CUDA Malloc, cusolverSsyevd_elpa_wrapper, pointer address: %p, size: %d \n", (void*)d_work, sizeof(float)*d_lwork);
#endif

  status = cusolverDnSsyevd(musaHandle, jobz, uplo, n, A, lda, eigenvalues, d_work, d_lwork, info_dev);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaFree(d_work);
  if (muerr != musaSuccess) {
    errormessage("Error in cusolver_DnSsyevd cuda_free(d_work): %s\n",musaGetErrorString(muerr));
  }
}

  
} // extern "C"
