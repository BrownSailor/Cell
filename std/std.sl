// slang standard library

def NULL 0 end

def tru 0 0  = end
def fls 0 0 != end


def stdin  0 end
def stdout 1 end
def stderr 2 end

def SYS_syscall                             0   33554432 + end
def SYS_exit                                1   33554432 + end
def SYS_fork                                2   33554432 + end
def SYS_read                                3   33554432 + end
def SYS_write                               4   33554432 + end
def SYS_open                                5   33554432 + end
def SYS_close                               6   33554432 + end
def SYS_wait4                               7   33554432 + end
def SYS_link                                9   33554432 + end
def SYS_unlink                              10  33554432 + end
def SYS_chdir                               12  33554432 + end
def SYS_fchdir                              13  33554432 + end
def SYS_mknod                               14  33554432 + end
def SYS_chmod                               15  33554432 + end
def SYS_chown                               16  33554432 + end
def SYS_getfsstat                           18  33554432 + end
def SYS_getpid                              20  33554432 + end
def SYS_setuid                              23  33554432 + end
def SYS_getuid                              24  33554432 + end
def SYS_geteuid                             25  33554432 + end
def SYS_ptrace                              26  33554432 + end
def SYS_recvmsg                             27  33554432 + end
def SYS_sendmsg                             28  33554432 + end
def SYS_recvfrom                            29  33554432 + end
def SYS_accept                              30  33554432 + end
def SYS_getpeername                         31  33554432 + end
def SYS_getsockname                         32  33554432 + end
def SYS_access                              33  33554432 + end
def SYS_chflags                             34  33554432 + end
def SYS_fchflags                            35  33554432 + end
def SYS_sync                                36  33554432 + end
def SYS_kill                                37  33554432 + end
def SYS_getppid                             39  33554432 + end
def SYS_dup                                 41  33554432 + end
def SYS_pipe                                42  33554432 + end
def SYS_getegid                             43  33554432 + end
def SYS_sigaction                           46  33554432 + end
def SYS_getgid                              47  33554432 + end
def SYS_sigprocmask                         48  33554432 + end
def SYS_getlogin                            49  33554432 + end
def SYS_setlogin                            50  33554432 + end
def SYS_acct                                51  33554432 + end
def SYS_sigpending                          52  33554432 + end
def SYS_sigaltstack                         53  33554432 + end
def SYS_ioctl                               54  33554432 + end
def SYS_reboot                              55  33554432 + end
def SYS_revoke                              56  33554432 + end
def SYS_symlink                             57  33554432 + end
def SYS_readlink                            58  33554432 + end
def SYS_execve                              59  33554432 + end
def SYS_umask                               60  33554432 + end
def SYS_chroot                              61  33554432 + end
def SYS_msync                               65  33554432 + end
def SYS_vfork                               66  33554432 + end
def SYS_munmap                              73  33554432 + end
def SYS_mprotect                            74  33554432 + end
def SYS_madvise                             75  33554432 + end
def SYS_mincore                             78  33554432 + end
def SYS_getgroups                           79  33554432 + end
def SYS_setgroups                           80  33554432 + end
def SYS_getpgrp                             81  33554432 + end
def SYS_setpgid                             82  33554432 + end
def SYS_setitimer                           83  33554432 + end
def SYS_swapon                              85  33554432 + end
def SYS_getitimer                           86  33554432 + end
def SYS_getdtablesize                       89  33554432 + end
def SYS_dup2                                90  33554432 + end
def SYS_fcntl                               92  33554432 + end
def SYS_select                              93  33554432 + end
def SYS_fsync                               95  33554432 + end
def SYS_setpriority                         96  33554432 + end
def SYS_socket                              97  33554432 + end
def SYS_connect                             98  33554432 + end
def SYS_getpriority                         100 33554432 + end
def SYS_bind                                104 33554432 + end
def SYS_setsockopt                          105 33554432 + end
def SYS_listen                              106 33554432 + end
def SYS_sigsuspend                          111 33554432 + end
def SYS_gettimeofday                        116 33554432 + end
def SYS_getrusage                           117 33554432 + end
def SYS_getsockopt                          118 33554432 + end
def SYS_readv                               120 33554432 + end
def SYS_writev                              121 33554432 + end
def SYS_settimeofday                        122 33554432 + end
def SYS_fchown                              123 33554432 + end
def SYS_fchmod                              124 33554432 + end
def SYS_setreuid                            126 33554432 + end
def SYS_setregid                            127 33554432 + end
def SYS_rename                              128 33554432 + end
def SYS_flock                               131 33554432 + end
def SYS_mkfifo                              132 33554432 + end
def SYS_sendto                              133 33554432 + end
def SYS_shutdown                            134 33554432 + end
def SYS_socketpair                          135 33554432 + end
def SYS_mkdir                               136 33554432 + end
def SYS_rmdir                               137 33554432 + end
def SYS_utimes                              138 33554432 + end
def SYS_futimes                             139 33554432 + end
def SYS_adjtime                             140 33554432 + end
def SYS_gethostuuid                         142 33554432 + end
def SYS_setsid                              147 33554432 + end
def SYS_getpgid                             151 33554432 + end
def SYS_setprivexec                         152 33554432 + end
def SYS_pread                               153 33554432 + end
def SYS_pwrite                              154 33554432 + end
def SYS_nfssvc                              155 33554432 + end
def SYS_statfs                              157 33554432 + end
def SYS_fstatfs                             158 33554432 + end
def SYS_unmount                             159 33554432 + end
def SYS_getfh                               161 33554432 + end
def SYS_quotactl                            165 33554432 + end
def SYS_mount                               167 33554432 + end
def SYS_csops                               169 33554432 + end
def SYS_csops_audittoken                    170 33554432 + end
def SYS_waitid                              173 33554432 + end
def SYS_kdebug_typefilter                   177 33554432 + end
def SYS_kdebug_trace_string                 178 33554432 + end
def SYS_kdebug_trace64                      179 33554432 + end
def SYS_kdebug_trace                        180 33554432 + end
def SYS_setgid                              181 33554432 + end
def SYS_setegid                             182 33554432 + end
def SYS_seteuid                             183 33554432 + end
def SYS_sigreturn                           184 33554432 + end
def SYS_thread_selfcounts                   186 33554432 + end
def SYS_fdatasync                           187 33554432 + end
def SYS_stat                                188 33554432 + end
def SYS_fstat                               189 33554432 + end
def SYS_lstat                               190 33554432 + end
def SYS_pathconf                            191 33554432 + end
def SYS_fpathconf                           192 33554432 + end
def SYS_getrlimit                           194 33554432 + end
def SYS_setrlimit                           195 33554432 + end
def SYS_getdirentries                       196 33554432 + end
def SYS_mmap                                197 33554432 + end
def SYS_lseek                               199 33554432 + end
def SYS_truncate                            200 33554432 + end
def SYS_ftruncate                           201 33554432 + end
def SYS_sysctl                              202 33554432 + end
def SYS_mlock                               203 33554432 + end
def SYS_munlock                             204 33554432 + end
def SYS_undelete                            205 33554432 + end
def SYS_open_dprotected_np                  216 33554432 + end
def SYS_fsgetpath_ext                       217 33554432 + end
def SYS_getattrlist                         220 33554432 + end
def SYS_setattrlist                         221 33554432 + end
def SYS_getdirentriesattr                   222 33554432 + end
def SYS_exchangedata                        223 33554432 + end
def SYS_searchfs                            225 33554432 + end
def SYS_delete                              226 33554432 + end
def SYS_copyfile                            227 33554432 + end
def SYS_fgetattrlist                        228 33554432 + end
def SYS_fsetattrlist                        229 33554432 + end
def SYS_poll                                230 33554432 + end
def SYS_getxattr                            234 33554432 + end
def SYS_fgetxattr                           235 33554432 + end
def SYS_setxattr                            236 33554432 + end
def SYS_fsetxattr                           237 33554432 + end
def SYS_removexattr                         238 33554432 + end
def SYS_fremovexattr                        239 33554432 + end
def SYS_listxattr                           240 33554432 + end
def SYS_flistxattr                          241 33554432 + end
def SYS_fsctl                               242 33554432 + end
def SYS_initgroups                          243 33554432 + end
def SYS_posix_spawn                         244 33554432 + end
def SYS_ffsctl                              245 33554432 + end
def SYS_nfsclnt                             247 33554432 + end
def SYS_fhopen                              248 33554432 + end
def SYS_minherit                            250 33554432 + end
def SYS_semsys                              251 33554432 + end
def SYS_msgsys                              252 33554432 + end
def SYS_shmsys                              253 33554432 + end
def SYS_semctl                              254 33554432 + end
def SYS_semget                              255 33554432 + end
def SYS_semop                               256 33554432 + end
def SYS_msgctl                              258 33554432 + end
def SYS_msgget                              259 33554432 + end
def SYS_msgsnd                              260 33554432 + end
def SYS_msgrcv                              261 33554432 + end
def SYS_shmat                               262 33554432 + end
def SYS_shmctl                              263 33554432 + end
def SYS_shmdt                               264 33554432 + end
def SYS_shmget                              265 33554432 + end
def SYS_shm_open                            266 33554432 + end
def SYS_shm_unlink                          267 33554432 + end
def SYS_sem_open                            268 33554432 + end
def SYS_sem_close                           269 33554432 + end
def SYS_sem_unlink                          270 33554432 + end
def SYS_sem_wait                            271 33554432 + end
def SYS_sem_trywait                         272 33554432 + end
def SYS_sem_post                            273 33554432 + end
def SYS_sysctlbyname                        274 33554432 + end
def SYS_open_extended                       277 33554432 + end
def SYS_umask_extended                      278 33554432 + end
def SYS_stat_extended                       279 33554432 + end
def SYS_lstat_extended                      280 33554432 + end
def SYS_fstat_extended                      281 33554432 + end
def SYS_chmod_extended                      282 33554432 + end
def SYS_fchmod_extended                     283 33554432 + end
def SYS_access_extended                     284 33554432 + end
def SYS_settid                              285 33554432 + end
def SYS_gettid                              286 33554432 + end
def SYS_setsgroups                          287 33554432 + end
def SYS_getsgroups                          288 33554432 + end
def SYS_setwgroups                          289 33554432 + end
def SYS_getwgroups                          290 33554432 + end
def SYS_mkfifo_extended                     291 33554432 + end
def SYS_mkdir_extended                      292 33554432 + end
def SYS_identitysvc                         293 33554432 + end
def SYS_shared_region_check_np              294 33554432 + end
def SYS_vm_pressure_monitor                 296 33554432 + end
def SYS_psynch_rw_longrdlock                297 33554432 + end
def SYS_psynch_rw_yieldwrlock               298 33554432 + end
def SYS_psynch_rw_downgrade                 299 33554432 + end
def SYS_psynch_rw_upgrade                   300 33554432 + end
def SYS_psynch_mutexwait                    301 33554432 + end
def SYS_psynch_mutexdrop                    302 33554432 + end
def SYS_psynch_cvbroad                      303 33554432 + end
def SYS_psynch_cvsignal                     304 33554432 + end
def SYS_psynch_cvwait                       305 33554432 + end
def SYS_psynch_rw_rdlock                    306 33554432 + end
def SYS_psynch_rw_wrlock                    307 33554432 + end
def SYS_psynch_rw_unlock                    308 33554432 + end
def SYS_psynch_rw_unlock2                   309 33554432 + end
def SYS_getsid                              310 33554432 + end
def SYS_settid_with_pid                     311 33554432 + end
def SYS_psynch_cvclrprepost                 312 33554432 + end
def SYS_aio_fsync                           313 33554432 + end
def SYS_aio_return                          314 33554432 + end
def SYS_aio_suspend                         315 33554432 + end
def SYS_aio_cancel                          316 33554432 + end
def SYS_aio_error                           317 33554432 + end
def SYS_aio_read                            318 33554432 + end
def SYS_aio_write                           319 33554432 + end
def SYS_lio_listio                          320 33554432 + end
def SYS_iopolicysys                         322 33554432 + end
def SYS_process_policy                      323 33554432 + end
def SYS_mlockall                            324 33554432 + end
def SYS_munlockall                          325 33554432 + end
def SYS_issetugid                           327 33554432 + end
def SYS___pthread_kill                      328 33554432 + end
def SYS___pthread_sigmask                   329 33554432 + end
def SYS___sigwait                           330 33554432 + end
def SYS___disable_threadsignal              331 33554432 + end
def SYS___pthread_markcancel                332 33554432 + end
def SYS___pthread_canceled                  333 33554432 + end
def SYS___semwait_signal                    334 33554432 + end
def SYS_proc_info                           336 33554432 + end
def SYS_sendfile                            337 33554432 + end
def SYS_stat64                              338 33554432 + end
def SYS_fstat64                             339 33554432 + end
def SYS_lstat64                             340 33554432 + end
def SYS_stat64_extended                     341 33554432 + end
def SYS_lstat64_extended                    342 33554432 + end
def SYS_fstat64_extended                    343 33554432 + end
def SYS_getdirentries64                     344 33554432 + end
def SYS_statfs64                            345 33554432 + end
def SYS_fstatfs64                           346 33554432 + end
def SYS_getfsstat64                         347 33554432 + end
def SYS___pthread_chdir                     348 33554432 + end
def SYS___pthread_fchdir                    349 33554432 + end
def SYS_audit                               350 33554432 + end
def SYS_auditon                             351 33554432 + end
def SYS_getauid                             353 33554432 + end
def SYS_setauid                             354 33554432 + end
def SYS_getaudit_addr                       357 33554432 + end
def SYS_setaudit_addr                       358 33554432 + end
def SYS_auditctl                            359 33554432 + end
def SYS_bsdthread_create                    360 33554432 + end
def SYS_bsdthread_terminate                 361 33554432 + end
def SYS_kqueue                              362 33554432 + end
def SYS_kevent                              363 33554432 + end
def SYS_lchown                              364 33554432 + end
def SYS_bsdthread_register                  366 33554432 + end
def SYS_workq_open                          367 33554432 + end
def SYS_workq_kernreturn                    368 33554432 + end
def SYS_kevent64                            369 33554432 + end
def SYS___old_semwait_signal                370 33554432 + end
def SYS___old_semwait_signal_nocancel       371 33554432 + end
def SYS_thread_selfid                       372 33554432 + end
def SYS_ledger                              373 33554432 + end
def SYS_kevent_qos                          374 33554432 + end
def SYS_kevent_id                           375 33554432 + end
def SYS___mac_execve                        380 33554432 + end
def SYS___mac_syscall                       381 33554432 + end
def SYS___mac_get_file                      382 33554432 + end
def SYS___mac_set_file                      383 33554432 + end
def SYS___mac_get_link                      384 33554432 + end
def SYS___mac_set_link                      385 33554432 + end
def SYS___mac_get_proc                      386 33554432 + end
def SYS___mac_set_proc                      387 33554432 + end
def SYS___mac_get_fd                        388 33554432 + end
def SYS___mac_set_fd                        389 33554432 + end
def SYS___mac_get_pid                       390 33554432 + end
def SYS_pselect                             394 33554432 + end
def SYS_pselect_nocancel                    395 33554432 + end
def SYS_read_nocancel                       396 33554432 + end
def SYS_write_nocancel                      397 33554432 + end
def SYS_open_nocancel                       398 33554432 + end
def SYS_close_nocancel                      399 33554432 + end
def SYS_wait4_nocancel                      400 33554432 + end
def SYS_recvmsg_nocancel                    401 33554432 + end
def SYS_sendmsg_nocancel                    402 33554432 + end
def SYS_recvfrom_nocancel                   403 33554432 + end
def SYS_accept_nocancel                     404 33554432 + end
def SYS_msync_nocancel                      405 33554432 + end
def SYS_fcntl_nocancel                      406 33554432 + end
def SYS_select_nocancel                     407 33554432 + end
def SYS_fsync_nocancel                      408 33554432 + end
def SYS_connect_nocancel                    409 33554432 + end
def SYS_sigsuspend_nocancel                 410 33554432 + end
def SYS_readv_nocancel                      411 33554432 + end
def SYS_writev_nocancel                     412 33554432 + end
def SYS_sendto_nocancel                     413 33554432 + end
def SYS_pread_nocancel                      414 33554432 + end
def SYS_pwrite_nocancel                     415 33554432 + end
def SYS_waitid_nocancel                     416 33554432 + end
def SYS_poll_nocancel                       417 33554432 + end
def SYS_msgsnd_nocancel                     418 33554432 + end
def SYS_msgrcv_nocancel                     419 33554432 + end
def SYS_sem_wait_nocancel                   420 33554432 + end
def SYS_aio_suspend_nocancel                421 33554432 + end
def SYS___sigwait_nocancel                  422 33554432 + end
def SYS___semwait_signal_nocancel           423 33554432 + end
def SYS___mac_mount                         424 33554432 + end
def SYS___mac_get_mount                     425 33554432 + end
def SYS___mac_getfsstat                     426 33554432 + end
def SYS_fsgetpath                           427 33554432 + end
def SYS_audit_session_self                  428 33554432 + end
def SYS_audit_session_join                  429 33554432 + end
def SYS_fileport_makeport                   430 33554432 + end
def SYS_fileport_makefd                     431 33554432 + end
def SYS_audit_session_port                  432 33554432 + end
def SYS_pid_suspend                         433 33554432 + end
def SYS_pid_resume                          434 33554432 + end
def SYS_pid_hibernate                       435 33554432 + end
def SYS_pid_shutdown_sockets                436 33554432 + end
def SYS_shared_region_map_and_slide_np      438 33554432 + end
def SYS_kas_info                            439 33554432 + end
def SYS_memorystatus_control                440 33554432 + end
def SYS_guarded_open_np                     441 33554432 + end
def SYS_guarded_close_np                    442 33554432 + end
def SYS_guarded_kqueue_np                   443 33554432 + end
def SYS_change_fdguard_np                   444 33554432 + end
def SYS_usrctl                              445 33554432 + end
def SYS_proc_rlimit_control                 446 33554432 + end
def SYS_connectx                            447 33554432 + end
def SYS_disconnectx                         448 33554432 + end
def SYS_peeloff                             449 33554432 + end
def SYS_socket_delegate                     450 33554432 + end
def SYS_telemetry                           451 33554432 + end
def SYS_proc_uuid_policy                    452 33554432 + end
def SYS_memorystatus_get_level              453 33554432 + end
def SYS_system_override                     454 33554432 + end
def SYS_vfs_purge                           455 33554432 + end
def SYS_sfi_ctl                             456 33554432 + end
def SYS_sfi_pidctl                          457 33554432 + end
def SYS_coalition                           458 33554432 + end
def SYS_coalition_info                      459 33554432 + end
def SYS_necp_match_policy                   460 33554432 + end
def SYS_getattrlistbulk                     461 33554432 + end
def SYS_clonefileat                         462 33554432 + end
def SYS_openat                              463 33554432 + end
def SYS_openat_nocancel                     464 33554432 + end
def SYS_renameat                            465 33554432 + end
def SYS_faccessat                           466 33554432 + end
def SYS_fchmodat                            467 33554432 + end
def SYS_fchownat                            468 33554432 + end
def SYS_fstatat                             469 33554432 + end
def SYS_fstatat64                           470 33554432 + end
def SYS_linkat                              471 33554432 + end
def SYS_unlinkat                            472 33554432 + end
def SYS_readlinkat                          473 33554432 + end
def SYS_symlinkat                           474 33554432 + end
def SYS_mkdirat                             475 33554432 + end
def SYS_getattrlistat                       476 33554432 + end
def SYS_proc_trace_log                      477 33554432 + end
def SYS_bsdthread_ctl                       478 33554432 + end
def SYS_openbyid_np                         479 33554432 + end
def SYS_recvmsg_x                           480 33554432 + end
def SYS_sendmsg_x                           481 33554432 + end
def SYS_thread_selfusage                    482 33554432 + end
def SYS_csrctl                              483 33554432 + end
def SYS_guarded_open_dprotected_np          484 33554432 + end
def SYS_guarded_write_np                    485 33554432 + end
def SYS_guarded_pwrite_np                   486 33554432 + end
def SYS_guarded_writev_np                   487 33554432 + end
def SYS_renameatx_np                        488 33554432 + end
def SYS_mremap_encrypted                    489 33554432 + end
def SYS_netagent_trigger                    490 33554432 + end
def SYS_stack_snapshot_with_config          491 33554432 + end
def SYS_microstackshot                      492 33554432 + end
def SYS_grab_pgo_data                       493 33554432 + end
def SYS_persona                             494 33554432 + end
def SYS_mach_eventlink_signal               496 33554432 + end
def SYS_mach_eventlink_wait_until           497 33554432 + end
def SYS_mach_eventlink_signal_wait_until    498 33554432 + end
def SYS_work_interval_ctl                   499 33554432 + end
def SYS_getentropy                          500 33554432 + end
def SYS_necp_open                           501 33554432 + end
def SYS_necp_client_action                  502 33554432 + end
def SYS___nexus_open                        503 33554432 + end
def SYS___nexus_register                    504 33554432 + end
def SYS___nexus_deregister                  505 33554432 + end
def SYS___nexus_create                      506 33554432 + end
def SYS___nexus_destroy                     507 33554432 + end
def SYS___nexus_get_opt                     508 33554432 + end
def SYS___nexus_set_opt                     509 33554432 + end
def SYS___channel_open                      510 33554432 + end
def SYS___channel_get_info                  511 33554432 + end
def SYS___channel_sync                      512 33554432 + end
def SYS___channel_get_opt                   513 33554432 + end
def SYS___channel_set_opt                   514 33554432 + end
def SYS_ulock_wait                          515 33554432 + end
def SYS_ulock_wake                          516 33554432 + end
def SYS_fclonefileat                        517 33554432 + end
def SYS_fs_snapshot                         518 33554432 + end
def SYS_register_uexc_handler               519 33554432 + end
def SYS_terminate_with_payload              520 33554432 + end
def SYS_abort_with_payload                  521 33554432 + end
def SYS_necp_session_open                   522 33554432 + end
def SYS_necp_session_action                 523 33554432 + end
def SYS_setattrlistat                       524 33554432 + end
def SYS_net_qos_guideline                   525 33554432 + end
def SYS_fmount                              526 33554432 + end
def SYS_ntp_adjtime                         527 33554432 + end
def SYS_ntp_gettime                         528 33554432 + end
def SYS_os_fault_with_payload               529 33554432 + end
def SYS_kqueue_workloop_ctl                 530 33554432 + end
def SYS___mach_bridge_remote_time           531 33554432 + end
def SYS_coalition_ledger                    532 33554432 + end
def SYS_log_data                            533 33554432 + end
def SYS_memorystatus_available_memory       534 33554432 + end
def SYS_shared_region_map_and_slide_2_np    536 33554432 + end
def SYS_pivot_root                          537 33554432 + end
def SYS_task_inspect_for_pid                538 33554432 + end
def SYS_task_read_for_pid                   539 33554432 + end
def SYS_preadv                              540 33554432 + end
def SYS_pwritev                             541 33554432 + end
def SYS_preadv_nocancel                     542 33554432 + end
def SYS_pwritev_nocancel                    543 33554432 + end
def SYS_ulock_wait2                         544 33554432 + end
def SYS_proc_info_extended_id               545 33554432 + end
def SYS_tracker_action                      546 33554432 + end
def SYS_debug_syscall_reject                547 33554432 + end
def SYS_MAXSYSCALL	                        548 33554432 + end
def SYS_invalid	                            63  33554432 + end

def write SYS_write sys3 end
def read  SYS_read  sys3 end
def exit  SYS_exit  sys1 end

def / divmod drop end
def % divmod swap drop end 
def mod % end
def div / end

def dup2  over over end
def drop2 drop drop end

def inc64 
    dup @64 1 + !64
end

def dec64
    dup @64 1 - !64
end

def inc32
    dup @32 1 + !32
end

def dec32
    dup @32 1 - !32
end

def inc16
    dup @16 1 + !16
end

def dec16
    dup @16 1 - !16
end

def sizeof(u64) 8 end
def sizeof(u32) 4 end
def sizeof(u16) 2 end
def sizeof(u8)  1 end

def sizeof(ptr) sizeof(u64) end