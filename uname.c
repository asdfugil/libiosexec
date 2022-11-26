/* Copyright (c) 2012, 2012 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#include <string.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include "libiosexec.h"

int
ie_uname(struct utsname* name)
{
	int mib[2], rval;
	size_t len;
	char *p;

	rval = 0;

	mib[0] = CTL_KERN;
	mib[1] = KERN_OSTYPE;
	len = sizeof(name->sysname);
	if (sysctl(mib, 2, &name->sysname, &len, NULL, 0) == -1)
		rval = -1;

	mib[0] = CTL_KERN;
	mib[1] = KERN_HOSTNAME;
	len = sizeof(name->nodename);
	if (sysctl(mib, 2, &name->nodename, &len, NULL, 0) == -1)
		rval = -1;

	mib[0] = CTL_KERN;
	mib[1] = KERN_OSRELEASE;
	len = sizeof(name->release);
	if (sysctl(mib, 2, &name->release, &len, NULL, 0) == -1)
		rval = -1;

	/* The version may have newlines in it, turn them into spaces. */
	mib[0] = CTL_KERN;
	mib[1] = KERN_VERSION;
	len = sizeof(name->version);
	if (sysctl(mib, 2, &name->version, &len, NULL, 0) == -1)
		rval = -1;
	else
		for (p = name->version; len--; ++p)
			if (*p == '\n' || *p == '\t') {
				if (len > 1) {
					*p = ' ';
				} else {
					*p = '\0';
				}
			}
	mib[0] = CTL_HW;
#if defined(__arm64__) && defined(__APPLE__)
	char *ie_machine = "arm64";
	strlcpy((char *)&name->machine, ie_machine, _SYS_NAMELEN);
#else
	mib[1] = HW_MACHINE;
	len = sizeof(name->machine);
	if (sysctl(mib, 2, &name->machine, &len, NULL, 0) == -1)
		rval = -1;
#endif
	return (rval);
}
DYLD_INTERPOSE(ie_uname, uname);
