#ifndef IOG_RETURN_CODES_H
#define IOG_RETURN_CODES_H

/* @file iog_return_codes.h */

/* @enum IoReturnCode
 * Defines error code of function
 */
enum IogReturnCode {
  OK                         = 0, ///< All ok

  ERR_ALREADY_INITIALIZED    = 1,
  ERR_ISNT_INITIALIZED       = 2,

  ERR_CANT_ALLOCATE_DATA     = 3,
  ERR_CANT_FREE_DATA         = 4,

  ERR_UNDERFLOW              = 5,
  ERR_OVERFLOW               = 6,
  ERR_CAPACITY_UNDERFLOW     = 7,

  ERR_NULLPTR                = 8,
  ERR_DATA_NULLPTR           = 9,

  ERR_TEST_FAILED            = 10,

  NR_RETURN_CODE             = 11, ///< Last return code
};

#endif // IOG_RETURN_CODES_H
