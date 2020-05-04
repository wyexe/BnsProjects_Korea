#ifndef __BNS_GAMESTRUCT_H__
#define __BNS_GAMESTRUCT_H__

#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>


#pragma warning(disable:4100)

/************************************************************************/
/* 基址                                                                     */
/************************************************************************/
#ifdef BnsProject_TaiWan
#define 人物基址 0x13ACF14
#define 人物邀请偏移 0x864
#define 人物瞬移钩挂地址 0x636EB2
#define 人物瞬移偏移 0x3A32
#define 隐藏玩家CALL 0x8E1EA0
#define 任务捡起CALL 0x4211B0
#define 捡NPC任务CALL 0x421780
#define 穿装备CALL 0x423110
#define 脱装备CALL 0x48CBC0
#define 大任务偏移 0x854
#define 当前任务杀怪数量CALL 0x903880
#define 背包返回名字参数偏移1 0x79C
#define 背包返回名字参数偏移2 0x7A0
#define 判断物品能否使用CALL 0x503E60
#define 获取物品类型CALL 0x4A9B80
#define 游戏申请内存CALL 0xE2514E
#define 发包CALL 0x5D93B0
#define 蹲下捡物CALL 0x42B3A0
#define 遁术发包ID 0x3B7
#define 遁术发包内容 0x1072020
#define 遁术CALL 0x5926E0
#define 开背包发包ID 0x410
#define 开背包发包内容 0x10720F8
#define 取任务IDCALL 0x4DE8E0
#define HookTask_Addr 0x52A14C
#define 取支线任务IDCALL 0x4DE820
#define HookBLTask_Addr 0x528EF5
#define HookMonsterSkillAddr2 0x65A290
#define HookTakeItemJudge 0x756C77
#define HookTakeItem_ForBlueAddr1 0x7B58F4
#define HookTakeItem_ForBlueAddr2 0x7B590B
#define 技能遍历偏移 0x858
#define 获取技能参数CALL 0x55BA10
#define 技能名字返回偏移1 0x2798
#define 技能冷却参数基址 0x144342C
#define 技能冷却偏移 0x7C4
#define 技能冷却CALL 0x42CC70
#define 释放技能CALL 0x427CA0
#define 技能加点CALL 0x9B4C10
#define 技能加点发包CALL 0x561530
#define 名字返回基址 0x13AE390
#define 名字返回偏移1 0x3430
#define 名字返回偏移2 0x48
#define 物品使用CALL 0x48DEC0
#define 物品分解CALL 0x48F610
#define 丢弃物品CALL 0x48F220
#define 整理背包CALL 0x4940C0
#define 武器强化CALL 0x492C00
#define 武器进化CALL 0x0
#define 解印CALL 0x490900
#define 开箱子CALL 0x48F610
#define 选择频道CALL 0x508100
#define 小退CALL 0xCEFD10
#define 领取礼包偏移 0x89C
#define 领取礼包CALL 0x588CE0
#define 获取人物状态偏移 0x268
#define 镶嵌宝石偏移 0x210A8
#define 镶嵌宝石CALL 0x7AC980
#define 取宝石CALL 0x48F4F0
#define 捡JB发包ID 0x3CC
#define 捡JB法宝内容 0x1072020
#define 存仓CALL 0x48AFE0
#define 刷新频道CALL 0x507E10
#define 买物品发包填充CALL 0x615E60
#define 卖物品CALL 0x9DC820
#define 根据ID返回OBJ基址 0x13AE390
#define 根据ID返回OBJ偏移1 0x1758
#define 根据ID返回OBJ偏移2 0x48
#define 删除角色CALL  0x5F0A30
#define 创建角色索引CALL  0xD3BFD0
#define 创建角色CALL  0xD3B390
#define PK偏移 0xCFC
#define PK状态偏移 0x48
#define PKCALL  0x5AF5F0
#define 离开PK房间CALL  0x5B0100
#define PK战斗结束CALL  0x5B6A20
#define PK过图偏移 0x9C0
#define PK准备CALL  0x5B68F0
#define 尸体名字返回偏移 0xCC
#define 人物BUFF偏移1 0x73B8
#define 人物BUFF偏移2 0x1608
#define 人物BUFF偏移3 0x214A4
#define 人物BUFF偏移_SELF 0x10
#define 人物BUFF偏移_OTHER 0x2658
#define 人物BUFF_ARRAY_SIZE 0x118
#define 抱尸体发包参数1 0x3D0
#define 抱尸体发包参数2 0x1072020
#define 角色遍历偏移 0x19C
#define 聊天消息框偏移1 0x554
#define 聊天消息框偏移2 0x88
#define 视角基址 0x13D6FC0
#define 获取点卡时间Base 0x12E0698
#define 获取点卡时间CALL 0xA117B0
#define HookDianKaAddr 0xC80CF8
#define 登录CALL 0xA195E0
#define 邮件偏移3 0x0
#define 邮件偏移1 0x0
#define 集体收邮件CALL 0x0
#define 收邮件偏移 0xAF3B7374
#define 设置收邮件CALL 0x0
#define 设置竞拍品质CALL 0x4EA080
#define 队伍遍历偏移 0x864
#define 离开队伍CALL 0x4E98A0
#define 邀请队员CALL 0x4E8B20
#define 一次捡物CALL 0x42B3A0
#define 切换新任务CALL 0x901A30
#define HookMonsterAddr 0x42F10C
#define 动作CALL 0x525580
#define 怪物BUFF偏移_OTHER 0x72E8
#define 系统BUFF偏移_OTHER 0x4CA0
#define 人物邀请交易ID偏移 0x8B0
#define 发起交易CALL 0x5073A0
#define 放置交易物品CALL 0x59F5B0
#define 放置交易物品偏移 0x6D0
#define 放置交易金钱CALL 0x59F3C0
#define 锁定交易框CALL 0x59F760
#define 是否锁定自己的交易框偏移 0xA0
#define 对方是否锁定交易偏移 0x140
#define 交易CALL 0x59F7A0
#define 战斗姿势偏移 0x184
#define 选择角色偏移 0x2A8
#define 取消交易CALL 0x5073F0
#define 接受交易CALL 0x5075D0
#define UI技能遍历偏移1 0x73C4
#define UI技能遍历偏移2 0x6C268
#define 物品右上角分类名字基址 0xF328F0
#define 交易物品数量偏移 0x98
#define 逃脱CALL 0x4F9B80
#define 背包购买物品发包参数 0x1071994
#define 第一颗宝石偏移 0x58
#define 隐藏UI技能偏移 0x64
#define 遁术CALL2  0x423E10
#define 遁术偏移 0x7C4
#define 打开礼箱CALL  0x45C930
#define 已打开礼箱物品偏移 0x94
#define 收礼包CALL  0x45CA60
#define 未打开礼箱物品偏移 0xA0
#define 二次捡物CALL  0x509550
#define 使用气宗劵CALL2  0xD3CFE0
#define 使用气宗劵CALL1  0x45C930
#define 获取劵对象CALL  0x45EA50
#define 队伍邀请队长ID偏移 0x98
#define 踢出队员CALL  0x4E9D80
#define 召唤拿猫发包参数1 0x47B
#define 召唤拿猫发包参数2 0xF8D800
#define 召唤拿猫CALL  0xDD93D0
#define 召唤猫释放技能CALL  0x428792
#define 白仙修练场召唤英雄CALL  0xC38F10
#define 白仙修练场偏移1 0x2678
#define 白仙修练场偏移3 0x540
#define 白仙修练场获取奖励CALL  0x52A860
#define 白仙修练场退出CALL  0x438440
#define 时空之塔转盘CALL  0x438390
#define 时空之塔领取奖励CALL  0x436C70
#define 退出时空之塔CALL  0x438440
#define Hook副本不过图Addr 0x693A86
#define 设置角度发包参数1 0x3F
#define 设置角度发包参数2 0x1071FF0
#define UI技能资源偏移 0xA8
#define 怪物BUFF偏移1 0x14
#define 怪物BUFF偏移2 0x73B8
#define 怪物BUFF偏移3 0x73B8
#define 怪物BUFF偏移4 0x18
#define 怪物BUFF偏移5 0x2658
#define 怪物BUFF偏移6 0x2648
#define 背包遍历偏移 0x850
#define 当前频道偏移 0x80C
#define 走路偏移 0x1B0
#define 背包格子数量偏移 0x180
#define 人物基址偏移1 0x50
#define 人物基址偏移2 0x80
#define 人物基址偏移3 0x48
#define Name偏移 0x8C
#define MAPID偏移 0x90
#define HP偏移 0xB8
#define Point偏移 0x1B8
#define 卖NPC偏移 0x98
#define 怪物遍历_偏移 0x9C
#define 怪物属性偏移 0x7C8
#define 交任务CALL  0x5296E0
#define 判断是否掉线偏移 0x7C
#define 加属性点发包参数1 0x70
#define 加属性点发包参数2 0x1071FF0
#define 切换技能系CALL 0x9B6570
#define BOSS被控制BUFF数量偏移 0x218
#define BOSS是否可控偏移 0x21C
#define 装备突破CALL 0x0
#define 进入副本空间CALL 0x0
#define 进入副本空间偏移 0x74
#define 召唤拿猫偏移1 0x1163
#define 召唤拿猫偏移2 0x1160
#define 召唤拿礼物CALL 0xDA2E30
#define 离开技能训练场CALL 0x56CFE0
#define 技能训练场偏移 0x54C
#define 喊话CALL 0x732B40
#define 输入OTPCALL 0xD3B2C0
#define 邮寄CALL 0x434C20
#define 邮寄参数偏移 0x8D0
#define 收无物品邮件CALL 0x434C20
#define 设置瞬移浮空偏移 0x3709
#define 瞬移次数偏移 0x3520
#define 瞬移距离偏移 0x38F0
#define 瞬移Buffer填充CALL 0x635A80
#define 全局加速偏移1 0x50
#define 全局加速偏移2 0x3C
#define 全局加速偏移3 0x434
#define 怪物隐形偏移 0x10C
#define 神行百变偏移 0x23C
#define 复活发包参数1 0x88
#define 复活发包参数2 0x1071FF0
#define 强制开箱子偏移 0x185
#define 领取星币发包参数1 0x3AD
#define 领取星币发包参数2 0x1071FF0
#define 星币偏移1 0x0
#define 星币偏移2 0x0
#define 正在走路偏移 0x200
#define 星币商店购物CALL 0xC5CE60
#define 捡物状态偏移 0x158
#define 星币数量偏移 0x258
#define 物品颜色偏移 0x88
#define 组队难度CALL 0x4E9650
#define 八卦强化CALL 0x491100
#define 八卦属性偏移 0xE34
#define 八卦属性CALL偏移 0x48
#define 物品八卦ID偏移 0x38
#define 竞拍偏移1 0x640
#define 竞拍偏移2 0x54
#define 创建副本房间发包参数1 0x1071FF0
#define 创建副本房间发包参数2 0x259
#define 寻找房间发包参数1 0x1071DE0
#define 寻找房间发包参数2 0x262
#define 副本房间偏移1 0xCFC
#define 副本房间偏移2 0x14
#define 房间准备CALL 0x5BEAB0
#define 房间是否准备CALL 0x5AFD40
#define 队长是否准备CALL 0x5C3600
#define 队长房间准备偏移 0x95
#define 选择副本CALL 0x5C1BD0
#define 选择副本难度CALL 0x5BE6D0
#define 隐藏UI偏移 0x3C
#define 退出50级UI偏移 0x0
#define 使用50级卷偏移1 0x0
#define 使用50级卷偏移2 0x0
#define 使用50级卷CALL1 0x0
#define 使用50级卷CALL2 0x0
#define 离开洪门修理场CALL 0x0
#define 退出门派CALL 0x46F180
#define 仓库号交易物品数量偏移 0x138
#define 退回跨服组队界面CALL 0x5BDD10
#define 副本房间偏移3 0xD00
#define 退出跨服组队房间CALL 0x5B0170
#define 退出跨服组队界面CALL 0x50CF87
#define 创建角色偏移  0x4C
#define 遍历礼箱偏移 0x5F
#define UI技能名字偏移 0xA8
#define 技能名字偏移1 0x1C
#define 技能名字偏移2 0x18
#define 转盘CALL 0xBF18C0

//钩挂任务
#define HookTask_JmpAddr					HookTask_Addr + 7
#define HookTaskCALL_Addr					HookTask_Addr + 0x11

#define HookBLTask_JmpAddr					HookBLTask_Addr + 7
#define HookBLTaskCALL_Addr					HookBLTask_Addr + 0x21
/* *********************************************************************** /
/* 基本固定的偏移                                                   */
/************************************************************************/
#define 获取当前城市IDCALL					遁术CALL

#define 最大频道偏移						当前频道偏移 + 0x4
#define 人物基址偏移4						人物基址偏移3



#define 等级偏移										MAPID偏移 + 0xC
#define 人物属性点偏移								0x5C
#define Money偏移									0xC0//c8
#define 阵营偏移										0xD0//d8
#define MP偏移										0xF0//f8
#define 怪物目标ID									0x108//110

#define 人物读条偏移									0x178//啊三
#define 状态偏移										0x181//?
#define 倒地偏移										0x188
#define PK豆偏移										0x1B8
#define 人物过图偏移									0x1E4//没用
#define MAXHP偏移1								0x218 //0x218
#define MAXHP偏移2								0x220 //0x220
#define TargetId偏移									0x228//没用
#define 过门偏移										0x2E4

#define Ui视觉偏移									0x24CD0



#define 一次捡物偏移									技能冷却偏移
#define Hook瞬移地址									人物瞬移钩挂地址

//8B038B90????00008BCBFFD2D945??51 + 0x11
#define 人物瞬移偏移2								0x2B8//2B8
#define	视角偏移										0x62
#define 怪物状态偏移									倒地偏移
#define 是否使用技能中偏移							0x34
#define 技能读条偏移									0x30
#define 白仙修练场偏移4								白仙修练场偏移2 + 0x4
#define 白仙修练场偏移2								白仙修练场偏移1 + 0x4
#define 白仙修练场进度偏移							0x3C
#define 获取等级劵数量偏移							0x1FC
#define 星币商店购物偏移							0x2C
#define 人物坐标偏移1						0x3BC
#define 人物坐标偏移2						0x40
#define 人物坐标偏移3						0x214
#define 人物坐标偏移4						0x220
#define 人物坐标偏移5						0x214
//怪物类型的偏移
#define 周围遍历分类偏移						0xE

// 暂时搁置的CALL
#define 修改视觉CALL									0x0
#define 重置视角CALL									0x0
#define 收邮件CALL									0x0
#define 邮件偏移2									0x64
#define 竞拍物品Index偏移							0x70
#define 任务尸体是否可捡偏移	              0x20
#define 人物BUFF偏移5                         0x10
#define  存入衣柜CALL 0x0
#endif //BnsProject_TaiWan

