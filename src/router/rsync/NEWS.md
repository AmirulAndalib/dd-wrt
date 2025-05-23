# NEWS for rsync 3.4.1 (16 Jan 2025)

Release 3.4.1 is a fix for regressions introduced in 3.4.0

## Changes in this version:

### BUG FIXES:

 - fixed handling of -H flag with conflict in internal flag values

 - fixed a user after free in logging of failed rename

 - fixed build on systems without openat()

 - removed dependency on alloca() in bundled popt

### DEVELOPER RELATED:

 - fix to permissions handling in the developer release script

------------------------------------------------------------------------------
# NEWS for rsync 3.4.0 (15 Jan 2025)

Release 3.4.0 is a security release that fixes a number of important vulnerabilities.

For more details on the vulnerabilities please see the CERT report
https://kb.cert.org/vuls/id/952657

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 32 to make it easier for
   administrators to check their servers have been updated

### SECURITY FIXES:

Many thanks to Simon Scannell, Pedro Gallegos, and Jasiel Spelman at
Google Cloud Vulnerability Research and Aleksei Gorban (Loqpa) for
discovering these vulnerabilities and working with the rsync project
to develop and test fixes.

- CVE-2024-12084 - Heap Buffer Overflow in Checksum Parsing.

- CVE-2024-12085 - Info Leak via uninitialized Stack contents defeats ASLR.
  
- CVE-2024-12086 - Server leaks arbitrary client files.

- CVE-2024-12087 - Server can make client write files outside of destination directory using symbolic links.

- CVE-2024-12088 - --safe-links Bypass.

- CVE-2024-12747 - symlink race condition.

### BUG FIXES:

- Fixed the included popt to avoid a memory error on modern gcc versions.

- Fixed an incorrect extern variable's type that caused an ACL issue on macOS.

- Fixed IPv6 configure check

### INTERNAL:

- Updated included popt to version 1.19.

### DEVELOPER RELATED:

- Various improvements to the release scripts and git setup.

- Improved packaging/var-checker to identify variable type issues.

- added FreeBSD and Solaris CI builds

------------------------------------------------------------------------------
# NEWS for rsync 3.3.0 (6 Apr 2024)

## Changes in this version:

### BUG FIXES:

- Fixed a bug with `--sparse --inplace` where a trailing gap in the source
  file would not clear out the trailing data in the destination file.

- Fixed an buffer overflow in the checksum2 code if SHA1 is being used for
  the checksum2 algorithm.

- Fixed an issue when rsync is compiled using `_FORTIFY_SOURCE` so that the
  extra tests don't complain about a strlcpy() limit value (which was too
  large, even though it wasn't possible for the larger value to cause an
  overflow).

- Add a backtick to the list of characters that the filename quoting needs to
  escape using backslashes.

- Fixed a string-comparison issue in the internal handling of `--progress` (a
  locale such as tr_TR.utf-8 needed the internal triggering of `--info` options
  to use upper-case flag names to ensure that they match).

- Make sure that a local transfer marks the sender side as trusted.

- Change the argv handling to work with a newer popt library -- one that likes
  to free more data than it used to.

- Rsync now calls `OpenSSL_add_all_algorithms()` when compiled against an older
  openssl library.

- Fixed a problem in the daemon auth for older protocols (29 and before) if the
  openssl library is being used to compute MD4 checksums.

- Fixed `rsync -VV` on Cygwin -- it needed a flush of stdout.

- Fixed an old stats bug that counted devices as symlinks.

### ENHANCEMENTS:

- Enhanced rrsync with the `-no-overwrite` option that allows you to ensure
  that existing files on your restricted but writable directory can't be
  modified.

- Enhanced the manpages to mark links with .UR & .UE. If your nroff doesn't
  support these idioms, touch the file `.md2man-force` in the source directory
  so that `md-convert` gets called with the `--force-link-text` option, and
  that should ensure that your manpages are still readable even with the
  ignored markup.

- Some manpage improvements on the handling of [global] modules.

- Changed the mapfrom & mapto perl scripts (in the support dir) into a single
  python script named idmap.  Converted a couple more perl scripts into python.

- Changed the mnt-excl perl script (in the support dir) into a python script.

### DEVELOPER RELATED:

 - Updated config.guess (timestamp 2023-01-01) and config.sub (timestamp
   2023-01-21).

------------------------------------------------------------------------------

# NEWS for rsync 3.2.7 (20 Oct 2022)

## Changes in this version:

### BUG FIXES:

- Fixed the client-side validating of the remote sender's filtering behavior.

- More fixes for the "unrequested file-list name" name, including a copy of
  "/" with `--relative` enabled and a copy with a lot of related paths with
  `--relative` enabled (often derived from a `--files-from` list).

- When rsync gets an unpack error on an ACL, mention the filename.

- Avoid over-setting sanitize_paths when a daemon is serving "/" (even if
  "use chroot" is false).

### ENHANCEMENTS:

- Added negotiated daemon-auth support that allows a stronger checksum digest
  to be used to validate a user's login to the daemon.  Added SHA512, SHA256,
  and SHA1 digests to MD5 & MD4.  These new digests are at the highest priority
  in the new daemon-auth negotiation list.

