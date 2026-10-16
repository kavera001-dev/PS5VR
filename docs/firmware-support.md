# Running PS5VR on other firmware

PS5VR ships export tables for the firmware below. 13.60 and 12.40 have been
tested on a console; the others were checked against that firmware's own
libraries but have not been run. This page is for anyone on another firmware
who wants theirs supported, and explains why it cannot be done from a 13.60
console.

| Firmware | Status | Not available |
|---|---|---|
| 13.60 | tested | |
| 12.40 | tested ¹ | |
| 13.20, 13.40 ², 12.20 | checked | |
| 10.00, 10.40, 10.60, 11.60 | checked | hand tracking |
| 7.20, 7.61, 8.40 | checked | hand tracking |
| 5.50 | checked ³ | hand tracking |

1. Playback can become choppy after a few seconds (being investigated).
2. Only 13.40's `libSceVrTracker2` was checked; it matches 13.60's.
3. 5.50 predates PS VR2's release, so headset support there may be incomplete.

`libSceSystemService` (clean exit) is still needed for 13.40, 11.60, 8.40, 7.20
and 7.61; without it the app ends its own process instead.

Libraries were shared by @rmznoky (5.50), @tertiumndatur (7.20), @tvk-source
(7.61), @Vorenu5 (8.40), @hu601 (10.00), @ytc44 (10.40), @hsv2049cs (10.60),
@jimicaoo (11.60), @mschifino (12.20), @mtp-24 (12.40), @Gitlizard2 (13.20)
and @jarmster (13.40). Thank you.

## Why there is a table at all

The PS VR2 system libraries (`libSceHmd2`, `libSceVrTracker2`, `libSceVrHand`)
keep the functions PS5VR needs out of their dynamic symbol tables, so there is
nothing to `sceKernelDlsym`. The app calls them at fixed offsets inside the
library instead, and those offsets are specific to one build of one firmware.

Nothing bad happens on the wrong firmware. Each table carries an *anchor*: a
string that must sit at a known offset in the library's read-only data. If it
does not match, the offsets are not used and PS5VR stops instead of calling
into the wrong place.

## Why a 13.60 console cannot produce another firmware's table

The offsets are read out of the decrypted library itself. A console only has
its own firmware's copy, so a 13.60 machine can only ever generate the 13.60
table. Supporting 12.40 needs the library from a 12.40 console.

## What is needed from you

Four decrypted libraries from a console on your firmware:

- `libSceHmd2.sprx`
- `libSceVrTracker2.sprx`
- `libSceVrHand.sprx`
- `libSceSystemService.sprx`

They live in `/system/common/lib/` and are encrypted on disk. Decrypt them
with `ps5-self-pager`, which is what the tooling here expects as input.

Open an issue saying which firmware you are on and attach them, or the
generated table if you would rather run the generator yourself:

```sh
python3 tools/gen_export_table.py --fw <fw> --optional <HMD2_OPT_FUNCS names> \
    libSceHmd2.sprx      sceHmd2Initialize sceHmd2Open ... \
    -- libSceVrTracker2.sprx  sceVrTracker2Init ... \
    -- libSceVrHand.sprx      sceVrHandInit ... \
    -- libSceSystemService.sprx sceShellCoreUtilExitApp \
  > app/src/vr_exports_<fw>.h
```

`<fw>` is the firmware without the dot (`1240`). Then add the header to
`app/src/vr_exports.h`.

Run `python3 tools/ps5_dynlib.py <lib.sprx>` first to list what a library
exports on your firmware. The names PS5VR needs are the ones already listed in
`app/src/vr_exports_1360.h`.

Please only send libraries from your own console.

## Then what

The app picks its table by anchor, so adding one is additive: a new firmware
cannot affect 13.60. Once a table is in, the only remaining work is a run
through the library and playback on that firmware to confirm nothing else
moved.
