#ifndef __Fjords_FjordsSound_h__
#define __Fjords_FjordsSound_h__
extern double ss_init();
extern double ss_unload();
double fjords_setsoundscalefactor(double factor);
extern double ss_setvol(double soundid, double volume);
extern double ss_loadsound(std::string fname);
extern double ss_loadmusic(std::string fname);
extern double ss_playsound(double soundid);
extern double ss_isplaying(double soundid);
extern double ss_loopsound(double soundid);
extern double ss_islooping(double soundid);
extern double ss_stopsound(double soundid);
extern double ss_setvol(double soundid, double volume);
extern double ss_getvol(double soundid);
extern double ss_setfreqAL(double soundid, double alvinandthechipmunks);
extern double ss_setfreq(double soundid, double alvinandthechipmunks);
extern double ss_getfreq(double soundid);
extern double ss_stopall();
extern double ss_getnextsoundid();
extern std::string ss_getname(double soundid);
#endif