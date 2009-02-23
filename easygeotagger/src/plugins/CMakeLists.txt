IF ( PYTHON_BINDINGS )
  SUBDIRS( python_console )
ENDIF ( PYTHON_BINDINGS )

SUBDIRS( export_to_csv google_plugin gps_exif_editor gps_plugin )
