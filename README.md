# PSC

# New refactor!

- server folder contains server sources, use cmake, any version is fine
- hedrot static lib is specifically for windows (ios::binary mode for 0x0D0A prevention)
- soundfile static lib has been modified by me to work with non standard PCM WAV headers
- all libs built with minGW, DLLs guaranteed to work with dlltool without special DEF export
- client folder contains client sources (obviously), use latest gradle to build, Android API 25 needed to run (it uses recent filestream functions)