#ifdef BnsProject_Korea
#define 人物基址 0x150F344
#define 人物邀请偏移 0x8EC
#define 人物瞬移钩挂地址 0x651CA9
#define 人物瞬移偏移 0x3A32
#define 隐藏玩家CALL 0x95ED90
#define 任务捡起CALL 0x4221F0
#define 捡NPC任务CALL 0x4227C0
#define 穿装备CALL 0x424150
#define 脱装备CALL 0x495F40
#define 大任务偏移 0x8DC
#define 当前任务杀怪数量CALL 0x982640
#define 背包返回名字参数偏移1 0x79C
#define 背包返回名字参数偏移2 0x7A0
#define 判断物品能否使用CALL 0x513AA0
#define 获取物品类型CALL 0x4B5550
#define 游戏申请内存CALL 0xF2C51E
#define 发包CALL 0x5EF1D0
#define 蹲下捡物CALL 0x42CB60
#define 遁术发包ID 0x3CE
#define 遁术发包内容 0x10AD86C
#define 遁术CALL 0x5A7370
#define 开背包发包ID 0x42E
#define 开背包发包内容 0x11991E8
#define 取任务IDCALL 0x4EA400
#define HookTask_Addr 0x53AADC
#define 取支线任务IDCALL 0x4EA340
#define HookBLTask_Addr 0x539885
#define HookMonsterSkillAddr2 0x676710
#define HookTakeItemJudge 0x78AE1A
#define HookTakeItem_ForBlueAddr1 0x9984CC
#define HookTakeItem_ForBlueAddr2 0x9984E3
#define 技能遍历偏移 0x8E0
#define 获取技能参数CALL 0x56DF40
#define 技能名字返回偏移1 0x296C
#define 技能冷却参数基址 0x14CEE28
#define 技能冷却偏移 0x844
#define 技能冷却CALL 0x42E480
#define 释放技能CALL 0x428E60
#define 技能加点CALL 0xA3DFC0
#define 技能加点发包CALL 0x573E80
#define 名字返回基址 0x15107E0
#define 名字返回偏移1 0x36A0
#define 名字返回偏移2 0x64
#define 物品使用CALL 0x497240
#define 物品分解CALL 0x498B90
#define 丢弃物品CALL 0x498680
#define 整理背包CALL 0x49D750
#define 武器强化CALL 0x49C1F0
#define 武器进化CALL 0x4B8670
#define 解印CALL 0x0
#define 开箱子CALL 0x498B90
#define 选择频道CALL 0x518200
#define 小退CALL 0xDC9450
#define 领取礼包偏移 0x924
#define 领取礼包CALL 0x59D800
#define 获取人物状态偏移 0x26C
#define 镶嵌宝石偏移 0x2BD48
#define 镶嵌宝石CALL 0x7E6A90
#define 取宝石CALL 0x498A50
#define 捡JB发包ID 0x3E6
#define 捡JB法宝内容 0x10AD86C
#define 存仓CALL 0x4942D0
#define 刷新频道CALL 0x517F10
#define 买物品发包填充CALL 0x62F530
#define 卖物品CALL 0xA70440
#define 根据ID返回OBJ基址 0x15107E0
#define 根据ID返回OBJ偏移1 0x1828
#define 根据ID返回OBJ偏移2 0x64
#define 删除角色CALL  0x607810
#define 创建角色索引CALL  0xE20AF0
#define 创建角色CALL  0xE1FDB0
#define PK偏移 0xD74
#define PK状态偏移 0x48
#define PKCALL  0x5C4F80
#define 离开PK房间CALL  0x5C5A90
#define PK战斗结束CALL  0x5CCC10
#define PK过图偏移 0xA50
#define PK准备CALL  0x0
#define 尸体名字返回偏移 0xD4
#define 人物BUFF偏移1 0xD628
#define 人物BUFF偏移2 0x18A8
#define 人物BUFF偏移3 0x214F8
#define 人物BUFF偏移_SELF 0x10
#define 人物BUFF偏移_OTHER 0x2658
#define 人物BUFF_ARRAY_SIZE 0x118
#define 抱尸体发包参数1 0x3EA
#define 抱尸体发包参数2 0x10AD86C
#define 角色遍历偏移 0x19C
#define 聊天消息框偏移1 0x578
#define 聊天消息框偏移2 0x88
#define 视角基址 0x153CA8C
#define 获取点卡时间Base 0x1438A60
#define 获取点卡时间CALL 0xA9CF90
#define HookDianKaAddr 0xD54448
#define 登录CALL 0xAA5250
#define 邮件偏移3 0x3960
#define 邮件偏移1 0x39D8
#define 集体收邮件CALL 0xB0F420
#define 收邮件偏移 0x6284
#define 设置收邮件CALL 0xB0A6F0
#define 设置竞拍品质CALL 0x4F88A0
#define 队伍遍历偏移 0x8EC
#define 离开队伍CALL 0x4F80D0
#define 邀请队员CALL 0x4F7350
#define 一次捡物CALL 0x42CB60
#define 切换新任务CALL 0x9807C0
#define HookMonsterAddr 0x4309FA
#define 动作CALL 0x535F50
#define 怪物BUFF偏移_OTHER 0x72E8
#define 系统BUFF偏移_OTHER 0x4CA0
#define 人物邀请交易ID偏移 0x938
#define 发起交易CALL 0x5174A0
#define 放置交易物品CALL 0x5B4560
#define 放置交易物品偏移 0x6F0
#define 放置交易金钱CALL 0x5B4370
#define 锁定交易框CALL 0x5B4710
#define 是否锁定自己的交易框偏移 0xA0
#define 对方是否锁定交易偏移 0x140
#define 交易CALL 0x5B4750
#define 战斗姿势偏移 0x19C
#define 选择角色偏移 0x2C8
#define 取消交易CALL 0x5174F0
#define 接受交易CALL 0x5176D0
#define UI技能遍历偏移1 0xD634
#define UI技能遍历偏移2 0x71AB0
#define 物品右上角分类名字基址 0x1044E38
#define 交易物品数量偏移 0x98
#define 逃脱CALL 0x509910
#define 背包购买物品发包参数 0x1198AEC
#define 第一颗宝石偏移 0x54
#define 隐藏UI技能偏移 0x64
#define 遁术CALL2  0x424E50
#define 遁术偏移 0x844
#define 打开礼箱CALL  0x462460
#define 已打开礼箱物品偏移 0xA0
#define 收礼包CALL  0x462590
#define 未打开礼箱物品偏移 0xA4
#define 二次捡物CALL  0x5197A0
#define 使用气宗劵CALL2  0xE21AE0
#define 使用气宗劵CALL1  0x462460
#define 获取劵对象CALL  0x466000
#define 队伍邀请队长ID偏移 0xB8
#define 踢出队员CALL  0x4F85A0
#define 召唤拿猫发包参数1 0x496
#define 召唤拿猫发包参数2 0x10A5A64
#define 召唤拿猫CALL  0xEDE680
#define 召唤猫释放技能CALL  0x4299F0
#define 白仙修练场召唤英雄CALL  0x0
#define 白仙修练场偏移1 0x0
#define 白仙修练场偏移3 0x0
#define 白仙修练场获取奖励CALL  0x53B240
#define 白仙修练场退出CALL  0x43BA90
#define 时空之塔转盘CALL  0x43A4B0
#define 时空之塔领取奖励CALL  0x438D10
#define 退出时空之塔CALL  0x43BA90
#define Hook副本不过图Addr 0x6B3CC6
#define 设置角度发包参数1 0x3F
#define 设置角度发包参数2 0x1198DE0
#define UI技能资源偏移 0xB0
#define 怪物BUFF偏移1 0x14
#define 怪物BUFF偏移2 0xD628
#define 怪物BUFF偏移3 0x22F8C
#define 怪物BUFF偏移4 0x18
#define 怪物BUFF偏移5 0x2658
#define 怪物BUFF偏移6 0x2648
#define 背包遍历偏移 0x8D8
#define 当前频道偏移 0x894
#define 走路偏移 0x1B8
#define 背包格子数量偏移 0x188
#define 人物基址偏移1 0x4C
#define 人物基址偏移2 0x80
#define 人物基址偏移3 0x44
#define Name偏移 0x8C
#define MAPID偏移 0x90
#define HP偏移 0xB0
#define Point偏移 0x1D0
#define 卖NPC偏移 0xB8
#define 怪物遍历_偏移 0xB8
#define 怪物属性偏移 0x850
#define 交任务CALL  0x53A070
#define 判断是否掉线偏移 0x74
#define 加属性点发包参数1 0x70
#define 加属性点发包参数2 0x1198DE0
#define 切换技能系CALL 0xA3FB60
#define BOSS被控制BUFF数量偏移 0x2B0
#define BOSS是否可控偏移 0x2B4
#define 装备突破CALL 0x4B8670
#define 进入副本空间CALL 0xBC81C0
#define 进入副本空间偏移 0x6C
#define 召唤拿猫偏移1 0x1463
#define 召唤拿猫偏移2 0x1460
#define 召唤拿礼物CALL 0xE98E90
#define 离开技能训练场CALL 0x596D50
#define 技能训练场偏移 0x570
#define 喊话CALL 0x764C70
#define 输入OTPCALL 0xE1FCE0
#define 邮寄CALL 0x436CC0
#define 邮寄参数偏移 0x958
#define 收无物品邮件CALL 0x436CC0
#define 设置瞬移浮空偏移 0x3709
#define 瞬移次数偏移 0x3520
#define 瞬移距离偏移 0x38F0
#define 瞬移Buffer填充CALL 0x650B90
#define 全局加速偏移1 0x50
#define 全局加速偏移2 0x3C
#define 全局加速偏移3 0x444
#define 怪物隐形偏移 0x104
#define 神行百变偏移 0x24C
#define 复活发包参数1 0x88
#define 复活发包参数2 0x1198DE0
#define 强制开箱子偏移 0x185
#define 领取星币发包参数1 0x3A9
#define 领取星币发包参数2 0x1198DE0
#define 星币偏移1 0x54
#define 星币偏移2 0x2C
#define 正在走路偏移 0x218
#define 星币商店购物CALL 0xD071B0
#define 捡物状态偏移 0x160
#define 星币数量偏移 0x258
#define 物品颜色偏移 0x88
#define 组队难度CALL 0x4F7E80
#define 八卦强化CALL 0x0
#define 八卦属性偏移 0x1C04
#define 八卦属性CALL偏移 0x64
#define 物品八卦ID偏移 0x38
#define 竞拍偏移1 0x710
#define 竞拍偏移2 0x54
#define 创建副本房间发包参数1 0x1198DE0
#define 创建副本房间发包参数2 0x24E
#define 寻找房间发包参数1 0x1198F18
#define 寻找房间发包参数2 0x257
#define 副本房间偏移1 0xD74
#define 副本房间偏移2 0x14
#define 房间准备CALL 0x5E1F30
#define 房间是否准备CALL 0x89C6E0
#define 队长是否准备CALL 0x5E6D80
#define 队长房间准备偏移 0x95
#define 选择副本CALL 0x5E5190
#define 选择副本难度CALL 0x5E1B30
#define 隐藏UI偏移 0x3C
#define 离开洪门修理场CALL 0xE95FC0
#define 退出门派CALL 0x477D30
#define 仓库号交易物品数量偏移 0x138
#define 退回跨服组队界面CALL 0x5D4410
#define 副本房间偏移3 0xD78
#define 退出跨服组队房间CALL 0x5C5B00
#define 退出跨服组队界面CALL 0x51D2FB
#define 创建角色偏移  0x48
#define 遍历礼箱偏移 0x54
#define UI技能名字偏移 0xB0
#define 技能名字偏移1 0x1C
#define 技能名字偏移2 0x18
#define 人物属性点偏移 0x60
#define Money偏移 0xB8
#define 阵营偏移 0xC8
#define MP偏移 0xE8
#define 怪物目标ID 0x100
#define 人物读条偏移 0x190
#define 状态偏移 0x199
#define 倒地偏移 0x1A0
#define MAXHP偏移1 0x238
#define MAXHP偏移2 0x240
#define 申请入队CALL 0x4F7B50
#define 同意申请入队CALL 0x4F7770
#define 跨服掉线进入CALL 0xAA9980

//钩挂任务
#define HookTask_JmpAddr					HookTask_Addr + 7
#define HookTaskCALL_Addr					HookTask_Addr + 0x11

#define HookBLTask_JmpAddr					HookBLTask_Addr + 7
#define HookBLTaskCALL_Addr					HookBLTask_Addr + 0x21
/* *********************************************************************** /
/* 基本固定的偏移                                                   */
/************************************************************************/
#define 获取当前城市IDCALL					遁术CALL

#define 最大频道偏移						当前频道偏移 + 0x4
#define 人物基址偏移4						人物基址偏移3



#define 等级偏移										MAPID偏移 + 0xC


#define PK豆偏移										0x1C8
#define 人物过图偏移									0x1E4//没用

#define TargetId偏移									0x228//没用
#define 过门偏移										0x2F4//没用

#define Ui视觉偏移										0x24CD0



#define 一次捡物偏移									技能冷却偏移
#define Hook瞬移地址									人物瞬移钩挂地址

//8B038B90????00008BCBFFD2D945??51 + 0x11
#define 人物瞬移偏移2								0x2F0//2D8
#define	视角偏移										0x62
#define 怪物状态偏移									倒地偏移
#define 是否使用技能中偏移							0x34
#define 技能读条偏移									0x30
#define 白仙修练场偏移4								白仙修练场偏移2 + 0x4
#define 白仙修练场偏移2								白仙修练场偏移1 + 0x4
#define 白仙修练场进度偏移							0x3C
#define 获取等级劵数量偏移							0x1FC
#define 星币商店购物偏移							0x2C
#define 人物坐标偏移1						0x3BC
#define 人物坐标偏移2						0x40
#define 人物坐标偏移3						0x224
#define 人物坐标偏移4						0x230
#define 人物坐标偏移5						0x224
//怪物类型的偏移
#define 周围遍历分类偏移						0xE
#define  退出50级UI偏移 0x0
// 暂时搁置的CALL
#define 修改视觉CALL									0x0
#define 重置视角CALL									0x0
#define 收邮件CALL									0x0
#define 邮件偏移2									0x64
#define 竞拍物品Index偏移							0x70
#define 任务尸体是否可捡偏移	              0x24
#define 人物BUFF偏移5                         0x10 //old-18
#define  存入衣柜CALL 0x0
#define 转盘CALL 0x0
#endif // BnsProject_korea

