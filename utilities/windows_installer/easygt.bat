@echo off
SET OSGEO4W_ROOT=C:\Documents and Settings\Roberto\Desktop\EasyGeoTagger_0.3.0_OSGeo4W
@echo off

start "Easy geotagger" /B "%OSGEO4W_ROOT%"\bin\easygeotagger.exe %*
