#ifndef __BOT_CVARS_H__
#define __BOT_CVARS_H__

extern ConVar rcbot_tf2_debug_spies_cloakdisguise;
extern ConVar rcbot_tf2_medic_letgotime;
extern ConVar rcbot_const_point_master_offset;
extern ConVar rcbot_tf2_pyro_airblast;
extern ConVar rcbot_projectile_tweak;
extern ConVar bot_cmd_enable_wpt_sounds;
extern ConVar bot_general_difficulty;
extern ConVar bot_visrevs_clients;
extern ConVar bot_spyknifefov;
extern ConVar bot_visrevs;
extern ConVar bot_pathrevs;
extern ConVar bot_command;
extern ConVar bot_attack;
extern ConVar bot_scoutdj;
extern ConVar bot_anglespeed;
extern ConVar bot_stop;
extern ConVar bot_waypointpathdist;
extern ConVar bot_rj;
extern ConVar bot_defrate;
extern ConVar bot_beliefmulti;
extern ConVar bot_belief_fade;
extern ConVar bot_change_class;
extern ConVar bot_use_vc_commands;
extern ConVar bot_use_disp_dist;
extern ConVar bot_max_cc_time;
extern ConVar bot_min_cc_time;
extern ConVar bot_avoid_radius;
extern ConVar bot_avoid_strength;
extern ConVar bot_messaround;
extern ConVar bot_heavyaimoffset;
extern ConVar bot_aimsmoothing;
extern ConVar bot_bossattackfactor;
extern ConVar rcbot_enemyshootfov;
extern ConVar rcbot_enemyshoot_gravgun_fov;
extern ConVar rcbot_wptplace_width;
extern ConVar rcbot_wpt_autoradius;
extern ConVar rcbot_wpt_autotype;
extern ConVar rcbot_move_sentry_time;
extern ConVar rcbot_move_sentry_kpm;
extern ConVar rcbot_smoke_time;
extern ConVar rcbot_move_disp_time;
extern ConVar rcbot_move_disp_healamount;
extern ConVar rcbot_demo_runup_dist;
extern ConVar rcbot_demo_jump;
extern ConVar rcbot_move_tele_time;
extern ConVar rcbot_move_tele_tpm;
extern ConVar rcbot_tf2_protect_cap_time;
extern ConVar rcbot_tf2_protect_cap_percent;
extern ConVar rcbot_tf2_spy_kill_on_cap_dist;
extern ConVar rcbot_move_dist;
extern ConVar rcbot_shoot_breakables;
extern ConVar rcbot_shoot_breakable_dist;
extern ConVar rcbot_shoot_breakable_cos;
extern ConVar rcbot_move_obj;
extern ConVar rcbot_taunt;
extern ConVar rcbot_notarget;
extern ConVar rcbot_nocapturing;
extern ConVar rcbot_jump_obst_dist;
extern ConVar rcbot_jump_obst_speed;
extern ConVar rcbot_speed_boost;
extern ConVar rcbot_melee_only;
extern ConVar rcbot_debug_iglev;
extern ConVar rcbot_dont_move;
extern ConVar rcbot_runplayercmd_dods;
extern ConVar rcbot_ladder_offs;
extern ConVar rcbot_ffa;
extern ConVar rcbot_prone_enemy_only;
extern ConVar rcbot_menu_update_time1;
extern ConVar rcbot_menu_update_time2;
extern ConVar rcbot_autowaypoint_dist;
extern ConVar rcbot_stats_inrange_dist;
extern ConVar rcbot_squad_idle_time;
extern ConVar rcbot_bots_form_squads;
extern ConVar rcbot_listen_dist;
extern ConVar rcbot_footstep_speed;
extern ConVar rcbot_bot_squads_percent;
extern ConVar rcbot_tooltips;
extern ConVar rcbot_debug_notasks;
extern ConVar rcbot_debug_dont_shoot;
extern ConVar rcbot_debug_show_route;
extern ConVar rcbot_tf2_autoupdate_point_time;
extern ConVar rcbot_tf2_payload_dist_retreat;
extern ConVar rcbot_spy_runaway_health;
extern ConVar rcbot_supermode;
extern ConVar rcbot_addbottime;
extern ConVar rcbot_gamerules_offset;
extern ConVar rcbot_datamap_offset;
extern ConVar rcbot_bot_quota_interval;
//extern ConVar rcbot_const_point_master_offset;
extern ConVar rcbot_util_learning;

//Synergy Cvars
extern ConVar rcbot_runplayercmd_syn;
extern ConVar rcbot_syn_use_search_range;

/** Additional convars by pongo1231 **/
extern ConVar rcbot_show_welcome_msg;
extern ConVar rcbot_force_class;

#if SOURCE_ENGINE <= SE_DARKMESSIAH
extern ConVar *sv_gravity;
extern ConVar *mp_teamplay;
extern ConVar *sv_tags;
extern ConVar *mp_friendlyfire;
extern ConVar *mp_stalemate_enable;
#else
extern ConVarRef sv_gravity;
extern ConVarRef mp_teamplay;
extern ConVarRef sv_tags;
extern ConVarRef mp_friendlyfire;
extern ConVarRef mp_stalemate_enable;
#endif

void RCBOT2_Cvar_setup (ICvar *cvar);

#endif