/************************************************************************/
/* 宏                                                                     */
/************************************************************************/
CONST DWORD SOCK_MSG_RECHARGECARD					= 0x17;
CONST DWORD SOCK_MSG_QUERY_CARDINFO					= 0x2;
CONST DWORD SOCK_MSG_KEEPALIVE						= 0x3;
CONST DWORD SOCK_MSG_QUERY_SCRIPTKEY				= 0x4;
CONST DWORD SOCK_MSG_CLIENTMSG						= 0x1;
CONST DWORD SOCK_MSG_TITLE							= 0x6;
CONST DWORD SOCK_MSG_GET_TASKSCRIPT					= 0x7;
CONST DWORD SOCK_MSG_GET_NEXT_TASKSCRIPT			= 0x8;
CONST DWORD SOCK_MSG_LOGINGAME						= 0x9;
CONST DWORD SOCK_MSG_GETRC4_KEY						= 0xA;
CONST DWORD SOCK_MSG_JOIN_TEAM_ORDER				= 0xB;
CONST DWORD SOCK_MSG_LEAVE_TEAM_ORDER				= 0xC;
CONST DWORD SOCK_MSG_UPDATE_TEAM_ORDER				= 0xD;
CONST DWORD SOCK_MSG_DISSOLUTION_TEAM				= 0xE;
CONST DWORD SOCK_MSG_CLIENT_ITEMSUPPORT				= 0xF;
CONST DWORD SOCK_MSG_CLIENT_WAREHOUSE_MAINTASK		= 0x10;
CONST DWORD SOCK_MSG_CLIENT_WAREHOUSE_PICKITEM		= 0x11;
CONST DWORD SOCK_MSG_CLIENT_PICKITEM				= 0x12;
CONST DWORD SOCK_MSG_CLIENT_GET_TEAMMEMBER_TASKINFO = 0x13;
CONST DWORD SOCK_MSG_CLIENT_CREATE_TEAMMEMBER_TASKINFO = 0x14;
CONST DWORD SOCK_MSG_CLIENT_DEALMESSAGE				= 0x15;
CONST DWORD SOCK_MSG_CLIENT_STOPCLIENT				= 0x16;
CONST DWORD SOCK_MSG_CLIENT_LOGINED_LOG				= 0x18;
CONST DWORD SOCK_MSG_CLIENT_BANNED					= 0x19;
CONST DWORD SOCK_MSG_JOIN_DUNGOEN_TEAM				= 0x1A;
CONST DWORD SOCK_MSG_DISSOLUTION_DUNGOEN_TEAM		= 0x1B;
CONST DWORD SOCK_MSG_GET_CHANNEL					= 0x1C;
CONST DWORD SOCK_MSG_SET_CONFIG_VALUE				= 0x1D;
CONST DWORD SOCK_MSG_GET_CONFIG_VALUE				= 0x1E;
CONST DWORD SOCK_MSG_UPDATE_TEAM_SCHEDULE			= 0x1F;
CONST DWORD SOCK_MSG_GET_TEAM_SCHEDULE				= 0x20;
CONST DWORD SOCK_MSG_CONSOLE_KEEPALIVE				= 0x21;
CONST DWORD SOCK_MSG_SET_TEAM_READONLY				= 0x22;
CONST DWORD SOCK_MSG_SAVE_LOCAL_ACCOUNT_LOG			= 0x23;
CONST DWORD SOCK_MSG_OTP							= 0x24;
CONST DWORD SOCK_MSG_QUERY_DAILYTASK_SCHEDULE		= 0x25;
CONST DWORD SOCK_MSG_UPDATE_DAILYTASK_SCHEDULE		= 0x26;

CONST std::string  LOCAL_IP							= "192.168.0.102";
CONST std::string  SERVER_IP						= "118.178.125.149";
CONST std::string  TEST_IP							= "120.27.136.99";	//"59.8.163.201"
CONST std::string  SERVER2_IP						= "153.36.233.103";
CONST std::string  MY_IP							= "100.81.157.169";
CONST SHORT SERVER_PORT								= 12376;

/////////云配置/////////////////////////////////////////////////////////////////
CONST std::wstring CLOUD_CONFIG_NAME_BAIXIAN_SECHEDULE	= L"白仙修练场进度";
CONST std::wstring CLOUD_CONFIG_NAME_ONE_TIME_TASK		= L"一次性任务"; // 后面加上任务ID, 如： 一次性任务50A 

///////UI Text///////////////////////////////////////////////////////////////////
CONST std::wstring UI_TEXT_F9						= L"GoodsStorePanel";
CONST std::wstring UI_TEXT_PARTYAUCTION				= L"PartyAuctionPanel";

///////////装备///////////////////////////////////////////////////////////////

///////////技能///////////////////////////////////////////////////////////////
#define Lux_SkillName_Tab							L"광풍"						//转圈圈
#define Lux_SkillName_Jmp							L"지진"						//跳
#define Lux_SkillName_V								L"철벽"						//加血
#define Lux_SkillName_1								L"지진"						//撼地
#define Lux_SkillName_2								L"격돌"						//冲锋
#define Lux_SkillName_3								L"발구르기"					//天崩地裂
#define Lux_SkillName_Floor_Tab1					L"탈출광풍"					//倒地转圈圈1
#define Lux_SkillName_Floor_Tab2					L"탈출광풍"					//倒地转圈圈2
#define Lux_SkillName_Floor_Tab3					L""							//倒地转圈圈3
#define Lux_SkillName_X								L"분쇄"						//V的时候用攻击
#define Lux_SkillName_E								L"분노"						//愤怒
#define Lux_SkillName_SS							L"후퇴"						//逆风行
#define Lux_SkillName_C								L"열화륜"					//风火轮
#define Lux_SkillName_Z								L"발차기"					//碎金脚
#define Lux_SkillName_ZZZ							L"끌어오기"					//万象天引
#define Lux_SkillName_X								L"분쇄"						//霸王击鼎
#define Lux_SkillName_R								L"심판"						//削骨
#define Lux_SkillName_T								L"파괴"						//毁灭
#define Lux_SkillName_4								L"절단"						//灭绝
#define Lux_SkillName_F								L"장악"						//扼喉
#define Lux_SkillName_ZZ							L"내다꽂기"					//千军倒
#define Lux_SkillName_FF							L"무릎차기"					//蛮牛昂首
#define Lux_SkillName_万象天引						L"끌어오기"					//万象天引

// 开启全局加速
#define ENABLE_GLOAB_SPEED							0
// 开启局部加速
#define ENABLE_LOCAL_SPEED							1

// 邮寄
#define ENABLE_CLIENT_DEALWAY_MAIL					0x1
// 交易
#define ENABLE_CLIENT_DEALWAY_DEAL					0x2

#define CLIENT_DEALWAY								ENABLE_CLIENT_DEALWAY_MAIL

// 力士技能
#define Lux_SkillId_SS								0x5E6A
#define Lux_SkillId_Floor_Tab						0x5E60
#define Lux_SkillId_Floor_1							0x5EA6
#define Lux_SkillId_Floor_2							0x5E9C
#define Lux_SkillId_E								0x5E1A
#define Lux_SkillId_Z_Kick							0x5DE8
#define Lux_SkillId_Z_Attract						0x5DE8
#define Lux_SkillId_F_Z								0x5E4C
#define Lux_SkillId_X								0x5E24
#define Lux_SkillId_C								0x5E1B
#define Lux_SkillId_C_c								0x5E3A
#define Lux_SkillId_1								0x5E74
#define Lux_SkillId_2								0x5DF2
#define Lux_SkillId_3								0x5E06
#define Lux_SkillId_4								0x5E10
#define Lux_SkillId_F_Throat						0x5E2E
#define Lux_SkillId_F_Kick							0x5E38
#define Lux_SkillId_Q								0x5DCC
#define Lux_SkillId_4_Fly							0x5E11
#define Lux_SkillId_F_Fly							0x5E13
#define Lux_SkillId_R								0x5DC1
#define Lux_SkillId_T								0x5DDE
#define Lux_SkillId_V								0x5DFC
#define Lux_SkillId_V_R								0x5ECE
#define Lux_SkillId_Tab								0x5DCA
#define Lux_SkillId_E_R								0x5DD7
#define Lux_SkillId_E_R2							0x5DD8
#define Lux_SkillId_E_R3							0x5DD9
#define Lux_SkillId_Floor_F							0x5EB0

#define Lux_BuffId_E								0x16F9595
#define Lux_BuffId_Tab_1							0x16E5D9E
#define Lux_BuffId_Tab_2							0x16E5DA2
#define Lux_BuffId_V_1								0x16F206B
#define Lux_BuffId_V_2								0xDBBDFA
#define Lux_BuffId_V_3								0x16F20A7
#define Lux_BuffId_V_4								0x16F2071


//召唤技能
#define Sum_SkillId_R                               0x65F5    //장미  玫瑰
#define Sum_SkillId_R2                              0x65F6    //장미  玫瑰2
#define Sum_SkillId_T                               0x65FB    //말벌  马蜂
#define Sum_SkillId_T2                              0x65FC    //말벌  向日葵
#define Sum_SkillId_T2_Ui                           0x6752    //말벌  向日葵2?
#define Sum_SkillId_T2_Ui_D                         0x6753    //말벌  大向日葵2?
#define Sum_SkillId_T_Floor                         0x65F7   //휘감기 绊倒 0x65F7
#define Sum_SkillId_T_Floor2                        0x672D  //휘감기 绊倒 672D
#define Sum_SkillId_Tab_cat                         0x65F4    //흐규흐규  泪光  读5秒 复活猫
#define Sum_SkillId_Tab                             0x6658   //달려들기   猫猫出击  66BC普通猫猫出击
#define Sum_SkillId_Tab_Suppress                    0x666D   //누르기 压制 665E? 65F4召唤猫
#define Sum_SkillId_Tab_Suppress_1                  0x665E   //누르기 压制 665E? 65F4召唤猫
#define Sum_SkillId_1                               0x65F9   //덩굴손 1   常春藤
#define Sum_SkillId_2                               0x65FA   //가시덩굴 2   荆刺藤
#define Sum_SkillId_3                               0x65FF   //꽃가루3  花粉
#define Sum_SkillId_4                               0x6600   //민들레씨4  蒲公英
#define Sum_SkillId_F                               0x65FC   //나팔꽃 흡수F   牵牛花
#define Sum_SkillId_F_4                             0x65FE   //隐身后的F   牵牛花
#define Sum_SkillId_E                               0x65FD   //강아지풀E   狗尾草
#define Sum_SkillId_C                               0x6663   //충격C   大榔头
#define Sum_SkillId_C_C                             0x6664   //충격C   大榔头
#define Sum_SkillId_V                               0x665A   //하단베기V  下段斩
#define Sum_SkillId_V_2                             0x6667   //V猫技能
#define Sum_SkillId_V_3                             0x6668   //飞天后压
#define Sum_SkillId_F_F                             0x66C6   //투척밤송이F   飞栗球
#define Sum_SkillId_4_4                             0x66C7   //群体免疫蒲公英
#define Sum_SkillId_Z                               0x65F8   //응원   鼓励
#define Sum_SkillId_X                               0x66C0   //우정 友情
#define Sum_SkillId_X_2                             0x6660   //X2
#define Sum_SkillId_Floor_1                         0x6602   //교체 倒地 1
#define Sum_SkillId_Floor_2                         0x6619   //후방 회피 倒地 2
#define Sum_SkillId_Floor_C                         0x6662   //망치 돌리기 倒地 C
#define Sum_SkillId_Floor_C1                        0x666A   //猫倒地
#define Sum_SkillId_Floor_F                         0x6601   //뒤구르기 倒地F
#define Sum_SkillId_SS						        0x660D
#define Sum_SkillId_Q						        0x6661    //웅크리기 猥琐
#define Sum_SkillId_Relive					        0x666C
#define SKILLID_RELIVE						        0x1F4
#define Sum_SkillId_G								0x67C5
#define Sum_SkillId_B								0x67C7


#define Sumoner_BuffId_X							0x1915E34
#define Sumoner_BuffId_3							0x18E6C6A
#define Sumoner_BuffId_1							0xF5BE37
#define Sumoner_BuffId_F							0x18E58D3
//x 20级


// 气宗技能
#define Kffm_Kfm_SkillId_Tab						0x75C6
#define Kffm_Kfm_SkillId_1							0x756C
#define Kffm_Kfm_SkillId_1_1						0x7570
#define Kffm_Kfm_SkillId_2							0x7580
#define Kffm_Kfm_SkillId_2_F						0x760C
#define Kffm_Kfm_SkillId_3							0x758A
#define Kffm_Kfm_SkillId_4							0x7594
#define Kffm_Kfm_SkillId_Z							0x75A8
#define Kffm_Kfm_SkillId_X							0x75BC
#define Kffm_Kfm_SKillID_X_X						0x768E
#define Kffm_Kfm_SkillId_C							0x75B2
#define Kffm_Kfm_SkillId_C_F						0x7562
#define Kffm_Kfm_SkillId_V							0x759E
#define Kffm_Kfm_SkillId_V_F						0x7581
#define Kffm_Kfm_SkillId_R							0x7533
#define Kffm_Kfm_SkillId_T							0x7544
#define Kffm_Kfm_SkillId_T_F						0x754F
#define Kffm_Kfm_SkillId_Q							0x7616
#define Kffm_Kfm_SkillId_E							0x7620
#define Kffm_Kfm_SkillId_Floor_Tab				0x763E
#define Kffm_Kfm_SkillId_Floor_1					0x7648
#define Kffm_Kfm_SkillId_Floor_2					0x7652
#define Kffm_Kfm_SkillId_Floor_22				0x76A2
#define Kffm_Kfm_SkillId_Floor_F					0x7634
//气宗 气模式
#define Kffm_Fm_SkillId_1								0x7571
#define Kffm_Fm_SkillId_1_1							0x7575
#define Kffm_Fm_SkillId_2								0x75E4
#define Kffm_Fm_SkillId_3								0x75EE
#define Kffm_Fm_SkillId_4								0x75F8
#define Kffm_Fm_SkillId_V							0x7602
#define Kffm_Fm_SkillId_SS							0x762A
#define Kffm_Fm_SkillId_Q							0x7617
#define Kffm_Fm_SkillId_E								0x7621
#define Kffm_Fm_SkillId_R							0x75D0
#define Kffm_Fm_SkillId_T								0x75DA
#define Kffm_Fm_SkillId_V_F							0x7688
#define Kffm_Fm_SkillId_X							0x75C2
#define Kffm_Fm_SkillId_Z							0x75A9
#define Kffm_Fm_SkillId_C							0x75B3


