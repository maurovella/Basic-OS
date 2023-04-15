#ifndef _ERRORS_H_
#define _ERRORS_H_

/* ----- ERROR CODES ----- */
#define ERR_CLOSING_FD       1
#define ERR_CREATING_PIPE    2
#define ERR_DUPLICATING_FD   3
#define ERR_SELECTING_FD     4
#define ERR_READING_FD       5
#define ERR_WRITING_FD       6

#define ERR_NO_FILES_FOUND   7
#define ERR_CREATING_FILE    8
#define ERR_CLOSE_FILE       9

#define ERR_CREATING_SHM     10
#define ERR_MAPPING_SHM      11
#define ERR_TRUNCATING_SHM   12
#define ERR_OPENING_SHM      13
#define ERR_WRITING_SHM      14
#define ERR_UNMAPPING_SHM    15
#define ERR_CLOSING_SHM      16
#define ERR_UNLINKING_SHM    17

#define ERR_CREATING_SEM     18
#define ERR_OPENING_SEM      19
#define ERR_POSTING_SEM      20
#define ERR_WAITING_SEM      21
#define ERR_CLOSING_SEM      22
#define ERR_UNLINKING_SEM    23

#define ERR_CREATING_SLAVE   24
#define ERR_KILLING_PROCESS  25

#endif
