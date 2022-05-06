#include <stdio.h>
#include <stdlib.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "LeapC.h"
#include "ExampleConnection.h"

int64_t lastFrameID = 0; //The last frame received

void *openFile(char *fileName)
{
  LEAP_RECORDING recordingHandle;
  LEAP_RECORDING_PARAMETERS params;

  params.mode = eLeapRecordingFlags_Reading;
  eLeapRS result = LeapRecordingOpen(&recordingHandle, fileName, params);
  if(LEAP_SUCCEEDED(result)){
    LEAP_TRACKING_EVENT *frame = 0;
    LEAP_RECORDING_STATUS *status = 0;
    while(true){
      uint64_t nextFrameSize = 0;
      result = LeapRecordingReadSize(recordingHandle, &nextFrameSize);
      if(!LEAP_SUCCEEDED(result))
        printf("Couldn't get next frame size: %s\n", ResultString(result));
      if(nextFrameSize > 0){
        frame = (LEAP_TRACKING_EVENT *)malloc((size_t)nextFrameSize);
        result = LeapRecordingRead(recordingHandle, frame, nextFrameSize);
        if(LEAP_SUCCEEDED(result)){
          //print every 20th frame
          if(frame->tracking_frame_id % 100 == 0){
            printf("Read frame %"PRIu64" with %i hands at timestamp %" PRId64 "\n", frame->tracking_frame_id, frame->nHands, frame->info.timestamp);
          }
          if(frame->nHands > 0){
            // make a string that is the concatenation of "data" and i
            char dataFile[100];
            sprintf(dataFile, "%s%s", fileName, ".csv");

            FILE *fptr = fopen(dataFile, "a");
            //index distal phalange, thumb distal phalange
            fprintf(fptr, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", frame->pHands[0].index.bones[0].prev_joint.x, frame->pHands[0].index.bones[0].prev_joint.y, frame->pHands[0].index.bones[0].prev_joint.z, frame->pHands[0].index.bones[1].prev_joint.x, frame->pHands[0].index.bones[1].prev_joint.y, frame->pHands[0].index.bones[1].prev_joint.z, frame->pHands[0].index.bones[2].prev_joint.x, frame->pHands[0].index.bones[2].prev_joint.y, frame->pHands[0].index.bones[2].prev_joint.z, frame->pHands[0].index.bones[3].prev_joint.x, frame->pHands[0].index.bones[3].prev_joint.y, frame->pHands[0].index.bones[3].prev_joint.z,     frame->pHands[0].thumb.bones[0].prev_joint.x, frame->pHands[0].thumb.bones[0].prev_joint.y, frame->pHands[0].thumb.bones[0].prev_joint.z, frame->pHands[0].thumb.bones[1].prev_joint.x, frame->pHands[0].thumb.bones[1].prev_joint.y, frame->pHands[0].thumb.bones[1].prev_joint.z, frame->pHands[0].thumb.bones[2].prev_joint.x, frame->pHands[0].thumb.bones[2].prev_joint.y, frame->pHands[0].thumb.bones[2].prev_joint.z, frame->pHands[0].thumb.bones[3].prev_joint.x, frame->pHands[0].thumb.bones[3].prev_joint.y, frame->pHands[0].thumb.bones[3].prev_joint.z,                frame->pHands[0].middle.bones[0].prev_joint.x, frame->pHands[0].middle.bones[0].prev_joint.y, frame->pHands[0].middle.bones[0].prev_joint.z, frame->pHands[0].middle.bones[1].prev_joint.x, frame->pHands[0].middle.bones[1].prev_joint.y, frame->pHands[0].middle.bones[1].prev_joint.z, frame->pHands[0].middle.bones[2].prev_joint.x, frame->pHands[0].middle.bones[2].prev_joint.y, frame->pHands[0].middle.bones[2].prev_joint.z, frame->pHands[0].middle.bones[3].prev_joint.x, frame->pHands[0].middle.bones[3].prev_joint.y, frame->pHands[0].middle.bones[3].prev_joint.z,          frame->pHands[0].ring.bones[0].prev_joint.x, frame->pHands[0].ring.bones[0].prev_joint.y, frame->pHands[0].ring.bones[0].prev_joint.z, frame->pHands[0].ring.bones[1].prev_joint.x, frame->pHands[0].ring.bones[1].prev_joint.y, frame->pHands[0].ring.bones[1].prev_joint.z, frame->pHands[0].ring.bones[2].prev_joint.x, frame->pHands[0].ring.bones[2].prev_joint.y, frame->pHands[0].ring.bones[2].prev_joint.z, frame->pHands[0].ring.bones[3].prev_joint.x, frame->pHands[0].ring.bones[3].prev_joint.y, frame->pHands[0].ring.bones[3].prev_joint.z,         frame->pHands[0].pinky.bones[0].prev_joint.x, frame->pHands[0].pinky.bones[0].prev_joint.y, frame->pHands[0].pinky.bones[0].prev_joint.z, frame->pHands[0].pinky.bones[1].prev_joint.x, frame->pHands[0].pinky.bones[1].prev_joint.y, frame->pHands[0].pinky.bones[1].prev_joint.z, frame->pHands[0].pinky.bones[2].prev_joint.x, frame->pHands[0].pinky.bones[2].prev_joint.y, frame->pHands[0].pinky.bones[2].prev_joint.z, frame->pHands[0].pinky.bones[3].prev_joint.x, frame->pHands[0].pinky.bones[3].prev_joint.y, frame->pHands[0].pinky.bones[3].prev_joint.z);
            fclose(fptr);
          }
        } else {
          printf("Could not read frame: %s\n", ResultString(result));
        }
      }else{
        printf("No more frames.\n");
        break;
      }
    }
    result = LeapRecordingClose(&recordingHandle);
    if(!LEAP_SUCCEEDED(result))
      printf("Failed to close recording: %s\n", ResultString(result));
  } else {
    printf("Failed to open recording for reading: %s\n", ResultString(result));
  }
}

int main(int argc, char** argv) {
  char* recordingFileName;
  printf("Enter the recording file to read: ");
  scanf("%s", recordingFileName);
  for(int i = 1; i < 2; i++){
    //create a copy of recording file name
    char* recordingFileNameCopy = (char*)malloc(sizeof(char)*(strlen(recordingFileName)+1));
    strcpy(recordingFileNameCopy, recordingFileName);
    // make a new char called recordingFile that is the concatenation of recordingFileName and i
    // char recordingFile[100];
    // sprintf(recordingFile, "%s%d%s", "participant", i,  recordingFileNameCopy);
    // printf("%s\n", recordingFile);
    openFile(recordingFileNameCopy);
    // Open the recording for reading
    // params.mode = eLeapRecordingFlags_Reading;
    // eLeapRS result = LeapRecordingOpen(&recordingHandle, recordingFile, params);
    // if(LEAP_SUCCEEDED(result)){
    //   LEAP_TRACKING_EVENT *frame = 0;
    //   LEAP_RECORDING_STATUS *status = 0;
    //   while(true){
    //     uint64_t nextFrameSize = 0;
    //     result = LeapRecordingReadSize(recordingHandle, &nextFrameSize);
    //     if(!LEAP_SUCCEEDED(result))
    //       printf("Couldn't get next frame size: %s\n", ResultString(result));
    //     if(nextFrameSize > 0){
    //       frame = (LEAP_TRACKING_EVENT *)malloc((size_t)nextFrameSize);
    //       result = LeapRecordingRead(recordingHandle, frame, nextFrameSize);
    //       if(LEAP_SUCCEEDED(result)){
    //         //print every 20th frame
    //         if(frame->tracking_frame_id % 100 == 0){
    //           printf("Read frame %"PRIu64" with %i hands at timestamp %" PRId64 "\n", frame->tracking_frame_id, frame->nHands, frame->info.timestamp);
    //         }
    //         if(frame->nHands > 0){
    //           // make a string that is the concatenation of "data" and i
    //           char dataFile[100];
    //           sprintf(dataFile, "%s%s", recordingFile, ".csv");

    //           FILE *fptr = fopen(dataFile, "a");
    //           //index distal phalange, thumb distal phalange
    //           fprintf(fptr, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", frame->pHands[0].index.bones[0].prev_joint.x, frame->pHands[0].index.bones[0].prev_joint.y, frame->pHands[0].index.bones[0].prev_joint.z, frame->pHands[0].index.bones[1].prev_joint.x, frame->pHands[0].index.bones[1].prev_joint.y, frame->pHands[0].index.bones[1].prev_joint.z, frame->pHands[0].index.bones[2].prev_joint.x, frame->pHands[0].index.bones[2].prev_joint.y, frame->pHands[0].index.bones[2].prev_joint.z, frame->pHands[0].index.bones[3].prev_joint.x, frame->pHands[0].index.bones[3].prev_joint.y, frame->pHands[0].index.bones[3].prev_joint.z,     frame->pHands[0].thumb.bones[0].prev_joint.x, frame->pHands[0].thumb.bones[0].prev_joint.y, frame->pHands[0].thumb.bones[0].prev_joint.z, frame->pHands[0].thumb.bones[1].prev_joint.x, frame->pHands[0].thumb.bones[1].prev_joint.y, frame->pHands[0].thumb.bones[1].prev_joint.z, frame->pHands[0].thumb.bones[2].prev_joint.x, frame->pHands[0].thumb.bones[2].prev_joint.y, frame->pHands[0].thumb.bones[2].prev_joint.z, frame->pHands[0].thumb.bones[3].prev_joint.x, frame->pHands[0].thumb.bones[3].prev_joint.y, frame->pHands[0].thumb.bones[3].prev_joint.z,                frame->pHands[0].middle.bones[0].prev_joint.x, frame->pHands[0].middle.bones[0].prev_joint.y, frame->pHands[0].middle.bones[0].prev_joint.z, frame->pHands[0].middle.bones[1].prev_joint.x, frame->pHands[0].middle.bones[1].prev_joint.y, frame->pHands[0].middle.bones[1].prev_joint.z, frame->pHands[0].middle.bones[2].prev_joint.x, frame->pHands[0].middle.bones[2].prev_joint.y, frame->pHands[0].middle.bones[2].prev_joint.z, frame->pHands[0].middle.bones[3].prev_joint.x, frame->pHands[0].middle.bones[3].prev_joint.y, frame->pHands[0].middle.bones[3].prev_joint.z,          frame->pHands[0].ring.bones[0].prev_joint.x, frame->pHands[0].ring.bones[0].prev_joint.y, frame->pHands[0].ring.bones[0].prev_joint.z, frame->pHands[0].ring.bones[1].prev_joint.x, frame->pHands[0].ring.bones[1].prev_joint.y, frame->pHands[0].ring.bones[1].prev_joint.z, frame->pHands[0].ring.bones[2].prev_joint.x, frame->pHands[0].ring.bones[2].prev_joint.y, frame->pHands[0].ring.bones[2].prev_joint.z, frame->pHands[0].ring.bones[3].prev_joint.x, frame->pHands[0].ring.bones[3].prev_joint.y, frame->pHands[0].ring.bones[3].prev_joint.z,         frame->pHands[0].pinky.bones[0].prev_joint.x, frame->pHands[0].pinky.bones[0].prev_joint.y, frame->pHands[0].pinky.bones[0].prev_joint.z, frame->pHands[0].pinky.bones[1].prev_joint.x, frame->pHands[0].pinky.bones[1].prev_joint.y, frame->pHands[0].pinky.bones[1].prev_joint.z, frame->pHands[0].pinky.bones[2].prev_joint.x, frame->pHands[0].pinky.bones[2].prev_joint.y, frame->pHands[0].pinky.bones[2].prev_joint.z, frame->pHands[0].pinky.bones[3].prev_joint.x, frame->pHands[0].pinky.bones[3].prev_joint.y, frame->pHands[0].pinky.bones[3].prev_joint.z);
    //           fclose(fptr);
    //         }
    //       } else {
    //         printf("Could not read frame: %s\n", ResultString(result));
    //       }
    //     }else{
    //       printf("No more frames.\n");
    //       break;
    //     }
    //   }
    //   result = LeapRecordingClose(&recordingHandle);
    //   if(!LEAP_SUCCEEDED(result))
    //     printf("Failed to close recording: %s\n", ResultString(result));
    // } else {
    //   printf("Failed to open recording for reading: %s\n", ResultString(result));
    // }
  }
  printf("Press any key to continue.\n");
  getch();
  return 0;
}