//咒术师技能
#define Warlock_SkillId_1	0x6D74	//	SkillName[방탄부,防弹符], ID=6D74, KeyText=1,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_1_F	0x6D7E	//	SkillName[진압,压制], ID=6D7E, KeyText=F,姿态=28, IsCool=1, IsActive=1	
#define Warlock_SkillId_2	0x6E0A	//	SkillName[결계,结界], ID=6E0A, KeyText=2,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_3	0x6D9C	//	SkillName[개문,神凝咒], ID=6D9C, KeyText=3,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_4	0x6DBA	//	SkillName[흑염룡,黑炎龙], ID=6DBA, KeyText=4,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_4_1	0x6DBB	//	SkillName[흑염룡,黑炎龙], ID=6DBB, KeyText=4,姿态=29, IsCool=1, IsActive=1	
#define Warlock_SkillId_B	0x6F0E	//	SkillName[사령결계,死灵结界], ID=6F0E, KeyText=B,姿态=28, IsCool=1, IsActive=0	
#define Warlock_SkillId_C	0x6E46	//	SkillName[항마진,降魔阵], ID=6E46, KeyText=C,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_E	0x701C	//	SkillName[호출,唤回], ID=701C, KeyText=E,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_F1_1	0x6E3C	//	SkillName[파열,破灭], ID=6E3C, KeyText=F,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_F1_2	0x6E3D	//	SkillName[파열,破灭], ID=6E3D, KeyText=F,姿态=29, IsCool=1, IsActive=1	
#define Warlock_SkillId_F2	0x6D6B	//	SkillName[강탈,掠夺], ID=6D6B, KeyText=F,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Floor_1	0x6EAA	//	SkillName[진공파,裂空波], ID=6EAA, KeyText=1,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Floor_2	0x6E96	//	SkillName[탈출,逃脱], ID=6E96, KeyText=2,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Floor_3	0x6EBE	//	SkillName[치환,调换], ID=6EBE, KeyText=3,姿态=29, IsCool=1, IsActive=1	有召唤兽的情况下可使用
#define Warlock_SkillId_Floor_F	0x6EA0	//	SkillName[뒤구르기,后滚翻], ID=6EA0, KeyText=F,姿态=28, IsCool=1, IsActive=0	
#define Warlock_SkillId_G	0x6EF0	//	SkillName[마령살,魔灵杀], ID=6EF0, KeyText=G,姿态=28, IsCool=1, IsActive=0	
#define Warlock_SkillId_God_4	0x6E24	//	SkillName[각성 흑염룡,觉醒黑炎龙], ID=6E24, KeyText=4,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_God_T	0x6F29	//	SkillName[각성 수라,觉醒修罗], ID=6F29, KeyText=T,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Grab_R_1	0x6F68	//	SkillName[혼령질주,灵魂疾行], ID=6F68, KeyText=R,姿态=28, IsCool=0, IsActive=1	抓举
#define Warlock_SkillId_Grab_R_2	0x6F69	//	SkillName[혼령질주,灵魂疾行], ID=6F69, KeyText=R,姿态=28, IsCool=1, IsActive=1	抓举
#define Warlock_SkillId_Q	0x7026	//	SkillName[명령,命令], ID=7026, KeyText=Q,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_R1_1	0x6D69	//	SkillName[염,炎], ID=6D69, KeyText=R,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_R1_2	0x6D61	//	SkillName[염,炎], ID=6D61, KeyText=R,姿态=28, IsCool=0, IsActive=1	
#define Warlock_SkillId_R1_3	0x6D62	//	SkillName[염,炎], ID=6D62, KeyText=R,姿态=29, IsCool=1, IsActive=1	
#define Warlock_SkillId_R2_1	0x6D65	//	SkillName[폭,爆], ID=6D65, KeyText=R,姿态=28, IsCool=0, IsActive=1	
#define Warlock_SkillId_R2_2	0x6D66	//	SkillName[폭,爆], ID=6D66, KeyText=R,姿态=29, IsCool=1, IsActive=1	
#define Warlock_SkillId_R3_1	0x6D83	//	SkillName[진언,真言], ID=6D83, KeyText=R,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_R3_2	0x6D84	//	SkillName[진언,真言], ID=6D84, KeyText=R,姿态=28, IsCool=1, IsActive=1	
#define Warlock_SkillId_SS	0x6E8C	//	SkillName[월식,逆风行], ID=6E8C, KeyText=SS,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_T1_1	0x6F22	//	SkillName[야차,野茶], ID=6F22, KeyText=T,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_T1_2	0x6F28	//	SkillName[야차,野茶], ID=6F28, KeyText=T,姿态=29, IsCool=0, IsActive=1	
#define Warlock_SkillId_T2_1	0x6F25	//	SkillName[나찰,罗刹], ID=6F25, KeyText=T,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_T2_2	0x6F2B	//	SkillName[나찰,罗刹], ID=6F2B, KeyText=T,姿态=29, IsCool=0, IsActive=0	
#define Warlock_SkillId_T3_1	0x6F26	//	SkillName[수라,修罗], ID=6F26, KeyText=T,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_T3_2	0x6F2C	//	SkillName[수라,修罗], ID=6F2C, KeyText=T,姿态=29, IsCool=0, IsActive=0	
#define Warlock_SkillId_Tab	0x6F54	//	SkillName[소환,召唤], ID=6F54, KeyText=Tab,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Tab_God	0x7080	//	SkillName[강신,超神], ID=7080, KeyText=Tab,姿态=28, IsCool=0, IsActive=1	
#define Warlock_SkillId_V_1	0x6D88	//	SkillName[사령쇄도,死灵降临], ID=6D88, KeyText=V,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_V_2	0x6D89	//	SkillName[사령쇄도,死灵降临], ID=6D89, KeyText=V,姿态=28, IsCool=0, IsActive=1	
#define Warlock_SkillId_X	0x6D92	//	SkillName[경계베기,警戒斩], ID=6D92, KeyText=X,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Z_1	0x6DA6	//	SkillName[보호부,保护符], ID=6DA6, KeyText=Z,姿态=28, IsCool=0, IsActive=0	
#define Warlock_SkillId_Z_2	0x6DA7	//	SkillName[보호부,保护符], ID=6DA7, KeyText=Z,姿态=28, IsCool=0, IsActive=1	







//剑士
#define BladeMaster_SkillId_Gesture_F 0x4EF3//击剑SkillName[격검,], ID=4EF3, KeyText=F,姿态=7
#define BladeMaster_SkillId_V_C 0x507B//天雷令(格挡远程攻击)SkillName[뇌령,], ID=507B, KeyText=C,姿态=2
#define BladeMaster_SkillId_V_C1 0x5087//天雷令SkillName[뇌령,], ID=5087, KeyText=C,姿态=2 有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_V_C 0x5085//天雷令SkillName[뇌령,], ID=5085, KeyText=C,姿态=7
#define BladeMaster_SkillId_Gesture_Z 0x5023//山崩地裂SkillName[대지 가르기,], ID=5023, KeyText=Z,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_2 0x4E9E//追风剑SkillName[돌진,], ID=4E9E, KeyText=2,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_21 0x4F06//袭击SkillName[습격,], ID=4F06, KeyText=2,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_1 0x4E89//格挡SkillName[막기,], ID=4E89, KeyText=1,姿态=2 有几率被姿态7遍历到
#define BladeMaster_SkillId_Tab 0x4EAA//满月斩SkillName[만월베기,满月斩], ID=4EAA, KeyText=Tab,姿态=2 有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_Tab 0x4EEF//满月斩SkillName[만월베기,], ID=4EEF, KeyText=Tab,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_R1 0x4EE9//拔剑SkillName[발도,], ID=4EE9, KeyText=R,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_R2 0x4EFC//拔剑SkillName[발도,], ID=4EFC, KeyText=R,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_R3 0x4EFD//拔剑SkillName[발도,], ID=4EFD, KeyText=R,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_R4 0x4EFE//拔剑SkillName[발도,], ID=4EFE, KeyText=R,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_R5 0x4EEB//快剑SkillName[쾌검,], ID=4EEB, KeyText=R,姿态=7
#define BladeMaster_SkillId_V_X 0x5078//金刚令SkillName[보호령,], ID=5078, KeyText=X,姿态=7 两种姿势公用的ID
#define BladeMaster_SkillId_V 0x4E23//凤翼SkillName[봉익,], ID=4E23, KeyText=V,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Z 0x4E88//飞燕剑SkillName[비연검,], ID=4E88, KeyText=Z,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_X 0x4E9F//破心脚SkillName[앞차기,], ID=4E9F, KeyText=X,姿态=2
#define BladeMaster_SkillId_Gesture_V_X 0x5079//御剑连斩SkillName[어검 연속 베기,御剑连斩], ID=5079, KeyText=X,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_1_F 0x4E8A//御剑保护SkillName[어검보호,], ID=4E8A, KeyText=F,姿态=2
#define BladeMaster_SkillId_C 0x4EAE//巧劲冲SkillName[어깨치기,], ID=4EAE, KeyText=C,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_E 0x4E91//侧身闪SkillName[역습,], ID=4E91, KeyText=E,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Q 0x4EA3//侧身闪SkillName[역습,侧身闪], ID=4EA3, KeyText=Q,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_E 0x4F02//侧身闪SkillName[역습,], ID=4F02, KeyText=E,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_Q 0x4F03//侧身闪SkillName[역습,], ID=4F03, KeyText=Q,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_3 0x4EED//五连斩SkillName[오연베기,], ID=4EED, KeyText=3,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_B 0x51F5//流星闪SkillName[유성섬,], ID=51F5, KeyText=B,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_B 0x51FF//流星闪SkillName[유성섬,], ID=51FF, KeyText=B,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_G 0x51FE//以气御剑SkillName[이기어검,], ID=51FE, KeyText=G,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_G 0x51F4//以气御剑SkillName[이기어검,], ID=51F4, KeyText=G,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_V 0x4EF4//一闪SkillName[일섬,], ID=4EF4, KeyText=V,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_2 0x4E84//疾风SkillName[질풍,], ID=4E84, KeyText=R,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_T1 0x4E87//刺剑式SkillName[찌르기,], ID=4E87, KeyText=T,姿态=2有几率被姿态2遍历到
#define BladeMaster_SkillId_T2 0x4E98//刺剑式SkillName[찌르기,], ID=4E98, KeyText=T,姿态=2有几率被姿态2遍历到
#define BladeMaster_SkillId_T3 0x4E9C//刺剑式SkillName[찌르기,], ID=4E9C, KeyText=T,姿态=2
#define BladeMaster_SkillId_1_T 0x4E9B//刺剑式(破招式后的刺剑式)SkillName[찌르기,], ID=4E9B, KeyText=T,姿态=2
#define BladeMaster_SkillId_4 0x4EEA//焦热斩SkillName[초열베기,], ID=4EEA, KeyText=4,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_4 0x4EF0//,焦热斩SkillName[초열베기], ID=4EF0, KeyText=4,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Floor_Tab 0x4E8F//逃脱？龙腾九天SkillName[탈출,], ID=4E8F, KeyText=Tab,姿态=7	
#define BladeMaster_SkillId_Floor_1 0x4EA2//斩马步SkillName[다리베기,], ID=4EA2, KeyText=1,姿态=7
#define BladeMaster_SkillId_Floor_3 0x4EA7//回击SkillName[받아치기,], ID=4EA7, KeyText=3,姿态=7
#define BladeMaster_SkillId_Floor_4 0x4F58//蟠龙伏地SkillName[누워막기,], ID=4F58, KeyText=4,姿态=7
#define BladeMaster_SkillId_Floor_F 0x4E8B//后滚翻SkillName[뒤구르기,后滚翻], ID=4E8B, KeyText=F,姿态=7	
#define BladeMaster_SkillId_V_V 0x507A//回天令SkillName[회천령,], ID=507A, KeyText=V,姿态=2
#define BladeMaster_SkillId_V_Z 0x5086//爆剑令SkillName[폭검령,], ID=5086, KeyText=Z,姿态=2
#define BladeMaster_SkillId_V_Z1 0x507D//爆剑令SkillName[폭검령,], ID=507D, KeyText=Z,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_Gesture_V_Z 0x507C//爆剑令SkillName[폭검령,], ID=507C, KeyText=Z,姿态=2
#define BladeMaster_SkillId_Gesture_C 0x5024//风月斩SkillName[풍월도,], ID=5024, KeyText=C,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_V_V 0x5084//回天令SkillName[회천령,], ID=5084, KeyText=V,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_3 0x4E8C//下段斩SkillName[하단베기,], ID=4E8C, KeyText=3,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_R7 0x4E84//落剑式 2段
#define BladeMaster_SkillId_R8 0x4E85//扫剑式 2段 SkillName[혈풍,], ID=4E85, KeyText=R,姿态=2有几率被姿态7遍历到
#define BladeMaster_SkillId_R9 0x4E86//劈剑式 2段
#define BladeMaster_SkillId_R1 0x4FBA//落剑式 1段
#define BladeMaster_SkillId_R2 0x4FBB//扫剑式 1段
#define BladeMaster_SkillId_R3 0x4FBC//劈剑式 1段
#define BladeMaster_SkillId_R4 0x4EE8//?
#define BladeMaster_SkillId_R5 0x4E96
#define BladeMaster_SkillId_Gesture_T1 0x4F09//火龙连斩SkillName[화룡연참,], ID=4F09, KeyText=T,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_T2 0x4F0A//火龙连斩SkillName[화룡연참,], ID=4F0A, KeyText=T,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_T3 0x4F0B//火龙连斩SkillName[화룡연참,], ID=4F0B, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T4 0x4F0E//火龙连斩SkillName[화룡연참,], ID=4F0E, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T5 0x4F0F//火龙连斩SkillName[화룡연참,], ID=4F0F, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T6 0x4F10//火龙连斩SkillName[화룡연참,], ID=4F10, KeyText=T,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_T7 0x5014//火龙连斩(击剑挡格后的)SkillName[화룡연참,], ID=5014, KeyText=T,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_T8 0x5015//火龙连斩SkillName[화룡연참,], ID=5015, KeyText=T,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Gesture_T9 0x5016//火龙连斩
#define BladeMaster_SkillId_Gesture_T10 0x5017//火龙连斩
#define BladeMaster_SkillId_Gesture_T11 0x5018//火龙连斩SkillName[화룡연참,], ID=5018, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T12 0x5019//火龙连斩SkillName[화룡연참,火龙连斩], ID=5019, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T13 0x501A//火龙连斩SkillName[화룡연참,火龙连斩], ID=501A, KeyText=T,姿态=7
#define BladeMaster_SkillId_Gesture_T14 0x4FB0//觉醒火龙连斩
#define BladeMaster_SkillId_1_R 0x4F52//风火轮SkillName[회전베기,], ID=4F52, KeyText=R,姿态=2
#define BladeMaster_SkillId_Gesture_3_R 0x4F8B//风火轮SkillName[회전베기,], ID=4F8B, KeyText=R,姿态=7
#define BladeMaster_SkillId_SS 0x4EA4//闪避SkillName[회피,闪避], ID=4EA4, KeyText=SS,姿态=2
#define BladeMaster_SkillId_Gesture_SS 0x4EFB//闪避SkillName[회피,闪避], ID=4EFB, KeyText=SS,姿态=7有几率被姿态2遍历到
#define BladeMaster_SkillId_Tab_F 0x4EC8
#define BladeMaster_SkillId_F 0x4E93
#define BladeMaster_SkillId_F1 0x4E8E
#define BladeMaster_SkillId_F2 0x4EC8//火龙闪？
#define BladeMaster_SkillId_Grab_F 0x5048//[홍문일검,洪门一剑] ID=5048 KeyText=F
#define BladeMaster_SkillId_Float_R 0x4E95
#define BladeMaster_SkillId_Float_T 0x4F05


