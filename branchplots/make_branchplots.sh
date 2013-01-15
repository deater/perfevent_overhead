#!/bin/sh

for i in \
164.gzip.static.graphic \
164.gzip.static.log \
164.gzip.static.program \
164.gzip.static.random \
164.gzip.static.source \
168.wupwise.static.default \
171.swim.static.default \
172.mgrid.static.default \
173.applu.static.default \
175.vpr.static.place \
175.vpr.static.route \
176.gcc.static.166 \
176.gcc.static.200 \
176.gcc.static.expr \
176.gcc.static.integrate \
176.gcc.static.scilab  \
177.mesa.static.default  \
178.galgel.static.default  \
179.art.static.110  \
179.art.static.470  \
181.mcf.static.default  \
183.equake.static.default  \
186.crafty.static.default  \
187.facerec.static.default  \
188.ammp.static.default  \
189.lucas.static.default  \
191.fma3d.static.default  \
197.parser.static.default  \
200.sixtrack.static.default  \
252.eon.static.cook  \
252.eon.static.kajiya  \
252.eon.static.rushmeier  \
253.perlbmk.static.535  \
253.perlbmk.static.704  \
253.perlbmk.static.850  \
253.perlbmk.static.957  \
253.perlbmk.static.diffmail  \
253.perlbmk.static.makerand  \
253.perlbmk.static.perfect  \
254.gap.static.default  \
255.vortex.static.1  \
255.vortex.static.2  \
255.vortex.static.3  \
256.bzip2.static.graphic \
256.bzip2.static.program \
256.bzip2.static.source \
300.twolf.static.default \
301.apsi.static.default ;
do echo $i $2;
mkdir -p graphs/$1 ;
./make_branchplot $i $1 $2 "$3" "$4" "$5" > graphs/$1/$i.$1.jgr ;
jgraph < graphs/$1/$i.$1.jgr > graphs/$1/$i.$1.eps ;
convert graphs/$1/$i.$1.eps graphs/$1/$i.$1.png ;
done
