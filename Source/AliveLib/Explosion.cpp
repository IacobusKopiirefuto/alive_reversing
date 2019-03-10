#include "stdafx.h"
#include "Explosion.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Map.hpp"
#include "Particle.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Gibs.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5C1BB6, short, word_5C1BB6, 0);

struct Path_Slig : public Path_TLV
{
    __int16 field_10_scale;
    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : __int16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        RunOffScreen_4 = 4,
        GameEnder_5 = 5,
    };
    StartState field_12_start_state;
    __int16 field_14_pause_time;
    __int16 field_16_pause_left_min;
    __int16 field_18_pause_left_max;
    __int16 field_1A_pause_right_min;
    __int16 field_1C_pause_right_max;
    __int16 field_1E_chal_number;
    __int16 field_20_chal_timer;
    __int16 field_22_num_times_to_shoot;
    __int16 field_24_unknown;
    __int16 field_26_code1;
    __int16 field_28_code2;
    __int16 field_2A_chase_abe;
    __int16 field_2C_start_direction;
    __int16 field_2E_panic_timeout;
    __int16 field_30_num_panic_sounds;
    __int16 field_32_panic_sound_timeout;
    __int16 field_34_stop_chase_delay;
    __int16 field_36_time_to_wait_before_chase;
    __int16 field_38_slig_id;
    __int16 field_3A_listen_time;
    __int16 field_3C_percent_say_what;
    __int16 field_3E_percent_beat_mud;
    __int16 field_40_talk_to_abe;
    __int16 field_42_dont_shoot;
    __int16 field_44_stay_awake;
    __int16 field_46_disable_resources;
    __int16 field_48_noise_wake_up_distance;
    __int16 field_4A_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x4C);

Explosion* Explosion::ctor_4A1200(FP xpos, FP ypos, FP scale, __int16 bSmall)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546CB8);
    field_4_typeId = Types::eExplosion_109;

    field_F4_bSmall = bSmall;
    if (field_F4_bSmall)
    {
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSmallExplo2ResID);
        Animation_Init_424E10(14108, 99, 46, ppRes, 1, 1);
    }
    else
    {
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
        Animation_Init_424E10(51156, 202, 91, ppRes, 1, 1);
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_20_animation.field_B_render_mode = 1;
    field_F8_scale = scale;
    field_D6_scale = scale == FP_FromInteger(1);
    field_CC_sprite_scale = scale * FP_FromInteger(2);

    if (field_F4_bSmall)
    {
        field_FC = scale * FP_FromDouble(0.5);
    }
    else
    {
        field_FC = scale;
    }

    field_DC_bApplyShadows &= ~1u;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    auto pScreenShake = alive_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(word_5C1BB6 ? 0 : 1, field_F4_bSmall);
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * field_FC);
    rect.y = FP_GetExponent(FP_FromInteger(10) * field_FC);
    rect.w = FP_GetExponent(FP_FromInteger(-10) * field_FC);
    rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC);

    DealBlastDamage_4A1BD0(&rect);

    SND_SEQ_PlaySeq_4CA960(14, 1, 1);

    return this;
}

BaseGameObject* Explosion::VDestructor(signed int flags)
{
    return vdtor_4A14C0(flags);
}

void Explosion::VUpdate()
{
    vUpdate_4A1510();
}

void Explosion::VScreenChanged()
{
    vScreenChanged_4A1EE0();
}