//气功
#define ForceMaster_SkillId_R	0x55F0	//	SkillName[열화장,烈火掌], ID=55F0, KeyText=R,姿态=10
#define ForceMaster_SkillId_T	0x55F1	//	SkillName[한빙장,寒冰掌], ID=55F1, KeyText=T,姿态=10
#define ForceMaster_SkillId_1_1	0x55F2	//	SkillName[충격파,真空破], ID=55F2, KeyText=1,姿态=8
#define ForceMaster_SkillId_1_2	0x55FC	//	SkillName[충격파,真空破], ID=55FC, KeyText=1,姿态=10
#define ForceMaster_SkillId_1_3	0x5606	//	SkillName[충격파,真空破], ID=5606, KeyText=1,姿态=10
#define ForceMaster_SkillId_2	0x55F3	//	SkillName[폭열염포,爆裂炎炮], ID=55F3, KeyText=2,姿态=10
#define ForceMaster_SkillId_2_2	0x55FD	//?	
#define ForceMaster_SkillId_3	0x55F4	//	SkillName[기공포,气功炮], ID=55F4, KeyText=3,姿态=10|9
#define ForceMaster_SkillId_4_1	0x55F5	//	SkillName[중력장,重力掌], ID=55F5, KeyText=4,姿态=10
#define ForceMaster_SkillId_4_4	0x55F6	//	SkillName[폭열연파,爆炎连破], ID=55F6, KeyText=4,姿态=10
#define ForceMaster_SkillId_4_2	0x55FF	//	SkillName[중력장,重力掌], ID=55FF, KeyText=4,姿态=10
#define ForceMaster_SkillId_Grab_4_1	0x55F8	//	SkillName[탄지공,一阳指], ID=55F8, KeyText=1,姿态=10
#define ForceMaster_SkillId_Grab_4_2	0x55F9	//	SkillName[밀기,裂空掌], ID=55F9, KeyText=2,姿态=10
#define ForceMaster_SkillId_Grab_4_4	0x5654	//	SkillName[흡수,吸星], ID=5654, KeyText=4,姿态=10
#define ForceMaster_SkillId_Grab_4_3	0x5655	//	SkillName[봉인,化骨绵掌], ID=5655, KeyText=3,姿态=10
#define ForceMaster_SkillId_Grab_4_Tab	0x5656	//	SkillName[허공,斗转星移], ID=5656, KeyText=Tab,姿态=10
#define ForceMaster_SkillId_Grab_4_V	0x5657	//	SkillName[화염지옥,焦土], ID=5657, KeyText=V,姿态=10
#define ForceMaster_SkillId_Grab_4_Z	0x5664	//	
#define ForceMaster_SkillId_Fure_Z	0x5659	//	SkillName[폭염,炎爆], ID=5659, KeyText=Z,姿态=10
#define ForceMaster_SkillId_Fure_Z_Z	0x5663	//	SkillName[폭염발산,余烬], ID=5663, KeyText=Z,姿态=10
#define ForceMaster_SkillId_Fure_X	0x565B	//	SkillName[화련장,火莲掌], ID=565B, KeyText=X,姿态=10
#define ForceMaster_SkillId_Fure_C	0x565D	//	SkillName[연화지,莲花指], ID=565D, KeyText=C,姿态=10
#define ForceMaster_SkillId_Fure_V	0x56B9	//	SkillName[유성지,流星指], ID=56B9, KeyText=V,姿态=10
#define ForceMaster_SkillId_Ice_Z	0x565A	//	SkillName[파음지,破冰], ID=565A, KeyText=Z,姿态=10
#define ForceMaster_SkillId_Ice_X	0x565C	//	SkillName[빙하장,冰河掌], ID=565C, KeyText=X,姿态=10
#define ForceMaster_SkillId_Ice_C	0x56B8	//	SkillName[호신장막,混元罩], ID=56B8, KeyText=C,姿态=10
#define ForceMaster_SkillId_Ice_V	0x56BA	//	SkillName[얼음꽃,荡华涟], ID=56BA, KeyText=V,姿态=10
#define ForceMaster_SkillId_Fure_F	0x56BB	//	SkillName[염룡파,炎龙破], ID=56BB, KeyText=F,姿态=10
#define ForceMaster_SkillId_Ice_F	0x56BC	//	SkillName[빙룡파,冰龙破], ID=56BC, KeyText=F,姿态=10
#define ForceMaster_SkillId_F	0x56BD	//	SkillName[쌍룡파,双龙破], ID=56BD, KeyText=F,姿态=10
#define ForceMaster_SkillId_Grab_F	0x56BE	//	SkillName[신화경,神化镜], ID=56BE, KeyText=F,姿态=10
#define ForceMaster_SkillId_Fure_Tab	0x56BF	//	SkillName[화염폭발,燎原之火], ID=56BF, KeyText=Tab,姿态=10
#define ForceMaster_SkillId_Ice_Tab	0x56C0	//	SkillName[결빙공,寒霜之体], ID=56C0, KeyText=Tab,姿态=10|9
#define ForceMaster_SkillId_Ice_Tab_Po	0x56CA	//破冰	
#define ForceMaster_SkillId_SS	0x571C	//	SkillName[이탈,逆风行], ID=571C, KeyText=SS,姿态=10
#define ForceMaster_SkillId_Q	0x571F	//	SkillName[화신보,火神步], ID=571F, KeyText=Q,姿态=10
#define ForceMaster_SkillId_E	0x5720	//	SkillName[한신보,冰神步], ID=5720, KeyText=E,姿态=10
#define ForceMaster_SkillId_Q1	0x5729	//	SkillName[화신보,火神步], ID=5729, KeyText=Q,姿态=10
#define ForceMaster_SkillId_E1	0x572A	//	SkillName[한신보,冰神步], ID=572A, KeyText=E,姿态=10
#define ForceMaster_SkillId_G	0x5820	//	SkillName[천룡포,天龙炮], ID=5820, KeyText=G,姿态=10
#define ForceMaster_SkillId_B	0x5821	//	SkillName[천빙장,天冰掌], ID=5821, KeyText=B,姿态=10
#define ForceMaster_SkillId_Floor_1	0x5610	//	SkillName[충격파,真空破], ID=5610, KeyText=1,姿态=10
#define ForceMaster_SkillId_Floor_F	0x571E	//	SkillName[뒤구르기,后滚翻], ID=571E, KeyText=F,姿态=10
#define ForceMaster_SkillId_Floor_Tab	0x56C1	//	SkillName[탈출,逃脱], ID=56C1, KeyText=Tab,姿态=10
#define ForceMaster_SkillId_Floor_Tab1	0x56F3 //被冰冻后的TAB

#define ForceMaster_Buff_炎玉							0xBB2E22
#define ForceMaster_Buff_冰玉							0xBB2E25
#define ForceMaster_Buff_火种							0xB71B23
#define ForceMaster_Buff_灼伤							0xB71B26

//刺客
#define Assassin_SkillId_R1_1 0x61A8//[안개베기, 迷雾斩]	ID = 61A8	KeyText = R	姿态 = 17
#define Assassin_SkillId_R1_2 0x62D4//[안개베기, 迷雾斩]	ID = 62D4	KeyText = R	姿态 = 17
#define Assassin_SkillId_R1_3 0x62D6//
#define Assassin_SkillId_R2_1 0x61A9//[이슬베기, 晨露斩]	ID = 61A9	KeyText = R	姿态 = 17
#define Assassin_SkillId_R2_2 0x62D5//[이슬베기, 晨露斩]	ID = 62D5	KeyText = R	姿态 = 17
#define Assassin_SkillId_R3_1 0x61AA//[달빛베기, 月光斩]	ID = 61AA	KeyText = R	姿态 = 17
#define Assassin_SkillId_R4_1 0x61C8//[달빛베기, 燕飞腿]	ID = 61C8KeyText = R	姿态 = 17
#define Assassin_SkillId_T1 0x61AB//[심장찌르기, 刺心]	ID = 61AB	KeyText = T	姿态 = 17
#define Assassin_SkillId_T2 0x620F//[심장찌르기, 刺心]	ID = 620F	KeyText = T	姿态 = 17
#define Assassin_SkillId_T3 0x6273//[심장찌르기, 刺心]	ID = 6273	KeyText = T	姿态 = 17
#define Assassin_SkillId_T4 0x62D7//[심장찌르기, 刺心]	ID = 62D7	KeyText = T	姿态 = 17
#define Assassin_SkillId_T5 0x61DC//[맹독찌르기, 毒刺]61DC
#define Assassin_SkillId_1_1 0x61B0//[잠입, 潜行]	ID = 61B0	KeyText = 1	姿态 = 17
#define Assassin_SkillId_1_2 0x62DC//[잠입, 潜行]	ID = 62DC	KeyText = 1	姿态 = 17
#define Assassin_SkillId_Grab_1 0x637E//SkillName[그림자 질주, 影子疾行], ID = 637E, KeyText = 1, 姿态 = 17, IsCool = 1, IsActive = 0
#define Assassin_SkillId_2 0x61AD//[나무토막, 替身术]	ID = 61AD	KeyText = 2	姿态 = 17
#define Assassin_SkillId_2_2 0x633D//[보호 나무토막, 木叶保护]	ID = 633D	KeyText = 2	姿态 = 17
#define Assassin_SkillId_2_F 0x61BF//[나뭇잎운신, 木叶闪现]	ID = 61BF	KeyText = F	姿态 = 17
#define Assassin_SkillId_2_T 0x61D5//[뇌격보, 雷击步]	ID = 61D5	KeyText = T	姿态 = 17
#define Assassin_SkillId_3 0x61AF//[맹독베기, 剧毒斩]	ID = 61AF	KeyText = 3	姿态 = 17
#define Assassin_SkillId_3_3 0x62DB//1段2阶 影斩
#define Assassin_SkillId_4 0x61B4//[독무투척, 投掷毒雾]	ID = 61B4	KeyText = 4	姿态 = 17
#define Assassin_SkillId_Z 0x61AE//[지뢰, 地雷]	ID = 61AE	KeyText = Z	姿态 = 17
#define Assassin_SkillId_Z_1 0x62DA//[폭파, 爆炸]	ID = 62DA	KeyText = Z	姿态 = 17
#define Assassin_SkillId_X 0x61B8//[수리검, 手里剑|연막,烟幕]	ID = 61B8	KeyText = X	姿态 = 17 
#define Assassin_SkillId_C 0x61B1//[연화난무, 莲华乱舞]	ID = 61B1	KeyText = C	姿态 = 17
#define Assassin_SkillId_V 0x627E//[투지발산, 斗志昂扬]	ID = 627E	KeyText = V	姿态 = 17
#define Assassin_SkillId_Tab_Tab 0x61B2//[거미줄, 缠丝术]	ID = 61B2	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab_Tab1 0x62DE//[거미줄, 缠丝术]	ID = 62DE	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab 0x61D1//[그림자침입, 回身踢]	ID = 61D1	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab1 0x62FD//[그림자침입, 回身踢]	ID = 62FD61B1	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab2 0x62EE//[나뭇잎날리기, 木叶移形]	ID = 62EE	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab3 0x61C7//[나뭇잎열풍, 影舞叶]	ID = 61C7	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Tab4 0x61C2//?
#define Assassin_SkillId_SS_F 0x61D9//[나뭇잎가르기, 木叶疾斩]	ID = 61D9	KeyText = F	姿态 = 17
#define Assassin_SkillId_F1 0x61CA//[암흑살, 暗黑杀]	ID = 61CA	KeyText = F	姿态 = 17
#define Assassin_SkillId_G 0x6392//[야행, 夜行]	ID = 6392	KeyText = G	姿态 = 17
#define Assassin_SkillId_G1 0x6393//[야행, 无影杀]	ID = 6392	KeyText = G	姿态 = 17
#define Assassin_SkillId_B 0x6389//[무영살, 禁影步]	ID = 6389	KeyText = B	姿态 = 17
//隐身
#define Assassin_SkillId_Hide_R 0x6220//[척추찌르기, 背刺]	ID = 6220	KeyText = R	姿态 = 17
#define Assassin_SkillId_Hide_T 0x61DC//?
#define Assassin_SkillId_Hide_1 0x6217//[그림자흡수, 吸影]	ID = 6217	KeyText = 1	姿态 = 18
#define Assassin_SkillId_Hide_11 0x62DF//[그림자흡수, 吸影]	ID = 62DF	KeyText = 1	姿态 = 18
#define Assassin_SkillId_Hide_2 0x6228//[시한폭탄, 长线火雷]	ID = 6228	KeyText = 2	姿态 = 17
#define Assassin_SkillId_Hide_3 0x626F//[무영보, 无影步]	ID = 626F	KeyText = 3	姿态 = 17
#define Assassin_SkillId_Hide_4 0x6236//[뇌절도, 雷决]	ID = 6236	KeyText = 4	姿态 = 17
#define Assassin_SkillId_Hide_F 0x61DA//[뇌전살, 雷电杀]	ID = 61DA	KeyText = F	姿态 = 18
#define Assassin_SkillId_Hide_X1 0x61BA//[그림자 밟기, 踏影术]	ID = 61BA	KeyText = X	姿态 = 17 移动到影匕命中敌人的背后
#define Assassin_SkillId_Hide_X2 0x61DB//[61DB,毒匕首] ID = 0x61DB KeyText = X
#define Assassin_SkillId_Hide_X 0x61B9//[그림자비수, 影匕]	ID = 61B9	KeyText = X	姿态 = 17
#define Assassin_SkillId_Hide_Tab 0x6227//[일격, 无影脚]	ID = 6227	KeyText = Tab	姿态 = 17
//倒地
#define Assassin_SkillId_Floor_F 0x61C6//[뒤구르기, 后滚翻]	ID = 61C6	KeyText = F	姿态 = 17
#define Assassin_SkillId_Floor_1 0x62F2//[수리검난무, 手里剑乱舞]	ID = 62F2	KeyText = 1	姿态 = 17
#define Assassin_SkillId_Floor_11 0x6363//[걷어차기, 挣扎]	ID = 6363	KeyText = 1	姿态 = 17
#define Assassin_SkillId_Floor_2 0x61C5//[볏짚인형, 金蝉脱壳]	ID = 61C5	KeyText = 2	姿态 = 17
#define Assassin_SkillId_Floor_21 0x6558//[연화, 莲华]	ID = 6558	KeyText = 2	姿态 = 17
#define Assassin_SkillId_Floor_Tab 0x61BB//[은신, 隐身,闪光]	ID = 61BB	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Floor_Tab1 0x634B//[은신, 隐身,闪光]	ID = 634B	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Floor_Tab2 0x6477//[은신, 隐身,闪光]	ID = 6477	KeyText = Tab	姿态 = 17
#define Assassin_SkillId_Floor_Tab3 0x64DB//被冰冻后的TAB
//抵抗
#define Assassin_SkillId_Q 0x61C0//[횡이동, 侧身闪]	ID = 61C0	KeyText = Q	姿态 = 17
#define Assassin_SkillId_E 0x61B5//[순보, 瞬步]	ID = 61B5	KeyText = E	姿态 = 17 使用完要设置面对BOSS
#define Assassin_SkillId_SS 0x61BE//[후방이동, 逆风行]	ID = 61BE	KeyText = SS	姿态 = 17

#define Assassin_Buff_毒 0x18CB6A8


