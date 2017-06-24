#!/bin/bash

#####
#    1. Invert the Q
#
#      -L the lag time. (data is from findBestWin)
#      -W the window of the incident phase. (data is from findBestWin)
#####
qinv -I../data/syn.t -C5/-10/20 -L8.3125 -W39.092072/44.127071 -T0.1 -S1.5 -F4096 -Oqinv

#####
#    2. Plot the Q^(-1)
#####
gmtset GRID_PEN_PRIMARY ._0.2p 
gmtset TICK_LENGTH 0.1c 
gmtset ANNOT_OFFSET_PRIMARY 0.2c 
gmtset LABEL_OFFSET 0.1c 
gmtset ANNOT_FONT_SIZE_PRIMARY 8 
gmtset LABEL_FONT_SIZE 8 
gmtset HEADER_FONT_SIZE 10 
gmtset HEADER_OFFSET 0.2c 
gmtset FRAME_WIDTH 0.2p 

PS="qinv.ps"
psbasemap -JX8cl/6cl -R0.01/3/0.001/1 -Ba1f3g3:"Frequency (HZ)":/a1f3g3WSne:"Q@+-1"::."Inverted Q@+-1": -K -X12c -P > ${PS}
awk '$1>0{print $0}' qinv.FreqQinv | psxy -JX -R -W0.2p,BLACK  -K -O >>${PS}
awk '$1>0{print $0}' qinv.FreqQinv | psxy -JX -R -Sc0.1c -GRED -K -O >>${PS}

#####
#    3. Plot the Power Spectrum of the incident and the reflected phase
#####
psbasemap -JX8cl/6c -R0.01/3/0/1.0 -Ba1f3g3:"Frequency (HZ)":/a0.5f0.1:"Normalized Power Spectrum"::."Power Spectrum":WSne -K -O -Y9c >> ${PS}
psxy -JX -R -W1p,BLACK,- -K -O >>${PS}<<EOF
0.01 0.7
3    0.7
EOF
pstext -JX -R -K -O >>${PS}<<EOF
0.02 0.75 12 0 0 CM 70%
EOF
MAXPOW=`minmax qinv.inc.FreqPow | awk '{print $6}' | awk -F/ '{print $2}' | sed 's/>//' `
awk -va=${MAXPOW} '$1>0{print $1,$2/a}' qinv.inc.FreqPow | psxy -JX -R -W0.8p,RED  -K -O >> ${PS}
awk -va=${MAXPOW} '$1>0{print $1,$2/a}' qinv.ref.FreqPow | psxy -JX -R -W0.8p,BLUE -K -O >> ${PS}

#####
#    4. Plot the waveform
#####
psbasemap -JX8c/6c -R0.0/5/-1.1/1.1 -Ba1f0.2g0.4:"Time (SECS)":/a0.5f0.1g0.2:"Normalized Amplitude"::."Incident(red) and Reflected(blue) Waveform":WSne -K -O -X-10c >>${PS}
MAXAMP1=`minmax qinv.inc.TimeTrace | awk '{print $6}' | awk -F/ '{print $2}' | sed 's/>//' `
T1=`minmax qinv.inc.TimeTrace | awk '{print $5}' | awk -F/ '{print $1}' | sed 's/<//' `
awk -vt=${T1} -va=${MAXAMP1} '{print $1-t,$2/a}' qinv.inc.TimeTrace | psxy -JX -R -W0.8p,RED -K -O >> ${PS}
MAXAMP2=`minmax qinv.ref.TimeTrace | awk '{print $6}' | awk -F/ '{print $2}' | sed 's/>//' `
T2=`minmax qinv.ref.TimeTrace | awk '{print $5}' | awk -F/ '{print $1}' | sed 's/<//' `
awk -vt=${T2} -va=${MAXAMP2} '{print $1-t,$2/a}' qinv.ref.TimeTrace | psxy -JX -R -W0.3p,BLUE,- -K -O >> ${PS}
awk -vt=${T2} -va=${MAXAMP1} '{print $1-t,$2/a}' qinv.ref.TimeTrace | psxy -JX -R -W0.8p,BLUE   -K -O >> ${PS}

#####
#    5. Plot the transfer function phase
#####
psbasemap -JX8cl/6c -R0.01/3/-180/180 -Ba1f3g3:"Frequency (HZ)":/a30f10g30:"Phase (DEG)"::."Transfer Function Phase":WSne -K -O -Y-9c >> ${PS}
awk '$1>0{print $0}' qinv.FreqGreenphase | psxy -JX -R -W0.2p,BLACK -K -O >>${PS}
awk '$1>0{print $0}' qinv.FreqGreenphase | psxy -JX -R -Sc0.1c -GRED -O >>${PS}

ps2raster -Tf -A ${PS}


