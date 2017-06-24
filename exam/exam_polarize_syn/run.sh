#!/bin/bash

#####
#    Assume synthetic Radial Trace to be NS component,
#    and Tangential Trace to be EW component, then the
#    synthetic BAZ is 180 (degree).
#
#####
SACN="../data/syn.r"
SACE="../data/syn.t"
SACZ="../data/syn.z"
SACR="./r_p.sac"
SACT="./t_p.sac"

polarizeP -N${SACN} -E${SACE} -Z${SACZ} -R${SACR} -T${SACT} -W8.2/9.2 1>polarize.info

#The output is "BAZ: 269.09 (DEG)", which coincide with the synthetic BAZ.

sac <<EOF>/dev/null
beginframe
qdp off
color on inc on
line increment on list solid
xlim 5 53
#############
xvport 0.05 0.95
yvport 0.05 0.40
r ../data/syn.t ./t_p.sac
xlabel "TIME (SEC)" LOCATION BOTTOM
title "T component"
p2
##############
xvport 0.05 0.95
yvport 0.6  0.95
r ../data/syn.r ./r_p.sac
xlabel "TIME (SEC)" LOCATION BOTTOM
title "R component"
p2
##############
endframe
saveimg compare.pdf
q
EOF