//灵剑士
#define Danncer_SkillId_1	0x69E5	//	SkillName[발차기,碎金脚], ID=69E5, KeyText=1,姿态=20	
#define Danncer_SkillId_2_1	0x69E7	//	SkillName[돌진,追风剑], ID=69E7, KeyText=2,姿态=20	
#define Danncer_SkillId_2_2	0x6A13	//SkillName[습격, 袭击], ID = 6A13, KeyText = 2, 姿态 = 22, IsCool = 1, IsActive = 1
#define Danncer_SkillId_3	0x69E9	//	SkillName[하단베기,下段斩], ID=69E9, KeyText=3,姿态=20	
#define Danncer_SkillId_4_1	0x6A40	//	SkillName[흡공,大挪移], ID=6A40, KeyText=4,姿态=20	
#define Danncer_SkillId_4_2	0x6A46	//	SkillName[흡공,大挪移], ID=6A46, KeyText=4,姿态=20	
#define Danncer_SkillId_4_3	0x6A44	//	SkillName[흡공,大挪移], ID=6A44, KeyText=4,姿态=20	
#define Danncer_SkillId_Awaken_F1	0x6BF1	//	SkillName[각성 가르기,觉醒断水流], ID=6BF1, KeyText=F,姿态=20	
#define Danncer_SkillId_Awaken_F2	0x6C34	//	SkillName[각성 낙뢰격,觉醒落雷击], ID=6C34, KeyText=F,姿态=20	
#define Danncer_SkillId_Awaken_F3	0x6B26	//	SkillName[각성 뇌연섬,觉醒雷炎闪], ID=6B26, KeyText=F,姿态=22	
#define Danncer_SkillId_Awaken_F4	0x6B27	//	SkillName[각성 뇌연섬,觉醒雷炎闪], ID=6B27, KeyText=F,姿态=22	
#define Danncer_SkillId_Awaken_F5	0x6B28	//	SkillName[각성 뇌연섬,觉醒雷炎闪], ID=6B28, KeyText=F,姿态=22	
#define Danncer_SkillId_Awaken_R	0x6BF0	//	SkillName[각성 쾌검,觉醒快剑], ID=6BF0, KeyText=R,姿态=22	
#define Danncer_SkillId_B	0x6B3A	//	SkillName[광흡장,广吸场], ID=6B3A, KeyText=B,姿态=20	
#define Danncer_SkillId_C	0x69F6	//	SkillName[번개베기,天隙流光], ID=69F6, KeyText=C,姿态=20	
#define Danncer_SkillId_E	0x69EA	//	SkillName[역습,侧身闪], ID=69EA, KeyText=E,姿态=20	
#define Danncer_SkillId_F1	0x6A2D	//	SkillName[낙뢰격,落雷击], ID=6A2D, KeyText=F,姿态=20	
#define Danncer_SkillId_Floor_1	0x69ED	//	SkillName[다리베기,斩马步], ID=69ED, KeyText=1,姿态=20|22	
#define Danncer_SkillId_Floor_2	0x6A10	//	SkillName[받아치기,回击], ID=6A10, KeyText=2,姿态=20	
#define Danncer_SkillId_Floor_F	0x6A0F	//	SkillName[뒤구르기,后滚翻], ID=6A0F, KeyText=F,姿态=20	
#define Danncer_SkillId_Floor_Tab	0x69F9	//	SkillName[탈출,逃脱], ID=69F9, KeyText=Tab,姿态=20	
#define Danncer_SkillId_Floor_Tab1	0x6A5D	//被冰冻后的倒地
#define Danncer_SkillId_Fly_F1	0x6A21	//	SkillName[올려베기,凌云撩剑], ID=6A21, KeyText=F,姿态=20	
#define Danncer_SkillId_Fly_F2	0x6AAD	//	SkillName[폭검,暴烈剑], ID=6AAD, KeyText=F,姿态=20	
#define Danncer_SkillId_Fly_R1	0x69F0	//	SkillName[비상,飞龙追日], ID=69F0, KeyText=R,姿态=20	
#define Danncer_SkillId_Fly_R2	0x69F1	//	SkillName[승천,飞龙在天], ID=69F1, KeyText=未知,姿态=20	
#define Danncer_SkillId_Fly_S	0x69F2	//	SkillName[추락,坠落], ID=69F2, KeyText=S,姿态=20	
#define Danncer_SkillId_Fly_T	0x69F7	//	SkillName[천무,飞龙夺月], ID=69F7, KeyText=T,姿态=20	
#define Danncer_SkillId_G	0x6B34	//	SkillName[파천검륜,破天剑轮], ID=6B34, KeyText=G,姿态=20	
#define Danncer_SkillId_Grab_1	0x6A41	//	SkillName[내다꽂기,千军倒],ID=6A41,KeyText=1	
#define Danncer_SkillId_Grab_2	0x6AA9	//	SkillName[어검징표,御剑缚灵],ID=6AA9,KeyText=2	
#define Danncer_SkillId_Grab_3	0x6A42	//	SkillName[흡수,吸星],ID=6A42,KeyText=3	
#define Danncer_SkillId_Grab_4	0x6A3B	//	SkillName[흡기,吐纳],ID=6A3B,KeyText=4	
#define Danncer_SkillId_Grab_E	0x6A1C	//	SkillName[역습,侧身闪], ID=6A1C, KeyText=E,姿态=22	
#define Danncer_SkillId_Grab_F	0x6AAF	//	SkillName[뇌령,天雷令],ID=6AAF,KeyText=F	
#define Danncer_SkillId_Grab_Q	0x6A1D	//	SkillName[역공,逆攻], ID=6A1D, KeyText=Q,姿态=22	
#define Danncer_SkillId_Grab_SS	0x6A20	//	SkillName[회피,闪避], ID=6A20, KeyText=SS,姿态=20	
#define Danncer_SkillId_Grab_X	0x6A43	//	SkillName[던지기,投掷],ID=6A43,KeyText=X	
#define Danncer_SkillId_Q	0x69EB	//	SkillName[역공,逆攻], ID=69EB, KeyText=Q,姿态=20	
#define Danncer_SkillId_R1	0x69DC	//	SkillName[질풍,疾风], ID=69DC, KeyText=R,姿态=20	
#define Danncer_SkillId_R2	0x69DD	//	SkillName[혈풍,扫剑式], ID=69DD, KeyText=R,姿态=20	
#define Danncer_SkillId_R3	0x69DE	//	SkillName[폭풍,劈剑式], ID=69DE, KeyText=R,姿态=20	
#define Danncer_SkillId_Remote_1_1	0x6AA4	//	SkillName[비연검,飞燕剑], ID=6AA4, KeyText=1,姿态=20	
#define Danncer_SkillId_Remote_1_2	0x6AA5	//	SkillName[비연검,飞燕剑], ID=6AA5, KeyText=1,姿态=20	
#define Danncer_SkillId_Run_R3	0x69FB	//	SkillName[신검합일,风行剑], ID=69FB, KeyText=未知,姿态=20	
#define Danncer_SkillId_SS	0x69EE	//	SkillName[회피,闪避], ID=69EE, KeyText=SS,姿态=20
#define Danncer_SkillId_Sword_2	0x6A13	//	SkillName[돌진,追风剑], ID=6A13, KeyText=2,姿态=22	
#define Danncer_SkillId_Sword_Awaken_F1	0x6B0C	//	SkillName[뇌연섬,雷炎闪], ID=6B0C, KeyText=F,姿态=22	
#define Danncer_SkillId_Sword_Awaken_F2	0x6B0D	//	SkillName[뇌연섬,雷炎闪], ID=6B0D, KeyText=F,姿态=22	
#define Danncer_SkillId_Sword_Awaken_F3	0x6B0E	//	SkillName[뇌연섬,雷炎闪], ID=6B0E, KeyText=F,姿态=22	
#define Danncer_SkillId_Sword_E	0x6A1E	//	SkillName[역습,侧身闪], ID=6A1E, KeyText=E,姿态=22	
#define Danncer_SkillId_Sword_Grab_T	0x6A90	//	SkillName[뇌우비검,雷雨飞剑], ID=6A90, KeyText=T,姿态=22	抓举时的拔剑姿态
#define Danncer_SkillId_Sword_Grab_T1	0x69F4	//	SkillName[번개가르기,雷切], ID=69F4, KeyText=T,姿态=20	
#define Danncer_SkillId_Sword_Q	0x6A1F	//	SkillName[역공,逆攻], ID=6A1F, KeyText=Q,姿态=22	
#define Danncer_SkillId_Sword_R1	0x6B08	//	SkillName[발도,拔剑], ID=6B08, KeyText=R,姿态=22	
#define Danncer_SkillId_Sword_R2	0x6B6C	//	SkillName[쾌검,快剑], ID=6B6C, KeyText=R,姿态=22	
#define Danncer_SkillId_Sword_T	0x6B77	//	SkillName[탄검,弹剑], ID=6B77, KeyText=T,姿态=22	
#define Danncer_SkillId_Sword_Z	0x6AC3	//	SkillName[호위령,护卫灵|鬼令箭], ID=6AC3, KeyText=Z,姿态=22	
#define Danncer_SkillId_T1	0x69E0	//	SkillName[가르기,断水流], ID=69E0, KeyText=T,姿态=20	
#define Danncer_SkillId_T2	0x69E2	//	SkillName[가르기,断水流], ID=69E2, KeyText=T,姿态=20	
#define Danncer_SkillId_T3	0x69E3	//	SkillName[가르기,断水流], ID=69E3, KeyText=T,姿态=20	
#define Danncer_SkillId_T4	0x69E4	//	SkillName[가르기,断水流], ID=69E4, KeyText=T,姿态=20	
#define Danncer_SkillId_Tab	0x69DF	//	SkillName[바람개비,风车], ID=69DF, KeyText=Tab,姿态=20	
#define Danncer_SkillId_Tab_F	0x6A0E	//	SkillName[회오리,旋风], ID=6A0E, KeyText=F,姿态=20	
#define Danncer_SkillId_V	0x6A08	//	SkillName[참월,残月|风月斩], ID=6A08, KeyText=V,姿态=20	
#define Danncer_SkillId_X	0x6B0F	//	SkillName[천둥베기,雷鸣斩], ID=6B0F, KeyText=X,姿态=20	
#define Danncer_SkillId_Z	0x6AA8	//	SkillName[호위령,护卫灵|鬼令箭], ID=6AA8, KeyText=Z,姿态=20	






//拳师 
#define KongFuMaster_SkillId_R1_1	0x5208	//SkillName[정권,正拳], ID=5208, KeyText=R
#define KongFuMaster_SkillId_R1_2	0x5334	//SkillName[정권,正拳], ID=5334, KeyText=R
#define KongFuMaster_SkillId_R1_3	0x5398	//SkillName[정권,正拳], ID=5398, KeyText=R
#define KongFuMaster_SkillId_R2_1	0x5209	//SkillName[강권,强拳], ID=5209, KeyText=R
#define KongFuMaster_SkillId_R2_2	0x5335	//SkillName[강권,强拳], ID=5335, KeyText=R
#define KongFuMaster_SkillId_R2_3	0x5399	//SkillName[강권,强拳], ID=5399, KeyText=R
#define KongFuMaster_SkillId_R3_1	0x520A	//SkillName[선풍각,旋风腿], ID=520A, KeyText=R
#define KongFuMaster_SkillId_R3_2	0x5336	//SkillName[선풍각,旋风腿], ID=5336, KeyText=R
#define KongFuMaster_SkillId_R3_3	0x539A	//SkillName[선풍각,旋风腿], ID=539A, KeyText=R
#define KongFuMaster_SkillId_F1_1	0x5226	//SkillName[백열각,爆裂脚], ID=5226, KeyText=F
#define KongFuMaster_SkillId_F1_2	0x5352	//SkillName[백열각,爆裂脚], ID=5352, KeyText=F
#define KongFuMaster_SkillId_F2_1	0x524E	//SkillName[강타,升龙拳], ID=524E, KeyText=F
#define KongFuMaster_SkillId_F2_2	0x537A	//SkillName[강타,升龙拳], ID=537A, KeyText=F
#define KongFuMaster_SkillId_F2_3	0x54A6	//SkillName[강타,升龙拳], ID=54A6, KeyText=F
#define KongFuMaster_SkillId_F3_1	0x5276	//SkillName[철산고,铁山靠], ID=5276, KeyText=F
#define KongFuMaster_SkillId_T	0x520F	//SkillName[전질보,迅步拳], ID=520F, KeyText=T
#define KongFuMaster_SkillId_T1	0x5307//反击之后	SkillName[진격권,冲锋拳], ID=5307, KeyText=T
#define KongFuMaster_SkillId_T2	0x536B//反击之后?	SkillName[진격권,冲锋拳], ID=536B, KeyText=T
#define KongFuMaster_SkillId_T3	0x5278//抵抗之后	SkillName[,烈焰脚], ID=5278, KeyText=T
#define KongFuMaster_SkillId_T4	0x52BC	//SkillName[올려차기,朝天蹬], ID=52BC, KeyText=T
#define KongFuMaster_SkillId_1	0x520D	//SkillName[반격,反击], ID=520D, KeyText=1
#define KongFuMaster_SkillId_2_1	0x5277	//SkillName[맹호장,猛虎掌], ID=5277, KeyText=2
#define KongFuMaster_SkillId_2_2	0x53A3	//SkillName[맹호장,猛虎掌], ID=53A3, KeyText=2
#define KongFuMaster_SkillId_2_3	0x54CF	//SkillName[맹호장,猛虎掌], ID=54CF, KeyText=2
#define KongFuMaster_SkillId_2_4	0x5534	//SkillName[맹호장,猛虎掌], ID=5534, KeyText=2
#define KongFuMaster_SkillId_3	0x5272	//SkillName[다리걸기,扫堂腿], ID=5272, KeyText=3
#define KongFuMaster_SkillId_3_1	0x5403	//SkillName[열화각,烈火脚], ID=5403, KeyText=3
#define KongFuMaster_SkillId_Grab_3_1	0x53CB	//SkillName[천수장,千手掌], ID=53CB, KeyText=3
#define KongFuMaster_SkillId_Grab_3_2	0x53CC	//SkillName[천수장,千手掌], ID=53CC, KeyText=3
#define KongFuMaster_SkillId_4	0x5274	//SkillName[붕권,冲拳], ID=5274, KeyText=4
#define KongFuMaster_SkillId_41_1	0x533C	//SkillName[붕권,冲拳], ID=533C, KeyText=4
#define KongFuMaster_SkillId_41_2	0x53A0	//SkillName[붕권,冲拳], ID=53A0, KeyText=4
#define KongFuMaster_SkillId_4_1	0x536C//释放后可点穴	SkillName[돌려차기,回旋踢], ID=536C, KeyText=4
#define KongFuMaster_SkillId_Z	0x523F	//SkillName[승룡각,盘龙腿], ID=523F, KeyText=Z
#define KongFuMaster_SkillId_X	0x5299	//SkillName[통배권,通背拳], ID=5299, KeyText=X
#define KongFuMaster_SkillId_X1	0x5361	//SkillName[통배권,通背拳], ID=5361, KeyText=X
#define KongFuMaster_SkillId_X2	0x53C5	//SkillName[통배권,通背拳], ID=53C5, KeyText=X
#define KongFuMaster_SkillId_X3	0x53C6	//SkillName[통배권,通背拳], ID=53C6, KeyText=X
#define KongFuMaster_SkillId_X4	0x53C7	//SkillName[통배권,通背拳], ID=53C7, KeyText=X
#define KongFuMaster_SkillId_X5	0x53C8	//SkillName[통배권,通背拳], ID=53C8, KeyText=X
#define KongFuMaster_SkillId_X_1	0x5473//点穴	SkillName[점혈,点穴], ID=5473, KeyText=X
#define KongFuMaster_SkillId_X_2	0x54D7//点穴	SkillName[점혈,点穴], ID=54D7, KeyText=X
#define KongFuMaster_SkillId_X_3	0x553B//点穴	SkillName[점혈,点穴], ID=553B, KeyText=X
#define KongFuMaster_SkillId_X_4	0x559F//点穴	SkillName[점혈,点穴], ID=559F, KeyText=X
#define KongFuMaster_SkillId_Awaken_X_1 0x55E5//SkillName[각성 점혈,觉醒点穴], ID=55E5, KeyText=X,姿态=4, IsCool=0, IsActive=0
#define KongFuMaster_SkillId_Awaken_X_2 0x55E6//SkillName[각성 점혈,觉醒点穴], ID=55E6, KeyText=X,姿态=4, IsCool=1, IsActive=1
#define KongFuMaster_SkillId_C	0x523D	//SkillName[권무,拳舞], ID=523D, KeyText=C
#define KongFuMaster_SkillId_V	0x5244	//SkillName[산사태,山崩], ID=5244, KeyText=V
#define KongFuMaster_SkillId_Tab	0x527B//跳	SkillName[뛰어찍기,天来跃], ID=527B, KeyText=Tab
#define KongFuMaster_SkillId_Tab1	0x5211//队伍BUFF 小超神	SkillName[투지발산,斗志昂扬], ID=5211, KeyText=Tab
#define KongFuMaster_SkillId_G	0x55DC	//SkillName[파진무,徐光辉], ID=55DC, KeyText=G
#define KongFuMaster_SkillId_B	0x55DD	//SkillName[염황포,炎皇爆], ID=55DD, KeyText=B
#define KongFuMaster_SkillId_Q	0x52D1	//SkillName[횡이동,侧身闪], ID=52D1, KeyText=Q
#define KongFuMaster_SkillId_E	0x52D0	//SkillName[파고들기,幽步], ID=52D0, KeyText=E
#define KongFuMaster_SkillId_SS	0x5273	//SkillName[후방이동,逆风行], ID=5273, KeyText=SS
#define KongFuMaster_SkillId_SS_R	0x5389	//SS后可释放的RSkillName[비염각,飞燕脚], ID=5389, KeyText=R,姿态=4, IsCool=1, IsActive=1
#define KongFuMaster_SkillId_Floor_Tab2	0x52DC	//被冰冻后的TAB
#define KongFuMaster_SkillId_Floor_Tab1	0x52D5	//SkillName[탈출,逃脱], ID=52D5, KeyText=Tab
#define KongFuMaster_SkillId_Floor_Tab	0x52D6	//SkillName[탈출,逃脱], ID=52D6, KeyText=Tab
#define KongFuMaster_SkillId_Floor_1	0x528C	//SkillName[회전차기,回旋踢], ID=528C, KeyText=1
#define KongFuMaster_SkillId_Floor_2	0x528E	//SkillName[반달차기,半月踢], ID=528E, KeyText=2
#define KongFuMaster_SkillId_Floor_F	0x528D	//SkillName[뒤구르기,后滚翻], ID=528D, KeyText=F




