IF ( PYTHON_BINDINGS )
  SUBDIRS( python_console )
ENDIF ( PYTHON_BINDINGS )

SUBDIRS( export_to_csv minimal_exif_editor file_reading )
