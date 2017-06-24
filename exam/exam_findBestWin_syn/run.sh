#!/bin/bash
#####
#    1. Find the best window for the incident and the reflected phase
#####
findBestWin -I../data/syn.t -C5/-10/30 -LF8.3121 -W-4.5/5 -T0.1 -F1/10 -S1.5 -X0.2 -Obest_win_syn 1>findBestWin.info

## The output is:
#SACHDR t5: 40.714573 , t6: 49.027031 , lag 8.312458 , dt: 0.002500
#PreOpt[ 36.214573 40.277073 ](1626), SufOpt[ 41.094574 45.714573 ](1848), LagOpt[ 0.000358 16.624557 ](6650)
#Best pre: 39.092072 , suf: 44.127071 , lag: 8.312500


#####
#    2. Plot the 2d Coef
#####
gmtset TICK_LENGTH 0.2c
gmtset ANNOT_OFFSET_PRIMARY 0.2c
gmtset LABEL_OFFSET 0.1c
gmtset ANNOT_FONT_SIZE_PRIMARY 8
gmtset LABEL_FONT_SIZE 8
gmtset HEADER_FONT_SIZE 10
gmtset HEADER_OFFSET 0.2c
gmtset FRAME_WIDTH 0.2p
gmtset FRAME_PEN   0.2p

PS="coef2d.ps"
awk '{print $1,$2,$3^2}' best_win_syn.coef2d | \
	xyz2grd -Gbest_win_syn.coef2d.grd -I0.0025/0.0025 -R36.2146/40.2771/41.0946/45.7121
grd2cpt best_win_syn.coef2d.grd -Cpolar -Z -Qo  > tmp.cpt
grdimage best_win_syn.coef2d.grd -JX6c/6c -R36.2146/40.2771/41.0946/45.7121 \
	-Ba0.5f0.1:"Pre (SECS)":/a0.5f0.1:"Suf (SECS)":WSne -Ctmp.cpt -P -K > ${PS}
psxy -JX -R -m -W0.2p,WHITE,- -K -O >> ${PS}<<EOF
36.222 44.13
40.274 44.13
>
39.09 41
39.09 46
EOF
pstext -JX -R -GWHITE -K -O >>${PS}<<EOF
39.2  41.3 8 0 0 LM 39.09
36.35 44   8 0 0 LM 44.13
EOF
psscale -Ctmp.cpt -D7c/3c/5c/0.2c -B0.00005/:"c@+2": -O >>${PS}
ps2raster -Tf -A ${PS}
rm tmp.cpt


#####
#    3. Plot the 2d CrossCor
#####
PS="cor2d.ps"
awk '{print $1,$2,$3}' best_win_syn.cor2d | \
	xyz2grd -Gbest_win_syn.cor2d.grd -I0.0025/0.0025 -R36.2146/40.2771/41.0946/45.7121
#grd2cpt best_win_syn.cor2d.grd -Cpolar -Z -E40 > tmp.cpt
makecpt -Cpolar -Z -T0.90/1/0.001 > tmp.cpt
grdimage best_win_syn.cor2d.grd -JX6c/6c -R36.2146/40.2771/41.0946/45.7121 \
	-Ba0.5f0.1:"Pre (SECS)":/a0.5f0.1:"Suf (SECS)":WSne -Ctmp.cpt -P -K > ${PS}
psxy -JX -R -m -W0.2p,WHITE,- -K -O >> ${PS}<<EOF
36.222 44.13
40.274 44.13
>
39.09 41
39.09 46
EOF
pstext -JX -R -GWHITE -K -O >>${PS}<<EOF
39.2  41.3 8 0 0 LM 39.09
36.35 44   8 0 0 LM 44.13
EOF
psscale -Ctmp.cpt -D7c/3c/5c/0.2c -B0.025/:"CrossCor": -O >>${PS}
ps2raster -Tf -A ${PS}
rm tmp.cpt

