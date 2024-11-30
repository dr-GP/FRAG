#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "task.h"
#include "blend_palette.h"
#include "text_window.h"
#include "menu.h"
#include "help_system.h"
#include "event_scripts.h"
#include "scanline_effect.h"
#include "pokeball.h"
#include "naming_screen.h"
#include "math_util.h"
#include "overworld.h"
#include "random.h"
#include "data.h"
#include "constants/songs.h"

#define INTRO_SPECIES SPECIES_PIKACHU

const u8 gText_DefaultPlayerName[7] = _("ASH");
const u8 gText_DefaultRivalName[7] = _("GARY");

enum
{
    WIN_INTRO_TEXTBOX,
    NUM_INTRO_WINDOWS,
};

struct OakSpeechResources
{
    void *oakSpeechBackgroundTiles;
    void *trainerPicTilemap;
    void *pikachuIntroTilemap;
    void *unused1;
    u16 hasPlayerBeenNamed;
    u16 currentPage;
    u16 windowIds[NUM_INTRO_WINDOWS];
    u8 textColor[3];
    u8 textSpeed;
    u8 unused2[0x1800];
    u8 bg2TilemapBuffer[0x400];
    u8 bg1TilemapBuffer[0x800];
};

static EWRAM_DATA struct OakSpeechResources *sOakSpeechResources = NULL;

static void Task_NewGameScene(u8);

static void Task_OakSpeech_Init(u8);
static void Task_OakSpeech_WelcomeToTheWorld(u8);
static void Task_OakSpeech_ThisWorld(u8);
static void Task_OakSpeech_ReleasePikachuFromPokeBall(u8);
static void Task_OakSpeech_IsInhabitedFarAndWide(u8);
static void Task_OakSpeech_IStudyPokemon(u8);
static void Task_OakSpeech_ReturnPikachuToPokeBall(u8);
static void Task_OakSpeech_Clear(u8);
static void Task_OakSpeech_FadeOutOak(u8);
static void Task_OakSpeech_ShowPlayersPic(u8);
static void Task_OakSpeech_LetsGo(u8);
static void Task_OakSpeech_FadeOutBGM(u8);
static void Task_OakSpeech_SetUpExitAnimation(u8);
static void Task_OakSpeech_SetUpShrinkPlayerPic(u8);
static void Task_OakSpeech_ShrinkPlayerPic(u8);
static void Task_OakSpeech_SetUpDestroyPlatformSprites(u8);
static void Task_OakSpeech_DestroyPlatformSprites(u8);
static void Task_OakSpeech_SetUpFadePlayerPicWhite(u8);
static void Task_OakSpeech_FadePlayerPicWhite(u8);
static void Task_OakSpeech_FadePlayerPicToBlack(u8);
static void Task_OakSpeech_WaitForFade(u8);
static void Task_OakSpeech_FreeResources(u8);

static void CreatePikachuSprite(u8);
static void LoadTrainerPic(u16, u16);
static void ClearTrainerPic(void);
static void CreateFadeInTask(u8, u8);
static void CreateFadeOutTask(u8, u8);

extern const u8 gText_Controls[];
extern const u8 gText_ABUTTONNext[];
extern const u8 gText_ABUTTONNext_BBUTTONBack[];
extern const u8 gText_Boy[];
extern const u8 gText_Girl[];
extern const struct OamData gOamData_AffineOff_ObjBlend_32x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x32;
extern const struct OamData gOamData_AffineOff_ObjNormal_32x16;
extern const struct OamData gOamData_AffineOff_ObjNormal_16x8;

