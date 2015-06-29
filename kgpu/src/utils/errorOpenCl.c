#include <stdio.h>
#include <CL/cl.h>

#include "utils/error.h"

static ERROR errorMessages[] =
{
  {NO_SPACE_ON_HOST, "No space on host for allocate the basic structure."},
  {CL_INVALID_PLATFORM, "Not a valid platform."},
  {CL_DEVICE_NOT_FOUND, "No OpenCL devices that matched device were found"},
  {CL_INVALID_DEVICE_TYPE, "Is not a valid value."},
  {CL_INVALID_CONTEXT, "The context is invalid."},
  {CL_INVALID_VALUE, "Invalid value."},
  {CL_INVALID_COMMAND_QUEUE, "Not a valid command queue"},
  {CL_OUT_OF_RESOURCES, "Failure to allocate resources required by the OpenCL"
                        "implementation on the device"},
  {CL_INVALID_BUFFER_SIZE, "Invalid buffer size."},
  {CL_INVALID_HOST_PTR, "Invalid host pointer."},
  {CL_INVALID_MEM_OBJECT, "Not a valid memory object."},
  {CL_MEM_OBJECT_ALLOCATION_FAILURE, "Problem when try to allocate memory."},
  {CL_OUT_OF_HOST_MEMORY, "Out of host memory."},
  {0,0}
};

int printErrorMessage(int pCode)
{
  int i = 0;
  for (i = 0; errorMessages[i].errorMessage; i++)
  {
    if (errorMessages[i].errorCode == pCode)
    {
      fprintf(stderr, " %s (%d) \n", errorMessages[i].errorMessage, pCode);
    }
  }
}
