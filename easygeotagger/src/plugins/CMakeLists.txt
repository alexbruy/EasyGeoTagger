IF ( PYTHON_BINDINGS )
  SUBDIRS( python_console )
ENDIF ( PYTHON_BINDINGS )

SUBDIRS( export_to_csv gps_exif_editor minimal_exif_editor file_reading )