static const u16 sOakSpeech_Background_Pals[] = INCBIN_U16("graphics/oak_speech/bg_tiles.gbapal"); // Shared by the Controls Guide, Pikachu Intro and Oak Speech scenes
static const u32 sOakSpeech_Background_Tiles[] = INCBIN_U32("graphics/oak_speech/oak_speech_bg.4bpp.lz");
static const u32 sOakSpeech_Background_Tilemap[] = INCBIN_U32("graphics/oak_speech/oak_speech_bg.bin.lz");
static const u16 sOakSpeech_Red_Pal[] = INCBIN_U16("graphics/oak_speech/red/pal.gbapal");
static const u32 sOakSpeech_Red_Tiles[] = INCBIN_U32("graphics/oak_speech/red/pic.8bpp.lz");
static const u16 sOakSpeech_Oak_Pal[] = INCBIN_U16("graphics/oak_speech/oak/pal.gbapal");
static const u32 sOakSpeech_Oak_Tiles[] = INCBIN_U32("graphics/oak_speech/oak/pic.8bpp.lz");
static const u16 sOakSpeech_Platform_Pal[] = INCBIN_U16("graphics/oak_speech/platform.gbapal");
static const u32 sOakSpeech_Platform_Gfx[] = INCBIN_U32("graphics/oak_speech/platform.4bpp.lz");

static const struct BgTemplate sBgTemplates[] =
    {
        {.bg = 0,
         .charBaseIndex = 2,
         .mapBaseIndex = 31,
         .screenSize = 0,
         .paletteMode = 0,
         .priority = 0,
         .baseTile = 0},
        {.bg = 1,
         .charBaseIndex = 0,
         .mapBaseIndex = 30,
         .screenSize = 0,
         .paletteMode = 0,
         .priority = 2,
         .baseTile = 0},
        {.bg = 2,
         .charBaseIndex = 0,
         .mapBaseIndex = 28,
         .screenSize = 1,
         .paletteMode = 1,
         .priority = 1,
         .baseTile = 0}};

static const struct WindowTemplate sIntro_WindowTemplates[NUM_INTRO_WINDOWS + 1] =
    {
        [WIN_INTRO_TEXTBOX] =
            {
                .bg = 0,
                .tilemapLeft = 1,
                .tilemapTop = 4,
                .width = 28,
                .height = 15,
                .paletteNum = 15,
                .baseBlock = 1},
        DUMMY_WIN_TEMPLATE};

static const u8 sTextColor_White[] = {0, 1, 2, 0};
static const u8 sTextColor_DarkGray[] = {0, 2, 3, 0};

#define GFX_TAG_PLATFORM 0x1000
#define GFX_TAG_PIKACHU 0x1001

#define PAL_TAG_PLATFORM 0x1000
#define PAL_TAG_PIKACHU 0x1001

static const struct CompressedSpriteSheet sOakSpeech_Platform_SpriteSheet =
    {
        .data = sOakSpeech_Platform_Gfx,
        .size = 0x600,
        .tag = GFX_TAG_PLATFORM};

static const struct SpritePalette sOakSpeech_Platform_SpritePalette =
    {
        .data = sOakSpeech_Platform_Pal,
        .tag = PAL_TAG_PLATFORM};

static const union AnimCmd sOakSpeech_PlatformLeft_Anim[] =
    {
        ANIMCMD_FRAME(0, 0),
        ANIMCMD_END};

static const union AnimCmd sOakSpeech_PlatformMiddle_Anim[] =
    {
        ANIMCMD_FRAME(16, 0),
        ANIMCMD_END};

static const union AnimCmd sOakSpeech_PlatformRight_Anim[] =
    {
        ANIMCMD_FRAME(32, 0),
        ANIMCMD_END};

static const union AnimCmd *const sOakSpeech_PlatformLeft_Anims[] =
    {
        sOakSpeech_PlatformLeft_Anim};

static const union AnimCmd *const sOakSpeech_PlatformMiddle_Anims[] =
    {
        sOakSpeech_PlatformMiddle_Anim};

static const union AnimCmd *const sOakSpeech_PlatformRight_Anims[] =
    {
        sOakSpeech_PlatformRight_Anim};

enum
{
    PLAYER_PIC,
    RIVAL_PIC,
    OAK_PIC
};

