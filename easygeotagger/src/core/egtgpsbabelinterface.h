extern "C"
{
  MS_DLL_SPEC int convert(const char* theInputFile, const char* theInputType, const char* theOutputFile, const char* theOutputType, const char* theErrorFile, const char* theInfoFile);
  MS_DLL_SPEC int convertWithOptions(const char* theInputFile, const char* theInputType, const char* theOutputFile, const char* theOutputType, const char* theErrorFile, const char* theInfoFile, const char* theOptions);
}
