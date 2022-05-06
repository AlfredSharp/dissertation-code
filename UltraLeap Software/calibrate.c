#include <stdio.h>
#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <conio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "LeapC.h"
#include "ExampleConnection.h"

int64_t lastFrameID = 0; //The last frame received

void recordPos(int time, eLeapRS result, LEAP_RECORDING recordingHandle){
    //run loop for time milliseconds
    int64_t startTime = LeapGetNow();
    int64_t endTime = startTime + time;
    while (LeapGetNow() < endTime){
        LEAP_TRACKING_EVENT *frame = GetFrame();
        if(frame && (frame->tracking_frame_id > lastFrameID)){
        lastFrameID = frame->tracking_frame_id;

        uint64_t dataWritten = 0;
        result = LeapRecordingWrite(recordingHandle, frame, &dataWritten);
        printf("Recorded %"PRIu64" bytes for frame %"PRIu64" with %i hands.\n", dataWritten, frame->tracking_frame_id, frame->nHands);
        }
    }

    result = LeapRecordingClose(&recordingHandle);
    if(!LEAP_SUCCEEDED(result))
      printf("Failed to close recording: %s\n", ResultString(result));
}

int main(int argc, char** argv) {
  OpenConnection();
  while(!IsConnected)
    millisleep(100); //wait a bit to let the connection complete

  printf("Connected.\n");
  LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();
  if(deviceProps)
    printf("Using device %s.\n", deviceProps->serial);
  printf("Starting time is %" PRId64 "\n", LeapGetNow());

  LEAP_RECORDING recordingHandle;
  LEAP_RECORDING_PARAMETERS params;

  //Open the recording for writing
  params.mode = eLeapRecordingFlags_Writing;
  eLeapRS result = LeapRecordingOpen(&recordingHandle, argv[1], params);

  if(LEAP_SUCCEEDED(result)){
      recordPos(25000000, result, recordingHandle);
  } else {
    printf("Failed to open recording for writing: %s\n", ResultString(result));
  }
  return 0;
}
