#include <pwd.h>

#include "libiosexec.h"

struct passwd *
ie_getpwent(void) {
	return getpwent();
}
DYLD_INTERPOSE(ie_getpwent, getpwent);

struct passwd *
ie_getpwnam(const char *name) {
	return getpwnam(name);
}
DYLD_INTERPOSE(ie_getpwnam, getpwnam);

int
ie_getpwnam_r(const char *name, struct passwd *pw, char *buf, size_t buflen, struct passwd **pwretp) {
	return getpwnam_r(name, pw, buf, buflen, pwretp);
}
DYLD_INTERPOSE(ie_getpwnam_r, getpwnam_r);

struct passwd *
ie_getpwuid(uid_t uid) {
	return getpwuid(uid);
}
DYLD_INTERPOSE(ie_getpwuid, getpwuid);

int
ie_getpwuid_r(uid_t uid, struct passwd *pw, char *buf, size_t buflen, struct passwd **pwretp) {
	return getpwuid_r(uid, pw, buf, buflen, pwretp);
}
DYLD_INTERPOSE(ie_getpwuid_r, getpwuid_r);

int
ie_setpassent(int stayopen) {
	return setpassent(stayopen);
}
DYLD_INTERPOSE(ie_setpassent, setpassent);

void
ie_setpwent(void) {
	return setpwent();
}
DYLD_INTERPOSE(ie_setpwent, setpwent);

void
ie_endpwent(void) {
	return endpwent();
}
DYLD_INTERPOSE(ie_endpwent, endpwent);

char *
ie_user_from_uid(uid_t uid, int nouser) {
	return user_from_uid(uid, nouser);
}
DYLD_INTERPOSE(ie_user_from_uid, user_from_uid);