void Explosion::DealBlastDamage_4A1BD0(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects_5C1B7C)
    {
        return;
    }

    PSX_RECT expandedRect = {};
    expandedRect.x = std::min(pRect->x, pRect->w);
    expandedRect.w = std::max(pRect->x, pRect->w);

    expandedRect.y = std::min(pRect->y, pRect->h);
    expandedRect.h = std::max(pRect->y, pRect->h);

    expandedRect.x += FP_GetExponent(field_B8_xpos);
    expandedRect.w += FP_GetExponent(field_B8_xpos);

    expandedRect.y += FP_GetExponent(field_BC_ypos);
    expandedRect.h += FP_GetExponent(field_BC_ypos);

    for (int idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
        {
            PSX_RECT boundRect = {};
            pObj->vGetBoundingRect_424FD0(&boundRect, 1);

            if (PSX_Rects_overlap_no_adjustment(&boundRect, &expandedRect) && /*boundRect.x <= expandedRect.w &&
                                                                              boundRect.w >= expandedRect.x &&
                                                                              boundRect.h >= expandedRect.y &&
                                                                              boundRect.y <= expandedRect.h &&*/
                field_D6_scale == pObj->field_D6_scale)
            {
                pObj->VTakeDamage_408730(this);
            }
        }
    }

    auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        expandedRect.x,
        expandedRect.y,
        expandedRect.w,
        expandedRect.h,
        TlvTypes::Slig_15));

    if (pTlv)
    {
        if (!pTlv->field_0_flags.Get(TLV_Flags::eBit2_Unknown) && pTlv->field_12_start_state == Path_Slig::StartState::Sleeping_2)
        {
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);

            const signed __int16 direction = gMap_5C3030.sub_4811A0(
                gMap_5C3030.sCurrentLevelId_5C3030,
                gMap_5C3030.sCurrentPathId_5C3032,
                FP_FromInteger(pTlv->field_8_top_left.field_0_x),
                FP_FromInteger(pTlv->field_8_top_left.field_2_y));

            if (direction == 3)
            {
                auto pGibs = alive_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_40FB40(1, field_B8_xpos + FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                }
            }
            else if (direction == 4)
            {
                auto pGibs = alive_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_40FB40(1, field_B8_xpos - FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                }
            }

            Start_Exploision_sounds_4CBA70(direction, 0);
        }
    }
}

void Explosion::vUpdate_4A1510()
{
    Event_Broadcast_422BC0(kEventShooting, this);
    Event_Broadcast_422BC0(kEventLoudNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (field_20_animation.field_92_current_frame)
    {
    case 2:
        rect.x = FP_GetExponent(FP_FromInteger(-20) * field_FC);
        rect.w = FP_GetExponent(FP_FromInteger(20) * field_FC);
        rect.y = FP_GetExponent(FP_FromInteger(-20) * field_FC);
        rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC);
        DealBlastDamage_4A1BD0(&rect);
        break;

    case 3:
    {
        auto pParticleBurst = alive_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_41CF50(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 6 : 20, field_F8_scale, 3, field_F4_bSmall ? 11 : 13);
        }

        auto pFlash = alive_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 3u, 1);
        }
    }
    break;

    case 4:
    {
        auto pFlash = alive_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 1, 1);
        }
        rect.x = FP_GetExponent(FP_FromInteger(-38) * field_FC);
        rect.w = FP_GetExponent(FP_FromInteger(38) * field_FC);
        rect.y = FP_GetExponent(FP_FromInteger(-38) * field_FC);
        rect.h = FP_GetExponent(FP_FromInteger(19) * field_FC);
        DealBlastDamage_4A1BD0(&rect);
    }
    break;

    case 6:
        rect.x = FP_GetExponent(FP_FromInteger(-60) * field_FC);
        rect.w = FP_GetExponent(FP_FromInteger(60) * field_FC);
        rect.y = FP_GetExponent(FP_FromInteger(-60) * field_FC);
        rect.h = FP_GetExponent(FP_FromInteger(30) * field_FC);
        DealBlastDamage_4A1BD0(&rect);
        break;

    case 8:
    {
        auto pParticleBurst = alive_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_41CF50(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 6 : 20, field_F8_scale, 3, field_F4_bSmall ? 11 : 13);
        }

        auto pFlash = alive_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 3u, 1);
        }
    }
    break;

    default:
        break;
    }

    if (field_20_animation.field_92_current_frame > 9)
    {
        if (field_F4_bSmall)
        {
            field_CC_sprite_scale -= FP_FromDouble(0.065);
        }
        else
        {
            field_CC_sprite_scale -= FP_FromDouble(0.2);
        }
    }

    if (field_20_animation.field_92_current_frame == 1)
    {
        BYTE** ppRes = field_F4_bSmall ?
            Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSmallExplo2ResID) :
            Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
        if (ppRes)
        {
            auto pParticle = alive_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_4CC4C0(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 14108 : 51156,
                    202, // Same size for both for some reason
                    91,
                    ppRes);

                if (pParticle->field_6_flags.Get(BaseGameObject::eListAddFailed))
                {
                    pParticle->field_6_flags.Set(BaseGameObject::eDead);
                }

                pParticle->field_DC_bApplyShadows &= ~1u;
                pParticle->field_20_animation.field_B_render_mode = 1;

                if (field_20_animation.field_92_current_frame == 3)
                {
                    pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                    pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.5);
                }
                else
                {
                    pParticle->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                    pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.25);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Explosion::vScreenChanged_4A1EE0()
{
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Explosion::dtor_4A14F0()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Explosion* Explosion::vdtor_4A14C0(signed int flags)
{
    dtor_4A14F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}