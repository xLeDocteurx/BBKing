#include <vector>
#include <string>

#include <cJSON.h>
// #include <esp_psram.h>
// #include <esp_system.h>
// #include <driver/spi_master.h>

#include <Defs.h>
#include <MyUtils.h>
#include <SDCard.h>
#include <Init.h>

// bool initState(&statePointer auto)
bool initState(State *statePointer)
{

    statePointer->wavFilePaths.push_back("/data/kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/1_micro kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/2_nt kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/3_nt kick b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/4_nt kick c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/5_nt kick d.wav");
    statePointer->wavFilePaths.push_back("/sdcard/6_lock kik.wav");
    statePointer->wavFilePaths.push_back("/sdcard/7_kick grit.wav");
    statePointer->wavFilePaths.push_back("/sdcard/8_kick verb.wav");
    statePointer->wavFilePaths.push_back("/sdcard/9_kick mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/10_kick bump.wav");
    statePointer->wavFilePaths.push_back("/sdcard/11_kick 9x9 dirt.wav");
    statePointer->wavFilePaths.push_back("/sdcard/12_kick 9x9 dirt b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/13_kick 9x9 dirt c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/14_kick 9x9 dirt d.wav");
    statePointer->wavFilePaths.push_back("/sdcard/15_kick 9x9 dirt e.wav");
    statePointer->wavFilePaths.push_back("/sdcard/16_kick hard.wav");
    statePointer->wavFilePaths.push_back("/sdcard/17_kick air.wav");
    statePointer->wavFilePaths.push_back("/sdcard/18_kick thud.wav");
    statePointer->wavFilePaths.push_back("/sdcard/19_cardio kick 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/20_afterparty kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/21_nt alt kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/22_nt alt kick b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/23_nt alt kick c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/24_kick dirt.wav");
    statePointer->wavFilePaths.push_back("/sdcard/25_kick sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/26_kick sp.wav");
    statePointer->wavFilePaths.push_back("/sdcard/27_kick flux.wav");
    statePointer->wavFilePaths.push_back("/sdcard/28_kick flux b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/29_kick sub b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/30_kick open.wav");
    statePointer->wavFilePaths.push_back("/sdcard/31_boomer kick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/100_nt snare.wav");
    statePointer->wavFilePaths.push_back("/sdcard/101_nt snare b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/102_nt snare c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/103_snare lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/104_snare mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/105_snare hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/106_snare 6x6 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/107_snare 6x6 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/108_snare 6x6 3.wav");
    statePointer->wavFilePaths.push_back("/sdcard/109_snare classic.wav");
    statePointer->wavFilePaths.push_back("/sdcard/110_snare fat.wav");
    statePointer->wavFilePaths.push_back("/sdcard/111_snare smack.wav");
    statePointer->wavFilePaths.push_back("/sdcard/112_snare burst.wav");
    statePointer->wavFilePaths.push_back("/sdcard/113_brushy snare 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/114_nt snare alt.wav");
    statePointer->wavFilePaths.push_back("/sdcard/115_nt snare alt b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/116_nt snare alt c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/117_snare rim.wav");
    statePointer->wavFilePaths.push_back("/sdcard/118_snare punch.wav");
    statePointer->wavFilePaths.push_back("/sdcard/119_snare pop.wav");
    statePointer->wavFilePaths.push_back("/sdcard/120_snare snap.wav");
    statePointer->wavFilePaths.push_back("/sdcard/121_snare vinyl.wav");
    statePointer->wavFilePaths.push_back("/sdcard/122_snare mp3k.wav");
    statePointer->wavFilePaths.push_back("/sdcard/123_snare mini bm.wav");
    statePointer->wavFilePaths.push_back("/sdcard/124_snare vinyl b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/125_snare res.wav");
    statePointer->wavFilePaths.push_back("/sdcard/126_snare open.wav");
    statePointer->wavFilePaths.push_back("/sdcard/127_snare wood.wav");
    statePointer->wavFilePaths.push_back("/sdcard/128_quik roll 3.wav");
    statePointer->wavFilePaths.push_back("/sdcard/129_london snare 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/130_nt rimshot.wav");
    statePointer->wavFilePaths.push_back("/sdcard/131_nt rimshot b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/132_nt rimshot c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/133_rim lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/134_rim mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/135_rim hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/136_rimshot ddd1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/137_rimshot vinyl.wav");
    statePointer->wavFilePaths.push_back("/sdcard/138_rimshot 7x7 sp.wav");
    statePointer->wavFilePaths.push_back("/sdcard/139_rimshot dmx.wav");
    statePointer->wavFilePaths.push_back("/sdcard/140_rim dark.wav");
    statePointer->wavFilePaths.push_back("/sdcard/141_rim smack.wav");
    statePointer->wavFilePaths.push_back("/sdcard/142_rim snap.wav");
    statePointer->wavFilePaths.push_back("/sdcard/143_know rimshot 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/144_sidequest snare.wav");
    statePointer->wavFilePaths.push_back("/sdcard/145_akkountant rim 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/200_nt hh closed.wav");
    statePointer->wavFilePaths.push_back("/sdcard/201_nt hh closed b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/202_nt hh closed c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/203_closed hat lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/204_closed hat mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/205_closed hat hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/206_closed hat pedal.wav");
    statePointer->wavFilePaths.push_back("/sdcard/207_closed hat foot.wav");
    statePointer->wavFilePaths.push_back("/sdcard/208_hat closed 9x9.wav");
    statePointer->wavFilePaths.push_back("/sdcard/209_hat closed 5x5.wav");
    statePointer->wavFilePaths.push_back("/sdcard/210_hat closed 6x6.wav");
    statePointer->wavFilePaths.push_back("/sdcard/211_hat closed 8x8.wav");
    statePointer->wavFilePaths.push_back("/sdcard/212_closed hat lo b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/213_closed hat hi b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/214_closed hat air.wav");
    statePointer->wavFilePaths.push_back("/sdcard/215_golden hat 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/216_crunch hat 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/217_shine hat 4.wav");
    statePointer->wavFilePaths.push_back("/sdcard/218_nt hh open.wav");
    statePointer->wavFilePaths.push_back("/sdcard/219_nt hh open b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/220_nt hh open c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/221_open hat real.wav");
    statePointer->wavFilePaths.push_back("/sdcard/222_open hat drive.wav");
    statePointer->wavFilePaths.push_back("/sdcard/223_open hat metal.wav");
    statePointer->wavFilePaths.push_back("/sdcard/224_hat open 9x9.wav");
    statePointer->wavFilePaths.push_back("/sdcard/225_hat open 5x5.wav");
    statePointer->wavFilePaths.push_back("/sdcard/226_hat open 6x6.wav");
    statePointer->wavFilePaths.push_back("/sdcard/227_hat open 8x8.wav");
    statePointer->wavFilePaths.push_back("/sdcard/228_open hat lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/229_open hat hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/230_open hat air.wav");
    statePointer->wavFilePaths.push_back("/sdcard/231_girth open hat 3.wav");
    statePointer->wavFilePaths.push_back("/sdcard/232_golden hat 1 b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/233_lucky dbl hat 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/234_brush hat 4.wav");
    statePointer->wavFilePaths.push_back("/sdcard/235_nt ride.wav");
    statePointer->wavFilePaths.push_back("/sdcard/236_nt ride b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/237_ride dark.wav");
    statePointer->wavFilePaths.push_back("/sdcard/238_ride brite.wav");
    statePointer->wavFilePaths.push_back("/sdcard/239_ride sakata.wav");
    statePointer->wavFilePaths.push_back("/sdcard/240_cymbal mp1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/241_ride 7x7.wav");
    statePointer->wavFilePaths.push_back("/sdcard/242_ride dark b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/243_ride lite.wav");
    statePointer->wavFilePaths.push_back("/sdcard/244_ride bell.wav");
    statePointer->wavFilePaths.push_back("/sdcard/245_ride hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/246_toaster bell.wav");
    statePointer->wavFilePaths.push_back("/sdcard/247_nt ride c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/248_crash.wav");
    statePointer->wavFilePaths.push_back("/sdcard/249_crash 9x9.wav");
    statePointer->wavFilePaths.push_back("/sdcard/250_china 626.wav");
    statePointer->wavFilePaths.push_back("/sdcard/251_splash.wav");
    statePointer->wavFilePaths.push_back("/sdcard/252_crash cym.wav");
    statePointer->wavFilePaths.push_back("/sdcard/253_80 great crash.wav");
    statePointer->wavFilePaths.push_back("/sdcard/300_nt clap.wav");
    statePointer->wavFilePaths.push_back("/sdcard/301_nt clap b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/302_nt clap c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/303_clap real.wav");
    statePointer->wavFilePaths.push_back("/sdcard/304_clap hard.wav");
    statePointer->wavFilePaths.push_back("/sdcard/305_clap snap.wav");
    statePointer->wavFilePaths.push_back("/sdcard/306_clap vinyl.wav");
    statePointer->wavFilePaths.push_back("/sdcard/307_clap trumulator.wav");
    statePointer->wavFilePaths.push_back("/sdcard/308_clap sp disco.wav");
    statePointer->wavFilePaths.push_back("/sdcard/309_clap sp disco b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/310_clap dark.wav");
    statePointer->wavFilePaths.push_back("/sdcard/311_clap lite.wav");
    statePointer->wavFilePaths.push_back("/sdcard/312_clap air.wav");
    statePointer->wavFilePaths.push_back("/sdcard/313_clap noise.wav");
    statePointer->wavFilePaths.push_back("/sdcard/314_gospel clap 3.wav");
    statePointer->wavFilePaths.push_back("/sdcard/315_homecoming clap.wav");
    statePointer->wavFilePaths.push_back("/sdcard/316_detroit clap 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/317_nt tambo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/318_nt tambo b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/319_nt tambo c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/320_hand drum hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/321_tamb.wav");
    statePointer->wavFilePaths.push_back("/sdcard/322_hand drum lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/323_bongo mid 4.wav");
    statePointer->wavFilePaths.push_back("/sdcard/324_conga mid 11.wav");
    statePointer->wavFilePaths.push_back("/sdcard/325_agogo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/326_bongo hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/327_maraca.wav");
    statePointer->wavFilePaths.push_back("/sdcard/328_bongo lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/329_guiro.wav");
    statePointer->wavFilePaths.push_back("/sdcard/330_clave.wav");
    statePointer->wavFilePaths.push_back("/sdcard/331_conga lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/332_conga hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/333_cabase lm1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/334_cabasa mp3k.wav");
    statePointer->wavFilePaths.push_back("/sdcard/335_cabasa lnndrum.wav");
    statePointer->wavFilePaths.push_back("/sdcard/336_shaker dmx.wav");
    statePointer->wavFilePaths.push_back("/sdcard/337_tamb b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/338_clave b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/339_cowbell.wav");
    statePointer->wavFilePaths.push_back("/sdcard/340_shaker.wav");
    statePointer->wavFilePaths.push_back("/sdcard/341_hermes hihat 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/342_loww clave 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/343_nt perc.wav");
    statePointer->wavFilePaths.push_back("/sdcard/344_nt perc b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/345_nt perc c.wav");
    statePointer->wavFilePaths.push_back("/sdcard/346_tom fat.wav");
    statePointer->wavFilePaths.push_back("/sdcard/347_tom skin.wav");
    statePointer->wavFilePaths.push_back("/sdcard/348_tom round lo.wav");
    statePointer->wavFilePaths.push_back("/sdcard/349_tom round hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/350_tom sp.wav");
    statePointer->wavFilePaths.push_back("/sdcard/351_tom lnndrum.wav");
    statePointer->wavFilePaths.push_back("/sdcard/352_tom lo flam.wav");
    statePointer->wavFilePaths.push_back("/sdcard/353_tom mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/354_tom hi.wav");
    statePointer->wavFilePaths.push_back("/sdcard/355_sweet shake 1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/356_function tom 3.wav");
    statePointer->wavFilePaths.push_back("/sdcard/357_hundobell perc 2.wav");
    statePointer->wavFilePaths.push_back("/sdcard/400_nt bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/401_s95x round.wav");
    statePointer->wavFilePaths.push_back("/sdcard/402_tubro bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/403_e bass round.wav");
    statePointer->wavFilePaths.push_back("/sdcard/404_basic.wav");
    statePointer->wavFilePaths.push_back("/sdcard/405_mp3k sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/406_syn a re.wav");
    statePointer->wavFilePaths.push_back("/sdcard/407_upright sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/408_e bass pick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/409_thmp.wav");
    statePointer->wavFilePaths.push_back("/sdcard/410_cat envelope.wav");
    statePointer->wavFilePaths.push_back("/sdcard/411_tb3x3 punch.wav");
    statePointer->wavFilePaths.push_back("/sdcard/412_uggbass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/413_e bass pick b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/414_perfect bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/415_prodigy sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/416_s95x sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/417_bass thub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/418_rockelbass mid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/419_e bass dist.wav");
    statePointer->wavFilePaths.push_back("/sdcard/420_ob sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/421_cx tone.wav");
    statePointer->wavFilePaths.push_back("/sdcard/422_buzz bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/423_rockelbass short.wav");
    statePointer->wavFilePaths.push_back("/sdcard/424_mno evo filter.wav");
    statePointer->wavFilePaths.push_back("/sdcard/425_synth 4th hit.wav");
    statePointer->wavFilePaths.push_back("/sdcard/426_thirty seven sub.wav");
    statePointer->wavFilePaths.push_back("/sdcard/427_orga.wav");
    statePointer->wavFilePaths.push_back("/sdcard/428_mud.wav");
    statePointer->wavFilePaths.push_back("/sdcard/429_reese.wav");
    statePointer->wavFilePaths.push_back("/sdcard/430_p.six simple.wav");
    statePointer->wavFilePaths.push_back("/sdcard/431_s6i2 electric.wav");
    statePointer->wavFilePaths.push_back("/sdcard/432_akubass midlong.wav");
    statePointer->wavFilePaths.push_back("/sdcard/433_mud b.wav");
    statePointer->wavFilePaths.push_back("/sdcard/434_wet bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/435_cs5 square.wav");
    statePointer->wavFilePaths.push_back("/sdcard/436_s6i2 warm.wav");
    statePointer->wavFilePaths.push_back("/sdcard/437_elec bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/438_boing.wav");
    statePointer->wavFilePaths.push_back("/sdcard/439_organ dx house.wav");
    statePointer->wavFilePaths.push_back("/sdcard/440_revy.wav");
    statePointer->wavFilePaths.push_back("/sdcard/441_sdsv tom.wav");
    statePointer->wavFilePaths.push_back("/sdcard/442_dr smpl warm.wav");
    statePointer->wavFilePaths.push_back("/sdcard/443_growly.wav");
    statePointer->wavFilePaths.push_back("/sdcard/444_organish.wav");
    statePointer->wavFilePaths.push_back("/sdcard/445_two six 00 smoot.wav");
    statePointer->wavFilePaths.push_back("/sdcard/446_prodigy punch.wav");
    statePointer->wavFilePaths.push_back("/sdcard/447_flate.wav");
    statePointer->wavFilePaths.push_back("/sdcard/448_kaa wa bell air.wav");
    statePointer->wavFilePaths.push_back("/sdcard/449_so round.wav");
    statePointer->wavFilePaths.push_back("/sdcard/450_de-ex solid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/451_chicago.wav");
    statePointer->wavFilePaths.push_back("/sdcard/452_subd.wav");
    statePointer->wavFilePaths.push_back("/sdcard/453_phonec grr.wav");
    statePointer->wavFilePaths.push_back("/sdcard/454_overdrive.wav");
    statePointer->wavFilePaths.push_back("/sdcard/455_prodigy square.wav");
    statePointer->wavFilePaths.push_back("/sdcard/456_tock.wav");
    statePointer->wavFilePaths.push_back("/sdcard/457_wlobb wobb.wav");
    statePointer->wavFilePaths.push_back("/sdcard/458_puff.wav");
    statePointer->wavFilePaths.push_back("/sdcard/459_rude twang.wav");
    statePointer->wavFilePaths.push_back("/sdcard/500_blue.wav");
    statePointer->wavFilePaths.push_back("/sdcard/501_piano s95x.wav");
    statePointer->wavFilePaths.push_back("/sdcard/502_wurli clean.wav");
    statePointer->wavFilePaths.push_back("/sdcard/503_mute strato.wav");
    statePointer->wavFilePaths.push_back("/sdcard/504_cute emu flute.wav");
    statePointer->wavFilePaths.push_back("/sdcard/505_ultra.wav");
    statePointer->wavFilePaths.push_back("/sdcard/506_squick.wav");
    statePointer->wavFilePaths.push_back("/sdcard/507_clav 360 phaser.wav");
    statePointer->wavFilePaths.push_back("/sdcard/508_baby chord.wav");
    statePointer->wavFilePaths.push_back("/sdcard/509_bg vocal.wav");
    statePointer->wavFilePaths.push_back("/sdcard/510_skyline string.wav");
    statePointer->wavFilePaths.push_back("/sdcard/511_octave stab.wav");
    statePointer->wavFilePaths.push_back("/sdcard/512_trumpet breezy.wav");
    statePointer->wavFilePaths.push_back("/sdcard/513_scary vibes.wav");
    statePointer->wavFilePaths.push_back("/sdcard/514_sly synth chord.wav");
    statePointer->wavFilePaths.push_back("/sdcard/515_epiano 360.wav");
    statePointer->wavFilePaths.push_back("/sdcard/516_organ dx verb.wav");
    statePointer->wavFilePaths.push_back("/sdcard/517_pluck hi bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/518_pluck hybrid.wav");
    statePointer->wavFilePaths.push_back("/sdcard/519_prophet piano.wav");
    statePointer->wavFilePaths.push_back("/sdcard/520_epiano 360 bass.wav");
    statePointer->wavFilePaths.push_back("/sdcard/521_houseorgan de-ex.wav");
    statePointer->wavFilePaths.push_back("/sdcard/522_soothe string.wav");
    statePointer->wavFilePaths.push_back("/sdcard/523_cello pluckz.wav");
    statePointer->wavFilePaths.push_back("/sdcard/524_high pan.wav");
    statePointer->wavFilePaths.push_back("/sdcard/525_synth micro funk.wav");
    statePointer->wavFilePaths.push_back("/sdcard/526_dr organ chord.wav");
    statePointer->wavFilePaths.push_back("/sdcard/527_leadaft.wav");
    statePointer->wavFilePaths.push_back("/sdcard/528_soothe strong.wav");
    statePointer->wavFilePaths.push_back("/sdcard/529_simple trombone.wav");
    statePointer->wavFilePaths.push_back("/sdcard/530_cello 360.wav");
    statePointer->wavFilePaths.push_back("/sdcard/531_flute 360 filter.wav");
    statePointer->wavFilePaths.push_back("/sdcard/532_organ dx200 stab.wav");
    statePointer->wavFilePaths.push_back("/sdcard/533_waterchord am7.wav");
    statePointer->wavFilePaths.push_back("/sdcard/534_sad guitar.wav");
    statePointer->wavFilePaths.push_back("/sdcard/535_horn 360 english.wav");
    statePointer->wavFilePaths.push_back("/sdcard/536_strings 360.wav");
    statePointer->wavFilePaths.push_back("/sdcard/537_house chord em7.wav");
    statePointer->wavFilePaths.push_back("/sdcard/538_upright chord.wav");
    statePointer->wavFilePaths.push_back("/sdcard/539_exotic pluck.wav");
    statePointer->wavFilePaths.push_back("/sdcard/540_sky lead.wav");
    statePointer->wavFilePaths.push_back("/sdcard/541_strings dr.wav");
    statePointer->wavFilePaths.push_back("/sdcard/542_liquid chord em7.wav");
    statePointer->wavFilePaths.push_back("/sdcard/543_chordy f9sus4.wav");
    statePointer->wavFilePaths.push_back("/sdcard/544_broken bell perc.wav");
    statePointer->wavFilePaths.push_back("/sdcard/545_pling chord.wav");
    statePointer->wavFilePaths.push_back("/sdcard/546_s6i2 verb hit.wav");
    statePointer->wavFilePaths.push_back("/sdcard/547_dr.wav");
    statePointer->wavFilePaths.push_back("/sdcard/548_vox s6i2 dark.wav");
    statePointer->wavFilePaths.push_back("/sdcard/549_sitar one shot.wav");
    statePointer->wavFilePaths.push_back("/sdcard/550_look organ.wav");
    statePointer->wavFilePaths.push_back("/sdcard/551_thirty 7 stab.wav");
    statePointer->wavFilePaths.push_back("/sdcard/552_buzz delay.wav");
    statePointer->wavFilePaths.push_back("/sdcard/553_vox s6i2 eh.wav");
    statePointer->wavFilePaths.push_back("/sdcard/554_guzheng.wav");
    statePointer->wavFilePaths.push_back("/sdcard/555_nt chordy.wav");
    statePointer->wavFilePaths.push_back("/sdcard/556_fx birds tape.wav");
    statePointer->wavFilePaths.push_back("/sdcard/557_vox s6i2 gated.wav");
    statePointer->wavFilePaths.push_back("/sdcard/558_03d.wav");
    statePointer->wavFilePaths.push_back("/sdcard/559_search chord.wav");

    statePointer->wavFilePaths.push_back("/sdcard/toxic-1.wav");
    statePointer->wavFilePaths.push_back("/sdcard/toxic-2.wav");

    std::string jsonString;
    bool readJsonFileRet = readJsonFile("/sdcard/songs.json", &jsonString);
    if (!readJsonFileRet)
    {
        printf("Failed to readJsonFile \"/sdcard/songs.json\"\n");
        return NULL;
    }
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");
    // printf("%s\n", jsonString.c_str());
    // printf("-\n");
    // printf("-\n");
    // printf("-\n");

    // Parse JSON
    cJSON *root = cJSON_Parse(jsonString.c_str());
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            printf("Error before: %s\n", error_ptr);
        }
        printf("Failed to parse JSON\n");
        // free(jsonBuffer);
        return NULL;
    }

    // Init state
    statePointer->masterGain = 1.0;

    statePointer->currentModeIndex = 0;
    statePointer->currentSelectedStepIndex = 0;
    statePointer->currentSongIndex = 0;

    // TODO : pushback error handling;
    cJSON *songArrayItem = cJSON_GetArrayItem(root, statePointer->currentSongIndex);

    cJSON *songName = cJSON_GetObjectItemCaseSensitive(songArrayItem, "songName");
    statePointer->songName = songName->valuestring;
    cJSON *songTempo = cJSON_GetObjectItemCaseSensitive(songArrayItem, "songTempo");
    statePointer->songTempo = songTempo->valueint;

    // Instruments
    cJSON *songInstruments = cJSON_GetObjectItemCaseSensitive(songArrayItem, "instruments");

    // int arraySize = cJSON_GetArraySize(root);
    // for (int i = 0; i < arraySize; i++)
    // TODO : cJsonForEach
    for (int i = 0; i < 10; i++)
    {
        // TODO : Error handling for loadInstrument and the rest
        Instrument instrument;
        cJSON *songInstrument = cJSON_GetArrayItem(songInstruments, i);
        cJSON *songInstrumentSample = cJSON_GetObjectItemCaseSensitive(songInstrument, "sample");
        cJSON *songInstrumentFilePath = cJSON_GetObjectItemCaseSensitive(songInstrumentSample, "filePath");
        cJSON *songInstrumentIsMono = cJSON_GetObjectItemCaseSensitive(songInstrumentSample, "isMono");
        cJSON *songInstrumentVolume = cJSON_GetObjectItemCaseSensitive(songInstrument, "volume");
        cJSON *songInstrumentPitch = cJSON_GetObjectItemCaseSensitive(songInstrument, "pitch");
        cJSON *songInstrumentStartPosition = cJSON_GetObjectItemCaseSensitive(songInstrument, "startPosition");
        cJSON *songInstrumentEndPosition = cJSON_GetObjectItemCaseSensitive(songInstrument, "endPosition");
        cJSON *songInstrumentIsReverse = cJSON_GetObjectItemCaseSensitive(songInstrument, "isReverse");
        loadInstrument(songInstrumentFilePath->valuestring, songInstrumentIsMono->valueint, (float)songInstrumentVolume->valuedouble, songInstrumentPitch->valueint, (float)songInstrumentStartPosition->valuedouble, (float)songInstrumentEndPosition->valuedouble, (bool)songInstrumentIsReverse->valueint, &instrument);
        statePointer->instruments.push_back(instrument);
    }

    statePointer->currentStaveIndex = 0;
    statePointer->currentOctaveIndex = 0;

    statePointer->currentPartIndex = 0;
    statePointer->currentPartInstrumentIndex = 0;
    // parts
    statePointer->parts = {};

    statePointer->currentStepIndex = 0;

    cJSON *songParts = cJSON_GetObjectItemCaseSensitive(songArrayItem, "parts");
    // cJSON *songInstrument = cJSON_GetArrayItem(songInstruments, i);
    int songPartsLength = cJSON_GetArraySize(songParts);
    // TODO : cJsonForEach
    for (int i = 0; i < songPartsLength; i++)
    {
        cJSON *songPart = cJSON_GetArrayItem(songParts, i);
        
        cJSON *songPartStaves = cJSON_GetObjectItemCaseSensitive(songPart, "staves");
        const int partStaves = songPartStaves->valueint;
        cJSON *songPartSteps = cJSON_GetObjectItemCaseSensitive(songPart, "steps");
        int songPartStepsLength = cJSON_GetArraySize(songPartSteps);
        std::vector<std::vector<Step>> partSteps = {};
        for (int j = 0; j < STATE_PART_STEPS_LENGTH * partStaves; j++)
        // for (int j = 0; j < songPartStepsLength; j++)
        {
            cJSON *songPartStep = cJSON_GetArrayItem(songPartSteps, j);
            int songPartStepContentLength = cJSON_GetArraySize(songPartStep);
            std::vector<Step> partStepContent = {};

            // printf("songPartStepContentLength : %i\n", songPartStepContentLength);
            for (int k = 0; k < songPartStepContentLength; k++)
            {
                cJSON *songPartStepContent = cJSON_GetArrayItem(songPartStep, k);
                cJSON *songPartStepContentInstrumentIndex = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "instrumentIndex");
                cJSON *songPartStepContentVolume = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "volume");
                cJSON *songPartStepContentPitch = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "pitch");
                cJSON *songPartStepContentStartPosition = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "startPosition");
                cJSON *songPartStepContentEndPosition = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "endPosition");
                cJSON *songPartStepContentIsReverse = cJSON_GetObjectItemCaseSensitive(songPartStepContent, "isReverse");
                partStepContent.push_back({songPartStepContentInstrumentIndex->valueint, (float)songPartStepContentVolume->valuedouble, songPartStepContentPitch->valueint, (float)songPartStepContentStartPosition->valuedouble, (float)songPartStepContentEndPosition->valuedouble, (bool)songPartStepContentIsReverse->valueint});
            }

            partSteps.push_back(partStepContent);
        }

        Part part = {partStaves, partSteps};
        statePointer->parts.push_back(part);
    }

    statePointer->isPlaying = false;

    // TODO : Remove
    statePointer->isBlbl = false;

    return true;
}