static void VBlankCB_NewGameScene(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_NewGameScene(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void StartNewGameScene(void)
{
    gPlttBufferUnfaded[0] = RGB_BLACK;
    gPlttBufferFaded[0] = RGB_BLACK;
    CreateTask(Task_NewGameScene, 0);
    SetMainCallback2(CB2_NewGameScene);
}

#define tSpriteTimer data[0]
#define tTrainerPicPosX data[1]
#define tTrainerPicFadeState data[2]
#define tTimer data[3]
#define tPikachuSpriteId data[4]
#define tPokeBallSpriteId data[6]
#define tPikachuPlatformSpriteId(i) data[7 + i] // Pikachu and the platform are built of three sprites,
                                                // data[8]     // so these are used to hold their sprite IDs
                                                // data[9]     //
#define tMenuWindowId data[13]
#define tTextboxWindowId data[14]
#define tDelta data[15]

static void Task_NewGameScene(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        DmaFill16(3, 0, VRAM, VRAM_SIZE);
        DmaFill32(3, 0, OAM, OAM_SIZE);
        DmaFill16(3, 0, PLTT + sizeof(u16), PLTT_SIZE - 2);
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetTempTileDataBuffers();
        SetHelpContext(HELPCONTEXT_NEW_GAME);
        break;
    case 1:
        sOakSpeechResources = AllocZeroed(sizeof(*sOakSpeechResources));
        CreateMonSpritesGfxManager(MON_SPR_GFX_MANAGER_A, MON_SPR_GFX_MODE_NORMAL);
        break;
    case 2:
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WIN1H, 0);
        SetGpuReg(REG_OFFSET_WIN1V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        break;
    case 3:
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(1, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        SetBgTilemapBuffer(1, sOakSpeechResources->bg1TilemapBuffer);
        SetBgTilemapBuffer(2, sOakSpeechResources->bg2TilemapBuffer);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        gSpriteCoordOffsetX = 0;
        gSpriteCoordOffsetY = 0;
        break;
    case 4:
        gPaletteFade.bufferTransferDisabled = TRUE;
        InitStandardTextBoxWindows();
        InitTextBoxGfxAndPrinters();
        Menu_LoadStdPalAt(BG_PLTT_ID(13));
        LoadPalette(sOakSpeech_Background_Pals, BG_PLTT_ID(0), sizeof(sOakSpeech_Background_Pals));
        LoadPalette(GetTextWindowPalette(2) + 15, BG_PLTT_ID(0), PLTT_SIZEOF(1));
        break;
    case 5:
        sOakSpeechResources->textSpeed = GetPlayerTextSpeedDelay();
        gTextFlags.canABSpeedUpPrint = TRUE;
        break;
    case 6:
        if (FreeTempTileDataBuffersIfPossible())
            return;
        ClearDialogWindowAndFrame(WIN_INTRO_TEXTBOX, TRUE);
        FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 32, 32);
        CopyBgTilemapBufferToVram(1);
        break;
    case 7:
        HofPCTopBar_AddWindow(0, 30, 0, 13, 0x1C4);
        FillBgTilemapBufferRect_Palette0(1, 0xD00F, 0, 0, 30, 2);
        FillBgTilemapBufferRect_Palette0(1, 0xD002, 0, 2, 30, 1);
        FillBgTilemapBufferRect_Palette0(1, 0xD00E, 0, 19, 30, 1);
        gPaletteFade.bufferTransferDisabled = FALSE;
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        break;
    case 10:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(1);
        SetVBlankCallback(VBlankCB_NewGameScene);
        PlayBGM(MUS_NEW_GAME_INSTRUCT);
        gTasks[taskId].func = Task_OakSpeech_Init;
        gMain.state = 0;
        return;
    }

    gMain.state++;
}

static void Task_OakSpeech_Init(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u32 size = 0;
    if (tTimer != 0)
    {
        tTimer--;
    }
    else
    {
        sOakSpeechResources->oakSpeechBackgroundTiles = MallocAndDecompress(sOakSpeech_Background_Tiles, &size);
        LoadBgTiles(1, sOakSpeechResources->oakSpeechBackgroundTiles, size, 0);
        CopyToBgTilemapBuffer(1, sOakSpeech_Background_Tilemap, 0, 0);
        CopyBgTilemapBufferToVram(1);
        CreatePikachuSprite(taskId);
        LoadTrainerPic(OAK_PIC, 0);
        PlayBGM(MUS_ROUTE24);
        BeginNormalPaletteFade(PALETTES_ALL, 5, 16, 0, RGB_BLACK);
        tTimer = 80;
        ShowBg(2);
        gTasks[taskId].func = Task_OakSpeech_WelcomeToTheWorld;
    }
}

static inline void OakSpeechPrintMessage(const u8 *str, u8 speed, bool32 isStringVar4)
{
    DrawDialogueFrame(WIN_INTRO_TEXTBOX, FALSE);
    if (!isStringVar4)
    {
        StringExpandPlaceholders(gStringVar4, str);
        AddTextPrinterParameterized2(WIN_INTRO_TEXTBOX, FONT_MALE, gStringVar4, speed, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    }
    else
    {
        AddTextPrinterParameterized2(WIN_INTRO_TEXTBOX, FONT_MALE, str, speed, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    }
    CopyWindowToVram(WIN_INTRO_TEXTBOX, COPYWIN_FULL);
}

static void Task_OakSpeech_WelcomeToTheWorld(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        if (tTimer != 0)
        {
            tTimer--;
        }
        else
        {
            OakSpeechPrintMessage(gOakSpeech_Text_WelcomeToTheWorld, sOakSpeechResources->textSpeed, FALSE);
            gTasks[taskId].func = Task_OakSpeech_ThisWorld;
        }
    }
}

static void Task_OakSpeech_ThisWorld(u8 taskId)
{
    if (!IsTextPrinterActive(WIN_INTRO_TEXTBOX))
    {
        OakSpeechPrintMessage(gOakSpeech_Text_ThisWorld, sOakSpeechResources->textSpeed, FALSE);
        gTasks[taskId].tTimer = 30;
        gTasks[taskId].func = Task_OakSpeech_ReleasePikachuFromPokeBall;
    }
}

static void Task_OakSpeech_ReleasePikachuFromPokeBall(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 spriteId;

    if (!IsTextPrinterActive(WIN_INTRO_TEXTBOX))
    {
        if (tTimer != 0)
            tTimer--;
        spriteId = gTasks[taskId].tPikachuSpriteId;
        gSprites[spriteId].invisible = FALSE;
        gSprites[spriteId].tSpriteTimer = 0;
        CreatePokeballSpriteToReleaseMon(spriteId, gSprites[spriteId].oam.paletteNum, 100, 66, 0, 0, 32, 0xFFFF1FFF);
        gTasks[taskId].func = Task_OakSpeech_IsInhabitedFarAndWide;
        gTasks[taskId].tTimer = 0;
    }
}

static void Task_OakSpeech_IsInhabitedFarAndWide(u8 taskId)
{
    if (IsCryFinished())
    {
        if (gTasks[taskId].tTimer >= 96)
            gTasks[taskId].func = Task_OakSpeech_IStudyPokemon;
    }
    if (gTasks[taskId].tTimer < 0x4000)
    {
        gTasks[taskId].tTimer++;
        if (gTasks[taskId].tTimer == 32)
        {
            OakSpeechPrintMessage(gOakSpeech_Text_IsInhabitedFarAndWide, sOakSpeechResources->textSpeed, FALSE);
            PlayCry_Normal(INTRO_SPECIES, 0);
        }
    }
}

static void Task_OakSpeech_IStudyPokemon(u8 taskId)
{
    if (!IsTextPrinterActive(WIN_INTRO_TEXTBOX))
    {
        OakSpeechPrintMessage(gOakSpeech_Text_IStudyPokemon, sOakSpeechResources->textSpeed, FALSE);
        gTasks[taskId].func = Task_OakSpeech_ReturnPikachuToPokeBall;
    }
}

static void Task_OakSpeech_ReturnPikachuToPokeBall(u8 taskId)
{
    u8 spriteId;

    if (!IsTextPrinterActive(WIN_INTRO_TEXTBOX))
    {
        ClearDialogWindowAndFrame(WIN_INTRO_TEXTBOX, TRUE);
        spriteId = gTasks[taskId].tPikachuSpriteId;
        gTasks[taskId].tPokeBallSpriteId = CreateTradePokeballSprite(spriteId, gSprites[spriteId].oam.paletteNum, 100, 66, 0, 0, 32, 0xFFFF1F3F);
        gTasks[taskId].tTimer = 48;
        gTasks[taskId].tSpriteTimer = 64;
        gTasks[taskId].func = Task_OakSpeech_Clear;
    }
}

static void Task_OakSpeech_Clear(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (tSpriteTimer != 0)
    {
        if (tSpriteTimer < 24)
            gSprites[tPikachuSpriteId].y--;
        tSpriteTimer--;
    }
    else
    {
        if (tTimer == 48)
        {
            DestroySprite(&gSprites[tPikachuSpriteId]);
            DestroySprite(&gSprites[tPokeBallSpriteId]);
        }
        if (tTimer != 0)
        {
            tTimer--;
        }
        else
        {
            gTasks[taskId].func = Task_OakSpeech_FadeOutOak;
        }
    }
}

static void Task_OakSpeech_FadeOutOak(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    ClearDialogWindowAndFrame(WIN_INTRO_TEXTBOX, 1);
    CreateFadeInTask(taskId, 2);
    tTimer = 48;
    LoadPalette(sOakSpeech_Background_Pals, BG_PLTT_ID(0), sizeof(sOakSpeech_Background_Pals) + PLTT_SIZEOF(48));
    ClearTrainerPic();
    gTasks[taskId].func = Task_OakSpeech_ShowPlayersPic;
}

static void Task_OakSpeech_SetDefaultNames(void)
{
    const u8 *name;
    const u8 *rivalname;
    u8 i;

    name = gText_DefaultPlayerName;
    rivalname = gText_DefaultRivalName;
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
    {
        gSaveBlock2Ptr->playerName[i] = name[i];
        gSaveBlock1Ptr->rivalName[i] = rivalname[i];
    }
    gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH] = EOS;
    gSaveBlock1Ptr->rivalName[PLAYER_NAME_LENGTH] = EOS;
}

static void Task_OakSpeech_ShowPlayersPic(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (tTrainerPicFadeState != 0)
    {
        ClearTrainerPic();
        if (tTimer != 0)
        {
            tTimer--;
        }
        else
        {
            gSaveBlock2Ptr->playerGender = MALE;
            Task_OakSpeech_SetDefaultNames();
            LoadTrainerPic(PLAYER_PIC, 0);
            gTasks[taskId].tTrainerPicPosX = 0;
            gSpriteCoordOffsetX = 0;
            ChangeBgX(2, 0, BG_COORD_SET);
            CreateFadeOutTask(taskId, 2);
            gTasks[taskId].func = Task_OakSpeech_LetsGo;
        }
    }
}

static void Task_OakSpeech_LetsGo(u8 taskId)
{
    if (gTasks[taskId].tTrainerPicFadeState != 0)
    {
        StringExpandPlaceholders(gStringVar4, gOakSpeech_Text_LetsGo);
        OakSpeechPrintMessage(gStringVar4, sOakSpeechResources->textSpeed, TRUE);
        gTasks[taskId].tTimer = 30;
        gTasks[taskId].func = Task_OakSpeech_FadeOutBGM;
    }
}

static void Task_OakSpeech_FadeOutBGM(u8 taskId)
{
    if (!IsTextPrinterActive(WIN_INTRO_TEXTBOX))
    {
        if (gTasks[taskId].tTimer != 0)
        {
            gTasks[taskId].tTimer--;
        }
        else
        {
            FadeOutBGM(4);
            gTasks[taskId].func = Task_OakSpeech_SetUpExitAnimation;
        }
    }
}

// The sOakSpeechResources struct's currentPage field is reused below as a timer for shrinking the player pic
#define shrinkTimer currentPage

static void Task_OakSpeech_SetUpExitAnimation(u8 taskId)
{
    sOakSpeechResources->shrinkTimer = 0;
    Task_OakSpeech_SetUpDestroyPlatformSprites(taskId);
    Task_OakSpeech_SetUpFadePlayerPicWhite(taskId);
    Task_OakSpeech_SetUpShrinkPlayerPic(taskId);
}

#define tPlayerPicFadeOutTimer data[0]
#define tScaleDelta data[2]
#define tPlayerIsShrunk data[15]

static void Task_OakSpeech_SetUpShrinkPlayerPic(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    SetBgAttribute(2, BG_ATTR_WRAPAROUND, 1);
    tPlayerPicFadeOutTimer = 0;
    data[1] = 0; // assigned, but never read
    tScaleDelta = 256;
    tPlayerIsShrunk = FALSE;
    gTasks[taskId].func = Task_OakSpeech_ShrinkPlayerPic;
}

static void Task_OakSpeech_ShrinkPlayerPic(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s16 x, y;
    u16 oldScaleDelta;

    sOakSpeechResources->shrinkTimer++;
    if (sOakSpeechResources->shrinkTimer % 20 == 0)
    {
        if (sOakSpeechResources->shrinkTimer == 40)
            PlaySE(SE_WARP_IN);
        oldScaleDelta = tScaleDelta;
        tScaleDelta -= 32;
        x = Q_8_8_inv(oldScaleDelta - 8);
        y = Q_8_8_inv(tScaleDelta - 16);
        SetBgAffine(2, 0x7800, 0x5400, 120, 84, x, y, 0);
        if (tScaleDelta <= 96)
        {
            tPlayerIsShrunk = TRUE;
            tPlayerPicFadeOutTimer = 36;
            gTasks[taskId].func = Task_OakSpeech_FadePlayerPicToBlack;
        }
    }
}

#define tBGFadeStarted data[1]

static void Task_OakSpeech_SetUpDestroyPlatformSprites(u8 taskId)
{
    u8 taskId2 = CreateTask(Task_OakSpeech_DestroyPlatformSprites, 1);
    s16 *data = gTasks[taskId2].data;
    data[0] = 0; // assigned, but never read
    tBGFadeStarted = 0;
    data[2] = 0;  // assigned, but never read
    data[15] = 0; // assigned, but never read
    BeginNormalPaletteFade(PALETTES_OBJECTS | 0x0FCF, 4, 0, 16, RGB_BLACK);
}

static void Task_OakSpeech_DestroyPlatformSprites(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        if (tBGFadeStarted != 0)
        {
            DestroyTask(taskId);
        }
        else
        {
            tBGFadeStarted++;
            BeginNormalPaletteFade(0x0000 | 0xF000, 0, 0, 16, RGB_BLACK);
        }
    }
}

