/*
 * vr_exports - the export tables of every supported firmware, per library.
 *
 * Each table is generated from one firmware's decrypted library
 * (tools/gen_export_table.py --fw <tag>, docs/firmware-support.md). The app
 * uses the first table whose anchor string matches the loaded library, so a
 * table for one firmware can never be used on another.
 *
 * Several firmware ship the same build of a library, and then one table
 * serves them all (same offsets and anchor, checked from their libraries):
 *   1360  13.20, 13.60 (and 13.40's libSceVrTracker2)
 *   1240  12.20, 12.40
 *   1000  10.00, 10.01, 10.40, 10.60 (10.01: libSceHmd2 checked)
 *   761   7.20, 7.61 (7.20's from theghostonline/PS5VR#15, identical)
 * 11.60, 8.40 and 5.50 have their own. libSceVrHand is listed only
 * where its API matches 13.60's (12.20 and later): 10.x and 11.60 take other
 * Initialize/Start/GetResult parameters, so hand tracking is left off there.
 * libSceSystemService (closing the app) has tables for 13.60, 12.40, 10.40,
 * 10.01 and 5.50;
 * elsewhere the app ends its own process instead.
 *
 * Adding a firmware: include its header and add a line to each library's list
 * it has a table for.
 */
#include "vr_exports_1360.h"
#include "vr_exports_1240.h"
#include "vr_exports_1160.h"
#include "vr_exports_1040.h"
#include "vr_exports_1001.h"
#include "vr_exports_1000.h"
#include "vr_exports_840.h"
#include "vr_exports_761.h"
#include "vr_exports_550.h"

#define EXPORT_TABLE(fw, lib) \
    {fw, k##lib##Exports_##fw, COUNT(k##lib##Exports_##fw), &k##lib##Anchor_##fw}

static const export_table_t klibSceHmd2Tables[] = {
    EXPORT_TABLE(1360, libSceHmd2),
    EXPORT_TABLE(1240, libSceHmd2),
    EXPORT_TABLE(1160, libSceHmd2),
    EXPORT_TABLE(1000, libSceHmd2),
    EXPORT_TABLE(840, libSceHmd2),
    EXPORT_TABLE(761, libSceHmd2),
    EXPORT_TABLE(550, libSceHmd2),
};
static const export_table_t klibSceVrTracker2Tables[] = {
    EXPORT_TABLE(1360, libSceVrTracker2),
    EXPORT_TABLE(1240, libSceVrTracker2),
    EXPORT_TABLE(1160, libSceVrTracker2),
    EXPORT_TABLE(1000, libSceVrTracker2),
    EXPORT_TABLE(840, libSceVrTracker2),
    EXPORT_TABLE(761, libSceVrTracker2),
    EXPORT_TABLE(550, libSceVrTracker2),
};
static const export_table_t klibSceVrHandTables[] = {
    EXPORT_TABLE(1360, libSceVrHand),
    EXPORT_TABLE(1240, libSceVrHand),
};
static const export_table_t klibSceSystemServiceTables[] = {
    EXPORT_TABLE(1360, libSceSystemService),
    EXPORT_TABLE(1240, libSceSystemService),
    EXPORT_TABLE(1040, libSceSystemService),
    EXPORT_TABLE(1001, libSceSystemService),
    EXPORT_TABLE(550, libSceSystemService),
};
