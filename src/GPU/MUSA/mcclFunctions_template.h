#define errormessage(x, ...) do { fprintf(stderr, "%s:%d " x, __FILE__, __LINE__, __VA_ARGS__ ); } while (0)

#ifdef DEBUG_MUSA
#define debugmessage(x, ...) do { fprintf(stderr, "%s:%d " x, __FILE__, __LINE__, __VA_ARGS__ ); } while (0)
#else
#define debugmessage(x, ...)
#endif
  
mcclUniqueId globalIDFixThis;

#ifdef WITH_NVIDIA_NCCL
extern "C" {
  int mcclGroupStartFromC() {
    mcclResult_t ncclError;

    ncclError = mcclGroupStart();
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclGroupStart: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclGroupStart: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int mcclGroupEndFromC() {
    mcclResult_t ncclError;

    ncclError = mcclGroupEnd();
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclGroupEnd: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclGroupEnd: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }


  int mcclGetUniqueIdFromC(mcclUniqueId *ncclID) {
    mcclResult_t ncclError;
    mcclUniqueId id_dummy;
    ncclError = mcclGetUniqueId(&id_dummy);
    for (int i=0; i<sizeof(mcclUniqueId);i++) {
       ncclID->internal[i] =  id_dummy.internal[i];
       //DEBUG
       //printf("a %c \n",ncclID->internal[i]);
    }
    
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclGetUniqueId: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclGetUniqueId: %s\n", "unknown error");
      }
      return 0;

    }
    return 1;
  }

  int mcclCommInitRankFromC(mcclComm_t *ncclComm, int nRanks, mcclUniqueId *ncclID, int myRank) {
    mcclResult_t ncclError;

    mcclUniqueId id_dummy;
    for (int i=0; i<sizeof(mcclUniqueId);i++) {
       // debug
       //printf("j %c \n",ncclID->internal[i]);
       id_dummy.internal[i] = ncclID->internal[i];
    }
    if (sizeof(mcclUniqueId) != 16*sizeof(intptr_t)) {
     printf("sizes of mcclUniqueId changed \n");
     return 0;
    }

    ncclError = mcclCommInitRank(ncclComm, nRanks, id_dummy, myRank);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclCommInitRank: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclCommInitRank: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  // only for version >= 2.13
  //int ncclCommFinalizeFromC(mcclComm_t ncclComm) {
  //  mcclResult_t ncclError;

  //  ncclError = ncclCommFinalize(ncclComm);
  //  if (ncclError != mcclSuccess) {
  //    errormessage("Error in ncclCommFinalize: %s\n", "unknown error");
  //    return 0;

  //  }
  //  return 1;
  //}

  int mcclCommDestroyFromC(mcclComm_t ncclComm) {
    mcclResult_t ncclError;

    //signature: mcclResult_t mcclCommDestroy(mcclComm_t comm)
    ncclError = mcclCommDestroy(ncclComm);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclCommDestroy: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclCommDestroy: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int ncclRedOpSumFromC(void) {
    int val = mcclSum;
    return val;
  }

  int ncclRedOpProdFromC(void) {
    int val = ncclProd;
    return val;
  }

  int ncclRedOpMinFromC(void) {
    int val = ncclMin;
    return val;
  }

  int ncclRedOpMaxFromC(void) {
    int val = ncclMax;
    return val;
  }

  int ncclRedOpAvgFromC(void) {
    int val = ncclAvg;
    return val;
  }

  int ncclDataTypeNcclIntFromC(void) {
    int val = ncclInt;
    return val;
  }

  int ncclDataTypeNcclInt32FromC(void) {
    int val = ncclInt32;
    return val;
  }

  int ncclDataTypeNcclInt64FromC(void) {
    int val = ncclInt64;
    return val;
  }

  int ncclDataTypeNcclFloat32FromC(void) {
    int val = mcclFloat32;
    return val;
  }

  int ncclDataTypeNcclFloatFromC(void) {
    int val = mcclFloat;
    return val;
  }

  int ncclDataTypeNcclFloat64FromC(void) {
    int val = mcclFloat64;
    return val;
  }

  int ncclDataTypeNcclDoubleFromC(void) {
    int val = mcclDouble;
    return val;
  }

  int mcclAllReduceFromC(const void *sendbuff, void *recvbuff, size_t count, ncclDataType_t ncclDatatype, ncclRedOp_t ncclOp, mcclComm_t ncclComm, musaStream_t musaStream) {
    mcclResult_t ncclError;

    ncclError = mcclAllReduce(sendbuff, recvbuff, count, ncclDatatype, ncclOp, ncclComm, musaStream);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclAllReduce: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclAllReduce: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int mcclReduceFromC(const void *sendbuff, void *recvbuff, size_t count, ncclDataType_t ncclDatatype, ncclRedOp_t ncclOp, int root, mcclComm_t ncclComm, musaStream_t musaStream) {
    mcclResult_t ncclError;

    ncclError = mcclReduce(sendbuff, recvbuff, count, ncclDatatype, ncclOp, root, ncclComm, musaStream);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclReduce: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclReduce: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclReduce: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclReduce: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclReduce: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclReduce: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclReduce: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int mcclBroadcastFromC(const void* sendbuff, void* recvbuff, size_t count, ncclDataType_t ncclDatatype, int root, mcclComm_t ncclComm, musaStream_t musaStream) {
    mcclResult_t ncclError;

    ncclError = mcclBroadcast(sendbuff, recvbuff, count, ncclDatatype, root, ncclComm, musaStream);

    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in mcclBroadcast: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in mcclBroadcast: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int ncclSendFromC(const void* sendbuff, size_t count, ncclDataType_t ncclDatatype, int peer, mcclComm_t ncclComm, musaStream_t musaStream) {
    mcclResult_t ncclError;

    ncclError = ncclSend(sendbuff, count, ncclDatatype, peer, ncclComm, musaStream);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in ncclSend: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in ncclSend: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in ncclSend: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in ncclSend: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in ncclSend: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in ncclSend: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in ncclSend: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }

  int ncclRecvFromC(void* recvbuff, size_t count, ncclDataType_t ncclDatatype, int peer, mcclComm_t ncclComm, musaStream_t musaStream) {
    mcclResult_t ncclError;

    ncclError = ncclRecv(recvbuff, count, ncclDatatype, peer, ncclComm, musaStream);
    if (ncclError != mcclSuccess) {
      if (ncclError == ncclUnhandledCudaError) {
        errormessage("Error in ncclRecv: %s\n", "ncclUnhandledCudaError");
      } else if (ncclError == ncclSystemError) {
        errormessage("Error in ncclRecv: %s\n", "ncclSystemError");
      } else if (ncclError == ncclInternalError) {
        errormessage("Error in ncclRecv: %s\n", "ncclInternalError");
      } else if (ncclError == ncclInvalidArgument) {
        errormessage("Error in ncclRecv: %s\n", "ncclInvalidArguments");
      } else if (ncclError == ncclInvalidUsage) {
        errormessage("Error in ncclRecv: %s\n", "ncclInvalidUsage");
      } else if (ncclNumResults) {
        errormessage("Error in ncclRecv: %s\n", "ncclNumResults");
      } else {
        errormessage("Error in ncclRecv: %s\n", "unknown error");
      }
      return 0;
    }
    return 1;
  }


}
#endif