#undef tBGFadeStarted

#define tPlayerPicFadeWhiteTimer data[0]
#define tUnderflowingTimer data[1]
#define tSecondaryTimer data[2]
#define tBlendCoefficient data[14]

static void Task_OakSpeech_SetUpFadePlayerPicWhite(u8 taskId)
{
    u8 taskId2 = CreateTask(Task_OakSpeech_FadePlayerPicWhite, 2);
    s16 *data = gTasks[taskId2].data;
    tPlayerPicFadeWhiteTimer = 8;
    tUnderflowingTimer = 0;
    tSecondaryTimer = 8;
    tBlendCoefficient = 0;
    data[15] = 0; // assigned, but never read
}

static void Task_OakSpeech_FadePlayerPicWhite(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 i;

    if (tPlayerPicFadeWhiteTimer != 0)
    {
        tPlayerPicFadeWhiteTimer--;
    }
    else
    {
        if (tUnderflowingTimer <= 0 && tSecondaryTimer != 0)
            tSecondaryTimer--;
        BlendPalette(BG_PLTT_ID(4), 0x20, tBlendCoefficient, RGB_WHITE);
        tBlendCoefficient++;
        tUnderflowingTimer--;
        tPlayerPicFadeWhiteTimer = tSecondaryTimer;
        if (tBlendCoefficient > 14)
        {
            for (i = 0; i < 32; i++)
            {
                gPlttBufferFaded[i + BG_PLTT_ID(4)] = RGB_WHITE;
                gPlttBufferUnfaded[i + BG_PLTT_ID(4)] = RGB_WHITE;
            }
            DestroyTask(taskId);
        }
    }
}

