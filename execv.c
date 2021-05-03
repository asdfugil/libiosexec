#include "utils.h"
#include <sys/syslimits.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libiosexec.h"

#define STRINGS_ARE_NOT_EQUAL(x, y, len) strncmp(x, y, len)

extern char** environ;

static inline int has_non_printable_char(char* str, size_t n) {
    for (int i = 0; i < n; i++) {
        if (!isprint(str[i])) {
                return 1;
        }
    }

    return 0;
} 

int ie_execve(const char* path, char* const argv[], char* const envp[]) {
    execve(path, argv, envp);
    int execve_ret = errno;

    if (execve_ret != EPERM) {
        return -1;
    }

    char* first_line = NULL;
    size_t read_amount = 0;
    
    FILE* ptr = fopen(path, "r");
    if (!ptr) {
        return -1;
    }

    // If the line cannot be retrieved/is empty return the errno value/ENOEXEC
    if (getline(&first_line, &read_amount, ptr) == 0) {
        return -1;
    }

    size_t first_line_len = strlen(first_line);

    // Strip the newline off the end of the string.
    first_line[first_line_len - 1] = '\0';
    first_line_len--;

    if (STRINGS_ARE_NOT_EQUAL("#!", first_line, 2)) {
        return -1;
    }

    if (has_non_printable_char(first_line, first_line_len)) {
        return -1;
    }

    // Remove the shebang from the line, for parsing. 
    char* freeme = first_line;
    first_line += 2;
    
    char* state;
    char* token = strtok_r(first_line, " ", &state);
    char* interp = token;
    char* arg_to_interpreter = strtok_r(NULL, "", &state);

    char* argv_new[1024];
    int offset = 0; 

    argv_new[0] = interp;
    if (arg_to_interpreter != NULL) {
        argv_new[1] = arg_to_interpreter;
        offset++;
    }

    size_t argcount = 0;
    while(argv[argcount]) argcount++;

    for (int i = 0; i < argcount; i++) {
        argv_new[offset + i + 1] = argv[i];
    }
    argv_new[offset + argcount + 1] = NULL;

    int ret = execve(interp, argv_new, envp);
    free(freeme);
    return ret;
}

int ie_execv(const char *path, char *const argv[]) {
    return ie_execve(path, argv, environ);
}

int ie_execvpe(const char *file, char *const argv[], char *const envp[])
{
	const char *p, *z, *path = getenv("PATH");
	size_t l, k;
	int seen_eacces = 0;

	errno = ENOENT;
	if (!*file) return -1;

	if (strchr(file, '/'))
		return ie_execve(file, argv, envp);

	if (!path) path = "/usr/local/bin:/bin:/usr/bin";
	k = strnlen(file, NAME_MAX+1);
	if (k > NAME_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}
	l = strnlen(path, PATH_MAX-1)+1;

	for(p=path; ; p=z) {
		char b[l+k+1];
		z = __strchrnul(p, ':');
		if (z-p >= l) {
			if (!*z++) break;
			continue;
		}
		memcpy(b, p, z-p);
		b[z-p] = '/';
		memcpy(b+(z-p)+(z>p), file, k+1);
		ie_execve(b, argv, envp);
		switch (errno) {
		case EACCES:
			seen_eacces = 1;
		case ENOENT:
		case ENOTDIR:
			break;
		default:
			return -1;
		}
		if (!*z++) break;
	}
	if (seen_eacces) errno = EACCES;
	return -1;
}

int ie_execvp(const char *file, char *const argv[])
{
	return ie_execvpe(file, argv, environ);
}