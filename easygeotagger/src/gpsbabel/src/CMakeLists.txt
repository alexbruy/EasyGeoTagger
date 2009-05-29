IF (WIN32)
 ADD_DEFINITIONS("-D__WIN32__=WIN32") 
ENDIF (WIN32)

SET (MINIMAL_FMTS_SRCS
magproto.c
gpx.c
geo.c
mapsend.c
mapsource.c
garmin.c
garmin_device_xml.c
garmin_tables.c
internal_styles.c
nmea.c
kml.c
)

SET (ALL_FMTS_SRCS
${MINIMAL_FMTS_SRCS}
gtm.c 
gpsutil.c 
pcx.c 
cetus.c 
copilot.c 
gpspilot.c
magnav.c 
	
psp.c 
holux.c 
tmpro.c 
tpg.c 
tpo.c 
xcsv.c 
gcdb.c 
tiger.c 
easygps.c 
quovadis.c 
gpilots.c 
saroute.c 
navicache.c 
psitrex.c 
geoniche.c 
delgpl.c 
ozi.c 
text.c 
html.c 
palmdoc.c 
netstumbler.c 
hsa_ndv.c 
igc.c
brauniger_iq.c 
shape.c 
hiketech.c 
glogbook.c 
coastexp.c
vcf.c 
overlay.c 
google.c
xhtmlent.c 
lowranceusr.c
an1.c 
tomtom.c
tef_xml.c 
maggeo.c 
pathaway.c 
vitosmt.c 
gdb.c 
bcr.c 
coto.c 
ignrando.c 
stmwpp.c 
msroute.c 
cst.c 
nmn4.c 
mag_pdb.c 
compegps.c 
yahoo.c 
unicsv.c 
wfff_xml.c 
garmin_txt.c 
axim_gpb.c 
gpssim.c 
wbt-200.c 
stmsdf.c 
gtrnctr.c 
dmtlog.c 
raymarine.c 
alan.c 
vitovtt.c 
ggv_log.c 
g7towin.c 
garmin_gpi.c 
lmx.c 
random.c 
xol.c 
dg-100.c 
navilink.c 
mtk_logger.c 
ik3d.c 
osm.c 
destinator.c 
exif.c 
vidaone.c
igo8.c
gopal.c 
humminbird.c 
mapasia.c 
gnav_trl.c 
navitel.c 
ggv_ovl.c 
jtr.c 
sbp.c 
mmo.c 
skyforce.c
#
egtgpsbabelinterface.c

)



SET(FILTERS_SRCS
position.c 
radius.c 
duplicate.c 
arcdist.c 
polygon.c 
smplrout.c 
reverse_route.c 
sort.c 
stackfilter.c 
trackfilter.c 
discard.c 
nukedata.c 
interpolate.c 
transform.c 
height.c
swapdata.c
)

SET(JEEPS_SRCS
jeeps/gpsapp.c
jeeps/gpscom.c 
jeeps/gpsmath.c 
jeeps/gpsmem.c  
jeeps/gpsprot.c 
jeeps/gpsread.c 
jeeps/gpsdevice.c 
jeeps/gpsdevice_ser.c 
jeeps/gpsdevice_usb.c 
jeeps/gpsrqst.c
jeeps/gpssend.c 
jeeps/gpsserial.c 
jeeps/gpsutil.c 
jeeps/gpsusbread.c 
jeeps/gpsusbsend.c 
jeeps/gpsusbcommon.c 
jeeps/gpslibusb.c

)


SET ( PALM_DB_SRCS
pdbfile.c 
)

SET(SHAPE_SRCS
shapelib/shpopen.c 
shapelib/dbfopen.c
)

SET( LIBOBJS_SRCS
defs.h
gbsleep.c
globals.c
queue.c
route.c
waypt.c
filter_vecs.c
util.c
vecs.c
mkshort.c
csv_util.c
strptime.c
grtcirc.c
vmem.c
util_crc.c
xmlgeneric.c
uuid.c
formspec.c
xmltag.c
cet.c
cet_util.c
fatal.c
rgbcolors.c
inifile.c
garmin_fs.c
units.c
gbser.c
gbfile.c
parse.c
avltree.c
session.c    
)

IF (WIN32)
  SET( GBSER_SRCS
       gbser_win.c
	 )
ELSE (WIN32)
  SET( GBSER_SRCS
       gbser_posix.c
	  )
ENDIF (WIN32)
include_directories (${GPSBABEL_SOURCE_DIR} ${GPSBABEL_SOURCE_DIR}/jeeps ) 



ADD_LIBRARY(gpsbabel SHARED ${LIBOBJS_SRCS} ${GBSER_SRCS} ${PALM_DB_SRCS} ${JEEPS_SRCS} ${SHAPE_SRCS} ${ALL_FMTS_SRCS} ${FILTERS_SRCS} )
TARGET_LINK_LIBRARIES( gpsbabel ${LIBMATH_LIBRARY} ${EXPAT_LIBRARY} ${ZLIB_LIBRARY} ${LIBUSB_LIBRARY} )

INSTALL( TARGETS gpsbabel DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
