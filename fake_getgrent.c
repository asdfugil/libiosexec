#include <grp.h>

#include "libiosexec.h"

struct group *
ie_getgrgid(gid_t gid) {
	return getgrgid(gid);
}
DYLD_INTERPOSE(ie_getgrgid, getgrgid);

int
ie_getgrgid_r(gid_t gid, struct group *grp, char *buffer, size_t bufsize, struct group **result) {
	return getgrgid_r(gid, grp, buffer, bufsize, result);
}
DYLD_INTERPOSE(ie_getgrgid_r, getgrgid_r);

struct group *
ie_getgrnam(const char *name) {
	return getgrnam(name);
}
DYLD_INTERPOSE(ie_getgrnam, getgrnam);

int
ie_getgrnam_r(const char *name, struct group *grp, char *buffer, size_t bufsize, struct group **result) {
	return getgrnam_r(name, grp, buffer, bufsize, result);
}
DYLD_INTERPOSE(ie_getgrnam_r, getgrnam_r);

struct group *
ie_getgrent(void) {
	return getgrent();
}
DYLD_INTERPOSE(ie_getgrent, getgrent);

void
ie_setgrent(void) {
	return setgrent();
}
DYLD_INTERPOSE(ie_setgrent, setgrent);

void
ie_endgrent(void) {
	return endgrent();
}
DYLD_INTERPOSE(ie_endgrent, endgrent);

char *
ie_group_from_gid(gid_t group, int nogroup) {
	return group_from_gid(group, nogroup);
}
DYLD_INTERPOSE(ie_group_from_gid, group_from_gid);
