# Qinv

This is a package for attenuation analysis for borehole observation.

# 1. Prerequest

[`fftw3`](http://www.fftw.org/) is required for this package.

# 2. How to install

```
make
make install
```

# 3. Tools

## Binary

- `polarizeP`: invert the Back-Azimuth given NEZ components, and rotate to radial and the tangential directions.
- `findBestWin`: find the best time window, and lag time for the incident and the surface reflected phases.
- `qinv`: invert the Q value given tangential trace, and the time windows of the incident and the reflected phases.
- `stackQ`: stack the inverted Q values in a single station to get a stable value.

## Script for volume production

- `findBestWinSet.py`: volume production for `findBestWin`.
- `qinvSet.py`: volume production for `qinv`.
- `stackQ.py`: volume production for `stackQ`.

## Binary tests

All binaries of `test_...` are for checking whether this package is correctly installed.

- `test_atten`: 
- `test_crossSpec`: 
- `test_findBestWin`: 
- `test_polarize`: 
- `test_qinv`: 
- `test_qjob`: 
- `test_ranStack`: 
- `test_stackQ`: 
- `test_tracef1d`: 
- `test_tracef1d_Spec`: 
- `test_winopt`: 
