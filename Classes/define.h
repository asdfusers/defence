#pragma once
//
//  Defines.h
//  DokdoDefenceCommand
//
//  Created by Jin-Ryul Kim on 13. 9. 22..
//
//

#ifndef DokdoDefenceCommand_Defines_h
#define DokdoDefenceCommand_Defines_h

// Z-Orders

#define ZORDER_SEA                              0   // 바다 배경
#define ZORDER_SPRITE_BATCH_NODE_SURFACE        1
#define ZORDER_SPRITE_BATCH_NODE_CLOUD          2
#define ZORDER_SPRITE_BATCH_NODE_EXPLOSION_S    3
#define ZORDER_SPRITE_BATCH_NODE_AIR            4
#define ZORDER_SPRITE_BATCH_NODE_EXPLOSION_A    5
#define ZORDER_SPRITE_BATCH_NODE_INTERFACE      6

// Surface Z-Order
#define ZORDER_RADAR        1   // 레이더
#define ZORDER_SUBMARINE    2   // 잠수함
#define ZORDER_SHIPTAIL     3   // 배의 물결
#define ZORDER_SHIP         4   // 배
#define ZORDER_DOKDO        5   // 독도
#define ZORDER_SHADOW       6   // 항공기 그림자
#define ZORDER_WATERCOLUMN  7   // 물기둥

// Air Z-Order
#define ZORDER_BULLET           1   // 총탄
#define ZORDER_LYNX             2  // 링스헬기
#define ZORDER_HELICOPTER       3  // 공격헬기
#define ZORDER_CLOUD            4  // 구름 그림자
#define ZORDER_BOMB             5  // 폭탄
#define ZORDER_BOMBER           6  // 폭격기
#define ZORDER_FIGHTER          7  // 전투기
#define ZORDER_STRATEGYBOMBER   8  // 전략폭격기
#define ZORDER_MISSILE          9  // 미사일
#define ZORDER_PEACEEYE         10  // 피스아이 조기 경보기
#define ZORDER_NUCLEAR          11  // 핵폭탄

//Title Z-Order
#define ZORDER_BK               1
#define ZORDER_MENU             2
#define ZORDER_BTN              3
#define ZORDER_DIALOG           4
#define ZORDER_TITLE            4

// Tags
#define TAG_SPRITE_BATCH_NODE_SURFACE       1000
#define TAG_SPRITE_BATCH_NODE_CLOUD         1001
#define TAG_SPRITE_BATCH_NODE_EXPLOSION_S   1002
#define TAG_SPRITE_BATCH_NODE_AIR           1003
#define TAG_SPRITE_BATCH_NODE_EXPLOSION_A   1004
#define TAG_SPRITE_BATCH_NODE_INTERFACE     1005
#define TAG_SPRITE_BATCH_NODE_MENU          1006

#define TAG_BULLET                          2000
#define TAG_NUCLEAR                         2001

#define TAG_INTERFACE_CANNON                3000
#define TAG_INTERFACE_VULCAN                3001
#define TAG_INTERFACE_MISSILE               3002
#define TAG_INTERFACE_ACTIVATE              3003
#define TAG_INTERFACE_SCOREBOARD            3004

#define TAG_WEAPON_E                        3100
#define TAG_WEAPON_A                        3101
#define TAG_UNIT_E                          3102
#define TAG_UNIT_A                          3103

#define TAG_MENU_START                      4000
#define TAG_MENU_SCORE                      4001
#define TAG_MENU_OPTION                     4002
#define TAG_MENU_FACEBOOK                   4003
#define TAG_MENU_TWITTER                    4004

// 유닛의 체력
#define HP_PATROL           10
#define HP_DESTROYER        20
#define HP_CRUISER          40
#define HP_CARRIER          80
#define HP_HELICOPTER       5
#define HP_FIGHTER          15
#define HP_BOMBER           30
#define HP_STRATEGYBOMBER   60

// 적 유닛 생성 시간
#define ADD_PATROL_TERM         4
#define ADD_DESTROYER_TERM      7
#define ADD_CRUISER_TERM        13
#define ADD_CARRIER_TERM        17

#define ADD_HELICOPTER_TERM     5
#define ADD_FIGHTER_TERM        9
#define ADD_BOMBER_TERM         14
#define ADD_STRATEGYBOMBER_TERM 21

// 구름 생성 시간
#define ADD_CLOUD_TERM          1.5

// 적 유닛 이동 시간
#define PATROL_MOVE_TIME        14
#define DESTROYER_MOVE_TIME     18
#define CRUISER_MOVE_TIME       24
#define CARRIER_MOVE_TIME       33

#define HELICOPTER_MOVE_TIME    12
#define FIGHTER_MOVE_TIME       9
#define BOMBER_MOVE_TIME        14
#define STRATEGYBOMBER_MOVE_TIME    20

// 독도 무기 쿨 타임
#define DOKDO_VULCAN_COOLTIME       0.2f
#define DOKDO_CANNON_COOLTIME       0.8f
#define DOKDO_MISSILE_COOLTIME      1.8f

// 독도 무기 위력
#define DOKDO_VULCAN_DAMAGE         1
#define DOKDO_CANNON_DAMAGE         8
#define DOKDO_MISSILE_DAMAGE        20

// 무기 이동시간
#define VULCAN_MOVE_TIME        1.5
#define CANNON_MOVE_TIME        2.5
#define MISSILE_MOVE_TIME       4

#define TAG_MENU_START 4000
#define TAG_MENU_SCORE 4001
#define TAG_MENU_OPTION 4002
#define TAG_MENU_LOGIN 4003
#define TAG_OPTION_DLG 4004

#define TAG_SEND_BTN 8000
#define TAG_OPTION_BTN 8001
#define TAG_STORE_BTN 8002
#define TAG_START_BTN 8003
#define TAG_SCORE_BTN 8004

#define CANNON_PRICE 100
#define MISSILE_PRICE 150
enum Ships
{

	DESTROYER = 1,
	CRUISER = 2,
	Bomber = 3,
	FIGHTER = 4,
	STRATEGYBOMBER = 5,

};
#endif
