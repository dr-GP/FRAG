#ifndef GUARD_DATA_H
#define GUARD_DATA_H

#include "global.h"
#include "constants/trainers.h"

#define SPECIES_SHINY_TAG 500
#define TRAINER_ENCOUNTER_MUSIC(trainer)((gTrainers[trainer].encounterMusic_gender & 0x7F))

struct MonCoords
{
    // This would use a bitfield, but some function
    // uses it as a u8 and casting won't match.
    u8 size; // u8 width:4, height:4;
    u8 y_offset;
};

#define MON_COORDS_SIZE(width, height)(DIV_ROUND_UP(width, 8) << 4 | DIV_ROUND_UP(height, 8))
#define GET_MON_COORDS_WIDTH(size)((size >> 4) * 8)
#define GET_MON_COORDS_HEIGHT(size)((size & 0xF) * 8)

// extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
// extern const u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern const u8 gTrainerClassNames[][13];

static inline u16 SanitizeTrainerId(u16 trainerId)
{
    if (trainerId >= NUM_TRAINERS)
        return TRAINER_NONE;
    return trainerId;
}

struct TypeInfo
{
    u8 name[TYPE_NAME_LENGTH + 1];
    u8 generic[17];
    u8 palette;
    u16 zMove;
    u16 maxMove;
    //u16 enhanceItem;
    //u16 berry;
    //u16 gem;
    //u16 plate;
    //u16 memory;
    //u16 zCrystal;
    //u16 teraShard;
    //u16 arceusForm;
};

// extern const struct MonCoords gMonFrontPicCoords[];
// extern const struct CompressedSpriteSheet gMonFrontPicTable[];
// extern const struct MonCoords gMonBackPicCoords[];
// extern const struct CompressedSpriteSheet gMonBackPicTable[];
// extern const struct CompressedSpritePalette gMonPaletteTable[];
// extern const struct CompressedSpritePalette gMonShinyPaletteTable[];
extern const union AnimCmd *const *const gTrainerFrontAnimsPtrTable[];
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct CompressedSpriteSheet gTrainerBackPicTable[];
extern const struct CompressedSpritePalette gTrainerFrontPicPaletteTable[];
extern const union AnimCmd *const *const gTrainerBackAnimsPtrTable[];
extern const struct MonCoords gTrainerBackPicCoords[];
extern const struct CompressedSpritePalette gTrainerBackPicPaletteTable[];

extern const struct CompressedSpriteSheet gSpriteSheet_EnemyShadow;
extern const struct SpriteTemplate gSpriteTemplate_EnemyShadow;

extern const u8 *const gBattleAnims_General[];
extern const u8 *const gBattleAnims_Special[];

extern const union AnimCmd *const gAnims_MonPic[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpritePlayerSide[];
extern const union AffineAnimCmd *const gAffineAnims_BattleSpriteOpponentSide[];
extern const struct SpriteFrameImage gBattlerPicTable_PlayerLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentLeft[];
extern const struct SpriteFrameImage gBattlerPicTable_PlayerRight[];
extern const struct SpriteFrameImage gBattlerPicTable_OpponentRight[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Red[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Leaf[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Pokedude[];
extern const struct SpriteFrameImage gTrainerBackPicTable_OldMan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSBrendan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSMay[];


extern const union AnimCmd sAnim_GeneralFrame0[];

// static inline const struct TrainerMon *GetTrainerPartyFromId(u16 trainerId)
// {
//     return gTrainers[SanitizeTrainerId(trainerId)].party;
// }

#endif // GUARD_DATA_H