static void Task_OakSpeech_FadePlayerPicToBlack(u8 taskId)
{
    if (gTasks[taskId].tPlayerPicFadeOutTimer != 0)
    {
        gTasks[taskId].tPlayerPicFadeOutTimer--;
    }
    else
    {
        BeginNormalPaletteFade(0x0000 | 0x0030, 2, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_OakSpeech_WaitForFade;
    }
}

static void Task_OakSpeech_WaitForFade(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OakSpeech_FreeResources;
}

static void Task_OakSpeech_FreeResources(u8 taskId)
{
    FreeAllWindowBuffers();
    DestroyMonSpritesGfxManager(MON_SPR_GFX_MANAGER_A);
    Free(sOakSpeechResources);
    sOakSpeechResources = NULL;
    gTextFlags.canABSpeedUpPrint = FALSE;
    SetMainCallback2(CB2_NewGame);
    DestroyTask(taskId);
}

static void CreatePikachuSprite(u8 taskId)
{
    u8 spriteId;

    LoadSpecialPokePic(MonSpritesGfxManager_GetSpritePtr(MON_SPR_GFX_MANAGER_A, 0), INTRO_SPECIES, 0, TRUE);
    LoadCompressedSpritePaletteUsingHeapWithTag(GetMonSpritePalFromSpeciesAndPersonality(INTRO_SPECIES, 0, 0), INTRO_SPECIES);
    SetMultiuseSpriteTemplateToPokemon(INTRO_SPECIES, 0);
    spriteId = CreateSprite(&gMultiuseSpriteTemplate, 96, 96, 1);
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 1;
    gSprites[spriteId].invisible = TRUE;
    gTasks[taskId].tPikachuSpriteId = spriteId;
}

#define sBodySpriteId data[0]

static void LoadTrainerPic(u16 whichPic, u16 tileOffset)
{
    u32 i;

    switch (whichPic)
    {
    case PLAYER_PIC:
        LoadPalette(sOakSpeech_Red_Pal, BG_PLTT_ID(4), sizeof(sOakSpeech_Red_Pal));
        LZ77UnCompVram(sOakSpeech_Red_Tiles, (void *)VRAM + 0x600 + tileOffset);
        break;
    case OAK_PIC:
        LoadPalette(sOakSpeech_Oak_Pal, BG_PLTT_ID(6), sizeof(sOakSpeech_Oak_Pal));
        LZ77UnCompVram(sOakSpeech_Oak_Tiles, (void *)VRAM + 0x600 + tileOffset);
        break;
    default:
        return;
    }

    sOakSpeechResources->trainerPicTilemap = AllocZeroed(0x60);
    for (i = 0; i < 0x60; i++)
        ((u8 *)sOakSpeechResources->trainerPicTilemap)[i] = i;
    FillBgTilemapBufferRect(2, 0, 0, 0, 32, 32, 16);
    CopyRectToBgTilemapBufferRect(2, sOakSpeechResources->trainerPicTilemap, 0, 0, 8, 12, 11, 2, 8, 12, 16, (tileOffset / 64) + 24, 0);
    CopyBgTilemapBufferToVram(2);
    Free(sOakSpeechResources->trainerPicTilemap);
    sOakSpeechResources->trainerPicTilemap = 0;
}

static void ClearTrainerPic(void)
{
    FillBgTilemapBufferRect(2, 0, 11, 1, 8, 12, 16);
    CopyBgTilemapBufferToVram(2);
}

#define tParentTaskId data[0]
#define tBlendTarget1 data[1]
#define tBlendTarget2 data[2]
#define tUnusedState data[3]
#define tFadeTimer data[4]

static void Task_SlowFadeIn(u8 taskId)
{
    if (gTasks[taskId].tBlendTarget1 == 0)
    {
        gTasks[gTasks[taskId].tParentTaskId].tTrainerPicFadeState = 1;
        DestroyTask(taskId);
    }
    else
    {
        if (gTasks[taskId].tFadeTimer != 0)
        {
            gTasks[taskId].tFadeTimer--;
        }
        else
        {
            gTasks[taskId].tFadeTimer = gTasks[taskId].tTimer;
            gTasks[taskId].tBlendTarget1--;
            gTasks[taskId].tBlendTarget2++;
            SetGpuReg(REG_OFFSET_BLDALPHA, (gTasks[taskId].tBlendTarget2 * 256) + gTasks[taskId].tBlendTarget1);
        }
    }
}

static void CreateFadeInTask(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_OBJ);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tTrainerPicFadeState = 0;
    taskId2 = CreateTask(Task_SlowFadeIn, 0);
    gTasks[taskId2].tParentTaskId = taskId;
    gTasks[taskId2].tBlendTarget1 = 16;
    gTasks[taskId2].tBlendTarget2 = 0;
    gTasks[taskId2].tUnusedState = delay; // assigned, but never read
    gTasks[taskId2].tFadeTimer = delay;
}

