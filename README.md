# Qinv

This package is for seismic attenuation characteristics (Q value) analysis based on borehole observations.

- Author: Sheng Wang
- Email: wangsheng.cas(at)gmail.com

Table of content
---
* [Library Dependence](https://github.com/sheng09/Qinv#1-library-dependence)
* [How to Install](https://github.com/sheng09/Qinv#2-how-to-install)
* [Tools](https://github.com/sheng09/Qinv#3-tools)
* [Examples](https://github.com/sheng09/Qinv#4-examples)
* [Citation](https://github.com/sheng09/Qinv#5-citation)

# 1. Library Dependence

[`fftw3`](http://www.fftw.org/) is required for this package.

# 2. How to Install

```
make
make install
```
Then, add the generated directory of `your path to Qinv/bin/`  into your `PATH`.

# 3. Tools

## Binary

- `polarizeP`: invert the Back-Azimuth given NEZ components, and generate corresponding radial and the tangential components;
- `findBestWin`: find the optimal time window, and lag time for incident and the surface reflected waves;
- `qinv`: invert frequency-dependent Q values given single tangential seismogram, and the time windows for incident and reflected waves;
- `stackQ`: stack inverted Q values by sets of seismograms at a single station to get stable values.

## Script for volume production

- `findBestWinSet.py`: batch processing for `findBestWin`.
- `qinvSet.py`: batch processing for `qinv`.
- `stackQ.py`: batch processing for `stackQ`.

## Binary tests

All binaries with name format of `test_...` are for dev unit tests.

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

# 4. Examples

Examples are in the directory of `exam/`, each of which as a script example, `run.sh`, for running.

# 4.1 `exam_polarize_syn/`

Polarization example using synthetic seismograms for inverting the Back-Azimuth given NEZ components, and generateingcorresponding radial and the tangential components.



![](exam/exam_polarize_syn/compare.png)

Fig 4.1 Comparison of the radial and tangential synthetic seismograms (black lines), and the inverted seismograms(red lines) from `polarizeP`. 

------



# 4.2 `exam_findBestWin_syn/`

Determine the optimal time windows of incident and surface reflected waves.

![](exam/exam_findBestWin_syn/coef2d.png)

Fig .2 Grid search isogram for the optimal windows of the incident and the surface reflected phases for synthetic tangential component. Waveform comparison is presented in Fig 4.3.

------



# 4.3 `exam_qinv_syn/`

Invert frequency-dependent Q values from the incident and the surface reflected waves.

![](exam/exam_qinv_syn/qinv.png)

Fig 4.3 (up-left):The comparison of incident and the surface reflected phases. (up-right):The power spectrum comparison, black dash line presents the level of 70% maximum power. (low-left): The phase of the transfer function. (low-right): The inverted Q value. Please note that Q value is valid for the frequency band of (0.2, 0.6)Hz , in which the signal power is strong, and the phase of the transfer function is stable and approximates 0 degree.

# 5. Citation

`Wang, S. and Li, Z., 2018. S-wave attenuation of the shallow sediments in the North China basin based on borehole seismograms of local earthquakes. Geophysical Journal International, 214(2), pp.1391-1400, https://doi.org/10.1093/gji/ggy223`
