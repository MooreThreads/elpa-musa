# elpa-musa

elpa-musa is maintained by Moore Threads as a MUSA adaptation of ELPA, a library for solving dense symmetric and Hermitian eigenvalue problems.

## Source Lineage

- Upstream repository: https://gitlab.mpcdf.mpg.de/elpa/elpa
- Inherited source version: v2026.02.001
- Upstream baseline commit: `847ae2a8703ad6ada1cdb7640d66873eee3e430b`
- MUSA migration ref: `origin/elpa-musa`
- MUSA migration commit: `8b4e009c185dc7f051f81c5386f5014536725fb7`

## MUSA Adaptation

- adds --enable-musa-gpu and --with-musa-path configure support
- adds MUSA compiler and MUSA math-library detection in the Autotools configuration
- retains inherited CPU, CUDA, HIP, OpenMP, MPI, test, and documentation source layout

The repository preserves the inherited upstream source structure unless a file is part of the MUSA adaptation. CUDA, CPU, Python, examples, tests, documentation, and third-party source material are retained so the published tree remains a complete source distribution.

## Dependencies

- Autoconf and Automake tooling
- MPI compiler toolchain
- Fortran and C compilers
- MUSA SDK with mcc, muBLAS, and muSOLVER

Install project-specific dependencies according to the inherited build files and use a MUSA SDK environment when building MUSA targets.

## Build

```bash
./autogen.sh
./configure --enable-musa-gpu --with-musa-path=/usr/local/musa
make -j1
```

These commands describe the static source build entry points. This publication workflow did not run a build or runtime test.

## Basic Usage

- link ELPA through the generated library and pkg-config metadata
- enable MUSA GPU support during configure for Moore Threads GPU builds

## Repository Layout

- src and elpa: inherited ELPA library source
- m4 and configure.ac: MUSA-aware Autotools configuration
- test: inherited test programs

## Contributing

Contributions should keep MUSA-specific changes clearly scoped, preserve upstream attribution, and avoid removing inherited platform support unless a change is explicitly agreed for this fork.

## Attribution and License

This project is derived from ELPA at the source version and baseline listed above. Upstream copyright and notice material is retained in the source tree. See [LICENSE](LICENSE) for the Moore Threads license declaration for this MUSA adaptation and the inherited source license terms.