static void Task_SlowFadeOut(u8 taskId)
{
    if (gTasks[taskId].tBlendTarget1 == 16)
    {
        if (!gPaletteFade.active)
        {
            gTasks[gTasks[taskId].tParentTaskId].tTrainerPicFadeState = 1;
            DestroyTask(taskId);
        }
    }
    else
    {
        if (gTasks[taskId].tFadeTimer != 0)
        {
            gTasks[taskId].tFadeTimer--;
        }
        else
        {
            gTasks[taskId].tFadeTimer = gTasks[taskId].tTimer;
            gTasks[taskId].tBlendTarget1 += 2;
            gTasks[taskId].tBlendTarget2 -= 2;

            SetGpuReg(REG_OFFSET_BLDALPHA, (gTasks[taskId].tBlendTarget2 * 256) + gTasks[taskId].tBlendTarget1);
        }
    }
}

static void CreateFadeOutTask(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_OBJ);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tTrainerPicFadeState = 0;

    taskId2 = CreateTask(Task_SlowFadeOut, 0);
    gTasks[taskId2].tParentTaskId = taskId;
    gTasks[taskId2].tBlendTarget1 = 0;
    gTasks[taskId2].tBlendTarget2 = 16;
    gTasks[taskId2].tUnusedState = delay; // assigned, but never read
    gTasks[taskId2].tFadeTimer = delay;
}

#undef tSpriteTimer
#undef tTrainerPicPosX
#undef tTrainerPicFadeState
#undef tTimer
#undef tPikachuSpriteId
#undef tPokeBallSpriteId
#undef tMenuWindowId
#undef tTextboxWindowId
#undef tDelta
#undef tPlayerPicFadeOutTimer
#undef tScaleDelta
#undef tPlayerIsShrunk
#undef shrinkTimer
#undef tPlayerPicFadeWhiteTimer
#undef tUnderflowingTimer
#undef tSecondaryTimer
#undef tBlendCoefficient
#undef tNameNotConfirmed
#undef sBodySpriteId
#undef tParentTaskId
#undef tBlendTarget1
#undef tBlendTarget2
#undef tUnusedState
#undef tFadeTimer
