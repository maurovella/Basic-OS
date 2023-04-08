#ifndef _ERRORS_H_
#define _ERRORS_H_

/* ----- ERROR CODES ----- */
#define ERR_CLOSE_FD 1
#define ERR_PIPE 2
#define ERR_DUP_FD 3

#define NO_FILES_FOUND 4
#define ERR_CREATING_FILE 5
#define ERR_CLOSE_FILE 6

#define ERR_CREATING_SHM 7
#define ERR_MAPPING_SHM 8
#define ERR_TRUNCATING_SHM 9
#define ERR_OPENING_SHM 10
#define ERR_WRITING_SHM 11
#define ERR_UNMAPPING_SHM 12
#define ERR_CLOSING_SHM 13
#define ERR_UNLINKING_SHM 14
#endif