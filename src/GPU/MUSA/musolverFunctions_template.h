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

musaDataType getCudaDataType(char dataType) {
  if      (dataType=='D') return MUSA_R_64F;
  else if (dataType=='S') return MUSA_R_32F;
  else if (dataType=='Z') return MUSA_C_64F;
  else if (dataType=='C') return MUSA_C_32F;
  else {
    errormessage("Error in getCudaDataType: unknown data type, %s\n", "aborting");
    return MUSA_R_64F;
  }
}

void elpa_cusolverPrintError(musolverStatus_t status){
  switch (status){
    case MUSOLVER_STATUS_SUCCESS:
        printf("cusolverStatus=MUSOLVER_STATUS_SUCCESS\n");
        break;
    case MUSOLVER_STATUS_NOT_INITIALIZED:
        printf("cusolverStatus=MUSOLVER_STATUS_NOT_INITIALIZED\n");
        break;
    case MUSOLVER_STATUS_ALLOC_FAILED:
        printf("cusolverStatus=MUSOLVER_STATUS_ALLOC_FAILED\n");
        break;
    case MUSOLVER_STATUS_INVALID_VALUE:
        printf("cusolverStatus=MUSOLVER_STATUS_INVALID_VALUE\n");
        break;
    case MUSOLVER_STATUS_ARCH_MISMATCH:
        printf("cusolverStatus=MUSOLVER_STATUS_ARCH_MISMATCH\n");
        break;
    case MUSOLVER_STATUS_MAPPING_ERROR:
        printf("cusolverStatus=MUSOLVER_STATUS_MAPPING_ERROR\n");
        break;
    case MUSOLVER_STATUS_EXECUTION_FAILED:
        printf("cusolverStatus=MUSOLVER_STATUS_EXECUTION_FAILED\n");
        break;
    case MUSOLVER_STATUS_INTERNAL_ERROR:
        printf("cusolverStatus=MUSOLVER_STATUS_INTERNAL_ERROR\n");
        break;
    case MUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
        printf("cusolverStatus=MUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED\n");
        break;
    case MUSOLVER_STATUS_NOT_SUPPORTED:
        printf("cusolverStatus=MUSOLVER_STATUS_NOT_SUPPORTED\n");
        break;
    default:
        printf("Unknown cusolverStatus status: %d\n", status);
  }
}

int cusolverGetVersionFromC() {

  int major=0, minor=0, patch=0;
#ifdef MUSOLVER_VERSION_MAJOR
  major = MUSOLVER_VERSION_MAJOR;
  minor = MUSOLVER_VERSION_MINOR;
  patch = MUSOLVER_VERSION_PATCH;
#endif

  return major*10000 + minor*100 + patch;
}

int cusolverSetStreamFromC(musolverDnHandle_t cusolver_handle, musaStream_t stream) {
  //musolverStatus_t status = musolverDnSetStream(*((musolverDnHandle_t*)cusolver_handle), *((musaStream_t*)stream));
  musolverStatus_t status = musolverDnSetStream(cusolver_handle, stream);
  if (status == MUSOLVER_STATUS_SUCCESS) {
    return 1;
  }
  else if (status == MUSOLVER_STATUS_NOT_INITIALIZED) {
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
  else if (status == MUSOLVER_STATUS_NOT_INITIALIZED) {
    errormessage("Error in cusolverCreate: %s\n", "the CUDA Runtime initialization failed");
    return 0;
  }
  else if (status == MUSOLVER_STATUS_ALLOC_FAILED) {
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
  else if (status == MUSOLVER_STATUS_NOT_INITIALIZED) {
    errormessage("Error in cusolverDestroy: %s\n", "the library has not been initialized");
    return 0;
  }
  else{
    errormessage("Error in cusolverDestroy: %s\n", "unknown error");
    return 0;
  }
}

//_________________________________________________________________________________________________
// musolver?trtri

void cusolverDtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, double *A, int64_t lda, int *info) {
  int info_dev = 0;
  int *devInfo = NULL;
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Dtrtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  musolverStatus_t status = musolverDnDtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), (int)n, A, (int)lda, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaMemcpy(&info_dev, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Dtrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }
  *info = info_dev;

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Dtrtri musaFree(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverStrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, float *A, int64_t lda, int *info) {
  int info_dev = 0;
  int *devInfo = NULL;
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Strtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  musolverStatus_t status = musolverDnStrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), (int)n, A, (int)lda, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaMemcpy(&info_dev, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Strtri info_gpu: %s\n",musaGetErrorString(muerr));
  }
  *info = info_dev;

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Strtri musaFree(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverZtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, double _Complex *A, int64_t lda, int *info) {
  int info_dev = 0;
  int *devInfo = NULL;
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ztrtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  muDoubleComplex* A_casted = (muDoubleComplex*) A;
  musolverStatus_t status = musolverDnZtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), (int)n, A_casted, (int)lda, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaMemcpy(&info_dev, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ztrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }
  *info = info_dev;

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ztrtri musaFree(devInfo): %s\n",musaGetErrorString(muerr));
  }
}