//枪手
#define Gunner_SkillId_1							0x5A0A
#define Gunner_SkillId_2							0x5A6E
#define Gunner_SkillId_3							0x5A32
#define Gunner_SkillId_3_F						0x5A1E
#define Gunner_SkillId_4							0x5AB4
#define Gunner_SkillId_4_1						0x5AB5
#define Gunner_SkillId_4_2						0x5AB6
#define Gunner_SkillId_Z							0x5D8E
#define Gunner_SkillId_X							0x5A64
#define Gunner_SkillId_C							0x5A96
#define Gunner_SkillId_V							0x5A50
#define Gunner_SkillId_SS							0x59D8
#define Gunner_SkillId_Q							0x5C8A
#define Gunner_SkillId_E							0x5A28
#define Gunner_SkillId_E_F						0x5A8C
#define Gunner_SkillId_Tab						0x59EC
#define Gunner_SkillId_R							0x59E6
#define Gunner_SkillId_T							0x59F7
#define Gunner_SkillId_F							0x5AC8
#define Gunner_SkillId_Tab_F					0x5BCC
#define Gunner_SkillId_Floor_Tab				0x5B5E
#define Gunner_SkillId_Floor_1					0x5B2C
#define Gunner_SkillId_Floor_F					0x5B7C



#define FarmMode_Priority_High						0x1							// 打怪优先级 - 高
#define FarmMode_Priority_Normal					0x2							// 打怪优先级 - 一般
#define FarmMode_Priority_Low						0x3							// 打怪优先级 - 低(不打)

#define MAX_TELPORT_SIZE							(34 * 512 / 4)

#define SZFILE_NAME_TEAM_SHAREINFO					L"BNS_KOR_Team_Share_Info"
#define SZFILE_NAME_LOGIN_SHAREINFO					L"BNS_KOR_Login_Share_Info"
#define SZFILE_NAME_SHAREDINFO						L"BNS_KOR_Share_Info"
#define MAX_GAME_COUNT								5000
#define MAX_OBJECT_NAME_LEN							32

#define GAMEUI_STR_DELETEPLAYER						L"SelectCharacterPanel"


#define VARIABLE_NAME_STOPTAKEITEM					L"停止打怪捡物"
#define VARIABLE_NAME_CHECKPAUSEPOINT				L"坐标超时"
#define VARIABLE_NAME_AUTOTAKEITEM					L"自动捡物"
#define VARIABLE_NAME_KADAOMODE						L"卡刀模式"
#define VARIABLE_NAME_FOODDIS						L"包子范围"
#define VARIABLE_NAME_FOODPERCENT					L"吃包子"
#define VARIABLE_NAME_FINISH_ALL_MAINTASK			L"做完所有主线"
#define VARIABLE_NAME_TAKENPCTIME					L"捡Npc休眠时间"
#define VARIABLE_NAME_JOINTEAM_TIMEOUT				L"组队超时时间"
#define VARIABLE_NMAE_MIANTASK_TIMEOUT				L"主线任务超时"
#define VARIABLE_NAME_KFFM_BOSS_MODE				L"气宗杀BOSS模式"
#define VARIABLE_NAME_ESCAPE_TIMEOUT				L"逃脱超时时间"
#define VARIABLE_NMAE_ISPUASEHELPTHREAD				L"停止吃药线程"
#define VARIABLE_NMAE_MAINTHREADID					L"主线程ID"
#define VARIABLE_NAME_GETAROUNDMONSTER_DIS			L"AOE范围"
#define VARIABLE_NAME_TASKTIMEOUTID					L"任务超时ID"
#define VARIABLE_NAME_BAIXIAN_STAGE_STEP			L"白仙训练场进度"
#define VARIABLE_NAME_BAIXIAN_DONE					L"白仙训练场完成"
#define VARIABLE_NAME_BAIXIAN_TIMEOUT				L"白仙训练场超时"
#define VARIABLE_NAME_KILLMONSTERR_MOVE_DIS			L"打怪走路距离"
#define VARIABLE_NAME_DISABLE_BECKONCAT				L"禁止召唤猫"
//#define VARIABLE_NAME_TEAMID						L"队伍ID"
#define VARIABLE_NAME_FORCESTOPPOINT				L"强制停止坐标超时"
#define VARIABLE_NAME_SNOWBOSS_FINSKILL				L"雪人大招"
#define VARIABLE_NAME_DISABLE_DELETEPLAYER			L"禁止删除角色"
#define VARIABLE_NAME_DISABLE_SWITCHVIEW			L"禁止切换视角"
#define VARIABLE_NAME_GUARD_TIMEOUTTHREAD			L"超时线程哨兵"
#define VARIABLE_NAME_SWITCHACCOUNT_MODE			L"召唤换号模式"
#define VARIABLE_NAME_WAREHOUSE_MODE				L"仓库模式"
#define VARIABLE_NAME_SUMMONER_LEVEL50_MODE			L"召唤50级模式"
#define VARIABLE_NAME_DELETEACCOUNT					L"删号"
#define VARIABLE_NAME_FLOATING						L"浮空"
#define VARIABLE_NAME_MAIL_MODE						L"一键邮寄选项"
#define VARIABLE_NAME_OPENBOX_SPECIAL				L"特殊开箱子选项"
#define VARIABLE_NMAE_DELETE_ALL_PLAYER				L"删除所有角色"
#define VARIABLE_NMAE_JUDGE_MAIL_TITLE				L"是否判断邮件标题"
#define VARIABLE_NAME_LASTPLAYER_EXCHANGEMODE		L"星币兑换模式最后一个帐号"
#define VARIABLE_NAME_KILLMONSTER_ROUND_MODE		L"圆形清怪模式"
#define VARIABLE_NAME_CURRENT_DAILYTASK				L"当前日常任务"
#define VARIABLE_NAME_DISABLE_UPDATEDAILYTASK		L"禁止更新日常数据"
#define VARIABLE_NAME_DUNGEON_SCHEDULE				L"副本进度"
#define VARIABLE_NAME_ENABLE_Z						L"启用Z"

#define MAIL_TITLE_TEXT								L"123123123"


#define BNSVERSION									L"5.51"

#define ReadDWORD(x)								MyTools::CCharacter::ReadDWORD(x)
#define ReadBYTE(x)									MyTools::CCharacter::ReadBYTE(x)
#define ReadFloat(x)								MyTools::CCharacter::ReadFloat(x)
#define ReadDouble(x)								MyTools::CCharacter::ReadDouble(x)

#define DSIABLE_COPY_AND_ASSIGN(TypeName)	\
	TypeName(CONST TypeName&) = delete;		\
	VOID operator=(CONST TypeName&) = delete;


/************************************************************************/
/* 结构                                                                     */
/************************************************************************/

typedef struct _Point
{
	float X, Z, Y;
	_Point()
	{
		X = Y = Z = 0;
	}
	explicit _Point(float pX, float pY, float pZ)
	{
		X = pX;
		Y = pY;
		Z = pZ;
	}
	explicit _Point(int nX, int nY, int nZ)
	{
		X = static_cast<float>(nX);
		Y = static_cast<float>(nY);
		Z = static_cast<float>(nZ);
	}
	bool operator == (const _Point & p) CONST
	{
		if (p.X == this->X && p.Y == this->Y)
		{
			return true;
		}
		return false;
	}
	bool operator != (const _Point & p) CONST
	{
		if (p.X == this->X && p.Y == this->Y)
		{
			return false;
		}
		return true;
	}
	bool IsZero() CONST
	{
		if (this->X == 0 && this->Y == 0 && this->Z == 0)
		{
			return true;
		}
		return false;
	}
	void ConvertToInt()
	{
		this->X = (float)(int)this->X;
		this->Y = (float)(int)this->Y;
		this->Z = (float)(int)this->Z;
	}
	int GetX() CONST
	{
		return (int)X;
	}
	int GetY() CONST
	{
		return (int)Y;
	}
	int GetZ() CONST
	{
		return (int)Z;
	}
	VOID Clear()
	{
		X = Y = Z = 0;
	}
}Point;

typedef enum _em_GameCmd
{
	em_GameCmd_None,
	em_GameCmd_Start,
	em_GameCmd_Stop,
	em_GameCmd_Exit
}em_GameCmd;

typedef enum _em_Camp
{
	em_Camp_浑天教,
	em_Camp_武林盟
}em_Camp;

typedef enum _em_LoginPlayerStatus
{
	em_LoginPlayer_UnExist,
	em_LoginPlayer_Deleted,
	em_LoginPlayer_Normal
}em_LoginPlayerStatus;

struct LearnSkillContent
{
	DWORD dwSkillId;
	DWORD dwIndex;
};


enum em_Player_Classes
{
	// 力士
	em_Player_Classes_Destroyer = 0x5,
	// 刺客
	em_Player_Classes_Assassin = 0x7,
	// 剑士
	em_Player_Classes_BladeMaster = 0x1,
	// 召唤师
	em_Player_Classes_Summoner = 0x6,
	// 气功
	em_Player_Classes_ForceMaster = 0x3,
	// 拳师
	em_Player_Classes_KongFuMaster = 0x2,
	// 灵剑士
	em_Player_Classes_Danncer = 0x8,
	// 咒术师
	em_Player_Classes_Warlock = 0x9,
	// 气宗
	em_Player_Classes_KongFuForceMaster = 0xA,
	//枪手
	em_Player_Classes_Gunner = 0x4
};

typedef struct _PlayerclassesText
{
	em_Player_Classes	emPlayerClasses;
	std::wstring				wsText;
}PlayerclassesText;

enum em_Player_Pose
{
	// 拳宗
	em_Player_Pose_KongFuMaster = 0x1E,
	// 气宗
	em_Player_Pose_ForceMaster = 0x1F,
	// 刺客
	em_Player_Pose_Assassin_Normal = 0x11,
	//
	em_Player_Pose_Assassin_Hide = 0x12,
	// 拔剑姿态
	em_Player_Pose_BladeMaster = 0x7,
	// 气功冰系
	em_Player_Pose_ForceMaster_Ice = 0x9,
	// 气功火系
	em_Player_Pose_ForceMaster_Fire = 0xA,
};

typedef struct _UnOpenGiftItem
{
	DWORD dwUnknowId;
	DWORD dwItemId;
	DWORD dwItemCount;
	DWORD dwUnknow0;
	std::wstring ItemName;
}UnOpenGiftItem;

// 未已经开启的礼包
typedef struct _UnOpenGiftInfo
{
	DWORD dwBase;
	DWORD dwParm;
	std::wstring wsTitle;
	std::vector<UnOpenGiftItem> vItem;
}UnOpenGiftInfo;

typedef struct _OpenGiftItem
{
	DWORD dwGiftID;
	DWORD dwGiftParm;
	DWORD dwUnknowId;
	DWORD dwItemId;
	DWORD dwCount;
	DWORD dwUnknow[3];
	std::wstring ItemName;
}OpenGiftItem;

typedef struct _OpenGiftInfo
{
	DWORD dwBase;
	DWORD dwParm;
	std::wstring wsTitle;
	std::vector<OpenGiftItem> vItem;
}OpenGiftInfo;

struct AuctionItem
{
	DWORD			dwItemId;
	DWORD			dwBase;
	std::wstring	wsItemName;
};

struct AuctionContent
{
	std::vector<AuctionItem>	VecAuctionItem;
	DWORD						Index;

	CONST AuctionItem& GetCurrentAuctionItem() CONST
	{
		return VecAuctionItem.at(Index);
	}
};

typedef enum _em_Auction_Quality
{
	em_Auction_Quality_KitKat = 0x3,	// 奇巧
	em_Auction_Quality_Pristine = 0x4,  // 精粹
	em_Auction_Quality_Extreme = 0x5	// 至尊
}em_Auction_Quality;

typedef struct _City_Src
{
	DWORD dwId;
	WCHAR wszName[32];
}City_Src;

typedef enum _em_WarStatus
{
	em_WarStatus_None = 0x1,		// 和平状态,非战斗状态
	em_WarStatus_Floor = 0x2,		// 倒地状态
	em_WarStatus_Hide = 0x3,		// 隐身状态
	em_WarStatus_Dying = 0x4,		// 濒死状态
	em_WarStatus_Healing = 0x5		// 疗伤状态
}em_WarStatus;

enum em_FightStatus
{
	em_FightStatus_Fall = 0xD, // 倒地状态
	em_FightStatus_Dizzy = 0xB, // 眩晕状态
	em_FightStatus_Weak = 0x12, // 虚弱状态
	em_FightStatus_Snatch = 0x30, // 抓举状态
	em_FightStatus_Other = 0x0
};

struct TaskInfoParm
{
	DWORD dwTaskId;
	DWORD dwScheduleId;

	TaskInfoParm()
	{
		Release();
	}

	TaskInfoParm(_In_ DWORD dwTaskId_, _In_ DWORD dwScheduleId_)
	{
		dwTaskId = dwTaskId_;
		dwScheduleId = dwScheduleId_;
	}

	bool operator == (CONST TaskInfoParm& TaskParm) CONST
	{
		return this->dwTaskId == TaskParm.dwTaskId && this->dwScheduleId == TaskParm.dwScheduleId;
	}

		bool operator != (CONST TaskInfoParm& TaskParm) CONST
	{
		return this->dwTaskId != TaskParm.dwTaskId || this->dwScheduleId != TaskParm.dwScheduleId;
	}
	
	void Release()
	{
		dwTaskId = dwScheduleId = NULL;
	}
};

struct TaskContentMsg
{
	TaskInfoParm Ti;
	DWORD		 dwKillCount;

	TaskContentMsg()
	{
		dwKillCount = 0;
	}
};


typedef std::function<VOID(VOID)> MainThreadFunPtr;

//////////邮寄////////////////////////////////////////////////////////////////
#pragma pack(1)
typedef struct _MailItem
{
	DWORD dwId;
	DWORD dwZero1;
	DWORD dwCount;
	DWORD dwZero2;
	_MailItem()
	{
		dwId = dwZero1 = dwZero2 = dwCount = NULL;
	}
}MailItem;
#pragma pack()

