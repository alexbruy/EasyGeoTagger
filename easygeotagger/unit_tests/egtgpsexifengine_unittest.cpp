#include "egtgpsexifengine_unittest.h"


EgtGpsExifEngine_UnitTest::EgtGpsExifEngine_UnitTest()
{
  cvGpsExifEngine =new EgtGpsExifEngine();
}

int EgtGpsExifEngine_UnitTest::runTest(QTextEdit* theOutputWindow)
{
  bool lvOk;
  int lvNumFailures = 0;

  cvGpsExifEngine->setFile("./test.jpg");

  lvOk = cvGpsExifEngine->writeAltitude("3.141592654");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeAltitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeAltitude()--> FAIL");
  }

  cvGpsExifEngine->altitude(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: altitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: altitude()--> FAIL");
  } 
  
  lvOk = cvGpsExifEngine->writeAreaInformation("AreaInformation"); //THIS ONE CRASHES!!
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeAreaInformation()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeAreaInformation()--> FAIL");
  } 

  cvGpsExifEngine->areaInformation(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: areaInformation()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: areaInformation()--> FAIL");
  }  

  lvOk = cvGpsExifEngine->writeDateStamp("DateStamp");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDateStamp()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDateStamp()--> FAIL");
  }  

  cvGpsExifEngine->dateStamp(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: dateStamp()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: dateStamp()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestBearing("120.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestBearing()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestBearing()--> FAIL");
  }  

  cvGpsExifEngine->destBearing(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destBearing()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destBearing()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestBearingRef("M");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestBearingRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestBearingRef()--> FAIL");
  }  

  cvGpsExifEngine->destBearingRef(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destBearingRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destBearingRef()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDifferential("3");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDifferential()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDifferential()--> FAIL");
  } 

  cvGpsExifEngine->differential(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: differential()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: differential()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDirection("121.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDirection()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDirection()--> FAIL");
  } 

  cvGpsExifEngine->direction(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: direction()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: direction()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDirectionRef("T");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDirectionRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDirectionRef()--> FAIL");
  } 

  cvGpsExifEngine->directionRef(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: directionRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: directionRef()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestDistance("4");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestDistance()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestDistance()--> FAIL");
  } 

  cvGpsExifEngine->destDistance(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destDistance()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destDistance()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestDistanceRef("K");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestDistanceRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestDistanceRef()--> FAIL");
  } 

  cvGpsExifEngine->destDistanceRef(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destDistanceRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destDistanceRef()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestLatitude("122.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestLatitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestLatitude()--> FAIL");
  } 

  cvGpsExifEngine->destLatitude(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destLatitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destLatitude()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeDestLongitude("123.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestLongitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeDestLongitude()--> FAIL");
  } 

  cvGpsExifEngine->destLongitude(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destLongitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: destLongitude()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeGpsDOP("5");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeGpsDOP()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeGpsDOP()--> FAIL");
  } 

  cvGpsExifEngine->gpsDOP(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: gpsDOP()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: gpsDOP()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeLatitude("124.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeLatitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeLatitude()--> FAIL");
  } 

  cvGpsExifEngine->latitude(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: latitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: latitude()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeLongitude("125.0");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeLongitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeLongitude()--> FAIL");
  } 

  cvGpsExifEngine->longitude(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: longitude()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: longitude()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeMapDatum("TOKYO");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeMapDatum()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeMapDatum()--> FAIL");
  } 

  cvGpsExifEngine->mapDatum(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: mapDatum()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: mapDatum()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeMeasureMode("2");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeMeasureMode()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeMeasureMode()--> FAIL");
  } 

  cvGpsExifEngine->measureMode(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: measureMode()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: measureMode()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeProcessingMethod("ProcessingMethod"); //THIS ONE CRASHES!!
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeProcessingMethod()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeProcessingMethod()--> FAIL");
  } 

  cvGpsExifEngine->processingMethod(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: processingMethod()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: processingMethod()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeSatellites("1 2 3");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSatellites()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSatellites()--> FAIL");
  } 

  cvGpsExifEngine->satellites(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: satellites()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: satellites()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeSpeed("6");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSpeed()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSpeed()--> FAIL");
  } 

  cvGpsExifEngine->speed(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: speed()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: speed()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeSpeedRef("K");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSpeedRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeSpeedRef()--> FAIL");
  } 

  cvGpsExifEngine->speedRef(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: speedRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: speedRef()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeStatus("A");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeStatus()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeStatus()--> FAIL");
  } 

  cvGpsExifEngine->status(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: status()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: status()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeTimeStamp("00:01:02");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTimeStamp()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTimeStamp()--> FAIL");
  } 

  cvGpsExifEngine->timeStamp(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: timeStamp()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: timeStamp()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeTrack("125");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTrack()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTrack()--> FAIL");
  } 

  cvGpsExifEngine->track(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: track()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: track()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeTrackRef("T");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTrackRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeTrackRef()--> FAIL");
  } 

  cvGpsExifEngine->trackRef(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: trackRef()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: trackRef()--> FAIL");
  } 

  lvOk = cvGpsExifEngine->writeVersionID("2");
  if ( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeVersionID()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: writeVersionID()--> FAIL");
  } 

  cvGpsExifEngine->versionID(&lvOk);
  if( lvOk )
  {
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: versionID()--> Ok");
  }
  else
  {
    lvNumFailures++;
    theOutputWindow->append("class: [EgtGpsExifEngine]. Function: versionID()--> FAIL");
  } 

  return lvNumFailures;
}