- Added support for the SHA1 digest in file checksums.  While this tends to be
  overkill, it is available if someone really needs it.  This overly-long
  checksum is at the lowest priority in the normal checksum negotiation list.
  See [`--checksum-choice`](rsync.1#opt) (`--cc`) and the `RSYNC_CHECKSUM_LIST`
  environment var for how to customize this.

- Improved the xattr hash table to use a 64-bit key without slowing down the
  key's computation.  This should make extra sure that a hash collision doesn't
  happen.

- If the `--version` option is repeated (e.g. `-VV`) then the information is
  output in a (still readable) JSON format.  Client side only.

- The script `support/json-rsync-version` is available to get the JSON style
  version output from any rsync.  The script accepts either text on stdin
  **or** an arg that specifies an rsync executable to run with a doubled
  `--version` option.  If the text we get isn't already in JSON format, it is
  converted. Newer rsync versions will provide more complete json info than
  older rsync versions. Various tweaks are made to keep the flag names
  consistent across versions.

- The [`use chroot`](rsyncd.conf.5#) daemon parameter now defaults to "unset"
  so that rsync can use chroot when it works and a sanitized copy when chroot
  is not supported (e.g., for a non-root daemon).  Explicitly setting the
  parameter to true or false (on or off) behaves the same way as before.

- The `--fuzzy` option was optimized a bit to try to cut down on the amount of
  computations when considering a big pool of files. The simple heuristic from
  Kenneth Finnegan resulted in about a 2x speedup.

- If rsync is forced to use protocol 29 or before (perhaps due to talking to an
  rsync before 3.0.0), the modify time of a file is limited to 4-bytes.  Rsync
  now interprets this value as an unsigned integer so that a current year past
  2038 can continue to be represented. This does mean that years prior to 1970
  cannot be represented in an older protocol, but this trade-off seems like the
  right choice given that (1) 2038 is very rapidly approaching, and (2) newer
  protocols support a much wider range of old and new dates.

- The rsync client now treats an empty destination arg as an error, just like
  it does for an empty source arg. This doesn't affect a `host:` arg (which is
  treated the same as `host:.`) since the arg is not completely empty.  The use
  of [`--old-args`](rsync.1#opt) (including via `RSYNC_OLD_ARGS`) allows the
  prior behavior of treating an empty destination arg as a ".".

### PACKAGING RELATED:

- The checksum code now uses openssl's EVP methods, which gets rid of various
  deprecation warnings and makes it easy to support more digest methods.  On
  newer systems, the MD4 digest is marked as legacy in the openssl code, which
  makes openssl refuse to support it via EVP.  You can choose to ignore this
  and allow rsync's MD4 code to be used for older rsync connections (when
  talking to an rsync prior to 3.0.0) or you can choose to configure rsync to
  tell openssl to enable legacy algorithms (see below).

- A simple openssl config file is supplied that can be installed for rsync to
  use.  If you install packaging/openssl-rsync.cnf to a public spot (such as
  `/etc/ssl/openssl-rsync.cnf`) and then run configure with the option
  `--with-openssl-conf=/path/name.cnf`, this will cause rsync to export the
  configured path in the OPENSSL_CONF environment variable (when the variable
  is not already set).  This will enable openssl's MD4 code for rsync to use.

- The packager may wish to include an explicit "use chroot = true" in the top
  section of their supplied /etc/rsyncd.conf file if the daemon is being
  installed to run as the root user (though rsync should behave the same even
  with the value unset, a little extra paranoia doesn't hurt).

- I've noticed that some packagers haven't installed support/nameconvert for
  users to use in their chrooted rsync configs.  Even if it is not installed
  as an executable script (to avoid a python3 dependency) it would be good to
  install it with the other rsync-related support scripts.

- It would be good to add support/json-rsync-version to the list of installed
  support scripts.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.6 (9 Sep 2022)

## Changes in this version:

### BUG FIXES:

- More path-cleaning improvements in the file-list validation code to avoid
  rejecting of valid args.

- A file-list validation fix for a [`--files-from`](rsync.1#opt) file that ends
  without a line-terminating character.

- Added a safety check that prevents the sender from removing destination files
  when a local copy using [`--remove-source-files`](rsync.1#opt) has some files
  that are shared between the sending & receiving hierarchies, including the
  case where the source dir & destination dir are identical.

- Fixed a bug in the internal MD4 checksum code that could cause the digest
  to be sporadically incorrect (the openssl version was/is fine).

- A minor tweak to rrsync added "copy-devices" to the list of known args, but
  left it disabled by default.

### ENHANCEMENTS:

- Rename `--protect-args` to [`--secluded-args`](rsync.1#opt) to make it
  clearer how it differs from the default backslash-escaped arg-protecting
  behavior of rsync.  The old option names are still accepted.  The
  environment-variable override did not change its name.

### PACKAGING RELATED:

- The configure option `--with-protected-args` was renamed to
  `--with-secluded-args`.  This option makes `--secluded-args` the default
  rsync behavior instead of using backslash escaping for protecting args.

- The mkgitver script now makes sure that a `.git` dir/file is in the top-level
  source dir before calling `git describe`. It also runs a basic check on the
  version value. This should avoid using an unrelated git description for
  rsync's version.

### DEVELOPER RELATED:

- The configure script no longer sets the -pedantic-errors CFLAG (which it
  used to try to do only for gcc).

- The name_num_obj struct was modified to allow its dynamic name_num_item list
  to be initialized in a better way.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.5 (14 Aug 2022)

## Changes in this version:

### SECURITY FIXES:

- Added some file-list safety checking that helps to ensure that a rogue
  sending rsync can't add unrequested top-level names and/or include recursive
  names that should have been excluded by the sender.  These extra safety
  checks only require the receiver rsync to be updated.  When dealing with an
  untrusted sending host, it is safest to copy into a dedicated destination
  directory for the remote content (i.e. don't copy into a destination
  directory that contains files that aren't from the remote host unless you
  trust the remote host). Fixes CVE-2022-29154.

 - A fix for CVE-2022-37434 in the bundled zlib (buffer overflow issue).

### BUG FIXES:

- Fixed the handling of filenames specified with backslash-quoted wildcards
  when the default remote-arg-escaping is enabled.

- Fixed the configure check for signed char that was causing a host that
  defaults to unsigned characters to generate bogus rolling checksums. This
  made rsync send mostly literal data for a copy instead of finding matching
  data in the receiver's basis file (for a file that contains high-bit
  characters).

- Lots of manpage improvements, including an attempt to better describe how
  include/exclude filters work.

- If rsync is compiled with an xxhash 0.8 library and then moved to a system
  with a dynamically linked xxhash 0.7 library, we now detect this and disable
  the XX3 hashes (since these routines didn't stabilize until 0.8).

### ENHANCEMENTS:

- The [`--trust-sender`](rsync.1#opt) option was added as a way to bypass the
  extra file-list safety checking (should that be required).

### PACKAGING RELATED:

- A note to those wanting to patch older rsync versions: the changes in this
  release requires the quoted argument change from 3.2.4. Then, you'll want
  every single code change from 3.2.5 since there is no fluff in this release.

- The build date that goes into the manpages is now based on the developer's
  release date, not on the build's local-timezone interpretation of the date.

### DEVELOPER RELATED:

- Configure now defaults GETGROUPS_T to gid_t when cross compiling.

- Configure now looks for the bsd/string.h include file in order to fix the
  build on a host that has strlcpy() in the main libc but not defined in the
  main string.h file.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.4 (15 Apr 2022)

## Changes in this version:

### BEHAVIOR CHANGES:

 - A new form of arg protection was added that works similarly to the older
   `--protect-args` ([`-s`](rsync.1#opt)) option but in a way that avoids
   breaking things like rrsync (the restricted rsync script): rsync now uses
   backslash escaping for sending "shell-active" characters to the remote shell
   (such as `$(){}<>#&` and others). This includes spaces, so fetching a remote
   file via a quoted filename value now works by default without any extra
   quoting:

   ```shell
       rsync -aiv host:'a simple file.pdf' .
   ```

   Wildcards are not escaped in filename args, but they are escaped in options
   like the [`--suffix`](rsync.1#opt) and [`--usermap`](rsync.1#opt) values.

   If a script depends on the old arg behavior (perhaps because it quotes or
   protects the args already, or perhaps because it expects arg splitting),
   there are two easy ways to get things going with a modern rsync: either
   `export RSYNC_OLD_ARGS=1` in the script's environment (perhaps in the script
   itself) or add the option [`--old-args`](rsync.1#opt) to the rsync commands
   that are run.  See also the [ADVANCED USAGE](rsync.1#) section of rsync's
   manpage for how to use a more modern arg style.

 - A long-standing bug was preventing rsync from figuring out the current
   locale's decimal point character, which made rsync always output numbers
   using the "C" locale.  Since this is now fixed in 3.2.4, a script that
   parses rsync's decimal numbers (e.g. from the verbose footer) may want to
   setup the environment in a way that the output continues to be in the C
   locale.  For instance, one of the following should work fine:

   ```shell
       export LC_ALL=C.UTF-8
   ```

   or if iconv translations are needed:

   ```shell
       if [ "${LC_ALL:-}" ]; then
           export LANG="$LC_ALL"
           export LC_CTYPE="$LC_ALL"
           unset LC_ALL
       fi
       export LC_NUMERIC=C.UTF-8
   ```

### SECURITY FIXES:

 - A fix for CVE-2018-25032 in the bundled zlib (memory corruption issue).

### BUG FIXES:

 - Fixed a bug with [`--inplace`](rsync.1#opt) + [`--sparse`](rsync.1#opt) (and
   a lack of [`--whole-file`](rsync.1#opt)) where the destination file could
   get reconstructed with bogus data.  Since the bug can also be avoided by
   using (the seemingly redundant) [`--no-W`](rsync.1#opt) on the receiving
   side, the latest rsync will now send `--no-W` to a remote receiver when this
   option combination occurs.  If your client rsync is not new enough to do
   this for you (or if you're just paranoid), you can manually specify `--no-W
   -M--no-W` (when not using [`--whole-file`](rsync.1#opt)) to make sure the
   bug is avoided.

 - Fixed a bug with [`--mkpath`](rsync.1#opt) if a single-file copy specifies
   an existing destination dir with a non-existing destination filename.

 - Fixed `--update -vv` to output "is uptodate" instead of "is newer" messages
   for files that are being skipped due to an identical modify time.  (This was
   a new output quirk in 3.2.3.)

 - When doing an append transfer, the sending side's file must not get shorter
   or it is skipped. Fixes a crash that could occur when the size changes to 0
   in the middle of the send negotiations.

 - When dealing with special files (see [`--specials`](rsync.1#opt)) in an
   alt-dest hierarchy, rsync now checks the non-permission mode bits to ensure
   that the 2 special files are really the same before hard-linking them
   together.

 - Fixed a bug where [`--delay-updates`](rsync.1#opt) with stale partial data
   could cause a file to fail to update.

 - Fixed a few places that would output an INFO message with
   [`--info=NAME`](rsync.1#opt) that should only have been output given
   [`--verbose`](rsync.1#opt) or [`--itemize-changes`](rsync.1#opt).

 - Avoid a weird failure if you run a local copy with a (useless)
   [`--rsh`](rsync.1#opt) option that contains a `V` in the command.

 - Fixed a long-standing compression bug where the compression level of the
   first file transferred affected the level for all future files.  Also, the
   per-file compression skipping has apparently never worked, so it is now
   documented as being ineffective.

 - Fixed a truncate error when a `--write-devices` copy wrote a file onto a
   device that was shorter than the device.

 - Made `--write-devices` support both `--checksum` and `--no-whole-file` when
   copying to a device.

 - Improved how the [`--stop-at`](rsync.1#opt), [`--stop-after`](rsync.1#opt),
   and (the deprecated) [`--time-limit`](rsync.1#opt) options check to see if
   the allowed time is over, which should make rsync exit more consistently.

 - Tweak --progress to display "`??:??:??`" when the time-remaining value is so
   large as to be meaningless.

 - Silence some chmod warnings about symlinks when it looks like we have a
   function to set their permissions but they can't really be set.

 - Fixed a potential issue in git-set-file-times when handling commits with
   high-bit characters in the description & when handling a description that
   might mimic the git raw-commit deliniators.  (See the support dir.)

 - The bundled systemd/rsync.service file now includes `Restart=on-failure`.

### ENHANCEMENTS:

 - Use openssl's `-verify_hostname` option in the rsync-ssl script.

 - Added extra info to the "FILENAME exists" output of
   [`--ignore-existing`](rsync.1#opt) when [`--info=skip2`](rsync.1#opt) is
   used.  The skip message becomes "FILENAME exists (INFO)" where the INFO is
   one of "type change", "sum change" (requires [`--checksum`](rsync.1#opt)),
   "file change" (based on the quick check), "attr change", or "uptodate".
   Prior versions only supported `--info=skip1`.

 - Added the [`--fsync`](rsync.1#opt) option (promoted from the patches repo).

 - Added the [`--copy-devices`](rsync.1#opt) option.  Compared to the
   historical version from the rsync-patches repo, this version: properly
   handles `--checksum`; fixes a truncation bug when doing an `--inplace` copy
   onto a longer file; fixes several bugs in the `--itemize` output; and only
   the sending side needs the enhanced rsync for the copy to work.

 - Reduced memory usage for an incremental transfer that has a bunch of small
   directories.

 - The rsync daemon can now handle a client address with an implied "%scope"
   suffix.

 - Added support for [`--atimes`](rsync.1#opt) on macOS and fixed a bug where
   it wouldn't work without [`--times`](rsync.1#opt).

 - Rsync can now update the xattrs on a read-only file when your user can
   temporarily add user-write permission to the file. (It always worked for a
   root transfer.)

 - Rsync can now work around an [`--inplace`](rsync.1#opt) update of a file
   that is being refused due to the Linux fs.protected_regular sysctl setting.

 - When [`--chown`](rsync.1#opt), [`--usermap`](rsync.1#opt), or
   [`--groupmap`](rsync.1#opt) is specified, rsync now makes sure that the
   appropriate [`--owner`](rsync.1#opt) and/or [`--group`](rsync.1#opt) options
   are enabled.

 - Added the [`--info=NONREG`](rsync.1#opt) setting to control if rsync should
   warn about non-regular files in the transfer.  This is enabled by default
   (keeping the behavior the same as before), so specifying `--info=nonreg0`
   can be used to turn the warnings off.

 - An optional asm optimization for the rolling checksum from Shark64. Enable
   it with `./configure --enable-roll-asm`.

 - Using `--debug=FILTER` now outputs a caution message if a filter rule
   has trailing whitespace.

 - Transformed rrsync into a python script with improvements:
   - Security has been beefed up.
   - The known rsync options were updated to include recent additions.
   - Make rrsync reject [`--copy-links`](rsync.1#opt) (`-L`),
     [`--copy-dirlinks`](rsync.1#opt) (`-k`), &
     [`--keep-dirlinks`](rsync.1#opt) (`-K`) by default to make it harder to
     exploit any out-of-subdir symlinks.
   - A new rrsync option of [`-munge`](rrsync.1#opt) tells rrsync to always
     enable rsync's [`--munge-links`](rsync.1#opt) option on the server side.
   - A new rrsync option of [`-no-lock`](rrsync.1#opt) disables a new
     single-use locking idiom that is the default when [`-ro`](rrsync.1#opt) is
     not used (useful with [`-munge`](rrsync.1#opt)).
   - A new rrsync option of [`-no-del`](rrsync.1#opt) disables all `--remove*`
     and `--delete*` rsync options on the server side.
   - The log format has been tweaked slightly to add seconds to the timestamp
     and to output the command executed as a tuple (making the args clearer).
   - An rrsync.1 manpage was added (in the support dir with rrsync).

 - Added options to the lsh script to facilitate rrsync testing. (See the
   support dir.)

 - Transformed the atomic-rsync script into a python script and added the
   ability to ignore one or more non-zero exit codes.  By default, it now
   ignores code 24, the file-vanished exit code. (See the support dir.)

 - Transformed the munge-symlinks script into python. (See the support dir.)

 - Improved the rsync-no-vanished script to not join stdout & stderr together.
   (See the support dir.)

 - Work around a glibc bug where lchmod() breaks in a chroot w/o /proc mounted.

 - Try to support a client that sent a remote rsync a wacko stderr file handle
   (such as an older File::RsyncP perl library used by BackupPC).

 - Lots of manpage improvements, including better HTML versions.

### PACKAGING RELATED:

 - Give configure the `--with-rrsync` option if you want `make install` to
   install the (now python3) rrsync script and its new manpage.

 - If the rrsync script is installed, its package should be changed to depend
   on python3 and the (suggested but not mandatory) python3 braceexpand lib.

 - When creating a package from a non-release version (w/o a git checkout), the
   packager can elect to create git-version.h and define RSYNC_GITVER to the
   string they want `--version` to output.  (The file is still auto-generated
   using the output of `git describe` when building inside a non-shallow git
   checkout, though.)

 - Renamed configure's `--enable-simd` option to `--enable-roll-simd` and added
   the option `--enable-roll-asm` to use the new asm version of the code.  Both
   are x86_64/amd64 only.

 - Renamed configure's `--enable-asm` option to `--enable-md5-asm` to avoid
   confusion with the asm option for the rolling checksum.  It is also honored
   even when openssl crypto is in use.  This allows: normal MD4 & MD5, normal
   MD4 + asm MD5, openssl MD4 & MD5, or openssl MD4 + asm MD5 depending on the
   configure options selected.

 - Made SIMD & asm configure checks default to "no" on non-Linux hosts due to
   various reports of problems on NetBSD & macOS hosts.  These were also
   tweaked to allow enabling the feature on a host_cpu of amd64 (was only
   allowed on x86_64 before).

 - Fixed configure to not fail at the SIMD check when cross-compiling.

 - Improved the IPv6 determination in configure.

 - Compile the C files with `-pedantic-errors` (when possible) so that we will
   get warned if a static initialization overflows in the future (among other
   things).

 - When linking with an external zlib, rsync renames its `read_buf()` function
   to `read_buf_()` to avoid a symbol clash on an unpatched zlib.

 - Added a SECURITY.md file.

### DEVELOPER RELATED:

 - Made it easier to write rsync tests that diff the output while also checking
   the status code, and used the idiom to improve the existing tests. (See the
   `checkdiff` and `checkdiff2` idioms in the `testsuite/*.test` files.

 - The packaging scripts & related python lib got some minor enhancements.

### INTERNAL

 - Use setenv() instead of putenv() when it is available.

 - Improve the logic in compat.c so that we don't need to try to remember to
   sprinkle `!local_server` exceptions throughout the protocol logic.

 - One more C99 Flexible Array improvement (started in the last release) and
   make use of the C99 `%zd` format string when printing size_t values (when
   possible).

 - Use mallinfo2() instead of mallinfo(), when available.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.3 (6 Aug 2020)

## Changes in this version:

### BUG FIXES:

 - Fixed a bug in the xattr code that was freeing the wrong object when trying
   to cleanup the xattr list.

 - Fixed a bug in the xattr code that was not leaving room for the "rsync."
   prefix in some instances where it needed to be added.

 - Restored the ability to use [`--bwlimit=0`](rsync.1#opt) to specify no
   bandwidth limit.  (It was accidentally broken in 3.2.2.)

 - Fixed a bug when combining [`--delete-missing-args`](rsync.1#opt) with
   [`--no-implied-dirs`](rsync.1#opt) & [`-R`](rsync.1#opt) where rsync might
   create the destination path of a missing arg.  The code also avoids some
   superfluous warnings for nested paths of removed args.

 - Fixed an issue where hard-linked devices could cause the rdev_major value to
   get out of sync between the sender and the receiver, which could cause a
   device to get created with the wrong major value in its major,minor pair.

 - Rsync now complains about a missing [`--temp-dir`](rsync.1#opt) before
   starting any file transfers.

 - A completely empty source arg is now a fatal error.  This doesn't change
   the handling of implied dot-dir args such as "localhost:" and such.

### ENHANCEMENTS:

 - Allow [`--max-alloc=0`](rsync.1#opt) to specify no limit to the alloc sanity
   check.

 - Allow [`--block-size=SIZE`](rsync.1#opt) to specify the size using units
   (e.g. "100K").

 - The name of the id-0 user & group are now sent to the receiver along with
   the other user/group names in the transfer (instead of assuming that both
   sides have the same id-0 names).

 - Added the [`--stop-after`](rsync.1#opt) and [`--stop-at`](rsync.1#opt)
   options (with a [`--time-limit`](rsync.1#opt) alias for `--stop-after`).
   This is an enhanced version of the time-limit patch from the patches repo.

 - Added the [`name converter`](rsyncd.conf.5#opt) daemon parameter to make it
   easier to convert user & group names inside a chrooted daemon module.  This
   is based on the nameconverter patch with some improvements, including a
   tweak to the request protocol (so if you used this patch in the past, be
   sure to update your converter script to use newlines instead of null chars).

 - Added [`--crtimes`](rsync.1#opt) (`-N`) option for preserving the file's
   create time (I believe that this is macOS only at the moment).

 - Added [`--mkpath`](rsync.1#opt) option to tell rsync that it should create a
   non-existing path component of the destination arg.

 - Added [`--stderr=errors|all|client`](rsync.1#opt) to replace the
   `--msgs2stderr` and `--no-msgs2stderr` options (which are still accepted).
   The default use of stderr was changed to be `--stderr=errors` where all the
   processes that have stderr available output directly to stderr, which should
   help error messages get to the user more quickly, especially when doing a
   push (which includes local copying).  This also allows rsync to exit quickly
   when a receiver failure occurs, since rsync doesn't need to try to keep the
   connection alive long enough for the fatal error to go from the receiver to
   the generator to the sender.  The old default can be requested via
   `--stderr=client`.  Also changed is that a non-default stderr mode is
   conveyed to the remote rsync (using the older option names) instead of
   requiring the user to use [`--remote-option`](rsync.1#opt) (`-M`) to tell
   the remote rsync what to do.

 - Added the ability to specify "@netgroup" names to the [`hosts
   allow`](rsyncd.conf.5#opt) and [`hosts deny`](rsyncd.conf.5#opt) daemon
   parameters.  This is a finalized version of the netgroup-auth patch from the
   patches repo.

 - Rsync can now hard-link symlinks on FreeBSD due to it making use of the
   linkat() function when it is available.

 - Output file+line info on out-of-memory & overflow errors while also avoiding
   the output of alternate build-dir path info that is not useful to the user.

 - Change configure to know that Cygwin supports Linux xattrs.

 - Improved the testsuite on FreeBSD & Cygwin.

 - Added some compatibility code for HPE NonStop platforms.

 - Improved the INSTALL.md info.

 - Added a few more suffixes to the default skip-compress list.

 - Improved configure's error handling to notify about several issues at once
   instead of one by one (for the newest optional features).

### INTERNAL:

 - Use a simpler overflow check idiom in a few spots.

 - Use a C99 Flexible Array for a trailing variable-size filename in a struct
   (with a fallback to the old 1-char string kluge for older compilers).

------------------------------------------------------------------------------

# NEWS for rsync 3.2.2 (4 Jul 2020)

## Changes in this version:

### BUG FIXES:

 - Avoid a crash when a daemon module enables `transfer logging` without
   setting a `log format` value.

 - Fixed installing rsync-ssl script from an alternate build dir.

 - Fixed the updating of configure.sh from an alternate build dir.

 - Apple requires the asm function name to begin with an underscore.

 - Avoid a test failure in the daemon test when `--atimes` is disabled.

### ENHANCEMENTS:

 - Allow the server side to restrict checksum & compression choices via the
   same environment variables the client uses.  The env vars can be divided
   into "client list & server list" by the "`&`" char or the same list can
   apply to both.

 - Simplify how the negotiation environment variables apply when interacting
   with an older rsync and also when a list contains only invalid names.

 - Do not allow a negotiated checksum or compression choice of "none" unless
   the user authorized it via an environment variable or command-line option.

 - Added the `--max-alloc=SIZE` option to be able to override the memory
   allocator's sanity-check limit.  It defaults to 1G (as before) but the error
   message when exceeding it specifically mentions the new option so that you
   can differentiate an out-of-memory error from a failure of this limit.  It
   also allows you to specify the value via the RSYNC_MAX_ALLOC environment
   variable.

 - Add the "open atime" daemon parameter to allow a daemon to always enable or
   disable the use of O_NOATIME (the default is to let the user control it).

 - The default systemd config was changed to remove the `ProtectHome=on`
   setting since rsync is often used to serve files in /home and /root and this
   seemed a bit too strict.  Feel free to use `systemctl edit rsync` to add
   that restriction (or maybe `ProtectHome=read-only`), if you like.  See the
   3.2.0 NEWS for the other restrictions that were added compared to 3.1.3.

 - The memory allocation functions now automatically check for a failure and
   die when out of memory.  This eliminated some caller-side check-and-die
   code and added some missing sanity-checking of allocations.

 - Put optimizations into their own list in the `--version` output.

 - Improved the manpage a bit more.

### PACKAGING RELATED:

 - Prepared the checksum code for an upcoming xxHash release that provides new
   XXH3 (64-bit) & XXH128 (128-bit) checksum routines.  These will not be
   compiled into rsync until the xxhash v0.8.0 include files are installed on
   the build host, and that release is a few weeks away at the time this was
   written.  So, if it's now the future and you have packaged and installed
   xxhash-0.8.0-devel, a fresh rebuild of rsync 3.2.2 will give you the new
   checksum routines.  Just make sure that the new rsync package depends on
   xxhash >= 0.8.0.

### DEVELOPER RELATED:

 - Moved the version number out of configure.ac into its own version.h file so
   that we don't need to reconfigure just because the version number changes.

 - Moved the daemon parameter list into daemon-parm.txt so that an awk script
   can create the interrelated structs and accessors that loadparm.c needs.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.1 (22 Jun 2020)

## Changes in this version:

### BUG FIXES:

 - Fixed a potential build issue with the MD5 assembly-language code by
   removing some non-portable directives.

 - Use the preprocessor with the asm file to ensure that if the code is
   unneeded, it doesn't get built.

 - Avoid the stack getting set to executable when including the asm code.

 - Some improvements in the SIMD configure testing to try to avoid build
   issues, such as avoiding a clang++ core dump when `-g` is combined with
   `-O2`.  Note that clang++ is quite buggy in this area, and it does still
   crash for some folks, so just use `--disable-simd` if you need to avoid
   their buggy compiler (since the configure test is apparently not finding
   all the compilers that will to crash and burn).

 - Fixed an issue in the md2man script when building from an alternate dir.

 - Disable `--atimes` on macOS (it apparently just ignores the atime change).

### ENHANCEMENTS:

 - The use of `--backup-dir=STR` now implies `--backup`.

 - Added `--zl=NUM` as a short-hand for `--compress-level=NUM`.

 - Added `--early-input=FILE` option that allows the client to send some
   data to a daemon's (optional) "early exec" script on its stdin.

 - Mention atimes in the capabilities list that `--version` outputs.

 - Mention either "default protect-args" or "optional protect-args" in the
   `--version` capabilities depending on how rsync was configured.

 - Some info on optimizations is now elided from the `--version` capabilities
   since they aren't really user-facing capabilities.  You can still see the
   info (plus the status of a couple extra optimizations) by repeating the
   `--version` option (e.g. `-VV`).

 - Updated various URLs to be https instead of http.

 - Some documentation improvements.

### PACKAGING RELATED:

 - If you had to use `--disable-simd` for 3.2.0, you might want to try removing
   that and see if it will succeed or auto-disable.  Some buggy clang++
   compilers are still not auto disabled, though.

 - The MD5 asm code is now under its own configure flag (not shared with the
   SIMD setting), so if you have any issues compiling it, re-run configure with
   `--disable-asm`.

 - Merged the OLDNEWS.md file into NEWS.md.

------------------------------------------------------------------------------

# NEWS for rsync 3.2.0 (19 Jun 2020)

## Changes in this version:

### BUG FIXES:

 - Avoid a potential out-of-bounds read in daemon mode if argc can be made to
   become 0.

 - Fix the default list of skip-compress files for non-daemon transfers.

 - Fix xattr filter rules losing an 'x' attribute in a non-local transfer.

 - Avoid an error when a check for a potential fuzzy file happens to reference
   a directory.

 - Make the atomic-rsync helper script have a more consistent error-exit.

 - Make sure that a signal handler's use of exit_cleanup() calls `_exit()`
   instead of exit().

 - Various zlib fixes, including security fixes for CVE-2016-9843,
   CVE-2016-9842, CVE-2016-9841, and CVE-2016-9840.

 - Fixed an issue with `--remove-source-files` not removing a source symlink
   when combined with `--copy-links`.

 - Fixed a bug where the daemon would fail to write early fatal error messages
   to the client, such as refused or unknown command-line options.

 - Fixed the block-size validation logic when dealing with older protocols.

 - Some rrsync fixes and enhancements to handle the latest options.

 - Fixed a problem with the `--link-dest`|`--copy-dest` code when `--xattrs`
   was specified along with multiple alternate-destination directories (it
   could possibly choose a bad file match while trying to find a better xattr
   match).

 - Fixed a couple bugs in the handling of files with the `--sparse` option.

 - Fixed a bug in the writing of the batch.sh file (w/`--write-batch`) when the
   source & destination args were not last on the command-line.

 - Avoid a hang when an overabundance of messages clogs up all the I/O buffers.

 - Fixed a mismatch in the RSYNC_PID values put into the environment of
   `pre-xfer exec` and a `post-xfer exec`.

 - Fixed a crash in the `--iconv` code.

 - Fixed a rare crash in the popt_unalias() code.

### ENHANCEMENTS:

 - The default systemd config was made stricter by default.  For instance,
   `ProtectHome=on` (which hides content in /root and /home/USER dirs),
   `ProtectSystem=full` (which makes /usr, /boot, & /etc dirs read-only), and
   `PrivateDevices=on` (which hides devices).  You can override any of these
   using the standard `systemctl edit rsync` and add one or more directives
   under a `[Service]` heading (and restart the rsync service).

 - Various checksum enhancements, including the optional use of openssl's MD4 &
   MD5 checksum algorithms, some x86-64 optimizations for the rolling checksum,
   some x86-64 optimizations for the (non-openssl) MD5 checksum, the addition
   of xxHash checksum support, and a negotiation heuristic that ensures that it
   is easier to add new checksum algorithms in the future.  The environment
   variable `RSYNC_CHECKSUM_LIST` can be used to customize the preference order
   of the negotiation, or use `--checksum-choice` (`--cc`) to force a choice.

 - Various compression enhancements, including the addition of zstd and lz4
   compression algorithms and a negotiation heuristic that picks the best
   compression option supported by both sides.  The environment variable
   `RSYNC_COMPRESS_LIST` can be used to customize the preference order of the
   negotiation, or use `--compress-choice` (`--zc`) to force a choice.

 - Added a `--debug=NSTR` option that outputs details of the new negotiation
   strings (for checksums and compression).  The first level just outputs the
   result of each negotiation on the client, level 2 outputs the values of the
   strings that were sent to and received from the server, and level 3 outputs
   all those values on the server side too (when the server was given the debug
   option).

 - The `--debug=OPTS` command-line option is no longer auto-forwarded to the
   remote rsync which allows for the client and server to have different levels
   of debug specified. This also allows for newer debug options to be
   specified, such as using `--debug=NSTR` to see the negotiated hash result,
   without having the command fail if the server version is too old to handle
   that debug item. Use `-M--debug=OPTS` to send the options to the remote side.

 - Added the `--atimes` option based on the long-standing patch (just with some
   fixes that the patch has been needing).

 - Added `--open-noatime` option to open files using `O_NOATIME`.

 - Added the `--write-devices` option based on the long-standing patch.

 - Added openssl & preliminary gnutls support to the rsync-ssl script, which is
   now installed by default.  This was unified with the old stunnel-rsync
   helper script to simplify packaging.  Note that the script accepts the use
   of `--type=gnutls` for gnutls testing, but does not look for gnutls-cli on
   the path yet.  The use of `--type=gnutls` will not work right until
   gnutls-cli no longer drops data.

 - Rsync was enhanced to set the `RSYNC_PORT` environment variable when running
   a daemon-over-rsh script. Its value is the user-specified port number (set
   via `--port` or an rsync:// URL) or 0 if the user didn't override the port.

 - Added the `proxy protocol` daemon parameter that allows your rsyncd to know
   the real remote IP when it is setup behind a proxy.

 - Added negated matching to the daemon's `refuse options` setting by using
   match strings that start with a `!` (such as `!compress*`).  This lets you
   refuse all options except for a particular approved list, for example.  It
   also lets rsync refuse certain options by default (such as `write-devices`)
   while allowing the config to override that, as desired.

 - Added the `early exec` daemon parameter that runs a script before the
   transfer parameters are known, allowing some early setup based on module
   name.

 - Added status output in response to a signal (via both SIGINFO & SIGVTALRM).

 - Added `--copy-as=USER` option to give some extra security to root-run rsync
   commands into/from untrusted directories (such as backups and restores).

 - When resuming the transfer of a file in the `--partial-dir`, rsync will now
   update that partial file in-place instead of creating yet another tmp file
   copy.  This requires both sender & receiver to be at least v3.2.0.

 - Added support for `RSYNC_SHELL` & `RSYNC_NO_XFER_EXEC` environment variables
   that affect the early, pre-xfer, and post-xfer exec rsync daemon parameters.

 - Optimize the `--fuzzy --fuzzy` heuristic to avoid the fuzzy directory scan
   until all other basis-file options are exhausted (such as `--link-dest`).

 - Have the daemon log include the normal-exit sent/received stats when the
   transfer exited with an error when possible (i.e. if it is the sender).

 - The daemon now locks its pid file (when configured to use one) so that it
   will not fail to start when the file exists but no daemon is running.

 - Various manpage improvements, including some html representations (that
   aren't installed by default).

 - Made `-V` the short option for `--version` and improved its information.

 - Pass the `-4` or `-6` option to the ssh command, making it easier to type
   than `--rsh='ssh -4'` (or the `-6` equivalent).

 - Added example config for rsyncd SSL proxy configs to rsyncd.conf.

 - More errors messages now mention if the error is coming from the sender or
   the receiver.

### PACKAGING RELATED:

 - Add installed bash script: /usr/bin/rsync-ssl

 - Add installed manpage: /usr/man/man1/rsync-ssl.1

 - Tweak auxiliary doc file names, such as: README.md, INSTALL.md, & NEWS.md.

 - The rsync-ssl script wants to run openssl or stunnel4, so consider adding a
   dependency for one of those options (though it's probably fine to just let
   it complain about being unable to find the program and let the user decide
   if they want to install one or the other).

 - If you packaged rsync + rsync-ssl + rsync-ssl-daemon as separate packages,
   the rsync-ssl package is now gone (rsync-ssl should be considered to be
   mainstream now that Samba requires SSL for its rsync daemon).

 - Add _build_ dependency for liblz4-dev, libxxhash-dev, libzstd-dev, and
   libssl-dev.  These development libraries will give rsync extra compression
   algorithms, extra checksum algorithms, and allow use of openssl's crypto
   lib for (potentially) faster MD4/MD5 checksums.

 - Add _build_ dependency for g++ or clang++ on x86_64 systems to enable the
   SIMD checksum optimizations.

 - Add _build_ dependency for _either_ python3-cmarkcfm or python3-commonmark
   to allow for patching of manpages or building a git release.  This is not
   required for a release-tar build, since it comes with pre-built manpages.
   Note that cmarkcfm is faster than commonmark, but they generate the same
   data.  The commonmark dependency is easiest to install since it's native
   python, and can even be installed via `pip3 install --user commonmark` if
   you want to just install it for the build user.

 - Remove yodl _build_ dependency (if it was even listed before).

### DEVELOPER RELATED:

 - Silenced some annoying warnings about major() & minor() by improving an
   autoconf include-file check.

 - Converted the manpages from yodl to markdown. They are now processed via a
   simple python3 script using the cmarkgfm **or** commonmark library.  This
   should make it easier to package rsync, since yodl is rather obscure.

 - Improved some configure checks to work better with strict C99 compilers.

 - Some perl building/packaging scripts were recoded into awk and python3.

 - Some defines in byteorder.h were changed into static inline functions that
   will help to ensure that the args don't get evaluated multiple times on
   "careful alignment" hosts.

 - Some code typos were fixed (as pointed out by a Fossies run).

------------------------------------------------------------------------------

# NEWS for rsync 3.1.3 (28 Jan 2018)

## Changes in this version:

### SECURITY FIXES:

 - Fixed a buffer overrun in the protocol's handling of xattr names and ensure
   that the received name is null terminated.

 - Fix an issue with `--protect-args` where the user could specify the arg in the
   protected-arg list and short-circuit some of the arg-sanitizing code.

### BUG FIXES:

 - Don't output about a new backup dir without appropriate info verbosity.

 - Fixed some issues with the sort functions in the rsyncstats script (in the
   support dir).

 - Added a way to specify daemon config lists (e.g. users, groups, etc) that
   contain spaces (see `auth users` in the latest rsyncd.conf manpage).

 - If a backup fails (e.g. full disk) rsync exits with an error.

 - Fixed a problem with a doubled `--fuzzy` option combined with `--link-dest`.

 - Avoid invalid output in the summary if either the start or end time had an
   error.

 - We don't allow a popt alias to affect the `--daemon` or `--server` options.

 - Fix daemon exclude code to disallow attribute changes in addition to
   disallowing transfers.

 - Don't force nanoseconds to match if a non-transferred, non-checksummed file
   only passed the quick-check w/o comparing nanoseconds.

### ENHANCEMENTS:

 - Added the ability for rsync to compare nanosecond times in its file-check
   comparisons, and added support nanosecond times on Mac OS X.

 - Added a short-option (`-@`) for `--modify-window`.

 - Added the `--checksum-choice=NAME[,NAME]` option to choose the checksum
   algorithms.

 - Added hashing of xattr names (with using `-X`) to improve the handling of
   files with large numbers of xattrs.

 - Added a way to filter xattr names using include/exclude/filter rules (see
   the `--xattrs` option in the manpage for details).

 - Added `daemon chroot|uid|gid` to the daemon config (in addition to the old
   chroot|uid|gid settings that affect the daemon's transfer process).

 - Added `syslog tag` to the daemon configuration.

 - Some manpage improvements.

### DEVELOPER RELATED:

 - Tweak the `make` output when yodl isn't around to create the manpages.

 - Changed an obsolete autoconf compile macro.

 - Support newer yodl versions when converting manpages.

------------------------------------------------------------------------------

# NEWS for rsync 3.1.2 (21 Dec 2015)

## Changes in this version:

### SECURITY FIXES:

 - Make sure that all transferred files use only path names from inside the
   transfer. This makes it impossible for a malicious sender to try to make the
   receiver use an unsafe destination path for a transferred file, such as a
   just-sent symlink.

### BUG FIXES:

 - Change the checksum seed order in the per-block checksums. This prevents
   someone from trying to create checksum blocks that match in sum but not
   content.

 - Fixed a with the per-dir filter files (using `-FF`) that could trigger an
   assert failure.

 - Only skip `set_modtime()` on a transferred file if the time is exactly
   right.

 - Don't create an empty backup dir for a transferred file that doesn't exist
   yet.

 - Fixed a bug where `--link-dest` and `--xattrs` could cause rsync to exit if
   a filename had a matching dir of the same name in the alt-dest area.

 - Allow more than 32 group IDs per user in the daemon's gid=LIST config.

 - Fix the logging of %b & %c via `--log-file` (daemon logging was already
   correct, as was `--out-format='%b/%c'`).

 - Fix erroneous acceptance of `--info=5` & `--debug=5` (an empty flag name is
   not valid).

### ENHANCEMENTS:

 - Added `(DRY RUN)` info to the `--debug=exit` output line.

 - Use usleep() for our msleep() function if it is available.

 - Added a few extra long-option names to rrsync script, which will make
   BackupPC happier.

 - Made configure choose to use Linux xattrs on NetBSD (rather than not
   supporting xattrs).

 - Added `-wo` (write-only) option to rrsync support script.

 - Misc. manpage tweaks.

### DEVELOPER RELATED:

 - Fixed a bug with the Makefile's use of `INSTALL_STRIP`.

 - Improve a test in the suite that could get an erroneous timestamp error.

 - Tweaks for newer versions of git in the packaging tools.

 - Improved the m4 generation rules and some autoconf idioms.

------------------------------------------------------------------------------

# NEWS for rsync 3.1.1 (22 Jun 2014)

## Changes in this version:

### BUG FIXES:

 - If the receiver gets bogus filenames from the sender (an unexpected leading
   slash or a `..` infix dir), exit with an error. This prevents a malicious
   sender from trying to inject filenames that would affect an area outside the
   destination directories.

 - Fixed a failure to remove the partial-transfer temp file when interrupted
   (and rsync is not saving the partial files).

 - Changed the chown/group/xattr-set order to avoid losing some security-
   related xattr info (that would get cleared by a chown).

 - Fixed a bug in the xattr-finding code that could make a non-root-run
   receiver not able to find some xattr numbers.

 - Fixed a bug in the early daemon protocol where a timeout failed to be
   honored (e.g. if the remote side fails to send us the initial protocol
   greeting).

 - Fixed unintended inclusion of commas in file numbers in the daemon log.

 - We once again send the 'f' sub-flag (of `-e`) to the server side so it knows
   that we can handle incremental-recursion directory errors properly in older
   protocols.

 - Fixed an issue with too-aggressive keep-alive messages causing a problem for
   older rsync versions early in the transfer.

 - Fixed an incorrect message about backup-directory-creation when using
   `--dry-run` and the backup dir is not an absolute path.

 - Fixed a bug where a failed deletion and/or a failed sender-side removal
   would not affect the exit code.

 - Fixed a bug that caused a failure when combining `--delete-missing-args`
   with `--xattrs` and/or `--acls`.

 - Fixed a strange `dir_depth` assertion error that was caused by empty-dir
   removals and/or duplicate files in the transfer.

 - Fixed a problem with `--info=progress2`'s output stats where rsync would
   only update the stats at the end of each file's transfer. It now uses the
   data that is flowing for the current file, making the stats more accurate
   and less jumpy.

 - Fixed an itemize bug that affected the combo of `--link-dest`, `-X`, and
   `-n`.

 - Fixed a problem with delete messages not appearing in the log file when the
   user didn't use `--verbose`.

 - Improve chunked xattr reading for OS X.

 - Removed an attempted hard-link xattr optimization that was causing a
   transfer failure. This removal is flagged in the compatibility code, so if a
   better fix can be discovered, we have a way to flip it on again.

 - Fixed a bug when the receiver is not configured to be able to hard link
   symlimks/devices/special-file items but the sender sent some of these items
   flagged as hard-linked.

 - We now generate a better error if the buffer overflows in `do_mknod()`.

 - Fixed a problem reading more than 16 ACLs on some OSes.

 - Fixed the reading of the secrets file to avoid an infinite wait when the
   username is missing.

 - Fixed a parsing problem in the `--usermap`/`--groupmap` options when using
   MIN-MAX numbers.

 - Switched Cygwin back to using socketpair `pipes` to try to speed it up.

 - Added knowledge of a few new options to rrsync.

### ENHANCEMENTS:

 - Tweaked the temp-file naming when `--temp-dir=DIR` is used: the temp-file
   names will not get a '.' prepended.

 - Added support for a new-compression idiom that does not compress all the
   matching data in a transfer. This can help rsync to use less cpu when a
   transfer has a lot of matching data, and also makes rsync compatible with a
   non-bundled zlib. See the `--new-compress` and `--old-compress` options in
   the manpage.

 - Added the rsync-no-vanished shell script. (See the support dir.)

 - Made configure more prominently mention when we failed to find yodl (in case
   the user wants to be able to generate manpages from `*.yo` files).

 - Have manpage mention how a daemon's max-verbosity setting affects info and
   debug options. Also added more clarification on backslash removals for
   excludes that contain wildcards.

 - Have configure check if for the attr lib (for getxattr) for those systems
   that need to link against it explicitly.

 - Change the early dir-creation logic to only use that idiom in an
   inc-recursive copy that is preserving directory times. e.g. using
   `--omit-dir-times` will avoid these early directories being created.

 - Fix a bug in `cmp_time()` that would return a wrong result if the 2 times
   differed by an amount greater than what a `time_t` can hold.

### DEVELOPER RELATED:

 - We now include an example systemd file (in packaging/systemd).

 - Tweaked configure to make sure that any intended use of the included popt
   and/or zlib code is put early in the CFLAGS.

------------------------------------------------------------------------------

# NEWS for rsync 3.1.0 (28 Sep 2013)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 31.

### OUTPUT CHANGES:

 - Output numbers in 3-digit groups by default (e.g. 1,234,567). See the
   `--human-readable` option for a way to turn it off. See also the daemon's
   `log format` parameter and related command-line options (including
   `--out-format`) for a modifier that can be used to request digit-grouping or
   human-readable output in log escapes. (Note that log output is unchanged by
   default.)

 - The `--list-only` option is now affected by the `--human-readable` setting.
   It will display digit groupings by default, and unit suffixes if higher
   levels of readability are requested. Also, the column width for the size
   output has increased from 11 to 14 characters when human readability is
   enabled. Use `--no-h` to get the old-style output and column size.

 - The output of the `--progress` option has changed: the string `xfer` was
   shortened to `xfr`, and the string `to-check` was shortened to `to-chk`,
   both designed to make room for the (by default) wider display of file size
   numbers without making the total line-length longer. Also, when incremental
   recursion is enabled, the string `ir-chk` will be used instead of `to-chk`
   up until the incremental-recursion scan is done, letting you know that the
   value to check and the total value will still be increasing as new files are
   found.

 - Enhanced the `--stats` output: 1) to mention how many files were created
   (protocol >= 28), 2) to mention how many files were deleted (a new line for
   protocol 31, but only output when `--delete` is in effect), and 3) to follow
   the file-count, created-count, and deleted-count with a subcount list that
   shows the counts by type. The wording of the transferred count has also
   changed so that it is clearer that it is only a count of regular files.

### BUG FIXES:

 - Fixed a bug in the iconv code when EINVAL or EILSEQ is returned with a full
   output buffer.

 - Fixed some rare bugs in `--iconv` processing that might cause a multi-byte
   character to get translated incorrectly.

 - Fixed a bogus `vanished file` error if some files were specified with `./`
   prefixes and others were not.

 - Fixed a bug in `--sparse` where an extra gap could get inserted after a
   partial write.

 - Changed the way `--progress` overwrites its prior output in order to make it
   nearly impossible for the progress to get overwritten by an error.

 - Improved the propagation of abnormal-exit error messages. This should help
   the client side to receive errors from the server when it is exiting
   abnormally, and should also avoid dying with an `connection unexpectedly
   closed` exit when the closed connection is really expected.

 - The sender now checks each file it plans to remove to ensure that it hasn't
   changed from the first stat's info. This helps to avoid losing file data
   when the user is not using the option in a safe manner.

 - Fixed a data-duplication bug in the compress option that made compression
   less efficient. This improves protocol 31 onward, while behaving in a
   compatible (buggy) manner with older rsync protocols.

 - When creating a temp-file, rsync is now a bit smarter about it dot-char
   choices, which can fix a problem on OS X with names that start with `..`.

 - Rsync now sets a cleanup flag for `--inplace` and `--append` transfers that
   will flush the write buffer if the transfer aborts. This ensures that more
   received data gets written out to the disk on an aborted transfer (which is
   quite helpful on a slow, flaky connection).

 - The reads that `map_ptr()` now does are aligned on 1K boundaries. This helps
   some filesystems and/or files that don't like unaligned reads.

 - Fix an issue in the msleep() function if time jumps backwards.

 - Fix daemon-server module-name splitting bug where an arg would get split
   even if `--protect-args` was used.

### ENHANCEMENTS:

 - Added the `--remote-option=OPT` (`-M OPT`) command-line option that is
   useful for things like sending a remote `--log-file=FILE` or `--fake-super`
   option.

 - Added the `--info=FLAGS` and `--debug=FLAGS` options to allow finer-grained
   control over what is output. Added an extra type of `--progress` output
   using `--info=progress2`.

 - The `--msgs2stderr` option can help with debugging rsync by allowing the
   debug messages to get output to stderr rather than travel via the socket
   protocol.

 - Added the `--delete-missing-args` and `--ignore-missing-args` options to
   either delete or ignore user-specified files on the receiver that are
   missing on the sender (normally the absence of user-specified files
   generates an error).

 - Added a `T` (terabyte) category to the `--human-readable` size suffixes.

 - Added the `--usermap`/`--groupmap`/`--chown` options for manipulating file
   ownership during the copy.

 - Added the `%C` escape to the log-output handling, which will output the MD5
   checksum of any transferred file, or all files if `--checksum` was specified
   (when protocol 30 or above is in effect).

 - Added the `reverse lookup` parameter to the rsync daemon config file to
   allow reverse-DNS lookups to be disabled.

 - Added a forward-DNS lookup for the daemon's hosts allow/deny config. Can be
   disabled via `forward lookup` parameter (defaults to enabled).

 - Added a way for more than one group to be specified in the daemon's config
   file, including a way to specify that you want all of the specified user's
   groups without having to name them. Also changed the daemon to complain
   about an inability to set explicitly-specified uid/gid values, even when not
   run by a super-user.

 - The daemon now tries to send the user the error messages from the pre-xfer
   exec script when it fails.

 - Improved the use of alt-dest options into an existing hierarchy of files: If
   a match is found in an alt-dir, it takes precedence over an existing file.
   (We'll need to wait for a future version before attribute-changes on
   otherwise unchanged files are safe when using an existing hierarchy.)

 - Added per-user authorization options and group-authorization support to the
   daemon's `auth users` parameter.

 - Added a way to reference environment variables in a daemon's config file
   (using %VAR% references).

 - When replacing a non-dir with a symlink/hard-link/device/special-file, the
   update should now be done in an atomic manner.

 - Avoid re-sending xattr info for hard-linked files w/the same xattrs
   (protocol 31).

 - The backup code was improved to use better logic maintaining the backup
   directory hierarchy. Also, when a file is being backed up, rsync tries to
   hard-link it into place so that the upcoming replacement of the destination
   file will be atomic (for the normal, non-inplace logic).

 - Added the ability to synchronize nanosecond modified times.

 - Added a few more default suffixes for the `dont compress` settings.

 - Added the checking of the `RSYNC_PROTECT_ARGS` environment variable to allow
   the default for the `--protect-args` command-line option to be overridden.

 - Added the `--preallocate` command-line option.

 - Allow `--password-file=-` to read the password from stdin (filename `-`).

 - Rsync now comes packaged with an rsync-ssl helper script that can be used to
   contact a remote rsync daemon using a piped-stunnel command.  It also
   includes an stunnel config file to run the server side to support ssl daemon
   connections. See the packaging/lsb/rsync.spec file for one way to package
   the resulting files. (Suggestions for how to make this even easier to
   install & use are welcomed.)

 - Improved the speed of some `--inplace` updates when there are lots of
   identical checksum blocks that end up being unusable.

 - Added the `--outbuf=N|L|B` option for choosing the output buffering.

 - Repeating the `--fuzzy` option now causes the code to look for fuzzy matches
   inside alt-dest directories too.

 - The `--chmod` option now supports numeric modes, e.g. `--chmod=644,D755`

 - Added some Solaris xattr code.

 - Made an rsync daemon (the listening process) exit with a 0 status when it
   was signaled to die. This helps launchd.

 - Improved the `RSYNC_*` environment variables for the pre-xfer exec script:
   when a daemon is sent multiple request args, they are now joined into a
   single return value (separated by spaces) so that the `RSYNC_REQUEST`
   environment variable is accurate for any `pre-xfer exec`. The values in
   `RSYNC_ARG#` vars are no longer truncated at the `.` arg (prior to the
   request dirs/files), so that all the requested values are also listed
   (separately) in `RSYNC_ARG#` variables.

### EXTRAS:

 - Added an `instant-rsyncd` script to the support directory, which makes it
   easy to configure a simple rsync daemon in the current directory.

 - Added the `mapfrom` and `mapto` scripts to the support directory, which
   makes it easier to do user/group mapping in a local transfer based on
   passwd/group files from another machine.

 - There's a new, improved version of the lsh script in the support dir: it's
   written in perl and supports `-u` without resorting to using sudo (when run
   as root). The old shell version is now named lsh.sh.

 - There is a helper script named rsync-slash-strip in the support directory
   for anyone that wants to change the way rsync handles args with trailing
   slashes. (e.g. arg/ would get stripped to arg while arg/. would turn into
   arg/).

### INTERNAL:

 - The I/O code was rewritten to be simpler and do bigger buffered reads over
   the socket. The I/O between the receiver and the generator was changed to be
   standard multiplexed-I/O (like that over the socket).

 - The sender tries to use any dead time while the generator is looking for
   files to transfer in order to do sender-side directory scanning in a more
   parallel manner.

 - A daemon can now inform a client about a daemon-configured timeout value so
   that the client can assist in the keep-alive activity (protocol 31).

 - The filter code received some refactoring to make it more extendable, to
   read better, and do better sanity checking.

 - Really big numbers are now output using our own big-num routine rather than
   casting them to a double and using a %.0f conversion.

 - The `pool_alloc` library has received some minor improvements in alignment
   handling.

 - Added `init_stat_x()` function to avoid duplication of acl/xattr init code.

 - The included zlib was upgraded from 1.2.3 to 1.2.8.

 - Rsync can now be compiled to use an unmodified zlib library instead of the
   tweaked one that is included with rsync. This will eventually become the
   default, at which point we'll start the countdown to removing the included
   zlib. Until then, feel free to configure using:

   ./configure `--with-included-zlib=no`

### DEVELOPER RELATED:

 - Added more conditional debug output.

 - Fixed some build issues for Android and Minix.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.9 (23 Sep 2011)

## Changes in this version:

### BUG FIXES:

 - Fix a crash bug in checksum scanning when `--inplace` is used.

 - Fix a hang if a hard-linked file cannot be opened by the sender (e.g.  if it
   has no read permission).

 - Fix preservation of a symlink's system xattrs (e.g. selinux) on Linux.

 - Fix a memory leak in the xattr code.

 - Fixed a bug with `--delete-excluded` when a filter merge file has a rule
   that specifies a receiver-only side restriction.

 - Fix a bug with the modifying of unwritable directories.

 - Fix `--fake-super`'s interaction with `--link-dest` same-file comparisons.

 - Fix the updating of the `curr_dir` buffer to avoid a duplicate slash.

 - Fix the directory permissions on an implied dot-dir when using `--relative`
   (e.g. /outside/path/././send/path).

 - Fixed some too-long sleeping instances when using `--bwlimit`.

 - Fixed when symlink ownership difference-checking gets compiled into
   `unchanged_attrs()`.

 - Improved the socket-error reporting when multiple protocols fail.

 - Fixed a case where a socket error could reference just-freed memory.

 - Failing to use a password file that was specified on the command-line is now
   a fatal error.

 - Fix the non-root updating of directories that don't have the read and/or
   execute permission.

 - Make daemon-excluded file errors more error-like.

 - Fix a compilation issue on older C compilers (due to a misplaced var
   declaration).

 - Make configure avoid finding socketpair on Cygwin.

 - Avoid trying to reference `SO_BROADCAST` if the OS doesn't support it.

 - Fix some issues with the post-processing of the manpages.

 - Fixed the user home-dir handling in the lsh script. (See the support dir.)

 - Some minor manpage improvements.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.8 (26 Mar 2011)

## Changes in this version:

### BUG FIXES:

 - Fixed two buffer-overflow issues: one where a directory path that is exactly
   MAXPATHLEN was not handled correctly, and one handling a `--backup-dir` that
   is extra extra large.

 - Fixed a data-corruption issue when preserving hard-links without preserving
   file ownership, and doing deletions either before or during the transfer
   (CVE-2011-1097). This fixes some assert errors in the hard-linking code, and
   some potential failed checksums (via `-c`) that should have matched.

 - Fixed a potential crash when an rsync daemon has a filter/exclude list and
   the transfer is using ACLs or xattrs.

 - Fixed a hang if a really large file is being processed by an rsync that
   can't handle 64-bit numbers. Rsync will now complain about the file being
   too big and skip it.

 - For devices and special files, we now avoid gathering useless ACL and/or
   xattr information for files that aren't being copied. (The un-copied files
   are still put into the file list, but there's no need to gather data that is
   not going to be used.) This ensures that if the user uses `--no-D`, that
   rsync can't possibly complain about being unable to gather extended
   information from special files that are in the file list (but not in the
   transfer).

 - Properly handle requesting remote filenames that start with a dash. This
   avoids a potential error where a filename could be interpreted as a (usually
   invalid) option.

 - Fixed a bug in the comparing of upper-case letters in file suffixes for
   `--skip-compress`.

 - If an rsync daemon has a module configured without a path setting, rsync
   will now disallow access to that module.

 - If the destination arg is an empty string, it will be treated as a reference
   to the current directory (as 2.x used to do).

 - If rsync was compiled with a newer time-setting function (such as lutimes),
   rsync will fall-back to an older function (such as utimes) on a system where
   the newer function is not around. This helps to make the rsync binary more
   portable in mixed-OS-release situations.

 - Fixed a batch-file writing bug that would not write out the full set of
   compatibility flags that the transfer was using. This fixes a potential
   protocol problem for a batch file that contains a sender-side I/O error: it
   would have been sent in a way that the batch-reader wasn't expecting.

 - Some improvements to the hard-linking code to ensure that device-number
   hashing is working right, and to supply more information if the hard-link
   code fails.

 - The `--inplace` code was improved to not search for an impossible checksum
   position. The quadruple-verbose chunk[N] message will now mention when an
   inplace chunk was handled by a seek rather than a read+write.

 - Improved ACL mask handling, e.g. for Solaris.

 - Fixed a bug that prevented `--numeric-ids` from disabling the translation of
   user/group IDs for ACLs.

 - Fixed an issue where an xattr and/or ACL transfer that used an alt-dest
   option (e.g. `--link-dest`) could output an error trying to itemize the
   changes against the alt-dest directory's xattr/ACL info but was instead
   trying to access the not-yet-existing new destination directory.

 - Improved xattr system-error messages to mention the full path to the file.

 - The `--link-dest` checking for identical symlinks now avoids considering
   attribute differences that cannot be changed on the receiver.

 - Avoid trying to read/write xattrs on certain file types for certain OSes.
   Improved configure to set `NO_SYMLINK_XATTRS`, `NO_DEVICE_XATTRS`, and/or
   `NO_SPECIAL_XATTRS` defines in config.h.

 - Improved the unsafe-symlink errors messages.

 - Fixed a bug setting xattrs on new files that aren't user writable.

 - Avoid re-setting xattrs on a hard-linked file w/the same xattrs.

 - Fixed a bug with `--fake-super` when copying files and dirs that aren't user
   writable.

 - Fixed a bug where a sparse file could have its last sparse block turned into
   a real block when rsync sets the file size (requires ftruncate).

 - If a temp-file name is too long, rsync now avoids truncating the name in the
   middle of adjacent high-bit characters. This prevents a potential filename
   error if the filesystem doesn't allow a name to contain an invalid
   multi-byte sequence.

 - If a muli-protocol socket connection fails (i.e., when contacting a daemon),
   we now report all the failures, not just the last one. This avoids losing a
   relevant error (e.g. an IPv4 connection-refused error) that happened before
   the final error (e.g. an IPv6 protocol-not-supported error).

 - Generate a transfer error if we try to call chown with a `-1` for a uid or a
   gid (which is not settable).

 - Fixed the working of `--force` when used with `--one-file-system`.

 - Fix the popt arg parsing so that an option that doesn't take an arg will
   reject an attempt to supply one (can configure `--with-included-popt` if
   your system's popt library doesn't yet have this fix).

 - A couple minor option tweaks to the rrsync script, and also some regex
   changes that make vim highlighting happier. (See the support dir.)

 - Fixed some issues in the mnt-excl script. (See the support dir.)

 - Various manpage improvements.

### ENHANCEMENTS:

 - Added `.hg/` to the default cvs excludes (see `-C` & `--cvs-exclude`).

### DEVELOPER RELATED:

 - Use lchmod() whenever it is available (not just on symlinks).

 - A couple fixes to the `socketpair_tcp()` routine.

 - Updated the helper scripts in the packaging subdirectory.

 - Renamed configure.in to configure.ac.

 - Fixed configure's checking for iconv routines for newer OS X versions.

 - Fixed the testsuite/xattrs.test script on OS X.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.7 (31 Dec 2009)

## Changes in this version:

### BUG FIXES:

 - Fixed a bogus free when using `--xattrs` with `--backup`.

 - Avoid an error when `--dry-run` was trying to stat a prior hard-link file
   that hasn't really been created.

 - Fixed a problem with `--compress` (`-z`) where the receiving side could
   return the error "`inflate (token) returned -5`".

 - Fixed a bug where `--delete-during` could delete in a directory before it
   noticed that the sending side sent an I/O error for that directory (both
   sides of the transfer must be at least 3.0.7).

 - Improved `--skip-compress`'s error handling of bad character-sets and got
   rid of a lingering debug fprintf().

 - Fixed the daemon's conveyance of `io_error` value from the sender.

 - An rsync daemon use seteuid() (when available) if it used setuid().

 - Get the permissions right on a `--fake-super` transferred directory that
   needs more owner permissions to emulate root behavior.

 - An absolute-path filter rule (i.e. with a '/' modifier) no longer loses its
   modifier when sending the filter rules to the remote rsync.

 - Improved the "`--delete does not work without -r or -d`" message.

 - Improved rsync's handling of `--timeout` to avoid a weird timeout case where
   the sender could timeout even though it has recently written data to the
   socket (but hasn't read data recently, due to the writing).

 - Some misc manpage improvements.

 - Fixed the chmod-temp-dir testsuite on a system without /var/tmp.

 - Make sure that a timeout specified in the daemon's config is used as a
   maximum timeout value when the user also specifies a timeout.

 - Improved the error-exit reporting when rsync gets an error trying to cleanup
   after an error: the initial error is reported.

 - Improved configure's detection of IPv6 for Solaris and Cygwin.

 - The AIX sysacls routines will now return ENOSYS if ENOTSUP is missing.

 - Made our (only used if missing) getaddrinfo() routine use `inet_pton()`
   (which we also provide) instead of `inet_aton()`.

 - The exit-related debug messages now mention the program's role so it is
   clear who output what message.

### DEVELOPER RELATED:

 - Got rid of type-punned compiler warnings output by newer gcc versions.

 - The Makefile now ensures that proto.h will be rebuilt if config.h changes.

 - The testsuite no longer uses `id -u`, so it works better on Solaris.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.6 (8 May 2009)

## Changes in this version:

### BUG FIXES:

 - Fixed a `--read-batch` hang when rsync is reading a batch file that was
   created from an incremental-recursion transfer.

 - Fixed the daemon's socket code to handle the simultaneous arrival of
   multiple connections.

 - Fix `--safe-links`/`--copy-unsafe-links` to properly handle symlinks that
   have consecutive slashes in the value.

 - Fixed the parsing of an `[IPv6_LITERAL_ADDR]` when a USER@ is prefixed.

 - The sender now skips a (bogus) symlink that has a 0-length value, which
   avoids a transfer error in the receiver.

 - Fixed a case where the sender could die with a tag-0 error if there was an
   I/O during the sending of the file list.

 - Fixed the rrsync script to avoid a server-side problem when `-e` is at the
   start of the short options.

 - Fixed a problem where a vanished directory could turn into an exit code 23
   instead of the proper exit code 24.

 - Fixed the `--iconv` conversion of symlinks when doing a local copy.

 - Fixed a problem where `--one-file-system` was not stopping deletions on the
   receiving side when a mount-point directory did not match a directory in the
   transfer.

 - Fixed the dropping of an ACL mask when no named ACL values were present.

 - Fixed an ACL/xattr corruption issue where the `--backup` option could cause
   rsync to associate the wrong ACL/xattr information with received files.

 - Fixed the use of `--xattrs` with `--only-write-batch`.

 - Fixed the use of `--dry-run` with `--read-batch`.

 - Fixed configure's erroneous use of target.

 - Fixed configure's `--disable-debug` option.

 - Fixed a run-time issue for systems that can't find `iconv_open()` by adding
   the `--disable-iconv-open` configure option.

 - Complain and die if the user tries to combine `--remove-source-files` (or
   the deprecated `--remove-sent-files`) with `--read-batch`.

 - Fixed an failure transferring special files from Solaris to Linux.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.5 (28 Dec 2008)

## Changes in this version:

### BUG FIXES:

 - Initialize xattr data in a couple spots in the hlink code, which avoids a
   crash when the xattr pointer's memory happens to start out non-zero.  Also
   fixed the itemizing of an alt-dest file's xattrs when hard-linking.

 - Don't send a bogus `-` option to an older server if there were no short
   options specified.

 - Fixed skipping of unneeded updates in a batch file when incremental
   recursion is active. Added a test for this. Made batch-mode handle `redo`
   files properly (and without hanging).

 - Fix the %P logfile escape when the daemon logs from inside a chroot.

 - Fixed the use of `-s` (`--protect-args`) when used with a remote source or
   destination that had an empty path (e.g. `host:`). Also fixed a problem when
   `-s` was used when accessing a daemon via a remote-shell.

 - Fixed the use of a dot-dir path (e.g. foo/./bar) inside a `--files-from`
   file when the root of the transfer isn't the current directory.

 - Fixed a bug with `-K --delete` removing symlinks to directories when
   incremental recursion is active.

 - Fixed a hard to trigger hang when using `--remove-source-files`.

 - Got rid of an annoying delay when accessing a daemon via a remote-shell.

 - Properly ignore (superfluous) source args on a `--read-batch` command.

 - Improved the manpage's description of the `*` wildcard to remove the
   confusing `non-empty` qualifier.

 - Fixed reverse lookups in the compatibility-library version of getnameinfo().

 - Fixed a bug when using `--sparse` on a sparse file that has over 2GB of
   consecutive sparse data.

 - Avoid a hang when using at least 3 `--verbose` options on a transfer with a
   client sender (which includes local copying).

 - Fixed a problem with `--delete-delay` reporting an error when it was ready
   to remove a directory that was now gone.

 - Got rid of a bunch of `warn_unused_result` compiler warnings.

 - If an ftruncate() on a received file fails, it now causes a partial-
   transfer warning.

 - Allow a path with a leading `//` to be preserved (CYGWIN only).

### ENHANCEMENTS:

 - Made the atomic-rsync script able to perform a fully atomic update of the
   copied hierarchy when the destination is setup using a particular symlink
   idiom. (See the support dir.)

------------------------------------------------------------------------------

# NEWS for rsync 3.0.4 (6 Sep 2008)

## Changes in this version:

### BUG FIXES:

 - Fixed a bug in the hard-linking code where it would sometimes try to
   allocate 0 bytes of memory (which fails on some OSes, such as AIX).

 - Fixed the hard-linking of files from a device that has a device number of 0
   (which seems to be a common device number on NetBSD).

 - Fixed the handling of a `--partial-dir` that cannot be created. This
   particularly impacts the `--delay-updates` option (since the files cannot be
   delayed without a partial-dir), and was potentially destructive if the
   `--remove-source-files` was also specified.

 - Fixed a couple issues in the `--fake-super` handling of xattrs when the
   destination files have root-level attributes (e.g. selinux values) that a
   non-root copy can't affect.

 - Improved the keep-alive check in the generator to fire consistently in
   incremental-recursion mode when `--timeout` is enabled.

 - The `--iconv` option now converts the content of a symlink too, instead of
   leaving it in the wrong character-set (requires 3.0.4 on both sides of the
   transfer).

 - When using `--iconv`, if a filename fails to convert on the receiving side,
   this no longer makes deletions in the root-dir of the transfer fail silently
   (the user now gets a warning about deletions being disabled due to IO error
   as long as `--ignore-errors` was not specified).

 - When using `--iconv`, if a server-side receiver can't convert a filename,
   the error message sent back to the client no longer mangles the name with
   the wrong charset conversion.

 - Fixed a potential alignment issue in the IRIX ACL code when allocating the
   initial `struct acl` object. Also, cast mallocs to avoid warnings.

 - Changed some errors that were going to stdout to go to stderr.

 - Made `human_num()` and `human_dnum()` able to output a negative number
   (rather than outputting a cryptic string of punctuation).

### ENHANCEMENTS:

 - Rsync will avoid sending an `-e` option to the server if an older protocol
   is requested (and thus the option would not be useful). This lets the user
   specify the `--protocol=29` option to access an overly-restrictive server
   that is rejecting the protocol-30 use of `-e` to the server.

 - Improved the message output for an `RERR_PARTIAL` exit.

### DEVELOPER RELATED:

 - The Makefile will not halt for just a timestamp change on the Makefile or
   the configure files, only for actual changes in content.

 - Changed some commands in the testsuite's xattrs.test that called `rsync`
   instead of `$RSYNC`.

 - Enhanced the release scripts to be able to handle a branch release and to do
   even more consistency checks on the files.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.3 (29 Jun 2008)

## Changes in this version:

### BUG FIXES:

 - Fixed a wildcard matching problem in the daemon when a module has `use
   chroot` enabled.

 - Fixed a crash bug in the hard-link code.

 - Fixed the sending of xattr directory information when the code finds a
   `--link-dest` or `--copy-dest` directory with unchanged xattrs -- the
   destination directory now gets these unchanged xattrs properly applied.

 - Fixed an xattr-sending glitch that could cause an `Internal abbrev` error.

 - Fixed the combination of `--xattrs` and `--backup`.

 - The generator no longer allows a '.' dir to be excluded by a daemon-exclude
   rule.

 - Fixed deletion handling when copying a single, empty directory (with no
   files) to a differently named, non-existent directory.

 - Fixed the conversion of spaces into dashes in the %M log escape.

 - Fixed several places in the code that were not returning the right errno
   when a function failed.

 - Fixed the backing up of a device or special file into a backup dir.

 - Moved the setting of the socket options prior to the connect().

 - If rsync exits in the middle of a `--progress` output, it now outputs a
   newline to help prevent the progress line from being overwritten.

 - Fixed a problem with how a destination path with a trailing slash or a
   trailing dot-dir was compared against the daemon excludes.

 - Fixed the sending of large (size > 16GB) files when talking to an older
   rsync (protocols < 30): we now use a compatible block size limit.

 - If a file's length is so huge that we overflow a checksum buffer count (i.e.
   several hundred TB), warn the user and avoid sending an invalid checksum
   struct over the wire.

 - If a source arg is excluded, `--relative` no longer adds the excluded arg's
   implied dirs to the transfer. This fix also made the exclude check happen in
   the better place in the sending code.

 - Use the `overflow_exit()` function for overflows, not `out_of_memory()`.

 - Improved the code to better handle a system that has only 32-bit file
   offsets.

### ENHANCEMENTS:

 - The rsyncd.conf manpage now consistently refers to the parameters in the
   daemon config file as `parameters`.

 - The description of the `--inplace` option was improved.

### EXTRAS:

 - Added a new script in the support directory, deny-rsync, which allows an
   admin to (temporarily) replace the rsync command with a script that sends an
   error message to the remote client via the rsync protocol.

### DEVELOPER RELATED:

 - Fixed a testcase failure if the tests are run as root and made some
   compatibility improvements.

 - Improved the daemon tests, including checking module comments, the listing
   of files, and the ensuring that daemon excludes can't affect a dot-dir arg.

 - Improved some build rules for those that build in a separate directory from
   the source, including better install rules for the manpages, and the fixing
   of a proto.h-tstamp rule that could make the binaries get rebuild without
   cause.

 - Improved the testsuite to work around a problem with some utilities (e.g.
   `cp -p` & `touch -r`) rounding sub-second timestamps.

 - Ensure that the early patches don't cause any generated-file hunks to
   bleed-over into patches that follow.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.2 (8 Apr 2008)

## Changes in this version:

### BUG FIXES:

 - Fixed a potential buffer overflow in the xattr code.

### ENHANCEMENTS:

 - None.

### DEVELOPER RELATED:

 - The RPM spec file was improved to install more useful files.

 - A few developer-oriented scripts were moved from the support dir to the
   packaging dir.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.1 (3 Apr 2008)

## Changes in this version:

### NOTABLE CHANGES IN BEHAVIOR:

 - Added the 'c'-flag to the itemizing of non-regular files so that the
   itemized output doesn't get hidden if there were no attribute changes, and
   also so that the itemizing of a `--copy-links` run will distinguish between
   copying an identical non-regular file and the creation of a revised version
   with a new value (e.g. a changed symlink referent, a new device number,
   etc.).

### BUG FIXES:

 - Fixed a crash bug when a single-use rsync daemon (via remote shell) was run
   without specifying a `--config=FILE` option.

 - Fixed a crash when backing up a directory that has a default ACL.

 - Fixed a bug in the handling of xattr values that could cause rsync to not
   think that a file's extended attributes are up-to-date.

 - Fixed the working of `--fake-super` with `--link-dest` and `--xattrs`.

 - Fixed a hang when combining `--dry-run` with `--remove-source-files`.

 - Fixed a bug with `--iconv`'s handling of files that cannot be converted: a
   failed name can no longer cause a transfer failure.

 - Fixed the building of the rounding.h file on systems that need custom
   CPPFLAGS to be used. Also improved the error reporting if the building of
   rounding.h fails.

 - Fixed the use of the `--protect-args` (`-s`) option when talking to a
   daemon.

 - Fixed the `--ignore-existing` option's protection of files on the receiver
   that are non-regular files on the sender (e.g. if a symlink or a dir on the
   sender is trying to replace a file on the receiver). The reverse protection
   (protecting a dir/symlink/device from being replaced by a file) was already
   working.

 - Fixed an assert failure if `--hard-links` is combined with an option that
   can skip a file in a set of hard-linked files (i.e. `--ignore-existing`,
   `--append`, etc.), without skipping all the files in the set.

 - Avoid setting the modify time on a directory that already has the right
   modify time set. This avoids tweaking the dir's ctime.

 - Improved the daemon-exclude handling to do a better job of applying the
   exclude rules to path entries. It also sends the user an error just as if
   the files were actually missing (instead of silently ignoring the user's
   args), and avoids sending the user the filter-action messages for these
   non-user-initiated rules.

 - Fixed some glitches with the dry-run code's missing-directory handling,
   including a problem when combined with `--fuzzy`.

 - Fixed some glitches with the skipped-directory handling.

 - Fixed the 'T'-flag itemizing of symlinks when `--time` isn't preserved.

 - Fixed a glitch in the itemizing of permissions with the `-E` option.

 - The `--append` option's restricting of transfers to those that add data no
   longer prevents the updating of non-content changes to otherwise up-to-date
   files (i.e. those with the same content but differing permissions,
   ownership, xattrs, etc.).

 - Don't allow `--fake-super` to be specified with `-XX` (double `--xattrs`)
   because the options conflict. If a daemon has `fake super` enabled, it
   automatically downgrades a `-XX` request to `-X`.

 - Fixed a couple bugs in the parsing of daemon-config excludes that could make
   a floating exclude rule get treated as matching an absolute path.

 - A daemon doesn't try to auto-refuse the `iconv` option if iconv-support
   wasn't compiled in to the daemon (avoiding a warning in the logs).

 - Fixed the inclusion of per-dir merge files from implied dirs.

 - Fixed the rrsync script to work with the latest options that rsync sends,
   including its flag-specifying use of `-e` to the server. (See the support
   dir.)

### ENHANCEMENTS:

 - Added the `--old-dirs` (`--old-d`) option to make it easier for a user to
   ask for file-listings with older rsync versions (this is easier than having
   to type `-r --exclude='/*/*'` manually).

 - When getting an error while asking an older rsync daemon for a file listing,
   rsync will try to notice if the error is a rejection of the `--dirs` (`-d`)
   option and let the user know how to work around the issue.

 - Added a few more `--no-OPTION` overrides.

 - Improved the documentation of the `--append` option.

 - Improved the documentation of the filter/exclude/include daemon parameters.

### INTERNAL:

 - Fixed a couple minor bugs in the included popt library (ones which I sent to
   the official popt project for inclusion in the 1.14 release).

 - Fixed a stat() call that should have been `do_stat()` so that the proper
   normal/64-bit stat() function gets called. (Was in an area that should not
   have caused problems, though.)

 - Changed the file-glob code to do a directory scan without using the `glob`
   and `glob.h`. This lets us do the globbing with less memory churn, and also
   avoid adding daemon-excluded items to the returned args.

### DEVELOPER RELATED:

 - The configure script tries to get the user's compiler to not warn about
   unused function parameters if the build is not including one or more of the
   ACL/xattrs/iconv features.

 - The configure script now has better checks for figuring out if the included
   popt code should be used or not.

 - Fixed two testsuite glitches: avoid a failure if someone's `cd` command
   outputs the current directory when cd-ing to a relative path, and made the
   itemized test query how rsync was built to determine if it should expect
   hard-linked symlinks or not.

 - Updated the testsuite to verify that various bug fixes remain fixed.

 - The RPM spec file was updated to have: (1) comments for how to use the
   rsync-patch tar file, and (2) an /etc/xinetd.d/rsync file.

 - Updated the build scripts to work with a revised FTP directory structure.

------------------------------------------------------------------------------

# NEWS for rsync 3.0.0 (1 Mar 2008)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 30.

### NOTABLE CHANGES IN BEHAVIOR:

 - The handling of implied directories when using `--relative` has changed to
   send them as directories (e.g. no implied dir is ever sent as a symlink).
   This avoids unexpected behavior and should not adversely affect most people.
   If you're one of those rare individuals who relied upon having an implied
   dir be duplicated as a symlink, you should specify the transfer of the
   symlink and the transfer of the referent directory as separate args. (See
   also `--keep-dirlinks` and `--no-implied-dirs`.) Also, exclude rules no
   longer have a partial effect on implied dirs.

 - Requesting a remote file-listing without specifying `-r` (`--recursive`) now
   sends the `-d` (`--dirs`) option to the remote rsync rather than sending
   `-r` along with an extra exclude of `/*/*`. If the remote rsync does not
   understand the `-d` option (i.e. it is 2.6.3 or older), you will need to
   either turn off `-d` (`--no-d`), or specify `-r --exclude='/*/*'` manually.

 - In `--dry-run` mode, the last line of the verbose summary text is output
   with a "(DRY RUN)" suffix to help remind you that no updates were made.
   Similarly, `--only-write-batch` outputs `(BATCH ONLY)`.

 - A writable rsync daemon with `use chroot` disabled now defaults to a
   symlink-munging behavior designed to make symlinks safer while also allowing
   absolute symlinks to be stored and retrieved. This also has the effect of
   making symlinks unusable while they're in the daemon's hierarchy. See the
   daemon's `munge symlinks` parameter for details.

 - Starting up an extra copy of an rsync daemon will not clobber the pidfile
   for the running daemon -- if the pidfile exists, the new daemon will exit
   with an error. This means that your wrapper script that starts the rsync
   daemon should be made to handle lock-breaking (if you want any automatic
   breaking of locks to be done).

### BUG FIXES:

 - A daemon with `use chroot = no` and excluded items listed in the daemon
   config file now properly checks an absolute-path arg specified for these
   options: `--compare-dest`, `--link-dest`, `--copy-dest`, `--partial-dir`,
   `--backup-dir`, `--temp-dir`, and `--files-from`.

 - A daemon can now be told to disable all user- and group-name translation on
   a per-module basis. This avoids a potential problem with a writable daemon
   module that has `use chroot` enabled -- if precautions weren't taken, a user
   could try to add a missing library and get rsync to use it. This makes rsync
   safer by default, and more configurable when id-translation is not desired.
   See the daemon's `numeric ids` parameter for full details.

 - A chroot daemon can now indicate which part of its path should affect the
   chroot call, and which part should become an inside-chroot path for the
   module. This allows you to have outside-the-transfer paths (such as for
   libraries) even when you enable chroot protection. The idiom used in the
   rsyncd.conf file is: `path = /chroot/dirs/./dirs/inside`

 - If a file's data arrived successfully on the receiving side but the rename
   of the temporary file to the destination file failed AND the
   `--remove-source-files` (or the deprecated `--remove-sent-files`) option was
   specified, rsync no longer erroneously removes the associated source file.

 - Fixed the output of `-ii` when combined with one of the `--*-dest` options:
   it now itemizes all the items, not just the changed ones.

 - Made the output of all file types consistent when using a `--*-dest` option.
   Prior versions would output too many creation events for matching items.

 - The code that waits for a child pid now handles being interrupted by a
   signal. This fixes a problem with the pre-xfer exec function not being able
   to get the exit status from the script.

 - A negated filter rule (i.e. with a '!' modifier) no longer loses the
   negation when sending the filter rules to the remote rsync.

 - Fixed a problem with the `--out-format` (aka `--log-format`) option %f: it
   no longer outputs superfluous directory info for a non-daemon rsync.

 - Fixed a problem with `-vv` (double `--verbose`) and `--stats` when `pushing`
   files (which includes local copies). Version 2.6.9 would complete the copy,
   but exit with an error when the receiver output its memory stats.

 - If `--password-file` is used on a non-daemon transfer, rsync now complains
   and exits. This should help users figure out that they can't use this option
   to control a remote shell's password prompt.

 - Make sure that directory permissions of a newly-created destination
   directory are handled right when `--perms` is left off.

 - The itemized output of a newly-created destination directory is now output
   as a creation event, not a change event.

 - Improved `--hard-link` so that more corner cases are handled correctly when
   combined with options such as `--link-dest` and/or `--ignore-existing`.

 - The `--append` option no longer updates a file that has the same size.

 - Fixed a bug when combining `--backup` and `--backup-dir` with `--inplace`:
   any missing backup directories are now created.

 - Fixed a bug when using `--backup` and `--inplace` with `--whole-file` or
   `--read-batch`: backup files are actually created now.

 - The daemon pidfile is checked and created sooner in the startup sequence.

 - If a daemon module's `path` value is not an absolute pathname, the code now
   makes it absolute internally (making it work properly).

 - Ensure that a temporary file always has owner-write permission while we are
   writing to it. This avoids problems with some network filesystems when
   transferring read-only files.

 - Any errors output about password-file reading no longer cause an error at
   the end of the run about a partial transfer.

 - The `--read-batch` option for protocol 30 now ensures that several more
   options are set correctly for the current batch file: `--iconv`, `--acls`,
   `--xattrs`, `--inplace`, `--append`, and `--append-verify`.

 - Using `--only-write-batch` to a daemon receiver now works properly (older
   versions would update some files while writing the batch).

 - Avoid outputting a "file has vanished" message when the file is a broken
   symlink and `--copy-unsafe-links` or `--copy-dirlinks` is used (the code
   already handled this for `--copy-links`).

 - Fixed the combination of `--only-write-batch` and `--dry-run`.

 - Fixed rsync's ability to remove files that are not writable by the file's
   owner when rsync is running as the same user.

 - When transferring large files, the sender's hashtable of checksums is kept
   at a more reasonable state of fullness (no more than 80% full) so that the
   scanning of the hashtable will not bog down as the number of blocks
   increases.

### ENHANCEMENTS:

 - A new incremental-recursion algorithm is now used when rsync is talking to
   another 3.x version. This starts the transfer going more quickly (before all
   the files have been found), and requires much less memory.  See the
   `--recursive` option in the manpage for some restrictions.

 - Lowered memory use in the non-incremental-recursion algorithm for typical
   option values (usually saving from 21-29 bytes per file).

 - The default `--delete` algorithm is now `--delete-during` when talking to a
   3.x rsync. This is a faster scan than using `--delete-before` (which is the
   default when talking to older rsync versions), and is compatible with the
   new incremental recursion mode.

 - Rsync now allows multiple remote-source args to be specified rather than
   having to rely on a special space-splitting side-effect of the remote-
   shell. Additional remote args must specify the same host or an empty one
   (e.g. empty: `:file1` or `::module/file2`). For example, this means that
   local use of brace expansion now works: `rsync -av host:dir/{f1,f2} .`

 - Added the `--protect-args` (`-s`) option, that tells rsync to send most of
   the command-line args at the start of the transfer rather than as args to
   the remote-shell command. This protects them from space-splitting, and only
   interprets basic wildcard special shell characters (`*?[`).

 - Added the `--delete-delay` option, which is a more efficient way to delete
   files at the end of the transfer without needing a separate delete pass.

 - Added the `--acls` (`-A`) option to preserve Access Control Lists. This is
   an improved version of the prior patch that was available, and it even
   supports OS X ACLs. If you need to have backward compatibility with old,
   ACL-patched versions of rsync, apply the acls.diff file from the patches
   dir.

 - Added the `--xattrs` (`-X`) option to preserve extended attributes. This is
   an improved version of the prior patch that was available, and it even
   supports OS X xattrs (which includes their resource fork data). If you need
   to have backward compatibility with old, xattr-patched versions of rsync,
   apply the xattrs.diff file from the patches dir.

 - Added the `--fake-super` option that allows a non-super user to preserve all
   attributes of a file by using a special extended-attribute idiom.  It even
   supports the storing of foreign ACL data on your backup server.  There is
   also an analogous `fake super` parameter for an rsync daemon.

 - Added the `--iconv` option, which allows rsync to convert filenames from one
   character-set to another during the transfer. The default is to make this
   feature available as long as your system has `iconv_open()`.  If compilation
   fails, specify `--disable-iconv` to configure, and then rebuild. If you want
   rsync to perform character-set conversions by default, you can specify
   `--enable-iconv=CONVERT_STRING` with the default value for the `--iconv`
   option that you wish to use. For example, `--enable-iconv=.` is a good
   choice. See the rsync manpage for an explanation of the `--iconv` option's
   settings.

 - A new daemon config parameter, `charset`, lets you control the character-
   set that is used during an `--iconv` transfer to/from a daemon module. You
   can also set your daemon to refuse `no-iconv` if you want to force the
   client to use an `--iconv` transfer (requiring an rsync 3.x client).

 - Added the `--skip-compress=LIST` option to override the default list of file
   suffixes that will not be compressed when using `--compress` (`-z`).

 - The daemon's default for `dont compress` was extended to include: `*.7z`
   `*.mp[34]` `*.mov` `*.avi` `*.ogg` `*.jpg` `*.jpeg` and the name-matching routine was also
   optimized to run more quickly.

 - The `--max-delete` option now outputs a warning if it skipped any file
   deletions, including a count of how many deletions were skipped. (Older
   versions just silently stopped deleting things.)

 - You may specify `--max-delete=0` to a 3.0.0 client to request that it warn
   about extraneous files without deleting anything. If you're not sure what
   version the client is, you can use the less-obvious `--max-delete=-1`, as
   both old and new versions will treat that as the same request (though older
   versions don't warn).

 - The `--hard-link` option now uses less memory on both the sending and
   receiving side for all protocol versions. For protocol 30, the use of a
   hashtable on the sending side allows us to more efficiently convey to the
   receiver what files are linked together. This reduces the amount of data
   sent over the socket by a considerable margin (rather than adding more
   data), and limits the in-memory storage of the device+inode information to
   just the sending side for the new protocol 30, or to the receiving side when
   speaking an older protocol (note that older rsync versions kept the
   device+inode information on both sides).

 - The filter rules now support a perishable (`p`) modifier that marks rules
   that should not have an effect in a directory that is being deleted. e.g.
   `-f '-p .svn/'` would only affect `live` .svn directories.

 - Rsync checks all the alternate-destination args for validity (e.g.
   `--link-dest`). This lets the user know when they specified a directory that
   does not exist.

 - If we get an ENOSYS error setting the time on a symlink, we don't complain
   about it anymore (for those systems that even support the setting of the
   modify-time on a symlink).

 - Protocol 30 now uses MD5 checksums instead of MD4.

 - Changed the `--append` option to not checksum the existing data in the
   destination file, which speeds up file appending.

 - Added the `--append-verify` option, which works like the older `--append`
   option (verifying the existing data in the destination file). For
   compatibility with older rsync versions, any use of `--append` that is
   talking protocol 29 or older will revert to the `--append-verify` method.

 - Added the `--contimeout=SECONDS` option that lets the user specify a
   connection timeout for rsync daemon access.

 - Documented and extended the support for the `RSYNC_CONNECT_PROG` variable
   that can be used to enhance the client side of a daemon connection.

 - Improved the dashes and double-quotes in the nroff manpage output.

 - Rsync now supports a lot more `--no-OPTION` override options.

### INTERNAL:

 - The file-list sorting algorithm now uses a sort that keeps any same-named
   items in the same order as they were specified. This allows rsync to always
   ensure that the first of the duplicates is the one that will be included in
   the copy. The new sort is also faster than the glibc version of qsort() and
   mergesort().

 - Rsync now supports the transfer of 64-bit timestamps (`time_t` values).

 - Made the file-deletion code use a little less stack when recursing through a
   directory hierarchy of extraneous files.

 - Fixed a build problem with older (2.x) versions of gcc.

 - Added some isType() functions that make dealing with signed characters
   easier without forcing variables via casts.

 - Changed strcat/strcpy/sprintf function calls to use safer versions.

 - Upgraded the included popt version to 1.10.2 and improved its use of
   string-handling functions.

 - Added missing prototypes for compatibility functions from the lib dir.

 - Configure determines if iconv() has a const arg, allowing us to avoid a
   compiler warning.

 - Made the sending of some numbers more efficient for protocol 30.

 - Make sure that a daemon process doesn't mind if the client was weird and
   omitted the `--server` option.

 - There are more internal logging categories available in protocol 30 than the
   age-old FINFO and FERROR, including `FERROR_XFER` and FWARN. These new
   categories allow some errors and warnings to go to stderr without causing an
   erroneous end-of-run warning about some files not being able to be
   transferred.

 - Improved the use of `const` on pointers.

 - Improved J.W.'s `pool_alloc` routines to add a way of incrementally freeing
   older sections of a pool's memory.

 - The getaddrinfo.c compatibility code in the `lib` dir was replaced with some
   new code (derived from samba, derived from PostgreSQL) that has a better
   license than the old code.

### DEVELOPER RELATED:

 - Rsync is now licensed under the GPLv3 or later.

 - Rsync is now being maintained in a `git` repository instead of CVS (though
   the old CVS repository still exists for historical access).  Several
   maintenance scripts were updated to work with git.

 - Generated files are no longer committed into the source repository. The
   autoconf and autoheader commands are now automatically run during the normal
   use of `configure` and `make`. The latest dev versions of all generated
   files can also be copied from the samba.org web site (see the prepare-source
   script's fetch option).

 - The `patches` directory of diff files is now built from branches in the
   rsync git repository (branch patch/FOO creates file patches/FOO.diff).  This
   directory is now distributed in a separate separate tar file named
   rsync-patches-VERSION.tar.gz instead of the main rsync-VERSION.tar.gz.

 - The proto.h file is now built using a simple perl script rather than a
   complex awk script, which proved to be more widely compatible.

 - When running the tests, we now put our per-test temp dirs into a sub-
   directory named testtmp (which is created, if missing). This allows someone
   to symlink the testtmp directory to another filesystem (which is useful if
   the build dir's filesystem does not support ACLs and xattrs, but another
   filesystem does).

 - Rsync now has a way of handling protocol-version changes during the
   development of a new protocol version. This causes any out-of-sync versions
   to speak an older protocol rather than fail in a cryptic manner.  This
   addition makes it safer to deploy a pre-release version that may interact
   with the public. This new exchange of sub-version info does not interfere
   with the `{MIN,MAX}_PROTOCOL_VERSION` checking algorithm (which does not
   have enough range to allow the main protocol number to be incremented for
   every minor tweak in that happens during development).

 - The csprotocol.txt file was updated to mention the daemon protocol change in
   the 3.0.0 release.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.9 (6 Nov 2006)

## Changes in this version:

### BUG FIXES:

 - If rsync is interrupted via a handled signal (such as SIGINT), it will once
   again clean-up its temp file from the destination dir.

 - Fixed an overzealous sanitizing bug in the handling of the `--link-dest`,
   `--copy-dest`, and `--compare-dest` options to a daemon without chroot: if
   the copy's destination dir is deeper than the top of the module's path,
   these options now accept a safe number of parent-dir (../) references (since
   these options are relative to the destination dir). The old code incorrectly
   chopped off all `../` prefixes for these options, no matter how deep the
   destination directory was in the module's hierarchy.

 - Fixed a bug where a deferred info/error/log message could get sent directly
   to the sender instead of being handled by rwrite() in the generator. This
   fixes an `unexpected tag 3` fatal error, and should also fix a potential
   problem where a deferred info/error message from the receiver might bypass
   the log file and get sent only to the client process. (These problems could
   only affect an rsync daemon that was receiving files.)

 - Fixed a bug when `--inplace` was combined with a `--*-dest` option and we
   update a file's data using an alternate basis file. The code now notices
   that it needs to copy the matching data from the basis file instead of
   (wrongly) assuming that it was already present in the file.

 - Fixed a bug where using `--dry-run` with a `--*-dest` option with a path
   relative to a directory that does not yet exist: the affected option gets
   its proper path value so that the output of the dry-run is right.

 - Fixed a bug in the %f logfile escape when receiving files: the destination
   path is now included in the output (e.g. you can now tell when a user
   specifies a subdir inside a module).

 - If the receiving side fails to create a directory, it will now skip trying
   to update everything that is inside that directory.

 - If `--link-dest` is specified with `--checksum` but without `--times`, rsync
   will now allow a hard-link to be created to a matching link-dest file even
   when the file's modify-time doesn't match the server's file.

 - The daemon now calls more timezone-using functions prior to doing a chroot.
   This should help some C libraries to generate proper timestamps from inside
   a chrooted daemon (and to not try to access /etc/timezone over and over
   again).

 - Fixed a bug in the handling of an absolute `--partial-dir=ABS_PATH` option:
   it now deletes an alternate basis file from the partial-dir that was used to
   successfully update a destination file.

 - Fixed a bug in the handling of `--delete-excluded` when using a per-dir
   merge file: the merge file is now honored on the receiving side, and only
   its unqualified include/exclude commands are ignored (just as is done for
   global include/excludes).

 - Fixed a recent bug where `--delete` was not working when transferring from
   the root (/) of the filesystem with `--relative` enabled.

 - Fixed a recent bug where an `--exclude='*'` could affect the root (/) of the
   filesystem with `--relative` enabled.

 - When `--inplace` creates a file, it is now created with owner read/write
   permissions (0600) instead of no permissions at all. This avoids a problem
   continuing a transfer that was interrupted (since `--inplace` will not
   update a file that has no write permissions).

 - If either `--remove-source-files` or `--remove-sent-files` is enabled and we
   are unable to remove the source file, rsync now outputs an error.

 - Fixed a bug in the daemon's `incoming chmod` rule: newly-created directories
   no longer get the 'F' (file) rules applied to them.

 - Fixed an infinite loop bug when a filter rule was rejected due to being
   overly long.

 - When the server receives a `--partial-dir` option from the client, it no
   longer runs the client-side code that adds an assumed filter rule (since the
   client will be sending us the rules in the usual manner, and they may have
   chosen to override the auto-added rule).

### ENHANCEMENTS:

 - Added the `--log-file=FILE` and `--log-file-format=FORMAT` options. These
   can be used to tell any rsync to output what it is doing to a log file.
   They work with a client rsync, a non-daemon server rsync (see the manpage
   for instructions), and also allows the overriding of rsyncd.conf settings
   when starting a daemon.

 - The `--log-format` option was renamed to be `--out-format` to avoid
   confusing it with affecting the log-file output. (The old option remains as
   an alias for the new to preserve backward compatibility.)

 - Made `log file` and `syslog facility` settable on a per-module basis in the
   daemon's config file.

 - Added the `--remove-source-files` option as a replacement for the (now
   deprecated) `--remove-sent-files` option. This new option removes all
   non-dirs from the source directories, even if the file was already
   up-to-date. This fixes a problem where interrupting an rsync that was using
   `--remove-sent-files` and restarting it could leave behind a file that the
   earlier rsync synchronized, but didn't get to remove.  (The deprecated
   `--remove-sent-files` is still understood for now, and still behaves in the
   same way as before.)

 - Added the option `--no-motd` to suppress the message-of-the-day output from
   a daemon when doing a copy. (See the manpage for a caveat.)

 - Added a new environment variable to the pre-/post-xfer exec commands (in the
   daemon's config file): `RSYNC_PID`. This value will be the same in both the
   pre- and post-xfer commands, so it can be used as a unique ID if the
   pre-xfer command wants to cache some arg/request info for the post-xfer
   command.

### INTERNAL:

 - Did a code audit using IBM's code-checker program and made several changes,
   including: replacing most of the strcpy() and sprintf() calls with
   strlcpy(), snprintf(), and memcpy(), adding a 0-value to an enum that had
   been intermingling a literal 0 with the defined enum values, silencing some
   uninitialized memory checks, marking some functions with a `noreturn`
   attribute, and changing an `if` that could never succeed on some platforms
   into a pre-processor directive that conditionally compiles the code.

 - Fixed a potential bug in `f_name_cmp()` when both the args are a top-level
   `.` dir (which doesn't happen in normal operations).

 - Changed `exit_cleanup()` so that it can never return instead of exit.  The
   old code might return if it found the `exit_cleanup()` function was being
   called recursively. The new code is segmented so that any recursive calls
   move on to the next step of the exit-processing.

 - The macro WIFEXITED(stat) will now be defined if the OS didn't already
   define it.

### DEVELOPER RELATED:

 - The acls.diff and xattrs.diff patches have received a bunch of work to make
   them much closer to being acceptable in the main distribution.  The xattrs
   patch also has some preliminary Mac OS X and FreeBSD compatibility code that
   various system types to exchange extended file-attributes.

 - A new diff in the patches dir, fake-root.diff, allows rsync to maintain a
   backup hierarchy with full owner, group, and device info without actually
   running as root. It does this using a special extended attribute, so it
   depends on xattrs.diff (which depends on acls.diff).

 - The rsync.yo and rsyncd.conf.yo files have been updated to work better with
   the latest yodl 2.x releases.

 - Updated config.guess and config.sub to their 2006-07-02 versions.

 - Updated various files to include the latest FSF address and to have
   consistent opening comments.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.8 (22 Apr 2006)

## Changes in this version:

### BUG FIXES:

 - Fixed a bug in the exclude code where an anchored exclude without any
   wildcards fails to match an absolute source arg, but only when `--relative`
   is in effect.

 - Improved the I/O code for the generator to fix a potential hang when the
   receiver gets an EOF on the socket but the generator's select() call never
   indicates that the socket is writable for it to be notified about the EOF.
   (This can happen when using stunnel).

 - Fixed a problem with the file-reading code where a failed read (such as that
   caused by a bad sector) would not advance the file's read-position beyond
   the failed read's data.

 - Fixed a logging bug where the `log file` directive was not being honored in
   a single-use daemon (one spawned by a remote-shell connection or by init).

 - If rsync cannot honor the `--delete` option, we output an error and exit
   instead of silently ignoring the option.

 - Fixed a bug in the `--link-dest` code that prevented special files (such as
   fifos) from being linked.

 - The ability to hard-link symlinks and special files is now determined at
   configure time instead of at runtime. This fixes a bug with `--link-dest`
   creating a hard-link to a symlink's referent on a BSD system.

### ENHANCEMENTS:

 - In daemon mode, if rsync fails to bind to the requested port, the error(s)
   returned by socket() and/or bind() are now logged.

 - When we output a fatal error, we now output the version of rsync in the
   message.

 - Improved the documentation for the `--owner` and `--group` options.

 - The rsyncstats script in `support` has an improved line-parsing regex that
   is easier to read and also makes it to parse syslog-generated lines.

 - A new script in `support`: file-attr-restore, can be used to restore the
   attributes of a file-set (the permissions, ownership, and group info) taken
   from the cached output of a `find ARG... -ls` command.

### DEVELOPER RELATED:

 - Removed the unused function `write_int_named()`, the unused variable
   `io_read_phase`, and the rarely used variable `io_write_phase`. This also
   elides the confusing 'phase "unknown"' part of one error message.

 - Removed two unused configure checks and two related (also unused)
   compatibility functions.

 - The xattrs.diff patch received a security fix that prevents a potential
   buffer overflow in the `receive_xattr()` code.

 - The acls.diff patch has been improved quite a bit, with more to come.

 - A new patch was added: log-file.diff. This contains an early version of a
   future option, `--log-file=FILE`, that will allow any rsync to log its
   actions to a file (something that only a daemon supports at present).

------------------------------------------------------------------------------

# NEWS for rsync 2.6.7 (11 Mar 2006)

## Changes in this version:

### OUTPUT CHANGES:

 - The letter 'D' in the itemized output was being used for both devices
   (character or block) as well as other special files (such as fifos and named
   sockets). This has changed to separate non-device special files under the
   'S' designation (e.g. `cS+++++++ path/fifo`). See also the `--specials`
   option, below.

 - The way rsync escapes unreadable characters has changed. First, rsync now
   has support for recognizing valid multi-byte character sequences in your
   current locale, allowing it to escape fewer characters than before for a
   locale such as UTF-8. Second, it now uses an escape idiom of `\#123`, which
   is the literal string `\#` followed by exactly 3 octal digits. Rsync no
   longer doubles a backslash character in a filename (e.g. it used to output
   `foo\\bar` when copying `foo\bar`) -- now it only escapes a backslash that
   is followed by a hash-sign and 3 digits (0-9) (e.g. it will output
   `foo\#134#789` when copying `foo\#789`). See also the `--8-bit-output`
   (`-8`) option, mentioned below.

   Script writers: the local rsync is the one that outputs escaped names, so if
   you need to support unescaping of filenames for older rsyncs, I'd suggest
   that you parse the output of `rsync --version` and only use the old
   unescaping rules for 2.6.5 and 2.6.6.

### BUG FIXES:

 - Fixed a really old bug that caused `--checksum` (`-c`) to checksum all the
   files encountered during the delete scan (ouch).

 - Fixed a potential hang in a remote generator: when the receiver gets a
   read-error on the socket, it now signals the generator about this so that
   the generator does not try to send any of the terminating error messages to
   the client (avoiding a potential hang in some setups).

 - Made hard-links work with symlinks and devices again.

 - If the sender gets an early EOF reading a source file, we propagate this
   error to the receiver so that it can discard the file and try requesting it
   again (which is the existing behavior for other kinds of read errors).

 - If a device-file/special-file changes permissions, rsync now updates the
   permissions without recreating the file.

 - If the user specifies a remote-host for both the source and destination, we
   now output a syntax error rather than trying to open the destination
   hostspec as a filename.

 - When `--inplace` creates a new destination file, rsync now creates it with
   permissions 0600 instead of 0000 -- this makes restarting possible when the
   transfer gets interrupted in the middle of sending a new file.

 - Reject the combination of `--inplace` and `--sparse` since the sparse-output
   algorithm doesn't work when overwriting existing data.

 - Fixed the directory name in the error that is output when `pop_dir()` fails.

 - Really fixed the parsing of a `!` entry in .cvsignore files this time.

 - If the generator gets a stat() error on a file, output it (this used to
   require at least `-vv` for the error to be seen).

 - If waitpid() fails or the child rsync didn't exit cleanly, we now handle the
   exit status properly and generate a better error.

 - Fixed some glitches in the double-verbose output when using `--copy-dest`,
   `--link-dest`, or `--compare-dest`. Also improved how the verbose output
   handles hard-links (within the transfer) that had an up-to-date alternate
   `dest` file, and copied files (via `--copy-dest`).

 - Fixed the matching of the dont-compress items (e.g. `*.gz`) against files
   that have a path component containing a slash.

 - If the code reading a filter/exclude file gets an EINTR error, rsync now
   clears the error flag on the file handle so it can keep on reading.

 - If `--relative` is active, the sending side cleans up trailing `/` or `/.`
   suffixes to avoid triggering a bug in older rsync versions. Also, we now
   reject a `..` dir if it would be sent as a relative dir.

 - If a non-directory is in the way of a directory and rsync is run with
   `--dry-run` and `--delete`, rsync no longer complains about not being able
   to opendir() the not-yet present directory.

 - When `--list-only` is used and a non-existent local destination dir was also
   specified as a destination, rsync no longer generates a warning about being
   unable to create the missing directory.

 - Fixed some problems with `--relative --no-implied-dirs` when the destination
   directory did not yet exist: we can now create a symlink or device when it
   is the first thing in the missing dir, and `--fuzzy` no longer complains
   about being unable to open the missing dir.

 - Fixed a bug where the `--copy-links` option would not affect implied
   directories without `--copy-unsafe-links` (see `--relative`).

 - Got rid of the need for `--force` to be used in some circumstances with
   `--delete-after` (making it consistent with
   `--delete-before`/`--delete-during`).

 - Rsync now ignores the SIGXFSZ signal, just in case your OS sends this when a
   file is too large (rsync handles the write error).

 - Fixed a bug in the Proxy-Authorization header's base64-encoded value: it was
   not properly padded with trailing '=' chars. This only affects a user that
   need to use a password-authenticated proxy for an outgoing daemon-rsync
   connection.

 - If we're transferring an empty directory to a new name, rsync no longer
   forces `S_IWUSR` if it wasn't already set, nor does it accidentally leave it
   set.

 - Fixed a bug in the debug output (`-vvvvv`) that could mention the wrong
   checksum for the current file offset.

 - Rsync no longer allows a single directory to be copied over a non-directory
   destination arg.

### ENHANCEMENTS:

 - Added the `--append` option that makes rsync append data onto files that are
   longer on the source than the destination (this includes new files).

 - Added the `--min-size=SIZE` option to exclude small files from the transfer.

 - Added the `--compress-level` option to allow you to set how aggressive
   rsync's compression should be (this option implies `--compress`).

 - Enhanced the parsing of the SIZE value for `--min-size` and `--max-size` to
   allow easy entry of multiples of 1000 (instead of just multiples of 1024)
   and off-by-one values too (e.g. `--max-size=8mb-1`).

 - Added the `--8-bit-output` (`-8`) option, which tells rsync to avoid
   escaping high-bit characters that it thinks are unreadable in the current
   locale.

 - The new option `--human-readable` (`-h`) changes the output of `--progress`,
   `--stats`, and the end-of-run summary to be easier to read. If repeated, the
   units become powers of 1024 instead of powers of 1000. (The old meaning of
   `-h`, as a shorthand for `--help`, still works as long as you just use it on
   its own, as in `rsync -h`.)

 - If lutimes() and/or lchmod() are around, use them to allow the preservation
   of attributes on symlinks.

 - The `--link-dest` option now affects symlinks and devices (when possible).

 - Added two config items to the rsyncd.conf parsing: `pre-xfer exec` and
   `post-xfer exec`. These allow a command to be specified on a per-module
   basis that will be run before and/or after a daemon-mode transfer. (See the
   manpage for a list of the environment variables that are set with
   information about the transfer.)

 - When using the `--relative` option, you can now insert a dot dir in the
   source path to indicate where the replication of the source dirs should
   start. For example, if you specify a source path of
   rsync://host/module/foo/bar/./baz/dir with `-R`, rsync will now only
   replicate the `baz/dir` part of the source path (note: a trailing dot dir is
   unaffected unless it also has a trailing slash).

 - Added some new `--no-FOO` options that make it easier to override unwanted
   implied or default options. For example, `-a --no-o` (aka `--archive
   --no-owner`) can be used to turn off the preservation of file ownership that
   is implied by `-a`.

 - Added the `--chmod=MODE` option that allows the destination permissions to
   be changed from the source permissions. E.g. `--chmod=g+w,o-rwx`

 - Added the `incoming chmod` and `outgoing chmod` daemon options that allow a
   module to specify what permissions changes should be applied to all files
   copied to and from the daemon.

 - Allow the `--temp-dir` option to be specified when starting a daemon, which
   sets the default temporary directory for incoming files.

 - If `--delete` is combined with `--dirs` without `--recursive`, rsync will
   now delete in any directory whose content is being synchronized.

 - If `--backup` is combined with `--delete` without `--backup-dir` (and
   without `--delete-excluded`), we add a `protect` filter-rule to ensure that
   files with the backup suffix are not deleted.

 - The file-count stats that are output by `--progress` were improved to better
   indicate what the numbers mean. For instance, the output: `(xfer#5,
   to-check=8383/9999)` indicates that this was the fifth file to be
   transferred, and we still need to check 8383 more files out of a total of
   9999.

 - The include/exclude code now allows a `dir/***` directive (with 3 trailing
   stars) to match both the dir itself as well as all the content below the dir
   (`dir/**` would not match the dir).

 - Added the `--prune-empty-dirs` (`-m`) option that makes the receiving rsync
   discard empty chains of directories from the file-list. This makes it easier
   to selectively copy files from a source hierarchy and end up with just the
   directories needed to hold the resulting files.

 - If the `--itemize-changes` (`-i`) option is repeated, rsync now includes
   unchanged files in the itemized output (similar to `-vv`, but without all
   the other verbose messages that can get in the way). Of course, the client
   must be version 2.6.7 for this to work, but the remote rsync only needs to
   be 2.6.7 if you're pushing files.

 - Added the `--specials` option to tell rsync to copy non-device special files
   (which rsync now attempts even as a normal user). The `--devices` option now
   requests the copying of just devices (character and block).  The `-D` option
   still requests both (e.g. `--devices` and `--specials`), `-a` still implies
   `-D`, and non-root users still get a silent downgrade that omits device
   copying.

 - Added the `--super` option to make the receiver always attempt super-user
   activities. This is useful for systems that allow things such as devices to
   be created or ownership to be set without being UID 0, and is also useful
   for someone who wants to ensure that errors will be output if the receiving
   rsync isn't being run as root.

 - Added the `--sockopts` option for those few who want to customize the TCP
   options used to contact a daemon rsync.

 - Added a way for the `--temp-dir` option to be combined with a partial-dir
   setting that lets rsync avoid non-atomic updates (for those times when
   `--temp-dir` is not being used because space is tight).

 - A new support script, files-to-excludes, will transform a list of files into
   a set of include/exclude directives that will copy those files.

 - A new option, `--executability` (`-E`) can be used to preserve just the
   execute bit on files, for those times when using the `--perms` option is not
   desired.

 - The daemon now logs each connection and also each module-list request that
   it receives.

 - New log-format options: %M (modtime), %U (uid), %G (gid), and %B (permission
   bits, e.g. `rwxr-xrwt`).

 - The `--dry-run` option no longer forces the enabling of `--verbose`.

 - The `--remove-sent-files` option now does a better job of incrementally
   removing the sent files on the sending side (older versions tended to clump
   up all the removals at the end).

 - A daemon now supersedes its minimal SIGCHLD handler with the standard
   PID-remembering version after forking. This ensures that the generator can
   get the child-exit status from the receiver.

 - Use of the `--bwlimit` option no longer interferes with the remote rsync
   sending error messages about invalid/refused options.

 - Rsync no longer returns a usage error when used with one local source arg
   and no destination: this now implies the `--list-only` option, just like the
   comparable situation with a remote source arg.

 - Added the `--copy-dirlinks` option, a more limited version of
   `--copy-links`.

 - Various documentation improvements, including: a better synopsis, some
   improved examples, a better discussion of the presence and absence of
   `--perms` (including how it interacts with the new `--executability` and
   `--chmod` options), an extended discussion of `--temp-dir`, an improved
   discussion of `--partial-dir`, a better description of rsync's pattern
   matching characters, an improved `--no-implied-dirs` section, and the
   documenting of what the `--stats` option outputs.

 - Various new and updated diffs in the patches dir, including: acls.diff,
   xattrs.diff, atimes.diff, detect-renamed.diff, and slp.diff.

### INTERNAL:

 - We now use sigaction() and sigprocmask() if possible, and fall back on
   signal() if not. Using sigprocmask() ensures that rsync enables all the
   signals that it needs, just in case it was started in a masked state.

 - Some buffer sizes were expanded a bit, particularly on systems where
   MAXPATHLEN is overly small (e.g. Cygwin).

 - If `io_printf()` tries to format more data than fits in the buffer, exit
   with an error instead of transmitting a truncated buffer.

 - If a `va_copy` macro is defined, lib/snprintf.c will use it when defining
   the `VA_COPY` macro.

 - Reduced the amount of stack memory needed for each level of directory
   recursion by nearly MAXPATHLEN bytes.

 - The wildmatch function was extended to allow an array of strings to be
   supplied as the string to match. This allows the exclude code to do less
   string copying.

 - Got rid of the `safe_fname()` function (and all the myriad calls) and
   replaced it with a new function in the log.c code that filters all the
   output going to the terminal.

 - Unified the `f_name()` and the `f_name_to()` functions.

 - Improved the hash-table code the sender uses to handle checksums to make it
   use slightly less memory and run just a little faster.

### DEVELOPER RELATED:

 - The diffs in the patches dir now require `patch -p1 <DIFF` instead of the
   previous `-p0`. Also, the version included in the release tar now affect
   generated files (e.g. configure, rsync.1, proto.h, etc.), so it is no longer
   necessary to run autoconf and/or yodl unless you're applying a patch that
   was checked out from CVS.

 - Several diffs in the patches dir now use the proper `--enable-FOO` configure
   option instead of `--with-FOO` to turn on the inclusion of the newly patched
   feature.

 - There is a new script, `prepare-source` than can be used to update the
   various generated files (proto.h, configure, etc.) even before configure has
   created the Makefile (this is mainly useful when patching the source with a
   patch that doesn't affect generated files).

 - The testsuite now sets HOME so that it won't be affected by a file such as
   ~/.popt.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.6 (28 Jul 2005)

## Changes in this version:

### SECURITY FIXES:

 - The zlib code was upgraded to version 1.2.3 in order to make it more secure.
   While the widely-publicized security problem in zlib 1.2.2 did not affect
   rsync, another security problem surfaced that affects rsync's zlib 1.1.4.

### BUG FIXES:

 - The setting of `flist->high` in `clean_flist()` was wrong for an empty list.
   This could cause `flist_find()` to crash in certain rare circumstances (e.g.
   if just the right directory setup was around when `--fuzzy` was combined
   with `--link-dest`).

 - The outputting of hard-linked files when verbosity was > 1 was not right:
   (1) Without `-i` it would output the name of each hard-linked file as though
   it had been changed; it now outputs a `is hard linked` message for the file.
   (2) With `-i` it would output all dots for the unchanged attributes of a
   hard-link; it now changes those dots to spaces, as is done for other totally
   unchanged items.

 - When backing up a changed symlink or device, get rid of any old backup item
   so that we don't get an `already exists` error.

 - A couple places that were comparing a local and a remote modification-time
   were not honoring the `--modify-window` option.

 - Fixed a bug where the 'p' (permissions) itemized-changes flag might get set
   too often (if some non-significant mode bits differed).

 - Fixed a really old, minor bug that could cause rsync to warn about being
   unable to mkdir() a path that ends in `/.` because it just created the
   directory (required `--relative`, `--no-implied-dirs`, a source path that
   ended in either a trailing slash or a trailing `/.`, and a non-existing
   destination dir to tickle the bug in a recent version).

### ENHANCEMENTS:

 - Made the `max verbosity` setting in the rsyncd.conf file settable on a
   per-module basis (which now matches the documentation).

 - The rrsync script has been upgraded to verify the args of options that take
   args (instead of rejecting any such options). It was also changed to try to
   be more secure and to fix a problem in the parsing of a pull operation that
   has multiple source args. (See the support dir.)

 - Improved the documentation that explains the difference between a normal
   daemon transfer and a daemon-over remote-shell transfer.

 - Some of the diffs supplied in the patches dir were fixed and/or improved.

### BUILD CHANGES:

 - Made configure define `NOBODY_USER` (currently hard-wired to `nobody`) and
   `NOBODY_GROUP` (set to either `nobody` or `nogroup` depending on what we
   find in the /etc/group file).

 - Added a test to the test suite, itemized.test, that tests the output of `-i`
   (log-format w/%i) and some double-verbose messages.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.5 (1 Jun 2005)

## Changes in this version:

### OUTPUT CHANGES:

 - Non-printable chars in filenames are now output using backslash-escaped
   characters rather than '?'s. Any non-printable character is output using 3
   digits of octal (e.g. `\n` -> `\012`), and a backslash is now output as
   `\\`. Rsync also uses your locale setting, which can make it treat fewer
   high-bit characters as non-printable.

 - If rsync received an empty file-list when pulling files, it would output a
   `nothing to do` message and exit with a 0 (success) exit status, even if the
   remote rsync returned an error (it did not do this under the same conditions
   when pushing files). This was changed to make the pulling behavior the same
   as the pushing behavior: we now do the normal end-of-run outputting
   (depending on options) and exit with the appropriate exit status.

### BUG FIXES:

 - A crash bug was fixed when a daemon had its `path` set to `/`, did not have
   chroot enabled, and used some anchored excludes in the rsyncd.conf file.

 - Fixed a bug in the transfer of a single file when `-H` is specified (rsync
   would either infinite loop or perhaps crash).

 - Fixed a case where the generator might try (and fail) to tweak the
   write-permissions of a read-only directory in list-only mode (this only
   caused an annoying warning message).

 - If `--compare-dest` or `--link-dest` uses a locally-copied file as the basis
   for an updated version, log this better when `--verbose` or `-i` is in
   effect.

 - Fixed the accidental disabling of `--backup` during the `--delete-after`
   processing.

 - Restored the ability to use the `--address` option in client mode (in
   addition to its use in daemon mode).

 - Make sure that some temporary progress information from the delete
   processing does not get left on the screen when it is followed by a newline.

 - When `--existing` skips a directory with extra verbosity, refer to it as a
   `directory`, not a `file`.

 - When transferring a single file to a different-named file, any generator
   messages that are source-file related no longer refer to the file by the
   destination filename.

 - Fixed a bug where hard-linking a group of files might fail if the generator
   hasn't created a needed destination directory yet.

 - Fixed a bug where a hard-linked group of files that is newly-linked to a
   file in a `--link-dest` dir doesn't link the files from the rest of the
   cluster.

 - When deleting files with the `--one-file-system` (`-x`) option set, rsync no
   longer tries to remove files from inside a mount-point on the receiving
   side. Also, we don't complain about being unable to remove the mount-point
   dir.

 - Fixed a compatibility problem when using `--cvs-ignore` (`-C`) and sending
   files to an older rsync without using `--delete`.

 - Make sure that a `- !` or `+ !` include/exclude pattern does not trigger the
   list-clearing action that is reserved for `!`.

 - Avoid a timeout in the generator when the sender/receiver aren't handling
   the generator's checksum output quickly enough.

 - Fixed the omission of some directories in the delete processing when
   `--relative` (`-R`) was combined with a source path that had a trailing
   slash.

 - Fixed a case where rsync would erroneously delete some files and then
   re-transfer them when the options `--relative` (`-R`) and `--recursive`
   (`-r`) were both enabled (along with `--delete`) and a source path had a
   trailing slash.

 - Make sure that `--max-size` doesn't affect a device or a symlink.

 - Make sure that a system with a really small MAXPATHLEN does not cause the
   buffers in `readfd_unbuffered()` to be too small to receive normal messages.
   (This mainly affected Cygwin.)

 - If a source pathname ends with a filename of `..`, treat it as if `../` had
   been specified (so that we don't copy files to the parent dir of the
   destination).

 - If `--delete` is combined with a file-listing rsync command (i.e. no
   transfer is happening), avoid outputting a warning that we couldn't delete
   anything.

 - If `--stats` is specified with `--delete-after`, ensure that all the
   `deleting` messages are output before the statistics.

 - Improved one `if` in the deletion code that was only checking errno for
   ENOTEMPTY when it should have also been checking for EEXIST (for
   compatibility with OS variations).

### ENHANCEMENTS:

 - Added the `--only-write-batch=FILE` option that may be used (instead of
   `--write-batch=FILE`) to create a batch file without doing any actual
   updating of the destination. This allows you to divert all the file-updating
   data away from a slow data link (as long as you are pushing the data to the
   remote server when creating the batch).

 - When the generator is taking a long time to fill up its output buffer (e.g.
   if the transferred files are few, small, or missing), it now periodically
   flushes the output buffer so that the sender/receiver can get started on the
   files sooner rather than later.

 - Improved the keep-alive code to handle a long silence between the sender and
   the receiver that can occur when the sender is receiving the checksum data
   for a large file.

 - Improved the auth-errors that are logged by the daemon to include some
   information on why the authorization failed: wrong user, password mismatch,
   etc. (The client-visible message is unchanged!)

 - Improved the client's handling of an `@ERROR` from a daemon so that it does
   not complain about an unexpectedly closed socket (since we really did expect
   the socket to close).

 - If the daemon can't open the log-file specified in rsyncd.conf, fall back to
   using syslog and log an appropriate warning. This is better than what was
   typically a totally silent (and fatal) failure (since a daemon is not
   usually run with the `--no-detach` option that was necessary to see the
   error on stderr).

 - The manpages now consistently refer to an rsync daemon as a `daemon`
   instead of a `server` (to distinguish it from the server process in a
   non-daemon transfer).

 - Made a small change to the rrsync script (restricted rsync -- in the support
   dir) to make a read-only server reject all `--remove-*` options when sending
   files (to future-proof it against the possibility of other similar options
   being added at some point).

### INTERNAL:

 - Rsync now calls `setlocale(LC_CTYPE, "")`. This enables isprint() to better
   discern which filename characters need to be escaped in messages (which
   should result in fewer escaped characters in some locales).

 - Improved the naming of the log-file open/reopen/close functions.

 - Removed some protocol-compatibility code that was only needed to help
   someone running a pre-release of 2.6.4.

### BUILD CHANGES:

 - Added configure option `--disable-locale` to disable any use of setlocale()
   in the binary.

 - Fixed a bug in the `SUPPORT{,_HARD}_LINKS` #defines which prevented rsync
   from being built without symlink or hard-link support.

 - Only #define `HAVE_REMSH` if it is going to be set to 1.

 - Configure now disables the use of mkstemp() under HP-UX (since they refuse
   to fix its broken handling of large files).

 - Configure now explicitly checks for the lseek64() function so that the code
   can use `HAVE_LSEEK64` instead of inferring lseek64()'s presence based on
   the presence of the `off64_t` type.

 - Configure no longer mentions the change in the default remote-shell (from
   rsh to ssh) that occurred for the 2.6.0 release.

 - Some minor enhancements to the test scripts.

 - Added a few new `*.diff` files to the patches dir, including a patch that
   enables the optional copying of extended attributes.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.4 (30 March 2005)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 29.

### OUTPUT CHANGES:

 - When rsync deletes a directory and outputs a verbose message about it, it
   now appends a trailing slash to the name instead of (only sometimes)
   outputting a preceding "directory " string.

 - The `--stats` output will contain file-list time-statistics if both sides
   are 2.6.4, or if the local side is 2.6.4 and the files are being pushed
   (since the stats come from the sending side).  (Requires protocol 29 for a
   pull.)

 - The `%o` (operation) log-format escape now has a third value (besides `send`
   and `recv`): `del.` (with trailing dot to make it 4 chars).  This changes
   the way deletions are logged in the daemon's log file.

 - When the `--log-format` option is combined with `--verbose`, rsync now
   avoids outputting the name of the file twice in most circumstances.  As long
   as the `--log-format` item does not refer to any post-transfer items (such
   as %b or %c), the `--log-format` message is output prior to the transfer, so
   `--verbose` is now the equivalent of a `--log-format` of '%n%L' (which
   outputs the name and any link info). If the log output must occur after the
   transfer to be complete, the only time the name is also output prior to the
   transfer is when `--progress` was specified (so that the name will precede
   the progress stats, and the full `--log-format` output will come after).

 - Non-printable characters in filenames are replaced with a '?' to avoid
   corrupting the screen or generating empty lines in the output.

### BUG FIXES:

 - Restore the list-clearing behavior of `!` in a .cvsignore file (2.6.3 was
   only treating it as a special token in an rsync include/exclude file).

 - The combination of `--verbose` and `--dry-run` now mentions the full list of
   changes that would be output without `--dry-run`.

 - Avoid a mkdir warning when removing a directory in the destination that
   already exists in the `--backup-dir`.

 - An OS that has a binary mode for its files (such as Cygwin) needed
   `setmode(fd, O_BINARY)` called on the temp-file we opened with mkstemp().
   (Fix derived from Cygwin's 2.6.3 rsync package.)

 - Fixed a potential hang when verbosity is high, the client side is the
   sender, and the file-list is large.

 - Fixed a potential protocol-corrupting bug where the generator could merge a
   message from the receiver into the middle of a multiplexed packet of data if
   only part of that data had been written out to the socket when the message
   from the generator arrived.

 - We now check if the OS doesn't support using mknod() for creating FIFOs and
   sockets, and compile-in some compatibility code using mkfifo() and socket()
   when necessary.

 - Fixed an off-by-one error in the handling of `--max-delete=N`. Also, if the
   `--max-delete` limit is exceeded during a run, we now output a warning about
   this at the end of the run and exit with a new error code (25).

 - One place in the code wasn't checking if fork() failed.

 - The `ignore nonreadable` daemon parameter used to erroneously affect
   readable symlinks that pointed to a non-existent file.

 - If the OS does not have lchown() and a chown() of a symlink will affect the
   referent of a symlink (as it should), we no longer try to set the user and
   group of a symlink.

 - The generator now properly runs the hard-link loop and the dir-time
   rewriting loop after we're sure that the redo phase is complete.

 - When `--backup` was specified with `--partial-dir=DIR`, where DIR is a
   relative path, the backup code was erroneously trying to backup a file that
   was put into the partial-dir.

 - If a file gets resent in a single transfer and the `--backup` option is
   enabled along with `--inplace`, rsync no longer performs a duplicate backup
   (it used to overwrite the first backup with the failed file).

 - One call to `flush_write_file()` was not being checked for an error.

 - The `--no-relative` option was not being sent from the client to a server
   sender.

 - If an rsync daemon specified `dont compress = ...` for a file and the client
   tried to specify `--compress`, the libz code was not handling a compression
   level of 0 properly. This could cause a transfer failure if the block-size
   for a file was large enough (e.g. rsync might have exited with an error for
   large files).

 - Fixed a bug that would sometimes surface when using `--compress` and sending
   a file with a block-size larger than 64K (either manually specified, or
   computed due to the file being really large). Prior versions of rsync would
   sometimes fail to decompress the data properly, and thus the transferred
   file would fail its verification.

 - If a daemon can't open the specified log file (i.e. syslog is not being
   used), die without crashing. We also output an error about the failure on
   stderr (which will only be seen if `--no-detach` was specified) and exit
   with a new error code (6).

 - A local transfer no longer duplicates all its include/exclude options (since
   the forked process already has a copy of the exclude list, there's no need
   to send them a set of duplicates).

 - The output of the items that are being updated by the generator (dirs,
   symlinks, devices) is now intermingled in the proper order with the output
   from the items that the receiver is updating (regular files) when pulling.
   This misordering was particularly bad when `--progress` was specified.
   (Requires protocol 29.)

 - When `--timeout` is specified, lulls that occur in the transfer while the
   generator is doing work that does not generate socket traffic (looking for
   changed files, deleting files, doing directory-time touch-ups, etc.) will
   cause a new keep-alive packet to be sent that should keep the transfer going
   as long as the generator continues to make progress. (Requires protocol 29.)

 - The stat size of a device is not added to the total file size of the items
   in the transfer (the size might be undefined on some OSes).

 - Fixed a problem with refused-option messages sometimes not making it back to
   the client side when a remote `--files-from` was in effect and the daemon
   was the receiver.

 - The `--compare-dest` option was not updating a file that differed in (the
   preserved) attributes from the version in the compare-dest DIR.

 - When rsync is copying files into a write-protected directory, fixed the
   change-report output for the directory so that we don't report an identical
   directory as changed.

### ENHANCEMENTS:

 - Rsync now supports popt's option aliases, which means that you can use
   /etc/popt and/or ~/.popt to create your own option aliases.

 - Added the `--delete-during` (`--del`) option which will delete files from
   the receiving side incrementally as each directory in the transfer is being
   processed. This makes it more efficient than the default,
   before-the-transfer behavior, which is now also available as
   `--delete-before` (and is still the default `--delete-WHEN` option that will
   be chosen if `--delete` or `--delete-excluded` is specified without a
   `--delete-WHEN` choice). All the `--del*` options infer `--delete`, so an
   rsync daemon that refuses `delete` will still refuse to allow any
   file-deleting options (including the new `--remove-sent-files` option).

 - All the `--delete-WHEN` options are now more memory efficient: Previously an
   duplicate set of file-list objects was created on the receiving side for the
   entire destination hierarchy. The new algorithm only creates one directory
   of objects at a time (for files inside the transfer).

 - Added the `--copy-dest` option, which works like `--link-dest` except that
   it locally copies identical files instead of hard-linking them.

 - Added support for specifying multiple `--compare-dest`, `--copy-dest`, or
   `--link-dest` options, but only of a single type. (Promoted from the patches
   dir and enhanced.) (Requires protocol 29.)

 - Added the `--max-size` option. (Promoted from the patches dir.)

 - The daemon-mode options are now separated from the normal rsync options so
   that they can't be mixed together. This makes it impossible to start a
   daemon that has improper default option values (which could cause problems
   when a client connects, such as hanging or crashing).

 - The `--bwlimit` option may now be used in combination with `--daemon` to
   specify both a default value for the daemon side and a value that cannot be
   exceeded by a user-specified `--bwlimit` option.

 - Added the `port` parameter to the rsyncd.conf file. (Promoted from the
   patches dir.) Also added `address`. The command-line options take precedence
   over a config-file option, as expected.

 - In `_exit_cleanup()`: when we are exiting with a partially-received file, we
   now flush any data in the write-cache before closing the partial file.

 - The `--inplace` support was enhanced to work with `--compare-dest`,
   `--link-dest`, and (the new) `--copy-dest` options. (Requires protocol 29.)

 - Added the `--dirs` (`-d`) option for an easier way to copy directories
   without recursion. Any directories that are encountered are created on the
   destination. Specifying a directory with a trailing slash copies its
   immediate contents to the destination.

 - The `--files-from` option now implies `--dirs` (`-d`).

 - Added the `--list-only` option, which is mainly a way for the client to put
   the server into listing mode without needing to resort to any internal
   option kluges (e.g. the age-old use of `-r --exclude='/*/*'` for a
   non-recursive listing). This option is used automatically (behind the
   scenes) when a modern rsync speaks to a modern daemon, but may also be
   specified manually if you want to force the use of the `--list-only` option
   over a remote-shell connection.

 - Added the `--omit-dir-times` (`-O`) option, which will avoid updating the
   modified time for directories when `--times` was specified. This option will
   avoid an extra pass through the file-list at the end of the transfer (to
   tweak all the directory times), which may provide an appreciable speedup for
   a really large transfer. (Promoted from the patches dir.)

 - Added the `--filter` (`-f`) option and its helper option, `-F`. Filter rules
   are an extension to the existing include/exclude handling that also supports
   nested filter files as well as per-directory filter files (like .cvsignore,
   but with full filter-rule parsing).  This new option was chosen in order to
   ensure that all existing include/exclude processing remained 100% compatible
   with older versions. Protocol 29 is needed for full filter-rule support, but
   backward-compatible rules work with earlier protocol versions.  (Promoted
   from the patches dir and enhanced.)

 - Added the `--delay-updates` option that puts all updated files into a
   temporary directory (by default `.~tmp~`, but settable via the
   `--partial-dir=DIR` option) until the end of the transfer. This makes the
   updates a little more atomic for a large transfer.

 - If rsync is put into the background, any output from `--progress` is
   reduced.

 - Documented the `max verbosity` setting for rsyncd.conf. (This setting was
   added a couple releases ago, but left undocumented.)

 - The sender and the generator now double-check the file-list index they are
   given, and refuse to try to do a file transfer on a non-file index (since
   that would indicate that something had gone very wrong).

 - Added the `--itemize-changes` (`-i`) option, which is a way to output a more
   detailed list of what files changed and in what way. The effect is the same
   as specifying a `--log-format` of `%i %n%L` (see both the rsync and
   rsyncd.conf manpages). Works with `--dry-run` too.

 - Added the `--fuzzy` (`-y`) option, which attempts to find a basis file for a
   file that is being created from scratch. The current algorithm only looks in
   the destination directory for the created file, but it does attempt to find
   a match based on size/mod-time (in case the file was renamed with no other
   changes) as well as based on a fuzzy name-matching algorithm. This option
   requires protocol 29 because it needs the new file-sorting order. (Promoted
   from patches dir and enhanced.) (Requires protocol 29.)

 - Added the `--remove-sent-files` option, which lets you move files between
   systems.

 - The hostname in HOST:PATH or HOST::PATH may now be an IPv6 literal enclosed
   in '[' and ']' (e.g. `[::1]`). (We already allowed IPv6 literals in the
   rsync://HOST:PORT/PATH format.)

 - When rsync recurses to build the file list, it no longer keeps open one or
   more directory handles from the dir's parent dirs.

 - When building under windows, the default for `--daemon` is now to avoid
   detaching, requiring the new `--detach` option to force rsync to detach.

 - The `--dry-run` option can now be combined with either `--write-batch` or
   `--read-batch`, allowing you to run a do-nothing test command to see what
   would happen without `--dry-run`.

 - The daemon's `read only` config item now sets an internal `read_only`
   variable that makes extra sure that no write/delete calls on the read-only
   side can succeed.

 - The log-format % escapes can now have a numeric field width in between the %
   and the escape letter (e.g. `%-40n %08p`).

 - Improved the option descriptions in the `--help` text.

### SUPPORT FILES:

 - Added atomic-rsync to the support dir: a perl script that will transfer some
   files using rsync, and then move the updated files into place all at once at
   the end of the transfer. Only works when pulling, and uses `--link-dest` and
   a parallel hierarchy of files to effect its update.

 - Added mnt-excl to the support dir: a perl script that takes the /proc/mounts
   file and translates it into a set of excludes that will exclude all mount
   points (even mapped mounts to the same disk). The excludes are made relative
   to the specified source dir and properly anchored.

 - Added savetransfer.c to the support dir: a C program that can make a copy of
   all the data that flows over the wire. This lets you test for data
   corruption (by saving the data on both the sending side and the receiving
   side) and provides one way to debug a protocol error.

 - Added rrsync to the support dir: this is an updated version of Joe Smith's
   restricted rsync perl script. This helps to ensure that only certain rsync
   commands can be run by an ssh invocation.

### INTERNAL:

 - Added better checking of the checksum-header values that come over the
   socket.

 - Merged a variety of file-deleting functions into a single function so that
   it is easier to maintain.

 - Improved the type of some variables (particularly blocksize vars) for
   consistency and proper size.

 - Got rid of the uint64 type (which we didn't need).

 - Use a slightly more compatible set of core #include directives.

 - Defined int32 in a way that ensures that the build dies if we can't find a
   variable with at least 32 bits.

### PROTOCOL DIFFERENCES FOR VERSION 29:

 - A 16-bit flag-word is transmitted after every file-list index. This
   indicates what is changing between the sender and the receiver. The
   generator now transmits an index and a flag-word to indicate when dirs and
   symlinks have changed (instead of producing a message), which makes the
   outputting of the information more consistent and less prone to screen
   corruption (because the local receiver/sender is now outputting all the
   file-change info messages).

 - If a file is being hard-linked, the `ITEM_XNAME_FOLLOWS` bit is enabled in
   the flag-word and the name of the file that was linked immediately follows
   in vstring format (see below).

 - If a file is being transferred with an alternate-basis file, the
   `ITEM_BASIS_TYPE_FOLLOWS` bit is enabled in the flag-word and a single byte
   follows, indicating what type of basis file was chosen. If that indicates
   that a fuzzy-match was selected, the `ITEM_XNAME_FOLLOWS` bit is set in the
   flag-word and the name of the match in vstring format follows the basis
   byte. A vstring is a variable length string that has its size written prior
   to the string, and no terminating null.  If the string is from 1-127 bytes,
   the length is a single byte. If it is from 128-32767 bytes, the length is
   written as ((len >> 8) | 0x80) followed by (len % 0x100).

 - The sending of exclude names is done using filter-rule syntax. This means
   that all names have a prefixed rule indicator, even excludes (which used to
   be sent as a bare pattern, when possible). The `-C` option will include the
   per-dir .cvsignore merge file in the list of filter rules so it is
   positioned correctly (unlike in some older transfer scenarios).

 - Rsync sorts the filename list in a different way: it sorts the subdir names
   after the non-subdir names for each dir's contents, and it always puts a
   dir's contents immediately after the dir's name in the list. (Previously an
   item named `foo.txt` would sort in between directory `foo/` and `foo/bar`.)

 - When talking to a protocol 29 rsync daemon, a list-only request is able to
   note this before the options are sent over the wire and the new
   `--list-only` option is included in the options.

 - When the `--stats` bytes are sent over the wire (or stored in a batch), they
   now include two elapsed-time values: one for how long it took to build the
   file-list, and one for how long it took to send it over the wire (each
   expressed in thousandths of a second).

 - When `--delete-excluded` is specified with some filter rules (AKA excludes),
   a client sender will now initiate a send of the rules to the receiver (older
   protocols used to omit the sending of excludes in this situation since there
   were no receiver-specific rules that survived `--delete-excluded` back
   then). Note that, as with all the filter-list sending, only items that are
   significant to the other side will actually be sent over the wire, so the
   filter-rule list that is sent in this scenario is often empty.

 - An index equal to the file-list count is sent as a keep-alive packet from
   the generator to the sender, which then forwards it on to the receiver. This
   normally invalid index is only a valid keep-alive packet if the 16-bit
   flag-word that follows it contains a single bit (`ITEM_IS_NEW`, which is
   normally an illegal flag to appear alone).

 - A protocol-29 batch file includes a bit for the setting of the `--dirs`
   option and for the setting of the `--compress` option. Also, the shell
   script created by `--write-batch` will use the `--filter` option instead of
   `--exclude-from` to capture any filter rules.

### BUILD CHANGES:

 - Handle an operating system that use mkdev() in place of makedev().

 - Improved configure to better handle cross-compiling.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.3 (30 Sep 2004)

## Changes in this version:

### SECURITY FIXES:

 - A bug in the `sanitize_path` routine (which affects a non-chrooted rsync
   daemon) could allow a user to craft a pathname that would get transformed
   into an absolute path for certain options (but not for file-transfer names).
   If you're running an rsync daemon with chroot disabled, **please upgrade**,
   ESPECIALLY if the user privs you run rsync under is anything above `nobody`.

   OUTPUT CHANGES (ATTN: those using a script to parse the verbose output):

 - Please note that the 2-line footer (output when verbose) now uses the term
   `sent` instead of `wrote` and `received` instead of `read`. If you are not
   parsing the numeric values out of this footer, a script would be better off
   using the empty line prior to the footer as the indicator that the verbose
   output is over.

 - The output from the `--stats` option was similarly affected to change
   `written` to `sent` and `read` to `received`.

 - Rsync ensures that a filename that contains a newline gets mentioned with
   each newline transformed into a question mark (which prevents a filename
   from causing an empty line to be output).

 - The `backed up ...` message that is output when at least 2 `--verbose`
   options are specified is now the same both with and without the
   `--backup-dir` option.

### BUG FIXES:

 - Fixed a crash bug that might appear when `--delete` was used and multiple
   source directories were specified.

 - Fixed a 32-bit truncation of the file length when generating the checksums.

 - The `--backup` code no longer attempts to create some directories over and
   over again (generating warnings along the way).

 - Fixed a bug in the reading of the secrets file (by the daemon) and the
   password file (by the client): the files no longer need to be terminated by
   a newline for their content to be read in.

 - If a file has a read error on the sending side or the reconstructed data
   doesn't match the expected checksum (perhaps due to the basis file changing
   during the transfer), the receiver will no longer retain the resulting file
   unless the `--partial` option was specified.  (Note: for the read-error
   detection to work, neither side can be older than 2.6.3 -- older receivers
   will always retain the file, and older senders don't tell the receiver that
   the file had a read error.)

 - If a file gets resent in a single transfer and the `--backup` option is
   enabled, rsync no longer performs a duplicate backup (it used to overwrite
   the original file in the backup area).

 - Files specified in the daemon's `exclude` or `exclude from` config items are
   now excluded from being uploaded (assuming that the module allows uploading
   at all) in addition to the old download exclusion.

 - Got rid of a potential hang in the receiver when near the end of a phase.

 - When using `--backup` without a `--backup-dir`, rsync no longer preserves
   the modify time on directories. This avoids confusing NFS.

 - When `--copy-links` (`-L`) is specified, we now output a separate error for
   a symlink that has no referent instead of claiming that a file `vanished`.

 - The `--copy-links` (`-L`) option no longer has the side-effect of telling
   the receiving side to follow symlinks. See the `--keep-dirlinks` option
   (mentioned below) for a way to specify that behavior.

 - Error messages from the daemon server's option-parsing (such as refused
   options) are now successfully transferred back to the client (the server
   used to fail to send the message because the socket wasn't in the right
   state for the message to get through).

 - Most transfer errors that occur during a daemon transfer are now returned to
   the user in addition to being logged (some messages are intended to be
   daemon-only and are not affected by this).

 - Fixed a bug in the daemon authentication code when using one of the
   batch-processing options.

 - We try to work around some buggy IPv6 implementations that fail to implement
   `IPV6_V6ONLY`. This should fix the `address in use` error that some daemons
   get when running on an OS with a buggy IPv6 implementation. Also, if the new
   code gets this error, we might suggest that the user specify `--ipv4` or
   `--ipv6` (if we think it will help).

 - When the remote rsync dies, make a better effort to recover any error
   messages it may have sent before dying (the local rsync used to just die
   with a socket-write error).

 - When using `--delete` and a `--backup-dir` that contains files that are
   hard-linked to their destination equivalents, rsync now makes sure that
   removed files really get removed (avoids a really weird rename() behavior).

 - Avoid a bogus run-time complaint about a lack of 64-bit integers when the
   int64 type is defined as an `off_t` and it actually has 64-bits.

 - Added a configure check for open64() without mkstemp64() so that we can
   avoid using mkstemp() when such a combination is encountered.  This bypasses
   a problem writing out large temp files on OSes such as AIX and HP-UX.

 - Fixed an age-old crash problem with `--read-batch` on a local copy (rsync
   was improperly assuming `--whole-file` for the local copy).

 - When `--dry-run` (`-n`) is used and the destination directory does not
   exist, rsync now produces a correct report of files that would be sent
   instead of dying with a chdir() error.

 - Fixed a bug that could cause a slow-to-connect rsync daemon to die with an
   error instead of waiting for the connection to finish.

 - Fixed an ssh interaction that could cause output to be lost when the user
   chose to combine the output of rsync's stdout and stderr (e.g.  using the
   `2>&1`).

 - Fixed an option-parsing bug when `--files-from` got passed to a daemon.

### ENHANCEMENTS:

 - Added the `--partial-dir=DIR` option that lets you specify where to
   (temporarily) put a partially transferred file (instead of overwriting the
   destination file). E.g. `--partial-dir=.rsync-partial` Also added support
   for the `RSYNC_PARTIAL_DIR` environment variable that, when found,
   transforms a regular `--partial` option (such as the convenient `-P` option)
   into one that also specifies a directory.

 - Added `--keep-dirlinks` (`-K`), which allows you to symlink a directory onto
   another partition on the receiving side and have rsync treat it as matching
   a normal directory from the sender.

 - Added the `--inplace` option that tells rsync to write each destination file
   without using a temporary file. The matching of existing data in the
   destination file can be severely limited by this, but there are also cases
   where this is more efficient (such as appending data).  Use only when needed
   (see the manpage for more details).

 - Added the `write only` option for the daemon's config file.

 - Added long-option names for `-4` and `-6` (namely `--ipv4` and `--ipv6`) and
   documented all these options in the manpage.

 - Improved the handling of the `--bwlimit` option so that it's less bursty,
   more accurate, and works properly over a larger range of values.

 - The rsync daemon-over-ssh code now looks for `SSH_CONNECTION` and
   `SSH2_CLIENT` in addition to `SSH_CLIENT` to figure out the IP address.

 - Added the `--checksum-seed=N` option for advanced users.

 - Batch writing/reading has a brand-new implementation that is simpler, fixes
   a few weird problems with the old code (such as no longer sprinkling the
   batch files into different dirs or even onto different systems), and is much
   less intrusive into the code (making it easier to maintain for the future).
   The new code generates just one data file instead of three, which makes it
   possible to read the batch on stdin via a remote shell. Also, the old
   requirement of forcing the same fixed checksum-seed for all batch processing
   has been removed.

 - If an rsync daemon has a module set with `list = no` (which hides its
   presence in the list of available modules), a user that fails to
   authenticate gets the same `unknown module` error that they would get if the
   module were actually unknown (while still logging the real error to the
   daemon's log file). This prevents fishing for module names.

 - The daemon's `refuse options` config item now allows you to match option
   names using wildcards and/or the single-letter option names.

 - Each transferred file now gets its permissions and modified-time updated
   before the temp-file gets moved into place. Previously, the finished file
   would have a very brief window where its permissions disallowed all group
   and world access.

 - Added the ability to parse a literal IPv6 address in an `rsync:` URL (e.g.
   rsync://[2001:638:500:101::21]:873/module/dir).

 - The daemon's wildcard expanding code can now handle more than 1000 filenames
   (it's now limited by memory instead of having a hard-wired limit).

### INTERNAL:

 - Some cleanup in the exclude code has saved some per-exclude memory and made
   the code easier to maintain.

 - Improved the argv-overflow checking for a remote command that has a lot of
   args.

 - Use rsyserr() in the various places that were still calling rprintf() with
   strerror() as an arg.

 - If an rsync daemon is listening on multiple sockets (to handle both IPv4 and
   IPv6 to a single port), we now close all the unneeded file handles after we
   accept a connection (we used to close just one of them).

 - Optimized the handling of larger block sizes (rsync used to slow to a crawl
   if the block size got too large).

 - Optimized away a loop in `hash_search()`.

 - Some improvements to the `sanitize_path()` and `clean_fname()` functions
   makes them more efficient and produce better results (while still being
   compatible with the file-name cleaning that gets done on both sides when
   sending the file-list).

 - Got rid of `alloc_sanitize_path()` after adding a destination-buffer arg to
   `sanitize_path()` made it possible to put all the former's functionality
   into the latter.

 - The file-list that is output when at least 4 verbose options are specified
   reports the uid value on the sender even when rsync is not running as root
   (since we might be sending to a root receiver).

### BUILD CHANGES:

 - Added a `gen` target to rebuild most of the generated files, including
   configure, config.h.in, the manpages, and proto.h.

 - If `make proto` doesn't find some changes in the prototypes, the proto.h
   file is left untouched (its time-stamp used to always be updated).

 - The variable `$STRIP` (that is optionally set by the install-strip target's
   rule) was changed to `$INSTALL_STRIP` because some systems have `$STRIP`
   already set in the environment.

 - Fixed a build problem when `SUPPORT_HARD_LINKS` isn't defined.

 - When cross-compiling, the gettimeofday() function is now assumed to be a
   modern version that takes two-args (since we can't test it).

### DEVELOPER RELATED:

 - The scripts in the testsuite dir were cleaned up a bit and a few new tests
   added.

 - Some new diffs were added to the patches dir, and some accepted ones were
   removed.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.2 (30 Apr 2004)

## Changes in this version:

### BUG FIXES:

 - Fixed a major bug in the sorting of the filenames when `--relative` is used
   for some sources (just sources such as `/` and `/*` were affected). This fix
   ensures that we ask for the right file-list item when requesting changes
   from the sender.

 - Rsync now checks the return value of the close() function to better report
   disk-full problems on an NFS file system.

 - Restored the old daemon-server behavior of logging error messages rather
   than returning them to the user. (A better long-term fix will be sought in
   the future.)

 - An obscure uninitialized-variable bug was fixed in the uid/gid code. (This
   bug probably had no ill effects.)

### BUILD CHANGES:

 - Got rid of the configure check for sys/sysctl.h (it wasn't used and was
   causing a problem on some systems). Also improved the
   broken-largefile-locking test to try to avoid failure due to an NFS
   build-dir.

 - Fixed a compile problem on systems that don't define `AI_NUMERICHOST`.

 - Fixed a compile problem in the popt source for compilers that don't support
   `__attribute__`.

### DEVELOPER RELATED:

 - Improved the testsuite's `merge` test to work on OSF1.

 - Two new diffs were added to the patches dir.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.1 (26 Apr 2004)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 28.

### SECURITY FIXES:

 - Paths sent to an rsync daemon are more thoroughly sanitized when chroot is
   not used. If you're running a non-read-only rsync daemon with chroot
   disabled, **please upgrade**, ESPECIALLY if the user privs you run rsync
   under is anything above `nobody`.

### ENHANCEMENTS:

 - Lower memory use, more optimal transfer of data over the socket, and lower
   CPU usage (see the INTERNAL section for details).

 - The `RSYNC_PROXY` environment variable can now contain a `USER:PASS@` prefix
   before the `HOST:PORT` information.  (Bardur Arantsson)

 - The `--progress` output now mentions how far along in the transfer we are,
   including both a count of files transferred and a percentage of the total
   file-count that we've processed. It also shows better
   current-rate-of-transfer and remaining-transfer-time values.

 - Documentation changes now attempt to describe some often misunderstood
   features more clearly.

### BUG FIXES:

 - When `-x` (`--one-file-system`) is combined with `-L` (`--copy-links`) or
   `--copy-unsafe-links,` no symlinked files are skipped, even if the referent
   file is on a different filesystem.

 - The `--link-dest` code now works properly for a non-root user when (1) the
   UIDs of the source and destination differ and `-o` was specified, or (2)
   when the group of the source can't be used on the destination and `-g` was
   specified.

 - Fixed a bug in the handling of `-H` (hard-links) that might cause the
   expanded PATH/NAME value of the current item to get overwritten (due to an
   expanded-name caching bug).

 - We now reset the `new data has been sent` flag at the start of each file we
   send. This makes sure that an interrupted transfer with the `--partial`
   option set doesn't keep a shorter temp file than the current basis file when
   no new data has been transferred over the wire for that file.

 - Fixed a byte-order problem in `--batch-mode` on big-endian machines.  (Jay
   Fenlason)

 - When using `--cvs-exclude`, the exclude items we get from a per-directory's
   .cvsignore file once again only affect that one directory (not all following
   directories too). The items are also now properly word-split and parsed
   without any +/- prefix parsing.

 - When specifying the USER@HOST: prefix for a file, the USER part can now
   contain an '@', if needed (i.e. the last '@' is used to find the HOST, not
   the first).

 - Fixed some bugs in the handling of group IDs for non-root users: (1) It
   properly handles a group that the sender didn't have a name for (it would
   previously skip changing the group on any files in that group). (2) If
   `--numeric-ids` is used, rsync no longer attempts to set groups that the
   user doesn't have the permission to set.

 - Fixed the `refuse options` setting in the rsyncd.conf file.

 - Improved the `-x` (`--one-file-system`) flag's handling of any mount-point
   directories we encounter. It is both more optimal (in that it no longer does
   a useless scan of the contents of the mount-point dirs) and also fixes a
   bug where a remapped mount of the original filesystem could get discovered
   in a subdir we should be ignoring.

 - Rsync no longer discards a double-slash at the start of a filename when
   trying to open the file. It also no longer constructs names that start with
   a double slash (unless the user supplied them).

 - Path-specifying options to a daemon should now work the same with or without
   chroot turned on. Previously, such a option (such as `--link-dest`) would
   get its absolute path munged into a relative one if chroot was not on,
   making that setting fairly useless.  Rsync now transforms the path into one
   that is based on the module's base dir when chroot is not enabled.

 - Fixed a compatibility problem interacting with older rsync versions that
   might send us an empty `--suffix` value without telling us that
   `--backup-dir` was specified.

 - The `hosts allow` option for a daemon-over-remote-shell process now has
   improved support for IPv6 addresses and a fix for systems that have a length
   field in their socket structs.

 - Fixed the ability to request an empty backup `--suffix` when sending files
   to an rsync daemon.

 - Fixed an option-parsing bug when `--files-from` was sent to a server sender.

### INTERNAL:

 - Most of the I/O is now buffered, which results in a pretty large speedup
   when running under MS Windows. (Craig Barratt)

 - Optimizations to the name-handling/comparing code have made some significant
   reductions in user-CPU time for large file sets.

 - Some cleanup of the variable types make the code more consistent.

 - Reduced memory requirements of hard link preservation.  (J.W. Schultz)

 - Implemented a new algorithm for hard-link handling that speeds up the code
   significantly. (J.W. Schultz and Wayne Davison)

 - The `--hard-link` option now uses the first existing file in the group of
   linked files as the basis for the transfer. This prevents the sub-optimal
   transfer of a file's data when a new hardlink is added on the sending side
   and it sorts alphabetically earlier in the list than the files that are
   already present on the receiving side.

 - Dropped support for protocol versions less than 20 (2.3.0 released 15 Mar
   1999) and activated warnings for protocols less than 25 (2.5.0 released 23
   Aug 2001). (Wayne Davison and J.W. Schultz, severally)

 - More optimal data transmission for `--hard-links` (protocol 28).

 - More optimal data transmission for `--checksum` (protocol 28).

 - Less memory is used when `--checksum` is specified.

 - Less memory is used in the file list (a per-file savings).

 - The generator is now better about not modifying the file list during the
   transfer in order to avoid a copy-on-write memory bifurcation (on systems
   where fork() uses shared memory).  Previously, rsync's shared memory would
   slowly become unshared, resulting in real memory usage nearly doubling on
   the receiving side by the end of the transfer. Now, as long as permissions
   are being preserved, the shared memory should remain that way for the entire
   transfer.

 - Changed hardlink info and `file_struct` + strings to use allocation pools.
   This reduces memory use for large file-sets and permits freeing memory to
   the OS. (J.W. Schultz)

 - The 2 pipes used between the receiver and generator processes (which are
   forked on the same machine) were reduced to 1 pipe and the protocol improved
   so that (1) it is now impossible to have the `redo` pipe fill up and hang
   rsync, and (2) trailing messages from the receiver don't get lost on their
   way through the generator over to the sender (which mainly affected
   hard-link messages and verbose `--stats` output).

 - Improved the internal uid/gid code to be more portable and a little more
   optimized.

 - The device numbers sent when using `--devices` are now sent as separate
   major/minor values with 32-bit accuracy (protocol 28).  Previously, the
   copied devices were sent as a single 32-bit number. This will make
   inter-operation of 64-bit binaries more compatible with their 32-bit
   brethren (with both ends of the connection are using protocol 28). Note that
   optimizations in the binary protocol for sending the device numbers often
   results in fewer bytes being used than before, even though more precision is
   now available.

 - Some cleanup of the exclude/include structures and its code made things
   clearer (internally), simpler, and more efficient.

 - The reading & writing of the file-list in batch-mode is now handled by the
   same code that sends & receives the list over the wire. This makes it much
   easier to maintain. (Note that the batch code is still considered to be
   experimental.)

### BUILD CHANGES:

 - The configure script now accepts `--with-rsyncd-conf=PATH` to override the
   default value of the /etc/rsyncd.conf file.

 - Fixed configure bug when running `./configure --disable-ipv6`.

 - Fixed compilation problem on Tru64 Unix (having to do with `sockaddr.sa_len`
   and `sockaddr.sin_len`).

### DEVELOPER RELATED:

 - Fixed `make test` bug when build dir is not the source dir.

 - Added a couple extra diffs in the `patches` dir, removed the ones that got
   applied, and rebuilt the rest.

------------------------------------------------------------------------------

# NEWS for rsync 2.6.0 (1 Jan 2004)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 27.  The maximum accepted protocol number
   was increased from 30 to 40.

### ENHANCEMENTS:

 - `ssh` is now the default remote shell for rsync. If you want to change this,
   configure like this: `./configure --with-rsh=rsh`.

 - Added `--files-from`, `--no-relative`, `--no-implied-dirs`, and `--from0`.
   Note that `--from0` affects the line-ending character for all the files read
   by the `--*-from` options. (Wayne Davison)

 - Length of csum2 is now per-file starting with protocol version
   27. (J.W. Schultz)

 - Per-file dynamic block size is now sqrt(file length). The per-file checksum
   size is determined according to an algorithm provided by Donovan Baarda
   which reduces the probability of rsync algorithm corrupting data and falling
   back using the whole md4 checksums. (J.W. Schultz, Donovan Baarda)

 - The `--stats` option no longer includes the (debug) malloc summary unless
   the verbose option was specified at least twice.

 - Added a new error/warning code for when files vanish from the sending side.
   Made vanished source files not interfere with the file-deletion pass when
   `--delete-after` was specified.

 - Various trailing-info sections are now preceded by a newline.

### BUG FIXES:

 - Fixed several exclude/include matching bugs when using wild-cards.  This has
   a several user-visible effects, all of which make the matching more
   consistent and intuitive. This should hopefully not cause anyone problems
   since it makes the matching work more like what people are expecting. (Wayne
   Davison)

 - A pattern with a `**` no longer causes a `*` to match slashes.  For example,
   with `/*/foo/**`, `foo` must be 2 levels deep.  [If your string has BOTH `*`
   and `**` wildcards, changing the `*` wildcards to `**` will provide the old
   behavior in all versions.]

 - `**/foo` now matches at the base of the transfer (like /foo does). [Use
   `/**/foo` to get the old behavior in all versions.]

 - A non-anchored wildcard term floats to match beyond the base of the
   transfer. E.g. `CVS/R*` matches at the end of the path, just like the
   non-wildcard term `CVS/Root` does. [Use `/CVS/R*` to get the old behavior in
   all versions.]

 - Including a `**` in the match term causes it to be matched against the
   entire path, not just the name portion, even if there aren't any interior
   slashes in the term. E.g. `foo**bar` would exclude `/path/foo-bar` (just
   like before) as well as `/foo-path/baz-bar` (unlike before). [Use `foo*bar`
   to get the old behavior in all versions.]

 - The exclude list specified in the daemon's config file is now properly
   applied to the pulled items no matter how deep the user's file-args are in
   the source tree. (Wayne Davison)

 - For protocol version >= 27, `mdfour_tail()` is called when the block size
   (including `checksum_seed`) is a multiple of 64.  Previously it was not
   called, giving the wrong MD4 checksum.  (Craig Barratt)

 - For protocol version >= 27, a 64 bit bit counter is used in mdfour.c as
   required by the RFC. Previously only a 32 bit bit counter was used, causing
   incorrect MD4 file checksums for file sizes >= 512MB - 4. (Craig Barratt)

 - Fixed a crash bug when interacting with older rsync versions and multiple
   files of the same name are destined for the same dir.  (Wayne Davison)

 - Keep tmp names from overflowing MAXPATHLEN.

 - Make `--link-dest` honor the absence of `-p`, `-o`, and `-g`.

 - Made rsync treat a trailing slash in the destination in a more consistent
   manner.

 - Fixed file I/O error detection. (John Van Essen)

 - Fixed bogus `malformed address {hostname}` message in rsyncd log when
   checking IP address against hostnames from `hosts allow` and `hosts deny`
   parameters in config file.

 - Print heap statistics when verbose >= 2 instead of when >= 1.

 - Fixed a compression (`-z`) bug when syncing a mostly-matching file that
   contains already-compressed data. (Yasuoka Masahiko and Wayne Davison)

 - Fixed a bug in the `--backup` code that could cause deleted files to not get
   backed up.

 - When the backup code makes new directories, create them with mode 0700
   instead of 0755 (since the directory permissions in the backup tree are not
   yet copied from the main tree).

 - Call setgroups() in a more portable manner.

 - Improved file-related error messages to better indicate exactly what
   pathname failed. (Wayne Davison)

 - Fixed some bugs in the handling of `--delete` and `--exclude` when using the
   `--relative` (`-R`) option. (Wayne Davison)

 - Fixed bug that prevented regular files from replacing special files and
   caused a directory in `--link-dest` or `--compare-dest` to block the
   creation of a file with the same path. A directory still cannot be replaced
   by a regular file unless `--delete` specified. (J.W. Schultz)

 - Detect and report when open or opendir succeed but read and readdir fail
   caused by network filesystem issues and truncated files. (David Norwood,
   Michael Brown, J.W. Schultz)

 - Added a fix that should give ssh time to restore the tty settings if the
   user presses Ctrl-C at an ssh password prompt.

### INTERNAL:

 - Eliminated vestigial support for old versions that we stopped supporting.
   (J.W. Schultz)

 - Simplified some of the option-parsing code. (Wayne Davison)

 - Some cleanup made to the exclude code, as well as some new defines added to
   enhance readability. (Wayne Davison)

 - Changed the protocol-version code so that it can interact at a lower
   protocol level than the maximum supported by both sides.  Added an
   undocumented option, `--protocol=N`, to force the value we advertise to the
   other side (primarily for testing purposes).  (Wayne Davison)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.7 (4 Dec 2003)

## Changes in this version:

### SECURITY FIXES:

 - Fix buffer handling bugs. (Andrew Tridgell, Martin Pool, Paul Russell,
   Andrea Barisani)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.6, aka "the dwd-between-jobs release" (26 Jan 2003)

## Changes in this version:

### ENHANCEMENTS:

 - The `--delete-after` option now implies `--delete`. (Wayne Davison)

 - The `--suffix` option can now be used with `--backup-dir`. (Michael
   Zimmerman)

 - Combining `::` syntax with the `--rsh`/`-e` option now uses the specified
   remote-shell as a transport to talk to a (newly-spawned) server-daemon. This
   allows someone to use daemon features, such as modules, over a secure
   protocol, such as ssh. (JD Paul)

 - The rsync:// syntax for daemon connections is now accepted in the
   destination field.

 - If the file name given to `--include-from` or `--exclude-from` is `-`, rsync
   will read from standard input. (J.W. Schultz)

 - New option `--link-dest` which is like `--compare-dest` except that
   unchanged files are hard-linked in to the destination directory.  (J.W.
   Schultz)

 - Don't report an error if an excluded file disappears during an rsync run.
   (Eugene Chupriyanov and Bo Kersey)

 - Added .svn to `--cvs-exclude` list to support subversion. (Jon Middleton)

 - Properly support IPv6 addresses in the rsyncd.conf `hosts allow` and `hosts
   deny` fields. (Hideaki Yoshifuji)

 - Changed exclude file handling to permit DOS or MAC style line terminations.
   (J.W. Schultz)

 - Ignore errors from chmod when `-p`/`-a`/`--preserve-perms` is not set.
   (Dave Dykstra)

### BUG FIXES:

 - Fix `forward name lookup failed` errors on AIX 4.3.3. (John L. Allen, Martin
   Pool)

 - Generate each file's rolling-checksum data as we send it, not in a separate
   (memory-eating) pass before hand. This prevents timeout errors on really
   large files. (Stefan Nehlsen)

 - Fix compilation on Tru64. (Albert Chin, Zoong Pham)

 - Better handling of some client-server errors. (Martin Pool)

 - Fixed a crash that would occur when sending a list of files that contains a
   duplicate name (if it sorts to the end of the file list) and using
   `--delete`. (Wayne Davison)

 - Fixed the file-name duplicate-removal code when dealing with multiple dups
   in a row. (Wayne Davison)

 - Fixed a bug that caused rsync to lose the exit status of its child processes
   and sometimes return an exit code of 0 instead of showing an error. (David
   R. Staples, Dave Dykstra)

 - Fixed bug in `--copy-unsafe-links` that caused it to be completely broken.
   (Dave Dykstra)

 - Prevent infinite recursion in cleanup code under certain circumstances.
   (Sviatoslav Sviridov and Marc Espie)

 - Fixed a bug that prevented rsync from creating intervening directories when
   `--relative-paths`/`-R` is set. (Craig Barratt)

 - Prevent `Connection reset by peer` messages from Cygwin. (Randy O'Meara)

### INTERNAL:

 - Many code cleanups and improved internal documentation. (Martin Pool, Nelson
   Beebe)

 - Portability fixes. (Dave Dykstra and Wayne Davison)

 - More test cases. (Martin Pool)

 - Some test-case fixes. (Brian Poole, Wayne Davison)

 - Updated included popt to the latest vendor drop, version 1.6.4.  (Jos
   Backus)

 - Updated config.guess and config.sub to latest versions; this means rsync
   should build on more platforms. (Paul Green)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.5, aka Snowy River (2 Apr 2002)

## Changes in this version:

### ENHANCEMENTS:

 - With `--progress`, when a transfer is complete show the time taken;
   otherwise show expected time to complete. (Cameron Simpson)

 - Make `make install-strip` works properly, and `make install` accepts a
   DESTDIR variable for help in building binary packages.  (Peter
   Breitenlohner, Greg Louis)

 - If configured with `--enable-maintainer-mode`, then on receipt of a fatal
   signal rsync will try to open an xterm running gdb, similarly to Samba's
   `panic action` or GNOME's bug-buddy.  (Martin Pool)

### BUG FIXES:

 - Fix situation where failure to fork (e.g. because out of process slots)
   would cause rsync to kill all processes owned by the current user. Yes,
   really! (Paul Haas, Martin Pool)

 - Fix test suite on Solaris. (Jos Backus, Martin Pool)

 - Fix minor memory leak in socket code. (Dave Dykstra, Martin Pool.)

 - Fix `--whole-file` problem that caused it to be the default even for remote
   connections. (Martin Pool, Frank Schulz)

 - Work around bug in Mac OS X mkdir(2), which cannot handle trailing slashes.
   <http://www.opensource.apple.com/bugs/X/BSD%20Kernel/2734739.html> (Martin
   Pool)

 - Improved network error handling. (Greg A. Woods)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.4, aka "Imitation lizard skin" (13 Mar 2002)

## Changes in this version:

### BUG FIXES:

 - Additional fix for zlib double-free bug. (Martin Pool, Andrew Tridgell) (CVE
   CAN-2002-0059)

### ENHANCEMENTS:

 - Merge in changes from zlib 1.1.3 to zlib 1.1.4. (Jos Backus) (Note that
   rsync still uses a custom version of zlib; you can not just link against a
   system library. See zlib/README.rsync)

 - Additional test cases for `--compress`. (Martin Pool)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.3, aka "Happy 26" (11 Mar 2002)

## Changes in this version:

### SECURITY FIXES:

 - Make sure that supplementary groups are removed from a server
   process after changing uid and gid. (Ethan Benson) (Debian bug
   #132272, CVE CAN-2002-0080)

### BUG FIXES:

 - Fix zlib double-free bug. (Owen Taylor, Mark J Cox) (CVE CAN-2002-0059)

 - Fixed problem that in many cases caused the error message unexpected read
   size of 0 in `map_ptr` and resulted in the wrong data being copied.

 - Fixed compilation errors on some systems caused by the use of `unsigned
   int64` in rsync.h.

 - Fixed problem on systems such as Sunos4 that do not support realloc on a
   NULL pointer; error was 'out of memory in "flist_expand"'.

 - Fix for rsync server processes hanging around after the client unexpectedly
   disconnects. (Colin Walters) (Debian bug #128632)

 - Cope with BSD systems on which mkdir() will not accept a trailing slash.

### ENHANCEMENTS:

 - Merge in changes from zlib 1.1.2 to zlib 1.1.3. (Note that rsync still uses
   a custom version of zlib; you can not just link against a system library.
   See zlib/README.rsync)

 - Command to initiate connections is only shown with `-vv`, rather than `-v`
   as in 2.5.2. Output from plain `-v` is more similar to what was historically
   used so as not to break scripts that try to parse the output.

 - Added `--no-whole-file` and `--no-blocking-io` options (Dave Dykstra)

 - Made the `--write-batch` and `--read-batch` options actually work and added
   documentation in the manpage (Jos Backus)

 - If the daemon is unable to fork a child to accept a connection, print an
   error message. (Colin Walters)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.2 (26 Jan 2002)

## Changes in this version:

### SECURITY FIXES:

 - Signedness security patch from Sebastian Krahmer <krahmer@suse.de> -- in
   some cases we were not sufficiently careful about reading integers from the
   network.

### PROTOCOL NUMBER:

 - The protocol number was changed to 26.

### BUG FIXES:

 - Fix possible string mangling in log files.

 - Fix for setting local address of outgoing sockets.

 - Better handling of hardlinks and devices on platforms with 64-bit `dev_t` or
   `ino_t`.

 - Name resolution on machines supporting IPv6 is improved.

 - Fix for device nodes. (dann frazier) (Debian #129135)

### ENHANCEMENTS:

 - With `-v`, rsync now shows the command used to initiate an ssh/rsh
   connection.

 - `--statistics` now shows memory heap usage on platforms that support
   mallinfo().

 - "The Ted T'so school of program optimization": make progress visible and
   people will think it's faster. (With `--progress`, rsync will show you how
   many files it has seen as it builds the `file_list`, giving some indication
   that it has not hung.)

 - Improvements to batch mode support. This is still experimental but testing
   would be welcome. (Jos Backus)

 - New `--ignore-existing` option, patch previously distributed with Vipul's
   Razor. (Debian #124286)

------------------------------------------------------------------------------

# NEWS for rsync 2.5.1 (3 Jan 2002)

## Changes in this version:

### BUG FIXES:

 - Fix for segfault in `--daemon` mode configuration parser. (Paul Mackerras)

 - Correct `string<->address` parsing for both IPv4 and 6.  (YOSHIFUJI Hideaki,
   SUMIKAWA Munechika and Jun-ichiro `itojun` Hagino)

 - Various fixes for IPv6 support. (Dave Dykstra)

 - rsync.1 typo fix. (Matt Kraai)

 - Test suite typo fixes. (Tom Schmidt)

 - rsync.1 grammar and clarity improvements. (Edward Welbourne)

 - Correction to ./configure tests for `inet_ntop`. (Jeff Garzik)

### ENHANCEMENTS:

 - `--progress` and `-P` now show estimated data transfer rate (in a multiple
   of bytes/s) and estimated time to completion. (Rik Faith)

 - `--no-detach` option, required to run as a W32 service and also useful when
   running on Unix under daemontools, AIX's SRC, or a debugger. (Max Bowsher,
   Jos Backus)

 - Clearer error messages for some conditions.

------------------------------------------------------------------------------

# NEWS for rsync 2.5.0 (30 Nov 2001)

## Changes in this version:

### PROTOCOL NUMBER:

 - The protocol number was changed to 25.

### ANNOUNCEMENTS:

 - Martin Pool <mbp@samba.org> is now a co-maintainer.

### NEW FEATURES:

 - Support for LSB-compliant packaging <http://www.linuxbase.org/>

 - Shell wildcards are allowed in `auth users` lines.

 - Merged UNC rsync+ patch to support creation of standalone patch sets. By
   Bert J. Dempsey and Debra Weiss, updated by Jos Backus.
   <http://www.ils.unc.edu/i2dsi/unc_rsync+.html>

 - IPv6 support based on a patch from KAME.net, on systems including modern
   versions of Linux, Solaris, and HP-UX. Also includes IPv6 compatibility
   functions for old OSs by the Internet Software Consortium, Paul Vixie, the
   OpenSSH portability project, and OpenBSD.

### ENHANCEMENTS:

 - Include/exclude cluestick: with `-vv`, print out whether files are included
   or excluded and why.

 - Many error messages have more friendly explanations and more details.

 - Manual page improvements plus scanty protocol documentation.

 - When running as `--daemon` in the background and using a `log file`
   rsyncd.conf directive, close the log file every time it is open when going
   to sleep on the socket. This allows the log file to get cleaned out by
   another process.

 - Change to using libpopt rather than getopt for processing options. This
   makes the code cleaner and the behaviour more consistent across platforms.
   popt is included and built if not installed on the platform.

 - More details in `--version`, including note about whether 64-bit files,
   symlinks and hardlinks are supported.

 - MD4 code may use less CPU cycles.

 - Use mkstemp on systems where it is secure. If we use mktemp, explain that we
   do it in a secure way.

 - `--whole-file` is the default when source and target are on the local
   machine.

### BUG FIXES:

 - Fix for various bugs causing rsync to hang.

 - Attempt to fix Large File Summit support on AIX.

 - Attempt to fix error handling lockup bug.

 - Give a non-0 exit code if **any** of the files we have been asked to
   transfer fail to transfer.

 - For log messages containing ridiculously long strings that might overflow a
   buffer rsync no longer aborts, but rather prints an ellipsis at the end of
   the string. (Patch from Ed Santiago.)

### PLATFORMS:

 - Improved support for UNICOS (tested on Cray T3E and Cray SV1)

 - autoconf2.52 (or later) is now required to rebuild the autoconf scripts. It
   is not required to simply build rsync.

 - Platforms thought to work in this release:

   - Cray SV1 UNICOS 10.0.0.8 cc
   - Debian Linux 2.2 UltraSparc gcc
   - Debian Linux testing/unstable ARM gcc
   - FreeBSD 3.3-RELEASE i386 cc
   - FreeBSD 4.1.1-RELEASE i386 cc
   - FreeBSD 4.3-STABLE i386 cc
   - HP PA-RISC HP-UX 10.20 gcc
   - HP PA-RISC HP-UX 11.11 cc
   - IRIX 6.5 MIPS cc
   - IRIX 6.5 MIPS gcc
   - Mac OS X PPC (`--disable-ipv6`) cc
   - NetBSD 1.5 i386 gcc
   - NetBSD Current i386 cc
   - OpenBSD 2.5 Sparc gcc
   - OpenBSD 2.9 i386 cc
   - OpenBSD Current i386 cc
   - RedHat 6.2 i386 gcc
   - RedHat 6.2 i386 insure++
   - RedHat 7.0 i386 gcc
   - RedHat 7.1 i386 (Kernel 2.4.10) gcc
   - Slackware 8.0 i686 (Kernel 2.4.10)
   - Solaris 8 UltraSparc cc
   - Solaris 8 UltraSparc gcc
   - Solaris 8 i386 gcc
   - SuSE 7.1 i386 gcc2.95.2
   - SuSE 7.1 ppc gcc2.95.2
   - i386-pc-sco3.2v5.0.5 cc
   - i386-pc-sco3.2v5.0.5 gcc
   - powerpc-ibm-aix4.3.3.0 cc
   - i686-unknown-sysv5UnixWare7.1.0 gcc
   - i686-unknown-sysv5UnixWare7.1.0 cc

### TESTING:

 - The existing test.sh script by Phil Hands has been merged into a
   test framework that works from both `make check` and the Samba
   build farm.

------------------------------------------------------------------------------

## Partial Protocol History

| RELEASE DATE | VER.   | DATE OF COMMIT\* | PROTOCOL    |
|--------------|--------|------------------|-------------|
| 16 Jan 2025  | 3.4.1  |                  | 32          |
| 15 Jan 2025  | 3.4.0  |                  | 32          |
| 06 Apr 2024  | 3.3.0  |                  | 31          |
| 20 Oct 2022  | 3.2.7  |                  | 31          |
| 09 Sep 2022  | 3.2.6  |                  | 31          |
| 14 Aug 2022  | 3.2.5  |                  | 31          |
| 15 Apr 2022  | 3.2.4  |                  | 31          |
| 06 Aug 2020  | 3.2.3  |                  | 31          |
| 04 Jul 2020  | 3.2.2  |                  | 31          |
| 22 Jun 2020  | 3.2.1  |                  | 31          |
| 19 Jun 2020  | 3.2.0  |                  | 31          |
| 28 Jan 2018  | 3.1.3  |                  | 31          |
| 21 Dec 2015  | 3.1.2  |                  | 31          |
| 22 Jun 2014  | 3.1.1  |                  | 31          |
| 28 Sep 2013  | 3.1.0  | 31 Aug 2008      | 31          |
| 23 Sep 2011  | 3.0.9  |                  | 30          |
| 26 Mar 2011  | 3.0.8  |                  | 30          |
| 31 Dec 2009  | 3.0.7  |                  | 30          |
| 08 May 2009  | 3.0.6  |                  | 30          |
| 28 Dec 2008  | 3.0.5  |                  | 30          |
| 06 Sep 2008  | 3.0.4  |                  | 30          |
| 29 Jun 2008  | 3.0.3  |                  | 30          |
| 08 Apr 2008  | 3.0.2  |                  | 30          |
| 03 Apr 2008  | 3.0.1  |                  | 30          |
| 01 Mar 2008  | 3.0.0  | 11 Nov 2006      | 30          |
| 06 Nov 2006  | 2.6.9  |                  | 29          |
| 22 Apr 2006  | 2.6.8  |                  | 29          |
| 11 Mar 2006  | 2.6.7  |                  | 29          |
| 28 Jul 2005  | 2.6.6  |                  | 29          |
| 01 Jun 2005  | 2.6.5  |                  | 29          |
| 30 Mar 2005  | 2.6.4  | 17 Jan 2005      | 29          |
| 30 Sep 2004  | 2.6.3  |                  | 28          |
| 30 Apr 2004  | 2.6.2  |                  | 28          |
| 26 Apr 2004  | 2.6.1  | 08 Jan 2004      | 28          |
| 01 Jan 2004  | 2.6.0  | 10 Apr 2003      | 27 (MAX=40) |
| 04 Dec 2003  | 2.5.7  |                  | 26          |
| 26 Jan 2003  | 2.5.6  |                  | 26          |
| 02 Apr 2002  | 2.5.5  |                  | 26          |
| 13 Mar 2002  | 2.5.4  |                  | 26          |
| 11 Mar 2002  | 2.5.3  |                  | 26          |
| 26 Jan 2002  | 2.5.2  | 11 Jan 2002      | 26          |
| 03 Jan 2002  | 2.5.1  |                  | 25          |
| 30 Nov 2001  | 2.5.0  | 23 Aug 2001      | 25          |
| 06 Sep 2000  | 2.4.6  |                  | 24          |
| 19 Aug 2000  | 2.4.5  |                  | 24          |
| 29 Jul 2000  | 2.4.4  |                  | 24          |
| 09 Apr 2000  | 2.4.3  |                  | 24          |
| 30 Mar 2000  | 2.4.2  |                  | 24          |
| 30 Jan 2000  | 2.4.1  | 29 Jan 2000      | 24          |
| 29 Jan 2000  | 2.4.0  | 28 Jan 2000      | 23          |
| 25 Jan 2000  | 2.3.3  | 23 Jan 2000      | 22          |
| 08 Nov 1999  | 2.3.2  | 26 Jun 1999      | 21          |
| 06 Apr 1999  | 2.3.1  |                  | 20          |
| 15 Mar 1999  | 2.3.0  | 15 Mar 1999      | 20          |
| 25 Nov 1998  | 2.2.1  |                  | 19          |
| 03 Nov 1998  | 2.2.0  |                  | 19          |
| 09 Sep 1998  | 2.1.1  |                  | 19          |
| 20 Jul 1998  | 2.1.0  |                  | 19          |
| 17 Jul 1998  | 2.0.19 |                  | 19          |
| 18 Jun 1998  | 2.0.17 |                  | 19          |
| 01 Jun 1998  | 2.0.16 |                  | 19          |
| 27 May 1998  | 2.0.13 | 27 May 1998      | 19          |
| 26 May 1998  | 2.0.12 |                  | 18          |
| 22 May 1998  | 2.0.11 |                  | 18          |
| 18 May 1998  | 2.0.9  | 18 May 1998      | 18          |
| 17 May 1998  | 2.0.8  |                  | 17          |
| 15 May 1998  | 2.0.1  |                  | 17          |
| 14 May 1998  | 2.0.0  |                  | 17          |
| 17 Apr 1998  | 1.7.4  |                  | 17          |
| 13 Apr 1998  | 1.7.3  |                  | 17          |
| 05 Apr 1998  | 1.7.2  |                  | 17          |
| 26 Mar 1998  | 1.7.1  |                  | 17          |
| 26 Mar 1998  | 1.7.0  | 26 Mar 1998      | 17 (MAX=30) |
| 13 Jan 1998  | 1.6.9  | 13 Jan 1998      | 15 (MAX=20) |

\* DATE OF COMMIT is the date the protocol change was committed to version
control.

@USE_GFM_PARSER@
