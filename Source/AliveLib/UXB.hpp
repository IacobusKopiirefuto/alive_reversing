#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void UXB_ForceLink();

struct Path_UXB
{
    Path_TLV field_0_mBase;
    __int16 field_10_num_patterns;
    __int16 field_12_pattern;
    __int16 field_14_scale;
    __int16 field_16_state;
    int field_18_disabled_resources;
};

enum UXB_Flags_1C8
{
    e1C8_Bit1 = 0x1,
    e1C8_Bit2_IsRed = 0x2,
};

class BaseBomb : BaseAnimatedWithPhysicsGameObject
{
public:
    /*virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;*/
    /*virtual void VDestructor(signed int flags) override; */

    EXPORT BaseBomb * ctor_423E70(FP x, FP y, int unused, FP scale);

public:

    BYTE field_E4[16];
    FP field_f4_scale;
};
ALIVE_ASSERT_SIZEOF(BaseBomb, 0xF8);

class UXB : public BaseAliveGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    /*virtual void VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void vnullsub_4DC0F0() override;*/

    EXPORT UXB * ctor_4DE9A0(Path_UXB *params, TlvItemInfoUnion itemInfo);
    EXPORT void Update_4DF030();
    EXPORT void Render_4DF3D0(int **pOt);
    EXPORT void InitBlinkAnim_4DEED0(AnimationEx *pAnimation);
    EXPORT void PlaySFX_4DE930(unsigned __int8 sfxIdx);
    EXPORT signed int sub_4DF630();

    EXPORT static int CC CreateFromSaveState_4DFAE0(const BYTE*);
public:
    BYTE gap116[2];
    WORD field_118;
    WORD field_11A;
    WORD field_11C_disabled_resources;
    BYTE gap11E[2];
    TlvItemInfoUnion field_120_tlv;
    DWORD field_124_next_state_frame;
    AnimationEx field_128_animation;
    WORD field_1C0_pattern_length;
    WORD field_1C2_pattern_index;
    WORD field_1C4_pattern;
    WORD field_1C6_red_blink_count;
    BitField16<UXB_Flags_1C8> field_1C8_flags;
    __int16 field_1CA;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1CC);