void cusolverCtrtri_elpa_wrapper (musolverDnHandle_t musaHandle, char uplo, char diag, int64_t n, float _Complex *A, int64_t lda, int *info) {
  int info_dev = 0;
  int *devInfo = NULL;
  musaError_t muerr = musaMalloc((void**)&devInfo, sizeof(int));
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ctrtri devInfo: %s\n",musaGetErrorString(muerr));
  }

  muComplex* A_casted = (muComplex*) A;
  musolverStatus_t status = musolverDnCtrtri(musaHandle, fill_mode_new_api(uplo), diag_type_new_api(diag), (int)n, A_casted, (int)lda, devInfo);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  muerr = musaMemcpy(&info_dev, devInfo, sizeof(int), musaMemcpyDeviceToHost);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ctrtri info_gpu: %s\n",musaGetErrorString(muerr));
  }
  *info = info_dev;

  muerr = musaFree(devInfo);
  if (muerr != musaSuccess) {
    errormessage("Error in musolver_Ctrtri musaFree(devInfo): %s\n",musaGetErrorString(muerr));
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
// cusolverXpotrf — MUSA has no generic Xpotrf, dispatch by dataType

void cusolverXpotrf_bufferSize_elpa_wrapper(musolverDnHandle_t cusolverHandle, char uplo, int n, char dataType, intptr_t A, int lda,
                                          size_t *workspaceInBytesOnDevice, size_t *workspaceInBytesOnHost){
  musolverStatus_t status;
  int lwork = 0;

  if (dataType == 'D') {
    status = musolverDnDpotrf_bufferSize(cusolverHandle, fill_mode_new_api(uplo), n, (double*)A, lda, &lwork);
  } else if (dataType == 'S') {
    status = musolverDnSpotrf_bufferSize(cusolverHandle, fill_mode_new_api(uplo), n, (float*)A, lda, &lwork);
  } else if (dataType == 'Z') {
    status = musolverDnZpotrf_bufferSize(cusolverHandle, fill_mode_new_api(uplo), n, (muDoubleComplex*)A, lda, &lwork);
  } else if (dataType == 'C') {
    status = musolverDnCpotrf_bufferSize(cusolverHandle, fill_mode_new_api(uplo), n, (muFloatComplex*)A, lda, &lwork);
  } else {
    errormessage("Error in cusolverXpotrf_bufferSize: unknown data type %s\n", "aborting");
    return;
  }

  if (dataType == 'D')      *workspaceInBytesOnDevice = (size_t)lwork * sizeof(double);
  else if (dataType == 'S') *workspaceInBytesOnDevice = (size_t)lwork * sizeof(float);
  else if (dataType == 'Z') *workspaceInBytesOnDevice = (size_t)lwork * sizeof(muDoubleComplex);
  else if (dataType == 'C') *workspaceInBytesOnDevice = (size_t)lwork * sizeof(muFloatComplex);
  *workspaceInBytesOnHost = 0;

  if (status != MUSOLVER_STATUS_SUCCESS){
    elpa_cusolverPrintError(status);
    errormessage("Error in musolverDnXpotrf_bufferSize %s \n", "aborting");
  }
}


void cusolverXpotrf_elpa_wrapper(musolverDnHandle_t cusolverHandle, char uplo, int n, char dataType, intptr_t A, int lda,
                                intptr_t buffer_dev , size_t *workspaceInBytesOnDevice,
                                intptr_t buffer_host, size_t *workspaceInBytesOnHost, int *info_dev){
  musolverStatus_t status;
  int lwork;

  if (dataType == 'D') {
    lwork = (int)(*workspaceInBytesOnDevice / sizeof(double));
    status = musolverDnDpotrf(cusolverHandle, fill_mode_new_api(uplo), n, (double*)A, lda, (double*)buffer_dev, lwork, info_dev);
  } else if (dataType == 'S') {
    lwork = (int)(*workspaceInBytesOnDevice / sizeof(float));
    status = musolverDnSpotrf(cusolverHandle, fill_mode_new_api(uplo), n, (float*)A, lda, (float*)buffer_dev, lwork, info_dev);
  } else if (dataType == 'Z') {
    lwork = (int)(*workspaceInBytesOnDevice / sizeof(muDoubleComplex));
    status = musolverDnZpotrf(cusolverHandle, fill_mode_new_api(uplo), n, (muDoubleComplex*)A, lda, (muDoubleComplex*)buffer_dev, lwork, info_dev);
  } else if (dataType == 'C') {
    lwork = (int)(*workspaceInBytesOnDevice / sizeof(muFloatComplex));
    status = musolverDnCpotrf(cusolverHandle, fill_mode_new_api(uplo), n, (muFloatComplex*)A, lda, (muFloatComplex*)buffer_dev, lwork, info_dev);
  } else {
    errormessage("Error in cusolverXpotrf: unknown data type %s\n", "aborting");
    return;
  }

  if (status != MUSOLVER_STATUS_SUCCESS){
    elpa_cusolverPrintError(status);
    errormessage("Error in musolverDnXpotrf %s \n", "aborting");
  }
}


//_________________________________________________________________________________________________
// musolverXsyevd

void cusolverDsyevd_elpa_wrapper (musolverDnHandle_t musaHandle, int n, double *A, int lda, double *eigenvalues, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  mublasEvect jobz = MUBLAS_EVECT_ORIGINAL;
  mublasFillMode_t uplo = MUBLAS_FILL_MODE_LOWER;

  size_t bufferSize = 0;
  status = musolverDnDsyevd_bufferSize(jobz, uplo, n, &bufferSize);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnDsyevd_bufferSize %s \n","aborting");
  }

  void *d_work = NULL;
  muerr = musaMalloc(&d_work, bufferSize);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc d_work: %s\n",musaGetErrorString(muerr));
  }

  double *E = NULL;
  muerr = musaMalloc((void**)&E, sizeof(double) * (n > 1 ? n - 1 : 1));
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc E: %s\n",musaGetErrorString(muerr));
  }

  status = musolverDnDsyevd(musaHandle, jobz, uplo, n, A, lda, eigenvalues, E, info_dev, d_work);

  if (status != MUSOLVER_STATUS_SUCCESS) elpa_cusolverPrintError(status);

  musaFree(E);
  musaFree(d_work);
}

