#include <stdio.h>
#include <CL/cl.h>

#include "utils/error.h"

static ERROR errorMessages[] =
{
  {CL_INVALID_CONTEXT, ""},
  {CL_INVALID_VALUE, ""},
  {CL_INVALID_BUFFER_SIZE, ""},
  {CL_INVALID_HOST_PTR, ""},
  {CL_MEM_OBJECT_ALLOCATION_FAILURE, ""},
  {CL_OUT_OF_HOST_MEMORY, ""},
  {0,0}
};

int printErrorMessage(int pCode)
{
  int i = 0;
  for (i = 0; errorMessages[i].errorMessage; i++)
  {
    if (errorMessages[i].code == pCode)
    {
      fprintf(stderr, " %s (%d) \n", errorMessages[i].errorMessage, pCode);
    }
  }
}