#pragma pack(1)
struct MAIL_TEXT_GAME
{
	union
	{
		wchar_t* pwStr;
		wchar_t wchArr[0x8];
	};
	int count;
	int size;
	int : 4;
};
#pragma pack()

#pragma pack(1)
typedef struct _MailParm_Src
{
	DWORD dwZero1;
	MAIL_TEXT_GAME Player;
	MAIL_TEXT_GAME Title;
	MAIL_TEXT_GAME Content;
	DWORD dwZeroArray[4];
	std::vector<MailItem> vlst;
	DWORD dwZero2;
	DWORD dwMoney;
	DWORD dwZero3;
	DWORD dwParm;
}MailParm_Src;
#pragma pack()

typedef struct _FixPointInfo
{
	Point FixPoint;
	float fDis;
	_FixPointInfo(_In_ CONST Point& FixPoint_, _In_ float fDis_) : FixPoint(FixPoint_), fDis(fDis_)
	{

	}
	_FixPointInfo() : FixPoint(), fDis(0)
	{

	}
	CONST Point& GetPoint() CONST
	{
		return FixPoint;
	}
}FixPointInfo;

typedef struct _TalkMsg_Src
{
	DWORD dwBase;
	std::wstring wsText;
	_TalkMsg_Src()
	{
		dwBase = NULL;
		wsText = L"";
	}
}TalkMsg_Src;

////Hook怪物技能//////////////////////////////////////////////////////////////////////
typedef struct _MonsterSkillInfo
{
	DWORD	dwSkillId;
	DWORD	dwTick;
	UINT	uCount;
	_MonsterSkillInfo()
	{
		Release();
	}
	VOID Release()
	{
		dwSkillId = dwTick = NULL;
		uCount = NULL;
	}
}MonsterSkillInfo;

/////BUFF/////////////////////////////////////////////////////////////////////
typedef struct _GAMEOBJ_BUFF
{
	DWORD dwBuffBase;
	DWORD dwBuffId;
	DWORD dwBuffCount;
	DWORD dwTargetId;
}GAMEOBJ_BUFF;

/////交任务/////////////////////////////////////////////////////////////////////
struct SumitTask_Src
{
	DWORD dwScheduleId;
	DWORD dwTaskId;
	DWORD dwParm3;
	DWORD dwParm4;
	DWORD dwParm5;
	DWORD dwObjId;
	DWORD dwObjIdParm;
	DWORD dwItemIndex;

	SumitTask_Src()
	{
		dwScheduleId = dwTaskId = dwParm3 = dwParm4 = dwParm5 = dwObjId = dwObjIdParm = dwItemIndex = NULL;
	}
	SumitTask_Src(_In_ DWORD dwValue1, _In_ DWORD dwValue2, _In_ DWORD dwValue3, _In_ DWORD dwValue4, _In_ DWORD dwValue5, _In_ DWORD dwValue6, _In_ DWORD dwValue7, _In_ DWORD dwValue8)
	{
		dwScheduleId = dwValue1;
		dwTaskId = dwValue2;
		dwParm3 = dwValue3;
		dwParm4 = dwValue4;
		dwParm5 = dwValue5;
		dwObjId = dwValue6;
		dwObjIdParm = dwValue7;
		dwItemIndex = dwValue8;
	}
};

struct SumitBLTask_Src
{
	DWORD dwNpcId;
	DWORD dwNpcIdParm;
	DWORD dwTaskId;
	DWORD dwParm4;
	DWORD dwParm5;

	SumitBLTask_Src()
	{
		dwNpcId = dwNpcIdParm = dwTaskId = dwParm4 = dwParm5 = NULL;
	}

	SumitBLTask_Src(_In_ DWORD dwValue1, _In_ DWORD dwValue2, _In_ DWORD dwValue3, _In_ DWORD dwValue4, _In_ DWORD dwValue5)
	{
		dwNpcId = dwValue1;
		dwNpcIdParm = dwValue2;
		dwTaskId = dwValue3;
		dwParm4 = dwValue4;
		dwParm5 = dwValue5;
	}
};

struct TeamMemberSchedule
{
	std::wstring		wsAccountName;
	std::wstring		wsPlayerName;
	DWORD				dwPlayerId;
	std::wstring		wsScheduleName;
	UINT				uScheduleIndex;
	DWORD				dwTaskId;
	DWORD				dwScheduleId;

	TeamMemberSchedule()
	{
		dwPlayerId = dwTaskId = dwScheduleId = NULL;
		uScheduleIndex = NULL;
	}

	bool operator < (CONST TeamMemberSchedule& itm) CONST
	{
		return itm.uScheduleIndex < this->uScheduleIndex;
	}
};

struct TeamDungeonSchedule
{
	std::vector<TeamMemberSchedule>	VecMember;
	DWORD						dwTeamId;

	BOOL FindByAccountName(_In_ CONST std::wstring& wsAccountName, _Out_ TeamMemberSchedule& TeamMemberSchedule_) CONST
	{
		return MyTools::CLPublic::Vec_find_if(VecMember, &TeamMemberSchedule_, [&wsAccountName](CONST TeamMemberSchedule& itm){return itm.wsAccountName == wsAccountName; });
	}

	BOOL FindByMaxScheduleIndex(_Out_ TeamMemberSchedule& TeamMemberSchedule_)
	{
		if (VecMember.size() == 0)
			return FALSE;

		sort(VecMember.begin(), VecMember.end());
		MyTools::CLPublic::SetPtr(&TeamMemberSchedule_, VecMember.at(0));
		return TRUE;
	}

	BOOL IsSameForScheduleIndex(_In_ UINT uScheduleIndex, _Out_ TeamMemberSchedule* pTeamMemberSchedule) CONST
	{
		return !MyTools::CLPublic::Vec_find_if(VecMember, pTeamMemberSchedule, [&uScheduleIndex](CONST TeamMemberSchedule& itm){return  itm.uScheduleIndex != uScheduleIndex; });
	}

	VOID Clear()
	{
		VecMember.clear();
		dwTeamId = NULL;
	}
};

typedef struct _ClientTeamTaskInfo
{
	std::wstring wsPlayerName;
	DWORD dwTaskId;
	DWORD dwScheduleId;
}ClientTeamTaskInfo;

enum em_Skill_Type
{
	em_Skill_Type_Person,
	em_Skill_Type_Cat
};

//////Socket////////////////////////////////////////////////////////////////////


///////////////帐号///////////////////////////////////////////////////////////
typedef struct _Account_Info
{
	WCHAR szUserName[64];			//帐号
	WCHAR szPassWord[32];			//密码
	DWORD dwServerId;				//服务器
	DWORD dwOrderIndex;				//角色列表
	WCHAR szPlayerName[32];			//玩家名称
	BOOL bAlive;					//活动标识，定期检测一次此标识，如果没有设置为TRUE表示目标进程出问题了。
	BOOL bLogin;					//登录完毕=FALSE
	WCHAR szCardNo[64];				//卡号
	em_GameCmd GameCmd;				// 游戏状态
	DWORD dwSortIndex;				// 排列序号
	DWORD dwClasses;				// 职业
	LPCVOID  lpAccountToken;
}ACCOUNT_INFO, *PACCOUNT_INFO;

typedef enum _em_Pk_Schedule
{
	em_Pk_Schedule_None,			// NULL
	em_Pk_Schedule_TaskUpgrade,		// 主线升级
	em_Pk_Schedule_ActiveCard,		// 激活卡号
	em_Pk_Schedule_PK,				// PK
	em_Pk_Schedule_AfterPK			// 待用
}em_Pk_Schedule;

typedef struct _SwitchAccount_PK_Setting
{
	BOOL bDone;						// 是否完成了所有的PK换号
	em_Pk_Schedule	emPkSchedule;
}SwitchAccount_PK_Setting;

typedef enum _em_Close_Result
{
	em_Close_Result_None,			// 正常状态
	em_Close_Result_ReStart,		// 重新上号
	em_Close_Result_LoseConnect,	// 掉线
	em_Close_Result_FixPoint,		// 5分钟
	em_Close_Result_CloseConsole,	// 关闭控制台
	em_Close_Result_UnExistTime, // 点卡没了
	em_Close_Result_Trading, // 交易模式密码错误
}em_Close_Result;

enum em_Login_Step
{
	em_Login_Step_InputAccount,
	em_Login_Step_Agreement,
	em_Login_Step_ChoicePlayer,
	em_Login_Step_Queue,
	em_Login_Step_Finish
};

enum em_ForceMail_Type
{
	em_ForceMail_Type_None,
	em_ForceMail_Type_DeleteAccount,
	em_ForceMail_Type_Stop
};

struct ClientCpuUsage
{
	UINT uCountForCPUUsage;									// CPU使用率=0的连续次数
	LONGLONG llLastTime;
	LONGLONG llLastSysTime;
	ClientCpuUsage() : uCountForCPUUsage(NULL), llLastSysTime(NULL), llLastTime(NULL) {}
};

typedef struct _AccountRunStatus
{
	BOOL bDone;												// 是否刷完了这个号
	BOOL bExist;											// 该帐号已经被占用了
	BOOL bLogining;											// 是否登录中
	BOOL bClose;											// 是否强制关闭该帐号
	em_Close_Result	emCloseResult;							// 关闭理由
	BOOL bLogined;											// 是否登录过
	BOOL bDelete;											// 是否要删除该帐号->Account.txt
	UINT uLoginFaildCount;									// 登录失败次数
	UINT uCreatePlayerFaildCount;							// 创建角色失败次数
	UINT uUnExistGameHwndCount;								// 查找窗口失败次数
	BOOL bFrozen;											// 冻结
	ULONGLONG ulOnLineTick;									// 在线时长
	BOOL bOccupy;											// 是否被占用
	em_Login_Step emLoginStep;								// 登录步骤
	em_ForceMail_Type emForceMail;							// 强制收邮件
	ClientCpuUsage CpuUsage;								// CPU使用率
	BOOL bStopRespond;										// 未响应
	DWORD dwStopRespondCount;								// 未响应次数
	BOOL bRunNcLanucher;					// 已经启动了登录器
}AccountRunStatus;

enum class em_ConsoleLog
{
	// 状态
	em_ConsoleLog_Status = 0x1,
	// 日志
	em_ConsoleLog_Content = 0x2,
	// 上次重登原因
	em_ConsoleLog_Result = 0x4,
	//
	em_ConsoleLog_BugReport = 0x5
};

typedef struct _Account_Info_GAME
{
	ACCOUNT_INFO             MyAcount_Info;								        // 帐号信息
	DWORD                    dwPid;											    // 进程ID
	HWND                     hGameWnd;											// 窗口句柄
	AccountRunStatus         AccountStatus;							            // 帐号状态
	SwitchAccount_PK_Setting PkSetting;						                    // PK状态

	LPCWSTR GetAccName()
	{
		return this->GetAccountInfo()->szUserName;
	}
	DWORD& GetOrderIndex()
	{
		return this->GetAccountInfo()->dwOrderIndex;
	}
	UINT& GetLoginFaildCount()
	{
		return this->AccountStatus.uLoginFaildCount;
	}
	PACCOUNT_INFO GetAccountInfo()
	{
		return &this->MyAcount_Info;
	}
	//LPWSTR GetCloseResultText()
	//{
	//	return this->AccountStatus.wszCloseResultText;
	//}
	/*VOID SetCloseResultText(_In_ LPCWSTR pwszText)
	{
		AccountStatus.bOccupy = TRUE;
		MyTools::CCharacter::wstrcpy_my(AccountStatus.wszCloseResultText, pwszText, _countof(AccountStatus.wszCloseResultText) - 1);
		AccountStatus.bOccupy = FALSE;
	}*/
	VOID Print();
}ACCOUNT_INFO_GAME, *PACCOUNT_INFO_GAME;



struct Share_Global_MailConfig
{
	WCHAR wszScriptName[32];								// 脚本名称
	WCHAR wszPlayerName[16][16];							// 邮寄角色名
	UINT  uCount;											// 邮寄角色名数量
	UINT  uIndex;											// 读取到哪个邮寄偏移了
};

struct Share_Global_Warehouse
{
	WCHAR wszPlayerName[16];
	WCHAR wszNpcName[16];
	WCHAR wszCityName[16];
	DWORD dwChannel;
};

struct Share_Global_WarehouseConfig
{
	UINT uCount;
	Share_Global_Warehouse Warehouse[16];
	UINT uIndex;
	WCHAR wszScriptName[32];								// 脚本名称
};

struct LoginLanucherConfig
{
	WCHAR wszNcLauncherPath[MAX_PATH];						// 游戏路径
	WCHAR wszAccountName[64];								// 登陆的账号
	WCHAR wszAccountPass[32];								// 登陆的密码

	DWORD dwLoginControlHwndTimeout;						// 等待获取登陆控件超时
	DWORD dwLoginTimeout;											// 登陆超时
	DWORD dwDisableLoginButtonTimeout;						// 禁用登陆按钮的超时
};

typedef struct _Share_Global_Account
{
	BOOL bRun;												// 是否开启全局设置
	DWORD dwAreaId;											// 大区Id
	WCHAR wszScriptName[32];								// 脚本名称
	WCHAR wszTypeName[32];									// 登录类型

	Share_Global_MailConfig Mail;							// 邮寄
	Share_Global_WarehouseConfig Warehouse;					// 仓库
	LoginLanucherConfig		LoginLanucher;					// 游戏登陆

	CHAR  szServerIp[32];									// 服务器的IP
	BOOL IsClose_By_Banned;									// 封号后是否关闭
	BOOL IsPK;												// 是否PK
	BOOL IsDailyTask;										// 是否做日常
	DWORD dwLoginCountOnce;									// 一次性登录数量
	DWORD dwLoginModeSleep;									// 登录后休眠的秒
	BOOL IsDeletePlayer;									// 是否删号
	WCHAR wszGroupTitle[32];								// 组队要用的标题
	BOOL IsNoModule;										// 是否无模块化
	BOOL bAccelerate;										// 加速
}Share_Global_Account;

typedef enum _em_WebLogin
{
	em_WebLogin_None,
	em_WebLogin_GameLogin,
	em_WebLogin_ActiveCard
}em_WebLogin;

struct BnsConsoleContent
{
	WCHAR szConsolePath[MAX_PATH];							// 控制台路径
	Share_Global_Account GlobalAccount;						// 全局设置
	HWND  hConsoleWnd;
};



typedef struct _Shared_Info
{
	BnsConsoleContent ConsoleContent;
	WCHAR wszGamePath[MAX_PATH];							// 游戏路径
	int nAccountCount;										// Size
	ACCOUNT_INFO_GAME arrGameInfo[MAX_GAME_COUNT];			// 数组
	

	PACCOUNT_INFO_GAME ExistEmptyArrGameInfo()
	{
		auto itr = std::find_if(std::begin(arrGameInfo), std::end(arrGameInfo), [](ACCOUNT_INFO_GAME& AccountGame){
			return !AccountGame.AccountStatus.bExist;
		});

		if (itr == std::end(arrGameInfo))
			return nullptr;
		return itr;
	}
	PACCOUNT_INFO_GAME GetCurrentAccountGame(_In_ DWORD dwPid)
	{
		auto itr = std::find_if(std::begin(arrGameInfo), std::end(arrGameInfo), [&dwPid](ACCOUNT_INFO_GAME& AccountGame){
			return AccountGame.dwPid == dwPid;
		});

		if (itr == std::end(arrGameInfo))
			return nullptr;

		return itr;
	}
}SHARED_INFO, *PSHARED_INFO;


#endif