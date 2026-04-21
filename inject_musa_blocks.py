#!/usr/bin/env python3
"""
Inject MUSA GPU support blocks into ELPA vendor_agnostic_*.F90 files.
For each WITH_NVIDIA_GPU_VERSION block, adds a parallel WITH_MUSA_GPU_VERSION block.
"""
import re
import sys

def create_musa_block_from_nvidia(nvidia_block):
    """Convert a NVIDIA block to MUSA block."""
    musa = nvidia_block
    musa = musa.replace('WITH_NVIDIA_GPU_VERSION', 'WITH_MUSA_GPU_VERSION')
    musa = musa.replace('use cuda_functions', 'use musa_functions')
    musa = musa.replace('use cusolver_functions', 'use musolver_functions')
    musa = musa.replace('use nccl_functions', 'use mccl_functions')
    musa = musa.replace('nvidia_gpu', 'musa_gpu')
    
    # Variable names: cuda* -> musa*
    musa = re.sub(r'cudaMemcpyHostToDevice', 'musaMemcpyHostToDevice', musa)
    musa = re.sub(r'cudaMemcpyDeviceToHost', 'musaMemcpyDeviceToHost', musa)
    musa = re.sub(r'cudaMemcpyDeviceToDevice', 'musaMemcpyDeviceToDevice', musa)
    musa = re.sub(r'cudaHostRegisterPortable', 'musaHostRegisterPortable', musa)
    musa = re.sub(r'cudaHostRegisterMapped', 'musaHostRegisterMapped', musa)
    musa = re.sub(r'cudaHostRegisterDefault', 'musaHostRegisterDefault', musa)
    musa = re.sub(r'cublasPointerModeDevice', 'mublasPointerModeDevice', musa)
    musa = re.sub(r'cublasPointerModeHost', 'mublasPointerModeHost', musa)
    
    # Function calls: cuda_* -> musa_*, cublas_* -> mublas_*, cusolver_* -> musolver_*
    musa = re.sub(r'cublas_', 'mublas_', musa)
    musa = re.sub(r'cusolver_', 'musolver_', musa)
    musa = re.sub(r'cuda_', 'musa_', musa)
    
    return musa

def inject_musa_blocks(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    
    lines = content.split('\n')
    new_lines = []
    i = 0
    
    while i < len(lines):
        line = lines[i]
        new_lines.append(line)
        
        # Detect start of NVIDIA block
        if line.strip() == '#ifdef WITH_NVIDIA_GPU_VERSION':
            # Collect the full block until matching #endif
            nvidia_block_lines = [line]
            depth = 1
            i += 1
            while i < len(lines) and depth > 0:
                line = lines[i]
                nvidia_block_lines.append(line)
                new_lines.append(line)
                if line.strip().startswith('#ifdef') or line.strip().startswith('#if '):
                    depth += 1
                elif line.strip() == '#endif':
                    depth -= 1
                i += 1
            
            # Create and insert MUSA block
            nvidia_block = '\n'.join(nvidia_block_lines)
            musa_block = create_musa_block_from_nvidia(nvidia_block)
            new_lines.append('')  # blank line
            new_lines.extend(musa_block.split('\n'))
            continue
        
        i += 1
    
    with open(filepath, 'w') as f:
        f.write('\n'.join(new_lines))
    
    print(f"Injected MUSA blocks into {filepath}")

if __name__ == '__main__':
    for fpath in sys.argv[1:]:
        inject_musa_blocks(fpath)
