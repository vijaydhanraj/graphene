Manifest syntax
===============

.. highlight:: text

A |~| manifest file is an application-specific configuration text file that
specifies the environment and resources for running an application inside
Graphene. A |~| manifest file contains entries separated by line breaks. Each
configuration entry consists of a |~| key and a |~| value. Whitespaces
before/after the key and before/after the value are ignored. The value can be
written in quotes, indicating that the value should be assigned to this string
verbatim. (The quotes syntax is useful for values with leading/trailing
whitespaces, e.g. ``" SPACES! "``.) Each entry must be in the following format::

   [Key][.Key][.Key] = [Value]  or  [Key][.Key][.Key] = "[Value]"

Comments can be inlined in a |~| manifest by starting them with a |~| hash sign
(``# comment...``). Any text after a |~| hash sign will be considered part of
a |~| comment and discarded while loading the manifest file.

Common syntax
-------------

Debug type
^^^^^^^^^^

::

    loader.debug_type=[none|inline]
    (Default: none)

This specifies the debug option while running the library OS. If the debug type
is ``none``, no debug output will be printed to standard output. If the debug
type is ``inline``, a dmesg-like debug output will be printed inline with
standard output.

Executable
^^^^^^^^^^

::

   loader.exec=[URI]

This syntax specifies the executable to be loaded into the library OS. The
executable must be an ELF binary, with an entry point defined to start its
execution (i.e., the binary needs a `main()` routine, it cannot just be
a |~| library).

::

   loader.argv0_override=[STRING]

This syntax specifies an arbitrary string (typically the executable name) that
will be passed as the first argument (``argv[0]``) to the executable.

If the string is not specified in the manifest, the application will get
``argv[0]`` from :program:`pal_loader` invocation.

Preloaded libraries
^^^^^^^^^^^^^^^^^^^

::

   loader.preload=[URI][,URI]...

This syntax specifies the libraries to be preloaded before loading the
executable. The URIs of the libraries must be separated by commas. The libraries
must be ELF binaries. This syntax currently always contains the LibOS library
``libsysdb.so``.

Command-line arguments
^^^^^^^^^^^^^^^^^^^^^^

::

   loader.insecure__use_cmdline_argv = 1

or

::

   loader.argv_src_file = file:file_with_serialized_argv

If you want your application to use commandline arguments you need to either set
``loader.insecure__use_cmdline_argv`` (insecure in almost all cases) or point
``loader.argv_src_file`` to a file containing output of
:file:`Tools/argv_serializer`.

``loader.argv_src_file`` is intended to point to either a trusted file or a
protected file. The former allows to securely hardcode arguments (current
manifest syntax doesn't allow to include them inline), the latter allows the
arguments to be provided at runtime from an external (trusted) source. *NOTE:*
Pointing to a protected file is currently not supported, due to the fact that
PF wrap key provisioning currently happens after setting up arguments.

Environment variables
^^^^^^^^^^^^^^^^^^^^^

::

   loader.insecure__use_host_env = 1

By default, environment variables from the host will *not* be passed to the app.
This can be overridden by the option above, but most applications and runtime
libraries trust their environment variables and are completely insecure when
these are attacker-controlled. For example, an attacker can execute an
additional dynamic library by specifying ``LD_PRELOAD`` variable.

To securely set up the execution environment for an app you should use one or
both of the following options:

::

   loader.env.[ENVIRON]=[VALUE]
   loader.env_src_file = file:file_with_serialized_envs

``loader.env.[ENVIRON]`` adds/overwrites a single environment variable and can
be used multiple times to specify more than one variable.

``loader.env_src_file`` allows to specify a URI to a file containing serialized
environment, which can be generated using :file:`Tools/argv_serializer`. This
option is intended to point to either a trusted file or a protected file. The
former allows to securely hardcode environments (in a more flexible way than
``loader.env.[ENVIRON]`` option), the latter allows the environments to be
provided at runtime from an external (trusted) source. *NOTE:* Pointing to a
protected file is currently not supported, due to the fact that PF wrap key
provisioning currently happens after setting up environment variables.

If the same variable is set in both, then ``loader.env.[ENVIRON]`` takes
precedence.

Disabling ASLR
^^^^^^^^^^^^^^

::

    loader.insecure__disable_aslr=[1|0]
    (Default: 0)

This specifies whether to disable Address Space Layout Randomization (ASLR).
Since disabling ASLR worsens security of the application, ASLR is enabled by
default.

Stack size
^^^^^^^^^^

::

    sys.stack.size=[# of bytes (with K/M/G)]

This specifies the stack size of each thread in each Graphene process. The
default value is determined by the library OS. Units like ``K`` |~| (KiB),
``M`` |~| (MiB), and ``G`` |~| (GiB) can be appended to the values for
convenience. For example, ``sys.stack.size=1M`` indicates a 1 |~| MiB stack
size.

Program break (brk) size
^^^^^^^^^^^^^^^^^^^^^^^^

::

    sys.brk.max_size=[# of bytes (with K/M/G)]

This specifies the maximal program break (brk) size in each Graphene process.
The default value of the program break size is determined by the library OS.
Units like ``K`` (KiB), ``M`` (MiB), and ``G`` (GiB) can be appended to the
values for convenience. For example, ``sys.brk.max_size=1M`` indicates
a 1 |~| MiB brk size.

Allowing eventfd
^^^^^^^^^^^^^^^^

::

    sys.insecure__allow_eventfd=[1|0]
    (Default: 0)

This specifies whether to allow system calls `eventfd()` and `eventfd2()`. Since
eventfd emulation currently relies on the host, these system calls are
disallowed by default due to security concerns.

FS mount points
^^^^^^^^^^^^^^^

::

    fs.mount.[identifier].path=[PATH]
    fs.mount.[identifier].type=[chroot|...]
    fs.mount.[identifier].uri=[URI]

This syntax specifies how file systems are mounted inside the library OS. For
dynamically linked binaries, usually at least one mount point is required in the
manifest (the mount point of the Glibc library).


SGX syntax
----------

If Graphene is *not* running with SGX, the SGX-specific syntax is ignored. All
keys in the SGX-specific syntax are optional.

Debug/production enclave
^^^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.debug=[1|0]
    (Default: 1)

This syntax specifies whether the enclave can be debugged. Set it to ``1`` for
a |~| debug enclave and to ``0`` for a |~| production enclave.

Enclave size
^^^^^^^^^^^^

::

    sgx.enclave_size=[SIZE]
    (default: 256M)

This syntax specifies the size of the enclave set during enclave creation time
(recall that SGX |~| v1 requires a predetermined maximum size of the enclave).
The PAL and library OS code/data count towards this size value, as well as the
application memory itself: application's code, stack, heap, loaded application
libraries, etc. The application cannot allocate memory that exceeds this limit.

Number of threads
^^^^^^^^^^^^^^^^^

::

    sgx.thread_num=[NUM]
    (Default: 4)

This syntax specifies the maximum number of threads that can be created inside
the enclave (recall that SGX |~| v1 requires a |~| predetermined maximum number
of thread slots). The application cannot have more threads than this limit *at
a time* (however, it is possible to create new threads after old threads are
destroyed).

Number of RPC threads (Exitless feature)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.rpc_thread_num=[NUM]
    (Default: 0)

This syntax specifies the number of RPC threads that are created outside of
the enclave. RPC threads are helper threads that run in untrusted mode
alongside enclave threads. RPC threads issue system calls on behalf of enclave
threads. This allows "exitless" design when application threads never leave
the enclave (except for a few syscalls where there is no benefit, e.g.,
``nanosleep()``).

If user specifies ``0`` or omits this directive, then no RPC threads are
created and all system calls perform an enclave exit ("normal" execution).

Note that the number of created RPC threads must match the maximum number of
simultaneous enclave threads. If there are more RPC threads, then CPU time is
wasted. If there are less RPC threads, some enclave threads may starve,
especially if there are many blocking system calls by other enclave threads.

The Exitless feature *may be detrimental for performance*. It trades slow
OCALLs/ECALLs for fast shared-memory communication at the cost of occupying
more CPU cores and burning more CPU cycles. For example, a single-threaded
Redis instance on Linux becomes 5-threaded on Graphene with Exitless. Thus,
Exitless may negatively impact throughput but may improve latency.

Optional CPU features (AVX, AVX512, MPX)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.require_avx=[1|0]
    sgx.require_avx512=[1|0]
    sgx.require_mpx=[1|0]
    (Default: 0)

This syntax ensures that the CPU features are available and enabled for the
enclave. If the options are set in the manifest but the features are unavailable
on the platform, enclave initialization should fail. If the options are unset,
enclave initialization should succeed even if these features are unavailable on
the platform.

ISV Product ID and SVN
^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.isvprodid=[NUM]
    sgx.isnsvn=[NUM]
    (Default: 0)

This syntax specifies the ISV Product ID and SVN to be added to the enclave
signature.

Allowed files
^^^^^^^^^^^^^

::

    sgx.allowed_files.[identifier]=[URI]

This syntax specifies the files that are allowed to be loaded into the enclave
unconditionally. These files are not cryptographically hashed and are thus not
protected. It is insecure to allow files containing code or critical
information; developers must not allow files blindly! Instead, use trusted or
protected files.

Trusted files
^^^^^^^^^^^^^

::

    sgx.trusted_files.[identifier]=[URI]

This syntax specifies the files to be cryptographically hashed, and thus allowed
to be loaded into the enclave. The signer tool will automatically generate
hashes of these files and add them into the SGX-specific manifest
(``.manifest.sgx``). This is especially useful for shared libraries:
a |~| trusted library cannot be silently replaced by a malicious host because
the hash verification will fail.

Protected files
^^^^^^^^^^^^^^^

::

    sgx.protected_files_key=[16-byte hex value]
    sgx.protected_files.[identifier]=[URI]

This syntax specifies the files that are encrypted on disk and transparently
decrypted when accessed by Graphene or by application running inside Graphene.
Protected files guarantee data confidentiality and integrity (tamper
resistance), as well as file swap protection (a protected file can only be
accessed when in a specific path).

URIs can be files or directories. If a directory is specified, all existing
files/directories within it are registered as protected recursively (and are
expected to be encrypted in the PF format). New files created in a protected
directory are automatically treated as protected.

Note that path size of a protected file is limited to 512 bytes and filename
size is limited to 260 bytes.

``sgx.protected_files_key`` specifies the wrap (master) encryption key and must
be used only for debugging purposes. In production environments, this key must
be provisioned to the enclave using local/remote attestation.

Allowing file creation
^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.allow_file_creation=[1|0]
    (Default: 0)

This syntax specifies whether file creation is allowed from within the enclave.
Set it to ``1`` to allow enclaves to create files and to ``0`` otherwise. Files
created during enclave execution do not need to be marked as ``allowed_files``
or ``trusted_files``.

File check policy
^^^^^^^^^^^^^^^^^

::

    sgx.file_check_policy=[strict|allow_all_but_log]
    (Default: strict)

This syntax specifies the file check policy, determining the behavior of
authentication when opening files. By default, only files explicitly listed as
_trusted_files_ or _allowed_files_ declared in the manifest are allowed for
access. If the file check policy is ``allow_all_but_log``, all files other than
trusted and allowed are allowed for access, and Graphene-SGX emits a warning
message for every such file. This is a convenient way to determine the set of
files that the ported application uses.

Trusted child processes
^^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.trusted_children.[identifier]=[URI of signature (.sig)]

This syntax specifies the signatures of allowed child processes of the current
application. Upon process creation, the enclave in the current (parent) process
will attest the enclave in the child process, by comparing to the signatures of
the trusted children. If the child process is not trusted, the enclave will
refuse to communicate with it.

Attestation and quotes
^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.remote_attestation=[1|0]
    (Default: 0)
    sgx.ra_client_linkable=[1|0]
    (Default: 0)
    sgx.ra_client_spid=[HEX]

This syntax specifies the parameters for remote attestation. To enable it,
``remote_attestation`` must be set to ``1``.

For ECDSA/DCAP based attestation, no additional parameters are required. For
EPID based attestation, ``ra_client_linkable`` and ``ra_client_spid`` must
be additionally specified (linkable/unlinkable mode and SPID of the client
respectively).

Enabling per-thread and process-wide SGX stats
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

    sgx.enable_stats=[1|0]
    (Default: 0)

This syntax specifies whether to enable SGX enclave-specific statistics:

#. ``TCS.FLAGS.DBGOPTIN`` flag. This flag is set in all enclave threads and
   enables certain debug and profiling features with enclaves, including
   breakpoints, performance counters, Intel PT, etc.

#. Printing the stats on SGX-specific events. Currently supported stats are:
   number of EENTERs (corresponds to ECALLs plus returns from OCALLs), number
   of EEXITs (corresponds to OCALLs plus returns from ECALLs) and number of
   AEXs (corresponds to interrupts/exceptions/signals during enclave
   execution). Prints per-thread and per-process stats.

*Note:* this option is insecure and cannot be used with production enclaves
(``sgx.debug = 0``). If the production enclave is started with this option set,
Graphene will fail initialization of the enclave.