void cusolverSsyevd_elpa_wrapper (musolverDnHandle_t musaHandle, int n, float *A, int lda, float *eigenvalues, int *info_dev) {
  musolverStatus_t status;
  musaError_t muerr;

  mublasEvect jobz = MUBLAS_EVECT_ORIGINAL;
  mublasFillMode_t uplo = MUBLAS_FILL_MODE_LOWER;

  size_t bufferSize = 0;
  status = musolverDnSsyevd_bufferSize(jobz, uplo, n, &bufferSize);
  if (status != MUSOLVER_STATUS_SUCCESS) {
    errormessage("Error in musolverDnSsyevd_bufferSize %s \n","aborting");
  }

  void *d_work = NULL;
  muerr = musaMalloc(&d_work, bufferSize);
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc d_work: %s\n",musaGetErrorString(muerr));
  }

  float *E = NULL;
  muerr = musaMalloc((void**)&E, sizeof(float) * (n > 1 ? n - 1 : 1));
  if (muerr != musaSuccess) {
    errormessage("Error in musaMalloc E: %s\n",musaGetErrorString(muerr));
  }

  status = musolverDnSsyevd(musaHandle, jobz, uplo, n, A, lda, eigenvalues, E, info_dev, d_work);

  if (status != MUSOLVER_STATUS_SUCCESS)
    elpa_cusolverPrintError(status);

  musaFree(E);
  musaFree(d_work);
}

  
} // extern "C"
