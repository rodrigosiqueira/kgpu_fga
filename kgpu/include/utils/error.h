/**
* @file error.h
*/

#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum
{
  NO_SPACE_ON_HOST = -999
}ERROR_MESSAGE;

/**
* @struct ERROR
* @brief Keep the error code and message.
*/
typedef struct _ERROR
{
  int errorCode;
  char * errorMessage;
}ERROR;

extern int printErrorMessage(int pCode);

#endif
